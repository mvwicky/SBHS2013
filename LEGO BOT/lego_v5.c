#include "../CBClib.h"

#define ARM_DUMP 200 // up for the pom getter arm
#define ARM_UP 635 // arm is straight up
#define ARM_DOWN 1850 // down for the pom getter arm
#define ARM_OUT 1590 // out for the arm, push the things away
#define B_UP 1600 // up for the basket
#define B_DOWN 750 // down for the basket (dumping)
#define P_UP 2047 // push pom
#define P_DOWN 0 // arm retracted

#define TOL 1
#define THRESH 14 // threshhold for the top hat sensor

#define DIFF 50
#define MID 900
#define HIGH MID + DIFF
#define LOW MID - DIFF

void turn_right(int speed); // function to turn the robot right at speed
void turn_left(int speed); // function to turn the robot left at speed 

inline int camera_move_x(); // move the camera in the x direction (kind of an arc)
inline int camera_move_y(); // move the camera in the y direction
inline void blob_update(); // update the camera and update all the current.channel.x,y,size

inline int get_left(); // get the value from the left top hat sensor
inline int get_middle(); // get the value from the middle top hat sensor
inline int get_right(); // get the value from the right top hat sensor
inline int get_ET(); // get the value from the ET sensor

int t_line_follow(); // fololow the line
int avoid_cubeguy(); // avoid the red cube
int avoid_booster(); // avoid the skycrane

void pom_push(); // push the pom into the basket
inline void get_pom(); // pick up the pom using the arm

struct blob_info { 
	struct color {
		int x;
		int y;
		int size;
	}green , orange , red , botguy , teal , pink;
}current , target;
// stores the blob info currently read (for the green pom)
// stores the blob info needed (for the green pom)

int arm_servo = 1; // servo of the pom getter arm
int push_servo = 0; // servo of the pusher arm
int basket_servo = 2; // servo of the basket

int left_s = 1; // port for the left top hat sensor
int middle_s = 4; // port for the middle top hat 
int right_s = 7; // port for the right top hat sensor
int ET_s = 0; // port for the ET sensor


int main()
{
	msleep(2500);
	set_analog_pullup(ET_s , 0); 
	extra_buttons_show(1); // show three extra buttons
	set_a_button_text("COORDS"); // set the text of various buttons
	set_b_button_text("POM SIZE");
	set_c_button_text("BOTGUY SIZE");
	set_x_button_text("CUBE SIZE"); 

	lego.left.port = 0; // set motor ports
	lego.right.port = 2;
	
	camera_open(LOW_RES);
	camera_update();
	while (a_button() == 0) // press the a button to set the coordinates
	{
		camera_update();
		target.green.x = get_object_center(0 , 0).x; // sets target coordinates (x)
		target.green.y = get_object_center(0 , 0).y; // sets target coordinates (y)
		printf("(%d , %d)\n" , target.green.x , target.green.y);
		msleep(10);
	}
	while (b_button() == 0)
	{
		camera_update();
		target.green.size = get_object_area(0 , 0);
		target.orange.size = get_object_area(1 , 0);
		printf("Orange Size = %d" , target.orange.size);
		printf(" Green Size = %d\n" , target.green.size);
	}
	enable_servo(arm_servo); 
	enable_servo(push_servo);
	enable_servo(basket_servo);
	set_servo_position(arm_servo , ARM_UP);
	set_servo_position(push_servo , P_DOWN);
	set_servo_position(basket_servo , B_UP);
	printf("(%d , %d)\n" , target.green.x , target.green.y);
	while(a_button() == 0)
	{
		printf("%d , %d , %d\n" , get_left() , get_middle() , get_right());
	}
	while (1) // line follow until poms are seen
	{
		blob_update();
		t_line_follow();
		if (current.orange.size > target.orange.size && current.green.size > target.green.size)
			break;
	}
	blob_update(); 
	get_pom(); // pick up a pom
	pom_push(); // push it into the basket
	while (1) // turn to next pom
	{
		blob_update();
		mav(lego.left.port , 300); 
		mav(lego.right.port , -300);
		msleep(10);
		if (current.green.size > target.green.size)
			break;
	}
	blob_update();
	get_pom(); // pick up pom
	pom_push(); // push it into the basket
	avoid_cubeguy(); // avoid the cube or botguy
	avoid_booster();
	while (1)
	{
		blob_update();
		t_line_follow();
		if (current.orange.size > target.orange.size && current.green.size > target.green.size)
			break;
	}
	blob_update();
	pom_push();
	while (1)
	{
		blob_update();
		mav(lego.left.port , 300);
		mav(lego.right.port , -300);
		msleep(10);
		if (current.green.size > target.green.size)
			break;
	}
	blob_update();
	get_pom();
	pom_push();
	avoid_cubeguy();
	int start_time = seconds();
	int t;
	while (1)
	{
		t_line_follow();
		if (seconds() < start_time + t)
			break;
	}
	while (1)
	{

	}
}

void turn_right(int speed)
{
	mav(lego.left.port , speed);
	mav(lego.right.port , -speed);
}

void turn_left(int speed)
{
	mav(lego.left.port , -speed);
	mav(lego.right.port , speed);
}

