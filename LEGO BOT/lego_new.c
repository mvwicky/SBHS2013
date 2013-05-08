// FIND A WAY TO GET AROUND THE BOOSTERS
// SET THE ET DISTANCE TO A CORRECT VALUE
// FIND THE CORRECT SIZE FOR STOL

#define ARM_DUMP 250 // dump the poms in the basket
#define ARM_UP 635 // arm is straight up
#define ARM_DOWN 1570 // down for the pom getter arm
#define ARM_SCAN 990 // camera is parallel to the ground
#define ARM_OUT 1315

#define B_UP 1600 // up for the basket
#define B_DOWN 750 // down for the basket (dumping)

#define P_UP 0 // push pom
#define P_DOWN 2047 // arm retracted

#define STOL 1
#define TOL 2
#define THRESH 400
#define ET_DIST 600 // subject to change
#define TH_DIST 125

#define DIFF 150
#define MID 800
#define HIGH MID + DIFF
#define LOW MID - DIFF

#define TICKS(d) ((1000 * d) / 185)

int left_s = 1; // port for the left top hat sensor
int right_s = 7; // port for the right top hat sensor
int middle_s = 4; // port for the middle top hat sensor
int ET_s = 0;
int TH_s = 2;

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

int get_left();
int get_middle();
int get_right();
int get_ET();
int get_touch();
int get_TH();

int t_line_follow();
int line_follow_slow();

int get_pom();

int get_pom();
int pom_push();

int avoid_cubeguy();
int avoid_booster();

int nv_servo(int s , int fpos);

int arm_servo = 1; // servo of the pom getter arm
int push_servo = 0; // servo of the pusher arm
int basket_servo = 2; // servo of the basket


