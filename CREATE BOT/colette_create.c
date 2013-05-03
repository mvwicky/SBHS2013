// Created on Fri April 26 2013

// Created on Thu April 25 2013

void spin_90_CCW();
void spin_90_CW();
void distance_straight(int distance, int speed);
//void lower();
//void center_booster();
//void booster_turn();
void drop_booster();
void move_arm(int ticks);
int drive_to_line(int speed);
void drive_to_bump(int speed);

#define ARM_MOTOR 0
#define ARM_SERVO 0
#define S_BLOB 220



/*Left 90*
Forward, align with line
Forward, turn right.
Find PVC pipe
Drive forward, pick up booster
Backup till you hit the back wall (switch)
Turn 45* left
Back up, align with black line
Slight forward, turn slightly right
Forward and drop pipe*/



int main()
{

	int x;
	int y;
	int teal = 0;
	int big = 0;
	
	camera_open(LOW_RES);
	camera_update();
	create_connect();
	//enable_servos();
	set_servo_position(ARM_SERVO,985);
	spin_90_CCW();
	move_arm(1600);
	drive_to_line(100);
	distance_straight (270, 200);
	spin_90_CW();
	
	
	
	
	/*camera_update();
	if (get_object_count(teal) > 0) // get biggest blob
	{
		x = get_object_center(teal, big).x;
		y = get_object_center(teal, big).y;
		printf("Biggest Blob at (%i, %i)/n", x,y);
	}
	else
	{
		printf("Nuttin Here/n");
		create_spin_CCW(190);
	}
	*/
	while(get_object_area(teal, big) < S_BLOB) // big is biggest blob; S_BLOB is smallest blob area
		
	{
		camera_update();
		create_drive_direct(200, 200);
	}
		
	/*
	move_arm(-1600);
	distance_straight(78, 200);
	set_servo_position(ARM_SERVO,1100);
	distance_straight(78, -200);
	drive_to_bump(-200);
	/*move_arm(-1600);
	distance_straight(500, 200);
	
	distance_straight(50,-100);
	move_arm(1600);
	drive_to_bump(-200);
	*/
	/*
	distance_straight(210,200);
	spin_90_CCW();
	drive_to_line(-100);
	distance_straight(350,200);
	drop_booster();
	*/
	
	
	
	//camera_update();
	
	/*
	while (b_button() == 0) // press the b button to set the coordinates
	{

		camera_update();
		x = get_object_center(0 , 0).x; // sets target coordinates (x)
		y = get_object_center(0 , 0).y; // sets target coordinates (y)
		printf(0 , 1 , "(%d , %d)\n" , x , y);
		msleep(10);
	}
	*/
}

void move_arm(int ticks) // 1600 ticks to all the way to the top from bottom, 
{
	mrp(ARM_MOTOR, 500, ticks);
	block_motor_done(ARM_MOTOR);
	freeze(ARM_MOTOR);
}

void spin_90_CW() //spin the create 90 degrees clockwise
{
	create_spin_CW(190);
	sleep(1);
	create_stop();
}

void spin_90_CCW() // spin the create 90 degrees counter-clockwise
{
	create_spin_CCW(190);
	sleep(1);
	create_stop();
}

void distance_straight(int distance, int speed) // distance in mm, speed in mm/s, if going backwards, make the SPEED negative
{
	int time = 0;
	time = abs(1000 * distance/speed); // time in s
	create_drive_direct(speed, speed);
	msleep(time);
	create_stop();
}

int drive_to_line(int speed) // drive to the black line (make negative to go backwards, 100 or -100 usually a good value)
{
	int lthreshold = 458; // sensors reversed
	int rthreshold = 300;
	
	while(get_create_rfcliff_amt(0.1) > rthreshold && get_create_lfcliff_amt(0.1) > lthreshold)
	{
		create_drive_direct(speed,speed);
		//msleep(0.5);
	}	
	if(get_create_rfcliff_amt(0.1) < rthreshold) //right sensor on line
	{
		while(get_create_lfcliff_amt(0.1) > lthreshold) // turn until the left sensor is on the line
		{
			create_drive_direct(speed/2,0);
		}
		create_stop();
		return 0;
	}
	if(get_create_lfcliff_amt(0.1) < lthreshold) //left sensor on line
	{
		while(get_create_rfcliff_amt(0.1) > rthreshold) // turn until the right sensor is ON the line
		{
			create_drive_direct(0,speed/2);
		}
		create_stop();
		return 0;
	}
}

void drive_to_bump(int speed) //drive the create until the back sensor hits something, make value negetive to go backwards
{
	while(digital(15)==0)
	{
		create_drive_direct(speed, speed);
	}
	printf("pvc detected");
	create_stop();
}

void drop_booster() // use this function when the arm has the booster right over the pole to put the booster on
{
	set_servo_position(ARM_SERVO,1400);
	mrp(ARM_MOTOR,200,-200);
	bmd(ARM_MOTOR);
	distance_straight(50,-50);
	set_servo_position(ARM_SERVO,1723);
	mrp(ARM_MOTOR,200,-200);
	bmd(ARM_MOTOR);
	distance_straight(150,-50);
	set_servo_position(ARM_SERVO,950);
}