/*============================================ 头文件说明 ============================================================
头文件名称：AD7711_ZY1.h
头文件用途：24位AD芯片驱动程序
时      间：2010-02-25
配      置：该芯片使用外部晶振，10M
================================================ END ===============================================================*/


/********************************************************************************

                            	数据类型的定义

********************************************************************************/
#define  	BYTE	unsigned char	
#define 	WORD	unsigned int
#define  	SBYTE	signed char
#define 	SWORD	signed int


#define         DRDY       BIT5
#define         DATA       BIT1
/*RFS的控制*/
#define		SET_RFS_1		(P3OUT |= BIT7)
#define		CLR_RFS_1		P3OUT &=~ BIT7

/*TFS的控制*/
#define		SET_TFS_1		P3OUT |= BIT6
#define		CLR_TFS_1		P3OUT &=~ BIT6

/*DRDY的状态*/  //读取管脚状态
//#define		STATUS_DRDY_1		(P3IN&0X20) //读取P35
#define		STATUS_DRDY_1		(P3IN & DRDY) //读取P35

/*SCLK的控制*/
#define		SET_SCLK		P4OUT |= BIT0
#define		CLR_SCLK		P4OUT &=~ BIT0

/*SDATA的控制*/
#define		SET_SDATA		P4OUT |= BIT1	
#define		CLR_SDATA		P4OUT &=~ BIT1
/*SDATA的状态*/
//#define		STATUS_SDATA	(P4IN&0X02)
#define		STATUS_SDATA	(P4IN & DATA)
/*SDATA输入输出的控制*/
/*SDATA_MASTER:ATmega32L输出，发送数据，为主器件。	*/
/*SDATA_SLAVE :ATmega32L输入，接受数据，为从器件。	*/
#define		SDATA_MASTER	        P4DIR |=BIT1
#define		SDATA_SLAVE		P4DIR&=~BIT1

/*A0的控制*/
/*A0=0,进入控制寄存器*/
/*A0=1,进入数据寄存器*/
#define		ACCESS_CTL_REG		P3OUT |= BIT4
#define		ACCESS_DATA_REG		P3OUT &=~ BIT4	
                                    
/*AD7710的控制寄存器*/
//#define		CTRL_REG_LOW		0x86		
//#define		CTRL_REG_MID		0x81
#define		CTRL_REG_LOW		0x0D	//  FS7-FS0: 00001101	
#define		CTRL_REG_MID		0x93	//	WL R0 B0 B/U FS11 FS10 FS9 FS8: 10000011
										//	WL = 1; 输出数据长度24位
										//	R0 = 0; 关
										//	B0 = 0;	熔断电流关
										//	B/U= 0;	双极，与 CH 配合使用
#define		CTRL_REG_HIG		0x20	//	MD2 MD1 MD0 G2 G1 G0 CH PD: 00100000
										//	MD2 MD1 MD0=001,激活自校准模式
										//	G2 G1 G0=000,增益--1
										//	CH = 0; AIN1通道，默认
										//	PD = 0;	标准运行状态
//#define		CTRL_REG_HIG		0x22	//	AIN2通道
//#define		CTRL_REG_HIG		0x30	//	通道1，增益8倍



//*********************************************   函数声明   **********************************************
void GetRealAD(void);
void send24Bit(BYTE *Comm,BYTE chs);//向AD7711指定的通道发送24bit数据，chs：选择通道数，0，1，2,3
void readCommReg(BYTE *CommReg,BYTE chs);//读AD7711控制寄存器，放在*CommReg里。 chs：选择通道数，0，1，2，3
BYTE ad7711Init(BYTE chs);//对AD7711初始化。 chs：选择通道数，0,1,2,3
signed long Read24DataFromAd7710(void);
//*************************************************** END *************************************************

/**********************************************************************
函数名称:void send24Bit(BYTE *Comm,BYTE chs)
函数功能:向AD7711指定的通道发送24bit数据，chs：选择通道数，0，1，2,3
时    间：2010-02-25
配    置：外部晶振，10M
**********************************************************************/
void send24Bit(BYTE *Comm,BYTE chs)
{
	BYTE i,temp,j;	
        ACCESS_DATA_REG;//A0=0;	//控制寄存器
        CLR_TFS_1;//_TFS = 0;//写数据模式 
	for(i=0;i<3;i++)
	{		
                temp=*(Comm+i);
		for(j=0;j<8;j++)
		{
                        SET_SCLK;
			if(temp&0x80)
			{
				SET_SDATA;
			}
			else
			{
				CLR_SDATA;
			}
			
			asm("nop");//DelayMS(1);//asm("nop");
			CLR_SCLK;
			temp<<=1;
		}
	}	
	ACCESS_CTL_REG;
        SET_TFS_1;
}

