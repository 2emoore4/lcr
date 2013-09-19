**WARNING** >>>>>>>>>>>>>>>>THIS IS AUTO GENERATED FILE DO NOT EDIT<<<<<<<<<<<<<<<<

#==================================================================================
#    Variable Bit-Depth Sequence use instructions for DLP LightCrafter EVM	       
#----------------------------------------------------------------------------------
#   Created on          :	Friday, March 22, 2013 12:17:33 PM
#   Created by          :	DLP LightCrafter Variable Bit-depth Sequence Generator
#   Tool Version#       :	1.1.0
#   Pattern Seq Mode#   :	INTERNAL PRE-STORED PATTERNS
#   Exposure time       :	8333 uSec
#   Trig Period         :	>= 8842 uSec
#==================================================================================


The instructions provided below can be used to configure LightCrafter to 
support custom variable bit-depth pattern sequence.                      


1. Sequence Equation: 1bppX1G_2bppX1G_3bppX1G_4bppX1G_5bppX1G_6bppX1G_7bppX1G_8bppX1G_8333uSec


2. Enable Custom Sequence using TCP command (0x0A 0x01) ==> StartVector = 0, NumOfVectors = 10


3. The pattern order used in the sequence is provided in the Table-0.

			Table-0: Pattern Order Table
	-----------		--------------------------		----------------
	PATTERN NO.		PATTERN BIT-DEPTH (in bpp)		PATTERN COLOR
	-----------		--------------------------		----------------


	1				1				Green

	2				2				Green

	3				3				Green

	4				4				Green

	5				5				Green

	6				6				Green

	7				7				Green

	8				8				Green




4. All the patterns must be split into individual binary patterns and then
   placed in the order as explained below Table-1.


		 Table-1: Split Individual binary patterns placement order 		
	  --- 		 ------------------- 
	  S/N 		 BINARY PAT DESC.    
	  --- 		 ------------------- 
	 [00] 		 PATTERN 6 BIT1 

	 [01] 		 PATTERN 6 BIT2 

	 [02] 		 PATTERN 6 BIT3 

	 [03] 		 PATTERN 6 BIT4 

	 [04] 		 PATTERN 6 BIT5 

	 [05] 		 INSERT_1BPP_BLACK_PATTERN 

	 [06] 		 INSERT_1BPP_BLACK_PATTERN 

	 [07] 		 INSERT_1BPP_BLACK_PATTERN 

	 [08] 		 PATTERN 4 BIT2 

	 [09] 		 PATTERN 4 BIT3 

	 [10] 		 PATTERN 5 BIT0 

	 [11] 		 PATTERN 5 BIT1 

	 [12] 		 PATTERN 5 BIT2 

	 [13] 		 PATTERN 5 BIT3 

	 [14] 		 PATTERN 5 BIT4 

	 [15] 		 PATTERN 6 BIT0 

	 [16] 		 PATTERN 1 BIT0 

	 [17] 		 PATTERN 2 BIT0 

	 [18] 		 PATTERN 2 BIT1 

	 [19] 		 PATTERN 3 BIT0 

	 [20] 		 PATTERN 3 BIT1 

	 [21] 		 PATTERN 3 BIT2 

	 [22] 		 PATTERN 4 BIT0 

	 [23] 		 PATTERN 4 BIT1 

	 [24] 		 INSERT_1BPP_BLACK_PATTERN 

	 [25] 		 INSERT_1BPP_BLACK_PATTERN 

	 [26] 		 INSERT_1BPP_BLACK_PATTERN 

	 [27] 		 INSERT_1BPP_BLACK_PATTERN 

	 [28] 		 INSERT_1BPP_BLACK_PATTERN 

	 [29] 		 INSERT_1BPP_BLACK_PATTERN 

	 [30] 		 INSERT_1BPP_BLACK_PATTERN 

	 [31] 		 INSERT_1BPP_BLACK_PATTERN 

	 [32] 		 PATTERN 8 BIT1 

	 [33] 		 PATTERN 8 BIT2 

	 [34] 		 PATTERN 8 BIT3 

	 [35] 		 PATTERN 8 BIT4 

	 [36] 		 PATTERN 8 BIT5 

	 [37] 		 PATTERN 8 BIT6 

	 [38] 		 PATTERN 8 BIT7 

	 [39] 		 INSERT_1BPP_BLACK_PATTERN 

	 [40] 		 PATTERN 7 BIT0 

	 [41] 		 PATTERN 7 BIT1 

	 [42] 		 PATTERN 7 BIT2 

	 [43] 		 PATTERN 7 BIT3 

	 [44] 		 PATTERN 7 BIT4 

	 [45] 		 PATTERN 7 BIT5 

	 [46] 		 PATTERN 7 BIT6 

	 [47] 		 PATTERN 8 BIT0 

