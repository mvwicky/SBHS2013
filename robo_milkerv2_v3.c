//1 degree ~ 3 ticks
/* 
NOTES
--for now, the plan for the getting around the cube and botguy are the same.

--also for now, the plan for getting around the booster is to drive on the line
until a pink or teal blob is seen. Then, it'll get around it somehow. Possibly
it will keep the blob on one side of the bot. This could also be a possible way
to speed up getting around the cube. 
	ADDENDUM:
	The plan now is to only get one side of poms. 
	avoid_booster will now turn around from the booster.	
	ADDENDUM:
	All the above is now irrevelent.
*/
#define ARM_UP 570 // arm is straight up
#define ARM_DOWN 1600 // arm is getting a pom
#define ARM_OUT 1568 // arm is straight out 
#define ARM_SCAN 1095 // arm islooking at the ground 
#define ARM_DUMP 365 // arm is dumping the poms in the basket

#define B_DUMP 1 // basket is dumping poms out
#define B_UP 1 // basket is collecting poms

#define P_UP 0 // arm is pushing pom in
#define P_DOWN 2047 // arm is retracted

#define TOL 2

#define DIFF 100
#define MID 1000 
#define HIGH MID + DIFF
#define LOW MID - DIFF

#define true 1
#define false 0

#define DEG 3

int xl_count = 0; // amount of times that the bot has moved left (camera move)
int xr_count = 0; // amount of times that the bot has moved right (camera move)
int y_count = 0; // amount of times that the bot has moved forward (camera move)

struct link { // this struct contains vital information for the link
	int thresh;
	struct motor {
		int port;
		int pos;
	}left , right , x_pos;
	struct servo {
		int port;
		int pos;
	}arm , push , basket;
	struct sensor {
		int port;
		int val;
	}th_left , th_middle , th_right , ET;
}lego;

struct blob { // this struct contains the x , y , and size values for each color for the current and target frame
	struct color {
		int x;
		int y;
		int size;
	}green , orange;
}current , target;

int update_blob(); // updates the screen
int update_link(); // updates some values related to the link
int update_bools(int c); /* Evaluates several different cases based on the camera. 
							This name is kind of a misnomer. 
							It has a switch that has the following cases:
							1: Orange size and Green size above target size
							2: Orange size above target size
							3: Green size above target size
						 */

void turn_right(int speed); // turn right at speed , - speed
void turn_left(int speed); // turn left at -speed , speed
void drive_straight(int speed); // drive forward at speed , speed
void drive_back(int speed); // drive backwards at -speed , -speed
int nv_servo(int s , int fpos , int step); // move the servo at a less violent speed
int af(); // freeze all motors

int camera_move_x(); // align the camera in the x direction
int camera_move_y(); // align the camera in the y direction

int left_on(); // return true if on line and false if not (for all these)
int right_on(); 
int middle_on();

int line_follow(); // follow the black line
int line_follow_slow(); // follow the black line quickly

int get_pom(); // get the pom (align), then pickup
int pom_push(); // push the pom into the basket 
int move_back(); // move back to the line after aligning

int avoid_cubeguy(); // avoid the cube or botguy

int average(int port , int samples); 
int delay(float t);

int gc = 0; // green channel
int oc = 1; // orange channel

