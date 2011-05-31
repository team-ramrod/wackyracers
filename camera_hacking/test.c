#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    
    printf("first\n");
    int ret = usleep(1000000);
    printf("second\n");
    printf("ret is %d\n", ret);
    
    return 0;
}