// LEGO BOT
#include "../CBClib.h"

#define T 150
#define L_PORT 0
#define R_PORT 1

int main()
{
	lego.motors_started = 0;
	lego.servos_started = 0;
	int poms_collected = 0;
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
			printf("Orange = %d , Green = %d\n" , or_size , g_size);
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
