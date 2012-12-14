/*============================================ 头文件说明 ============================================================
头文件名称：Setting.h
头文件用途：按键设置的相关功能
================================================ END ===============================================================*/

//*********************************************   函数声明   **********************************************



void AB_shanshuo_repair(void);//为防止同时出现两个闪烁位
void MN_shanshuo_repair(void);//为防止同时出现两个闪烁位
void Position_shanshuo_repair(void);//为防止同时出现两个闪烁位

void AB_Setting(void);
void MN_Setting(void);
//void FeatureSelection_Port(void);
void FeatureSelection_Port_First(void);
void FrequencyPointSelection_Port(void);
void FeatureSelection(void);
void FrequencyPointSelection(void);
void TimeWindow(void);


void Position(void);
void PositionControl(void);
//*************************************************** END *************************************************

/*******************************************************************************
函数名称： 
函数功能： 按下 设置 键后，第一屏
时间	： 2010-2-19
说明    ： 点亮1/2AB，默认值0006.000 m  
*******************************************************************************/

void AB_Setting(void)
{
   uchar j=4;
   uint	 a;
   uchar *p;
   if( Setting_up==2 )
   {
      Setting_up=0;
	  AB_shanshuo --;
	  if( AB_shanshuo<=0 )
	     AB_shanshuo=4;
   }   
   if( Setting_down==2 )
   {
      Setting_down=0;
	  AB_shanshuo ++;
	  if( AB_shanshuo>4 )
	     AB_shanshuo=1;
   }//////////////////////////////////////////////////////闪烁位选择
   
   if( Setting_up==1 )
   {
      Setting_up=0;
	  j=j-AB_shanshuo;
	  //AB_data+=pow(10,j);//10的n次幂	
	  p= Decomposition( AB_data );
	  if( *(p+j)!=9 )
	     AB_data+=pow10(j) ;
	  else
	     AB_data = AB_data- 9*pow10(j);
   }
   if( Setting_down==1 )
   {
      Setting_down=0;
	  j=j-AB_shanshuo;
	  p= Decomposition( AB_data );
	  if( *(p+j)!=0 )
	     AB_data-=pow10(j) ;
	  else
	     AB_data = AB_data+ 9*pow10(j);
   }///////////////////////////////////////修改闪烁数字的大小
   
   
   if( second%2==0 )
   {
      Ht1621WrOneData( 0,8 );//点亮公司的标识  
   	  Ht1621WrOneData( 9,8+4 ); //点亮公司的标识  //点亮1/2AB
   	  Ht1621WrOneData( 11,4 );		//点亮公司的标识  	
   	  Ht1621WrOneData( 13,8 );	//单位：m
   	  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ 0 ][1] );//
   	  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ AB_data/1000 ][0] );
   	  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ AB_data/1000 ][1] );//
   	  a=AB_data%1000;//得到三位数
   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ a/100 ][0] );
   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ a/100 ][1] );//
   	  a=a%100; //得到两位数  
   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ a/10 ][0] );
   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ a/10 ][1] );///////默认值0006.000m
   	  Ht1621WrOneData( 12,1 );	//小数点
   	  a=a%10;//得到个位数
   	  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ a ][0] );
   	  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ a ][1] );// 
   	  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ 0 ][1] );//
   	  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ 0 ][1] );//   	  
   }
   else
   {
      Ht1621WrOneData( disp_data_addr[ AB_shanshuo ][0],0 );
      Ht1621WrOneData( disp_data_addr[ AB_shanshuo ][1],0 );//加减位闪烁
      AB_shanshuo_repair( );
   }
}

