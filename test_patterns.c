#include <unistd.h>

#include "common.h"
#include "error.h"
#include "lcr_cmd.h"

static void mSleep(unsigned long int mSeconds);

/* main() function */
int main(int argc, char *argv[])
{
    uint8 ch;

    //Connect to the LCr H/W via TCP interface
    if(LCR_CMD_Open())
    {
        printf("Unable to connect to the DLP LCr\n");
        return -1;
    }

    printf("this doesn't do anything yet. press 'x' to quit.\n");

    while((ch=getchar()) != 'x') {

    }

    LCR_CMD_Close();

    return 0;
}

/*Function is used to create delay in mSeconds */
/* This is used in demo code to show code flow */
static void mSleep(unsigned long int mSeconds)
{
    /*Add logic and call system function that will create mSeconds millisecond delay */
    sleep((mSeconds/1000.0));
}