/*********************************************************************************
项目名称： 电法仪
编译环境： IAR 4.20
MCU 型号： MSP430F149
外部晶振： 10M
*********************************************************************************/

#include"main.h"
uchar count=0;
/*********************************************************************************
函数名称：void timera_0_isr(void)																
函数功能：定时器TA中断函数
说    明：
*********************************************************************************/
//#pragma interrupt_handler timera_0_isr:TIMERA0_VECTOR
//void timera_0_isr(void)
#pragma vector = TIMERA0_VECTOR           //TA0中断服务,用来参考稳定
__interrupt void ta0_isr(void)
{          
         if( (Save_Flag==2)||(Delete_flag==2) )//
         {
            SaveTimeCount++;
            if( SaveTimeCount > Basic_Time )
            {
              SaveTimeCount = 0;
              Save_Flag=0;  // 清楚开始存储标志
              Delete_flag=0;//清除 删除 标识
              ////////////////////////////////////为适应工程需要，将“存储”按键赋予了“工作停”的功能。当存储完成后，Save_Flag==2
              /*
              Acquisition_flag = 0;//按压“工作停”，关闭采集
              WorkOn_flag      =   0;//
              FatBuffer.RecordNumber = 0;//关闭"查询"
              HT1621_all_off();
              aStatic_Show();
              */
              Save_WorkStop_flag      =       1;
              //////////////////////////////////////////////////////////////////
            }
         }         
         ////////////////////////////////////////////////////////////////////////////////
         if( Acquisition_flag==1 )
            AcquisitionTime++;
         else
            AcquisitionTime = 0;
       
         if( AcquisitionTime>=(Basic_Time*2+1)  )
         {
           AcquisitionTime=0;
         }
         /////////////////////////////////////////////////////////////////////////////////
         //////////////////////////////////////////////////////////////////////////////
         if( (WorkOn_flag==1)||(FatBuffer.RecordNumber > 0) )
           WorkOnTime++;
         else
           WorkOnTime = 0;
         if( WorkOnTime>=(Basic_Time*3+1) ) WorkOnTime = 0;
         //////////////////////////////////////////////////////////////////////////////
         time++;   //time==120, it is almost one time.  1s //if(time == 120)  	 
	 if(time == 200)	 
  	 {
      	        second++;
		time=0;                 
                
                
                LED1_Flicker; // LED1 指示灯闪烁	

                if( (WorkOn_Transmit_STA==1)&&(WorkOn_flag==1) )
                {//////发射板控制---------注：一旦打开标校中的电流标校，此段代码不起作用
                  WorkOn_Transmit++;
                  if( TimeWindow_data==0 )
                  {
                    if( WorkOn_Transmit>10 )//大于10秒，停止发射
                    {
                      WorkOn_Transmit = 0;
                      WorkOn_Transmit_STA=0;
                    }
                  }
                  if( TimeWindow_data==1 )
                  {
                    if( WorkOn_Transmit>20 )
                    {
                      WorkOn_Transmit = 0;
                      WorkOn_Transmit_STA=0;
                    }
                  }
                  if( TimeWindow_data==2 )
                  {
                    if( WorkOn_Transmit>30 )
                    {
                      WorkOn_Transmit = 0;
                      WorkOn_Transmit_STA=0;
                    }
                  }
                  if( TimeWindow_data==3 )
                  {
                    if( WorkOn_Transmit>40 )
                    {
                      WorkOn_Transmit = 0;
                      WorkOn_Transmit_STA=0;
                    }
                  }
                }
  	 }
         ///////////////////////////////////////////////////////////////////////////////////	 
         if(keytime_start	==	1) //按键计时部分
	 {
		keytime++;
		keytime_save = keytime;
                
                if( keytime_save>keylongtime )
                {
                  keyon = 0;
                  keylong_flag        =       1;
                  //key_use1();
                }
                
	 }
         //////////////////////////////////////////////////////////////////////////////// 
         
         ////////////////////////////////////////////////////////////////////////////////
	 CCR0 = 10000;	 
}