int main()
{
	extra_buttons_show(1); // show the x , y , and z buttons
	set_a_button_text("Coords"); 
	set_b_button_text("Green Size");
	set_c_button_text("Orange Size");
	set_z_button_text("Go");
	camera_open(LOW_RES); // open the camera in the lowest resolution (preferable)
	camera_update(); // get a new frame from the camera
	lego.thresh = 450; // set the threshold (black - white) (for line following)
	lego.left.port = 3; // left drive motor port
	lego.right.port = 0; // right drive motor port
	lego.x_pos.port = 1; // arm x coordinate adjuster port
	lego.arm.port = 1; // pom picker upper arm servo port
	lego.push.port = 0; // pom pusher servo port
	lego.basket.port = 2; // pom basket servo port 
	lego.th_left.port = 2; // left tophat sensor port
	lego.th_right.port = 0; // right tophat sensor port
	lego.th_middle.port = 1; // middle tophar sensor port
	lego.ET.port = 3; // ET sensor port
	set_analog_pullup(lego.ET.port , 0); // change the mode of the ET sensor port to return the correct values
	set_analog_pullup(lego.th_left.port , 1); // change the mode of the tophat sensors to return the correct value
	set_analog_pullup(lego.th_middle.port , 1);
	set_analog_pullup(lego.th_right.port , 1);
	enable_servo(lego.arm.port); 
	enable_servo(lego.push.port);
	enable_servo(lego.basket.port);
	nv_servo(lego.arm.port , ARM_SCAN , 8); // move arm to scanning mode (picking up a pom)
	//nv_servo(lego.basket.port , B_UP , 8); // move basket up (collecting poms)
	set_servo_position(lego.push.port , P_DOWN); // move pusher down (not pushing a pom)
	msleep(500);
	while (a_button() == 0) // set the coordinates where the arm should come down
	{
		camera_update();
		target.green.x = get_object_center(gc , 0).x;
		target.green.y = get_object_center(gc , 0).y;
		printf("(%d , %d)\n" , target.green.x , target.green.y);
	}
	/*
		NOTE: Try to keep blob sizes as small as possible.
	*/
	while (b_button() == 0) // set the size of the green blob (pom) required for detection 
	{
		camera_update();
		target.green.size = get_object_area(gc , 0);
		printf("GREEN SIZE = %d\n" , target.green.size);
	}
	while (c_button() == 0) // set the size of the orange blob (pile of poms) required for detection 
	{
		camera_update();
		target.orange.size = get_object_area(oc , 0);
		printf("ORANGE SIZE = %d\n" , target.orange.size);
	}
	int cyc = 0; // variable used to change the behavior of some loops
	msleep(500);
	while (z_button() == 0) // wait and update until the z button is pressed
	{
		update_blob();
		update_link();
	}
	shut_down_in(118);
	/* The next few lines of code are being written before the bot has been finished.
	   The assumed starting point is "above" the black line. 
	   This way the bot can drive to the black line and do its default behavior: turn right.
	   For the most part, it will be commented per while loop. */

	while (1) // drive straight until the line is detected
	{
		drive_straight(500);
		if (left_on() == true || middle_on() == true || right_on() == true)
			break;
	}
	while (1) // follow the line until orange and green poms are found. Then pickup.
	{
		update_blob();
		line_follow();
		if (cyc % 50 == 0)
		{
			move_left(300);
			msleep(10);
		}
		if (update_bools(1) == true)
		{
			get_pom(); // get 1st green pom
			pom_push();
			break;
		}
	}
	move_back(); // move back the the line
	while (1)
	{
		update_blob();
		turn_right(300);
		if (update_bools(1) == true)
		{
			get_pom(); // get 2nd green pom
			pom_push();
		}
	}
	move_back(); // move back to the line
	while (1)
	{
		turn_left(300);
		msleep(10);
		if (middle_on() == true)
			break;
	}
	avoid_cubeguy();


}

int update_blob()
{
	camera_update(); // grab a new frame
	current.green.x = get_object_center(gc , 0).x;  
	current.green.y = get_object_center(gc , 0).y;
	current.green.size = get_object_area(gc , 0);
	current.orange.x = get_object_center(oc , 0).x;
	current.orange.y = get_object_center(oc , 0).y;
	current.orange.size = get_object_area(oc , 0);	
	current.pink.x  = get_object_center(pc , 0).x;
	current.pink.y = get_object_center(pc , 0).y;
	current.pink.size = get_object_area(pc , 0);
	current.teal.x = get_object_center(tc , 0).x;
	current.teal.y = get_object_center(tc , 0).y;
	current.teal.size = get_object_area(tc , 0);
	return 0;
}

int update_link()
{
	lego.left.pos = get_motor_position_counter(lego.left.port);
	lego.right.pos = get_motor_position_counter(lego.right.port);
	lego.x_pos.pos = get_motor_position_counter(lego.x_pos.port);
	lego.arm.pos = get_servo_position(lego.arm.port);
	lego.push.pos = get_servo_position(lego.push.port);
	lego.basket.pos = get_servo_position(lego.basket.port);
	lego.th_left.val = analog10(lego.th_left.port);
	lego.th_middle.val = analog10(lego.th_middle.port);
	lego.th_right.val = analog10(lego.th_right.port);
	lego.ET.val = analog10(lego.ET.port);
	return 0;
}

int update_bools(int c) 
{
	switch (c) {
		case 1:
			if (current.orange.size >= target.orange.size && current.green.size >= target.green.size)
				return true;
			else 
				return false;	
		case 2: 
			if (current.orange.size >= target.orange.size)
				return true;
			else
				return false;
		case 3:
			if (current.green.size >= target.green.size)
				return true;
			else 
				return false;
		case 4:
			if (current.pink.size >= target.pink.size)
				return true;
			else 
				return false;
		case 5:
			if (current.teal.size >= target.teal.size)
				return true;
			else
				return false;

	}
	return 0;
}

void turn_right(int speed)
{
	mav(lego.left.port , speed);
	mav(lego.right.port , -speed);
}

