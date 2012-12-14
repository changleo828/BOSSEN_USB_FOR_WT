/*============================================ 头文件说明 ============================================================
头文件名称：Acquisition.h
头文件用途：采集控制
时      间：2010-02-27
================================================ END ===============================================================*/

#ifndef __Acquisition_H
#define __Acquisition_H

//*********************************************   函数声明   **********************************************
void PositionShow(void);
void CurrentShow( signed long  current_xiuzheng );
//void ClockShow(void);
void AcquisitionControl(void);
void WorkOnControl(void);
//*************************************************** END *************************************************

/**********************************************************************************************
函数名称：void PositionShow(void)
函数功能：显示某个点号
说    明：1、该函数不与”点号“按键配合使用
时    间：2010-02-27
**********************************************************************************************/
void PositionShow(void)
{
    if( Save_Flag==0 )//没有按压“存储”，或者存储没有结束
    {
          Ht1621WrOneData( disp_data_addr[0][0],disp_data[ Position_data[0] ][0] );
   	  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ Position_data[0] ][1] );//
   	  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ Position_data[1] ][0] );
   	  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ Position_data[1] ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ Position_data[2] ][0]  ); 
   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ Position_data[2] ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ Position_data[3] ][0] + 8 ); // 点亮“点号”
   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ Position_data[3] ][1] );//
   	  
	  Ht1621WrOneData( 12,1 );	//小数点
   	  
   	  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ Position_data[4] ][0] );
   	  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ Position_data[4] ][1] );// 
   	  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ Position_data[5] ][0] );
   	  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ Position_data[5] ][1] );//
   	  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ Position_data[6] ][0] );
   	  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ Position_data[6] ][1] );//
          
          Ht1621WrOneData( 13,0 );//单位：mV---关掉
    }
    else
    {
          Ht1621WrOneData( disp_data_addr[0][0],disp_data[ Position_data[0] ][0] );
   	  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ Position_data[0] ][1] );//
   	  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ Position_data[1] ][0] );
   	  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ Position_data[1] ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ Position_data[2] ][0]  ); 
   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ Position_data[2] ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ 0 ][0] + 8 ); // 点亮“点号”
   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ 0 ][1] );//
   	  
	  Ht1621WrOneData( 12,1 );	//小数点
   	  
   	  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ Position_data[4] ][0] + 8 ); //   + "保存"
   	  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ Position_data[4] ][1] );// 
   	  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ Position_data[5] ][0] );
   	  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ Position_data[5] ][1] );//
   	  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ Position_data[6] ][0] );
   	  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ Position_data[6] ][1] );//
          
          Ht1621WrOneData( 13,0 );//单位：mV---关掉
    }
}

/**********************************************************************************************
函数名称：void CurrentShow(void)
函数功能：显示电流
说    明：1、在“工作开”时，电流采集只取采到的第一个值
          2、
时    间：2010-02-27
**********************************************************************************************/
/*
#define current    (long)( (((float)(MCU_Ref)*(float)(A4result)*1000.0)/((float)(4095)))/((float)(Current_R)) )  //  直接换算成电流
#define Current_K   2.1683  //  斜率
#define Current_B   6729.8  //  常数
*/
//#define vol_430    (long)( (((float)(MCU_Ref)*(float)(A4result))/((float)(4095))) )
//uchar  mmma[8];
#define current_xiuzheng_max  200000  //  防止电流数据太大发生溢出
void CurrentShow( signed long  current_xiuzheng )
{
  //long current=1234567;          
  uchar *p;
  uchar i=0;
  
  current_xiuzheng = XiuZheng_Result;
  //current_xiuzheng = ScreenValueForCurrent(  ); //
  //current_xiuzheng = Current_K * current - Current_B;// 此值没有标校的因素
  /*
  if( biaojiao_cur_sign==0 )  //  加入标校的因素
  {
    current_xiuzheng = current_xiuzheng + biaojiao_cur_chazhi;
  }
  else
  {
    current_xiuzheng = current_xiuzheng - biaojiao_cur_chazhi;
  }
  */
  if( current_xiuzheng<=0 )
  {
    current_xiuzheng = 0;
  }
  if( current_xiuzheng > current_xiuzheng_max )
  {
    current_xiuzheng = current_xiuzheng_max;
  }
  
  if( (Acquisition_flag==1)||(WorkOn_flag==1) )
  {
    if( Cur_Calculate_One_Time == 1 )
    {
      p=Decomposition8( current_xiuzheng );
      Cur_Calculate_One_Time =0;
      for( i=0;i<8;i++ )
      {
        mmma[i]= *(p+i) ;
      }
    }
  }
  else
  {
    p=Decomposition8( current_xiuzheng );
    for( i=0;i<8;i++ )
    {
      mmma[i]= *(p+i) ;
    }
  }
  if( Save_Flag==0 )//没有按压“存储”，或者存储没有结束
  {
    Ht1621WrOneData( disp_data_addr[0][0],disp_data[ mmma[6] ][0] );
    Ht1621WrOneData( disp_data_addr[0][1],disp_data[ mmma[6] ][1] );//
    Ht1621WrOneData( disp_data_addr[1][0],disp_data[ mmma[5] ][0] );
    Ht1621WrOneData( disp_data_addr[1][1],disp_data[ mmma[5] ][1] );//
    Ht1621WrOneData( disp_data_addr[2][0],disp_data[ mmma[4] ][0] );
    Ht1621WrOneData( disp_data_addr[2][1],disp_data[ mmma[4] ][1] );//
    Ht1621WrOneData( disp_data_addr[3][0],disp_data[ mmma[3] ][0] );
    Ht1621WrOneData( disp_data_addr[3][1],disp_data[ mmma[3] ][1] );//
    Ht1621WrOneData( disp_data_addr[4][0],disp_data[ mmma[2] ][0] );
    Ht1621WrOneData( disp_data_addr[4][1],disp_data[ mmma[2] ][1] );//
    Ht1621WrOneData( disp_data_addr[5][0],disp_data[ mmma[1] ][0] );
    Ht1621WrOneData( disp_data_addr[5][1],disp_data[ mmma[1] ][1] );//
    Ht1621WrOneData( disp_data_addr[6][0],disp_data[ mmma[0] ][0] );
    Ht1621WrOneData( disp_data_addr[6][1],disp_data[ mmma[0] ][1] );//
    Ht1621WrOneData( 12,1+2 );	//小数点+ma
    Ht1621WrOneData( 13,0 );//单位：mV---关掉
    //Ht1621WrOneData( 13,1 );//单位：mV
  }
  else
  {
    Ht1621WrOneData( disp_data_addr[0][0],disp_data[ mmma[6] ][0] );
    Ht1621WrOneData( disp_data_addr[0][1],disp_data[ mmma[6] ][1] );//
    Ht1621WrOneData( disp_data_addr[1][0],disp_data[ mmma[5] ][0] );
    Ht1621WrOneData( disp_data_addr[1][1],disp_data[ mmma[5] ][1] );//
    Ht1621WrOneData( disp_data_addr[2][0],disp_data[ mmma[4] ][0] );
    Ht1621WrOneData( disp_data_addr[2][1],disp_data[ mmma[4] ][1] );//
    Ht1621WrOneData( disp_data_addr[3][0],disp_data[ mmma[3] ][0] );
    Ht1621WrOneData( disp_data_addr[3][1],disp_data[ mmma[3] ][1] );//
    Ht1621WrOneData( disp_data_addr[4][0],disp_data[ mmma[2] ][0]+8 );  //  + 保存
    Ht1621WrOneData( disp_data_addr[4][1],disp_data[ mmma[2] ][1] );//
    Ht1621WrOneData( disp_data_addr[5][0],disp_data[ mmma[1] ][0] );
    Ht1621WrOneData( disp_data_addr[5][1],disp_data[ mmma[1] ][1] );//
    Ht1621WrOneData( disp_data_addr[6][0],disp_data[ mmma[0] ][0] );
    Ht1621WrOneData( disp_data_addr[6][1],disp_data[ mmma[0] ][1] );//
    Ht1621WrOneData( 12,1+2 );	//小数点+ma
    Ht1621WrOneData( 13,0 );//单位：mV---关掉
  }
}


