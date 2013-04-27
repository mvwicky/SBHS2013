#include <stdbool.h>
#include "../CBClib.h"

#define ARM_DUMP 200 // up for the pom getter arm
#define ARM_UP 635
#define ARM_DOWN 1800 // down for the pom getter arm
#define B_UP 1600 // up for the basket
#define B_DOWN 750 // down for the basket (dumping)
#define TOL 5

inline int camera_move_x();
inline int camera_move_y();

struct coords {
	int x;
	int y;
}current , target;
// stores the coordinates currently read (for the green pom)
// stores the coordinates needed (for the green pom)
int arm_servo = 1; // servo of the pom getter arm
bool x_in = false; // checks if the x coordinate of the pom is corrent
bool y_in = false; // checks if the y coordinate of the pom is correct

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
		target.x = get_object_center(0 , 0).x; // sets target coordinates (x)
		target.y = get_object_center(0 , 0).y; // sets target coordinates (y)
		printf(0 , 1 , "(%d , %d)\n" , target.x , target.y);
		msleep(10);
	}
	enable_servo(arm_servo);
	set_servo_position(arm_servo , ARM_UP);
	printf("(%d , %d)\n" , target.x , target.y);
	while(a_button() == 0);
	while (1) // position to get the pom
	{
		camera_move_y();
		camera_move_x();
		x_in = (target.x >= (target.x - TOL) && (target.x <= target.x + TOL));
		// is true if the x coordinate is equal to the target or within a ten unit range (5 on each side)
		y_in = (target.y >= (target.y - TOL) && (target.y <= target.y + TOL));
		// is true if the y coordinate is equal to the target or within a ten unit range (5 on each side)
		if (y_in == true && x_in == true)
		{
			break;
		}
	}	
	set_servo_position(arm_servo , ARM_DOWN);
	msleep(500);
	set_servo_position(arm_servo , ARM_UP);
	msleep(500);


}


inline int camera_move_x()
{
	int lspeed = -300;
	int hspeed = 300;
	camera_open(LOW_RES);
	camera_update();
	printf("MOVING X\n");
	if (get_object_center(0 , 0).x < target.x)
	{
		printf("LEFT\n");
		mav(lego.left.port , lspeed);
		mav(lego.right.port , hspeed);
		msleep(1);
		return 1;
	}
	if (get_object_center(0 , 0).x > target.x)
	{
		printf("RIGHT\n");
		mav(lego.left.port , hspeed);
		mav(lego.right.port , lspeed);
		msleep(1);
		return 1;
	}
	if (get_object_center(0 , 0).x >= target.x && get_object_center(0 , 0).x <= target.x)
	{
		return 0;
	}
}

inline int camera_move_y()
{ 	
	camera_open(LOW_RES);
	camera_update();
	int speed = 200;
	int back = -200;
	camera_open(LOW_RES);
	camera_update();
	if (get_object_center(0 , 0).y > target.y)
	{
		printf("TOO CLOSE\n");
		mav(lego.left.port , back);
		mav(lego.right.port , back);
		msleep(1);
		return 1;
	}
	if (get_object_center(0 , 0).y < target.y)
	{
		printf("TOO FAR\n");
		mav(lego.left.port , speed);
		mav(lego.left.port , speed);
		msleep(1);
		return 1;
	}
	if (get_object_center(0 , 0).y >= target.y && get_object_center(0 , 0).y <= target.y)
	{
		printf("GOLDILOCKS\n");
		return 0;
	}
}