void turn_left(int speed)
{
	mav(lego.left.port , -speed);
	mav(lego.right.port , speed);
}

void drive_straight(int speed)
{
	mav(lego.left.port , speed);
	mav(lego.right.port , speed);
}

void drive_back(int speed)
{
	mav(lego.left.port , -speed);
	mav(lego.right.port , -speed);
}

int nv_servo(int s , int fpos , int step)
{
	int cpos = get_servo_position(s);
	if (cpos > fpos)
	{
		while (cpos > fpos)
		{
			cpos -= step;
			set_servo_position(s , cpos);
			cpos = get_servo_position(s);
			msleep(10);
		}
		return 0;
	}
	else if (cpos < fpos)
	{
		while (cpos < fpos)
		{
			cpos += step;
			set_servo_position(s , cpos);
			cpos = get_servo_position(s);
			msleep(10);
		}
		return 0;		
	}
	else if (cpos == fpos)
		return 0;	
	return 0;
}

int af()
{
	freeze(0);
	freeze(1);
	freeze(2);
	freeze(3);
	return 0;
}

int camera_move_x()
{
	update_blob();
	update_link();
	if ((current.green.x > (target.green.x + TOL))) // target is to the left
	{
		
		return 1;
	}
	else if ((current.green.x < (target.green.x - TOL))) // target is to the right
	{
		return 1;
	}
	else if ((current.green.x >= (target.green.x - TOL)) && (current.green.x <= (target.green.x + TOL))) // target is in range
	{
		af();
		return 0;
	}
	return 1;
}

int camera_move_y() 
{
	int speed = 200;
	update_blob();
	update_link();
	if ((current.green.y > (target.green.y + TOL))) // target has too great a y value
	{
		printf("MOVING BACKWARDS\n");
		mrp(lego.left.port , -speed , DEG);
		mrp(lego.right.port , -speed , DEG);
		bmd(lego.left.port);
		bmd(lego.right.port);
		return 1;
	}
	else if ((current.green.y < (target.green.y - TOL))) // target is too low a y value
	{
		printf("MOVING FORWARDS\n");
		mrp(lego.left.port , speed , DEG);
		mrp(lego.right.port , speed , DEG);
		bmd(lego.left.port);
		bmd(lego.right.port);
		y_count += 1;
		return 1;
	}
	else if ((current.green.y >= (target.green.y - TOL)) && (current.green.y <= (target.green.y + TOL))) // target is in range
	{
		af();
		return 0;
	}
	return 1;
}

int left_on()
{
	update_link();
	if (lego.th_left.val > lego.thresh)
		return true;
	if (lego.th_left.val <= lego.thresh)
		return false;
	return 1;
}

int middle_on()
{
	update_link();
	if (lego.th_middle.val > lego.thresh)
		return true;
	if (lego.th_middle.val <= lego.thresh)
		return false;
	return 1;
}

int right_on()
{
	update_link();
	if (lego.th_right.val > lego.thresh)
		return true;
	if (lego.th_right.val <= lego.thresh)
		return false;
	return 1;
}

int line_follow()
{
	update_link();
	printf("%d , %d , %d\n" , left_on() , middle_on() , right_on());
	if (left_on() == false && middle_on() == false && right_on() == false) // 0 , 0 , 0 // spin in place
	{
		mav(lego.left.port , LOW);
		mav(lego.right.port , -LOW);
		msleep(10);
		return 0;
	}
	if (left_on() == true && middle_on() == false && right_on() == false) // 1 , 0 , 0 // 
	{
		mav(lego.left.port , LOW);
		mav(lego.right.port , HIGH);
		msleep(10);
		return 0;
	}
	if (left_on() == false && middle_on() == true && right_on() == false) // 0 , 1 , 0
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , HIGH);
		msleep(10);
		return 0;
	}
	if (left_on() == false && middle_on() == false && right_on() == true) // 0 , 0 , 1
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}  
	if (left_on() == true && middle_on() == true && right_on() == false) // 1 , 1 , 0
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}
	if (left_on() == false && middle_on() == true && right_on() == true) // 0 , 1 , 1
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}
	if (left_on() == true && middle_on() == true && right_on() == true) // 1 , 1 , 1
	{
		mav(lego.left.port , LOW);
		mav(lego.right.port , -LOW);
		msleep(10);
		return 0;
	}
	return 0;	
}

