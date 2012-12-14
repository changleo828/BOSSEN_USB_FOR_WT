
//////////////////////////////////// 函数声明 /////////////////////////////////////
void ZhuangHaoAutomaticallyIncreases(void);
void SetScaleForPosition(unsigned char sign);
void ZhuangHao_RST(void);

void ZhuangHaoAutomaticallyIncreases_FeatureSelection_1(void);
void SetScaleForPosition_FeatureSelection_1(unsigned char sign);
void ZhuangHao_RST_FeatureSelection_1(void);
void CheckFeatureSelection(void);
void Position_shanshuo_Control( unsigned char sign );
//void Position_shanshuo_Control_For_0( unsigned char sign );
void ZhuangHaoAutomaticallyIncreases_FeatureSelection_0(void);//功能选择0
void ZhuangHao_RST_FeatureSelection_0(void);
////////////////////////////////////// END ////////////////////////////////////////

/*********************************************************************************
函数名称：void ZhuangHaoAutomaticallyIncreases(void)
函数功能：按压存储按键后，桩号自动加 1 。
创建时间：2010-05-15
说    明：
*********************************************************************************/
void ZhuangHaoAutomaticallyIncreases(void)
{
  
  if( FeatureSelection_data==0 )
  {
    ZhuangHaoAutomaticallyIncreases_FeatureSelection_0( );
  }
  else
    if( FeatureSelection_data==1 )
    {
      ZhuangHaoAutomaticallyIncreases_FeatureSelection_1( );
    }
    else
      if( FeatureSelection_data==2 )
      {
        ZhuangHaoAutomaticallyIncreases_FeatureSelection_1( );
      }
      else
      {
        ZhuangHaoAutomaticallyIncreases_FeatureSelection_1( );
      }
  /*
  if( FeatureSelection_data==0 )
  {
    ZhuangHaoAutomaticallyIncreases_FeatureSelection_0( );
  }
  if( FeatureSelection_data==1 )
  {
    ZhuangHaoAutomaticallyIncreases_FeatureSelection_1( );
  }
  if( FeatureSelection_data==2 )
  {
        ZhuangHaoAutomaticallyIncreases_FeatureSelection_1( );
  }
  if( FeatureSelection_data==3 )
  {
        ZhuangHaoAutomaticallyIncreases_FeatureSelection_1( );
  }
  */
}

void ZhuangHaoAutomaticallyIncreases_FeatureSelection_0(void)//功能选择0
{
  uchar *pk;
  uint  num=0;
  Position_data[6] = Position_data[6] + 1 ;
  if( Position_data[6]>3 )
  {
    Position_data[6] = 1;
    num = Position_data[2]*10 + Position_data[3];
    num = num + 1;
    if( (num>=1)&&(num<=99) )
    {
      pk=Decomposition( num );
      Position_data[2] = *(pk+1);
      Position_data[3] = *(pk+0);
      PositionRecordRun.PositionOu_InFeatureSelection_0 = num;
    }
  }
  PositionRecordRun.PositionA_InFeatureSelection_0 = Position_data[6];
}

void ZhuangHaoAutomaticallyIncreases_FeatureSelection_1(void)//功能选择1
{
  uchar *pk;
  uint  num=0;
  num = Position_data[4]*100 + Position_data[5]*10 + Position_data[6] + 1;  //  注：加 1  
  if( (num>=1)&&(num<=999) )
  {
    pk=Decomposition( num );
    Position_data[6] = *(pk+0);
    Position_data[5] = *(pk+1);
    Position_data[4] = *(pk+2);
    PositionRecordRun.PositionA_InFeatureSelection_1 = num ;
  }
}

