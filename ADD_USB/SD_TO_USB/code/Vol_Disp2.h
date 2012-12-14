

void Vol_Disp(signed long vol);




/*******************************************************************************
函数名称：void Vol_Disp(signed long vol)
函数功能：将计算得到的电压值在屏幕上显示，精度：1uV
说    明：
作    者：猪的传说
创建时间：2010-02-25
修改时间：2010-04-30
*******************************************************************************/
//uchar  mmmm[8];
/*
void Vol_Disp(signed long vol)
{
  uchar  *p;
  uchar i=0;
  
  if( (Acquisition_flag==1)||(WorkOn_flag==1) )
  {
    if( Vol_Calculate_One_Time == 1 )
    {
      p=Decomposition8(vol); 
      Vol_Calculate_One_Time =0;
      for( i=0;i<8;i++ )
      {
        mmmm[i]= *(p+i) ;
      }
    }
  }
  else
  {
    p=Decomposition8(vol); 
    for( i=0;i<8;i++ )
    {
      mmmm[i]= *(p+i) ;
    }
  }
  
  if( Save_Flag==0 )//没有按压“存储”，或者存储没有结束
  {
    if( vol<FullData )
    {
      Ht1621WrOneData( disp_data_addr[0][0],disp_data[ mmmm[6] ][0] );
      Ht1621WrOneData( disp_data_addr[0][1],disp_data[ mmmm[6] ][1] );//
      Ht1621WrOneData( disp_data_addr[1][0],disp_data[ mmmm[5] ][0] );
      Ht1621WrOneData( disp_data_addr[1][1],disp_data[ mmmm[5] ][1] );//
      Ht1621WrOneData( disp_data_addr[2][0],disp_data[ mmmm[4] ][0] );
      Ht1621WrOneData( disp_data_addr[2][1],disp_data[ mmmm[4] ][1] );//
      Ht1621WrOneData( disp_data_addr[3][0],disp_data[ mmmm[3] ][0] );
      Ht1621WrOneData( disp_data_addr[3][1],disp_data[ mmmm[3] ][1] );//
      Ht1621WrOneData( disp_data_addr[4][0],disp_data[ mmmm[2] ][0] );
      Ht1621WrOneData( disp_data_addr[4][1],disp_data[ mmmm[2] ][1] );//
      Ht1621WrOneData( disp_data_addr[5][0],disp_data[ mmmm[1] ][0] );
      Ht1621WrOneData( disp_data_addr[5][1],disp_data[ mmmm[1] ][1] );//
      Ht1621WrOneData( disp_data_addr[6][0],disp_data[ mmmm[0] ][0] );
      Ht1621WrOneData( disp_data_addr[6][1],disp_data[ mmmm[0] ][1] );//
      Ht1621WrOneData( 13,1 );//单位：mV
      Ht1621WrOneData( 12,1 );	//小数点
    }
    else  //  超量程，显示“FULL”
    {
      Ht1621WrOneData( disp_data_addr[0][0],7 );
      Ht1621WrOneData( disp_data_addr[0][1],8 );//
      Ht1621WrOneData( disp_data_addr[1][0],5 );
      Ht1621WrOneData( disp_data_addr[1][1],7 );//
      Ht1621WrOneData( disp_data_addr[2][0],5 );
      Ht1621WrOneData( disp_data_addr[2][1],1 );//
      Ht1621WrOneData( disp_data_addr[3][0],5 );
      Ht1621WrOneData( disp_data_addr[3][1],1 );//
      Ht1621WrOneData( disp_data_addr[4][0],0 );
      Ht1621WrOneData( disp_data_addr[4][1],0 );//
      Ht1621WrOneData( disp_data_addr[5][0],0 );
      Ht1621WrOneData( disp_data_addr[5][1],0 );//
      Ht1621WrOneData( disp_data_addr[6][0],0 );
      Ht1621WrOneData( disp_data_addr[6][1],0 );//
      Ht1621WrOneData( 13,1 );//单位：mV
      Ht1621WrOneData( 12,0 );	//小数点+ma 屏蔽掉
    }
  }
  else
  {
    if( vol<FullData )
    {
      Ht1621WrOneData( disp_data_addr[0][0],disp_data[ mmmm[6] ][0] );
      Ht1621WrOneData( disp_data_addr[0][1],disp_data[ mmmm[6] ][1] );//
      Ht1621WrOneData( disp_data_addr[1][0],disp_data[ mmmm[5] ][0] );
      Ht1621WrOneData( disp_data_addr[1][1],disp_data[ mmmm[5] ][1] );//
      Ht1621WrOneData( disp_data_addr[2][0],disp_data[ mmmm[4] ][0] );
      Ht1621WrOneData( disp_data_addr[2][1],disp_data[ mmmm[4] ][1] );//
      Ht1621WrOneData( disp_data_addr[3][0],disp_data[ mmmm[3] ][0] );
      Ht1621WrOneData( disp_data_addr[3][1],disp_data[ mmmm[3] ][1] );//
      Ht1621WrOneData( disp_data_addr[4][0],disp_data[ mmmm[2] ][0]+8 );//// + 保存
      Ht1621WrOneData( disp_data_addr[4][1],disp_data[ mmmm[2] ][1] );//
      Ht1621WrOneData( disp_data_addr[5][0],disp_data[ mmmm[1] ][0] );
      Ht1621WrOneData( disp_data_addr[5][1],disp_data[ mmmm[1] ][1] );//
      Ht1621WrOneData( disp_data_addr[6][0],disp_data[ mmmm[0] ][0] );
      Ht1621WrOneData( disp_data_addr[6][1],disp_data[ mmmm[0] ][1] );//
      Ht1621WrOneData( 13,1 );//单位：mV
      Ht1621WrOneData( 12,1 );	//小数点
    }
    else  //  超量程，显示“FULL”
    {
      Ht1621WrOneData( disp_data_addr[0][0],7 );
      Ht1621WrOneData( disp_data_addr[0][1],8 );//
      Ht1621WrOneData( disp_data_addr[1][0],5 );
      Ht1621WrOneData( disp_data_addr[1][1],7 );//
      Ht1621WrOneData( disp_data_addr[2][0],5 );
      Ht1621WrOneData( disp_data_addr[2][1],1 );//
      Ht1621WrOneData( disp_data_addr[3][0],5 );
      Ht1621WrOneData( disp_data_addr[3][1],1 );//
      Ht1621WrOneData( disp_data_addr[4][0],0  +8 );//// + 保存
      Ht1621WrOneData( disp_data_addr[4][1],0 );//
      Ht1621WrOneData( disp_data_addr[5][0],0 );
      Ht1621WrOneData( disp_data_addr[5][1],0 );//
      Ht1621WrOneData( disp_data_addr[6][0],0 );
      Ht1621WrOneData( disp_data_addr[6][1],0 );//
      Ht1621WrOneData( 13,1 );//单位：mV
      Ht1621WrOneData( 12,0 );	//小数点+ma 屏蔽掉
    }
  }
}
*/

