// Created on Sun March 24 2013
// 100 = up
// 1230 = down

#include <stdbool.h>

#define UP 100
#define DOWN 1220

int move_servo_to(int , int);
int current_coords[2];
int target_coords[2];

int main()
{
	enable_servo(3);
	move_servo_to(3 , DOWN);
	camera_open(LOW_RES);
	printf("press b to set location\n");
	while (b_button() == 0)
	{
		camera_update();
		target_coords[0] = get_object_center(0 , 0).x;
		target_coords[1] = get_object_center(0 , 0).y;
	}
	printf("(%d , %d)\n" , target_coords[0] , target_coords[1]);
	while (side_button() == 0);
	move_servo_to(3 , UP);
	while (a_button() == 0)
	{
		camera_update();
		current_coords[0] = get_object_center(0 , 0).x;
		current_coords[1] = get_object_center(0 , 0).y;
		printf("Current = (%d , %d)" , current_coords[0] , current_coords[1]);
		printf(" Target = (%d , %d)\n" , (current_coords[0] + 2) , (current_coords[1] + 30));
		msleep(10);
		if (current_coords[0] == (target_coords[0] + 2) && current_coords[1] == (target_coords[1] + 30))
		{
			move_servo_to(3 , DOWN);
			move_servo_to(3 , UP);
		}
	}
	return 0;
}

int move_servo_to(int port , int f_pos)
{
	int c_pos = get_servo_position(port);
	if (c_pos < f_pos)
	{
		while (c_pos < f_pos)
		{
			set_servo_position(port , c_pos);
			c_pos += 1;
			msleep(10);
		}
		return 0;
	}
	if (c_pos > f_pos)
	{
		while (c_pos > f_pos)
		{
			set_servo_position(port , c_pos);
			c_pos -= 1;
			msleep(10);
		}
		return 0;
	}
	if (c_pos == f_pos)
	{
		return 0;
	}
}
