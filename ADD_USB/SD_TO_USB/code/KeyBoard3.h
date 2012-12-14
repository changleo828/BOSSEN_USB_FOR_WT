/*============================================ 头文件说明 ============================================================
头文件名称：KeyBoard.h
头文件用途：矩阵式键盘，3行4列
创作时间  ：2010-02-18
说明	  ：P2.6----第1行
		  	P2.5----第2行
			P2.4----第3行
			P2.3----第4列
			P2.2----第3列
			P2.1----第2列
			P2.0----第1列
================================================ END ===============================================================*/

//*********************************************   函数声明   **********************************************
void keyboard_RST(void);
void FeatureSelectionPortReset(void);
void key_flag_del(void);
void key_time_calculate(void);
void key_scan(void);
void key_use1(void);
void key_use(void);
void SaveUseWorkStop(void);
//*************************************************** END *************************************************

/*******************************************************************************************************
函数名称：	void key_RST(void)
函数功能：	键盘复位
修改日期：      2010-02-28
说    明：      1、键值 keyboard_value 清零
·              2、按键计时器 
*******************************************************************************************************/
void keyboard_RST(void)
{
  keytime_start	 = 0;//关闭计时，此处是为了保证按键不松下，而数值会继续变化
  keyboard_value = 0; 
  keytime	 = 0;
  keytime_save   = 0;  
  keylong_flag   = 0;
}

/*******************************************************************************************************
函数名称：	void key_flag_del(void)
函数功能：	清楚键盘的标志位
修改日期：      2010-02-28
说    明：      1、标志位不全，因为有的按键功能还没有加上；
·              2、倘若有一个标志位没有清零，则执行“清屏”。
*******************************************************************************************************/
void key_flag_del(void)
{
  uchar sum=0;
  sum = WorkOn_Transmit_STA + WorkOn_flag + Acquisition_flag + Position_flag + Setting_flag + ZengYi_flag + Rst_flag;
  
  if( sum!=0 )  //  倘若有一个标志位没有清零，则执行“清屏”。
    HT1621_all_off();	
  
  WorkOn_Transmit_STA     =       0;      //  频率发射开始标志
  WorkOn_flag             =       0;      //  按键”工作开“。
  Acquisition_flag        =       0;      //采集
  Position_flag	          =	  0;	  //按键 --点号
  Setting_flag	          =	  0;	  //按键 设置。
  ZengYi_flag		  =	  0;	  //”增益“
  Rst_flag		  =	  0;      //复位  
}

