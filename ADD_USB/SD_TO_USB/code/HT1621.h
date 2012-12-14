/*============================================ 头文件说明 ============================================================
液晶屏
================================================ END ===============================================================*/


#define BIAS   0x52  //0b1000 0101 0010 1/3duty 4com
#define SYSDIS 0X00  //0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define SYSEN  0X02  //0b1000 0000 0010 打开系统振荡器
#define LCDOFF 0X04  //0b1000 0000 0100 关LCD偏压
#define LCDON  0X06  //0b1000 0000 0110 打开LCD偏压
#define XTAL   0x28  //0b1000 0010 1000 外部接时钟
#define RC256  0X30  //0b1000 0011 0000 内部时钟
#define TONEON 0X12  //0b1000 0001 0010 打开声音输出
#define TONEOFF 0X10 //0b1000 0001 0000 关闭声音输出
#define WDTDIS 0X0A  //0b1000 0000 1010 禁止看门狗

/*HT1621控制位（液晶模块接口定义，根据自已的需要更改）*/
/*定义端口HT1621数据端口 */
/*
#define HT1621_WR1       P5OUT |= BIT3  
#define HT1621_WR0       P5OUT &= ~BIT3 
#define HT1621_DATA1     P5OUT |= BIT1 
#define HT1621_DATA0     P5OUT &= ~BIT1 
#define HT1621_CS1       P5OUT |= BIT4 
#define HT1621_CS0       P5OUT &= ~BIT4 
*/
#define HT1621_CS1       P4OUT |= BIT5 
#define HT1621_CS0       P4OUT &= ~BIT5
#define HT1621_WR1       P4OUT |= BIT4  
#define HT1621_WR0       P4OUT &= ~BIT4 
#define HT1621_DATA1     P4OUT |= BIT3 
#define HT1621_DATA0     P4OUT &= ~BIT3 
//uchar ShowSecendFlag=0;

//uchar time_dig[17];  //存放时间数据
void SmallDelay(void);
void DelayMS(uint iMs);
void Ht1621Wr_Data(uchar Data,uchar cnt);
void Ht1621WrCmd(uchar Cmd);
void Ht1621WrOneData(uchar Addr,uchar Data);
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt);
void Ht1621_Init(void);
void HT1621_all_off(void) ;
void HT1621_all_on(void);



/*delay us*/
void SmallDelay(void) //5,7,9
{
  uchar i=0;
  for(i=0;i<4;i++);
}

//delay ms
void DelayMS(uint iMs)
{
  uint i,j;
  for(i=0;i<iMs;i++)
    for(j=0;j<30;j++) SmallDelay();
}

/******************************************************
写数据函数,cnt为传送数据位数,数据传送为低位在前
*******************************************************/
void Ht1621Wr_Data(uchar Data,uchar cnt)
{
    uchar i;
    for (i=0;i<cnt;i++)
    {
        HT1621_WR0;
        SmallDelay();
        if(Data&0x80) HT1621_DATA1; 
           else HT1621_DATA0;
        //SmallDelay();
        HT1621_WR1;
        SmallDelay();
        Data<<=1;
    }
}
/********************************************************
函数名称：void Ht1621WrCmd(uchar Cmd)
功能描述: HT1621命令写入函数
全局变量：无
参数说明：Cmd为写入命令数据
返回说明：无
版 本：1.0
说 明：写入命令标识位100
********************************************************/
void Ht1621WrCmd(uchar Cmd)
{
  HT1621_CS0;
  SmallDelay();
  Ht1621Wr_Data(0x80,4); //写入命令标志100
  Ht1621Wr_Data(Cmd,8); //写入命令数据
  HT1621_CS1;
  SmallDelay();
}
/********************************************************
函数名称：void Ht1621WrOneData(uchar Addr,uchar Data)
功能描述: HT1621在指定地址写入数据函数
全局变量：无
参数说明：Addr为写入初始地址，Data为写入数据
返回说明：无
版 本：1.0
说 明：因为HT1621的数据位4位，所以实际写入数据为参数的后4位
********************************************************/
void Ht1621WrOneData(uchar Addr,uchar Data)
{
  HT1621_CS0;
  Ht1621Wr_Data(0xa0,3); //写入数据标志101
  Ht1621Wr_Data(Addr<<2,6); //写入地址数据
  Ht1621Wr_Data(Data<<4,4); //写入数据
  HT1621_CS1;
  SmallDelay();
}
/********************************************************
函数名称：void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
功能描述: HT1621连续写入方式函数
全局变量：无
参数说明：Addr为写入初始地址，*p为连续写入数据指针，
cnt为写入数据总数
返回说明：无
版 本：1.0
说 明：HT1621的数据位4位，此处每次数据为8位，写入数据
总数按8位计算
********************************************************/
void Ht1621WrAllData(uchar Addr,uchar *p,uchar cnt)
{
    uchar i;
    HT1621_CS0;
    Ht1621Wr_Data(0xa0,3); //写入数据标志101
    Ht1621Wr_Data(Addr<<2,6); //写入地址数据
    for (i=0;i<cnt;i++)
    {
        Ht1621Wr_Data(*p,8); //写入数据
        p++;
    }
    HT1621_CS1;
    SmallDelay();
}
/********************************************************
函数名称：void Ht1621_Init(void)
功能描述: HT1621初始化
全局变量：无
参数说明：无
返回说明：无
版 本：1.0
说 明：初始化后，液晶屏所有字段均显示
********************************************************/
void Ht1621_Init(void)
{
  HT1621_CS1;
  HT1621_WR1;
  HT1621_DATA1;
  DelayMS(1); //延时使LCD工作电压稳定
  Ht1621WrCmd(LCDOFF);
  Ht1621WrCmd(BIAS);
  Ht1621WrCmd(RC256); //使用内部振荡器
  Ht1621WrCmd(SYSDIS);
  Ht1621WrCmd(WDTDIS);
  Ht1621WrCmd(SYSEN);
  Ht1621WrCmd(LCDON);
}

/**------------------------------------------------------------------------- 
                           Name: all_off(清除1621显示) 
---------------------------------------------------------------------------*/ 
void HT1621_all_off(void) 
{ 
   uchar i; 
   uchar addr=0; 
   for(i=0;i<34;i++) 
   { 
      Ht1621WrOneData(addr,0x00); 
      addr+=1; 
   } 
} 
/**************************************************************************** 
                           Name: all_on(全部点亮1621) 
****************************************************************************/ 
void HT1621_all_on(void) 
{ 
   uchar i; 
   uchar addr=0; 
   for(i=0;i<34;i++) 
   { 
     Ht1621WrOneData(addr,0xff); 
     addr+=1; 
   } 
} 

