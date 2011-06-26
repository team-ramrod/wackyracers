#include <stdio.h>

#include "camera.h"

int main(int argc, char *argv[])
{
    printf("testing");
    int testcam = camera_init("/dev/ttyU1");

    camera_reset(testcam);

    camera_set_baud(testcam, 9600);

    reconfigure_baud(testcam, 9600);
    
    camera_snap(testcam);

    int filesize = camera_get_filesize(testcam);
    fprintf(stderr, "filesize = %d\n", filesize);

    int i = 0;
    for (i = 0; i < 800; i++) {
        char p;
        printf("pause: ");
        scanf("%c", &p);
        int x = camera_get_block(testcam, 16*i, 16);
        while (x == -1) {
            x = camera_get_block(testcam, 16*i, 16);
        }
        printf("---------------------i = %d\n", i);
    }



    //camera_get_image(testcam, filesize);

    //camera_stop(testcam);
    
    //camera_close(testcam);
    
    return 0;
}