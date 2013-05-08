// Created on Sat May 4 2013

void create_cover();

int main()

{

    wait_for_light(4);

    printf("Hello, World!\n");

    create_connect();

    create_drive_direct(200 , 200);

    msleep(2000);

    create_spin_CCW ( 100 );

    msleep(2000);

    create_stop();

    create_drive_direct(200 , 200 );

    msleep(4000);

    create_stop();

    create_drive_direct (200 , 200);

    msleep(5000);

    create_stop();

    create_spin_CCW ( 100 );

    msleep(2000);

    create_drive_direct (200 , 200);

    msleep(3000);

    create_spin_CCW ( 100 );

    msleep(2000);

   

    create_cover();

    create_stop();

   

   

    create_disconnect();

    return 0;

}


