#include <stdbool.h>
#include "../CBClib.h"

#define ARM_DUMP 200 // up for the pom getter arm
#define ARM_UP 635
#define ARM_DOWN 1800 // down for the pom getter arm
#define B_UP 1600 // up for the basket
#define B_DOWN 750 // down for the basket (dumping)
#define TOL 1

inline int camera_move_x();
inline int camera_move_y();
inline void coord_update();

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
	lego.left.port = 0;
	lego.right.port = 2;
	camera_open(LOW_RES);
	coord_update();
	printf("\nPress B to set Location\n");
	while (b_button() == 0) // press the b button to set the coordinates
	{
		coord_update();
		target.x = get_object_center(0 , 0).x; // sets target coordinates (x)
		target.y = get_object_center(0 , 0).y; // sets target coordinates (y)
		printf("(%d , %d)\n" , target.x , target.y);
		msleep(10);
	}
	while (c_button() == 0)
	{
		camera_update();
		printf("Orange Size = %d\n" , get_object_center(1 , 0));
		printf("Green Size = %d\n" , get_object_center(0 , 0));
	}
	enable_servo(arm_servo);
	set_servo_position(arm_servo , ARM_UP);
	printf("(%d , %d)\n" , target.x , target.y);
	while(a_button() == 0);
		coord_update();
	while (1) // position to get the pom
	{
		coord_update();
		printf("(%d , %d) , (%d , %d)\n" , target.x , target.y , current.x , current.y);
		x_in = (current.x >= (target.x - TOL) && (current.x <= target.x + TOL));
		// is true if the x coordinate is equal to the target or within a ten unit range (5 on each side)
		y_in = (current.y >= (target.y - TOL) && (current.y <= target.y + TOL));
		// is true if the y coordinate is equal to the target or within a ten unit range (5 on each side)
		if ((current.y >= (target.y - TOL) && (current.y <= target.y + TOL)) && (current.x >= (target.x - TOL) && (current.x <= target.x + TOL)))
		{
			ao();
			printf("IN POS\n");
			set_servo_position(arm_servo , ARM_DOWN);
			msleep(500);
			set_servo_position(arm_servo , ARM_UP);
			msleep(500);
			break;
		}
		camera_move_y();
		camera_move_x();
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
