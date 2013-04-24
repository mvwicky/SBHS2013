// Created on Sun March 24 2013
#include <stdbool.h>
#include "../CBClib.h"
// servo/motor constants as macros (arm being up or down etc)
// ports as variables
// channel 0 = green
// channel 1 = orange

#define UP 200 // up for the pom getter arm
#define DOWN 1800 // down for the pom getter arm
#define TOL 5 
#define THRESH 250 // threshold for the top hat sensor
#define SIZE // size of the orange blob for detection

// functions
int camera_move_x(); // move bot in the x direction
int camera_move_y(); // move bot in the y direction


// globals
int current_coords[2]; // stores the coordinates currently read (for the green pom)
int target_coords[2]; // stores the coordinates needed (for the green pom)
int poms_collected = 0; // stores the amount of poms collected so far
bool x_in = false; // checks if the x coordinate of the pom is correct
bool y_in = false; // checks if the y coordinate of the pom is corrent
bool pickup = false; // checks if the arm has been engaged

int main()
{
	int arm_servo = 1; // servo of the pom getter arm
	int push_servo = 2; // servo of the pusher arm
	int basket_servo = 0; // servo of the basket
	int middle_hat = 0;
	int left_motor = build_left_motor(lego , 0 , 79,04 , 1000 , 56); // sets up the left drive motor
	int right_motor = build_right_motor(lego , 1 , 79.04 , 1000 , 56); // sets up the right drive motor
	camera_open(LOW_RES); // opens the camera
	printf("\npress b to set location\n");
	while (b_button() == 0) // press the b button to set the coordinates
	{
		camera_update();
		target_coords[0] = get_object_center(0 , 0).x; // sets target coordinates (x)
		target_coords[1] = get_object_center(0 , 0).y; // sets target coordinates (y)
		msleep(10);
		printf("(%d , %d)\n" , target_coords[0] , target_coords[1]);
	}
	enable_servo(arm_servo); // enable the arm servo
	set_servo_position(arm_servo , UP); // set the arm servo up
	printf("(%d , %d)\n" , target_coords[0] , target_coords[1]);
	while (a_button() == 0); // wait until the a button is pressed
//	move_servo_to(3 , UP);
	while (poms_collected < 4) // there are four piles of poms
	{
		pickup = false; // the arm hasn't been engaged

		if (get_object_area(0 , 1) > SIZE) // if a big pile of orange poms is seen
		{
			while (pickup == false) // while the arm hasn't been engaged 
			{
				camera_update();
				current_coords[0] = get_object_center(0 , 0).x; // updates the current coordinates (x)
				current_coords[1] = get_object_center(0 , 0).y; // updates the current coordinates (y)
				printf("Current = (%d , %d)" , current_coords[0] , current_coords[1]);
				printf(" Target = (%d , %d)\n" , (target_coords[0] + 2) , (target_coords[1] + 30));
				msleep(10);
				x_in = (current_coords[0] == target_coords[0]) || (current_coords[0] >= (target_coords[0] - TOL) && (current_coords[0] <= target_coords[0] + TOL);
				// is true if the x coordinate is equal to the target or within a ten unit range (5 on each side)
				y_in = (current_coords[1] == target_coords[1]) || (current_coords[1] >= (target_coords[1] - TOL) && (current_coords[1] <= target_coords[1] + TOL);
				// is true if the y coordinate is equal to the target or within a ten unit range (5 on each side)
				camera_move_y(); // move in the x
				camera_move_x(); // move in the y
				if (x_in == true && y_in == true)
				{
					set_servo_position(1 , DOWN);
					msleep(1000);
					set_servo_position(1 , UP);
					msleep(1000);
					poms_collected += 1;
					drive_spin(lego , 90 , -1);
					while (pickup == false)
					{
						camera_update();
						current_coords[0] = get_object_center(0 , 0).x; // updates the current coordinates (x)
						current_coords[1] = get_object_center(0 , 0).y; // updates the current coordinates (y)
						printf("Current = (%d , %d)" , current_coords[0] , current_coords[1]);
						printf(" Target = (%d , %d)\n" , (target_coords[0] + 2) , (target_coords[1] + 30));
						msleep(10);
						x_in = (current_coords[0] == target_coords[0]) || (current_coords[0] >= (target_coords[0] - TOL) && (current_coords[0] <= target_coords[0] + TOL);
						// is true if the x coordinate is equal to the target or within a ten unit range (5 on each side)
						y_in = (current_coords[1] == target_coords[1]) || (current_coords[1] >= (target_coords[1] - TOL) && (current_coords[1] <= target_coords[1] + TOL);
						// is true if the y coordinate is equal to the target or within a ten unit range (5 on each side)
						camera_move_y(); // move in the x
						camera_move_x(); // move in the y
						if (x_in == true && y_in == true)
						{
							set_servo_position(1 , DOWN);
							msleep(1000);
							set_servo_position(1 , UP);
							msleep(1000);
							poms_collected += 1;
							pickup == true;
							while (analog10(middle_hat) < THRESH)
							{
								mav(left_motor , -500)
								mav(right_motor , 500)
							}
						}
					}
				}
		}
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