/*******************************************************************************
函数名称： void MN_Setting(void)
函数功能： 按下 设置 键后，第2屏
时间	： 2010-2-19
说明    ： 点亮1/2MN，默认值0003.000 m
*******************************************************************************/
void MN_Setting(void)
{
   uchar j=4;
   uint	 a;
   uchar *p;
   if( Setting_up==2 )
   {
      Setting_up=0;
	  AB_shanshuo --;
	  if( AB_shanshuo<=0 )
	     AB_shanshuo=4;
   }   
   if( Setting_down==2 )
   {
      Setting_down=0;
	  AB_shanshuo ++;
	  if( AB_shanshuo>4 )
	     AB_shanshuo=1;
   }//////////////////////////////////////////////////////闪烁位选择
   
   if( Setting_up==1 )
   {
      Setting_up=0;
	  j=j-AB_shanshuo;
	  //AB_data+=pow(10,j);//10的n次幂	
	  p= Decomposition( MN_data );
	  if( *(p+j)!=9 )
	     MN_data+=pow10(j) ;
	  else
	     MN_data = MN_data- 9*pow10(j);
   }
   if( Setting_down==1 )
   {
      Setting_down=0;
	  j=j-AB_shanshuo;
	  p= Decomposition( MN_data );
	  if( *(p+j)!=0 )
	     MN_data-=pow10(j) ;
	  else
	     MN_data = MN_data+ 9*pow10(j);
   }///////////////////////////////////////修改闪烁数字的大小
   
   if( second%2==0 )
   {
      Ht1621WrOneData( 0,8 );//点亮公司的标识  
      Ht1621WrOneData( 9,8+2 ); //点亮公司的标识  //点亮1/2MN
      Ht1621WrOneData( 11,4 );		//点亮公司的标识  	
      Ht1621WrOneData( 13,8 );	//单位：m
      Ht1621WrOneData( disp_data_addr[0][0],disp_data[ 0 ][0] );
      Ht1621WrOneData( disp_data_addr[0][1],disp_data[ 0 ][1] );//
   	  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ MN_data/1000 ][0] );
   	  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ MN_data/1000 ][1] );//
   	  a=MN_data%1000;//得到三位数
   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ a/100 ][0] );
   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ a/100 ][1] );//
   	  a=a%100; //得到两位数  
   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ a/10 ][0] );
   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ a/10 ][1] );///////默认值0006.000m
   	  Ht1621WrOneData( 12,1 );	//小数点
   	  a=a%10;//得到个位数
      Ht1621WrOneData( disp_data_addr[4][0],disp_data[ a ][0] );
      Ht1621WrOneData( disp_data_addr[4][1],disp_data[ a ][1] );// 
      Ht1621WrOneData( disp_data_addr[5][0],disp_data[ 0 ][0] );
      Ht1621WrOneData( disp_data_addr[5][1],disp_data[ 0 ][1] );//
      Ht1621WrOneData( disp_data_addr[6][0],disp_data[ 0 ][0] );
      Ht1621WrOneData( disp_data_addr[6][1],disp_data[ 0 ][1] );//   	  
   }
   else
   {
      Ht1621WrOneData( disp_data_addr[ AB_shanshuo ][0],0 );
      Ht1621WrOneData( disp_data_addr[ AB_shanshuo ][1],0 );//加减位闪烁 
      
      MN_shanshuo_repair();
   }
   
}

/*******************************************************************************
函数名称： void FeatureSelection(void)
函数功能： 按下 设置 键后，第3屏
时间	： 2010-2-19
说明    ： 点亮"功能选择"，默认值0000.000
*******************************************************************************/
void FeatureSelection(void)
{
   Ht1621WrOneData( 0,8 );//点亮公司的标识  
   Ht1621WrOneData( 9,8 ); //点亮公司的标识  
   Ht1621WrOneData( 11,4 );		//点亮公司的标识  	
   
   if( Setting_up==1 )
   {
      Setting_up=0;
	  FeatureSelection_data ++;
	  if( FeatureSelection_data>3 )
	     FeatureSelection_data=0;
      CheckFeatureSelection( );//对点号数组Position_data【7】进行初始化
   }
   if( Setting_down==1 )
   {
      Setting_down=0;
	  if( FeatureSelection_data!=0 )
	     FeatureSelection_data --;
	  else
	     FeatureSelection_data=3;//0 1 2 3
      CheckFeatureSelection( );//对点号数组Position_data【7】进行初始化
   }///////////////////////////////////////修改闪烁数字的大小
   //FeatureSelection_Port();//
   
   ////display
   if( second%2==0 )
   {      
   	  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ 0 ][1] );//
   	  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ 0 ][0]+8 );//点亮“功能选择”
   	  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ 0 ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ 0 ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ FeatureSelection_data ][0] );
   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ FeatureSelection_data ][1] );///////默认值0006.000m
   	  //Ht1621WrOneData( disp_data_addr[3][0],disp_data[ 9 ][0] );
   	  //Ht1621WrOneData( disp_data_addr[3][1],disp_data[ 9 ][1] );///////默认值0006.000m
   	  
	  Ht1621WrOneData( 12,1 );	//小数点
   	  
   	  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ 0 ][1] );// 
   	  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ 0 ][1] );//
   	  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ 0 ][1] );//   	  
   }
   else
   {
      Ht1621WrOneData( disp_data_addr[ 3 ][0],0 );
   	  Ht1621WrOneData( disp_data_addr[ 3 ][1],0 );//加减位闪烁   
   }
}

