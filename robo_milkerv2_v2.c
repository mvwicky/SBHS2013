//1 degree ~ 3 ticks
#define ARM_UP // arm is straight up
#define ARM_DOWN // arm is getting a pom
#define ARM_OUT // arm is straight out 
#define ARM_SCAN // arm islooking at the ground 
#define ARM_DUMP // arm is dumping the poms in the basket

#define B_DUMP // basket is dumping poms out
#define B_UP // basket is collecting poms

#define P_UP // arm is pushing pom in
#define P_DOWN // arm is retracted

#define TOL
#define THRESH 450

#define DIFF 100
#define MID 1000 
#define HIGH MID + DIFF
#define LOW MID - DIFF

#define CM_SPEED  300

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
	}left_s , middle_s , right_s , ET;
}lego;

struct blob { // this struct contains the x , y , and size values for each color for the current and target frame
	struct color {
		int x;
		int y;
		int size;
	}green , orange , pink , teal;
}current , target;

int update_blob(); // updates the screen
int update_link(); // updates some values related to the link

void turn_right(int speed); // turn right at speed , - speed
void turn_left(int speed); // turn left at -speed , speed
void drive_straight(int speed); // drive forward at speed , speed
void drive_back(int speed); // drive backwards at -speed , -speed
int nv_servo(int s , int fpos , int step = 8);
int af();

int camera_move_x(); // align the camera in the x direction
int camera_move_y(); // align the camera in the y direction
int left_on(); // return true if on line and false if not (for all these)
int right_on(); 
int middle_on();

int get_ET();

int line_follow(); // follow the black line
int line_follow_slow(); // follow the black line quickly

int get_pom(); // get the pom (align), then pickup
int pom_push(); // push the pom into the basket 
int move_back(); // move back to the line after aligning

int avoid_cubeguy(); // avoid the cube or botguy
int avoid_booster(); // avoid the booster

int gc = 0; // green channel
int oc = 1; // orange channel
int pc = 2; // pink channel
int tc = 3; // teal channel

int arm_servo = 1; // servo for the picker upper arm
int push_servo = 0; // servo for the pusher arm
int basket_servo = 2; // servo for the basket
int left_s = 2; // port for the left sensor
int right_s = 0; // port for the right sensor
int middle_s = 1; // port for the middle sensor
int ET_s = 3; // port for the ET sensor

int main()
{
	camera_open(LOW_RES);
	camera_update();
	lego.thresh = 450;
	lego.left.port = 0;
	lego.right.port = 2;
	lego.x_pos.port = 1;
	lego.arm.port = 1;
	lego.push.port = 0;
	lego.basket.port = 2;
	lego.left_s.port = 2;
	lego.right_s.port = 0;
	lego.middle_s.port = 1;
	lego.ET.port = 3;
	set_analog_pullup(lego.ET.port , 0);
	set_analog_pullup(lego.left_s.port , 1);
	set_analog_pullup(lego.middle_s.port , 1);
	set_analog_pullup(lego.right_s.port , 1);
	enable_servo(lego.arm.port);
	enable_servo(lego.push.port);
	enable_servo(lego.basket.port);
	nv_servo(lego.arm.port , ARM_SCAN);
	nv_servo(lego.basket.port , B_UP);
	nv_servo(lego.push.port , P_DOWN);


}

int update_blob()
{
	camera_update();
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
	lego.left_s.val = analog10(lego.left_s.port);
	lego.middle_s.val = analog10(lego.middle_s.port);
	lego.right_s.val = analog10(lego.right_s.port);
	lego.ET.val = analog10(lego.ET.port);
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

int nv_servo(int s , int fpos , int step = 8)
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
	if (cpos < fpos)
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
	if (cpos == fpos)
		return 0;	
}

int af()
{
	freeze(0);
	freeze(1);
	freeze(2);
	freeze(3);
}

int camera_move_x()
{
	update_blob();
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
	if (lego.left_s.val > THRESH)
		return true;
	if (lego.left_s.val <= THRESH)
		return false;
}

int middle_on()
{
	update_link();
	if (lego.middle_s.val > THRESH)
		return true;
	if (lego.middle_s.val <= THRESH)
		return false;
}

int right_on()
{
	update_link();
	if (lego.right_s.val > THRESH)
		return true;
	if (lego.right_s.val <= THRESH)
		return false;
}

int line_follow()
{
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
	int low = (int)(LOW / 2);
	int high = (int)(HIGH / 2);
	if (left_on() == false && middle_on() == false && right_on() == false // 0 , 0 , 0 // spin in place
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
	y_count = 0;
	update_blob();
	unsigned int x = camera_move_x();
	unsigned int y = camera_move_y();
	while (1)
	{
		x = camera_move_x();
		y = camera_move_y();
		if (x == 0 && y == 0)
		{
			af();
			printf("IN POS\n");
			nv_servo(lego.arm.port , ARM_DOWN);
			nv_servo(lego.arm.port , ARM_OUT);
			nv_servo(lego.arm.port , ARM_DOWN);
			nv_servo(lego.arm.port , ARM_DUMP);
			msleep(500);
			break;
		}
	}
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

/*

*/