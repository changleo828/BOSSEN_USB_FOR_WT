/*============================================ 头文件说明 ============================================================
头文件名称：ZengYi.h
头文件用途：增益控制
================================================ END ===============================================================*/

#define		A0_H	  P3OUT |= BIT1
#define		A0_L	  P3OUT &=~ BIT1
#define		A1_H	  P3OUT |= BIT0
#define		A1_L	  P3OUT &=~ BIT0
#define		A2_H	  P4OUT |= BIT7
#define		A2_L	  P4OUT &=~ BIT7
#define		A3_H	  P5OUT |= BIT0
#define		A3_L	  P5OUT &=~ BIT0
#define		A4_H	  P1OUT |= BIT0
#define		A4_L	  P1OUT &=~ BIT0
#define		A5_H	  P1OUT |= BIT1
#define		A5_L	  P1OUT &=~ BIT1
//*********************************************   函数声明   **********************************************
void BinaryConversion10_2(uchar data);
void GainControl(void);
void GainControlFirst(void);
void GAIN_AUTO(unsigned char zengyi);//只在自动增益中起作用
void GAIN_AUTO_MAX( void );//将自动增益值设为最大级数
//*************************************************** END *************************************************

/*******************************************************************************
函数名称： void BinaryConversion10_2(uchar data)
函数功能： 十进制转换成二进制
作者	： 猪的传说
创建时间： 2010-2-19
修改时间： 2010-04-28
说明    ： 数组 Binary_2 中存放转换后的二进制，从高位到低位：Binary_2[DIGIT-1]、
		   Binary_2[DIGIT-2]、......Binary_2[0] 。
*******************************************************************************/
#define BASE   2        //*要转换成几进制数/* 
#define DIGIT  4     //*转换后，数的位数/* 
uchar Binary_2[DIGIT];
void BinaryConversion10_2(uchar data)
{
   uchar i=0;
   uchar mmm=0;
   //for( i=0;data!=0;i++ )//这样有问题：如果 data 先赋值1，则Binary_2[0]==1；紧接着，将 data 改为0，就会使全局变量数组中的 Binary_2[0] 没有清零。
   for( i=0;i<4;i++ )
   {
      mmm=data%2;
      data/=BASE;
	  Binary_2[i]=mmm;
   }
}

/*******************************************************************************
函数名称： void GainControl(void)
函数功能： 对增益控制脚进行控制，拉高或者拉低
作者	： 猪的传说
时间	： 2010-4-26
说明    ： 1、控制脚，从高到低依次为:P1.0 P1.1 P1.2 P1.3
                                  A0   A1   A2   A3、
           2、改为两级增益控制
*******************************************************************************/
void GainControl(void)
{
   if( ZengYi_flag==1 )
   {
      //memset( Binary_2,0,4 );      
      
      if( ZengYi_disp<=3 )//第 3 级增益(最靠近AD7711的增益)起作用，第二级全为0
      {          
          BinaryConversion10_2( ZengYi_disp );
          switch( Binary_2[DIGIT-4] )
          {
          //case 0:	A4_L;	break;
	  //case 1:	A4_H;	break;
          case 0:	A5_L;	break;
	  case 1:	A5_H;	break;  
	  default:			break;			  
          }
          switch( Binary_2[DIGIT-3] )
          {          
          //case 0:	A5_L;	break;
	  //case 1:	A5_H;	break;
          case 0:	A4_L;	break;
	  case 1:	A4_H;	break;
	  default:			break;			  
          }
          A0_L; //  第二即与第1级清零
          A1_L;
          A2_L;
          A3_L;
      }
      else
        if( ZengYi_disp<=6 )//控制第二级增益，第一级全为1
        {
          A4_H; //  第三级全高
          A5_H;
          A2_L; //  第一极全低
          A3_L;
          BinaryConversion10_2( ZengYi_disp-4 +1 );//不要 0 0 档
          switch( Binary_2[DIGIT-4] )
          {
          //case 0:	A0_L;	break;
	  //case 1:	A0_H;	break;
          case 0:	A1_L;	break;
	  case 1:	A1_H;	break;  
	  default:			break;			  
          }
          switch( Binary_2[DIGIT-3] )
          {          
          //case 0:	A1_L;	break;
	  //case 1:	A1_H;	break;
          case 0:	A0_L;	break;
	  case 1:	A0_H;	break;
	  default:			break;			  
          }
        }
        else//,ZengYi_disp>6
          if( ZengYi_disp<=9 )  //  屏显7,8,9////第一级增益
          {
            A0_H; //  第3级与第二级全高
            A1_H;
            A4_H;
            A5_H;
            BinaryConversion10_2( ZengYi_disp-7 +1 );//不要 0 0 档
            switch( Binary_2[DIGIT-4] )
            {
            case 0:	A2_L;	break;
	    case 1:	A2_H;	break;
	    default:			break;			  
            }
            switch( Binary_2[DIGIT-3] )
            {          
            case 0:	A3_L;	break;
	    case 1:	A3_H;	break;
	    default:			break;			  
            }
          }
          else  //  10 11 12
          {
            A0_H;
            A1_H;
            A3_H;
            A2_H;
            A4_H;
            A5_H;
          }          
   }
}

/*******************************************************************************
函数名称： void GainControlFirst(void)
函数功能： MCU上电后初始化，
作者	： 猪的传说
时间	： 2010-2-19
说明    ： 1、控制脚，从高到低依次为:P1.0 P1.1 P1.2 P1.3
           2、由于上电后 ZengYi_disp==0 ，所以直接将四个管脚拉低。
*******************************************************************************/
void GainControlFirst(void)
{   
   A0_L;
   A1_L;
   A2_L;
   A3_L;
   A4_L; //  第三极全低
   A5_L;
}

///////////////////////////////////////////////////////////////////////////////////

/*********************************************************************************************************
函数名称：void GAIN_AUTO(unsigned char zengyi)
函数功能：自动增益中对增益值进行设定。
说    明：      
作    者：猪的传说
创建时间：2010-07-12
修改时间：
*********************************************************************************************************/
void GAIN_AUTO(unsigned char zengyi)//只在自动增益中起作用
{//使引脚都是 高
  //ZengYi_disp = ZengYi_MAX;/////直接打到最高一级增益
  ZengYi_disp = zengyi;
  ZengYi_flag = 1;
  GainControl();
  ZengYi_flag = 0;
}

/*********************************************************************************************************
函数名称：void GAIN_AUTO_MAX( void )
函数功能：自动增益方式下，将增益值变为最大值。
说    明：该函数在以下几点被调用：1、key_use1(void)                                
                                  2、SaveControl(void)
          自动增益时，当检测完一个点后，由于现场操作的限制，使得工作人员必须用手接触金属桩，使得信号极大，
          而进入下一个点的检测时，只要重新按压“工作开”或者“采集”，就可以表明：正在检测一个新的“点”。
作    者：猪的传说
创建时间：2010-07-12
修改时间：
*********************************************************************************************************/
void GAIN_AUTO_MAX( void )
{
  if( AutoZengYiRun.ZengYi_Ever_Used == 0 )
    GAIN_AUTO( ZengYi_MAX-1 );
}


