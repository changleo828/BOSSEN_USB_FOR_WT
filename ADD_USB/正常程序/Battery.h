/*============================================ 头文件说明 ============================================================
头文件名称：Battery.h
头文件用途：1、显示电池电量
            2、电池的欠压保护
            3、发射极的过流保护
说      明：1、P6.2/A2，电池电压采集脚。
            2、分压电阻：  ，分压比例：
            3、
时      间：2010-02-27
================================================ END ===============================================================*/

//*********************************************   函数声明   **********************************************
void Current_Protect(void);
void BatteryShow(void);
void GUOLIU_Show(void);
//*************************************************** END *************************************************


/**********************************************************************************************
函数名称：void BatteryShow(void)
函数功能：电池电量显示
说    明：1、
          2、
          3、
          4、
创建时间：2010-02-27
修改时间：2010-05-19
**********************************************************************************************/
//以下5个值，均为AD值
#define   bat4    ( ( ((float)(4095))*((float)(12600))*(Proportional_Battery) )/MCU_Ref )//4格电对应的AD值
#define   bat3    ( ( ((float)(4095))*((float)(12000))*(Proportional_Battery) )/MCU_Ref )//3格电
#define   bat2    ( ( ((float)(4095))*((float)(11000))*(Proportional_Battery) )/MCU_Ref )//2格电
#define   bat1    ( ( ((float)(4095))*((float)(9500))*(Proportional_Battery) )/MCU_Ref )//1格电
#define   bat0    ( ( ((float)(4095))*((float)(9000))*(Proportional_Battery) )/MCU_Ref )//0格电
void BatteryShow(void)
{  
  //A2result = bat0-5;////////////////////try
  if( time%199==0 )
  {
  if( A2result>bat4 )
  {
    Ht1621WrOneData( 20,1 );//电池外圈
    Ht1621WrOneData( 21,1 );//电池第4格电
    Ht1621WrOneData( 22,1 );//电池第3格电
    Ht1621WrOneData( 23,1 );//电池第2格电
    Ht1621WrOneData( 24,1 );//电池第1格电
  }
  if( (A2result>bat3)&&(A2result<=bat4) )
  {
    Ht1621WrOneData( 20,1 );//电池外圈 
    Ht1621WrOneData( 21,0 );//电池第4格电
    Ht1621WrOneData( 22,1 );//电池第3格电
    Ht1621WrOneData( 23,1 );//电池第2格电
    Ht1621WrOneData( 24,1 );//电池第1格电
  }
  if( (A2result>bat2)&&(A2result<=bat3) )
  {
    Ht1621WrOneData( 20,1 );//电池外圈
    Ht1621WrOneData( 21,0 );//电池第4格电
    Ht1621WrOneData( 22,0 );//电池第3格电
    Ht1621WrOneData( 23,1 );//电池第2格电
    Ht1621WrOneData( 24,1 );//电池第1格电
  }
  if( (A2result>bat1)&&(A2result<=bat2) )
  {
    Ht1621WrOneData( 20,1 );//电池外圈 
    Ht1621WrOneData( 21,0 );//电池第4格电
    Ht1621WrOneData( 22,0 );//电池第3格电
    Ht1621WrOneData( 23,0 );//电池第2格电
    Ht1621WrOneData( 24,1 );//电池第1格电
  }
  if( (A2result>bat0)&&(A2result<=bat1) )
  {
    if( second%2==0 )
    {
      Ht1621WrOneData( 20,1 );//电池外圈
      Ht1621WrOneData( 21,0 );//电池第4格电
      Ht1621WrOneData( 22,0 );//电池第3格电
      Ht1621WrOneData( 23,0 );//电池第2格电
      Ht1621WrOneData( 24,0 );//电池第1格电
    }
    else
    {
      Ht1621WrOneData( 20,0 );//电池外圈
      Ht1621WrOneData( 21,0 );//电池第4格电
      Ht1621WrOneData( 22,0 );//电池第3格电
      Ht1621WrOneData( 23,0 );//电池第2格电
      Ht1621WrOneData( 24,0 );//电池第1格电
    }
  }
  }
  if( A2result<=bat0 )  //  ≤9.0V自动关机，单片机P4.7口输出低
  {//欠压保护脚不应该被屏蔽掉，这里只是临时性的
    
    MCU_Start_flag = 0;    //  单片机关闭
    QIANYABAOHU;          //  欠压保护脚,当单片机61脚（A2）采集到的电池电压≤9V时输出低。
    
    if( second%2==0 )
    {
      Ht1621WrOneData( 20,1 );//电池外圈
      Ht1621WrOneData( 21,0 );//电池第4格电
      Ht1621WrOneData( 22,0 );//电池第3格电
      Ht1621WrOneData( 23,0 );//电池第2格电
      Ht1621WrOneData( 24,0 );//电池第1格电
    }
    else
    {
      Ht1621WrOneData( 20,0 );//电池外圈
      Ht1621WrOneData( 21,0 );//电池第4格电
      Ht1621WrOneData( 22,0 );//电池第3格电
      Ht1621WrOneData( 23,0 );//电池第2格电
      Ht1621WrOneData( 24,0 );//电池第1格电
    }
  }
}

