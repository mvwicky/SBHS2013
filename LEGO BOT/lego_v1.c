// LEGO BOT
#include "../CBClib.h"

#define T 200
#define L_PORT 0 
#define R_PORT 2

int main()
{
	lego.motors_started = 0;
	lego.servos_started = 0;
	build_left_motor(lego , L_PORT ,  79.04 , 1000 ,  56);
	build_right_motor(lego , R_PORT , 79.04 , 1000 ,  56);
	build_s_top_hat(lego , 0 , 0);
	build_s_top_hat(lego , 1 , 1);
	//build servos
	//build motors
	camera_open(LOW_RES);
	lego.s_top_hat[0].thresh = T;
	lego.s_top_hat[0].thresh = T;
	while(1 /*all four poms aren't gotten*/)
	{
		while (1 /*until the camera sees something*/)
		{
			//line follow
			break;
		}
		break;
	}
	// dump that out
	return 0;
}
