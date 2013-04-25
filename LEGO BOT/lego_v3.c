// third version of team 1 (0639) link bot code
// in this version I strive for: consistent naming
//								 clean code
//								 good, clear flow
//								 documentation
//								 Globals = coordinates, ports , bools, poms collected
//								 Macros = constants for actuators (UP/DOWN etc.)

// possible strategy -> DO WHILE
#include <stdbool.h>
#include "../CBClib.h"

#define ARM_UP 200 // up for the pom getter arm
#define ARM_DOWN 1800 // down for the pom getter arm
#define B_UP 1600 // up for the basket
#define B_DOWN 750 // down for the basket (dumping)
/*
	define the pusher servo constants
*/
#define TOL 5 // tolerance for coordinates
#define THRESH 250 // threshhold for the top hat sensor
#define OSIZE 750 // size of the orange blob for detection
#define GSIZE 10 // size of the green blob for detection
#define SSIZE 3000 // size of the skycrane boosters for detection


// functions
inline int camera_move_x();
inline int camera_move_y();
inline int get_left();
inline int get_middle();
inline int get_right();
int t_line_follow();

// globals
int current_coords[2]; // stores the coordinates currently read (for the green pom)
int target_coords[2];// stores the coordinates needed (for the green pom)
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
	int left_motor = build_left_motor(lego , 0 , 79.04 , 1000 , 56); // sets up the left drive motor
	int right_motor = build_right_motor(lego , 1 , 79.04 , 1000 , 56); // sets up the right drive motor
	camera_open(LOW_RES);
	camera_update();
	printf("\nPress B to set Location\n");
	while (b_button() == 0) // press the b button to set the coordinates
	{
		camera_update();
		target_coords[0] = get_object_center(0 , 0).x; // sets target coordinates (x)
		target_coords[1] = get_object_center(0 , 0).y; // sets target coordinates (y)
		display_printf(0 , 1 , "(%d , %d)\n" , target_coords[0] , target_coords[1]);
		msleep(10);
		display_clear();
	}
	enable_servo(arm_servo);
	enable_servo(push_servo);
	enable_servo(basket_servo);
	set_servo_position(arm_servo , ARM_UP);
	set_servo_position(basket_servo , B_UP);
	// set pusher servo to starting position
	printf("(%d , %d)\n" , target_coords[0] , target_coords[1]);
	// wait_for_light(1);
	while (a_button() == 0); // for testing, use wait for light for the actual game
	while (get_middle() > THRESH)
	{
		mav(lego.left.port , 500);
		mav(lego.right.port , 500);
	}
	drive_spin(lego , 500 , 90 , -1);
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
		camera_move_y();
		camera_move_x();
		x_in = ((current_coords[0] == target_coords[0]) || (current_coords[0] >= (target_coords[0] - TOL) && (current_coords[0] <= target_coords[0] + TOL)));
		// is true if the x coordinate is equal to the target or within a ten unit range (5 on each side)
		y_in = ((current_coords[1] == target_coords[1]) || (current_coords[1] >= (target_coords[1] - TOL) && (current_coords[1] <= target_coords[1] + TOL)));
		// is true if the y coordinate is equal to the target or within a ten unit range (5 on each side)
		if (y_in == true)
		{
			if (x_in == true)
				break;
		}
	}	
	set_servo_position(arm_servo , ARM_DOWN);
	msleep(500);
	set_servo_position(arm_servo , ARM_UP);
	msleep(500);
	poms_collected += 1;
	while (1)
	{
		mav(lego.left.port , 300);
		mav(lego.right.port , -300);
		if (get_object_area(1 , 0) > OSIZE)
		{
			if (get_object_area(0 , 0) > GSIZE)
				break;
		}
	}
	while (1) // position to get the pom
	{
		camera_move_y();
		camera_move_x();
		x_in = ((current_coords[0] == target_coords[0]) || (current_coords[0] >= (target_coords[0] - TOL) && (current_coords[0] <= target_coords[0] + TOL)));
		// is true if the x coordinate is equal to the target or within a ten unit range (5 on each side)
		y_in = ((current_coords[1] == target_coords[1]) || (current_coords[1] >= (target_coords[1] - TOL) && (current_coords[1] <= target_coords[1] + TOL)));
		// is true if the y coordinate is equal to the target or within a ten unit range (5 on each side)
		if (y_in == true)
		{
			if (x_in == true)
				break;
		}
	}	
	set_servo_position(arm_servo , ARM_DOWN);
	msleep(500);
	set_servo_position(arm_servo , ARM_UP);
	msleep(500);
	poms_collected += 1;
}
while (1)
{
	mav(lego.left.port , -400);
	mav(lego.left.port , -400);
	if (get_middle() > THRESH)
	{
		break;
	}
}
while (1)
{
	
}