/**********************************************************************************************
函数名称：void Current_Protect(void)
函数功能：发射极的过流保护
说    明：1、运行于“工作开”
          2、如果不是“工作开”，过流保护脚拉低
时    间：2010-02-27
**********************************************************************************************/
void Current_Protect(void)//这种保护可以自恢复吗？？？？？？
{
  /*
  if( WorkOn_flag==1 )
  {
    if( XiuZheng_Result > Current_Max )//发生过流
    {
      GUOLIU;
      GUOLIU_Flag = 1;
      GUOLIU_Show( );//显示“FULLAAA”。
    }
    else
    {
      GUOLIU_Normal;
      GUOLIU_Flag = 0;
    }
  }
  else
  {
    GUOLIU;
  }
  */
  /*
  if( WorkOn_flag==1 )/////2010-10-03 修改
  {
    if( XiuZheng_Result > Current_Max )//发生过流
    {
      GUOLIU;
      GUOLIU_Flag = 1;
      GUOLIU_Show( );//显示“FULLAAA”。
    }
    else
    {
      //GUOLIU_Normal;
      GUOLIU;
      GUOLIU_Flag = 0;
    }
  }
  else
  {
    //GUOLIU;
    GUOLIU_Normal;
  }
  */
  if( WorkOn_flag==1 )  //  2010-10-03
  {
    if( XiuZheng_Result > Current_Max )//发生过流
    {
      GUOLIU;
      GUOLIU_Flag = 1;
      GUOLIU_Show( );//显示“FULLAAA”。
    }
    else
    {
      GUOLIU_Normal;
      GUOLIU_Flag = 0;
    }
  }
  else
  {
    GUOLIU;
  }
  
}

/**********************************************************************************************
函数名称：void GUOLIU_Show(void)
函数功能：过流后，屏显为“FULLAAA”。
说    明：
创建时间：2010-09-06
修改时间：
**********************************************************************************************/
void GUOLIU_Show(void)
{
          Ht1621WrOneData( disp_data_addr[0][0],7 );
          Ht1621WrOneData( disp_data_addr[0][1],8 );//
          Ht1621WrOneData( disp_data_addr[1][0],5 );
          Ht1621WrOneData( disp_data_addr[1][1],7 );//
          Ht1621WrOneData( disp_data_addr[2][0],5 );
          Ht1621WrOneData( disp_data_addr[2][1],1 );//
          Ht1621WrOneData( disp_data_addr[3][0],5 );
          Ht1621WrOneData( disp_data_addr[3][1],1 );//
          
          Ht1621WrOneData( disp_data_addr[4][0],7 );
          Ht1621WrOneData( disp_data_addr[4][1],14 );//
          Ht1621WrOneData( disp_data_addr[5][0],7 );
          Ht1621WrOneData( disp_data_addr[5][1],14 );//
          Ht1621WrOneData( disp_data_addr[6][0],7 );
          Ht1621WrOneData( disp_data_addr[6][1],14 );//
          Ht1621WrOneData( 13,0 );//单位：mV
          Ht1621WrOneData( 12,0 );	//小数点+ma 屏蔽掉
          Ht1621WrOneData( 10,0 );	//点亮“工作”图标-屏蔽掉
}