/**************************************************************************************
函数名称：void FeatureSelection_Port(void)
函数功能：4种输出频率的控制脚
时    间：2010-06-24
说    明：1、
          2、
控制第二路带通芯片的管脚：
单片机的14（P1.2）脚和15（P1.3）脚。
定义如下：按键按压至"功能选择"选0，打0，0
          按键按压至"功能选择"选1，打0，1
          按键按压至"功能选择"选2，打1，0
          按键按压至"功能选择"选3，打1，1

**************************************************************************************/
void FeatureSelection_Port(void)
{  
   //BinaryConversion10_2( FeatureSelection_data );////  
   BinaryConversion10_2( FrequencyPointSelection_data );//
   if( Binary_2[1]==1 )  FeatureSelection1_H;
   else  FeatureSelection1_L;
   if( Binary_2[0]==1 )  FeatureSelection0_H;
   else  FeatureSelection0_L;  
}

//控制第二路带通芯片的管脚：
void GradeTwo_DaiTong(void)
{
  BinaryConversion10_2( FrequencyPointSelection_data );//
   if( Binary_2[1]==1 )  FeatureSelection1_H;
   else  FeatureSelection1_L;
   if( Binary_2[0]==1 )  FeatureSelection0_H;
   else  FeatureSelection0_L; 
}

void FeatureSelection_Port_First(void)///初始化全低
{
  FeatureSelection1_L;FeatureSelection0_L; 
}

/*******************************************************************************
函数名称： void FrequencyPointSelection(void)
函数功能： 按下 设置 键后，第4屏---频点选择
时间	： 2010-2-19
说明    ： 点亮"频点选择"，默认值0000.000
*******************************************************************************/
void FrequencyPointSelection(void)
{
   Ht1621WrOneData( 0,8 );//点亮公司的标识  
   Ht1621WrOneData( 9,8 ); //点亮公司的标识  
   Ht1621WrOneData( 11,4 );		//点亮公司的标识  	
   
   if( Setting_up==1 )
   {
      Setting_up=0;
	  FrequencyPointSelection_data ++;
	  if( FrequencyPointSelection_data>3 )
	     FrequencyPointSelection_data=0;
   }
   if( Setting_down==1 )
   {
      Setting_down=0;
	  if( FrequencyPointSelection_data!=0 )
	     FrequencyPointSelection_data --;
	  else
	     FrequencyPointSelection_data=3;
   }///////////////////////////////////////修改闪烁数字的大小
   
   FrequencyPointSelection_Port();
   GradeTwo_DaiTong( );
   
   ///display
   if( second%2==0 )
   {      
   	  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ 0 ][1] );//
   	  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ 0 ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ 0 ][0] +8 ); //点亮“频点选择”
   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ 0 ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ FrequencyPointSelection_data ][0] );
   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ FrequencyPointSelection_data ][1] );///////默认值0006.000m
   	  //Ht1621WrOneData( disp_data_addr[3][0],disp_data[ 9 ][0] );
   	  //Ht1621WrOneData( disp_data_addr[3][1],disp_data[ 9 ][1] );///////默认值0006.000m
   	  
	  Ht1621WrOneData( 12,1 );	//小数点
   	  
   	  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ 0 ][1] );// 
   	  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ 0 ][1] );//
   	  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ 0 ][1] );//   	  
   }
   else
   {
      Ht1621WrOneData( disp_data_addr[ 3 ][0],0 );
   	  Ht1621WrOneData( disp_data_addr[ 3 ][1],0 );//加减位闪烁   
   }
}
void FrequencyPointSelection_Port(void)
{
   BinaryConversion10_2( FrequencyPointSelection_data );
   
   if( Binary_2[1]==1 )  FrequencyPointSelection1_H;
   else  FrequencyPointSelection1_L;
   if( Binary_2[0]==1 )  FrequencyPointSelection0_H;
   else  FrequencyPointSelection0_L;
}

