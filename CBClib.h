#ifndef CBClib_H_INCLUDED
#define CBClib_H_INCLUDED

#define PI 3.14159265359
#define E 2.71828182845
#define DEG_TO_RAD (PI / 180)

struct link {
	int motors_started;
	int servos_started;
	struct drive_motors {
		int port;
		float radius_to_middle;
		int ticks;
		float diameter;
	}left , right;
	struct gen_motor {
		int port;
		int ticks;
		float diameter;
	}gen[4];
	struct servo {
		int port;
		int ticks;
		int min;
		int max;
		int pos;
	}servo[6];
	struct s_analog {
		int port;
		int value;
		int thresh;
	}top_hat , ET, s_top_hat[5] , light[3] , pot , slide;
	struct s_digital {
		int port;
		int value;
	}lever[2] , s_touch[4] , l_touch[6] , slot[2];
};

struct link lego , create;

int build_left_motor(struct link , int , float , int , float); // port , radius to middle (of chassis) , # of ticks , diameter (of wheel)
int build_right_motor(struct link , int , float , int , float); // port , radius to middle (of chassis) , # of ticks , diameter (of wheel)
int build_gen_motor(struct link , int , int , int); // index , port , # number of ticks
int build_servo(struct link , int, int , int , int , int); // index ,  port , # of ticks , min , max
int build_top_hat(struct link , int , int); // port , threshold (white and black)
int build_ET(struct link , int); // port
int build_s_top_hat(struct link , int , int); // index , port
int build_light(struct link , int , int); // index , port
int build_slide(struct link , int); // port
int build_pot(struct link , int); // port
int build_lever(struct link , int , int); // index , port
int build_s_touch(struct link , int , int); // index , port
int build_l_touch(struct link , int , int); // index , port
int build_slot(struct link , int , int); // index , port 
float mm_to_ticks(struct link , float); // distance in mm
float ticks_to_mm(struct link , float); // amount of ticks
float arc_length(float , float); // angle , radius
int drive_straight(struct link , int , int); // speed , distance(mm)
int drive_arc(struct link , int , float , float , int); // speed , radius , angle , direction
int drive_spin(struct link , int , float , int); // speed , angle , direction
int motor_spin_for(struct link , int , int , int); // motor index , speed , time
int msf(struct link , int , int , int); // abbreviated motor_spin_for
int motor_spin_ticks(struct link , int , int , int); // motor index , speed , ticks
int mst(struct link , int , int , int); // abbreviated motor_spin_ticks
void bmd_both(struct link); //
int move_servo_to(struct link , int , int , int); // servo index , servo final position , step
int double_servo_to(struct link , int , int , int , int); // servo 1 index , servo 2 index , final position , step
int average(int , int); // port (analog sensor) , amount of samples
int ramp_up(struct link , float , float); // speed , distance
int line_follow(struct link , int , int , int , int); // left index , right index , high speed , low speed
int delay(float); // time to delay



int build_left_motor(struct link bot , int port , float radius , int ticks , float diameter)
{
	if ((port != 0 && port != 1 && port != 2 && port != 3) || bot.motors_started > 4)
		return -1;
	bot.left.port = port;
	bot.left.radius_to_middle = radius;
	bot.left.ticks = ticks;
	bot.left.diameter = diameter;
	bot.motors_started += 1;
	return 0;
}

int build_right_motor(struct link bot , int port , float radius , int ticks , float diameter)
{
	if ((port != 0 && port != 1 && port != 2 && port != 3) || bot.motors_started > 4)
		return -1;
	bot.right.port = port;
	bot.right.radius_to_middle = radius;
	bot.right.ticks = ticks;
	bot.right.diameter = diameter;
	bot.motors_started += 1;
	return 0;
}

int build_gen_motor(struct link bot , int index , int port , int ticks)
{
	if ((port != 0 && port != 1 && port != 2 && port != 3) || bot.motors_started > 4)
		return -1;
	bot.gen[index].port = port;
	bot.gen[index].ticks = ticks;
	bot.motors_started += 1;
	return 0;
}

