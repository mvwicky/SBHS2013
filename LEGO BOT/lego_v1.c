#include <stdbool.h>
#include "../CBClib.h"

// channel 0 = green
// channel 1 = orange

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

int x_c;
int y_c;

void camera_move_x();
void camera_move_y();

int main()
{
	enable_servos();
	lego.motors_started = 0;
	lego.servos_started = 0;
	int poms_collected = 0;
	bool see_blob = false;
	bool green_x = false;
	bool green_y = false;
	int left = build_left_motor(lego , L_PORT ,  79.04 , 1000 ,  56);
	int right = build_right_motor(lego , R_PORT , 79.04 , 1000 ,  56);
	int arm = build_gen_motor(lego , 0 , ARM_PORT , 1000);
	int pusher = build_servo(lego , 0 , PUSH_P , 2048 , 0 , 2047);
	int basket = build_servo(lego , 1 , BASKET_PORT , 2048 , 0 , 2047);
	if (left == -1 || right == -1)
	{
		printf("Problem assigning motor\n");
	}
	build_s_top_hat(lego , 0 , 0);
	build_s_top_hat(lego , 1 , 1);
	camera_open(LOW_RES);
	lego.s_top_hat[0].thresh = T;
	lego.s_top_hat[0].thresh = T;
	set_b_button_text("SET");
	while (b_button_clicked == 0)
	{
		x_c = get_object_center(0 , 0).x;
		y_c = get_object_center(0 , 0).y;
		printf("%d , %d\n" , x_c , y_c);
		if (b_button_clicked() == 1)
			break;
	}
	msleep(2000);
	printf("STARTING\n");
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
			green_x = ((g_x >= (x_c - TOL) && g_x <= (x_c + TOL)) && (g_y >= (y_c - TOL) && g_y <= (y_c + TOL)));
			if (see_blob == true || green_x == true)
			{
				while (green_x == false)
				{
					green_x = ((g_x >= (x_c - TOL) && g_x <= (x_c + TOL)) && (g_y >= (y_c - TOL) && g_y <= (y_c + TOL)));
					camera_move_x();
					camera_move_y();
					set_servo_position(lego.servo[basket].port , 1300);
					if (green_x == true)
					{
						mtp(lego.gen[arm].port , 100 , -398);
						bmd(lego.gen[arm].port);
						mtp(lego.gen[arm].port , 100 , 0);
						bmd(lego.gen[arm].port);
					}
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

void camera_move_x()
{
	int lspeed = 10;
	int hspeed = 80;
	camera_open(LOW_RES);
	camera_update();
	while (get_object_center(0 , 0).x >= x_c && get_object_center(0 , 0).x <= x_c)
	{
		printf("MOVING X\n");
		camera_update();
		if (get_object_center(0 , 0).x < x_c)
		{
			mav(L_PORT , lspeed);
			mav(R_PORT , hspeed);
			printf("LEFT\n");
		}
		if (get_object_center(0 , 0).x > x_c)
		{
			mav(L_PORT , hspeed);
			mav(R_PORT , lspeed);
			printf("RIGHT\n");
		}
		if (get_object_center(0 , 0).x >= x_c && get_object_center(0 , 0).x <= x_c)
		{
			break;
		}
	}
}

void camera_move_y()
{
	int speed = 100;
	camera_open(LOW_RES);
	camera_update();
	while ((get_object_center(0 , 0).y >= (y_c - TOL) && get_object_center(0 , 0).y <= (y_c + TOL)))
	{
		printf("MOVING Y\n");
		camera_update();
		if (get_object_center(0 , 0).y < (y_c - TOL))
		{
			mav(L_PORT , speed);
			mav(R_PORT , speed);
			printf("FAR AWAY\n");
		}
		if (get_object_center(0 , 0).y > (y_c + TOL))
		{
			mav(L_PORT , -speed);
			mav(R_PORT , -speed);
			printf("TOO CLOSE\n");
		}
		if (get_object_center(0 , 0).y == y_c || (get_object_center(0 , 0).y >= (y_c - TOL) && get_object_center(0 , 0).y <= (y_c + TOL)))
		{
			
		}	
	}
}
