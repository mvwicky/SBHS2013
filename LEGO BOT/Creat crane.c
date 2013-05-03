// Created on Fri May 3 2013

#define ARM_ANGLE analog10 (1)
#define BACK digital (15)
#define LAG .001
#define LCLIFF get_create_lcliff_amt(LAG)
#define RCLIFF get_create_rcliff_amt(LAG)

int main()
{
	create_connect();
	enable_servos();
	clear_motor_position_counter(0);
	set_servo_position(0 , 700);
	msleep(500);
	
	//set_create_normalized_angle(angle);
	/*while (get_create_normalized_angle(10) <90)
	{
		create_spin_CCW(75);
		printf("%d\n", get_create_normalized_angle(10));
	}*/
	

	create_spin_CCW (100);
	msleep(2000);
	create_stop();
	printf("%d", ARM_ANGLE);
	/*while (ARM_ANGLE < 1010)
	{
		printf("%d \n", ARM_ANGLE);
		mav( 0 , 200);
		msleep(300);
	}
	*/
	mtp ( 0, 200, 1400);
	msleep(4000);

	
	while (LCLIFF > 300  && RCLIFF  > 300)
	{
		create_drive_straight (100);
	}
	while(1)
	{
		set_servo_position(0 , 700);
	create_drive_straight(200);
	msleep(1000);
	create_stop();
	create_spin_CW (100);
	msleep(2000);
	create_stop();
	
	/*while (ARM_ANGLE > 976)
	{
		printf("%d \n", ARM_ANGLE);
		mav( 0 , -200);
		msleep(300);
	}*/
	mtp ( 0, 200, -470);
	msleep(6000);
	create_stop();
	
	while ( BACK == 0)
	{
		create_drive_straight (-100);
	}
	
	create_drive_straight(200);//Get first booster
	msleep(2400);
	create_stop();
	
	set_servo_position (0 , 1350);
	msleep(500);
	
	while ( BACK == 0)
	{
		create_drive_straight (-100);
	}
	
	create_drive_straight (200);
	msleep(1000);
	create_stop();
	
	mtp( 0 , 200 , 1400);
	msleep(10000);
	
	create_spin_CCW (100);
	msleep(2000);
	
	while (LCLIFF > 300  && RCLIFF  > 300)
		{
			create_drive_straight (-100);
		}	
	create_stop();
	
	create_drive_straight (200);
	msleep(1200);
	create_stop();
	
		
	mtp( 0 , 200 , 800);
	msleep(4000);
		
	set_servo_position(0 , 700);
		
	create_drive_straight (100);
	msleep(300);
	create_stop();
	mtp( 0 , 200 , 400);
	msleep(500);
	create_drive_straight( -100);
	msleep(500);
	create_stop();
	mtp( 0 , 200 , 200);
	msleep(500);
	create_drive_straight (-100);
	msleep(2000);
	create_stop();
	
		
	while (LCLIFF > 300  && RCLIFF  > 300)
	{			
		create_drive_straight (50);
	}

	mtp ( 0, 200, 1400);
	msleep(4000);
}
		
	
	create_disconnect();
	return 0;
}
