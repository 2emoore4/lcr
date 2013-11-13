#include <unistd.h>
#include <time.h>

#include "common.h"
#include "error.h"
#include "lcr_cmd.h"

static void mSleep(unsigned long int mSeconds);
static void flash_solid_color();
static void flash_solid_color_at_speed(int speed, int exposure_time);
static void flash_half_color();
static void flash_half_color_at_speed(int speed, int exposure_time);
static void flash_diag_pattern();
static void scan_vertical_lines();
static void scan_vert_seven();
static void dynamic_load_test();
static void scan_gray_code();

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
        } else if (ch == '3') {
            flash_diag_pattern();
        } else if (ch == '4') {
            scan_vertical_lines();
        } else if (ch == '5') {
            scan_vert_seven();
        } else if (ch == '6') {
            scan_gray_code();
	} else if (ch == '0') {
            dynamic_load_test();
        }
    }

    LCR_CMD_Close();

    return 0;
}

static void flash_solid_color() {
    int speed = 13950;
    int exposure_time = 600;
    flash_solid_color_at_speed(speed, exposure_time);

    uint8 ch;
    printf("enter 's' to stop sequence.\n");
    while((ch = getchar()) != 's') {
        if (ch == 'u') {
            speed = speed + 100;
            flash_solid_color_at_speed(speed, exposure_time);
        } else if (ch == 'y') {
            speed = speed - 100;
            flash_solid_color_at_speed(speed, exposure_time);
        } else if (ch == 'r') {
            exposure_time = exposure_time + 100;
            flash_solid_color_at_speed(speed, exposure_time);
        } else if (ch == 'e') {
            exposure_time = exposure_time - 100;
            flash_solid_color_at_speed(speed, exposure_time);
        } else if (ch == 'j') {
            speed = speed + 50;
            flash_solid_color_at_speed(speed, exposure_time);
        } else if (ch == 'h') {
            speed = speed - 50;
            flash_solid_color_at_speed(speed, exposure_time);
        }
    }

    LCR_CMD_StartPatternSeq(0); //Stop pattern sequence

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}

static void flash_solid_color_at_speed(int speed, int exposure_time) {
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\nrunning flash_solid_color demo\n\n");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 2;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = speed;
    patSeqSet.ExposureTime = exposure_time;
    patSeqSet.LEDSelect = LED_GREEN;
    patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Downloading pat0\n");
    LCR_CMD_DefinePatternBMP(0,"./Images/test/white.bmp");

    printf("Downloading pat1...\n");
    LCR_CMD_DefinePatternBMP(1,"./Images/test/black.bmp");

    printf("Starting Pattern Sequence at speed %d, and exposure %d\n", speed, exposure_time);
    LCR_CMD_StartPatternSeq(1);
}

static void flash_half_color() {
    int speed = 13950;
    int exposure_time = 600;
    flash_half_color_at_speed(speed, exposure_time);

    uint8 ch;
    printf("enter 's' to stop sequence.\n");
    while((ch = getchar()) != 's') {
        if (ch == 'u') {
            speed = speed + 100;
            flash_half_color_at_speed(speed, exposure_time);
        } else if (ch == 'y') {
            speed = speed - 100;
            flash_half_color_at_speed(speed, exposure_time);
        } else if (ch == 'r') {
            exposure_time = exposure_time + 100;
            flash_half_color_at_speed(speed, exposure_time);
        } else if (ch == 'e') {
            exposure_time = exposure_time - 100;
            flash_half_color_at_speed(speed, exposure_time);
        } else if (ch == 'j') {
            speed = speed + 50;
            flash_half_color_at_speed(speed, exposure_time);
        } else if (ch == 'h') {
            speed = speed - 50;
            flash_half_color_at_speed(speed, exposure_time);
        }
    }

    LCR_CMD_StartPatternSeq(0); //Stop pattern sequence

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}

