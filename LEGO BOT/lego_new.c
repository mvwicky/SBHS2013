// FIND A WAY TO GET AROUND THE BOOSTERS
// SET THE ET DISTANCE TO A CORRECT VALUE
// FIND THE CORRECT SIZE FOR STOL

#define ARM_DUMP 200 // up for the pom getter arm
#define ARM_UP 635 // arm is straight up
#define ARM_DOWN 1850 // down for the pom getter arm
#define ARM_OUT 1590 // out for the arm, push the things away

#define B_UP 1600 // up for the basket
#define B_DOWN 750 // down for the basket (dumping)

#define P_UP 2047 // push pom
#define P_DOWN 0 // arm retracted

#define TOL 2
#define STOL 15
#define THRESH 400
#define ET_DIST 350 // subject to change

#define DIFF 150
#define MID 800
#define HIGH MID + DIFF
#define LOW MID - DIFF

int left_s = 1; // port for the left top hat sensor
int right_s = 7; // port for the right top hat sensor
int middle_s = 4; // port for the middle top hat sensor
int ET_s = 0;

struct robot {
	struct motor {
		int port;
	}left , right;
}lego;

struct blob {
	struct color {
		int x;
		int y;
		int size;
	}green , orange;
}current , target;

void update_blob();

void turn_right(int speed);
void turn_left(int speed);

int camera_move_x();
int camera_move_y();
int alt_camera_move_y();

int get_left();
int get_middle();
int get_right();
int get_ET();
int get_touch();

int t_line_follow();

int get_pom();

int get_pom();
int pom_push();

int avoid_cubeguy();
int avoid_booster();

int arm_servo = 1; // servo of the pom getter arm
int push_servo = 0; // servo of the pusher arm
int basket_servo = 2; // servo of the basket


int main()
{
	extra_buttons_show(1);
	set_a_button_text("COORDS");
	set_b_button_text("GREEN SIZE");
	set_c_button_text("ORANGE SIZE");
	camera_open(LOW_RES);
	camera_update();
	set_analog_pullup(ET_s , 0);
	set_analog_pullup(left_s , 1);
	set_analog_pullup(middle_s , 1);
	set_analog_pullup(right_s , 1);
	while (a_button() == 0) // set target green coordinates 
	{
		camera_update();
		target.green.x = get_object_center(0 , 0).x;
		target.green.y = get_object_center(0 , 0).y;
		
	}
	while (b_button() == 0) // set target green size
	{
		camera_update();
		target.green.size = get_object_area(0 , 0);
	}
	while (c_button() == 0) // set target orange size
	{
		camera_update();
		target.orange.size = get_object_area(0 , 0);
	}
	while (side_button() == 0) // wait
	{
		update_blob();
	}
	while (1) // follow the line until a blob of orange and green is seen
	{
		update_blob();
		t_line_follow();
		if (current.green.size >= target.green.size && current.orange.size >= target.green.size)
			break;
	}
	get_pom(); // get first pom
	pom_push();
	while (1) // turn to next pom
	{
		update_blob();
		turn_right(300);
		if (current.green.size >= target.green.size && current.orange.size >= target.green.size)
			break;
	}
	get_pom(); // get second pom
	pom_push();
	avoid_cubeguy();
	get_pom(); // get third pom
	pom_push();
	while (1) // turn to next pom
	{
		update_blob();
		turn_left(300);
		if (current.green.size >= target.green.size && current.orange.size >= target.green.size)
			break;
	}
	get_pom(); // get fourth pom
	pom_push();
	avoid_booster();
	while (1) // follow the line until a blob of orange and green is seen
	{
		update_blob();
		t_line_follow();
		if (current.green.size >= target.green.size && current.orange.size >= target.green.size)
			break;
	}
	get_pom(); // get fifth pom
	pom_push();
	while (1) // turn to next pom
	{
		update_blob();
		turn_right(300);
		if (current.green.size >= target.green.size && current.orange.size >= target.green.size)
			break;
	}
	get_pom(); // get sixth pom
	pom_push();
	avoid_cubeguy();
	get_pom(); // get seventh pom
	pom_push();
	while (1) // turn to next pom
	{
		update_blob();
		turn_left(300);
		if (current.green.size >= target.green.size && current.orange.size >= target.green.size)
			break;
	}
	get_pom(); // get eighth pom
	pom_push();
	avoid_booster();
	/*

	** TODO **
	FIND A WAY TO GET BACK TO THE BASE AND DUMP OFF THE POMS

	*/

}

