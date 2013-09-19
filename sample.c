/*****************************************************************************
**             TEXAS INSTRUMENTS PROPRIETARY INFORMATION
**
**  (c) Copyright, Texas Instruments Incorporated, 2012-2013.
**      All Rights Reserved.
**
**  Property of Texas Instruments Incorporated. Restricted Rights -
**  Use, duplication, or disclosure is subject to restrictions set
**  forth in TI's program license agreement and associated documentation.
******************************************************************************/
/**
*
* @file    main.c
*
* @brief	This file contatins main() function from where the DM365 Command Interfaces
*			can be called.
*			This file also contain demo example functions which shows how the APIs can be
*			called in sequence.
**/
/*****************************************************************************/
#include <unistd.h>

#include "common.h"
#include "error.h"
#include "lcr_cmd.h"

static void mSleep(unsigned long int mSeconds);

//Demo code show how the DM365 command interfaces can be called.
static void Demo_1_ReadLightCrafterVersion();
static void Demo_2_AdjustLEDCurrent();
static void Demo_3_StaticImageDownload();
static void Demo_4_DisplayModeChange();
static void Demo_5_1BppPatternSeqDemo();
static void Demo_6_8BppPatternSeqDemo();
static void Demo_7_InternalHWTestPatternModeDemo();
static void Demo_8_VariableBitDepthPatSeqDemo();
static void Demo_9_SolutionDemo();
static void do_cool_stuff();

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

    printf("**********************************************************************************\n");
    printf("Select '1' thru '9' to run a demo, Select '0' to run 'All' demo, and 'x' to 'exit'\n");
    printf("**********************************************************************************\n");
    printf("Demo#1 - Reading DLP LightCrafter Version information\n");
    printf("Demo#2 - Test LED Currents\n");
    printf("Demo#3 - Downloading 24bpp image\n");
    printf("Demo#4 - Switching between Display modes - StaticImage/InternalTestPattern/ExternVideo/PatternSequence\n");
    printf("Demo#5 - Configuring 1bpp pattern Sequence\n");
    printf("Demo#6 - Configuring 8bpp pattern Sequence\n");
	printf("Demo#7 - Run Internal Hardware test patterns\n");
	printf("Demo#8 - Custom Sequence configuration\n");
    printf("9 - do cool stuff\n\n");

    while((ch=getchar()) != 'x')
    {

        switch(ch)
        {
        case '1':
            Demo_1_ReadLightCrafterVersion();
            break;
        case '2':
            Demo_2_AdjustLEDCurrent();
            break;
        case '3':
            Demo_3_StaticImageDownload();
            break;
        case '4':
            Demo_4_DisplayModeChange();
            break;
        case '5':
            Demo_5_1BppPatternSeqDemo();
            break;
        case '6':
            Demo_6_8BppPatternSeqDemo();
            break;
        case '7':
            Demo_7_InternalHWTestPatternModeDemo();
            break;
        case '8':
            Demo_8_VariableBitDepthPatSeqDemo();
            break;
        case '9':
            do_cool_stuff();
            break;
        case '0':
            Demo_1_ReadLightCrafterVersion();
            Demo_2_AdjustLEDCurrent();
            Demo_3_StaticImageDownload();
            Demo_4_DisplayModeChange();
            //Demo_5_1BppPatternSeqDemo(); This will be called in Demo_4_DisplayModeChange interface
            Demo_6_8BppPatternSeqDemo();
            Demo_7_InternalHWTestPatternModeDemo();
			Demo_8_VariableBitDepthPatSeqDemo();
			Demo_9_SolutionDemo();
            break;
        default:
            break;
        }

        printf("**********************************************************************************\n");
        printf("Select '1' thru '9' to run a demo, Select '0' to run 'All' demo, and 'x' to 'exit'\n");
        printf("**********************************************************************************\n");
    }

    LCR_CMD_Close();

    return 0;
}

/*Demo-1: How to read the LightCrafter Version */
static void Demo_1_ReadLightCrafterVersion()
{
    char verName[LCR_CMD_VERSION_STR_LEN];

    printf("\n\n****Running Demo #1 - Reading DLP LightCrafter(TM) Software version(s)****\n\n");

    LCR_CMD_GetRevision(REV_DM365,&verName[0]);
    printf("DM365 Revision: %s\n",verName);

    LCR_CMD_GetRevision(REV_FPGA,&verName[0]);
    printf("FPGA Revision: %s\n",verName);

    LCR_CMD_GetRevision(REV_MSP430,&verName[0]);
    printf("MSP430 Revision: %s\n",verName);

}