int build_servo(struct link bot , int index , int port , int ticks , int min , int max)
{
	if ((port != 0 && port != 1 && port != 2 && port != 3) || bot.servos_started > 4)
		return -1;
	bot.servo[index].port = port;
	bot.servo[index].ticks = ticks;
	bot.servo[index].min = min;
	bot.servo[index].max = max;
	bot.servos_started += 1;
	return 0;
}

int build_top_hat(struct link bot , int port , int thresh)
{
	bot.top_hat.port = port;
	bot.top_hat.thresh = thresh;
	return 0;
}

int build_ET(struct link bot , int port)
{
	bot.ET.port = port;
	return 0;
}

int build_s_top_hat(struct link bot , int index , int port)
{
	bot.s_top_hat[index].port = port;
	return 0;
}

int build_light(struct link bot , int index , int port)
{
	bot.s_top_hat[index].port = port;
	return 0;
}

int build_slide(struct link bot , int port)
{
	bot.slide.port = port;
	return 0;
}

int build_pot(struct link bot , int port)
{
	bot.pot.port = port;
	return 0;
}

int build_lever(struct link bot , int index , int port)
{
	bot.lever[index].port = port;
	return 0;
}

int build_s_touch(struct link bot , int index , int port)
{
	bot.s_touch[index].port  = port;
	return 0;
}

int build_l_touch(struct link bot , int index , int port)
{
	bot.l_touch[index].port = port;
	return 0;
}

int build_slot(struct link bot , int index , int port)
{
	bot.slot[index].port = port;
	return 0;
}

float mm_to_ticks(struct link bot , float dist)
{ 
	float ticks = ((dist * bot.left.ticks) / (bot.left.diameter * PI));
	return ticks;
}

float ticks_to_mm(struct link bot , float ticks)
{
	float dist = ((ticks * (bot.left.diameter * PI)) / bot.left.ticks);
	return dist;
}

float arc_length(float angle , float radius)
{
	float alength = ((DEG_TO_RAD * angle) * radius);
	return alength;
}

int drive_straight(struct link bot , int speed , int distance)
{
	float mticks = mm_to_ticks(bot , distance);
	mrp(bot.left.port , (int)mticks , speed);
	mrp(bot.left.port , (int)mticks , speed);
	bmd_both(bot);
	return 0;	
}

int drive_arc(struct link bot , int speed , float radius , float angle , int direction)
// this code makes the bot drive in an arc 
// args = struct containing the bot , base speed , raduis of circle (mm) , angle (degrees) , direction 1 = CCW -1 = CW
{
	if (speed > 1000 || speed < 0 || radius < bot.left.radius_to_middle || radius < bot.right.radius_to_middle)
	{
		return -1;
	}
	float S = arc_length(angle , radius); // base arc length
	float sinner; // variable to contain inner arc length
	float souter; // variable to contain outer arc length
	float in_ticks; // amount of ticks the inner motor will move
	float out_ticks; // amount of ticks the outer motor will move
	float sprop; // will contain the proportion between the outer and inner arc lengths
	float lspeed; // will contain left motor speed
	float rspeed; // will contain right motor speed
	if (direction == 1) // CCW
	{
		sinner = arc_length((radius - bot.left.radius_to_middle) , angle); // compute inner arc length
		souter = arc_length((radius + bot.right.radius_to_middle) , angle); // compute outer arc length
		sprop = souter/sinner; // compute the ratio between the outer and inner arc lengths
		in_ticks = mm_to_ticks(bot , sinner); // compute inner ticks
		out_ticks = mm_to_ticks(bot , souter); // compute outer ticks
		lspeed = (float)speed * sprop * (souter / S); // compute left motor speed
		rspeed = (float)speed * sprop * (sinner / S); // compute right motor speed
		mrp(bot.right.port , (int)rspeed , (int)in_ticks); // move right 
		mrp(bot.left.port , (int)lspeed , (int)out_ticks); // move left
		bmd_both(bot); // bmd
		return 0; // executed succesfully
	}
	if (direction == -1) // CW
	{
		sinner = arc_length((radius - bot.right.radius_to_middle) , angle); // compute inner arc length
		souter = arc_length((radius + bot.left.radius_to_middle) , angle); // compute outer arc length
		sprop = souter/sinner; // compute the ratio between outer and inner arc lengths
		in_ticks = mm_to_ticks(bot , sinner); // compute inner ticks
		out_ticks = mm_to_ticks(bot , souter); // compute outer ticks
		lspeed = (float)speed  * sprop * (sinner / S); // compute left motor speed
		rspeed = (float)speed * sprop * (souter / S); // compute right motor speed
		mrp(bot.right.port , (int)rspeed , (int)in_ticks); // move right
		mrp(bot.left.port , (int)lspeed , (int)out_ticks); // move left
		bmd_both(bot); // bmd
		return 0; // executed succesfully
	}
	if (direction != 1 && direction != -1)
		return -1; // did not execute successfully
}

