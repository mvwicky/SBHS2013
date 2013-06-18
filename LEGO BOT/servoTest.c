// Created on Wed May 22 2013

#define ARM_UP 635 // arm is straight up
#define ARM_OUT 1315

int nv_servo(int , int);

int main()
{
	enable_servo(1);
	while (a_button() == 0);
	set_servo_position(1 , ARM_UP);
	msleep(100);
	set_servo_position(1 , ARM_OUT);
	msleep(100);
	set_servo_position(1 , ARM_UP);
	msleep(100);
	while (b_button() == 0);
	nv_servo(1 , ARM_OUT);
	nv_servo(1 , ARM_UP);
}

int nv_servo(int s , int fpos)
{
	int cpos = get_servo_position(s);
	if (cpos > fpos)
	{
		while (cpos > fpos)
		{
			cpos -= 16;
			set_servo_position(s , cpos);
			cpos = get_servo_position(s);
			msleep(10);
		}
		return 0;
	}
	if (cpos < fpos)
	{
		while (cpos < fpos)
		{
			cpos += 16;
			set_servo_position(s , cpos);
			cpos = get_servo_position(s);
			msleep(10);
		}
		return 0;		
	}
	if (cpos == fpos)
		return 0;
}