/*Demo-2: How to adjust LED currents */
static void Demo_2_AdjustLEDCurrent()
{
    //Test for LED current update
    LCR_LEDCurrent_t LEDSetting;
    LCR_LEDCurrent_t tempLEDSetting;

    printf("\n\n****Running Demo #2 - Adjusting LED Currents****\n\n");

    if(LCR_CMD_GetLEDCurrent(&LEDSetting))
    {
        printf("Couldn't read the LED Current setting\n");
    }
    else
    {
        printf("Existing LED Current setting Red = %d\tGreen = %d\tBlue = %d\n",LEDSetting.Red,LEDSetting.Green,LEDSetting.Blue);
        printf("Set MIN 0x00 LED setting\n");
        tempLEDSetting.Red = 0;
        tempLEDSetting.Green = 0;
        tempLEDSetting.Blue = 0;
        if(LCR_CMD_SetLEDCurrent(&tempLEDSetting))
        {
            printf("Couldn't set the LED Currents\n");
        }
        else
        {
            mSleep(3000);
            printf("Set MAX 0x3FF LED setting\n");
            tempLEDSetting.Red = 0x3FF;
            tempLEDSetting.Green = 0x3FF;
            tempLEDSetting.Blue = 0x3FF;
            if(LCR_CMD_SetLEDCurrent(&tempLEDSetting))
            {
                printf("Couldn't set the LED Currents\n");
            }
            else
            {
                mSleep(3000);
                printf("Set default values of LED setting\n");
                LCR_CMD_SetLEDCurrent(&LEDSetting);
            }
        }

    }
}

/* Demo-3: How to download a 24bpp static image */
static void Demo_3_StaticImageDownload()
{
    printf("\n\n****Running Demo #3 - Downloading 24bpp static image****\n\n");
    //Set display mode to 0x00 - static image
    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00));

    printf("Downloading RGB888 Color Ramp Image \n");
    LCR_CMD_DisplayStaticImage("./Images/StaticImages/Color_RGB888.bmp");
    mSleep(3000);

    printf("Downloading LightCrafter image\n");
    LCR_CMD_DisplayStaticImage("./Images/StaticImages/LightCrafter.bmp");
    mSleep(3000);

    printf("Downloading Mangrove\n");
    LCR_CMD_DisplayStaticImage("./Images/StaticImages/Mangrove.bmp");
    mSleep(3000);

    printf("Displaying static pleasing color...\n");
    uint32 color;
    color = 0x00FFFFFF;
    LCR_CMD_DisplayStaticColor(color);
    mSleep(3000);

    printf("Downloading RGB888 ramp again...\n");
    LCR_CMD_DisplayStaticImage("./Images/StaticImages/Color_RGB888.bmp");
}

/* Demo-4: Cycle through Display Modes */
static void Demo_4_DisplayModeChange()
{
    printf("\n\n****Running Demo #4 - Display Mode changine****\n\n");

    LCR_DisplayMode_t Mode;
    Mode = LCR_CMD_GetDisplayMode();
    printf("Default display Mode: %d\n",Mode);

    printf("Setting Display Mode = Static Image\n");
    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00));
    mSleep(3000);

    printf("Setting Display Mode = Internal Test Pattern\n");
    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x01));
    mSleep(3000);

    printf("Setting Display Mode = HDMI Video, if HDMI is connected to a source you will get video\n");
    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x02));
    mSleep(3000);

    printf("Setting Display Mode = Pattern Seq Mode\n");
    Demo_5_1BppPatternSeqDemo();
}