/*******************************************************************************
函数名称： void SetScaleForPosition(unsigned char sign)
函数功能： 对点号和桩号的数值范围进行设定
时    间： 2010-05-21
说    明： 1、输入参数：sign---0：加操作，1：减操作
           2、
*******************************************************************************/
void SetScaleForPosition(unsigned char sign)
{
  if( FeatureSelection_data==0 )
  {
    SetScaleForPosition_FeatureSelection_1( sign );
  }
  else
    if( FeatureSelection_data==1 )
    {
      SetScaleForPosition_FeatureSelection_1( sign );
    }
    else
      if( FeatureSelection_data==2 )
      {
        SetScaleForPosition_FeatureSelection_1( sign );
      }
      else
      {
        SetScaleForPosition_FeatureSelection_1( sign );
      }
}

void SetScaleForPosition_FeatureSelection_1(unsigned char sign)//设定点号和桩号的范围，要求 1-999
{ 
  ///基本要求：各个位的范围是 0-9
  if( Position_data[ Position_shanshuo ]>9 )
      Position_data[ Position_shanshuo ]=0;
  if( Position_data[ Position_shanshuo ]<0 )
      Position_data[ Position_shanshuo ]=9;
  /////特别要求：设置加 或者 按压存储 自动加1时，对于10 20 。。这样的数，个位同样是0 ，但也是合法的，所以必须进行特别保护处理
  if( (sign==0)&&(Position_data[ Position_shanshuo ]==0) )
  {
      if( Position_shanshuo==3 )//点号的个位数闪烁
      {
          if( (Position_data[1]==0)&&(Position_data[2]==0) )//如果点号的百位与十位也为 0
          {
                Position_data[ Position_shanshuo ]=1;
          }
      }
      else
          if( Position_shanshuo==6 )//桩号的个位闪烁
          {
              if( (Position_data[4]==0)&&(Position_data[5]==0) )//如果 桩号的百位与十位也为 0
              {
                  Position_data[ Position_shanshuo ]=1;
              }
          }
  }
  /////////特别要求：点号和桩号，其最小值为 1 ，所以，一旦按压“设置减”减到0怎么办？这里进行处理
  if( (sign==1)&&(Position_data[ Position_shanshuo ]==0) )//设置减
  {
      if( Position_shanshuo==3 )//点号的个位数闪烁
      {
          if( (Position_data[1]==0)&&(Position_data[2]==0) )//如果点号的百位与十位也为 0
          {
                Position_data[ Position_shanshuo ]=9;
          }
      }
      else
          if( Position_shanshuo==6 )//桩号的个位闪烁
          {
              if( (Position_data[4]==0)&&(Position_data[5]==0) )//如果 桩号的百位与十位也为 0
              {
                  Position_data[ Position_shanshuo ]=9;
              }
          }
  }
}

/*******************************************************************************
函数名称： void ZhuangHao_RST(void)
函数功能： 巷道号、点号变化后，桩号复位为 001
创建时间： 2010-2-20
修改时间： 2010-07-19
说明    ： 1、Position_shanshuo = 0  巷道号、
           2、Position_shanshuo = 1、2 、3  点号
           3、只针对功能选择1
*******************************************************************************/
void ZhuangHao_RST(void)
{
  if( FeatureSelection_data==0 )
  {
    ZhuangHao_RST_FeatureSelection_0( );
  }
  else
    if( FeatureSelection_data==1 )
    {
      ZhuangHao_RST_FeatureSelection_1( );
    }
    else
      if( FeatureSelection_data==2 )
      {
        ZhuangHao_RST_FeatureSelection_1( );
      }
      else
      {
        ZhuangHao_RST_FeatureSelection_1( );
      }
}

void ZhuangHao_RST_FeatureSelection_0(void)
{//认为只有巷道号在闪烁，并且，该位一旦发生加减运算，就立即将O点号和A点号清为初始态
  //uchar *pk;
  if( Position_shanshuo == 0 )
  {
    Position_data[1]=0;
    Position_data[2]=0;
    Position_data[3]=1;
    Position_data[4]=0;
    Position_data[5]=0;
    Position_data[6]=1;
    PositionRecordRun.PositionOu_InFeatureSelection_0 = Position_data[2]*10 + Position_data[3];
    PositionRecordRun.PositionA_InFeatureSelection_0 = Position_data[6];
  }
}

