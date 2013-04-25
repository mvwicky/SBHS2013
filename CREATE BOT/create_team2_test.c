
int main()
{
	create_connect();
	create_drive_direct(-500 , -500);
	msleep(2000);
	create_drive_direct(300 , -300);
	msleep(500);
	create_disconnect();
}