/*******************************************************************************
函数名称： void TimeWindow(void)
函数功能： 按下 设置 键后，第5屏---时窗设置
时间	： 2010-2-19
说明    ： 1、点亮"时窗设置"，默认值0000.000 S
		   2、时窗设置条件下只有数字的第4位参与循环，可选数值为0，1，2，3。
		   3、0为按压"工作开"后P1.5及P1.6打频10S即自动停止，1为20S,2为30S，3为40S.

*******************************************************************************/
void TimeWindow(void)
{
   Ht1621WrOneData( 0,8 );//点亮公司的标识  
   Ht1621WrOneData( 9,8 ); //点亮公司的标识  
   Ht1621WrOneData( 11,4 );		//点亮公司的标识  
   
   Ht1621WrOneData( 10,1 );	//  点亮"时窗设置"
   Ht1621WrOneData( 13,2 );	//  单位：S
   
   if( Setting_up==1 )
   {
      Setting_up=0;
	  TimeWindow_data ++;
	  if( TimeWindow_data>3 )
	     TimeWindow_data=0;
   }
   if( Setting_down==1 )
   {
      Setting_down=0;
	  if( TimeWindow_data!=0 )
	     TimeWindow_data --;
	  else
	     TimeWindow_data=3;
   }///////////////////////////////////////修改闪烁数字的大小
   
   ///display
   if( second%2==0 )
   {      
   	  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ 0 ][1] );//
   	  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ 0 ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ 0 ][0]  ); 
   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ 0 ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ TimeWindow_data ][0] );
   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ TimeWindow_data ][1] );///////默认值0006.000m
   	  
	  Ht1621WrOneData( 12,1 );	//小数点
   	  
   	  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ 0 ][1] );// 
   	  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ 0 ][1] );//
   	  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ 0 ][0] );
   	  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ 0 ][1] );//   	  
   }
   else
   {
      Ht1621WrOneData( disp_data_addr[ 3 ][0],0 );
   	  Ht1621WrOneData( disp_data_addr[ 3 ][1],0 );//加减位闪烁   
   }

}

/*******************************************************************************
函数名称： void Position(void)
函数功能： 按下 设置 键后，第6屏---
时间	： 2010-2-19
说明    ： 点亮"点号"，默认值0010.001，功能与"点号"键重复，见点号键。
*******************************************************************************/
void Position(void)
{
   //uchar j=0;
   Ht1621WrOneData( 0,8 );//点亮公司的标识  
   Ht1621WrOneData( 9,8 ); //点亮公司的标识  
   Ht1621WrOneData( 11,4 );//点亮公司的标识  
   Ht1621WrOneData( 10,0 );	//点亮“采集”图标--关闭
   Ht1621WrOneData( 13,0 );//单位：mV---关掉    
   /*
   if( Setting_up==2 )  //  闪烁位
   {
      Setting_down=0;
      Position_shanshuo++;          
      if( Position_shanshuo>6 )   
        Position_shanshuo = 0;
      Position_shanshuo_Control( 0 );
   }
   if( Setting_down==2 )  
   {
      Setting_up=0;
      if( Position_shanshuo!=0 )
      {   
	     Position_shanshuo--;             
      }
      else
	     Position_shanshuo=6;
      Position_shanshuo_Control( 1 );
   } 
   */
   if( Setting_up==2 )  //  闪烁位
   {
      Setting_up = 0;
      Position_shanshuo++;          
      if( Position_shanshuo>6 )   
        Position_shanshuo = 0;
      Position_shanshuo_Control( 0 );
   }
   //////////////////////////////////////////////////////闪烁位选择
   
   if( Setting_up==1 )
   {
      Setting_up=0;
      Position_data[ Position_shanshuo ]++;
      SetScaleForPosition( 0 );      
      ZhuangHao_RST( );//////////////////
   }
   if( Setting_down==1 )
   {
      Setting_down=0;
      Position_data[ Position_shanshuo ]--;
      SetScaleForPosition( 1 );          
      ZhuangHao_RST( );////////////////////
   }///////////////////////////////////////修改闪烁数字的大小
   
   ///display
   if( second%2==0 )
   {      
   	  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ Position_data[0] ][0] );
   	  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ Position_data[0] ][1] );//
   	  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ Position_data[1] ][0] );
   	  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ Position_data[1] ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ Position_data[2] ][0]  ); 
   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ Position_data[2] ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ Position_data[3] ][0] + 8 ); // 点亮“点号”
   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ Position_data[3] ][1] );////
   	  
	  Ht1621WrOneData( 12,1 );	//小数点
   	  
   	  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ Position_data[4] ][0] );
   	  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ Position_data[4] ][1] );// 
   	  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ Position_data[5] ][0] );
   	  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ Position_data[5] ][1] );//
   	  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ Position_data[6] ][0] );
   	  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ Position_data[6] ][1] );//  
          
           
   }
   else//加减位闪烁 
   {      
      if( Position_shanshuo==3 )  //  为了防止"点号"两个字也闪烁
      {
        Ht1621WrOneData( disp_data_addr[ Position_shanshuo ][0],0+8 );  //  "点号"
        Ht1621WrOneData( disp_data_addr[ Position_shanshuo ][1],0 );//加减位闪烁 
      }
      else
      {
        Ht1621WrOneData( disp_data_addr[ Position_shanshuo ][0],0 );
        Ht1621WrOneData( disp_data_addr[ Position_shanshuo ][1],0 );
      }
      Position_shanshuo_repair( );
   }
   
}

