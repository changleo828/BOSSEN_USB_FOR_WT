/*============================================ 头文件说明 ============================================================
头文件名称：Vol_Display.h
头文件用途：将AD读数进行转换并且显示
作      者：猪的传说
时      间：2010-02-25
================================================ END ===============================================================*/
signed long VolResultEnd(signed long real);
signed long AD2VOL(signed long data);

signed long VolFiltering(void);
signed long VolNOFiltering(void);///为自动增益缩写



/*******************************************************************************
函数名称：signed long AD2VOL(signed long data)
函数功能：将AD值换算成实际电压值，增益：1
说    明：signed long 的数据长度是: -2147483648～+2147483647
作    者：猪的传说
时    间：2010-02-25
*******************************************************************************/
signed long AD2VOL(signed long data)
{
  float ah=0;
  ah = ( (float)(data) )*( (float)(RefInput) );
  //ah = ah>>24;
  ah = ah/( (float)(16777216) );
  return (signed long)(ah);
}

/*******************************************************************************
函数名称：signed long VolFiltering(void)
函数功能：将得到的实际电压进行均值计算
说    明：1、此滤波很费时间
          2、加上标校值的计算
作    者：猪的传说
时    间：2010-02-25
*******************************************************************************/
#define     VolFiltering_count      11
signed long VolFiltering(void)
{
  uchar i=0;
  float sum1=0.0000;
  for( i=0;i<VolFiltering_count;i++ )
  {
    sum1 = sum1+AD2VOL( Read24DataFromAd7710() );//实际电压值
  }
  sum1 = sum1/( (float)(VolFiltering_count) );
  sum1 = VolResultEnd( (signed long)(sum1) );
  
  //return (signed long)(sum1);
  return LINEAR( (signed long)(sum1) ) ;
}

/*******************************************************************************
函数名称：signed long VolNOFiltering(void)
函数功能：配合自动增益，计算实际电压
说    明：自动增益需要的时间开支比较大，会影响“工作开”的分屏幕显示效果，导致
          电压显示的时间太短，甚至显示不出来。所以，在这里就将计算均值的过程省略。
          返回真实AD7711电压。
作    者：猪的传说
时    间：2010-07-10
*******************************************************************************/
signed long VolNOFiltering(void)///为了自动增益缩写
{
  uchar i=0;
  float sum1=0.0000;
  for( i=0;i<1;i++ )
  {
    sum1 = sum1+AD2VOL( Read24DataFromAd7710() );//实际电压值
  }
  //sum1 = sum1/( (float)(VolFiltering_count) );
  sum1 = VolResultEnd( (signed long)(sum1) );
  return (signed long)(sum1);
  //return LINEAR( (signed long)(sum1) ) ;
}

signed long VolResultEnd( signed long real )
{
  
  if( real<0 )
  {
    real = 0;
  }
  return real;
}