int line_follow_slow()
{
	update_link();
	printf("%d , %d , %d\n" , left_on() , middle_on() , right_on());
	int low = (int)(LOW / 2);
	int high = (int)(HIGH / 2);
	if (left_on() == false && middle_on() == false && right_on() == false )// 0 , 0 , 0 // spin in place
	{
		mav(lego.left.port , low);
		mav(lego.right.port , -low);
		msleep(10);
		return 0;
	}
	if (left_on() == true && middle_on() == false && right_on() == false) // 1 , 0 , 0 // 
	{
		mav(lego.left.port , low);
		mav(lego.right.port , high);
		msleep(10);
		return 0;
	}
	if (left_on() == false && middle_on() == true && right_on() == false) // 0 , 1 , 0
	{
		mav(lego.left.port , high);
		mav(lego.right.port , high);
		msleep(10);
		return 0;
	}
	if (left_on() == false && middle_on() == false && right_on() == true) // 0 , 0 , 1
	{
		mav(lego.left.port , high);
		mav(lego.right.port , low);
		msleep(10);
		return 0;
	}  
	if (left_on() == true && middle_on() == true && right_on() == false) // 1 , 1 , 0
	{
		mav(lego.left.port , high);
		mav(lego.right.port , low);
		msleep(10);
		return 0;
	}
	if (left_on() == false && middle_on() == true && right_on() == true) // 0 , 1 , 1
	{
		mav(lego.left.port , high);
		mav(lego.right.port , low);
		msleep(10);
		return 0;
	}
	if (left_on() == true && middle_on() == true && right_on() == true) // 1 , 1 , 1
	{
		mav(lego.left.port , low);
		mav(lego.right.port , -low);
		msleep(10);
		return 0;
	}
	return 0;	
}

int get_pom()
{
	update_link();
	y_count = 0;
	update_blob();
	unsigned int x = camera_move_x();
	unsigned int y = camera_move_y();
	while (1)
	{
		update_link();
		x = camera_move_x();
		y = camera_move_y();
		if (x == 0 && y == 0)
		{
			af();
			printf("IN POS\n");
			nv_servo(lego.arm.port , ARM_DOWN , 8);
			nv_servo(lego.arm.port , ARM_OUT , 8);
			nv_servo(lego.arm.port , ARM_DOWN , 8);
			nv_servo(lego.arm.port , ARM_DUMP , 8);
			msleep(500);
			update_link();
			break;
		}
	}
	update_link();
	return 0;
}

int pom_push()
{
	msleep(100);
	nv_servo(lego.arm.port , ARM_DUMP , 8);
	msleep(500);
	set_servo_position(lego.push.port , P_UP);
	msleep(500);
	set_servo_position(lego.push.port , P_DOWN);
	msleep(500);
	set_servo_position(lego.push.port , P_UP);
	msleep(500);
	set_servo_position(lego.push.port , P_DOWN);
	msleep(500);
	set_servo_position(lego.push.port , P_UP);
	msleep(500);
	set_servo_position(lego.push.port , P_DOWN);
	msleep(500);
	ao();
	update_link();
	return 0;
}

int move_back()
{
	int y_back = y_count * DEG;
	mrp(lego.left.port , -200 , y_back);
	mrp(lego.right.port , -200 , y_back);
	bmd(lego.left.port);
	bmd(lego.right.port);
	return 0;
}

int avoid_cubeguy()
{
	update_link();
	nv_servo(lego.arm.port , ARM_UP , 8);
	while (1)
	{
		update_link();
		turn_left(500);
		msleep(10);
		if (middle_on() == true)
		{
			af();
			break;
		}
	}
	while (1)
	{
		update_link();
		line_follow_slow();
		if (lego.ET.val < 600)
		{
			af();
			break;
		}
	}
	turn_right(300);
	msleep(1250);
	af();
	nv_servo(lego.arm.port , ARM_OUT , 8);
	while (1)
	{
		turn_left(300);
		msleep(10);
		if (right_on() == true)
		{
			af();
			break;
		}
	}
	nv_servo(lego.arm.port , ARM_UP , 8);
	while (1)
	{
		turn_right(300);
		msleep(10);
		if (middle_on() == true)
		{
			af();
			break;
		}
	}
	return 0;
}

int average(int port , int samples)
// function that returns the average of an amount of sensor readings
// args = port of sensor to be read , amount of samples
{ 
	int nums[samples]; // create array for the readings
	int sum = 0; // create variable for the sum
	float average; // create variable for the average
	int n;
	for (n = 0; n < samples; n += 1) 
	// for n in samples, assign the current reading of the sensor to the current index of nums and add it to sums
	{
		nums[n] = analog10(port);
		sum += nums[n];
	}
	average = ((float)sum / (float)samples); // create the average
	return ((int)average); // return the average as an integer
}

int delay(float t)
{
	float start_time = seconds();
	while (seconds() < start_time + t);
	return 0;
}

/*

*/
