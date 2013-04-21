// Created on Wed March 13 2013
// left white  = 1092 , left black  = 236
// right white = 1692 , right white = 300


#define LAG .001
#define RADIUS 145
#define PI 3.141592654
#define DEG_TO_RAD (PI/ 180)
#define SANG(x) set_create_total_angle(x)
#define SDIST(x) set_create_distance(x)
#define GANG get_create_total_angle(LAG)
#define GDIST get_create_distance(LAG)
#define T 714
#define LFCLIFF get_create_lfcliff_amt(LAG)
#define RFCLIFF get_create_rfcliff_amt(LAG)
#define LCLIFF get_create_lcliff_amt(LAG)
#define RCLIFF get_create_rcliff_amt(LAG)

void line_follow();
void turn_while(float , float , int); // d is positive for CCW, negative for CW

int main()
{
	create_connect();
	enable_servos();
	set_servo_position(3 , 822);
	while (side_button() == 0)
	{
		line_follow();
		set_servo_position(3 , 822);
		msleep(10);
	}
	create_disconnect();
	return 0;
}

void line_follow()
{
	int ldrive = -150;
	int hdrive = 150;
	int mdrive = 200;
	if (LFCLIFF >= T && RFCLIFF <= T)
	{
		create_drive_direct(hdrive , ldrive);
	}
	else if (LFCLIFF <= T && RFCLIFF >= T)
	{
		create_drive_direct(ldrive , hdrive);
	}
	else if  (LFCLIFF >= T && RFCLIFF >= T)
	{
		create_drive_direct(hdrive , hdrive);
	}
	else if (LFCLIFF <= T && RFCLIFF <= T)
	{
		create_drive_direct(mdrive , mdrive);
	}
	else if (LCLIFF >= T)
	{
		create_stop();
		turn_while(250 , 77 , 1);
	}
	else if (RCLIFF >= T)
	{
		create_stop();
		turn_while(250 , 77 , -1);
	}
}

void turn_while(float s , float a , int d)
{
	a *= DEG_TO_RAD;
	float t = ((RADIUS * a) / s);
	if (d == 1)
	{
		create_spin_CCW((int)s);
		msleep((int)(t * 1000));
		
	}
	if (d == -1)
	{
		create_spin_CW((int)s);
		msleep((int)(t * 1000));
	}
}
