#ifndef CBClib_H_INCLUDED
#define CBClib_H_INCLUDED

#define PI 3.14159265359
#define E 2.71828182845
#define DEG_TO_RAD (PI / 180)

struct cbc {
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

struct cbc lego , create;

int build_left_motor(struct cbc , int , float , int , float); // port , radius to middle (of chassis) , # of ticks , diameter (of wheel)
int build_right_motor(struct cbc , int , float , int , float); // port , radius to middle (of chassis) , # of ticks , diameter (of wheel)
int build_gen_motor(struct cbc , int , int , int); // index , port , # number of ticks
int build_servo(struct cbc , int, int , int , int , int); // index ,  port , # of ticks , min , max
int build_top_hat(struct cbc , int , int); // port , threshold (white and black)
int build_ET(struct cbc , int); // port
int build_s_top_hat(struct cbc , int , int); // index , port
int build_light(struct cbc , int , int); // index , port
int build_slide(struct cbc , int); // port
int build_pot(struct cbc , int); // port
int build_lever(struct cbc , int , int); // index , port
int build_s_touch(struct cbc , int , int); // index , port
int build_l_touch(struct cbc , int , int); // index , port
int build_slot(struct cbc , int , int); // index , port 
float mm_to_ticks(struct cbc , float); // distance in mm
float ticks_to_mm(struct cbc , float); // amount of ticks
float arc_length(float , float); // angle , radius
int drive_straight(struct cbc , int , int); // speed , distance(mm)
int drive_arc(struct cbc , int , float , float , int); // speed , radius , angle , direction
int drive_spin(struct cbc , int , float , int); // speed , angle , direction
int motor_spin_for(struct cbc , int , int , int); // motor index , speed , time
int msf(struct cbc , int , int , int); // abbreviated motor_spin_for
int motor_spin_ticks(struct cbc , int , int , int); // motor index , speed , ticks
int mst(struct cbc , int , int , int); // abbreviated motor_spin_ticks
void bmd_both(struct cbc); //
int move_servo_to(struct cbc , int , int , int); // servo index , servo final position , step
int double_servo_to(struct cbc , int , int , int); // servo 1 index , servo 2 index , final position , step
int average(struct cbc , int , int); // port (analog sensor) , amount of samples
int ramp_up(struct cbc , float , float); // speed , distance
int line_follow(struct cbc , int , int , int , int); // left index , right index , high speed , low speed
int delay(float); // time to delay



int build_left_motor(struct cbc bot , int port , float radius , int ticks , float diameter)
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

int build_right_motor(struct cbc bot , int port , float radius , int ticks , float diameter)
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

int build_gen_motor(struct cbc bot , int index , int port , int ticks)
{
	if ((port != 0 && port != 1 && port != 2 && port != 3) || bot.motors_started > 4)
		return -1;
	bot.gen[index].port = port;
	bot.gen[index].ticks = ticks;
	bot.motors_started += 1;
	return 0;
}

int build_servo(struct cbc bot , int index , int port , int ticks , int min , int max)
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

int build_top_hat(struct cbc bot , int port , int thresh)
{
	bot.top_hat.port = port;
	bot.top_hat.thresh = thresh;
	return 0;
}

int build_ET(struct cbc bot , int port)
{
	bot.ET.port = port;
	return 0;
}

int build_s_top_hat(struct cbc bot , int index , int port)
{
	bot.s_top_hat[index].port = port;
	return 0;
}

int build_light(struct cbc bot , int index , int port)
{
	bot.s_top_hat[index].port = port;
	return 0;
}

int build_slide(struct cbc bot , int port)
{
	bot.slide.port = port;
	return 0;
}

int build_pot(struct cbc bot , int port)
{
	bot.pot.port = port;
	return 0;
}

int build_lever(struct cbc bot , int index , int port)
{
	bot.lever[index].port = port;
	return 0;
}

int build_s_touch(struct cbc bot , int index , int port)
{
	bot.s_touch[index].port  = port;
	return 0;
}

int build_l_touch(struct cbc bot , int index , int port)
{
	bot.l_touch[index].port = port;
	return 0;
}

int build_slot(struct cbc bot , int index , int port)
{
	bot.slot[index].port = port;
	return 0;
}

float mm_to_ticks(struct cbc bot , float dist)
{ 
	float ticks = ((dist * bot.left.ticks) / (bot.left.diameter * PI));
	return ticks;
}

float ticks_to_mm(struct cbc bot , float ticks)
{
	float dist = ((ticks * (bot.left.diameter * PI)) / bot.left.ticks);
	return dist;
}

float arc_length(float angle , float radius)
{
	float alength = ((DEG_TO_RAD * angle) * radius);
	return alength;
}

int drive_straight(struct cbc bot , int speed , int distance)
{
	float mticks = mm_to_ticks(bot , distance);
	mrp(bot.left.port , (int)mticks , speed);
	mrp(bot.left.port , (int)mticks , speed);
	bmd_both(bot);
	return 0;	
}

int drive_arc(struct cbc bot , int speed , float radius , float angle , int direction)
{
	float S = arc_length(angle , radius);
	float sinner;
	float souter;
	float in_ticks;
	float out_ticks;
	float sprop;
	float lspeed;
	float rspeed;
	if (direction == 1) // CCW
	{
		sinner = arc_length((radius - bot.left.radius_to_middle) , angle);
		souter = arc_length((radius + bot.right.radius_to_middle) , angle);
		sprop = souter/sinner;
		in_ticks = mm_to_ticks(bot , sinner);
		out_ticks = mm_to_ticks(bot , souter);
		lspeed = (float)speed * sprop * (souter / S);
		rspeed = (float)speed * sprop * (sinner / S);
		mrp(bot.right.port , (int)rspeed , (int)in_ticks);
		mrp(bot.left.port , (int)lspeed , (int)out_ticks);
		bmd_both(bot);
		return 0;
	}
	if (direction == -1) // CW
	{
		sinner = arc_length((radius - bot.right.radius_to_middle) , angle);
		souter = arc_length((radius + bot.left.radius_to_middle) , angle);
		sprop = souter/sinner;
		in_ticks = mm_to_ticks(bot , sinner);
		out_ticks = mm_to_ticks(bot , souter);
		lspeed = (float)speed  * sprop * (sinner / S);
		rspeed = (float)speed * sprop * (souter / S);
		mrp(bot.right.port , (int)rspeed , (int)in_ticks);
		mrp(bot.left.port , (int)lspeed , (int)out_ticks);
		bmd_both(bot);
		return 0;
	}
	if (direction != 1 && direction != -1)
		return -1;
}

int drive_spin(struct cbc bot , int speed , float angle , int direction)
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

int motor_spin_for(struct cbc bot , int index , int speed , int t)
{
	float ticks = ((float)speed * (float)t);
	mrp(bot.gen[index].port , speed , (int)ticks);
	bmd(bot.gen[index].port);
	return 0;
}

int msf(struct cbc bot , int index , int speed , int t)
{
	return motor_spin_for(bot , index , speed , t);
}

int motor_spin_ticks(struct cbc bot , int index , int speed , int ticks)
{
	mrp(bot.gen[index].port , speed , ticks);
	bmd(bot.gen[index].port);
	return 0;
}

int mst(struct cbc bot , int index , int speed , int ticks)
{
	return motor_spin_ticks(bot , index, speed , ticks);
}

void bmd_both(struct cbc bot)
{
	bmd(bot.left.port);
	bmd(bot.right.port);
}

int move_servo_to(struct cbc bot , int index , int final , int step)
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
	return 0;
	
}

