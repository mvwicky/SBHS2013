#include <stdbool.h>

int t_line_follow(int left , int middle , int right);


int main()
{
	int left_s;
	int middle_s;
	int right_s;
}

int t_line_follow(int left , int middle , int right)
{
	int left_sens = analog10(left);
	int middle_sense = analog10(middle);
	int right_sense = analog10(right);
	if (left_sens < THRESH && middle_sense < THRESH && right_sense < THRESH) // 0 , 0 , 0
	{
		
	}
	if (left_sens > THRESH && middle_sense < THRESH && right_sense < THRESH) // 1 , 0 , 0
	{

	}
	if (left_sens < THRESH && middle_sense > THRESH && right_sense < THRESH) // 0 , 1 , 0
	{

	}
	if (left_sens < THRESH && middle_sense < THRESH && right_sense > THRESH) // 0 , 0 , 1
	{

	}  
	if (left_sens > THRESH && middle_sense > THRESH && right_sense < THRESH) // 1 , 1 , 0
	{

	}
	if (left_sens < THRESH && middle_sense > THRESH && right_sense > THRESH) // 0 , 1 , 1
	{
		
	}
	if (left_sens > THRESH && middle_sense > THRESH && right_sense > THRESH) // 1 , 1 , 1
	{

	}
	return 0;
}