inline int camera_move_x()
{
	int lspeed = -300;
	int hspeed = 300;
	camera_open(LOW_RES);
	camera_update();
	printf("MOVING X\n");
	if (get_object_center(0 , 0).x < target_coords[0])
	{
		printf("LEFT\n");
		mav(lego.left.port , lspeed);
		mav(lego.right.port , hspeed);
		return 1;
	}
	if (get_object_center(0 , 0).x > target_coords[0])
	{
		printf("RIGHT\n");
		mav(lego.left.port , hspeed);
		mav(lego.right.port , lspeed);
		return 1;
	}
	if (get_object_center(0 , 0).x >= target_coords[0] && get_object_center(0 , 0).x <= target_coords[0])
	{
		return 0;
	}
}

inline int camera_move_y();
{
	camera_open(LOW_RES);
	camera_update();
	int speed = 200;
	int back = -200
	camera_open(LOW_RES);
	camera_update();
	if (get_object_center(0 , 0).y < target_coords[1])
	{
		printf("TOO CLOSE\n");
		mav(lego.left.port , back);
		mav(lego.right.port , back);
		return 1;
	}
	if (get_object_center(0 , 0).y < target_coords[1])
	{
		printf("TOO FAR\n");
		mav(lego.left.port , speed);
		mav(lego.left.port , speed);
		return 1;
	}
	if (get_object_center(0 , 0).y >= target_coords[1] && get_object_center(0 , 0).y <= target_coords[1])
	{
		printf("GOLDILOCKS\n");
		return 0;
	}
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

int t_line_follow();
{
	if (get_left() < THRESH && get_middle() < THRESH && get_right() < THRESH) // 0 , 0 , 0
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , 300);
	}
	if (get_left() > THRESH && get_middle() < THRESH && get_right() < THRESH) // 1 , 0 , 0
	{
		mav(lego.left.port , 100);
		mav(lego.right.port , 500);
	}
	if (get_left() < THRESH && get_middle() > THRESH && get_right() < THRESH) // 0 , 1 , 0
	{
		mav(lego.left.port , 600);
		mav(lego.right.port , 600);
	}
	if (get_left() < THRESH && get_middle() < THRESH && get_right() > THRESH) // 0 , 0 , 1
	{
		mav(lego.left.port , 500);
		mav(lego.right.port , 100);
	}  
	if (get_left() > THRESH && get_middle() > THRESH && get_right() < THRESH) // 1 , 1 , 0
	{
		mav(lego.left.port , 600);
		mav(lego.right.port , 200);
	}
	if (get_left() < THRESH && get_middle() > THRESH && get_right() > THRESH) // 0 , 1 , 1
	{
		mav(lego.left.port , 200);
		mav(lego.right.port , 600);
	}
	if (get_left() > THRESH && get_middle() > THRESH && get_right() > THRESH) // 1 , 1 , 1
	{
		mav(lego.left.port , 300);
		mav(lego.right.port , -300);
	}
	return 0;
}