int drive_spin(struct link bot , int speed , float angle , int direction)
{
	float ldistance;
	float rdistance;
	float lticks;
	float rticks;
	float lspeed;
	float rspeed;
	if (direction == 1) // CCW
	{
		ldistance = -1.0 * arc_length(bot.left.radius_to_middle , angle);
		rdistance = arc_length(bot.right.radius_to_middle , angle);
		lticks = mm_to_ticks(bot , ldistance);
		rticks = mm_to_ticks(bot , rdistance);
		lspeed = (float)speed * bot.left.radius_to_middle / (2 * bot.left.radius_to_middle);
		rspeed = (float)speed * bot.right.radius_to_middle / (2 * bot.right.radius_to_middle);
		mrp(bot.left.port , (int)lspeed , (int)lticks);
		mrp(bot.right.port , (int)rspeed , (int)rticks);
		bmd_both(bot);
	}
	if (direction == -1) // CW
	{
		ldistance = arc_length(bot.left.radius_to_middle , angle);
		rdistance = -1.0 * arc_length(bot.right.radius_to_middle , angle);
		lticks = mm_to_ticks(bot , ldistance);
		rticks = mm_to_ticks(bot , rdistance);
		lspeed = (float)speed , bot.left.radius_to_middle / (2 * bot.left.radius_to_middle);
		rspeed = (float)speed , bot.right.radius_to_middle / (2 * bot.right.radius_to_middle);
		mrp(bot.left.port , (int)lspeed , (int)lticks);
		mrp(bot.right.port , (int)rspeed , (int)rticks);
		bmd_both(bot);
	}
	if (direction != 1 && direction != -1)
		return -1;
}

int motor_spin_for(struct link bot , int index , int speed , int t)
{
	float ticks = ((float)speed * (float)t);
	mrp(bot.gen[index].port , speed , (int)ticks);
	bmd(bot.gen[index].port);
	return 0;
}

int msf(struct link bot , int index , int speed , int t)
{
	return motor_spin_for(bot , index , speed , t);
}

int motor_spin_ticks(struct link bot , int index , int speed , int ticks)
{
	mrp(bot.gen[index].port , speed , ticks);
	bmd(bot.gen[index].port);
	return 0;
}

int mst(struct link bot , int index , int speed , int ticks)
{
	return motor_spin_ticks(bot , index, speed , ticks);
}

void bmd_both(struct link bot)
{
	bmd(bot.left.port);
	bmd(bot.right.port);
}

int move_servo_to(struct link bot , int index , int final , int step)
{
	bot.servo[index].pos = get_servo_position(bot.servo[index].port);
	if (bot.servo[index].pos < final)
	{
		while (bot.servo[index].pos < final)
		{
			set_servo_position(bot.servo[index].port , (bot.servo[index].pos += step));
			msleep(10);
			bot.servo[index].pos = get_servo_position(bot.servo[index].port);
		}
	}
	if (bot.servo[index].pos > final)
	{
		while (bot.servo[index].pos > final)
		{
			set_servo_position(bot.servo[index].port , (bot.servo[index].pos -= step));
			msleep(10);
			bot.servo[index].pos = get_servo_position(bot.servo[index].port);
		}
	}
	if (bot.servo[index].pos == final)
		return 0;
	if (bot.servo[index].pos > bot.servo[index].max || bot.servo[index].pos < bot.servo[index].min)
		printf("Value was too low or too high\n");
		return -1;
	return 0;
	
}