/* Demo-5: Pattern Sequence Demo (1BPP) */
static void Demo_5_1BppPatternSeqDemo()
{
    uint8 i;
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\n****Running Demo #5 - 1bpp Pattern Sequence ****\n\n");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    printf("Selecting BIT_DEPTH = 1, NUM_PAT = 8, TRIGGER_TYPE = AUTO, AUTO_TRIG_PEIORD = 33334uSec, EXPOSURE = 33334uSec, LED = GREEN\n");
    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 8;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 33334;
    patSeqSet.ExposureTime = 33334;
    patSeqSet.LEDSelect = LED_GREEN;
	patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Downloading pat0\n");
    LCR_CMD_DefinePatternBMP(0,"./Images/PatSeqImages/pattern_1_00.bmp");

    printf("Downloading pat1...\n");
    LCR_CMD_DefinePatternBMP(1,"./Images/PatSeqImages/pattern_1_01.bmp");

    printf("Downloading pat2...\n");
    LCR_CMD_DefinePatternBMP(2,"./Images/PatSeqImages/pattern_1_02.bmp");

    printf("Downloading pat3...\n");
    LCR_CMD_DefinePatternBMP(3,"./Images/PatSeqImages/pattern_1_03.bmp");

    printf("Downloading pat4...\n");
    LCR_CMD_DefinePatternBMP(4,"./Images/PatSeqImages/pattern_1_04.bmp");

    printf("Downloading pat5...\n");
    LCR_CMD_DefinePatternBMP(5,"./Images/PatSeqImages/pattern_1_05.bmp");

    printf("Downloading pat6...\n");
    LCR_CMD_DefinePatternBMP(6,"./Images/PatSeqImages/pattern_1_06.bmp");

    printf("Downloading pat7...\n");
    LCR_CMD_DefinePatternBMP(7,"./Images/PatSeqImages/pattern_1_07.bmp");

    printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);
    mSleep(10000);
    LCR_CMD_StartPatternSeq(0); //Stop pattern sequence

    printf("Changing Exposure Time and Trigger time AUTO_TRIG_PEIORD = 250uSec, EXPOSURE = 250uSec LED = RED\n");
    LCR_CMD_GetPatternSeqSetting(&patSeqSet);
    patSeqSet.AutoTriggerPeriod = 250; //250uSec
    patSeqSet.ExposureTime = 250; //250uSec
	patSeqSet.LEDSelect = LED_RED;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);
    mSleep(10000);

    printf("Changing Auto Trigger -> SW Trigger Mode\n");
	printf("Changing Exposure Time and Trigger time AUTO_TRIG_PEIORD = 1000000uSec, EXPOSURE = 1000000uSec LED = BLUE\n");
    LCR_CMD_GetPatternSeqSetting(&patSeqSet);
    patSeqSet.AutoTriggerPeriod = 1000000;
    patSeqSet.ExposureTime = 1000000;
	patSeqSet.LEDSelect = LED_BLUE;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_SW;//Change input trigger to SW
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);

    i = 8;
    while(i--)
    {
        LCR_CMD_AdvancePatternSeq();
        mSleep(1000); //wait for 1Second
    }

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}

/* Demo-6: Pattern Sequence Demo (1BPP)  */
static void Demo_6_8BppPatternSeqDemo()
{
    uint8 i;
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\n****Running Demo #6 - 8bpp Pattern Sequence ****\n\n");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

	printf("Selecting BIT_DEPTH = 8, NUM_PAT = 3, TRIGGER_TYPE = AUTO, AUTO_TRIG_PEIORD = 8700uSec, EXPOSURE = 8333, LED = BLUE\n");
    patSeqSet.BitDepth = 8;
    patSeqSet.NumPatterns = 3;
	patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
	patSeqSet.InputTriggerDelay = 0;
    patSeqSet.AutoTriggerPeriod = 8700;
    patSeqSet.ExposureTime = 8333;
    patSeqSet.LEDSelect = LED_BLUE;
	patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Downloading pat0\n");
    LCR_CMD_DefinePatternBMP(0,"./Images/PatSeqImages/pattern_8_00.bmp");

    printf("Downloading pat1...\n");
    LCR_CMD_DefinePatternBMP(1,"./Images/PatSeqImages/pattern_8_01.bmp");

    printf("Downloading pat2...\n");
    LCR_CMD_DefinePatternBMP(2,"./Images/PatSeqImages/pattern_8_02.bmp");

	printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);
    mSleep(10000);
    LCR_CMD_StartPatternSeq(0); //Stop pattern sequence

    printf("Changing Exposure Time and Trigger time AUTO_TRIG_PEIORD = 10000uSec, EXPOSURE = 10000uSec LED = GREEN\n");
    LCR_CMD_GetPatternSeqSetting(&patSeqSet);
    patSeqSet.AutoTriggerPeriod = 10000; //uSec
    patSeqSet.ExposureTime = 10000; //uSec
	patSeqSet.LEDSelect = LED_GREEN;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);
    mSleep(10000);
	LCR_CMD_StartPatternSeq(0);

    printf("Changing Auto Trigger -> SW Trigger Mode\n");
	printf("Changing Exposure Time and Trigger time AUTO_TRIG_PEIORD = 20000uSec, EXPOSURE = 20000uSec LED = RED\n");
    LCR_CMD_GetPatternSeqSetting(&patSeqSet);
    patSeqSet.AutoTriggerPeriod = 20000;
    patSeqSet.ExposureTime = 20000;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_SW;//Change input trigger to SW
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);

    i = 3;
    while(i--)
    {
        LCR_CMD_AdvancePatternSeq();
        mSleep(1000);
    }

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}

