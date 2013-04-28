// fourth version of team 1 (0639) link bot code
// in this version I strive for: consistent naming
//								 clean code
//								 good, clear flow
//								 documentation
//								 Globals = coordinates, ports , bools, poms collected
//								 Macros = constants for actuators (UP/DOWN etc.)

// possible strategy -> DO WHILE
#include <stdbool.h>
#include "../CBClib.h"

#define ARM_DUMP 200 // up for the pom getter arm
#define ARM_UP 635
#define ARM_DOWN 1800 // down for the pom getter arm
#define B_UP 1600 // up for the basket
#define B_DOWN 750 // down for the basket (dumping)
/*
	define the pusher servo constants
*/
#define TOL 1 // tolerance for coordinates
#define THRESH 400 // threshhold for the top hat sensor
#define OSIZE 750 // size of the orange blob for detection
#define GSIZE 10 // size of the green blob for detection
#define SSIZE 3000 // size of the skycrane boosters for detection
#define DIFF 150
#define MID 800
#define HIGH MID + DIFF
#define LOW MID - DIFF


// functions
inline int camera_move_x();
inline int camera_move_y();
inline void coord_update();
inline int get_left();
inline int get_middle();
inline int get_right();
int t_line_follow();

// globals
struct coords {
	int x;
	int y;
}current , target;
// stores the coordinates currently read (for the green pom)
// stores the coordinates needed (for the green pom)
int poms_collected = 0;// stores the amount of poms collected so far
int left_s = 5; // port for the left top hat sensor
int right_s = 3; // port for the right top hat sensor
int middle_s = 6; // port for the middle top hat sensor
int arm_servo = 1; // servo of the pom getter arm
int push_servo = 0; // servo of the pusher arm
int basket_servo = 2; // servo of the basket
bool x_in = false; // checks if the x coordinate of the pom is corrent
bool y_in = false; // checks if the y coordinate of the pom is correct
bool pickup = false; 

int main()
{
	lego.left.port = 0;
	lego.right.port = 2;
	camera_open(LOW_RES);
	camera_update();
	printf("\nPress B to set Location\n");
	while (b_button() == 0) // press the b button to set the coordinates
	{
		camera_update();
		target.x = get_object_center(0 , 0).x; // sets target coordinates (x)
		target.y = get_object_center(0 , 0).y; // sets target coordinates (y)
		printf("(%d , %d)\n" , target.x , target.y);
		msleep(10);
	}
	enable_servo(arm_servo);
	enable_servo(push_servo);
	enable_servo(basket_servo);
	set_servo_position(arm_servo , ARM_UP);
	set_servo_position(basket_servo , B_UP);
	// set pusher servo to starting position
	printf("(%d , %d)\n" , target.x , target.y);
	// wait_for_light(1);
	while (a_button() == 0)
		coord_update(); // for testing, use wait for light for the actual game
	while (get_middle() > THRESH)
	{
		mav(lego.left.port , 500);
		mav(lego.right.port , 500);
	}
	while (1)
	{
		t_line_follow();
		if (get_object_area(1 , 0) > OSIZE)
		{
			if (get_object_area(0 , 0) > GSIZE)
				break;
		}
	}
	while (1) // position to get the pom
	{
		coord_update();
		camera_move_y();
		camera_move_x();
		if ((current.y >= (target.y - TOL) && (current.y <= target.y + TOL)) && (current.x >= (target.x - TOL) && (current.x <= target.x + TOL)))
		{
			ao();
			printf("IN POS\n");
			set_servo_position(arm_servo , ARM_DOWN);
			msleep(500);
			set_servo_position(arm_servo , ARM_UP);
			msleep(500);
			poms_collected += 1;
			break;
		}
	}
}	
inline int camera_move_x()
{
	int lspeed = -150;
	int hspeed = 150;
	coord_update();
	printf("MOVING X\n");
	if (current.x < target.x)
	{
		printf("LEFT\n");
		mav(lego.left.port , lspeed);
		mav(lego.right.port , hspeed);
		return 1;
	}
	if (current.x > target.x)
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
	coord_update();
	if (current.y > target.y)
	{
		printf("TOO CLOSE\n");
		mav(lego.left.port , back);
		mav(lego.right.port , back);
		return 1;
	}
	if (current.y < target.y)
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

inline void coord_update()
{
	camera_update();
	current.x = get_object_center(0 , 0).x;
	current.y = get_object_center(0 , 0).y;
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
