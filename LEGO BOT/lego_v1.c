#include <stdbool.h>
#include "../CBClib.h"

#define T 150
#define L_PORT 0
#define R_PORT 1
#define ARM_PORT 2
#define ARM_DOWN -400
#define ARM_UP 1
#define PUSH_P 1
#define PUSH_D 0
#define PUSH_U 2047
#define TOL 5
#define BASKET_PORT 0 
#define BASKET_UP 1300
#define BASKET_DOWN 750 
#define x_c 61
#define y_c 110

void camera_move();

int main()
{
	lego.motors_started = 0;
	lego.servos_started = 0;
	int poms_collected = 0;
	bool see_blob = false;
	bool green_in = false;
	int l = build_left_motor(lego , L_PORT ,  79.04 , 1000 ,  56);
	int r = build_right_motor(lego , R_PORT , 79.04 , 1000 ,  56);
	if (l == -1 || r == -1)
	{
		printf("Problem assigning motor\n");
	}
	build_s_top_hat(lego , 0 , 0);
	build_s_top_hat(lego , 1 , 1);
	//build_servo(lego , 0 , 2048 , 0 , 2047);
	build_gen_motor(lego , 0 , 2 , 1000);
	camera_open(LOW_RES);
	lego.s_top_hat[0].thresh = T;
	lego.s_top_hat[0].thresh = T;
	while(poms_collected < 4)
	{
		while (1 /*until the camera sees something*/)
		{
			camera_update();
			int or_size = get_object_area(1 , 0);
			int g_size = get_object_area(0 , 0);
			int g_x = get_object_center(0 , 0).x;
			int g_y = get_object_center(0 , 0).y;
			printf("Orange = %d , Green = %d\n" , or_size , g_size);
			printf("(%d , %d) , %d\n" , g_x , g_y , see_blob);
			see_blob = ((or_size >= 550 && or_size <= 575) || or_size >= 575); 
			if (see_blob == true)
			{
				while (green_in == false)
				{
					green_in = ((g_x >= (x_c - TOL) && g_x <= (x_c + TOL)) && (g_y >= (y_c - TOL) && g_y <= (y_c + TOL)))
					camera_move();
				}
			}
			if (a_button() == 1)
			{
				break;
				break;
			}
		}
		//break;
	}
	// dump that out
	return 0;
}

void camera_move()
{
	int lspeed = 10;
	int hspeed = 80;
	camera_open(LOW_RES);
	while (side_button() == 0)
	{
		camera_update();
		if (get_object_center(0 , 0).x < 45)
		{
			mav(L_PORT , lspeed);
			mav(R_PORT , hspeed);
			printf("LEFT\n");
		}
		if (get_object_center(0 , 0).x > 115)
		{
			mav(L_PORT , hspeed);
			mav(R_PORT , lspeed);
			printf("RIGHT\n");
		}
		if (get_object_center(0 , 0).x >= 45 && get_object_center(0 , 0).x <= 115)
		{
			mav(L_PORT , hspeed);
			mav(R_PORT , hspeed);
			printf("CENTER\n");
		}
	}
}