/********************************************************************************************
函数名称：void tb0_isr(void)
函数功能：定时器 B 中断
说    明：
时    间：2010-02-27
********************************************************************************************/
#pragma vector = TIMERB0_VECTOR           //TB0中断服务,用来参考稳定---ACLK
__interrupt void tb0_isr(void)
{
  
  if( WorkOn_flag==1 )
  {  //Transmitter( FeatureSelection_data ); //  发射频率 
    if(count++ >10)
    {
    P15_Flicker;
    P32_Flicker;
    P33_Flicker;
    count =0;
    }
    //P16_Flicker;
  }  
  
}

/********************************************************************************************
函数名称：void adc_isr(void)
函数功能：A/D转换中断函数
说    明：
时    间：2010-02-27
********************************************************************************************/
#define   ADc12max      89                //  AD转换次数最大值，等于次数，就要将“和”平均
unsigned long      sum_2         =     0;          //  存储 A3 通道转换的AD值，求和
unsigned long      sum_4         =     0;          //  存储 A4 通道转换的AD值，求和
unsigned long      sum_0         =     0; //信号溢出检测 第 1 路
unsigned long      sum_1         =     0; //信号溢出检测 第 2 路

uchar     ADc12count    =     0;          //  AD转换次数计数变量
#define current    (long)( (((float)(MCU_Ref)*(float)(A4result)*1000.0)/((float)(4095)))/((float)(Current_R)) )  //  直接换算成电流
#define Current_K   2.1683  //  斜率
//#define Current_K   3.7783  //  斜率
#define Current_B   6729.8  //  常数
//#define Current_B   26729.8  //  常数



#pragma vector = ADC12_VECTOR             //  ADC12中断服务
__interrupt void adc_isr(void)
{  
  ADc12count++;
  
  sum_2 += ADC12MEM0;//电池电压
  ADC12MEM0 = 0;
  sum_4 += ADC12MEM1;
  ADC12MEM1 = 0;
  sum_0 += ADC12MEM2;
  ADC12MEM2 = 0;
  sum_1 += ADC12MEM3;
  ADC12MEM3 = 0;
  
  if( ADc12count>=ADc12max )
  {
    ADc12count = 0;
    A2result = sum_2/ADc12max;
    A4result = sum_4/ADc12max;
    sum_2 = 0; 
    sum_4 = 0; 
    A0result = sum_0/ADc12max;
    A1result = sum_1/ADc12max;
    sum_0 = 0; 
    sum_1 = 0; 
    /////////////////////////////
    /*///此部分是过去的电流，两点标校方法
    iXiuZhengCount++;    
    sum_XiuZheng  += Current_K * current - Current_B; //i_current_xiuzheng = Current_K * current - Current_B;// 此值没有标校的因素
    if( iXiuZhengCount >= XiuZhengCount )
    {
      iXiuZhengCount  = 0;
      XiuZheng_Result = sum_XiuZheng/XiuZhengCount;
      sum_XiuZheng = 0;
    }
    */
    CurrentFilter();
  }
}

/*********************************************************************************
函数名称：void main(void)																	
函数功能：主函数
说    明：
*********************************************************************************/
void main(void)
{ 

         InitSys();
	 init_devices();

	 HT1621_all_off();
	 DelayMS(4000);
	 HT1621_all_on();
	 DelayMS(9000);	 
	 Disp_0_9();	 
	 Started();         
         
	 MCU_Start_flag = 1;  //  单片机启动
         QIANYABAOHU_Normal;  //  欠压保护脚，接采集板电源可控硅EA脚。常态下输出高
         GUOLIU_Normal;       //  过流保护脚，接输出板电源模块EA脚。常态下输出高         
         First_Check();
	 FrequencyPointSelection1_L;
         FrequencyPointSelection0_L;
         GainControlFirst();
         FeatureSelection_Port_First( );
         WorkOnControl_H;//工作开控制脚，常态下  高。。。，工作开：低。。
         
	 while(1)
	 {            
		  key_use();		  
		  SettingControl();
		  PositionControl();
                  AcquisitionControl();
                  WorkOnControl();
                  BatteryShow();
                  Current_Protect();
                  GainControl();//控制脚
                  
                  FeatureSelectionPortSTOP();
                  SaveControl(); 
                  DemandControl();
                  DeleteControl();
                  SaveUseWorkStop( );
	 } 
}