void ZhuangHao_RST_FeatureSelection_1(void)
{
  if( (Position_shanshuo>=0)&&(Position_shanshuo<=3) )
    {
      Position_data[4] = 0;
      Position_data[5] = 0;
      Position_data[6] = 1;
      PositionRecordRun.PositionA_InFeatureSelection_1 = 1;
    }
  
}

////////////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
函数名称： void CheckFeatureSelection(void)
函数功能： 检查现在是什么功能选择，并且对点号数组Position_data【7】进行初始化
创建时间： 2010-07-19
修改时间： 2010-07-19
说明    ： 1、该函数使用于   FeatureSelection();设置按键里的内容         
*******************************************************************************/
void CheckFeatureSelection(void)
{
  uchar *pk;
  if( FeatureSelection_data==0 )
  {
    //Position_data[7]={0,0,0,1,0,0,1};
    Position_data[0] = PositionRecordRun.NumberOfXiangDao;
    Position_data[1]=0;
    Position_data[2] = PositionRecordRun.PositionOu_InFeatureSelection_0 /10;
    Position_data[3] = PositionRecordRun.PositionOu_InFeatureSelection_0 %10;
    Position_data[4]=0;
    Position_data[5]=0;
    Position_data[6] = PositionRecordRun.PositionA_InFeatureSelection_0;//1,2,3
  }
  else
    if( FeatureSelection_data==1 )
    {      
      Position_data[0] = PositionRecordRun.NumberOfXiangDao;
      pk=Decomposition( PositionRecordRun.PositionOu_InFeatureSelection_1 );
      Position_data[3] = *(pk+0);
      Position_data[2] = *(pk+1);
      Position_data[1] = *(pk+2);
      pk=Decomposition( PositionRecordRun.PositionA_InFeatureSelection_1 );
      Position_data[6] = *(pk+0);
      Position_data[5] = *(pk+1);
      Position_data[4] = *(pk+2);
    }
    else
      if( FeatureSelection_data==2 )
      {
        Position_data[0] = PositionRecordRun.NumberOfXiangDao;
        pk=Decomposition( PositionRecordRun.PositionOu_InFeatureSelection_2 );
        Position_data[3] = *(pk+0);
        Position_data[2] = *(pk+1);
        Position_data[1] = *(pk+2);
        pk=Decomposition( PositionRecordRun.PositionA_InFeatureSelection_2 );
        Position_data[6] = *(pk+0);
        Position_data[5] = *(pk+1);
        Position_data[4] = *(pk+2);
      }
      else// ==3
      {
        Position_data[0] = PositionRecordRun.NumberOfXiangDao;
        pk=Decomposition( PositionRecordRun.PositionOu_InFeatureSelection_3 );
        Position_data[3] = *(pk+0);
        Position_data[2] = *(pk+1);
        Position_data[1] = *(pk+2);
        pk=Decomposition( PositionRecordRun.PositionA_InFeatureSelection_3 );
        Position_data[6] = *(pk+0);
        Position_data[5] = *(pk+1);
        Position_data[4] = *(pk+2);
      }
}

/*******************************************************************************
函数名称： void Position_shanshuo_Control(void)
函数功能： 检查现在是什么功能选择，并且对闪烁位进行范围控制进行初始化
创建时间： 2010-07-19
修改时间： 2010-07-19
说明    ： 1、该函数使用于 
           2、输入参数：sign---0：加操作，1：减操作
*******************************************************************************/
void Position_shanshuo_Control( unsigned char sign )
{
  if( FeatureSelection_data==0 )
  {
    //Position_shanshuo_Control_For_0( sign );
  }
  else
    if( FeatureSelection_data==1 )
    {
      
    }
    else
      if( FeatureSelection_data==2 )
      {
        
      }
      else// ==3
      {
        
      }
}
/*
void Position_shanshuo_Control_For_0( unsigned char sign )//功能选择 0
{//只可以修改巷道号，闪烁的是左边第一位，和右边最后一位
  Position_shanshuo = 0;  
}
*/