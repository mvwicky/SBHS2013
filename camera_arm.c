// Created on Sun March 24 2013
// 100 = up
// 1230 = down

#include <stdbool.h>

#define UP 0
#define DOWN -400

int current_coords[2];
int target_coords[2];

int main()
{
	
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
	set_servo_position(1 , 200);
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
		if (current_coords[0] == (target_coords[0]) && current_coords[1] == (target_coords[1]))
		{
			set_servo_position(1 , 1800);
			msleep(1000);
			set_servo_position(1 , 200);
			msleep(1000);
			break;
		}
	}
	return 0;
}