/*******************************************************************************************************
函数名称：	void key_scan(void)	
函数功能：	矩阵式键盘按键扫描
修改日期：      2010-02-17
说    明：      1、确认是否有按键按下；
                2、得到按键的键值；
*******************************************************************************************************/
void key_scan(void)
{
	
	uchar X,Y;
	uchar ZZ; 
	uchar key;
	P2SEL = 0X00;
	P2DIR = 0XFF;     // 设置方向：输出
	P2OUT = 0x8F;	  // P2.6-P2.4置0，其余置1
	P2DIR = 0XF0;     // 设置方向：输入
	key   = P2IN;	  // 读取P2管脚状态
	key   = key<<1;		//左移一位，去掉P2.7
	
	if(key!=0x1E)	//	0x1E	=	0B00011110	
	{	 	
		//DelayMS(2100);//DelayMS(100);		
		DelayMS(1500);
                key=P2IN;	  // 读取P2管脚状态
		key=key<<1;		//左移一位，去掉P2.7
		if(key!=0x1E)//确认有按键按下
		{
			if( keylong_flag == 0 ) keyon=1; // 确认有按键按下的标志
                        //keyon=1;
		 	X=key;	//存储“列”
			P2OUT = 0x1F;	  // P2.4置1，其余置0			
			key	  =	P2IN;	  // 读取P2管脚状态
			key	  =	key<<4;		  //左移一位，去掉P2.7,P2.6,P2.5,P2.4;
			if( key==0xF0 )	  // P2.4 所在的行
			{
			   Y=0xC0;
			}
			else
			{			   
			   P2OUT = 0x2F;	  // P2.5置1，其余置0			   
			   key	 = P2IN;	  // 读取P2管脚状态
			   key   = key<<4;		  //左移一位，去掉P2.7,P2.6,P2.5,P2.4;
			   if( key==0xF0 )	  // P2.5 所在的行
			   {
			      Y=0xA0;
			   }
			   else//直接使用P2.6所在的行
			   {			      
			   	  P2OUT = 0x4F;	  // P2.6置1，其余置0				  
			   	  key	= P2IN;	  // 读取P2管脚状态
			   	  key   = key<<4;		  //左移一位，去掉P2.7,P2.6,P2.5,P2.4;
				  if( key==0xF0 )	  // P2.5 所在的行
				     Y=0x60;
			   }
			}			
			ZZ=X|Y;           //取出键值		
			
			//switch ( ZZ )     //判断键值（那一个键按下）
			{
			 	
                                if( ZZ==0x7C )  {//line:1, column:1
							   keyboard_value	=	0X7C; 							 
							}					
				 if( ZZ==0x7A )	{//line:1, column:2					 				   
								keyboard_value	=	0X7A; 
							}
				if( ZZ==0x76 )	{//line:1, column:3
					 			keyboard_value	=	0X76; 
							}
				if( ZZ==0x6E )	{//line:1, column:4
					 			keyboard_value	=	0X6E; 
							}
				if( ZZ==0xBC )	{//line:2, column:1
					 			keyboard_value	=	0XBC; 
							}
				if( ZZ==0xBA )	{//line:2, column:2
					 			keyboard_value	=	0XBA; 
							}
				if( ZZ==0xB6 )	{//line:2, column:3
					 			keyboard_value	=	0XB6; 
							}
				if( ZZ==0xAE )	{//line:2, column:4
					 			keyboard_value	=	0XAE; 
							}
				if( ZZ==0xDC )	{//line:3, column:1
					 			keyboard_value	=	0XDC; 
							}							
				if( ZZ==0xDA )	{//line:3, column:2
					 			keyboard_value	=	0XDA; 
							}
				if( ZZ==0xD6 )	{//line:3, column:3
					 			keyboard_value	=	0XD6; 
							}
				if( ZZ==0xCE )	{//line:3, column:4
					 			keyboard_value	=	0XCE; 
							}		
				
			}
			
                        key_time_calculate(); 	// 按键时间计算
		}
		
	}
	
	else //无任何按键
	{
	    keyon = 0; // 确认 没有 按键按下的标志
            keytime_start = 0;//关闭计时
            
	}								
	
}

void key_use(void)
{
  key_scan();
  key_use1();
}