static void flash_half_color_at_speed(int speed, int exposure_time) {
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\nrunning flash_half_color demo for ten seconds\n\n");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 2;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = speed;
    patSeqSet.ExposureTime = exposure_time;
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

static void flash_diag_pattern() {
    LCR_PatternSeqSetting_t patSeqSet;

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 2;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 13950;
    patSeqSet.ExposureTime = 600;
    patSeqSet.LEDSelect = LED_GREEN;
    patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Downloading pat0\n");
    LCR_CMD_DefinePatternBMP(0,"./Images/test/diag1.bmp");

    printf("Downloading pat1...\n");
    LCR_CMD_DefinePatternBMP(1,"./Images/test/diag2.bmp");

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

static void scan_vertical_lines() {
    LCR_PatternSeqSetting_t patSeqSet;

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    int frame_count = 96;
    int multiplier = 608 / frame_count;
    int frame_duration = 250000;

    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = frame_count;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = frame_duration;
    patSeqSet.ExposureTime = frame_duration;
    patSeqSet.LEDSelect = LED_GREEN;
    patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    int i;
    for (i = 0; i < frame_count; i++) {
        char buf[35];
        sprintf(buf, "./Images/test/vertical/line%d.bmp", i * multiplier);
        printf("downloading pattern %s\n", buf);
        LCR_CMD_DefinePatternBMP(i, buf);
    }

    LCR_CMD_StartPatternSeq(1);

    uint8 ch;
    printf("enter 's' to stop sequence.\n");
    while((ch = getchar()) != 's') {
        // nothing yet.
    }

    LCR_CMD_StartPatternSeq(0);

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00));
}

static void scan_vert_seven() {
    LCR_PatternSeqSetting_t patSeqSet;

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 86;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 25000;
    patSeqSet.ExposureTime = 25000;
    patSeqSet.LEDSelect = LED_GREEN;
    patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    int i;
    for (i = 0; i < 86; i++) {
        char buf[37];
        sprintf(buf, "./Images/test/vert_seven/line%d.bmp", i);
        printf("downloading pattern %s\n", buf);
        LCR_CMD_DefinePatternBMP(i, buf);
    }

    LCR_CMD_StartPatternSeq(1);

    uint8 ch;
    printf("enter 's' to stop sequence.\n");
    while((ch = getchar()) != 's') {
        // nothing yet.
    }

    LCR_CMD_StartPatternSeq(0);

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00));
}

static void scan_gray_code() {
    LCR_PatternSeqSetting_t patSeqSet;

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 4;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 250000;
    patSeqSet.ExposureTime = 250000;
    patSeqSet.LEDSelect = LED_GREEN;
    patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    LCR_CMD_DefinePatternBMP(0, "Images/test/code/frame00.bmp");
    LCR_CMD_DefinePatternBMP(0, "Images/test/code/frame01.bmp");
    LCR_CMD_DefinePatternBMP(0, "Images/test/code/frame02.bmp");
    LCR_CMD_DefinePatternBMP(0, "Images/test/code/frame03.bmp");

    LCR_CMD_StartPatternSeq(1);

    uint8 ch;
    printf("enter 's' to stop sequence.\n");
    while((ch = getchar()) != 's') {}

    LCR_CMD_StartPatternSeq(0);
    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00));
}

static void dynamic_load_test() {
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\nrunning flash_solid_color demo\n\n");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 2;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 500000;
    patSeqSet.ExposureTime = 500000;
    patSeqSet.LEDSelect = LED_GREEN;
    patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Downloading pat0\n");
    LCR_CMD_DefinePatternBMP(0,"./Images/test/white.bmp");

    printf("Downloading pat1...\n");
    LCR_CMD_DefinePatternBMP(1,"./Images/test/black.bmp");

    LCR_CMD_DefinePatternBMP(2, "./Images/test/whitetop.bmp");
    LCR_CMD_DefinePatternBMP(3, "./Images/test/whitebottom.bmp");

    LCR_CMD_StartPatternSeq(1);

    mSleep(5000);

    clock_t begin, end;
    double time_spent;

    begin = clock();

    patSeqSet.NumPatterns = 4;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);
    printf("Downloading pat3...\n");
    LCR_CMD_StartPatternSeq(1);

    end = clock();

    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("loading of an extra 2 frames took %f seconds\n", time_spent);

    uint8 ch;
    printf("enter 's' to stop sequence.\n");
    while((ch = getchar()) != 's') {
        // nothing yet.
    }

    LCR_CMD_StartPatternSeq(0); //Stop pattern sequence

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}

/*Function is used to create delay in mSeconds */
/* This is used in demo code to show code flow */
static void mSleep(unsigned long int mSeconds)
{
    /*Add logic and call system function that will create mSeconds millisecond delay */
    sleep((mSeconds/1000.0));
}