/* Demo-7: Internal HW Pattern Sequence Demo (1BPP)*/
static void Demo_7_InternalHWTestPatternModeDemo()
{
    uint8 i;
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\n****Running Demo #7 - Internal 1bpp HW Pattern Sequence ****\n\n");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

	printf("Selecting BIT_DEPTH = 1, NUM_PAT = 32, TRIGGER_TYPE = AUTO, AUTO_TRIG_PEIORD = 16667uSec, EXPOSURE = 16667uSec, LED = BLUE\n");
    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 32;
	patSeqSet.PatternType = PTN_TYPE_HW;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
	patSeqSet.InputTriggerDelay = 0;
    patSeqSet.AutoTriggerPeriod = 16667;
    patSeqSet.ExposureTime = 16667;
    patSeqSet.LEDSelect = LED_BLUE;
	patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

	//Define Hardware Patterns
	LCR_HWPatternSeqDef_t hwPatDef;
	hwPatDef.index = 0;
	hwPatDef.numOfPatn = 32;
	//First Sequence without invert
	for(i=0; i<hwPatDef.numOfPatn/2;i++)
	{
		hwPatDef.hwPatArray[i].Number = i;
		hwPatDef.hwPatArray[i].Invert = 0;
	}
	//2nd sequence with invert
	for( i=16; i<hwPatDef.numOfPatn;i++)
	{
		hwPatDef.hwPatArray[i].Number = (i-16);
		hwPatDef.hwPatArray[i].Invert = 1;
	}
	LCR_CMD_DefineHWPatSequence(&hwPatDef);

	printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);
    mSleep(10000);
    LCR_CMD_StartPatternSeq(0); //Stop pattern sequence

    printf("Changing Exposure Time and Trigger time AUTO_TRIG_PEIORD = 100000uSec, EXPOSURE = 100000uSec LED = RED...\n");
    LCR_CMD_GetPatternSeqSetting(&patSeqSet);
    patSeqSet.AutoTriggerPeriod = 100000; //uSec
    patSeqSet.ExposureTime = 100000; //uSec
    patSeqSet.LEDSelect = LED_RED;
	LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);
    mSleep(10000);
	LCR_CMD_StartPatternSeq(0); //Stop pattern sequence

	printf("Changing Auto Trigger -> SW Trigger Mode\n");
    LCR_CMD_GetPatternSeqSetting(&patSeqSet);
    printf("Changing Exposure Time and Trigger time AUTO_TRIG_PEIORD = 1000000uSec, EXPOSURE = 1000000uSec LED = GREEN...\n");
    patSeqSet.AutoTriggerPeriod = 1000000; //uSec
    patSeqSet.ExposureTime = 1000000; //uSec
	patSeqSet.InputTriggerType = TRIGGER_TYPE_SW;//Change input trigger to SW
    patSeqSet.LEDSelect = LED_GREEN;
	LCR_CMD_SetPatternSeqSetting(&patSeqSet);

	printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);

    i = 32;
    while(i--)
    {
        LCR_CMD_AdvancePatternSeq();
        mSleep(1000);
    }

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}

