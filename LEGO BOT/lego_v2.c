// Created on Sun March 24 2013
#include <stdbool.h>
#include "../CBClib.h"
// servo/motor constants as macros (arm being up or down etc)
// ports as variables
// channel 0 = green
// channel 1 = orange

#define UP 200 // up for the pom getter arm
#define DOWN 1800 // down for the pom getter arm
#define B_UP 1600
#define B_DOWN 750
#define TOL 5 
#define THRESH 250 // threshold for the top hat sensor
#define OSIZE 750 // size of the orange blob for detection
#define GSIZE 10 // size of the green blob for detection

// functions
int camera_move_x(int left_mot , int right_mot); // move bot in the x direction
int camera_move_y(int left_mot , int right_mot); // move bot in the y direction
int t_line_follow(int left , int middle , int right);


// globals
int current_coords[2]; // stores the coordinates currently read (for the green pom)
int target_coords[2]; // stores the coordinates needed (for the green pom)
int poms_collected = 0; // stores the amount of poms collected so far
int left_s = 5;
int right_s = 3;
int middle_s = 6; 
bool x_in = false; // checks if the x coordinate of the pom is correct
bool y_in = false; // checks if the y coordinate of the pom is corrent
bool pickup = false; // checks if the arm has been engaged

int main()
{
	int arm_servo = 1; // servo of the pom getter arm
	int push_servo = 0; // servo of the pusher arm
	int basket_servo = 2; // servo of the basket
	int left_motor = build_left_motor(lego , 0 , 79.04 , 1000 , 56); // sets up the left drive motor
	int right_motor = build_right_motor(lego , 1 , 79.04 , 1000 , 56); // sets up the right drive motor
	camera_open(LOW_RES); // opens the camera
	printf("\npress b to set location\n");
	while (b_button() == 0) // press the b button to set the coordinates
	{
		camera_update();
		target_coords[0] = get_object_center(0 , 0).x; // sets target coordinates (x)
		target_coords[1] = get_object_center(0 , 0).y; // sets target coordinates (y)
		printf("(%d , %d)\n" , target_coords[0] , target_coords[1]);
		msleep(10);
		display_clear();
	}
	enable_servo(arm_servo); // enable the arm servo
	set_servo_position(arm_servo , UP); // set the arm servo up
	set_servo_position(basket_servo , B_UP);
	printf("(%d , %d)\n" , target_coords[0] , target_coords[1]);
	//wait_for_light(1); // wait for it
	while (a_button() == 0);
	while (poms_collected < 4) // there are four piles of poms
	{
		pickup = false; // the arm hasn't been engaged
		t_line_follow(left_s , middle_s , right_s);
		if (get_object_area(1 , 0) > OSIZE && get_object_area(0 , 0) > GSIZE) // if a big pile of orange poms is seen
		{
			while (pickup == false) // while the arm hasn't been engaged 
			{
				camera_update();
				current_coords[0] = get_object_center(0 , 0).x; // updates the current coordinates (x)
				current_coords[1] = get_object_center(0 , 0).y; // updates the current coordinates (y)
				printf("Current = (%d , %d)" , current_coords[0] , current_coords[1]);
				printf(" Target = (%d , %d)\n" , (target_coords[0] + 2) , (target_coords[1] + 30));
				msleep(10);
				x_in = ((current_coords[0] == target_coords[0]) || (current_coords[0] >= (target_coords[0] - TOL) && (current_coords[0] <= target_coords[0] + TOL)));
				// is true if the x coordinate is equal to the target or within a ten unit range (5 on each side)
				y_in = ((current_coords[1] == target_coords[1]) || (current_coords[1] >= (target_coords[1] - TOL) && (current_coords[1] <= target_coords[1] + TOL)));
				// is true if the y coordinate is equal to the target or within a ten unit range (5 on each side)
				camera_move_y(lego.left.port , lego.right.port); // move in the x
				camera_move_x(lego.left.port , lego.right.port); // move in the y
				if (x_in == true && y_in == true)
				{
					set_servo_position(1 , DOWN); // get the pom
					msleep(1000); // wait
					set_servo_position(1 , UP); // pick it up
					msleep(1000); // wait
					poms_collected += 1; // we've got one more pom 
					drive_spin(lego , 500 , 90 , -1); // turn to the next pom
					while (pickup == false)
					{
						camera_update();
						current_coords[0] = get_object_center(0 , 0).x; // updates the current coordinates (x)
						current_coords[1] = get_object_center(0 , 0).y; // updates the current coordinates (y)
						printf("Current = (%d , %d)" , current_coords[0] , current_coords[1]);
						printf(" Target = (%d , %d)\n" , (target_coords[0] + 2) , (target_coords[1] + 30));
						msleep(10);
						x_in = ((current_coords[0] == target_coords[0]) || (current_coords[0] >= (target_coords[0] - TOL) && (current_coords[0] <= target_coords[0] + TOL)));
						// is true if the x coordinate is equal to the target or within a ten unit range (5 on each side)
						y_in = ((current_coords[1] == target_coords[1]) || (current_coords[1] >= (target_coords[1] - TOL) && (current_coords[1] <= target_coords[1] + TOL)));
						// is true if the y coordinate is equal to the target or within a ten unit range (5 on each side)
						camera_move_y(lego.left.port , lego.right.port); // move in the x
						camera_move_x(lego.left.port , lego.right.port); // move in the y
						if (x_in == true && y_in == true)
						{
							set_servo_position(1 , DOWN);
							msleep(1000);
							set_servo_position(1 , UP);
							msleep(1000);
							poms_collected += 1;
							pickup == true;
							while (analog10(middle_s) < THRESH) // turn back to the line
							{
								mav(left_motor , -500);
								mav(right_motor , 500);
							} // end turn back to line loop
							while (get_object_area(0 , 0) > 3000) // avoid the skycrane
							{
								mav(lego.left.port , 400);
								mav(lego.right.port , 400);
								if (get_object_area(0 , 0) > 3000)
								{
									mav(lego.left.port , -400);
									mav(lego.right.port , -400);
									msleep(250);
									mav(lego.left.port , 450);
									mav(lego.right.port , 350);
									msleep(500);
									while (analog10(middle_s) > THRESH)
									{
										mav(lego.left.port , 500);
										mav(lego.right.port , 500);
									}
								}
							}
						} // end pick up pom 2 if
					} // end find pom 2 loop
				} // end pick up pom 1 if
			} // end find pom 1 loop
		} // end find block of poms if

	} // end get 4 poms loop
	return 0;
}

