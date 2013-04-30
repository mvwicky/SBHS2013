#include "../CBClib.h"

#define ARM_DUMP 200 // up for the pom getter arm
#define ARM_UP 635 // arm is straight up
#define ARM_DOWN 1850 // down for the pom getter arm
#define ARM_OUT 1590 // out for the arm, push the things away
#define B_UP 1600 // up for the basket
#define B_DOWN 750 // down for the basket (dumping)
#define P_UP 2047 // push pom
#define P_DOWN 0 // arm retracted

#define TOL 1
#define THRESH 400 // threshhold for the top hat sensor

#define DIFF 150
#define MID 800
#define HIGH MID + DIFF
#define LOW MID - DIFF

void turn_right();
void turn_left();

inline int camera_move_x();
inline int camera_move_y();
inline void blob_update();

inline int get_left();
inline int get_middle();
inline int get_right();

int t_line_follow();
int avoid_cube();
int avoid_botguy();

void pom_push();
inline void get_pom();

struct blob_info { 
	struct color {
		int x;
		int y;
		int size;
	}green , orange , red , botguy;
}current , target;
// stores the blob info currently read (for the green pom)
// stores the blob info needed (for the green pom)

int arm_servo = 1; // servo of the pom getter arm
int push_servo = 0; // servo of the pusher arm
int basket_servo = 2; // servo of the basket

int left_s = 5; // port for the left top hat sensor
int right_s = 3; // port for the right top hat sensor
int middle_s = 6; // port for the middle top hat sensor