inline int camera_move_x()
{
	  
	int lspeed = -100;
	int hspeed = 100;
	blob_update();
	printf("MOVING X\n");
	if (current.green.x < (target.green.x + TOL))
	{
		printf("LEFT\n");
		mav(lego.left.port , lspeed);
		mav(lego.right.port , hspeed);
		return 1;
	}
	if (current.green.x > (target.green.x - TOL))
	{
		printf("RIGHT\n");
		mav(lego.left.port , hspeed);
		mav(lego.right.port , lspeed);
		return 1;
	}
	if (current.green.x >= (target.green.x - TOL) && current.green.x <= (target.green.x + TOL))
	{
		ao();
		return 0;
	}
}

inline int camera_move_y()
{ 	
	  
	int speed = 200;
	int back = -200;
	blob_update();
	if (current.green.y > (target.green.y - TOL))
	{
		printf("TOO CLOSE\n");
		mav(lego.left.port , back);
		mav(lego.right.port , back);
		return 1;
	}
	if (current.green.y < (target.green.y + TOL))
	{
		printf("TOO FAR\n");
		mav(lego.left.port , speed);
		mav(lego.right.port , speed);
		return 1;
	}
	if (current.green.y >= (target.green.y - TOL) && current.green.y <= (target.green.y + TOL))
	{
		printf("GOLDILOCKS\n");
		ao();
		return 0;
	}
}

inline void blob_update()
{
	  
	// set config file
	camera_update();
	//update green state
	current.green.x = get_object_center(0 , 0).x;
	current.green.y = get_object_center(0 , 0).y;
	current.green.size = get_object_area(0 , 0);
	//update orange state
	current.orange.x = get_object_center(1 , 0).x;
	current.orange.y = get_object_center(1 , 0).y;
	current.orange.size = get_object_area(1 , 0);
	//update red state
	current.red.x = get_object_center(2 , 0).x;
	current.red.y = get_object_center(2 , 0).y;
	current.red.size = get_object_area(2 , 0);
	// update botguy state
	current.botguy.x = get_object_center(3 , 0).x;
	current.botguy.y = get_object_center(3 , 0).y;
	current.botguy.size = get_object_area(3 , 0);
	 
	// set config file
	camera_update();
	// update pink booster state
	current.pink.x = get_object_center(0 , 0).x;
	current.pink.y = get_object_center(0 , 0).y;
	current.pink.size = get_object_area(0 , 0);
	// update teal booster state
	current.teal.x = get_object_center(1 , 0).x;
	current.teal.y = get_object_center(1 , 0).y;
	current.teal.size = get_object_area(1 , 0);
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

inline int get_ET()
{
	return analog10(ET_s);
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

int avoid_cubeguy()
{
	
	while (1)
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , -300);
		msleep(10);
		if (get_left() < THRESH)
			break;
	}
	while (1)
	{
		t_line_follow();
		if (digital(15) == 1)
			break;
	}
	mav(lego.left.port , -300);
	mav(lego.right.port , -300);
	msleep(2500);
	while (1)
	{
		mav(lego.left.port , 300);
		mav(lego.right.port , -300);
		if (get_middle() > THRESH && get_left() > THRESH && get_right() > THRESH)
			break;
	}
	set_servo_position(arm_servo , ARM_OUT);
	msleep(500);
	while (1)
	{
		mav(lego.left.port , -300);
		mav(lego.right.port , 300);
		if (get_middle() < THRESH)
			break;
	}
	return 0;	
}

int avoid_booster()
{
	register unsigned int current_ET; // declare ET value
	while (1)
	{
		current_ET = get_ET();
		mav(lego.left.port , 100);
		mav(lego.right.port , 300);
		msleep(10); // arc until you are at the closest point to the wall (should be parallel)
		if (current_ET > get_ET())
			ao();
			mav(lego.left.port , -100);
			mav(lego.right.port , -300);
			msleep(10);
			break;
	}
	while (1)
	{
		mav(lego.left.port , 300);
		mav(lego.right.port , 300);
		msleep(10); // drive to the line
		if (get_middle() < THRESH)
			break;
	}

}

void pom_push()
{
	msleep(100);
	set_servo_position(arm_servo , ARM_DUMP);
	msleep(500);
	set_servo_position(push_servo , P_UP);
	msleep(500);
	set_servo_position(push_servo , P_DOWN);
	msleep(500);
	set_servo_position(push_servo , P_UP);
	msleep(500);
	set_servo_position(push_servo , P_DOWN);
	msleep(500);
	set_servo_position(push_servo , P_UP);
	msleep(500);
	set_servo_position(push_servo , P_DOWN);
	msleep(500);
	ao();
	blob_update();
}

inline void get_pom()
{
	  
	while (1) // position to get the pom
	{
		blob_update();
		printf("(%d , %d) , (%d , %d)\n" , target.green.x , target.green.y , current.green.x , current.green.y);
		if ((current.green.y >= (target.green.y - TOL) && (current.green.y <= target.green.y + TOL)) && (current.green.x >= (target.green.x - TOL) && (			current.green.x <= target.green.x + TOL)))
		{
			ao();
			printf("IN POS\n");
			set_servo_position(arm_servo , ARM_DOWN);
			msleep(500);
			set_servo_position(arm_servo , ARM_OUT);
			msleep(500);
			set_servo_position(arm_servo , ARM_DOWN);
			msleep(500);
			set_servo_position(arm_servo , ARM_UP);
			msleep(500);
			break;
		}
		camera_move_y();
		msleep(10);
		camera_move_x();	
		msleep(10);
	}
}

