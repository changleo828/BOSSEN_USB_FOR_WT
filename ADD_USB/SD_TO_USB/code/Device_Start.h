/*============================================ 头文件说明 ============================================================
头文件名称：
头文件用途：开机后单片机
================================================ END ===============================================================*/

//*********************************************   函数声明   **********************************************
void Disp_0_9(void);
void Started(void);
void aStatic_Show(void);
void First_Check(void);
void PositionRecordInit(void);
void ShowNextPosition(void);
void ShowCurrentPosition(void);
//*************************************************** END *************************************************

/*******************************************************************************
函数名称： void Disp_0_9(void)
函数功能： 开机后，7个数码管统一显示 0-9
说明    ： 
*******************************************************************************/
void Disp_0_9(void)
{
  unsigned char i;
  HT1621_all_off();
  Ht1621WrOneData( 0,8 );//点亮公司的标识 
  Ht1621WrOneData( 9,8 ); //点亮公司的标识 
  Ht1621WrOneData( 11,4 );		//点亮公司的标识  
  Ht1621WrOneData( 12,1 );	//小数点
  for(i=0;i<10;i++)
  //for(j=0;j<10;j++)
  {
      Ht1621WrOneData( disp_data_addr[0][0],disp_data[i][0] );
      Ht1621WrOneData( disp_data_addr[0][1],disp_data[i][1] );//第一个 8
      Ht1621WrOneData( disp_data_addr[1][0],disp_data[i][0] );
  	 Ht1621WrOneData( disp_data_addr[1][1],disp_data[i][1] );//第2个 8
	 Ht1621WrOneData( disp_data_addr[2][0],disp_data[i][0] );
  	 Ht1621WrOneData( disp_data_addr[2][1],disp_data[i][1] );//第3个 8
	 Ht1621WrOneData( disp_data_addr[3][0],disp_data[i][0] );
  	 Ht1621WrOneData( disp_data_addr[3][1],disp_data[i][1] );//第4个 8
	 Ht1621WrOneData( disp_data_addr[4][0],disp_data[i][0] );
  	 Ht1621WrOneData( disp_data_addr[4][1],disp_data[i][1] );//第5个 8
	 Ht1621WrOneData( disp_data_addr[5][0],disp_data[i][0] );
  	 Ht1621WrOneData( disp_data_addr[5][1],disp_data[i][1] );//第6个 8
	 Ht1621WrOneData( disp_data_addr[6][0],disp_data[i][0] );
  	 Ht1621WrOneData( disp_data_addr[6][1],disp_data[i][1] );//第7个 8
	 //DelayMS(7000);
    DelayMS(4000);	 
  }
}
/*******************************************************************************
函数名称： void Started(void)
函数功能： 开机后，显示完成0-9后，停止在此状态
说明    ： 数字回0并闪烁2次
*******************************************************************************/
void Started(void)
{
   uchar j=0;
   HT1621_all_off();
   Ht1621WrOneData( 0,8 );//点亮公司的标识 
   Ht1621WrOneData( 9,8 ); //点亮公司的标识 
   Ht1621WrOneData( 11,4 );		//点亮公司的标识  
   DelayMS(9000);   
   for( j=0;j<7;j++ )
   {
      Ht1621WrOneData( disp_data_addr[j][0],disp_data[0][0] );
	  Ht1621WrOneData( disp_data_addr[j][1],disp_data[0][1] );
   }
   Ht1621WrOneData( 12,1 );	//小数点
   DelayMS(9000);
   HT1621_all_off();
   
   Ht1621WrOneData( 0,8 );//点亮公司的标识 
   Ht1621WrOneData( 9,8 ); //点亮公司的标识 
   Ht1621WrOneData( 11,4 );		//点亮公司的标识  
   DelayMS(9000);
   for( j=0;j<7;j++ )
   {
      Ht1621WrOneData( disp_data_addr[j][0],disp_data[0][0] );
	  Ht1621WrOneData( disp_data_addr[j][1],disp_data[0][1] );
   }
   Ht1621WrOneData( 12,1 );	//小数点
}

/*********************************************************************************************************
函数名称：void Static_Show(void)
函数功能：静态显示 7 个“0”
说    明：
时    间：2010-02-28
*********************************************************************************************************/
void aStatic_Show(void)
{
   uchar j=0;
   Ht1621WrOneData( 0,8 );//点亮公司的标识 
   Ht1621WrOneData( 9,8 ); //点亮公司的标识 
   Ht1621WrOneData( 11,4 );		//点亮公司的标识  
   for( j=0;j<7;j++ )
   {
      Ht1621WrOneData( disp_data_addr[j][0],disp_data[0][0] );
	  Ht1621WrOneData( disp_data_addr[j][1],disp_data[0][1] );
   }
   Ht1621WrOneData( 12,1 );	//小数点
}