/*******************************************************************************************************
函数名称：	void key_use(void)	
函数功能：	取得键值后执行相关功能
说明：		1、每进入一个case,首先将keyboard_value赋值为0；
			2、每进入一个case,如果不是”增益“”设置加“”设置减“三个按键，那么ZengYi_flag	=	0;
*******************************************************************************************************/
void key_use1(void)
{
    uchar j=0;
    //key_scan();
    if( keyon==0 )//按键松下后。。。
    {
	switch(keyboard_value)
	{
	    
	 	case 0X7C:	{  //L1,C1---复位
		                              keyboard_value=0;		   	   
						   Rst_flag		=	1;	
						   Started();
						   Rst_flag		=	0;	
						   ZengYi_flag	=	0;
						   Setting_flag	=   0;
						   Position_flag=0;
                                                   Acquisition_flag=0;//关闭“采集”
                                                   WorkOn_flag=0;//关闭“工作开”	
                                                   FatBuffer.RecordNumber = 0;//关闭"查询"
                                                   AutoZengYiRun.ZengYi_Ever_Used = 0;//检测“增益”键是否按下，当复位后，此值必须回0
                                                   keyboard_RST();
					   
					}break;
					//goto AA2;
					
		case 0X7A:	{  //L1,C2-----查询
			 		   Setting_flag	=   0;
					   ZengYi_flag	=	0;
					   keyboard_value=0;
					   Position_flag=0;
                                           Acquisition_flag=0;//关闭采集
                                           WorkOn_flag=0;//关闭“工作开”
                                           
                                           FatBuffer.RecordNumber = 1;
                                           FatBuffer.NoRecord_flag_InFile = 0;
                                           
                                           keyboard_RST();
		            }break;
		case 0X76:	{  //L1,C3------设置 加
			 		   keyboard_value	=	0;
			 		   Acquisition_flag=0;//关闭采集
                                           WorkOn_flag=0;//关闭“工作开”
                                           /*
					   if( keytime_save>keylongtime ) //长按键	,3s				
					   {
					 	   if( (Setting_flag!=0)||(Position_flag!=0) )
						      Setting_up	=	2;
						   keytime_save=0;
					   }
                                           */
					   //else	// 短按键					   
					   {	
					       				   
					       if( ZengYi_flag==1 )
						   {
					   	      ZengYi_disp++;	
						   	  if( ZengYi_disp>= ZengYi_MAX )
						   	     ZengYi_disp=0;
						   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ZengYi_disp/10][0] );
						   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ZengYi_disp/10][1] );
						   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ZengYi_disp%10][0] );
						   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ZengYi_disp%10][1] );
							  Ht1621WrOneData( 1,8+5 );//屏上的增益标志
						   }
						   else
						   {
						      //if( (Setting_flag!=0)||(Position_flag!=0)||(BIAOXIAO_flag==2) )
                                                      if( (Setting_flag!=0)||(Position_flag!=0) )
							     Setting_up	=	1;
						   }
                                               if( FatBuffer.RecordNumber > 0)//在查询状况下
                                               {
                                                  FatBuffer.LastRecordNumber = FatBuffer.RecordNumber ;
                                                  FatBuffer.RecordNumber++;//当记录条数达到上限时，别的地方会给其清理的
                                               }
					   }
                                           
                                           keyboard_RST();
		            }break;
		case 0X6E:	{ //L1,C4-----工作停
		
			 		   Setting_flag	=   0;
					   ZengYi_flag	=	0;
					   keyboard_value=0;
					   Position_flag=0;
                                           //Acquisition_flag=0;//关闭采集
                                           
					   /////////////////////////////////////////////////
                                           Acquisition_flag = 0;//按压“工作停”，关闭采集
                                           WorkOn_flag      =   0;//
                                           FatBuffer.RecordNumber = 0;//关闭"查询"
                                           HT1621_all_off();	
					   aStatic_Show();  //显示全0----
                                           //ShowCurrentPosition( );  //按压“工作停”后，屏上显示当前的点号，不显示全0----
                                           //GUOLIU;  //过流保护脚拉低。
                                           keyboard_RST();
		            }break;		
		case 0XBC:	{ //L2,C1---”增益“
		               keyboard_value	=	0;
					   ZengYi_flag		=	1;
                                           
					   Setting_flag	=   0;
					   Position_flag=0;
					   Acquisition_flag=0;//关闭 采集
                                           WorkOn_flag=0;//关闭“工作开”
                                           FatBuffer.RecordNumber = 0;//关闭"查询"
			 		   //if(keytime_save>600) //长按键	,3s				
					   {
					 	
					   }
					   //else	// 短按键
					   {
					   	   HT1621_all_off();
   						   Ht1621WrOneData( 0,8 );
   						   Ht1621WrOneData( 9,8 ); 
						   Ht1621WrOneData( 11,4 );		//点亮公司的标识  				   
   						   for( j=0;j<7;j++ )
   						   {
   						      Ht1621WrOneData( disp_data_addr[j][0],disp_data[0][0] );
							  Ht1621WrOneData( disp_data_addr[j][1],disp_data[0][1] );
   						   }
						   Ht1621WrOneData( disp_data_addr[2][0],disp_data[ZengYi_disp/10][0] );
						   Ht1621WrOneData( disp_data_addr[2][1],disp_data[ZengYi_disp/10][1] );
						   Ht1621WrOneData( disp_data_addr[3][0],disp_data[ZengYi_disp%10][0] );
						   Ht1621WrOneData( disp_data_addr[3][1],disp_data[ZengYi_disp%10][1] );
						   Ht1621WrOneData( 1,8+5 );//屏上的增益标志
   						   Ht1621WrOneData( 12,1 );	//小数点
						   
                                                   //GainControl();//控制脚
                                                   AutoZengYiRun.ZengYi_Ever_Used = 1;//////此值为1，不可以使用自动增益
					   }
                                           keyboard_RST();
		            }break;	
		case 0XBA:	{ //L2,C2-----采集
			 		   
					   Setting_flag	=   0;
					   ZengYi_flag	=	0;
					   keyboard_value=0;
					   Position_flag=0;	
                                           ////////////////////////////////////////////////////////////////
                                           HT1621_all_off();	//清屏
                                           Acquisition_flag = 1;
                                           GAIN_AUTO_MAX(  ); // 将自动增益值设为最大值( ZengYi_MAX-1 )
                                           WorkOn_flag=0;//关闭“工作开”	
                                           FatBuffer.RecordNumber = 0;//关闭"查询"
                                           
                                           keyboard_RST();
					   
		            }break;	
		case 0XB6:	{  //L2,C3----设置 减
		
			 		   keyboard_value	=	0;
					   Acquisition_flag=0;//关闭采集
                                           WorkOn_flag=0;//关闭“工作开”
                                           
                                           //if( BIAOXIAO_flag==0 )
                                           {
			 		      if(keytime_save>600) //长按键	,3s				
					      {
					        if( (Setting_flag!=0)||(Position_flag!=0) )
					 	      Setting_down	=	2;
						   keytime_save=0;
					      }
					      else	// 短按键
					      {
					   	   if( ZengYi_flag==1 )
						   {					   	          
                                                          if( ZengYi_disp!=0 )
                                                            ZengYi_disp--;	
                                                          else
                                                            ZengYi_disp=ZengYi_MAX-1;
                                                          
						   	  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ZengYi_disp/10][0] );
						   	  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ZengYi_disp/10][1] );
						   	  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ZengYi_disp%10][0] );
						   	  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ZengYi_disp%10][1] );
							  Ht1621WrOneData( 1,8+5 );//屏上的增益标志
						   }
						   else
						   {
						      //if( (Setting_flag!=0)||(Position_flag!=0)||(BIAOXIAO_flag==2)||(BIAOXIAO_flag==1) )
                                                      if( (Setting_flag!=0)||(Position_flag!=0) )
						         Setting_down	=	1;
						   }
                                                   if( FatBuffer.RecordNumber > 0)//在查询状况下
                                                   {
                                                      FatBuffer.LastRecordNumber = FatBuffer.RecordNumber ;
                                                      FatBuffer.RecordNumber --;
                                                      if( FatBuffer.RecordNumber==0 )
                                                      {
                                                        FatBuffer.RecordNumber = 1;//暂时设为1，本应为最前面的那条记录
                                                      }
                                                   }
						   
					      }
                                           }
                                           
                                           keyboard_RST();
		            }break;		
		case 0XAE:	{  //L2,C4------删除
			 		   
					   Setting_flag	=   0;
					   ZengYi_flag	=	0;
					   keyboard_value=0;
					   Position_flag=0;
					   Acquisition_flag=0;//关闭采集
                                           WorkOn_flag=0;//关闭“工作开”
                                           //FatBuffer.RecordNumber = 0;//关闭"查询"--删除是查询的下属功能，不可以在这里清除“查询”的标识
                                           /*
					   if(keytime_save>600) //长按键	,3s				
					   {					 	
					   }
					   else	// 短按键
					   {     
					   }
                                           */
                                           Delete_flag = 1; //开始删除
                                           keyboard_RST();
		            }break;		
		case 0XDC:	{  //L3,C1-----设置
			 		   keyboard_value	=	0;
					   ZengYi_flag		=	0;
					   Position_flag=0;
					   Acquisition_flag=0;//关闭采集
                                           WorkOn_flag=0;//关闭“工作开”
                                           FatBuffer.RecordNumber = 0;//关闭"查询"
                                           
					   Setting_flag++;
					   if( Setting_flag>6 ) Setting_flag=1;//这是一个循环模式
					   
					   HT1621_all_off();	
                                           
                                           keyboard_RST();
					   
		            }break;
					//goto AA2;
		case 0XDA:	{  //L3,C2----工作开
			 		   
					   Setting_flag	=   0;
					   ZengYi_flag	=	0;
					   keyboard_value=0;
					   Position_flag=0;
                                           Acquisition_flag=0;//关闭采集
                                           FatBuffer.RecordNumber = 0;//关闭"查询"
					   /////////////////////////////////////////////////////////////
                                           FeatureSelectionPortReset();//先将两个发射脚复位
                                           SetTimeB0( FrequencyPointSelection_data );//设置定时器中断时间,
                                           WorkOn_flag    =   1;
                                           GAIN_AUTO_MAX(  ); //将自动增益值设为最大值( ZengYi_MAX-1 )
                                           WorkOn_Transmit_STA = 1;//开始发射频率
                                           
                                           
                                           HT1621_all_off();
                                           
                                           keyboard_RST();
		            }break;		
		case 0XD6:	{  //L3,C3----点号
			 		   
					   //Setting_flag	=   0;
					   ZengYi_flag	=	0;
					   keyboard_value=0;
					   Acquisition_flag=0;//关闭采集
                                           WorkOn_flag=0;//关闭“工作开”
                                           FatBuffer.RecordNumber = 0;//关闭"查询"
                                           
					   //Position_flag	=	1;//点号
                                           
                                           if( keytime_save > keylongtime ) //长按键	,3s				
					   {	
                                             Setting_flag	=   0;
                                             
					   }
					   else	// 短按键
                                             if( Setting_flag != 0 )
					     {   
                                                Setting_up	=	2;
					     }
                                           
                                           keyboard_RST();
		            }break;		
		case 0XCE:	{  //L3,C4-----存储
			 		   Position_flag=0;					   
					   Setting_flag	=   0;
					   ZengYi_flag	=	0;
					   keyboard_value=0;
                                           FatBuffer.RecordNumber = 0;//关闭"查询"
                                           /*
                                           if( (Acquisition_flag==0)&&(WorkOn_flag==0) )
                                           {
					      Acquisition_flag=0;//关闭采集
                                              WorkOn_flag=0;//关闭“工作开”
                                           }
                                           */
					   /////////////////////////////////////////////////////
                                           if( (Acquisition_flag!=0)||(WorkOn_flag!=0) )
                                              Save_Flag       =   1;  //进行存储----任何按键不可以对“存储”清零！！！
                                           //////////////////////////////////////////////////
                                           keyboard_RST();
		            }break;																						
		default:	{
					   //keyboard_value=0;
                                            keyboard_RST();
					}break;
					
	}
    }
}

