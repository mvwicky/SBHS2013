// Created on Tue April 16 2013

int main()
{
	int rport = 7, leftmtr = 0, rghtmtr = 3, fport = 2; 
	int threshold = 250; 
	int high = 100,low = -20; 
	printf("Line following: position robot on tape\n");
	printf("Press B button when ready\n\nPress side button to stop\n");
	while(b_button()==0); 
	while(side_button()==0)
	{ 
		while (analog10(rport) > threshold) 
		{ 
			motor(leftmtr,low); 
			motor(rghtmtr,high); 
			if (side_button()!=0) 
				break; 
			if (analog10(fport) < threshold)
				break;
		} 
		while (analog10(rport) <= threshold)
		{	
			motor(leftmtr,high); 
			motor(rghtmtr,low); 
			if (side_button()!=0) 
				break;
			if (analog10(fport) < threshold)
				break;
			//msleep(35500);
		} 
		if (analog10(fport) < threshold)
		{
			motor(rghtmtr , 100);
			motor(leftmtr , -50);
			msleep(1000);
		}
	}
	ao(); 
	printf("how about that\n");
return 0;//PaTTY
}