/*********************************************************************************************************
函数名称：void First_Check(void)
函数功能：开机自检
时    间：2010-02-28
说    明：1、读取单片机内部 Flash 的内容，并且对其进行初始化；
          2、读取 SD 卡内的相关记录信息；
*********************************************************************************************************/
void First_Check(void)
{
  //uchar i=0;
  //uchar pig[64];
  /*
  read_array((uchar *)BIAOXIAO_save, &memory[0],64);//---->开机后，将信息放入 memory[64]
  if( memory[0]==255 )//第一次开机
  {
    for( i=0;i<64;i++ )
      memory[i]=0;
    Write_Flash_String(BIAOXIAO_save,memory,128);
  }
  */
  ///////////////////////////////////////////////////////////////////////
  AutoZengYiRun.which_grade_yichu = 0;//自动增益的初始化
  AutoZengYiRun.ZengYi_Ever_Used = 0;
  //////////////////////////////////////////////////////////////////////
  PositionRecordInit( );//点号的临时记录，初始化  
}

void PositionRecordInit(void)//点号的临时记录，初始化
{
  PositionRecordRun.PositionOu_InFeatureSelection_0 = 1;
  PositionRecordRun.PositionA_InFeatureSelection_0 = 1;
  PositionRecordRun.PositionOu_InFeatureSelection_1 = 1;
  PositionRecordRun.PositionA_InFeatureSelection_1 = 1;
  PositionRecordRun.PositionOu_InFeatureSelection_2 = 1;
  PositionRecordRun.PositionA_InFeatureSelection_2 = 1;
  PositionRecordRun.PositionOu_InFeatureSelection_3 = 1;
  PositionRecordRun.PositionA_InFeatureSelection_3 = 1;
  PositionRecordRun.NumberOfXiangDao = 0;
}

/*********************************************************************************************************
函数名称：void ShowNextPosition(void)
函数功能：按压“存储”后，屏上显示下一个点号，不显示全0----
创建时间：2010-09-06
修改时间：
说    明：1、在函数 SaveUseWorkStop(void) 里使用；
*********************************************************************************************************/
void ShowNextPosition(void)
{
  //uchar j=0;
  Ht1621WrOneData( 0,8 );//点亮公司的标识 
  Ht1621WrOneData( 9,8 ); //点亮公司的标识 
  Ht1621WrOneData( 11,4 );		//点亮公司的标识 
  /*
  for( j=0;j<7;j++ )
  {
    Ht1621WrOneData( disp_data_addr[j][0],disp_data[ Position_data[j] ][0] );
    Ht1621WrOneData( disp_data_addr[j][1],disp_data[ Position_data[j] ][1] );
  }
  */
  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ Position_data[0] ][0] );
  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ Position_data[0] ][1] );//
  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ Position_data[1] ][0] );
  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ Position_data[1] ][1] );//
   	  
  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ Position_data[2] ][0]  ); 
  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ Position_data[2] ][1] );//
   	  
  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ Position_data[3] ][0] + 8 ); // 点亮“点号”
  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ Position_data[3] ][1] );//
   	  
  Ht1621WrOneData( 12,1 );	//小数点
   	  
  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ Position_data[4] ][0]  ); //  
  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ Position_data[4] ][1] );// 
  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ Position_data[5] ][0] );
  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ Position_data[5] ][1] );//
  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ Position_data[6] ][0] );
  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ Position_data[6] ][1] );//
}

/*********************************************************************************************************
函数名称：void ShowCurrentPosition(void)
函数功能：按压“工作停”后，屏上显示当前的点号，不显示全0----
创建时间：2010-09-06
修改时间：
说    明：1、在函数 key_use1(void) 里使用；
*********************************************************************************************************/
void ShowCurrentPosition(void)
{
  Ht1621WrOneData( 0,8 );//点亮公司的标识 
  Ht1621WrOneData( 9,8 ); //点亮公司的标识 
  Ht1621WrOneData( 11,4 );		//点亮公司的标识 
  ////
  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ Position_data[0] ][0] );
  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ Position_data[0] ][1] );//
  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ Position_data[1] ][0] );
  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ Position_data[1] ][1] );//   	  
  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ Position_data[2] ][0]  ); 
  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ Position_data[2] ][1] );//   	  
  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ Position_data[3] ][0] + 8 ); // 点亮“点号”
  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ Position_data[3] ][1] );//   	  
  Ht1621WrOneData( 12,1 );	//小数点   	  
  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ Position_data[4] ][0]  ); //  
  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ Position_data[4] ][1] );// 
  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ Position_data[5] ][0] );
  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ Position_data[5] ][1] );//
  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ Position_data[6] ][0] );
  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ Position_data[6] ][1] );//
}