int main()
{
	extra_buttons_show(1); // show three extra buttons
	set_a_button_text("SET COORDS"); // set the text of various buttons
	set_c_button_text("SET POM SIZE");
	set_x_button_text("SET BOTGUY SIZE");
	set_y_button_text("SET CUBE SIZE"); 
	lego.left.port = 0;
	lego.right.port = 2;
	camera_open(LOW_RES);
	blob_update();
	printf("\nPress B to set Location\n");
	while (b_button() == 0) // press the b button to set the coordinates
	{
		blob_update();
		target.green.x = current.green.x; // sets target coordinates (x)
		target.green.y = current.green.x; // sets target coordinates (y)
		printf("(%d , %d)\n" , target.green.x , target.green.y);
		msleep(10);
	}
	while (c_button() == 0)
	{
		blob_update();
		target.green.size = current.green,size;
		target.orange.size = current.orange.size;
		printf("Orange Size = %d" , current.orange.size);
		printf(" Green Size = %d\n" , current.green.size;
	}
	while (x_button() == 0)
	{
		blob_update();
		target.botguy.size = current.botguy.size;
		target.botguy.x = current.botguy.x;
		target.botguy.y = current.botguy.y
		printf("Bot Guy Size = %d\n" , target.bgsize);
	}
	while (y_button() == 0)
	{
		blob_update();
		target.red.size = current.red.size;
		target.red.x = current.red.x;
		target.red.y = current.red.y;
		printf("Red Cube Size = %d\n" , target.rsize);
	}
	//printf("IN RANGE , O = %d , G = %d\n" ,get_object_area(1 , 0) , get_object_area(0 , 0)); 
	enable_servo(arm_servo);
	enable_servo(push_servo);
	enable_servo(basket_servo);
	set_servo_position(arm_servo , ARM_UP);
	set_servo_position(push_servo , P_DOWN);
	set_servo_position(basket_servo , B_UP);
	printf("(%d , %d)\n" , target.green,.x , target.green.y);
	while(a_button() == 0);
		blob_update();
	while (1)
	{
		blob_update();
		t_line_follow();
		if (current.orange.size > target.orange.size)
			break;
	}
	blob_update()
	get_pom();
	pom_push();
	while (1)
	{
		blob_update();
		mav(lego.left.port , 300);
		mav(lego.right.port , -300);
		msleep(10);
		if (current.gsize > 200)
			break;
	}
	blob_update();
	get_pom();
	pom_push();
	blob_update();
	if (current.red.size > target.red.size)
		avoid_cube();
	if (current.botguy.size > target.botguy.size)
		avoid_botguy();
	while (1)
	{
		blob_update();
		t_line_follow();
		if (current.orange.size > target.orange.size && current.green.size > 200)
			break;
	}
	blob_update();
	pom_push();
	pom_push();
	while (1)
	{
		blob_update();
		mav(lego.left.port , 300);
		mav(lego.right.port , -300);
		msleep(10);
		if (current.gsize > 200)
			break;
	}
	blob_update();
	get_pom();
	pom_push();
	blob_update();
	if (current.red.size > target.red.size)
		avoid_cube();
	if (current.botguy.size > target.botguy.size)
		avoid_botguy();
	int start_time = seconds();
	int t;
	while (1)
	{
		t_line_follow();
		if (seconds() < start_time + t)
			break;
	}
	while (1)
	{

	}
}

inline int camera_move_x()
{
	int lspeed = -150;
	int hspeed = 150;
	blob_update();
	printf("MOVING X\n");
	if (current.x < (target.x + TOL))
	{
		printf("LEFT\n");
		mav(lego.left.port , lspeed);
		mav(lego.right.port , hspeed);
		return 1;
	}
	if (current.x > (target.x - TOL))
	{
		printf("RIGHT\n");
		mav(lego.left.port , hspeed);
		mav(lego.right.port , lspeed);
		return 1;
	}
	if (current.x >= (target.x - TOL) && current.x <= (target.x + TOL))
	{
		return 0;
	}
}

inline int camera_move_y()
{ 	
	int speed = 150;
	int back = -150;
	blob_update();
	if (current.y > (target.y - TOL))
	{
		printf("TOO CLOSE\n");
		mav(lego.left.port , back);
		mav(lego.right.port , back);
		return 1;
	}
	if (current.y < (target.y + TOL))
	{
		printf("TOO FAR\n");
		mav(lego.left.port , speed);
		mav(lego.right.port , speed);
		return 1;
	}
	if (current.y >= (target.y - TOL) && current.y <= (target.y + TOL))
	{
		printf("GOLDILOCKS\n");
		return 0;
	}
}

inline void blob_update()
{
	camera_update();
	//update green state
	current.green.x = get_object_center(0 , 0).x;
	current.green.y = get_object_center(0 , 0).y;
	current.green.size = get_object_area(0 , 0);
	//update orange state
	current.orange.x = get_object_center(1 , 0).x;
	current.orange.y = get_object_center(1 , 0).y;
	current.orange.size = get_object_area(1 , 0);
	//update red state
	current.red.x = get_object_center(2 , 0).x;
	current.red.y = get_object_center(2 , 0).y;
	current.red.size = get_object_area(2 , 0);
	// update botguy state
	current.botguy.x = get_object_center(3 , 0).x;
	current.botguy.y = get_object_center(3 , 0).y;
	current.botguy.size = get_object_area(3 , 0);
}


inline int get_left()
{
	return analog10(left_s);
}

inline int get_middle()
{
	return analog10(middle_s);
}

inline int get_right()
{
	return analog10(right_s);
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

int avoid_cube()
{
	blob_update();
	while (1)
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , -300);
		msleep(10);
		if (get_middle() < THRESH)
			break;
	}
	while (1)
	{
		blob_update();
		t_line_follow()
		if (current.rsize > target.rsize)
			break;
	}
	while (1)
	{
		mav(lego.left.port , 300);
		mav(lego.right.port , -300);
		if (get_middle() > THRESH && get_left() > THRESH)
			break;
	}
	set_servo_position(arm_servo , ARM_OUT);
	msleep(500);
	while (1)
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , 300);
		if (get_middle() < THRESH)
			break;
	}
	return 0;	
}

int avoid_botguy()
{
	blob_update();
	while (1)
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , -300);
		msleep(10);
		if (get_middle() < THRESH)
			break;
	}
	while (1)
	{
		blob_update();
		t_line_follow()
		if (current.bgsize > target.bgsize)
			break;
	}
	while (1)
	{
		mav(lego.left.port , 300);
		mav(lego.right.port , -300);
		if (get_middle() > THRESH && get_left() > THRESH)
			break;
	}
	set_servo_position(arm_servo , ARM_OUT);
	msleep(500);
	while (1)
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , 300);
		if (get_middle() < THRESH)
			break;
	}
	return 0;	
}

void pom_push()
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
	blob_update();
}

inline void get_pom()
{
	while (1) // position to get the pom
	{
		blob_update();
		printf("(%d , %d) , (%d , %d)\n" , target.green.x , target.green.y , current.green.x , current.green.y);
		if ((current.green.y >= (target.green.y - TOL) && (current.green.y <= target.green.y + TOL)) && (current.green.x >= (target.green.x - TOL) && (current.green.x <= target.green.x + TOL)))
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
		camera_move_y();
		camera_move_x();	
		msleep(10);
	}
}