int double_servo_to(struct link bot , int s1 , int s2 , int final , int step)
{
	bot.servo[s1].pos = get_servo_position(bot.servo[s1].port);
	bot.servo[s2].pos = get_servo_position(bot.servo[s2].port);
	if (bot.servo[s1].pos <= final && bot.servo[s2].pos <= final)
	{
		while (bot.servo[s1].pos <= final && bot.servo[s2].pos <= final)
		{
			set_servo_position(bot.servo[s1].port , (bot.servo[s1].pos += step));
			set_servo_position(bot.servo[s2].port , (bot.servo[s2].pos += step));
			msleep(10);
			bot.servo[s1].pos = get_servo_position(bot.servo[s1].port);
			bot.servo[s2].pos = get_servo_position(bot.servo[s2].port);
		}
	}
	if (bot.servo[s1].pos >= final && bot.servo[s2].pos >= final)
	{
		while (bot.servo[s1].pos >= final && bot.servo[s2].pos >= final)
		{
			set_servo_position(bot.servo[s1].port , (bot.servo[s1].pos -= step));
			set_servo_position(bot.servo[s2].port , (bot.servo[s2].pos -= step));
			msleep(10);
			bot.servo[s1].pos = get_servo_position(bot.servo[s1].port);
			bot.servo[s2].pos = get_servo_position(bot.servo[s2].port);
		}
	}
	if (bot.servo[s1].pos == final && bot.servo[s2].pos == final)
		return 0;
	return 0;
}

int average(int port , int samples)
// function that returns the average of an amount of sensor readings
// args = port of sensor to be read , amount of samples
{ 
	int nums[samples]; // create array for the readings
	int sum = 0; // create variable for the sum
	float average; // create variable for the average
	int n;
	for (n = 0; n < samples; n += 1) 
	// for n in samples, assign the current reading of the sensor to the current index of nums and add it to sums
	{
		nums[n] = analog10(port);
		sum += nums[n];
	}
	average = ((float)sum / (float)samples); // create the average
	return ((int)average); // return the average as an integer
}

int line_follow(struct link bot , int l_i , int r_i , int h_speed , int l_speed) 
// function to follow a black line, uses variables from the link struct
// args = struct containing variables for link control , index of left sensor , index of right motor , high speed , low speed
{
	bot.s_top_hat[l_i].value = average(bot.s_top_hat[l_i].port , 5); // assign left value
	bot.s_top_hat[r_i].value = average(bot.s_top_hat[r_i].port , 5); // assign right value
	if (bot.s_top_hat[l_i].value > 1023 || bot.s_top_hat[l_i].value < 0 || bot.s_top_hat[r_i].value > 1023 || bot.s_top_hat[r_i].value < 0)
	{ // these braces are not necessary but maintain flow
		return -1; //did not execute properly
	}
	if (bot.s_top_hat[l_i].value >= bot.s_top_hat[l_i].thresh && bot.s_top_hat[r_i].value < bot.s_top_hat[r_i].thresh)
	{ // if left is on the line and right is off:
		mav(bot.left.port , l_speed); // move left motor at low speed
		mav(bot.right.port , h_speed); // move right motor at high speed
		msleep(10);
	}
	if (bot.s_top_hat[l_i].value < bot.s_top_hat[l_i].thresh && bot.s_top_hat[r_i].value >= bot.s_top_hat[r_i].thresh)
	{ // if left is off the line and right is on
		mav(bot.left.port , h_speed); // move left motor at high speed
		mav(bot.right.port , l_speed); // move right motor at low speed
		msleep(10);
	}
	if (bot.s_top_hat[l_i].value < bot.s_top_hat[l_i].thresh && bot.s_top_hat[r_i].value < bot.s_top_hat[r_i].thresh)
	{ // if both are off the line
		mav(bot.left.port , h_speed); // move both at high speed
		mav(bot.right.port , h_speed);
		msleep(10);
	}
	if (bot.s_top_hat[l_i].value >= bot.s_top_hat[l_i].thresh && bot.s_top_hat[r_i].value >= bot.s_top_hat[r_i].thresh)
	{ // if both are on the line (assume straddling)
		mav(bot.left.port , h_speed); // move both at high speed 
		mav(bot.right.port , h_speed);
		msleep(10);
	}
	return 0; // function executed properly
}

int delay(float t)
{
	float start_time = seconds();
	while (seconds() < start_time + t);
	return 0;
}

#endif 

