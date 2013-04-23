// Created on Sun March 24 2013
// up or down constants as macros
// ports as variables
#include <stdbool.h>
#include "../CBClib.h"

#define UP 200
#define DOWN 1800
#define TOL 5

// functions
int camera_move_x();
int camera_move_y();

// globals
int current_coords[2]; // stores the coordinates currently read (for the green pom)
int target_coords[2]; // stores the coordinates needed (for the green pom)
bool x_in;
bool y_in;

int main()
{
	int arm_servo = 1;
	int push_servo = 2;
	int basket_servo = 0;
	int left_motor = build_left_motor(lego , 0 , 79,04 , 1000 , 56);
	int right_motor = build_right_motor(lego , 1 , 79.04 , 1000 , 56);
	camera_open(LOW_RES);
	printf("\npress b to set location\n");
	while (b_button() == 0)
	{
		camera_update();
		target_coords[0] = get_object_center(0 , 0).x;
		target_coords[1] = get_object_center(0 , 0).y;
		msleep(10);
		printf("(%d , %d)\n" , target_coords[0] , target_coords[1]);
	}
	enable_servo(1);
	set_servo_position(1 , UP);
	printf("(%d , %d)\n" , target_coords[0] , target_coords[1]);
	while (a_button() == 0);
//	move_servo_to(3 , UP);
	while (1)
	{
		camera_update();
		current_coords[0] = get_object_center(0 , 0).x;
		current_coords[1] = get_object_center(0 , 0).y;
		printf("Current = (%d , %d)" , current_coords[0] , current_coords[1]);
		printf(" Target = (%d , %d)\n" , (target_coords[0] + 2) , (target_coords[1] + 30));
		msleep(10);
		x_in = (current_coords[0] == target_coords[0]) || (current_coords[0] >= (target_coords[0] - TOL) && (current_coords[0] <= target_coords[0] + TOL);
		y_in = (current_coords[1] == target_coords[1]) || (current_coords[1] >= (target_coords[1] - TOL) && (current_coords[1] <= target_coords[1] + TOL);
		camera_move_y();
		camera_move_x();
		if (x_in == true && y_in == true)
		{
			set_servo_position(1 , DOWN);
			msleep(1000);
			set_servo_position(1 , UP);
			msleep(1000);
			break;
		}
	}
	return 0;
}

int camera_move_x()
{
	int lspeed = 100;
	int hspeed = 600;
	camera_open(LOW_RES);
	camera_update();
	printf("MOVING X\n");
	camera_update();
	if (get_object_center(0 , 0).x < target_coords[0])
	{
		printf("LEFT\n");
		mav(left_motor , lspeed);
		mav(right_motor , hspeed);
		return 1;
	}
	if (get_object_center(0 , 0).x > target_coords[0])
	{
		printf("RIGHT\n");
		mav(left_motor , hspeed);
		mav(right_motor , lspeed);
		return 1;
	}
	if (get_object_center(0 , 0).x >= target_coords[0] && get_object_center(0 , 0).x <= target_coords[0])
	{
		return 0;
	}
}

int camera_move_y()
{
	int speed = 200;
	camera_open(LOW_RES);
	camera_update();
	if (get_object_center(0 , 0).y < target_coords[1])
	{
		printf("TOO CLOSE\n");
		mav(left_motor , -speed);
		mav(right_motor , -speed);
	}
	if (get_object_center(0 , 0).y < target_coords[1])
	{
		printf("TOO FAR\n");
		mav(left_motor , speed);
		mav(right_motor , speed);
	}
	if (get_object_center(0 , 0).y >= target_coords[1] && get_object_center(0 , 0).y <= target_coords[1])
	{
		printf("GOLDILOCKS\n");
		return 0;
	}
}