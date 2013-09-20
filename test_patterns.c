#include <unistd.h>

#include "common.h"
#include "error.h"
#include "lcr_cmd.h"

static void flash_solid_color();
static void flash_half_color();

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

    printf("press 'x' to quit, enter '1' to flash a solid frame, enter '2' to flash a half-filled-frame.\n");

    while((ch = getchar()) != 'x') {
        if (ch == '1') {
            flash_solid_color();
        } else if (ch == '2') {
            flash_half_color();
        }
    }

    LCR_CMD_Close();

    return 0;
}

static void flash_solid_color() {
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\nrunning flash_solid_color demo for ten seconds\n\n");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    printf("Selecting BIT_DEPTH = 1, NUM_PAT = 2, TRIGGER_TYPE = AUTO, AUTO_TRIG_PEIORD = 33334uSec, EXPOSURE = 33334uSec, LED = GREEN\n");
    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 2;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 8333;
    patSeqSet.ExposureTime = 8333;
    patSeqSet.LEDSelect = LED_GREEN;
    patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Downloading pat0\n");
    LCR_CMD_DefinePatternBMP(0,"./Images/test/white.bmp");

    printf("Downloading pat1...\n");
    LCR_CMD_DefinePatternBMP(1,"./Images/test/black.bmp");

    printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);

    uint8 ch;
    printf("enter 's' to stop sequence.\n");
    while((ch = getchar()) != 's') {
        // nothing yet.
    }

    LCR_CMD_StartPatternSeq(0); //Stop pattern sequence

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}

static void flash_half_color() {
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\nrunning flash_half_color demo for ten seconds\n\n");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    printf("Selecting BIT_DEPTH = 1, NUM_PAT = 2, TRIGGER_TYPE = AUTO, AUTO_TRIG_PEIORD = 33334uSec, EXPOSURE = 33334uSec, LED = GREEN\n");
    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 2;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 8333;
    patSeqSet.ExposureTime = 8333;
    patSeqSet.LEDSelect = LED_GREEN;
    patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Downloading pat0\n");
    LCR_CMD_DefinePatternBMP(0,"./Images/test/half1.bmp");

    printf("Downloading pat1...\n");
    LCR_CMD_DefinePatternBMP(1,"./Images/test/half2.bmp");

    printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);

    uint8 ch;
    printf("enter 's' to stop sequence.\n");
    while((ch = getchar()) != 's') {
        // nothing yet.
    }

    LCR_CMD_StartPatternSeq(0); //Stop pattern sequence

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}