/*******************************************************************************************************
函数名称：	void key_time_calculate(void)	
函数功能：	按键时间计算
说明：		1、前提是已经有按键按下；
                2、前后读到的键值不一样，就要重新计时；
*******************************************************************************************************/
void  key_time_calculate(void)
{
 	if(Z==key_save)
	{
	 	 keytime_start	=	1;//开始计时
	}
	else
	{
	  	 key_save = Z;
		 keytime_start	=	0;
		 keytime		=	0;
		 keytime_save	=	0;
	}
}

/**************************************************************************************
函数名称：void FeatureSelectionPortReset(void)
函数功能：2根频率输出脚复位，即：一高一低
创建时间：2010-04-28
说    明：1、该函数用于键盘头文件中，具体在“工作开”
          2、
**************************************************************************************/
void FeatureSelectionPortReset(void)
{
  P15_H;
  //P16_L;
}

/**************************************************************************************
函数名称：void FeatureSelectionPortSTOP(void)
函数功能：2根频率输出脚拉低，用于不发射频率时
创建时间：2010-04-28
说    明：1、用于主函数中
          2、
**************************************************************************************/
void FeatureSelectionPortSTOP(void)
{
  if( WorkOn_flag!=1 )//没有按下按键“工作开”
  {
    P15_L;
    //P16_L;
  }
}


/**************************************************************************************
函数名称：void SaveUseWorkStop(void)
函数功能：存储完成后，执行“工作停”的功能。
创建时间：2010-04-28
修改时间：2010-09-06
说    明：1、在主函数里使用；
          2、按压“存储”后，屏上显示下一个点号，不显示全0----
          3、
**************************************************************************************/
void SaveUseWorkStop(void)
{
  if( Save_WorkStop_flag==1 )
  {
    Save_WorkStop_flag = 0; //清标志
    Acquisition_flag = 0;//按压“工作停”，关闭采集
    WorkOn_flag      =   0;//
    FatBuffer.RecordNumber = 0;//关闭"查询"
    HT1621_all_off();
    //aStatic_Show();       ///按压“存储”后，屏上显示下一个点号，不显示全0----
    ShowNextPosition( );  //屏上显示下一个点号
    //GUOLIU;               //过流保护拉低，使电源模块无高压输出，即无发射频率输出
  }
}