/**********************************************************************************************
函数名称：
函数功能：按压采集按键后的相应显示
说    明：1、按压 采集 按键时，先在按键程序里使用HT1621_all_off();清屏
          2、按压 采集 按键时，Acquisition_flag = 1;  直到按压“工作停”按键，才将其赋值为0
          3、
          4、
时    间：2010-02-27
**********************************************************************************************/
//#define     Basic_Time      (500/2)
#define     Basic_Time      (160)
void AcquisitionControl(void)
{
  if( Acquisition_flag==1 )
  {
    Ht1621WrOneData( 0,8 );//点亮公司的标识  
    Ht1621WrOneData( 9,8 ); //点亮公司的标识 
    Ht1621WrOneData( 11,4 );	//点亮公司的标识 
    Ht1621WrOneData( 10,2 );	//点亮“采集”图标
    
    if( AcquisitionTime<=Basic_Time ) 
    {
      PositionShow();//显示点号
      Vol_Calculate_One_Time = 1;
      //GAIN_AUTO_MAX(  );
    }
    if( (AcquisitionTime>Basic_Time)&&(AcquisitionTime<=Basic_Time*2) )  Vol_Disp( VolFiltering() );//显示电压，24位AD
    
  }
}


/**********************************************************************************************
函数名称：void WorkOnControl(void)
函数功能：按压”工作开“按键后的相应显示
说    明：1、按压 ”工作开“ 按键时，先在按键程序里使用HT1621_all_off();清屏
          2、按压 采集 按键时，WorkOn_flag = 1;  直到按压“工作停”按键，才将其赋值为0
          3、
          4、
时    间：2010-02-27
**********************************************************************************************/
void WorkOnControl(void)
{
  if( WorkOn_flag==1 )
  {
    if( GUOLIU_Flag==0 )//没有发生过流
    {
      FeatureSelection_Port();//频率控制脚起作用，发射频率    
      Ht1621WrOneData( 0,8 );//点亮公司的标识  
      Ht1621WrOneData( 9,8 ); //点亮公司的标识 
      Ht1621WrOneData( 11,4 );	//点亮公司的标识 
      Ht1621WrOneData( 10,4 );	//点亮“工作”图标
      if( WorkOnTime<=Basic_Time ) 
      {
        PositionShow();//显示点号
        Cur_Calculate_One_Time  =  1;
        //GAIN_AUTO_MAX(  );
      }
      if( (WorkOnTime>Basic_Time)&&(WorkOnTime<=Basic_Time*2 -60) )  
      {
        CurrentShow( XiuZheng_Result );//显示电流
        Vol_Calculate_One_Time = 1;
      }
      if( (WorkOnTime>Basic_Time*2-60)&&(WorkOnTime<=Basic_Time*3) )  Vol_Disp( VolFiltering() );//显示电压，24位AD
    
      WorkOnControl_L;
    }
  }
  else
  {
    WorkOnControl_H;
  }
}


#endif