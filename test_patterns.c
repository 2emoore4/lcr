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
static void flash_max_patterns();
static void display_vertical_line();
static void dynamic_load_test();

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
            flash_max_patterns();
        } else if (ch == '4') {
            display_vertical_line();
        } else if (ch == '5') {
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

static void flash_max_patterns() {
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\nrunning sequence with a bunch of patterns\n\n");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 100;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 250;
    patSeqSet.ExposureTime = 250;
    patSeqSet.LEDSelect = LED_BLUE;
    patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Downloading 48 1bpp images that are generated after splitting the patterns \n");
    printf("Downloading ./Images/CustSeqInput/SingleColor/00_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(0,"./Images/CustSeqInput/SingleColor/00_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/01_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(1,"./Images/CustSeqInput/SingleColor/01_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/02_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(2,"./Images/CustSeqInput/SingleColor/02_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/03_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(3,"./Images/CustSeqInput/SingleColor/03_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/04_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(4,"./Images/CustSeqInput/SingleColor/04_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/05_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(5,"./Images/CustSeqInput/SingleColor/05_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/06_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(6,"./Images/CustSeqInput/SingleColor/06_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/07_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(7,"./Images/CustSeqInput/SingleColor/07_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/08_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(8,"./Images/CustSeqInput/SingleColor/08_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/09_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(9,"./Images/CustSeqInput/SingleColor/09_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/10_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(10,"./Images/CustSeqInput/SingleColor/10_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/11_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(11,"./Images/CustSeqInput/SingleColor/11_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/12_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(12,"./Images/CustSeqInput/SingleColor/12_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/13_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(13,"./Images/CustSeqInput/SingleColor/13_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/14_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(14,"./Images/CustSeqInput/SingleColor/14_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/15_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(15,"./Images/CustSeqInput/SingleColor/15_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/16_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(16,"./Images/CustSeqInput/SingleColor/16_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/17_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(17,"./Images/CustSeqInput/SingleColor/17_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/18_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(18,"./Images/CustSeqInput/SingleColor/18_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/19_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(19,"./Images/CustSeqInput/SingleColor/19_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/20_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(20,"./Images/CustSeqInput/SingleColor/20_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/21_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(21,"./Images/CustSeqInput/SingleColor/21_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/22_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(22,"./Images/CustSeqInput/SingleColor/22_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/23_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(23,"./Images/CustSeqInput/SingleColor/23_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/24_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(24,"./Images/CustSeqInput/SingleColor/24_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/25_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(25,"./Images/CustSeqInput/SingleColor/25_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/26_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(26,"./Images/CustSeqInput/SingleColor/26_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/27_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(27,"./Images/CustSeqInput/SingleColor/27_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/28_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(28,"./Images/CustSeqInput/SingleColor/28_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/29_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(29,"./Images/CustSeqInput/SingleColor/29_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/30_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(30,"./Images/CustSeqInput/SingleColor/30_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/31_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(31,"./Images/CustSeqInput/SingleColor/31_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/32_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(32,"./Images/CustSeqInput/SingleColor/32_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/33_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(33,"./Images/CustSeqInput/SingleColor/33_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/34_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(34,"./Images/CustSeqInput/SingleColor/34_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/35_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(35,"./Images/CustSeqInput/SingleColor/35_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/36_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(36,"./Images/CustSeqInput/SingleColor/36_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/37_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(37,"./Images/CustSeqInput/SingleColor/37_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/38_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(38,"./Images/CustSeqInput/SingleColor/38_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/39_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(39,"./Images/CustSeqInput/SingleColor/39_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/40_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(40,"./Images/CustSeqInput/SingleColor/40_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/41_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(41,"./Images/CustSeqInput/SingleColor/41_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/42_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(42,"./Images/CustSeqInput/SingleColor/42_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/43_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(43,"./Images/CustSeqInput/SingleColor/43_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/44_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(44,"./Images/CustSeqInput/SingleColor/44_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/45_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(45,"./Images/CustSeqInput/SingleColor/45_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/46_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(46,"./Images/CustSeqInput/SingleColor/46_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/47_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(47,"./Images/CustSeqInput/SingleColor/47_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/47_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(48,"./Images/CustSeqInput/SingleColor/47_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/47_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(49,"./Images/CustSeqInput/SingleColor/47_PAT.bmp");

    LCR_CMD_DefinePatternBMP(50,"./Images/CustSeqInput/SingleColor/00_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/01_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(51,"./Images/CustSeqInput/SingleColor/01_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/02_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(52,"./Images/CustSeqInput/SingleColor/02_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/03_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(53,"./Images/CustSeqInput/SingleColor/03_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/04_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(54,"./Images/CustSeqInput/SingleColor/04_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/05_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(55,"./Images/CustSeqInput/SingleColor/05_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/06_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(56,"./Images/CustSeqInput/SingleColor/06_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/07_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(57,"./Images/CustSeqInput/SingleColor/07_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/08_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(58,"./Images/CustSeqInput/SingleColor/08_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/09_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(59,"./Images/CustSeqInput/SingleColor/09_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/10_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(60,"./Images/CustSeqInput/SingleColor/10_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/11_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(61,"./Images/CustSeqInput/SingleColor/11_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/12_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(62,"./Images/CustSeqInput/SingleColor/12_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/13_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(63,"./Images/CustSeqInput/SingleColor/13_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/14_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(64,"./Images/CustSeqInput/SingleColor/14_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/15_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(65,"./Images/CustSeqInput/SingleColor/15_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/16_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(66,"./Images/CustSeqInput/SingleColor/16_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/17_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(67,"./Images/CustSeqInput/SingleColor/17_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/18_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(68,"./Images/CustSeqInput/SingleColor/18_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/19_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(69,"./Images/CustSeqInput/SingleColor/19_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/20_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(70,"./Images/CustSeqInput/SingleColor/20_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/21_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(71,"./Images/CustSeqInput/SingleColor/21_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/22_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(72,"./Images/CustSeqInput/SingleColor/22_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/23_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(73,"./Images/CustSeqInput/SingleColor/23_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/24_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(74,"./Images/CustSeqInput/SingleColor/24_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/25_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(75,"./Images/CustSeqInput/SingleColor/25_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/26_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(76,"./Images/CustSeqInput/SingleColor/26_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/27_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(77,"./Images/CustSeqInput/SingleColor/27_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/28_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(78,"./Images/CustSeqInput/SingleColor/28_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/29_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(79,"./Images/CustSeqInput/SingleColor/29_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/30_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(80,"./Images/CustSeqInput/SingleColor/30_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/31_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(81,"./Images/CustSeqInput/SingleColor/31_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/32_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(82,"./Images/CustSeqInput/SingleColor/32_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/33_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(83,"./Images/CustSeqInput/SingleColor/33_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/34_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(84,"./Images/CustSeqInput/SingleColor/34_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/35_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(85,"./Images/CustSeqInput/SingleColor/35_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/36_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(86,"./Images/CustSeqInput/SingleColor/36_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/37_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(87,"./Images/CustSeqInput/SingleColor/37_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/38_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(88,"./Images/CustSeqInput/SingleColor/38_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/39_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(89,"./Images/CustSeqInput/SingleColor/39_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/40_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(90,"./Images/CustSeqInput/SingleColor/40_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/41_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(91,"./Images/CustSeqInput/SingleColor/41_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/42_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(92,"./Images/CustSeqInput/SingleColor/42_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/43_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(93,"./Images/CustSeqInput/SingleColor/43_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/44_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(94,"./Images/CustSeqInput/SingleColor/44_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/45_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(95,"./Images/CustSeqInput/SingleColor/45_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/46_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(96,"./Images/CustSeqInput/SingleColor/46_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/47_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(97,"./Images/CustSeqInput/SingleColor/47_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/47_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(98,"./Images/CustSeqInput/SingleColor/47_PAT.bmp");

    printf("Downloading ./Images/CustSeqInput/SingleColor/47_PAT.bmp\n");
    LCR_CMD_DefinePatternBMP(99,"./Images/CustSeqInput/SingleColor/47_PAT.bmp");

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

static void display_vertical_line() {
    //Set display mode to 0x00 - static image
    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00));

    LCR_CMD_DisplayStaticImage("./Images/test/line.bmp");

    uint8 ch;
    printf("enter 's' to stop sequence.\n");
    while((ch = getchar()) != 's') {
        // nothing yet.
    }
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