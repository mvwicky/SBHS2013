// Created on Tue June 25 2013
#define DIFF 100
#define MID 800
#define HIGH MID + DIFF
#define LOW MID - DIFF
#define THRESH 600
#define true 1
#define false 0


int get_left();
int get_middle();
int get_right();
int left_on();
int middle_on();
int right_on();

int t_line_follow();

int left_s = 2; // port for the left top hat sensor
int right_s = 0; // port for the right top hat sensor
int middle_s = 1; // port for the middle top hat sensor

struct robot {
	struct motor {
		int port;
	}left , right;
}lego;

int main()
{
	lego.left.port = 2;
	lego.right.port = 0;
	float start = seconds();
	while (c_button() == false) 
	{
		line_follow_left();
	}
	printf("%f\n" , (seconds() - start));
	printf("Hello, World!\n");
	return 0;
}


int get_left()
{
	return analog10(left_s);
}

int get_middle()
{
	return analog10(middle_s);
}

int get_right()
{
	return analog10(right_s);
}

int left_on()
{
	if (get_left() > THRESH)
		return true;
	if (get_left() <= THRESH)
		return false;
}

int middle_on()
{
	if (get_middle() > THRESH)
		return true;
	if (get_middle() <= THRESH)
		return false;
}

int right_on()
{
	if (get_right() > THRESH)
		return true;
	if (get_right() <= THRESH)
		return false;
}

int line_follow_left()
{
	if (get_left() < THRESH && get_middle() < THRESH && get_right() < THRESH) // 0 , 0 , 0 // spin in place
	{
		mav(lego.left.port , -LOW);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}
	if (get_left() > THRESH && get_middle() < THRESH && get_right() < THRESH) // 1 , 0 , 0 // 
	{
		mav(lego.left.port , LOW);
		mav(lego.right.port , HIGH);
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() > THRESH && get_right() < THRESH) // 0 , 1 , 0
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , HIGH);
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() < THRESH && get_right() > THRESH) // 0 , 0 , 1
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}  
	if (get_left() > THRESH && get_middle() > THRESH && get_right() < THRESH) // 1 , 1 , 0
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() > THRESH && get_right() > THRESH) // 0 , 1 , 1
	{
		mav(lego.left.port , HIGH);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}
	if (get_left() > THRESH && get_middle() > THRESH && get_right() > THRESH) // 1 , 1 , 1
	{
		mav(lego.left.port , -LOW);
		mav(lego.right.port , LOW);
		msleep(10);
		return 0;
	}
	return 0;
}