int camera_move_x(int left_mot , int right_mot)
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
		mav(left_mot , lspeed);
		mav(right_mot , hspeed);
		return 1;
	}
	if (get_object_center(0 , 0).x > target_coords[0])
	{
		printf("RIGHT\n");
		mav(left_mot , hspeed);
		mav(right_mot , lspeed);
		return 1;
	}
	if (get_object_center(0 , 0).x >= target_coords[0] && get_object_center(0 , 0).x <= target_coords[0])
	{
		return 0;
	}
}

int camera_move_y(int left_mot , int right_mot)
{
	int speed = 200;
	camera_open(LOW_RES);
	camera_update();
	if (get_object_center(0 , 0).y < target_coords[1])
	{
		printf("TOO CLOSE\n");
		mav(left_mot , -speed);
		mav(right_mot , -speed);
	}
	if (get_object_center(0 , 0).y < target_coords[1])
	{
		printf("TOO FAR\n");
		mav(left_mot , speed);
		mav(right_mot , speed);
	}
	if (get_object_center(0 , 0).y >= target_coords[1] && get_object_center(0 , 0).y <= target_coords[1])
	{
		printf("GOLDILOCKS\n");
		return 0;
	}
}

int t_line_follow(int left , int middle , int right)
{
	int left_sens = analog10(left);
	int middle_sense = analog10(middle);
	int right_sense = analog10(right);
	if (left_sens < THRESH && middle_sense < THRESH && right_sense < THRESH) // 0 , 0 , 0
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , 300);
	}
	if (left_sens > THRESH && middle_sense < THRESH && right_sense < THRESH) // 1 , 0 , 0
	{
		mav(lego.left.port , 100);
		mav(lego.right.port , 500);
	}
	if (left_sens < THRESH && middle_sense > THRESH && right_sense < THRESH) // 0 , 1 , 0
	{
		mav(lego.left.port , 600);
		mav(lego.right.port , 600);
	}
	if (left_sens < THRESH && middle_sense < THRESH && right_sense > THRESH) // 0 , 0 , 1
	{
		mav(lego.left.port , 500);
		mav(lego.right.port , 100);
	}  
	if (left_sens > THRESH && middle_sense > THRESH && right_sense < THRESH) // 1 , 1 , 0
	{
		mav(lego.left.port , 600);
		mav(lego.right.port , 200);
	}
	if (left_sens < THRESH && middle_sense > THRESH && right_sense > THRESH) // 0 , 1 , 1
	{
		mav(lego.left.port , 200);
		mav(lego.right.port , 600);
	}
	if (left_sens > THRESH && middle_sense > THRESH && right_sense > THRESH) // 1 , 1 , 1
	{
		mav(lego.left.port , 300);
		mav(lego.right.port , -300);
	}
	return 0;
}