void Vol_Disp(signed long vol)
{
  uchar  *p;
  uchar i=0;
  
  
  
  if( (Acquisition_flag==1)||(WorkOn_flag==1) )
  {
    if( Vol_Calculate_One_Time == 1 )
    {
      //AutoZengyi();/////////////////////////////////////////////////////进行自动增益检测
      ZengYiTEST();//////
      p=Decomposition8(vol); 
      Vol_Calculate_One_Time =0;
      for( i=0;i<8;i++ )
      {
        mmmm[i]= *(p+i) ;
      }
    }
  }
  else
  {
    p=Decomposition8(vol); 
    for( i=0;i<8;i++ )
    {
      mmmm[i]= *(p+i) ;
    }
  }
  //AutoZengyi();/////////////////////////////////////////////////////进行自动增益检测
  if( Save_Flag==0 )//没有按压“存储”，或者存储没有结束
  {
      if( AutoZengYiRun.which_grade_yichu == 0 )//无任何溢出
      {
          Ht1621WrOneData( disp_data_addr[0][0],disp_data[ mmmm[6] ][0] );
          Ht1621WrOneData( disp_data_addr[0][1],disp_data[ mmmm[6] ][1] );//
          Ht1621WrOneData( disp_data_addr[1][0],disp_data[ mmmm[5] ][0] );
          Ht1621WrOneData( disp_data_addr[1][1],disp_data[ mmmm[5] ][1] );//
          Ht1621WrOneData( disp_data_addr[2][0],disp_data[ mmmm[4] ][0] );
          Ht1621WrOneData( disp_data_addr[2][1],disp_data[ mmmm[4] ][1] );//
          Ht1621WrOneData( disp_data_addr[3][0],disp_data[ mmmm[3] ][0] );
          Ht1621WrOneData( disp_data_addr[3][1],disp_data[ mmmm[3] ][1] );//
          Ht1621WrOneData( disp_data_addr[4][0],disp_data[ mmmm[2] ][0] );
          Ht1621WrOneData( disp_data_addr[4][1],disp_data[ mmmm[2] ][1] );//
          Ht1621WrOneData( disp_data_addr[5][0],disp_data[ mmmm[1] ][0] );
          Ht1621WrOneData( disp_data_addr[5][1],disp_data[ mmmm[1] ][1] );//
          Ht1621WrOneData( disp_data_addr[6][0],disp_data[ mmmm[0] ][0] );
          Ht1621WrOneData( disp_data_addr[6][1],disp_data[ mmmm[0] ][1] );//
          Ht1621WrOneData( 13,1 );//单位：mV
          Ht1621WrOneData( 12,1 );	//小数点
      }
      else///FULL
      {
          Ht1621WrOneData( disp_data_addr[0][0],7 );
          Ht1621WrOneData( disp_data_addr[0][1],8 );//
          Ht1621WrOneData( disp_data_addr[1][0],5 );
          Ht1621WrOneData( disp_data_addr[1][1],7 );//
          Ht1621WrOneData( disp_data_addr[2][0],5 );
          Ht1621WrOneData( disp_data_addr[2][1],1 );//
          Ht1621WrOneData( disp_data_addr[3][0],5 );
          Ht1621WrOneData( disp_data_addr[3][1],1 );//
          Ht1621WrOneData( disp_data_addr[4][0],disp_data[ AutoZengYiRun.which_grade_yichu ][0] );
          Ht1621WrOneData( disp_data_addr[4][1],disp_data[ AutoZengYiRun.which_grade_yichu ][1] );//
          Ht1621WrOneData( disp_data_addr[5][0],0 );
          Ht1621WrOneData( disp_data_addr[5][1],0 );//
          Ht1621WrOneData( disp_data_addr[6][0],0 );
          Ht1621WrOneData( disp_data_addr[6][1],0 );//
          Ht1621WrOneData( 13,1 );//单位：mV
          Ht1621WrOneData( 12,0 );	//小数点+ma 屏蔽掉
      }
  }
  else
  {
      if( AutoZengYiRun.which_grade_yichu == 0 )//无任何溢出
      {
          Ht1621WrOneData( disp_data_addr[0][0],disp_data[ mmmm[6] ][0] );
          Ht1621WrOneData( disp_data_addr[0][1],disp_data[ mmmm[6] ][1] );//
          Ht1621WrOneData( disp_data_addr[1][0],disp_data[ mmmm[5] ][0] );
          Ht1621WrOneData( disp_data_addr[1][1],disp_data[ mmmm[5] ][1] );//
          Ht1621WrOneData( disp_data_addr[2][0],disp_data[ mmmm[4] ][0] );
          Ht1621WrOneData( disp_data_addr[2][1],disp_data[ mmmm[4] ][1] );//
          Ht1621WrOneData( disp_data_addr[3][0],disp_data[ mmmm[3] ][0] );
          Ht1621WrOneData( disp_data_addr[3][1],disp_data[ mmmm[3] ][1] );//
          Ht1621WrOneData( disp_data_addr[4][0],disp_data[ mmmm[2] ][0]+8 );//// + 保存
          Ht1621WrOneData( disp_data_addr[4][1],disp_data[ mmmm[2] ][1] );//
          Ht1621WrOneData( disp_data_addr[5][0],disp_data[ mmmm[1] ][0] );
          Ht1621WrOneData( disp_data_addr[5][1],disp_data[ mmmm[1] ][1] );//
          Ht1621WrOneData( disp_data_addr[6][0],disp_data[ mmmm[0] ][0] );
          Ht1621WrOneData( disp_data_addr[6][1],disp_data[ mmmm[0] ][1] );//
          Ht1621WrOneData( 13,1 );//单位：mV
          Ht1621WrOneData( 12,1 );	//小数点
      }
      else
      {
          Ht1621WrOneData( disp_data_addr[0][0],7 );
          Ht1621WrOneData( disp_data_addr[0][1],8 );//
          Ht1621WrOneData( disp_data_addr[1][0],5 );
          Ht1621WrOneData( disp_data_addr[1][1],7 );//
          Ht1621WrOneData( disp_data_addr[2][0],5 );
          Ht1621WrOneData( disp_data_addr[2][1],1 );//
          Ht1621WrOneData( disp_data_addr[3][0],5 );
          Ht1621WrOneData( disp_data_addr[3][1],1 );//
          Ht1621WrOneData( disp_data_addr[4][0],disp_data[ AutoZengYiRun.which_grade_yichu ][0]  +8 );//// + 保存
          Ht1621WrOneData( disp_data_addr[4][1],disp_data[ AutoZengYiRun.which_grade_yichu ][1] );//
          Ht1621WrOneData( disp_data_addr[5][0],0 );
          Ht1621WrOneData( disp_data_addr[5][1],0 );//
          Ht1621WrOneData( disp_data_addr[6][0],0 );
          Ht1621WrOneData( disp_data_addr[6][1],0 );//
          Ht1621WrOneData( 13,1 );//单位：mV
          Ht1621WrOneData( 12,0 );	//小数点+ma 屏蔽掉
      }
  }
}
//*/