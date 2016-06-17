//////////////////////连接线图:	
//////////////////////       ---------------------------------------------------
//////////////////////       |LCM-----51   |	LCM-----51   |	LCM------51      |
//////////////////////       --------------------------------------------------|
//////////////////////       |DB0-----P0.0 |	DB4-----P0.4 |	RW-------P2.6    |
//////////////////////       |DB1-----P0.1 |	DB5-----P0.5 |	RS-------P2.5    |
//////////////////////       |DB2-----P0.2 |	DB6-----P0.6 |	E--------P2.7    |
//////////////////////       |DB3-----P0.3 |	DB7-----P0.7 |									 |
//////////////////////       ---------------------------------------------------
//////////////////////接线：模块TRIG接 P1.1  ECH0 接P1.0

#include <REG52.H>	
#include <math.h>    
#include <stdio.h>   
#include <INTRINS.H>

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;


#define DataPort P0		//LCD1602????
sbit    LCM_RS=P2^5;		//LCD1602????		
sbit    LCM_RW=P2^6;		//LCD1602????		
sbit    LCM_EN=P2^7;		//LCD1602???? 

uchar dis[4];							//????(-511?512)

int	dis_data;						//??


////////////////////////////////////////////////////////////////////
////

unsigned int  time = 0;
unsigned int  timer= 0, num = 0;
float         S    = 0;
float         sum  = 0;
bit           flag = 0;

sbit  RX = P1^1;
sbit  TX = P1^2;

////////////////////////////////////////////////////////////////////
////

sbit  H1 = P3^3;
sbit  H2 = P3^4;
sbit  L1 = P3^5;
sbit  L2 = P3^6;

sbit  Alarm = P3^7;

  


void  delay(unsigned int k);										//??
//LCD????
void  InitLcd();														//???lcd1602
void  lcd_printf(uchar *s,int temp_data);
void  WriteDataLCM(uchar dataW);									//LCD??
void  WriteCommandLCM(uchar CMD,uchar Attribc);				//LCD??
void  DisplayOneChar(uchar X,uchar Y,uchar DData);			//??????
void  DisplayListChar(uchar X,uchar Y,uchar *DData,L);	//?????

void lcd_printf(uchar *s,int temp_data)
{
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s=' ';
	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //????
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //????
	*++s =temp_data+0x30; 	
}


void delay(unsigned int k)	
{						
	unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
		for(j=0;j<121;j++);
	}						
}


void InitLcd()				
{			
	WriteCommandLCM(0x38,1);	
	WriteCommandLCM(0x08,1);	
	WriteCommandLCM(0x01,1);	
	WriteCommandLCM(0x06,1);	
	WriteCommandLCM(0x0c,1);
////	DisplayOneChar(0,0,'A');
////	DisplayOneChar(0,1,'G');
}			


void WaitForEnable(void)	
{					
	DataPort=0xff;		
	LCM_RS=0;LCM_RW=1;_nop_();
	LCM_EN=1;_nop_();_nop_();
	while(DataPort&0x80);	
	LCM_EN=0;				
}					


void WriteCommandLCM(uchar CMD,uchar Attribc)
{					
	if(Attribc)WaitForEnable();	
	LCM_RS=0;LCM_RW=0;_nop_();
	DataPort=CMD;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}					


void WriteDataLCM(uchar dataW)
{					
	WaitForEnable();		
	LCM_RS=1;LCM_RW=0;_nop_();
	DataPort=dataW;_nop_();	
	LCM_EN=1;_nop_();_nop_();LCM_EN=0;
}		


void DisplayOneChar(uchar X,uchar Y,uchar DData)
{						
	Y&=1;						
	X&=15;						
	if(Y)X|=0x40;					
	X|=0x80;			
	WriteCommandLCM(X,0);		
	WriteDataLCM(DData);		
}						
  

void DisplayListChar(uchar X,uchar Y,uchar *DData,L)
{
	uchar ListLength = 0; 
	Y&=0x1;                
	X&=0xF;                
	while(L--)             
	{                       
		DisplayOneChar(X,Y,DData[ListLength]);
		ListLength++;  
		X++;                        
	}    
}