void update_blob()
{
	camera_update();
	current.green.x = get_object_center(0 , 0).x;
	current.green.y = get_object_center(0 , 0).y;
	current.green.size = get_object_area(0 , 0);
	current.orange.x = get_object_center(0 , 0).x;
	current.orange.y = get_object_center(0 , 0).y;
	current.orange.size = get_object_area(0 , 0);
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

int camera_move_x()
{	
	int hspeed = 150;
	int lspeed = -hspeed;
	update_blob();
	if (current.green.x > (target.green.x + TOL)) // target is to the right
	{
		printf("MOVING LEFT\n");
		mav(lego.left.port , lspeed);
		mav(lego.right.port , hspeed);
		msleep(10);
		return 1;
	}
	if (current.green.x < (target.green.x - TOL)) // target is to the left
	{
		printf("MOVING RIGHT\n");
		mav(lego.left.port , hspeed);
		mav(lego.right.port , lspeed);
		msleep(10);
		return 1;
	}
	if (current.green.x >= (target.green.x - TOL) && current.green.x <= (target.green.x + TOL)) // target is in range
	{
		ao();
		return 0;
	}
	return 1;
}

int camera_move_y()
{
	int hspeed = 200;
	int lspeed = -hspeed;
	update_blob();
	if (current.green.y > (target.green.y + TOL)) // target is too close
	{
		printf("MOVING FORWARDS\n");
		mav(lego.left.port , lspeed);
		mav(lego.right.port , lspeed);
		msleep(10);
		return 1;
	}
	if (current.green.y < (target.green.y - TOL)) // target is too far
	{
		printf("MOVING BACKWARDS\n");
		mav(lego.left.port , hspeed);
		mav(lego.right.port , hspeed);
		msleep(10);
		return 1;
	}
	if (current.green.y >= (target.green.y - TOL) && current.green.y <= (target.green.y + TOL)) // target is in range
	{
		ao();
		return 0;
	}
	return 1;
}

int alt_camera_move_y()
{
	int hspeed = 200;
	int lspeed = -hspeed;
	update_blob();
	if (current.green.size > (target.green.size + STOL)) // too far away
	{
		printf("MOVING FORWARDS\n");
		mav(lego.left.port , hspeed);
		mav(lego.right.port , hspeed);
		msleep(10);
		return 1;
	}
	if (current.green.size < (target.green.size - STOL)) // too close
	{

		printf("MOVING BACKWARDS\n");
		mav(lego.left.port , lspeed);
		mav(lego.right.port , lspeed);
		msleep(10);
		return 1;
	}
	if (current.green.size >= (target.green.size - STOL) && current.green.size <= (target.green.size + STOL)) // in range
	{
		ao();
		return 0;
	}
	return 1;
}

int get_left()
{
	return analog10(left_s);
}

int get_middle()
{
	return analog10(middle_s);
}

int get_right()
{
	return analog10(right_s);
}

int get_ET()
{
	return analog10(ET_s);
}

int get_touch()
{
	return digital(8);
}

int t_line_follow()
{
	if (get_left() < THRESH && get_middle() < THRESH && get_right() < THRESH) // 0 , 0 , 0 // spin in place
	{
		mav(lego.left.port , LOW);
		mav(lego.right.port , -LOW);
		msleep(10);
		return 0;
	}
	if (get_left() > THRESH && get_middle() < THRESH && get_right() < THRESH) // 1 , 0 , 0 // 
	{
		mav(lego.left.port , LOW);
		mav(lego.right.port , HIGH);
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() > THRESH && get_right() < THRESH) // 0 , 1 , 0
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , HIGH);
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() < THRESH && get_right() > THRESH) // 0 , 0 , 1
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}  
	if (get_left() > THRESH && get_middle() > THRESH && get_right() < THRESH) // 1 , 1 , 0
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() > THRESH && get_right() > THRESH) // 0 , 1 , 1
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}
	if (get_left() > THRESH && get_middle() > THRESH && get_right() > THRESH) // 1 , 1 , 1
	{
		mav(lego.left.port , LOW);
		mav(lego.right.port , -LOW);
		msleep(10);
		return 0;
	}
	return 0;
}

int get_pom()
{
	while (1)
	{
		update_blob();
		int x = camera_move_x();
		msleep(10);
		int y = camera_move_y();
		msleep(10);
		if (x == 0 && y == 0)
		{
			ao();
			printf("IN POS\n");
			set_servo_position(arm_servo , ARM_DOWN);
			msleep(500);
			set_servo_position(arm_servo , ARM_OUT);
			msleep(500);
			set_servo_position(arm_servo , ARM_DOWN);
			msleep(500);
			set_servo_position(arm_servo , ARM_UP);
			msleep(500);
			break;
		}
	}
	return 0;
}

int pom_push()
{
	msleep(100);
	set_servo_position(arm_servo , ARM_DUMP);
	msleep(500);
	set_servo_position(push_servo , P_UP);
	msleep(500);
	set_servo_position(push_servo , P_DOWN);
	msleep(500);
	set_servo_position(push_servo , P_UP);
	msleep(500);
	set_servo_position(push_servo , P_DOWN);
	msleep(500);
	set_servo_position(push_servo , P_UP);
	msleep(500);
	set_servo_position(push_servo , P_DOWN);
	msleep(500);
	ao();
	update_blob();
	return 0;
}

int avoid_cubeguy()
{
	int hspeed = 400;
	int lspeed = 350;
	while (1) // drive to wall
	{
		mav(lego.left.port , 300);
		mav(lego.right.port , 300);
		msleep(10);
		if (get_ET() >= ET_DIST)
		{
			ao();
			break;
		}
	}
	while (1) // drive along the wall until some poms are seen
	{
		update_blob();
		if (get_ET() > ET_DIST) // too close
		{
			printf("TOO CLOSE\n");
			mav(lego.left.port , lspeed);
			mav(lego.right.port , hspeed);
			msleep(10);
		} 
		if (get_ET() < ET_DIST) // too far
		{
			printf("TOO FAR\n");
			mav(lego.left.port , hspeed);
			mav(lego.right.port , lspeed);
			msleep(10);
		}
		if (get_ET() == ET_DIST) // correct distance
		{
			mav(lego.left.port , hspeed);
			mav(lego.left.port , hspeed);
			msleep(10);
		}
		if (current.green.size >= target.green.size)
		{
			ao();
		}
	}
	return 0;
}

int avoid_booster() //  not finsihed
{
	while (1)
	{
		t_line_follow();
		if (get_touch() == 1)
		{
			ao();
			break;
		}
	}
	mav(lego.left.port , -300);
	mav(lego.right.port , -300);
	msleep(3000);
	ao();
	return 0;
}