/**********************************************************************
functionName:void readCommReg(BYTE *CommReg,BYTE chs)
description:读AD7711控制寄存器，放在*CommReg里。 chs：选择通道数，0，1，2，3
运行时间：70.53us  7.3728MHZ晶振
**********************************************************************/
void readCommReg(BYTE *CommReg,BYTE chs)
{
	BYTE i,j,temp=0;	
	SDATA_SLAVE;		
        ACCESS_DATA_REG;//A0=0; ////控制寄存器
        CLR_RFS_1;//_RFS = 0;//读数据模式 	
        asm("nop");
	for(i=0;i<3;i++)
	{
		temp=0;			
		asm("nop");//DelayMS(20);//asm("nop");
		for(j=0;j<8;j++)
		{
			SET_SCLK;                        
			temp<<=1;			
			if(STATUS_SDATA)///////////////////////////////////////////////////
			temp|=0x01;				
			asm("nop");//DelayMS(1);//asm("nop");	
			CLR_SCLK;
		}
		*(CommReg+i)=temp;
	}
	SET_RFS_1;	
        ACCESS_CTL_REG;
	SDATA_MASTER;
}

/**********************************************************************
functionName:BYTE ad7711Init(BYTE chs)
description:对AD7711初始化。 chs：选择通道数，0,1,2,3
如果初始化成功返回0xff,失败返回0x00;
**********************************************************************/
uchar ad7711init;////////////////////////////////////////////////////////////
BYTE ad7711CtrlReg[3],temp1[3];
BYTE ad7711Init(BYTE chs)
{
	BYTE i,j,result;
RE:
	ad7711CtrlReg[0]=CTRL_REG_HIG;
	ad7711CtrlReg[1]=CTRL_REG_MID;
	ad7711CtrlReg[2]=CTRL_REG_LOW;
	send24Bit(ad7711CtrlReg,chs);
	//usDelay(32);
	asm("nop");//DelayMS(1);
	readCommReg(temp1,chs);
	i=0x00;
	while(i<20)//重复检查初始化是否成功
	{
		result=0xFF;
                ad7711init = result;//////////////////////////
		for(j=0;j<3;j++)
		{
			if(temp1[j]!=ad7711CtrlReg[j])
			{
				result=0x00;
                                ad7711init = result;////////////////////////
			}
		}
		if(result)
		{
			return(result);
		}
		i++;
		send24Bit(ad7711CtrlReg,chs);
		//usDelay(32);
		asm("nop");//DelayMS(1);
		readCommReg(temp1,chs);
	}
        ad7711init = result;        
        
        if( result==0x00 )  goto RE;//重复检查20次初始化失败后，继续返回检查。。。。
        
	return(result);
}



/*******************************************************************************
函数名称：Read24DataFromAd7710(void)
函数功能：读24位数据
说    明：
时    间：2010-02-25
*******************************************************************************/
unsigned long ReadData=0;
signed long Read24DataFromAd7710(void)
{
  uchar i=0;  
  ReadData=0;
  SDATA_SLAVE;   
  while(STATUS_DRDY_1)//
  {
      ;
  }  
  ACCESS_CTL_REG	;//A0=1;	//数据、校准寄存器
  CLR_RFS_1;//_RFS = 0; //读数据模式
  for(i=0;i<24;i++)
  {
	  SET_SCLK;//SCLK=1;
	  ReadData=ReadData<<1;
          if(STATUS_SDATA)
	  {
	      ReadData+=1;
          }
	  CLR_SCLK;//SCLK=0;
  }
  ACCESS_CTL_REG	;//A0 = 1; 
  SET_RFS_1;//_RFS = 1;
  //ReadData = ReadData>>ZengYi_disp; //消除增益 
  return ReadData;
}

/*******************************************************************************
函数名称：
函数功能：将前端增益消除，使得到的AD值变为原值
说    明：
时    间：2010-02-25
*******************************************************************************/
void GetRealAD(void)
{
  
}
