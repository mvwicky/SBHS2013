// Created on Thu April 25 2013
#include "../CBClib.h"
#include <stdbool.h>

#define THRESH 400 // threshhold for the top hat sensor
#define DIFF 150
#define MID 800
#define HIGH MID + DIFF
#define LOW MID - DIFF


int t_line_follow();
inline int get_left();
inline int get_middle();
inline int get_right();

int left_s = 5; // port for the left top hat sensor
int right_s = 3; // port for the right top hat sensor
int middle_s = 6; // port for the middle top hat sensor

int main()
{
	while (b_button_clicked() == 0);
	while (a_button() == 0)
	{
		t_line_follow();
		printf("%d , %d , %d\n" , get_left() , get_middle() , get_right());
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
	if (get_left() < THRESH && get_middle() < THRESH && get_right() < THRESH) // 0 , 0 , 0 // spin in place
	{
		mav(0 , LOW);
		mav(2 , -LOW);
		msleep(10);
		return 0;
	}
	if (get_left() > THRESH && get_middle() < THRESH && get_right() < THRESH) // 1 , 0 , 0 // 
	{
		mav(0 , LOW);
		mav(2 , HIGH);
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() > THRESH && get_right() < THRESH) // 0 , 1 , 0
	{
		mav(0 , HIGH);
		mav(2 , HIGH);
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() < THRESH && get_right() > THRESH) // 0 , 0 , 1
	{
		mav(0 , HIGH);
		mav(2 , LOW);
		msleep(10);
		return 0;
	}  
	if (get_left() > THRESH && get_middle() > THRESH && get_right() < THRESH) // 1 , 1 , 0
	{
		mav(0 , HIGH);
		mav(2 , LOW);
		msleep(10);
		return 0;
	}
	if (get_left() < THRESH && get_middle() > THRESH && get_right() > THRESH) // 0 , 1 , 1
	{
		mav(0 , HIGH);
		mav(2 , LOW);
		msleep(10);
		return 0;
	}
	if (get_left() > THRESH && get_middle() > THRESH && get_right() > THRESH) // 1 , 1 , 1
	{
		mav(0 , -LOW);
		mav(2 , LOW);
		msleep(10);
		return 0;
	}
	return 0;
}