int double_servo_to(struct cbc bot , int s1 , int s2 , int final , int step)
{
	bot.servo[s1].pos = get_servo_position(bot.servo[s1].port);
	bot.servo[s2].pos = get_servo_position(bot.servo[s2].port);
	if (bot.servo[s1].pos <= final && bot.servo[s2].pos <= final)
	{
		set_servo_position(bot.servo[s1].port , (bot.servo[s1].pos += step));
		set_servo_position(bot.servo[s2].port , (bot.servo[s2].pos += step));
		msleep(10);
		bot.servo[s1].pos = get_servo_position(bot.servo[s1].port);
		bot.servo[s2].pos = get_servo_position(bot.servo[s2].port);
	}
	if (bot.servo[s1].pos <= final && bot.servo[s2].pos <= final)
	{
		set_servo_position(bot.servo[s1].port , (bot.servo[s1].pos -= step));
		set_servo_position(bot.servo[s2].port , (bot.servo[s2].pos -= step));
		msleep(10);
		bot.servo[s1].pos = get_servo_position(bot.servo[s1].port);
		bot.servo[s2].pos = get_servo_position(bot.servo[s2].port);
	}
	if (bot.servo[s1].pos == final && bot.servo[s2].pos == final)
		return 0;
	return 0;
}

int line_follow(struct cbc bot , int l_i , int r_i , int h_speed , int l_speed)
{
	bot.s_top_hat[l_i].value = analog10(bot.s_top_hat[l_i].port);
	bot.s_top_hat[r_i].value = analog10(bot.s_top_hat[r_i].port);
	if (bot.s_top_hat[l_i].value >= bot.s_top_hat[l_i].thresh && bot.s_top_hat[r_i].value < bot.s_top_hat[r_i].thresh)
	{
		mav(bot.left.port , l_speed);
		mav(bot.right.port , h_speed);
		msleep(10);
	}
	if (bot.s_top_hat[l_i].value < bot.s_top_hat[l_i].thresh && bot.s_top_hat[r_i].value >= bot.s_top_hat[r_i].thresh)
	{
		mav(bot.left.port , h_speed);
		mav(bot.right.port , l_speed);
		msleep(10);
	}
	if (bot.s_top_hat[l_i].value < bot.s_top_hat[l_i].thresh && bot.s_top_hat[r_i].value < bot.s_top_hat[r_i].thresh)
	{
		mav(bot.left.port , h_speed);
		mav(bot.right.port , h_speed);
		msleep(10);
	}
	if (bot.s_top_hat[l_i].value >= bot.s_top_hat[l_i].thresh && bot.s_top_hat[r_i].value >= bot.s_top_hat[r_i].thresh)
	{
		mav(bot.left.port , h_speed);
		mav(bot.right.port , h_speed);
		msleep(10);
	}
	return 0;
}

int delay(float t)
{
	float start_time = seconds();
	while (seconds() < start_time + t);
	return 0;
}

#endif 

