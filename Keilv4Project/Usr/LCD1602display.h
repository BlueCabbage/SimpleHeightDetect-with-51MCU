#ifndef _LCD1602DISPLAY_H_
#define _LCD1602DISPLAY_H_
#include<reg52.h>
#include<intrins.h>
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
//************************LCD????
sbit LCM_RW = P2^6; //¶¨ÒåLCDÒý½Å
sbit LCM_RS = P2^5;
sbit LCM_E  = P2^7;

//**************************??
void delay();
//**************************???
void InitLCM();
//****************************???
uchar ReadStatusLCM();
//******************************???
//uchar ResdDataLCM();
//***************************???
void WriteCommandLCM(uchar WCLCM, Busy);
//***************************???
void WriteDataLCM(uchar WDLCM);
//******************************??1???
void DisplayOneChar(uchar X,uchar Y,uchar DData);
//******************************?????
void DisplayListChar(uchar X,uchar Y,uchar code  *DData);
#endif
 