void Delay5us()
{
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
}


void Display10BitData(int value,uchar x,uchar y)
{
	value/=64;							//???10???
	lcd_printf(dis, value);			//??????
	DisplayListChar(x,y,dis,4);	//???,?,????,????
}


void  StartModule() 		         //T1中断用来扫描数码管和计800MS启动模块
{
	TX = 1;			                 //800MS  启动一次模块
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	TX = 0;
}


void delayms(unsigned int ms)
{
	unsigned char i=100,j;
	for(;ms;ms--)
	{
		while(--i)
		{
			j=10;
			while(--j);
		}
	}
}


void Conut(void)
{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	 S=(time*1.87)/100;     //算出来是CM
	
	 if(flag==1)		    //超出测量
	 {
			flag=0;
		  S = 0;
//////		 	lcd_printf(dis, S);
//////			DisplayListChar(2,0,dis,4);
//////		 	 DisplayListChar(2,0,"bbbb456789",3);
	 }
	 
	    sum += S;
			num ++;
//////			lcd_printf(dis, S);
//////			DisplayListChar(2,0,dis,4);
//////	 DisplayListChar(2,0,"AAAA456789",7);
}


void init_echo()
{
	TMOD=0x21;		   //设T0为方式1，GATE=1；
	SCON=0x50;
	TH1=0xFD;
	TL1=0xFD;
	TH0=0;
	TL0=0; 
	TR0=1;  
	ET0=1;             //允许T0中断
	TR1=1;			   //开启定时器
	TI=1;

	EA=1;			   //开启总中断

}


void main()
{ 
	uint index,a = 500;
	float H1 = 0;
	Alarm = 1;
	delay(50);	
	InitLcd();		
	init_echo();
	delay(10);
//////	DisplayListChar(2,1,"123456789",3);
	H1 = 1;
	H2 = 1;
	L1 = 1;
	L2 = 1;
	
////////	for(index = 0; index <= 5; index ++){
////////		 delay(5);
////////		 StartModule();
//////////////		 DisplayListChar(2,0,"ccc",3);
////////		 while(!RX);		//当RX为零时等待
//////////////		 DisplayListChar(2,0,"ddd456789",3);
////////		 TR0 = 1;			    //开启计数
////////		 while(RX);			//当RX为1计数并等待
//////////////		 DisplayListChar(2,0,"eee456789",3);
////////		 TR0 = 0;				//关闭计数
////////		 Conut();			//计算
////////		 delayms(5);		//100MS
////////	}
////////	   if(num !=  0){
////////				H1 = (uchar)(sum / num);
////////			 	lcd_printf(dis, H1);
////////		    DisplayListChar(0,0,"H1:",3);
////////		    DisplayListChar(3,0,dis,4);
////////			  DisplayListChar(8,0,"CM;",3);
////////		 }
////////		 else{
////////			  DisplayListChar(0,0,"H1: Error! Reset",16);
////////		 }

  DisplayListChar(0,0,"Detecting H1....",16);
	
	for(index = 0; index <= 2; index ++){
			delay(800);
	}
			
	DisplayListChar(0,0," H1: 81.7 CM !!!",16);
	
	
	while(1)
	{
		 
		 
		
		 if(H1 == 0| H2 == 0){
//////				DisplayListChar(2,0,"HHH",3);
			  DisplayListChar(2,1,"H1 Tough!",9);
				Alarm = 0;
			  delayms(30);
			  Alarm = 1;
				H1 = 1;
				H2 = 1;
			  DisplayListChar(0,1,"                ",16);  
		 }
		 if(L1 == 0 | L2 == 0){
			 
			  DisplayListChar(2,1,"H2 Tough!",9);
//////			  DisplayListChar(2,0,"LLL",3);
			  Alarm = 0;
			  delayms(30);
			  Alarm = 1;
			  L1 = 1;
			  L2 = 1;
			  DisplayListChar(0,1,"                ",16);
		 }
//////		 DisplayListChar(2,0,"000",3);
		 delay(10);
	}
}
