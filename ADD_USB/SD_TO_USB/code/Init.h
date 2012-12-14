/*============================================ 头文件说明 ============================================================
头文件名称：init.h
头文件用途：各种功能模块以及单片机的初始化
================================================ END ===============================================================*/

//////////////////////////////////// 函数声明 /////////////////////////////////////
void InitSys(void);
void TimeA_Init(void);
void TimeB_Init(void);
void  AVRcontrol(unsigned char fffre);
void SetTimeB0(unsigned char fre);
void port_init(void);
void init_devices(void);
void adc12_init(void);

////////////////////////////////////// END ////////////////////////////////////////

/*********************************************************************************
函数名称：void InitSys(void)																	
函数功能：系统初始化
说明：
*********************************************************************************/
void InitSys(void)
{
 	//volatile uint i=0;	
	uchar i=0;
    WDTCTL = WDTPW + WDTHOLD;  	 			//停止看门狗
	
	BCSCTL1&=~XT2OFF; 						//使XT2有效,XT2上电时默认为关闭的.
    do
    {
      IFG1 &= ~OFIFG;   					 //清振荡器失效标志             
      for (i = 0xFF; i > 0; i--);  			 //延时,待稳定.
    }
	while ((IFG1 & OFIFG)!=0);     			 //若振荡器失效标志有效
	//BCSCTL2 =SELM_2+SELS;//BCSCTL2 += SELM1;  //MCLK的时钟源为TX2CLK，分频因子为1
	                   //BCSCTL2 += SELS;  //SMCLK的时钟源为TX2CLK，分频因子为1                     
	
					   			 //SVSCTL=0X60+PORON;//SVS
	BCSCTL2 = 0x00;
	BCSCTL2 |= SELM1;//MCLK 时钟为XT2
	BCSCTL2 |= SELS;//SMCLK时钟为XT2
    
}

/*********************************************************************************
函数名称：void port_init(void)																	
函数功能：管脚初始化
说明：
*********************************************************************************/
void port_init(void)
{/*
  P1DIR |=BIT4+BIT5+BIT6+BIT7;  //蜂鸣器管脚使能输出 背景指示灯
  P1DIR &= 0xf0;
  P2DIR &=~BIT2;
  P2DIR &=~BIT3;//按键对应接口，设置为输入
  P2DIR |= BIT0+BIT4+BIT1;
  P4DIR |= BIT2;
  P5DIR |=BIT1+BIT3+BIT4;
  P3DIR |=BIT1+BIT2+BIT3+BIT4;
  P3DIR &=~BIT5;
  P3SEL |= 0x30; 
  P1DIR |=BIT0 ;//SVS   
  */
  P1DIR    |=   BIT5 + BIT6;  //频点选择，两个频率发射脚
  //P1SEL |= BIT5+BIT6;
  //P3DIR |=	BIT1+BIT2+BIT3;   		   //ds1302
  P3DIR |=	BIT0 + BIT1; // 增益
  P2DIR |=      BIT7; // 增益
  P1DIR |=	BIT0+BIT1+BIT2+BIT3+BIT4;  //增益，LED1 
  P4DIR |=	BIT3+BIT4+BIT5;			   //液晶屏   
  //P1DIR	&=	~BIT7;   			//标校按键，设置为输入，低电平有效  
  P5DIR    |=   BIT5 + BIT6 ;  //  控制 AVR 单片机
  
  P3DIR |=	BIT6 + BIT7 + BIT4; //ad7711  
  P3DIR	&=	~BIT5;  //ad7711  
  P4DIR |=	BIT0 + BIT1; //ad7711  
  
  P4DIR |=	BIT7;//---现在是增益
  P5DIR |=	BIT0;//过流保护脚---现在是增益
  //P6DIR |=	BIT0+BIT1;//增益
  
  P6DIR |=	BIT7;//欠压保护脚
  P6DIR |=	BIT6;//过流保护脚
  
  P5DIR |=	BIT1 + BIT2 + BIT3 + BIT4;//memory
  P3DIR |=      BIT2 + BIT3;
  
  CH376_PORT_INIT();
}

/*********************************************************************************
函数名称：void init_devices(void)																	
函数功能：单片机内部各个模块初始化
说明：
*********************************************************************************/
void init_devices(void)
{
 //stop errant interrupts until set up
 //DINT(); //disable all interrupts
  _DINT();
 ME1=0X00; //disable sfr peripherals
 ME2=0X00;  
 IE1=0x00; //disable sfr interrupts
 IE2=0x00; 
 //initialise other peripherals
 port_init();  
 Ht1621_Init();
 TimeA_Init(); 
 TimeB_Init();
 adc12_init(); 
 ad7711Init(0); 
 /////////////////////////
 /*
 MMC_Init();
 MMC_get_volume_info();
 FAT32_Init(&Init_Arg);
 */
 MemoryInit();
 ////////////////////////////
 //EINT(); //re-enable interrupts
 _EINT();
}