/*******************************************************************************
函数名称： void PositionControl(void)
函数功能： 按键；；；点号
时间	： 2010-2-19
说明    ： 
*******************************************************************************/
void PositionControl(void)
{
   if( Position_flag==1 )
   {
      Position();
   }
}

/*******************************************************************************
函数名称： void SettingControl(void)
函数功能： 设置按键的6个屏
时间	： 2010-2-19
说明    ： 
*******************************************************************************/
void SettingControl(void)
{
   /*
   switch( Setting_flag )
   {
      case 1: AB_Setting();
	  	   	  break;
	  case 2:
	  case 3:
	  case 4:
	  case 5:
	  case 6:
	  default:
	         break;
   }
   */
   if( Setting_flag==1 ) AB_Setting();
   if( Setting_flag==2 ) MN_Setting();
   if( Setting_flag==3 ) FeatureSelection();
   if( Setting_flag==4 ) FrequencyPointSelection();//该功能取消
   //if( Setting_flag==4 ) Setting_flag = 5 ;
   if( Setting_flag==5 ) TimeWindow();
   if( Setting_flag==6 ) Position();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AB_shanshuo_repair(void)//为防止同时出现两个闪烁位
{
  uint a=0;
  if( AB_shanshuo_cmp!=AB_shanshuo )
  {
    AB_shanshuo_cmp = AB_shanshuo ;
    
    Ht1621WrOneData( 0,8 );//点亮公司的标识  
    Ht1621WrOneData( 9,8+4 ); //点亮公司的标识  //点亮1/2AB
    Ht1621WrOneData( 11,4 );		//点亮公司的标识  	
    Ht1621WrOneData( 13,8 );	//单位：m
    Ht1621WrOneData( disp_data_addr[0][0],disp_data[ 0 ][0] );
    Ht1621WrOneData( disp_data_addr[0][1],disp_data[ 0 ][1] );//
    Ht1621WrOneData( disp_data_addr[1][0],disp_data[ AB_data/1000 ][0] );
    Ht1621WrOneData( disp_data_addr[1][1],disp_data[ AB_data/1000 ][1] );//
    a=AB_data%1000;//得到三位数
    Ht1621WrOneData( disp_data_addr[2][0],disp_data[ a/100 ][0] );
    Ht1621WrOneData( disp_data_addr[2][1],disp_data[ a/100 ][1] );//
    a=a%100; //得到两位数  
    Ht1621WrOneData( disp_data_addr[3][0],disp_data[ a/10 ][0] );
    Ht1621WrOneData( disp_data_addr[3][1],disp_data[ a/10 ][1] );///////默认值0006.000m
    Ht1621WrOneData( 12,1 );	//小数点
    a=a%10;//得到个位数
    Ht1621WrOneData( disp_data_addr[4][0],disp_data[ a ][0] );
    Ht1621WrOneData( disp_data_addr[4][1],disp_data[ a ][1] );// 
    Ht1621WrOneData( disp_data_addr[5][0],disp_data[ 0 ][0] );
    Ht1621WrOneData( disp_data_addr[5][1],disp_data[ 0 ][1] );//
    Ht1621WrOneData( disp_data_addr[6][0],disp_data[ 0 ][0] );
    Ht1621WrOneData( disp_data_addr[6][1],disp_data[ 0 ][1] );// 
  }
}

void MN_shanshuo_repair(void)//为防止同时出现两个闪烁位
{
  uint  a=0;
  if( MN_shanshuo_cmp!=AB_shanshuo )
  {
      MN_shanshuo_cmp = AB_shanshuo ;
    
      Ht1621WrOneData( 0,8 );//点亮公司的标识  
      Ht1621WrOneData( 9,8+2 ); //点亮公司的标识  //点亮1/2MN
      Ht1621WrOneData( 11,4 );		//点亮公司的标识  	
      Ht1621WrOneData( 13,8 );	//单位：m
      Ht1621WrOneData( disp_data_addr[0][0],disp_data[ 0 ][0] );
      Ht1621WrOneData( disp_data_addr[0][1],disp_data[ 0 ][1] );//
      Ht1621WrOneData( disp_data_addr[1][0],disp_data[ MN_data/1000 ][0] );
      Ht1621WrOneData( disp_data_addr[1][1],disp_data[ MN_data/1000 ][1] );//
      a=MN_data%1000;//得到三位数
      Ht1621WrOneData( disp_data_addr[2][0],disp_data[ a/100 ][0] );
      Ht1621WrOneData( disp_data_addr[2][1],disp_data[ a/100 ][1] );//
      a=a%100; //得到两位数  
      Ht1621WrOneData( disp_data_addr[3][0],disp_data[ a/10 ][0] );
      Ht1621WrOneData( disp_data_addr[3][1],disp_data[ a/10 ][1] );///////默认值0006.000m
      Ht1621WrOneData( 12,1 );	//小数点
      a=a%10;//得到个位数
      Ht1621WrOneData( disp_data_addr[4][0],disp_data[ a ][0] );
      Ht1621WrOneData( disp_data_addr[4][1],disp_data[ a ][1] );// 
      Ht1621WrOneData( disp_data_addr[5][0],disp_data[ 0 ][0] );
      Ht1621WrOneData( disp_data_addr[5][1],disp_data[ 0 ][1] );//
      Ht1621WrOneData( disp_data_addr[6][0],disp_data[ 0 ][0] );
      Ht1621WrOneData( disp_data_addr[6][1],disp_data[ 0 ][1] );//  
  }
}
/* 功能选择只有一个闪烁位，所以不用此函数
void FeatureSelection_shanshuo_repair(void)//为防止同时出现两个闪烁位
{  
}
*/

void Position_shanshuo_repair(void)//为防止同时出现两个闪烁位
{
    if( Position_shanshuo_cmp!=Position_shanshuo )
    {
          Position_shanshuo_cmp = Position_shanshuo ;
          
          Ht1621WrOneData( disp_data_addr[0][0],disp_data[ Position_data[0] ][0] );
   	  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ Position_data[0] ][1] );//
   	  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ Position_data[1] ][0] );
   	  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ Position_data[1] ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ Position_data[2] ][0]  ); 
   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ Position_data[2] ][1] );//
   	  
   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ Position_data[3] ][0] + 8 ); // 点亮“点号”
   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ Position_data[3] ][1] );////
   	  
	  Ht1621WrOneData( 12,1 );	//小数点
   	  
   	  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ Position_data[4] ][0] );
   	  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ Position_data[4] ][1] );// 
   	  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ Position_data[5] ][0] );
   	  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ Position_data[5] ][1] );//
   	  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ Position_data[6] ][0] );
   	  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ Position_data[6] ][1] );//  
    }
}



