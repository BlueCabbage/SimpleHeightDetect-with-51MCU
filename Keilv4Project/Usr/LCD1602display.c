#include "LCD1602display.h"
//********************??
void delay(uchar z)
{
 uchar i,j;
 for(i=z;i>0;i--)
  for(j=110;j>0;j--);
}


//**********************LCM???
void InitLCM()
{
 WriteCommandLCM(0x38,0);  //????????,??????
 delay(1);
 WriteCommandLCM(0x38,0);
 delay(1);
 WriteCommandLCM(0x38,0);
 delay(1);
 WriteCommandLCM(0x38,1);  //??????:16×2??,5×7??,8?????
 WriteCommandLCM(0x0c,1);  //??????:???,???
 WriteCommandLCM(0x06,1);  //??????:????,????
 WriteCommandLCM(0x01,1);  //?????,??????????
}
//*********************???
uchar ReadStatusLCM()
{
 P0 = 0x80;
 LCM_RS = 0;
 LCM_RW = 1;
 LCM_E = 1;
 while(P0 & 0x80); //?????
 return(0);
}
//***********************???
/*uchar ResdDataLCM()
{
 LCM_RS=1;
 LCM_RW=1;
 LCM_E=1;
 return(0);
} */
//**********************???
void WriteCommandLCM(uchar WCLCM, Busy)
{
 if(Busy) ReadStatusLCM();
 P0 = WCLCM;
 LCM_RS=0;
 LCM_RW=0;
 LCM_E=0;
 LCM_E=1;
}
//***********************???
void WriteDataLCM(uchar WDLCM)
{
 ReadStatusLCM();
 P0=WDLCM;
 LCM_RS=1;
 LCM_RW=0;
 LCM_E=0;
 LCM_E=1;
}
//*************************????
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{
 if(Y) X|=0X40;   //Y=1?????,Y=0?????
 X|=0X80;
 WriteCommandLCM(X,1); //X???????,1??????
 WriteDataLCM(DData); //DData?????
}
//**************************?????
void DisplayListChar(uchar X,uchar Y,uchar code *DData)
{
 uchar ListLength;
 ListLength=0;
 while(DData[ListLength]!='\0')
 {
  if(X<=15)
  {
   DisplayOneChar( X,Y,DData[ListLength]);
   ListLength++;
   X++;
  }
 }
}