/*********************************************************************************
函数名称：void TimeA_Init(void)																	
函数功能：定时器A初始化
说明：
*********************************************************************************/
void TimeA_Init(void)
{
  
  //CCR0 = 50000;
  //CCR0 = 12500; // (1/800)秒中断一次
  CCR0 = 10000;
  
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode
 
  
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  
}

/**************************************************************************************
函数名称: TimeB_Init()
函数功能: 定时器 B 初始化
时    间：2010-02-27
说    明: 1、时钟选择 ACLK；
          2、单步中断时间，需要满足400Hz的频率---40.96
          3、最小中断时间：30.5 uS
          4、频率（Hz）     TBCCR0
              120           136.53
              15            1092.27
              5             3276.8
**************************************************************************************/
void TimeB_Init(void)
{
  TBCTL = TBSSEL_2 + TBCLR + MC0;//ACLK==32768
  //TBCTL |= MC0;
  TBCCTL0 = CCIE;                       // CCR0 interrupt enabled
  
  //TBCCR0=10;
  //TBCCR0=yui;
}

/************************************************************************************
函数名称：void SetTimeB0(unsigned char fre)
函数功能：根据选择的频率来设置定时时间----定时器B0
创建时间：2010-04-28
说    明：1、ti：要定时的时间，与时钟源的频率有关
          2、fre：选择的频率; 0-5hz,1-15hz,2-120hz,3-400hz
          3、原先的 4 种频率都要分别除以 2 ；
************************************************************************************/
#define frequency_5     41690      //  注："/2"是因为原来是要发射 5 赫兹的频率，而现在要发射的是 10 赫兹。
#define frequency_15    13823
#define frequency_120   1730
#define frequency_400   519
void SetTimeB0(unsigned char fre)
{    
    if( fre==0 )
      TBCCR0=frequency_5;				//定时时间
    if( fre==1 )
      TBCCR0=frequency_15;				//定时时间
    if( fre==2 )
      TBCCR0=frequency_120;				//定时时间
    if( fre==3 )
      TBCCR0=frequency_400;				//定时时间
    ///////////////////////////////
    AVRcontrol( fre );
}

void  AVRcontrol( unsigned char fffre )
{
    if( fffre == 0 )
    {
      AVR_1_L;
      AVR_2_L;
    }
    if( fffre == 1 )
    {
      AVR_1_L;
      AVR_2_H;
    }
    if( fffre == 2 )
    {
      AVR_1_H;
      AVR_2_L;
    }
    if( fffre == 3 )
    {
      AVR_1_H;
      AVR_2_H;
    }
}

/**************************************************************************************
函数名称: adc12_init()
函数功能: AD初始化
时    间：2010-02-27
说    明: 1、使用片内参考电压 1.5V，但是由于硬件原因，1.5V的参考电压没有调试===已经调试通过；
          2、使用AD中断来完成；
**************************************************************************************/
void adc12_init(void)
{
  
  //P6SEL =0xff;//   
  //P6SEL =0x18;//00011000
  P6SEL =0x1B;//00011011
  ADC12CTL0 = ADC12ON+MSC+SHT0_15+REFON;// Turn on ADC12, set sampling time,片内参考电压
  
  //ADC12CTL1 = SHP+CONSEQ_3;             // Use sampling timer, repeated sequence
  //ADC12CTL1 = ADC12SSEL_2 ;             //  ACLK  
  //ADC12CTL1 =  ADC12DIV_5 ;             //  实践证明：ADC12CTL1 寄存器必须一次将所有值均赋值。
  ADC12CTL1 = SHP + CONSEQ_3 + ADC12SSEL_2 + ADC12DIV_5;//  SHP--采样信号(SAMPCON)选择控制位
                                        //  CONSEQ_3--*序列通道多次转换
                                        //  ADC12SSEL_2--时钟选择：MCLK
                                        //  ADC12DIV_5-- 6分频
  
  ADC12MCTL0 = INCH_3;                  // ref+=AVcc, channel = A3 电池电压采集
  ADC12MCTL1 = INCH_4 ;              // ref+=AVcc, channel = A4, end seq. 采集发射板电流
  ADC12MCTL2 = INCH_0;
  ADC12MCTL3 = INCH_1 + EOS ;         
  ADC12IE = 0x02;                       // Enable ADC12IFG.1
  ADC12CTL0 |= ENC;                     // Enable conversions
  ADC12CTL0 |= ADC12SC;                 // Start conversion
}

