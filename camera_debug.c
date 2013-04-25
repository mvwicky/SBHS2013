// Created on Wed April 24 2013

int main()
{
	camera_open(LOW_RES);
	while (a_button() == 0)
	{
		camera_update();
		printf("Orange = %d\n" , get_object_area(1 , 0));
		printf("Green = %d\n" , get_object_area(0 , 0));
		msleep(100);
	}
	return 0;
}