/*Demo-8: Demostrates how to configure hardware for running custom sequence. */
static void Demo_8_VariableBitDepthPatSeqDemo()
{
    //Create a 1bpp pat sequence and store it as a solution
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\n****Running Demo #8 - Running custom sequence ****\n\n");

	//Refer to ../Images/CustSeqInput/SingleColor/22Mar2013_12_17_PM_Readme.txt for details
	printf(">>>>>>>Running Variable bit-depth pattern sequence with GREEN color<<<<<<<\n");
	LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));
    LCR_CMD_GetPatternSeqSetting(&patSeqSet);
    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 48;
	patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 8333;
    patSeqSet.ExposureTime = 8842;
    patSeqSet.LEDSelect = LED_DEFAULT; //For configuring custom sequence the value must be set to default
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);
	//Load the custom sequence and set the vector table
	printf("Loading custom sequence ./Images/CustSeqInput/SingleColor/22Mar2013_12_17_PM.bin\n");
	LCR_CMD_LoadCustomSequence("./Images/CustSeqInput/SingleColor/22Mar2013_12_17_PM.bin");
	uint8 startvector = 0;
	uint8 numOfvectors = 10;
	printf("Setting up Start and Number of vectors\n");
	LCR_CMD_SetupCustomSequencevectors(startvector,numOfvectors);

    //Load the 48 1bpp images that are generated after splitting the patterns
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
	printf("Starting the sequence\n");
	LCR_CMD_StartPatternSeq(1);
	mSleep(10000);
	printf("Stoping the sequence\n");

	//Refer to ..\Images\CustSeqInput\MultiColor\22Mar2013_12_18_PM_Readme.txt for details
	printf("\n>>>>>>>Running Variable bit-depth pattern sequence with individual pattern color selection color<<<<<<<\n");
	LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));
    LCR_CMD_GetPatternSeqSetting(&patSeqSet);
    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 48;
	patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 20000;
    patSeqSet.ExposureTime = 20360;
    patSeqSet.LEDSelect = LED_DEFAULT; //For configuring custom sequence the value must be set to default
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);
	//Load the custom sequence and set the vector table
	printf("Loading custom sequence ../Images/CustSeqInput/MultiColor/22Mar2013_12_18_PM.bin\n");
	LCR_CMD_LoadCustomSequence("./Images/CustSeqInput/MultiColor/22Mar2013_12_18_PM.bin");
	startvector = 0;
	numOfvectors = 10;
	printf("Setting up Start and Number of vectors\n");
	LCR_CMD_SetupCustomSequencevectors(startvector,numOfvectors);

    //Load the 48 1bpp images that are generated after splitting the patterns
	printf("Downloading 48 1bpp images that are generated after splitting the patterns \n");
	printf("Downloading ./Images/CustSeqInput/MultiColor/00_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(0,"./Images/CustSeqInput/MultiColor/00_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/01_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(1,"./Images/CustSeqInput/MultiColor/01_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/02_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(2,"./Images/CustSeqInput/MultiColor/02_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/03_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(3,"./Images/CustSeqInput/MultiColor/03_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/04_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(4,"./Images/CustSeqInput/MultiColor/04_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/05_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(5,"./Images/CustSeqInput/MultiColor/05_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/06_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(6,"./Images/CustSeqInput/MultiColor/06_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/07_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(7,"./Images/CustSeqInput/MultiColor/07_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/08_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(8,"./Images/CustSeqInput/MultiColor/08_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/09_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(9,"./Images/CustSeqInput/MultiColor/09_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/10_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(10,"./Images/CustSeqInput/MultiColor/10_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/11_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(11,"./Images/CustSeqInput/MultiColor/11_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/12_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(12,"./Images/CustSeqInput/MultiColor/12_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/13_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(13,"./Images/CustSeqInput/MultiColor/13_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/14_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(14,"./Images/CustSeqInput/MultiColor/14_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/15_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(15,"./Images/CustSeqInput/MultiColor/15_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/16_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(16,"./Images/CustSeqInput/MultiColor/16_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/17_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(17,"./Images/CustSeqInput/MultiColor/17_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/18_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(18,"./Images/CustSeqInput/MultiColor/18_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/19_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(19,"./Images/CustSeqInput/MultiColor/19_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/20_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(20,"./Images/CustSeqInput/MultiColor/20_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/21_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(21,"./Images/CustSeqInput/MultiColor/21_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/22_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(22,"./Images/CustSeqInput/MultiColor/22_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/23_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(23,"./Images/CustSeqInput/MultiColor/23_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/24_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(24,"./Images/CustSeqInput/MultiColor/24_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/25_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(25,"./Images/CustSeqInput/MultiColor/25_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/26_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(26,"./Images/CustSeqInput/MultiColor/26_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/27_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(27,"./Images/CustSeqInput/MultiColor/27_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/28_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(28,"./Images/CustSeqInput/MultiColor/28_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/29_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(29,"./Images/CustSeqInput/MultiColor/29_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/30_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(30,"./Images/CustSeqInput/MultiColor/30_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/31_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(31,"./Images/CustSeqInput/MultiColor/31_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/32_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(32,"./Images/CustSeqInput/MultiColor/32_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/33_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(33,"./Images/CustSeqInput/MultiColor/33_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/34_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(34,"./Images/CustSeqInput/MultiColor/34_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/35_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(35,"./Images/CustSeqInput/MultiColor/35_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/36_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(36,"./Images/CustSeqInput/MultiColor/36_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/37_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(37,"./Images/CustSeqInput/MultiColor/37_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/38_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(38,"./Images/CustSeqInput/MultiColor/38_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/39_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(39,"./Images/CustSeqInput/MultiColor/39_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/40_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(40,"./Images/CustSeqInput/MultiColor/40_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/41_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(41,"./Images/CustSeqInput/MultiColor/41_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/42_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(42,"./Images/CustSeqInput/MultiColor/42_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/43_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(43,"./Images/CustSeqInput/MultiColor/43_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/44_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(44,"./Images/CustSeqInput/MultiColor/44_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/45_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(45,"./Images/CustSeqInput/MultiColor/45_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/46_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(46,"./Images/CustSeqInput/MultiColor/46_PAT.bmp");

	printf("Downloading ./Images/CustSeqInput/MultiColor/47_PAT.bmp\n");
	LCR_CMD_DefinePatternBMP(47,"./Images/CustSeqInput/MultiColor/47_PAT.bmp");
	printf("Starting the sequence\n");
	LCR_CMD_StartPatternSeq(1);
	mSleep(10000);
	printf("Stoping the sequence\n");

	LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}

// my own demo
static void do_cool_stuff() {
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\n****Running Demo #5 - 1bpp Pattern Sequence ****\n\n");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));

    printf("Selecting BIT_DEPTH = 1, NUM_PAT = 8, TRIGGER_TYPE = AUTO, AUTO_TRIG_PEIORD = 33334uSec, EXPOSURE = 33334uSec, LED = GREEN\n");
    patSeqSet.BitDepth = 1;
    patSeqSet.NumPatterns = 8;
    patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 33334;
    patSeqSet.ExposureTime = 33334;
    patSeqSet.LEDSelect = LED_GREEN;
    patSeqSet.Repeat = 0;
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);

    printf("Downloading pat0\n");
    LCR_CMD_DefinePatternBMP(0,"./Images/PatSeqImages/pattern_1_00.bmp");

    printf("Downloading pat1...\n");
    LCR_CMD_DefinePatternBMP(1,"./Images/PatSeqImages/pattern_1_01.bmp");

    printf("Downloading pat2...\n");
    LCR_CMD_DefinePatternBMP(2,"./Images/PatSeqImages/pattern_1_02.bmp");

    printf("Downloading pat3...\n");
    LCR_CMD_DefinePatternBMP(3,"./Images/PatSeqImages/pattern_1_03.bmp");

    printf("Downloading pat4...\n");
    LCR_CMD_DefinePatternBMP(4,"./Images/PatSeqImages/pattern_1_04.bmp");

    printf("Downloading pat5...\n");
    LCR_CMD_DefinePatternBMP(5,"./Images/PatSeqImages/pattern_1_05.bmp");

    printf("Downloading pat6...\n");
    LCR_CMD_DefinePatternBMP(6,"./Images/PatSeqImages/pattern_1_06.bmp");

    printf("Downloading pat7...\n");
    LCR_CMD_DefinePatternBMP(7,"./Images/PatSeqImages/pattern_1_07.bmp");

    printf("Starting Pattern Sequence...\n");
    LCR_CMD_StartPatternSeq(1);
    mSleep(10000);
    LCR_CMD_StartPatternSeq(0); //Stop pattern sequence

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}

/* Demo-9: Demonstrated saving, loading and deleting a solution */
static void Demo_9_SolutionDemo()
{
    //Create a 1bpp pat sequence and store it as a solution
    LCR_PatternSeqSetting_t patSeqSet;

    printf("\n\n****Running Demo #9 - Solution Load, Save and Delete ****\n\n");

    printf("Configuring 2bpp pattern sequence mode...\n");
    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));
    LCR_CMD_GetPatternSeqSetting(&patSeqSet);
    patSeqSet.BitDepth = 2;
    patSeqSet.NumPatterns = 4;
	patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 10000;
    patSeqSet.ExposureTime = 10000;
    patSeqSet.LEDSelect = LED_RED; //Red LED
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);
    LCR_CMD_DefinePatternBMP(0,"./Images/PatSeqImages/pattern_2_00.bmp");
    LCR_CMD_DefinePatternBMP(1,"./Images/PatSeqImages/pattern_2_01.bmp");
    LCR_CMD_DefinePatternBMP(2,"./Images/PatSeqImages/pattern_2_02.bmp");
    LCR_CMD_DefinePatternBMP(3,"./Images/PatSeqImages/pattern_2_03.bmp");
	LCR_CMD_StartPatternSeq(1);
    //Saving this configuration as solution-1
    printf("Saving it as \"SOLUTION_1\"...\n");
    LCR_CMD_SaveSolution("SOLUTION_1");

    LCR_CMD_StartPatternSeq(0);
    printf("Configuring 7bpp pattern sequence mode...\n");
    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x04));
    LCR_CMD_GetPatternSeqSetting(&patSeqSet);
    patSeqSet.BitDepth = 7;
    patSeqSet.NumPatterns = 3;
	patSeqSet.PatternType = PTN_TYPE_NORMAL;
    patSeqSet.InputTriggerDelay = 0;
    patSeqSet.InputTriggerType = TRIGGER_TYPE_AUTO;
    patSeqSet.AutoTriggerPeriod = 15000;
    patSeqSet.ExposureTime = 15000;
    patSeqSet.LEDSelect = LED_GREEN; //Green LED
    LCR_CMD_SetPatternSeqSetting(&patSeqSet);
    LCR_CMD_DefinePatternBMP(0,"./Images/PatSeqImages/pattern_7_00.bmp");
    LCR_CMD_DefinePatternBMP(1,"./Images/PatSeqImages/pattern_7_01.bmp");
	LCR_CMD_DefinePatternBMP(2,"./Images/PatSeqImages/pattern_7_02.bmp");
    LCR_CMD_StartPatternSeq(1);

    //Saving this configuration as solution-2
    printf("Saving it as \"SOLUTION_2\"...\n");
    LCR_CMD_SaveSolution("SOLUTION_2");

    printf("Switching to static display mode\n");
    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00));
    mSleep(5000);

    printf("Switching to SOLUTION_1...\n");
    LCR_CMD_ManageSolution(SOL_LOAD, "SOLUTION_1");
    LCR_CMD_StartPatternSeq(1);
    mSleep(5000);

    printf("Switching to SOLUTION_2...\n");
    LCR_CMD_ManageSolution(SOL_LOAD, "SOLUTION_2");
    LCR_CMD_StartPatternSeq(1);
    mSleep(5000);

    printf("Switching back to SOLUTION_1...\n");
    LCR_CMD_ManageSolution(SOL_LOAD, "SOLUTION_2");
    LCR_CMD_StartPatternSeq(1);
    mSleep(5000);

    printf("Printing stored solutions names in the LightCrafter\n");
    uint8 Count;
    uint8 DefaultSolution;
    char SolutionName[LCR_CMD_SOLUTION_NAME_LEN*255];
    LCR_CMD_GetSolutionNames(&Count, &DefaultSolution, &SolutionName[0]);
    int i = 0;
    while(i<(Count*LCR_CMD_SOLUTION_NAME_LEN))
    {
        putchar(SolutionName[i]);
        i++;
    }
    printf("\n");

    printf("Deleting solutions SOLUTION_1 & SOLUTION_2\n");

    LCR_CMD_ManageSolution(SOL_DELETE,"SOLUTION_1");

    LCR_CMD_ManageSolution(SOL_DELETE,"SOLUTION_2");

    LCR_CMD_SetDisplayMode((LCR_DisplayMode_t)(0x00)); //Setting back to static display mode
}

/*Function is used to create delay in mSeconds */
/* This is used in demo code to show code flow */
static void mSleep(unsigned long int mSeconds)
{
    /*Add logic and call system function that will create mSeconds millisecond delay */
    sleep((mSeconds/1000.0));
}
