// UGH

#define ARM_OUT 1590 // out for the arm, push the things away
#define ARM_UP 635 // arm is straight up
#define TOL 1
#define THRESH 400 // threshhold for the top hat sensor

#define DIFF 50
#define MID 900
#define HIGH MID + DIFF
#define LOW MID - DIFF

struct robo {
	struct moto {
		int port;
	}left , right;
}lego;

int avoid_cubeguy();

inline int get_left(); // get the value from the left top hat sensor
inline int get_middle(); // get the value from the middle top hat sensor
inline int get_right(); // get the value from the right top hat sensor
	
int t_line_follow();
	
int arm_servo = 1; // servo of the pom getter arm

int left_s = 1; // port for the left top hat sensor
int middle_s = 4; // port for the middle top hat 
int right_s = 7; // port for the right top hat sensor
int ET_s = 0;
inline int get_ET(); // get the value from the ET sensor
	
int main()
{
	set_analog_pullup(ET_s , 0); 
	set_analog_pullup(left_s , 1);
	set_analog_pullup(right_s , 1);
	set_analog_pullup(middle_s , 1);
	enable_servo(arm_servo);
	set_servo_position(arm_servo , ARM_UP);
	lego.left.port = 0;
	lego.right.port = 2;
	msleep(1000);
	while (b_button() == 0)
	{
		printf("%d\n" , get_ET());
	}
	while (a_button() == 0)
	{
		printf("%d , %d , %d\n" , get_left() , get_middle() , get_right());
	}
	/*
	while (1)
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , -300);
		msleep(10);
		if (get_left() < THRESH)
			break;
	}
	*/
	while (1)
	{
		t_line_follow();
		if (digital(15) == 1)
			break;
	}
	motor(lego.left.port , -30);
	motor(lego.right.port , -30);
	msleep(1400);
	ao();
	msleep(10);
	while (1)
	{
		motor(lego.left.port, 30);
		motor(lego.right.port , -30);
		if (get_middle() < THRESH)
		{
			ao();
			break;
		}
	}
	ao();
	while (1)
	{
		motor(lego.left.port , 30);
		motor(lego.right.port , 30);
		msleep(100); // arc until you are at the closest point to the wall (should be parallel)
		if (get_ET() > 350)
		{
			ao();
			break;
		}
	}
	while (1)
	{
		motor(lego.left.port , 0);
		motor(lego.right.port , 30);
		if (get_ET() > 620)
		{
			ao();
			break;
		}
	}
}


int avoid_cubeguy()
{
	/*
	while (1)
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , -300);
		msleep(10);
		if (get_left() < THRESH)
			break;
	}
	*/
	while (1)
	{
		t_line_follow();
		if (digital(15) == 1)
			break;
	}
	mav(lego.left.port , -300);
	mav(lego.right.port , -300);
	msleep(1500);
	ao();
	msleep(100);
	mav(lego.left.port , 300);
	mav(lego.right.port , -300);
	msleep(1000);
	set_servo_position(arm_servo , ARM_OUT);
	msleep(500);
	while (1)
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , 300);
		if (get_middle() > THRESH)
			break;
	}
	return 0;	
}

inline int get_left()
{
	return analog10(left_s);
}

inline int get_middle()
{
	return analog10(middle_s);
}

inline int get_right()
{
	return analog10(right_s);
}

int t_line_follow()
{
	printf("%d , %d , %d\n" , get_left() , get_middle() , get_right());
	if (get_left() < THRESH && get_middle() < THRESH && get_right() < THRESH) // 0 , 0 , 0 // spin in place
	{
		mav(lego.left.port , LOW);
		mav(lego.right.port , -LOW);
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
		mav(lego.left.port , LOW);
		mav(lego.right.port , -LOW);
		msleep(10);
		return 0;
	}
	return 0;
}


inline int get_ET()
{
	return analog10(ET_s);
}