int main()
{
	enable_servo(arm_servo);
	nv_servo(arm_servo , ARM_SCAN);
	lego.left.port = 0;
	lego.right.port = 2;
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
		printf("(%d , %d)\n" , target.green.x , target.green.y);
		printf("SERVO POS = %d\n" , get_servo_position(1));
		
	}
	while (b_button() == 0) // set target green size
	{
		camera_update();
		target.green.size = get_object_area(0 , 0);
		printf("GREEN SIZE = %d\n" , target.green.size);
	}
	while (c_button() == 0) // set target orange size
	{
		camera_update();
		target.orange.size = get_object_area(0 , 0);
		printf("ORANGE SIZE = %d\n" , target.orange.size); 
	}
	set_a_button_text("START");
	enable_servo(push_servo);
	enable_servo(basket_servo);
	nv_servo(arm_servo , ARM_SCAN);
	set_servo_position(push_servo , P_UP);
	set_servo_position(basket_servo , 2047);
	wait_for_light(2);
	shut_down_in(118);
	update_blob();
	int t = seconds();
	int cyc = 0;
	set_servo_position(basket_servo , B_UP);
	msleep(500);
	while (get_left() > THRESH && get_middle() > THRESH && get_right() > THRESH)
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , -300);
		msleep(10);
	}
	while (1)
	{
		turn_left(300);
		if (get_left() > THRESH)
			break;
	}
	while (1) // follow the line until a blob of orange and green is seen
	{
		update_blob();
		t_line_follow();
		cyc += 1;
		if (cyc % 50 == 0)
		{
			ao();
			turn_left(300);
			msleep(150);
			
		}
		if (current.orange.size >= 75 && current.green.size > target.green.size)
			break;
	}
	turn_left(300);
	msleep(100);
	mav(lego.left.port , -300);
	mav(lego.right.port , -300);
	msleep(250);
	get_pom(); // get first pom
	pom_push();
	nv_servo(arm_servo , ARM_SCAN);
	ao();
	turn_right(300);
	msleep(1000);
	cyc = 0;
	while (1) // turn to next pom
	{
		update_blob();
		turn_right(300);
		msleep(10);
		if (current.green.size >= target.green.size && current.orange.size >= target.orange.size)
		{
			turn_left(300);
			msleep(10);
			break;
		}
	}
	get_pom(); // get second pom
	pom_push();
	msleep(100);
	set_servo_position(basket_servo , 1400);
	msleep(250);
	set_servo_position(basket_servo , 1600);
	avoid_cubeguy();
	mav(lego.left.port , 300);
	mav(lego.right.port , 300);
	msleep(1200);
	cyc = 0;
	while (1)
	{
		update_blob();
		t_line_follow();
		cyc += 1;
		if (cyc % 15 == 0)
		{
			ao();
			turn_left(300);
			msleep(150);
			
		}
		if (current.orange.size >= 75)
			break;
	}
	turn_right(300);
	msleep(75);
	get_pom(); // get third pom
	pom_push();
	msleep(500);
	set_servo_position(basket_servo , 1400);
	msleep(250);
	set_servo_position(basket_servo , 1600);
	nv_servo(arm_servo , ARM_SCAN);
	while (1) // turn to next pom
	{
		update_blob();
		turn_right(300);
		if (current.green.size >= target.green.size && current.orange.size >= target.orange.size)
			break;
	}
	
	get_pom(); // get fourth pom
	pom_push();
	nv_servo(arm_servo , ARM_SCAN);
	while (1)
	{
		turn_right(400);
		msleep(10);
		if (get_middle() < THRESH)
		{
			ao();
			break;
		}
	}
	cyc = 0;
	while (1)
	{	
		cyc += 1;
		t_line_follow();
		if (get_left() < THRESH && get_middle() < THRESH && get_right() < THRESH)
		{
			while (1)
			{
				turn_left(300);
				if (get_right() > THRESH)
					break;
			}
		}
		if (cyc == 1300)
		{
			freeze(lego.left.port);
			freeze(lego.right.port);
			break;
		}
	}
	mav(lego.left.port , 200);
	mav(lego.right.port , 500);
	msleep(1300);
	turn_right(300);
	msleep(1500);
	set_servo_position(basket_servo , B_DOWN);
	msleep(500);
	set_servo_position(basket_servo , B_UP);
	msleep(500);
	set_servo_position(basket_servo , B_DOWN);
	msleep(500);
	/*
	if (seconds() - t > 55)
	{
		turn_left(500);
		msleep(2000);
		while (1)
		{
			t_line_follow();
		}
	}
	
	while (get_ET() < 610)
	{
		mav(lego.left.port , 300);
		mav(lego.right.port , 300);
		msleep(10);
		if (get_ET() > 600)
		{
			ao();
			break;
		}
	}
	while (get_TH() > 300)
	{
		mav(lego.right.port , 300);
		msleep(10);
	}
	ao();
	while (1)
	{
		mav(lego.left.port , 400);
		mav(lego.right.port , 400);
		if (get_middle() < THRESH)
		{
			turn_right(400);
			msleep(500);
			break;
		}
	}
	nv_servo(arm_servo , ARM_SCAN);
	while (1)
	{
		update_blob();
		t_line_follow();
		if (current.green.size > target.green.size)
		{
			ao();
			break;
		}
	}
	mav(lego.left.port , -300);
	mav(lego.right.port , -300);
	msleep(250);
	get_pom();
	

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
	int speed = 150;
	update_blob();
	if ((current.green.x > (target.green.x + TOL))) // target is to the left
	{
		printf("MOVING RIGHT\n");
		turn_right(speed);
		msleep(10);
		return 1;
	}
	if ((current.green.x < (target.green.x - TOL))) // target is to the right
	{
		printf("MOVING LEFT\n");
		turn_left(speed);
		msleep(10);
		return 1;
	}
	if ((current.green.x >= (target.green.x - TOL)) && (current.green.x <= (target.green.x + TOL))) // target is in range
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
	if ((current.green.y > (target.green.y + TOL))) // target has too great a y value
	{
		printf("MOVING BACKWARDS\n");
		mav(lego.left.port , lspeed);
		mav(lego.right.port , lspeed);
		msleep(10);
		return 1;
	}
	if ((current.green.y < (target.green.y - TOL))) // target is too low a y value
	{
		printf("MOVING FORWARDS\n");
		mav(lego.left.port , hspeed);
		mav(lego.right.port , hspeed);
		msleep(10);
		return 1;
	}
	if ((current.green.y >= (target.green.y - TOL)) && (current.green.y <= (target.green.y + TOL))) // target is in range
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

int get_TH()
{
	return analog10(TH_s);
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


int line_follow_slow()
{
	if (get_left() < THRESH && get_middle() < THRESH && get_right() < THRESH) // 0 , 0 , 0 // spin in place
	{
		mav(lego.left.port , (int)(LOW / 2));
		mav(lego.right.port , (int)(-LOW / 2));
		msleep(10);
		return 0;
	}
	if (get_left() > THRESH && get_middle() < THRESH && get_right() < THRESH) // 1 , 0 , 0 // 
	{
		mav(lego.left.port , (int)(LOW / 2));
		mav(lego.right.port , (int)(HIGH / 2));
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() > THRESH && get_right() < THRESH) // 0 , 1 , 0
	{
		mav(lego.left.port , (int) (HIGH / 2));
		mav(lego.right.port , (int)(HIGH / 2));
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() < THRESH && get_right() > THRESH) // 0 , 0 , 1
	{
		mav(lego.left.port , (int)(HIGH / 2));
		mav(lego.right.port , (int)(LOW / 2));
		msleep(10);
		return 0;
	}  
	if (get_left() > THRESH && get_middle() > THRESH && get_right() < THRESH) // 1 , 1 , 0
	{
		mav(lego.left.port , (int)(HIGH / 2));
		mav(lego.right.port , (int)(LOW / 2));
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() > THRESH && get_right() > THRESH) // 0 , 1 , 1
	{
		mav(lego.left.port , (int)(HIGH / 2));
		mav(lego.right.port , (int)(LOW / 2));
		msleep(10);
		return 0;
	}
	if (get_left() > THRESH && get_middle() > THRESH && get_right() > THRESH) // 1 , 1 , 1
	{
		mav(lego.left.port , (int)(LOW / 2));
		mav(lego.right.port , (int)(-LOW / 2));
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
		int y = camera_move_y();
		if (x == 0 && y == 0)
		{
			ao();
			printf("IN POS\n");
			nv_servo(arm_servo , (ARM_DOWN + 150));
			set_servo_position(arm_servo , ARM_DOWN);
			msleep(100);
			nv_servo(arm_servo , ARM_OUT);
			msleep(500);
			nv_servo(arm_servo , ARM_DOWN);
			msleep(500);
			nv_servo(arm_servo , ARM_UP);
			msleep(500);
			break;
		}
	}
	return 0;
}

int pom_push()
{
	msleep(100);
	nv_servo(arm_servo , ARM_DUMP);
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
	nv_servo(arm_servo , ARM_UP);
	while (1)
	{
		mav(lego.left.port , -500);
		mav(lego.right.port , 500);
		msleep(10);
		if (get_middle() > THRESH)
		{
			ao();
			break;
		}
	}
	while (1)
	{
		line_follow_slow();
		if (get_ET() > 600)
		{
			ao();
			break;
		}
	}
	mav(lego.left.port , 300);
	mav(lego.right.port , -300);
	msleep(1250);
	ao();
	msleep(100);
	nv_servo(arm_servo , ARM_OUT);
	msleep(500);
	while (1)
	{
		mav(lego.left.port , -500);
		mav(lego.right.port , 500);
		msleep(10);
		if (get_middle() < THRESH)
		{
			break;
		}
	}
	while (1)
	{
		mav(lego.left.port , -500);
		mav(lego.right.port , 500);
		msleep(10);
		if (get_middle() > THRESH)
		{
			ao();
			break;
		}
	}
	while (1)
	{
		mav(lego.left.port , 300);
		mav(lego.right.port , -300);
		msleep(10);
		if (get_middle() < THRESH)
		{
			ao();
			break;
		}
	}
	nv_servo(arm_servo , ARM_SCAN);
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

int nv_servo(int s , int fpos)
{
	int cpos = get_servo_position(s);
	if (cpos > fpos)
	{
		while (cpos > fpos)
		{
			cpos -= 8;
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
			cpos += 8;
			set_servo_position(s , cpos);
			cpos = get_servo_position(s);
			msleep(10);
		}
		return 0;		
	}
	if (cpos == fpos)
		return 0;
}
