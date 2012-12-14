
//*********************************************   函数声明   **********************************************
signed long LINEAR( signed long real );
signed long ScreenValueForZengYi_0( signed long real );
signed long ScreenValueForZengYi_1( signed long real );
signed long ScreenValueForZengYi_2( signed long real );
signed long ScreenValueForZengYi_3( signed long real );
signed long ScreenValueForZengYi_4( signed long real );
signed long ScreenValueForZengYi_5( signed long real );
signed long ScreenValueForZengYi_6( signed long real );
signed long ScreenValueForZengYi_7( signed long real );
signed long ScreenValueForZengYi_8( signed long real );
signed long ScreenValueForZengYi_9( signed long real );
signed long RealValue_to_ScreenValue( long ScreenValueBig,long ScreenValueSmall,long RealValueBig,long RealValueSmall,long RealValueMiddle );

//*************************************************** END *************************************************


#define   SCREEN_0       0    //  无输入

#define   SCREEN_00001    1  //  00001uV
#define   SCREEN_00002    2
#define   SCREEN_00003    3
#define   SCREEN_00004    4
#define   SCREEN_00005    5
#define   SCREEN_00006    6
#define   SCREEN_00007    7
#define   SCREEN_00008    8
#define   SCREEN_00009    9
#define   SCREEN_00010    10
#define   SCREEN_00011    11
#define   SCREEN_00012    12
#define   SCREEN_00015    15  // 15 uV
#define   SCREEN_00020    20
#define   SCREEN_00030    30
#define   SCREEN_00040    40
#define   SCREEN_00050    50
#define   SCREEN_00060    60
#define   SCREEN_00070    70
#define   SCREEN_00080    80
#define   SCREEN_00090    90
#define   SCREEN_00100    100// 100 uV
#define   SCREEN_00110    110
#define   SCREEN_00120    120
#define   SCREEN_00130    130
#define   SCREEN_00140    140
#define   SCREEN_00150    150
#define   SCREEN_00170    170
#define   SCREEN_00190    190
#define   SCREEN_00210    210
#define   SCREEN_00250    250
#define   SCREEN_00290    290
#define   SCREEN_00330    330
#define   SCREEN_00370    370// 370 uV

#define   SCREEN_0400    400  //  0.400mv
#define   SCREEN_0500    500
#define   SCREEN_0600    600
#define   SCREEN_0700    700
#define   SCREEN_0800    800
#define   SCREEN_0900    900
#define   SCREEN_1000    1000 //  1mv，屏显值
#define   SCREEN_2000    2000
#define   SCREEN_3000    3000
#define   SCREEN_4000    4000
#define   SCREEN_5000    5000
#define   SCREEN_6000    6000
#define   SCREEN_7000    7000
#define   SCREEN_8000    8000
#define   SCREEN_9000    9000
#define   SCREEN_10000   10000  //  10mv
#define   SCREEN_20000   20000
#define   SCREEN_30000   30000
#define   SCREEN_40000   40000
#define   SCREEN_50000   50000
#define   SCREEN_100000  100000 //  100mv
#define   SCREEN_200000  200000 //  200mv
#define   SCREEN_300000  300000 //  
#define   SCREEN_400000  400000 //  
#define   SCREEN_500000  500000 //  
#define   SCREEN_550000  550000 //  550mv


signed long LINEAR( signed long real )
{
  signed long ScreenValue = 0;
  
  if( ZengYi_disp == 0 )
  {
    ScreenValue = ScreenValueForZengYi_0( real ) ;
  }
  else
    if( ZengYi_disp == 1 )
    {
      ScreenValue = ScreenValueForZengYi_1( real ) ;
    }
    else
      if( ZengYi_disp == 2 )
      {
        ScreenValue = ScreenValueForZengYi_2( real ) ;
      }
      else
        if( ZengYi_disp == 3 )
        {
          ScreenValue = ScreenValueForZengYi_3( real ) ;
        }
        else
          if( ZengYi_disp == 4 )
          {
            ScreenValue = ScreenValueForZengYi_4( real ) ;
          }
          else
            if( ZengYi_disp == 5 )
            {
              ScreenValue = ScreenValueForZengYi_5( real ) ;
            }
            else
              if( ZengYi_disp == 6 )
              {
                ScreenValue = ScreenValueForZengYi_6( real ) ;
              }
              else
                if( ZengYi_disp == 7 )
                {
                  ScreenValue = ScreenValueForZengYi_7( real ) ;
                }
                else
                  if( ZengYi_disp == 8 )
                  {
                    ScreenValue = ScreenValueForZengYi_8( real ) ;
                  }
                  else
                    if( ZengYi_disp == 9 )
                    {
                      ScreenValue = ScreenValueForZengYi_9( real ) ;
                    }
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long ScreenValueForZengYi_0( signed long real )
函数功能： 计算得到屏显数据，增益0
说    明： 1、real --- AD7711计算得到的真实电压值 ，未经过修正
创建时间： 2010-07-10
修改时间： 2010-07-10
*******************************************************************************/
signed long ScreenValueForZengYi_0( signed long real )
{
  signed long ScreenValue = 0;
  if( real <= data_0000_zy_0 )// 0
  {
    ScreenValue = 0;
  }
  else
    if( ( real > data_0000_zy_0 )&&( real <= data_00001_zy_0 ) )//  0.001mv--0.000mv
    {
      ScreenValue = RealValue_to_ScreenValue( SCREEN_00001,SCREEN_0,data_00001_zy_0,data_0000_zy_0,real );
    }
    else
      if( ( real > data_00001_zy_0 )&&( real <= data_00005_zy_0 ) )//  0.001mv--0.005mv
      {
        ScreenValue = RealValue_to_ScreenValue( SCREEN_00005,SCREEN_00001,data_00005_zy_0,data_00001_zy_0,real );
      }
      else
        if( ( real > data_00005_zy_0 )&&( real <= data_00010_zy_0 ) )//  0.005mv--0.010mv
        {
          ScreenValue = RealValue_to_ScreenValue( SCREEN_00010,SCREEN_00005,data_00010_zy_0,data_00005_zy_0,real );
        }      
        else
                            if( ( real > data_00010_zy_0 )&&( real <= data_00015_zy_0 ) )//  0.010mv--0.015mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00015,SCREEN_00010,data_00015_zy_0,data_00010_zy_0,real );
                            }
                            else
                              if( ( real > data_00015_zy_0 )&&( real <= data_00020_zy_0 ) )//  0.015mv--0.020mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00020,SCREEN_00015,data_00020_zy_0,data_00015_zy_0,real );
                              }                              
                                else
                                  if( ( real > data_00020_zy_0 )&&( real <= data_00040_zy_0 ) )//  0.020mv--0.040mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00040,SCREEN_00020,data_00040_zy_0,data_00020_zy_0,real );
                                  }                                  
                                    else
                                      if( ( real > data_00040_zy_0 )&&( real <= data_00060_zy_0 ) )//  0.040mv--0.060mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00060,SCREEN_00040,data_00060_zy_0,data_00040_zy_0,real );
                                      }
                                      else                                        
                                          if( ( real > data_00060_zy_0 )&&( real <= data_00080_zy_0 ) )//  0.060mv--0.080mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_00080,SCREEN_00060,data_00080_zy_0,data_00060_zy_0,real );
                                          }
                                          else                                            
                                              if( ( real > data_00080_zy_0 )&&( real <= data_00100_zy_0 ) )//  0.080mv--0.100mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00100,SCREEN_00080,data_00100_zy_0,data_00080_zy_0,real );
                                              }
                                              else                                                
                                                  if( ( real > data_00100_zy_0 )&&( real <= data_00120_zy_0 ) )//  0.100mv--0.120mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00120,SCREEN_00100,data_00120_zy_0,data_00100_zy_0,real );
                                                  }
                                                  else                                                    
                                                      if( ( real > data_00120_zy_0 )&&( real <= data_00140_zy_0 ) )//  0.120mv--0.140mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00140,SCREEN_00120,data_00140_zy_0,data_00120_zy_0,real );
                                                      }
                                                      else                                                                                                                 
                                                            if( ( real > data_00140_zy_0 )&&( real <= data_00190_zy_0 ) )//  0.140mv--0.190mv
                                                            {
                                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00190,SCREEN_00140,data_00190_zy_0,data_00140_zy_0,real );
                                                            }
                                                            else                                                              
                                                                if( ( real > data_00190_zy_0 )&&( real <= data_00250_zy_0 ) )//  0.190mv--0.250mv
                                                                {
                                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_00250,SCREEN_00190,data_00250_zy_0,data_00190_zy_0,real );
                                                                }
                                                                else                                                                  
                                                                    if( ( real > data_00250_zy_0 )&&( real <= data_00330_zy_0 ) )//  0.250mv--0.330mv
                                                                    {
                                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_00330,SCREEN_00250,data_00330_zy_0,data_00250_zy_0,real );
                                                                    }
                                                                    else                                                                      
                                                                        if( ( real > data_00330_zy_0 )&&( real <= data_0400_zy_0 ) )//  0.330mv--0.400mv
                                                                        {
                                                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_0400,SCREEN_00330,data_0400_zy_0,data_00330_zy_0,real );
                                                                        }
                                                                        else
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
      if( ( real > data_0400_zy_0 )&&( real <= data_0500_zy_0 ) )//  0.400mv--0.500mv
      {
        ScreenValue = SCREEN_0500 - (signed long)((SCREEN_0500 - SCREEN_0400)*(data_0500_zy_0 - real))/(data_0500_zy_0 - data_0400_zy_0);
      }
      else        
        if( ( real > data_0500_zy_0 )&&( real <= data_0600_zy_0 ) )//  0.500mv--0.600mv
        {
          ScreenValue = SCREEN_0600 - (signed long)((SCREEN_0600 - SCREEN_0500)*(data_0600_zy_0 - real))/(data_0600_zy_0 - data_0500_zy_0);
        }
        else
          if( ( real > data_0600_zy_0 )&&( real <= data_0700_zy_0 ) )//  0.600mv--0.700mv
          {
            ScreenValue = SCREEN_0700 - (signed long)((SCREEN_0700 - SCREEN_0600)*(data_0700_zy_0 - real))/(data_0700_zy_0 - data_0600_zy_0);
          }
          else
            if( ( real > data_0700_zy_0 )&&( real <= data_0800_zy_0 ) )//  0.700mv--0.800mv
            {
              ScreenValue = SCREEN_0800 - (signed long)((SCREEN_0800 - SCREEN_0700)*(data_0800_zy_0 - real))/(data_0800_zy_0 - data_0700_zy_0);
            }
            else
              if( ( real > data_0800_zy_0 )&&( real <= data_0900_zy_0 ) )//  0.800mv--0.900mv
              {
                ScreenValue = SCREEN_0900 - (signed long)((SCREEN_0900 - SCREEN_0800)*(data_0900_zy_0 - real))/(data_0900_zy_0 - data_0800_zy_0);
              }
              else
                if( ( real > data_0900_zy_0 )&&( real <= data_1000_zy_0 ) )//  0.900mv--1.000mv
                {
                  ScreenValue = SCREEN_1000 - (signed long)((SCREEN_1000 - SCREEN_0900)*(data_1000_zy_0 - real))/(data_1000_zy_0 - data_0900_zy_0);
                }
                else            
                  if( ( real > data_1000_zy_0 )&&( real <= data_2000_zy_0 ) )//  1.000mv--2.000mv
                  {
                    ScreenValue = RealValue_to_ScreenValue( SCREEN_2000,SCREEN_1000,data_2000_zy_0,data_1000_zy_0,real );
                  }
                  else
                    if( ( real > data_2000_zy_0 )&&( real <= data_3000_zy_0 ) )//  2.000mv--3.000mv
                    {
                      ScreenValue = RealValue_to_ScreenValue( SCREEN_3000,SCREEN_2000,data_3000_zy_0,data_2000_zy_0,real );
                    }
                    else
                      if( ( real > data_3000_zy_0 )&&( real <= data_4000_zy_0 ) )//  3.000mv--4.000mv
                      {
                        ScreenValue = RealValue_to_ScreenValue( SCREEN_4000,SCREEN_3000,data_4000_zy_0,data_3000_zy_0,real );
                      }
                      else
                        if( ( real > data_4000_zy_0 )&&( real <= data_5000_zy_0 ) )//  4.000mv--5.000mv
                        {
                          ScreenValue = RealValue_to_ScreenValue( SCREEN_5000,SCREEN_4000,data_5000_zy_0,data_4000_zy_0,real );
                        }
                        else
                          if( ( real > data_5000_zy_0 )&&( real <= data_6000_zy_0 ) )//  5.000mv--6.000mv
                          {
                            ScreenValue = RealValue_to_ScreenValue( SCREEN_6000,SCREEN_5000,data_6000_zy_0,data_5000_zy_0,real );
                          }
                          else
                            if( ( real > data_6000_zy_0 )&&( real <= data_7000_zy_0 ) )//  6.000mv--7.000mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_7000,SCREEN_6000,data_7000_zy_0,data_6000_zy_0,real );
                            }
                            else
                              if( ( real > data_7000_zy_0 )&&( real <= data_8000_zy_0 ) )//  7.000mv--8.000mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_8000,SCREEN_7000,data_8000_zy_0,data_7000_zy_0,real );
                              }
                              else
                                if( ( real > data_8000_zy_0 )&&( real <= data_9000_zy_0 ) )//  8.000mv--9.000mv
                                {
                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_9000,SCREEN_8000,data_9000_zy_0,data_8000_zy_0,real );
                                }
                                else
                                  if( ( real > data_9000_zy_0 )&&( real <= data_10000_zy_0 ) )//  9.000mv--10.000mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_10000,SCREEN_9000,data_10000_zy_0,data_9000_zy_0,real );
                                  }
                                  else
                                    if( ( real > data_10000_zy_0 )&&( real <= data_20000_zy_0 ) )//  10.000mv--20.000mv
                                    {
                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_20000,SCREEN_10000,data_20000_zy_0,data_10000_zy_0,real );
                                    }
                                    else
                                      if( ( real > data_20000_zy_0 )&&( real <= data_30000_zy_0 ) )//  20.000mv--30.000mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_30000,SCREEN_20000,data_30000_zy_0,data_20000_zy_0,real );
                                      }
                                      else
                                        if( ( real > data_30000_zy_0 )&&( real <= data_40000_zy_0 ) )//  30.000mv--40.000mv
                                        {
                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_40000,SCREEN_30000,data_40000_zy_0,data_30000_zy_0,real );
                                        }
                                        else
                                          if( ( real > data_40000_zy_0 )&&( real <= data_50000_zy_0 ) )//  40.000mv--50.000mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_50000,SCREEN_40000,data_50000_zy_0,data_40000_zy_0,real );
                                          }
                                          else
                                            if( ( real > data_50000_zy_0 )&&( real <= data_100000_zy_0 ) )//  50.000mv--100.000mv
                                            {
                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_100000,SCREEN_50000,data_100000_zy_0,data_50000_zy_0,real );
                                            }
                                            else
                                              if( ( real > data_100000_zy_0 )&&( real <= data_200000_zy_0 ) )//  100.000mv--200.000mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_200000,SCREEN_100000,data_200000_zy_0,data_100000_zy_0,real );
                                              }
                                              else
                                                if( ( real > data_200000_zy_0 )&&( real <= data_300000_zy_0 ) )//  200.000mv--300.000mv
                                                {
                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_300000,SCREEN_200000,data_300000_zy_0,data_200000_zy_0,real );
                                                }
                                                else
                                                  if( ( real > data_300000_zy_0 )&&( real <= data_400000_zy_0 ) )//  300.000mv--400.000mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_400000,SCREEN_300000,data_400000_zy_0,data_300000_zy_0,real );
                                                  }
                                                  else
                                                    if( ( real > data_400000_zy_0 )&&( real <= data_500000_zy_0 ) )//  400.000mv--500.000mv
                                                    {
                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_500000,SCREEN_400000,data_500000_zy_0,data_400000_zy_0,real );
                                                    }
                                                    else
                                                      if( ( real > data_500000_zy_0 )&&( real <= data_550000_zy_0 ) )//  500.000mv--550.000mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_550000,SCREEN_500000,data_550000_zy_0,data_500000_zy_0,real );
                                                      }
  //
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long ScreenValueForZengYi_1( signed long real )
函数功能： 计算得到屏显数据，增益1
说    明： 1、real --- AD7711计算得到的真实电压值 ，未经过修正
创建时间： 2010-07-10
修改时间： 2010-07-10
*******************************************************************************/
signed long ScreenValueForZengYi_1( signed long real )
{
  signed long ScreenValue = 0;
  
  if( real <= data_0000_zy_1 )// 0
  {
    ScreenValue = 0;
  }
  else
    if( ( real > data_0000_zy_1 )&&( real <= data_00001_zy_1 ) )//  0.001mv--0.000mv
    {
      ScreenValue = RealValue_to_ScreenValue( SCREEN_00001,SCREEN_0,data_00001_zy_1,data_0000_zy_1,real );
    }
    else
      if( ( real > data_00001_zy_1 )&&( real <= data_00005_zy_1 ) )//  0.001mv--0.005mv
      {
        ScreenValue = RealValue_to_ScreenValue( SCREEN_00005,SCREEN_00001,data_00005_zy_1,data_00001_zy_1,real );
      }
      else
        if( ( real > data_00005_zy_1 )&&( real <= data_00010_zy_1 ) )//  0.005mv--0.010mv
        {
          ScreenValue = RealValue_to_ScreenValue( SCREEN_00010,SCREEN_00005,data_00010_zy_1,data_00005_zy_1,real );
        }      
        else
                            if( ( real > data_00010_zy_1 )&&( real <= data_00015_zy_1 ) )//  0.010mv--0.015mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00015,SCREEN_00010,data_00015_zy_1,data_00010_zy_1,real );
                            }
                            else
                              if( ( real > data_00015_zy_1 )&&( real <= data_00020_zy_1 ) )//  0.015mv--0.020mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00020,SCREEN_00015,data_00020_zy_1,data_00015_zy_1,real );
                              }                              
                                else
                                  if( ( real > data_00020_zy_1 )&&( real <= data_00040_zy_1 ) )//  0.020mv--0.040mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00040,SCREEN_00020,data_00040_zy_1,data_00020_zy_1,real );
                                  }                                  
                                    else
                                      if( ( real > data_00040_zy_1 )&&( real <= data_00060_zy_1 ) )//  0.040mv--0.060mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00060,SCREEN_00040,data_00060_zy_1,data_00040_zy_1,real );
                                      }
                                      else                                        
                                          if( ( real > data_00060_zy_1 )&&( real <= data_00080_zy_1 ) )//  0.060mv--0.080mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_00080,SCREEN_00060,data_00080_zy_1,data_00060_zy_1,real );
                                          }
                                          else                                            
                                              if( ( real > data_00080_zy_1 )&&( real <= data_00100_zy_1 ) )//  0.080mv--0.100mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00100,SCREEN_00080,data_00100_zy_1,data_00080_zy_1,real );
                                              }
                                              else                                                
                                                  if( ( real > data_00100_zy_1 )&&( real <= data_00120_zy_1 ) )//  0.100mv--0.120mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00120,SCREEN_00100,data_00120_zy_1,data_00100_zy_1,real );
                                                  }
                                                  else                                                    
                                                      if( ( real > data_00120_zy_1 )&&( real <= data_00140_zy_1 ) )//  0.120mv--0.140mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00140,SCREEN_00120,data_00140_zy_1,data_00120_zy_1,real );
                                                      }
                                                      else                                                                                                                 
                                                            if( ( real > data_00140_zy_1 )&&( real <= data_00190_zy_1 ) )//  0.140mv--0.190mv
                                                            {
                                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00190,SCREEN_00140,data_00190_zy_1,data_00140_zy_1,real );
                                                            }
                                                            else                                                              
                                                                if( ( real > data_00190_zy_1 )&&( real <= data_00250_zy_1 ) )//  0.190mv--0.250mv
                                                                {
                                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_00250,SCREEN_00190,data_00250_zy_1,data_00190_zy_1,real );
                                                                }
                                                                else                                                                  
                                                                    if( ( real > data_00250_zy_1 )&&( real <= data_00330_zy_1 ) )//  0.250mv--0.330mv
                                                                    {
                                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_00330,SCREEN_00250,data_00330_zy_1,data_00250_zy_1,real );
                                                                    }
                                                                    else                                                                      
                                                                        if( ( real > data_00330_zy_1 )&&( real <= data_0400_zy_1 ) )//  0.330mv--0.400mv
                                                                        {
                                                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_0400,SCREEN_00330,data_0400_zy_1,data_00330_zy_1,real );
                                                                        }
                                                                        else
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
      if( ( real > data_0400_zy_1 )&&( real <= data_0500_zy_1 ) )//  0.400mv--0.500mv
      {
        ScreenValue = SCREEN_0500 - (signed long)((SCREEN_0500 - SCREEN_0400)*(data_0500_zy_1 - real))/(data_0500_zy_1 - data_0400_zy_1);
      }
      else        
        if( ( real > data_0500_zy_1 )&&( real <= data_0600_zy_1 ) )//  0.500mv--0.600mv
        {
          ScreenValue = SCREEN_0600 - (signed long)((SCREEN_0600 - SCREEN_0500)*(data_0600_zy_1 - real))/(data_0600_zy_1 - data_0500_zy_1);
        }
        else
          if( ( real > data_0600_zy_1 )&&( real <= data_0700_zy_1 ) )//  0.600mv--0.700mv
          {
            ScreenValue = SCREEN_0700 - (signed long)((SCREEN_0700 - SCREEN_0600)*(data_0700_zy_1 - real))/(data_0700_zy_1 - data_0600_zy_1);
          }
          else
            if( ( real > data_0700_zy_1 )&&( real <= data_0800_zy_1 ) )//  0.700mv--0.800mv
            {
              ScreenValue = SCREEN_0800 - (signed long)((SCREEN_0800 - SCREEN_0700)*(data_0800_zy_1 - real))/(data_0800_zy_1 - data_0700_zy_1);
            }
            else
              if( ( real > data_0800_zy_1 )&&( real <= data_0900_zy_1 ) )//  0.800mv--0.900mv
              {
                ScreenValue = SCREEN_0900 - (signed long)((SCREEN_0900 - SCREEN_0800)*(data_0900_zy_1 - real))/(data_0900_zy_1 - data_0800_zy_1);
              }
              else
                if( ( real > data_0900_zy_1 )&&( real <= data_1000_zy_1 ) )//  0.900mv--1.000mv
                {
                  ScreenValue = SCREEN_1000 - (signed long)((SCREEN_1000 - SCREEN_0900)*(data_1000_zy_1 - real))/(data_1000_zy_1 - data_0900_zy_1);
                }
                else            
                  if( ( real > data_1000_zy_1 )&&( real <= data_2000_zy_1 ) )//  1.000mv--2.000mv
                  {
                    ScreenValue = RealValue_to_ScreenValue( SCREEN_2000,SCREEN_1000,data_2000_zy_1,data_1000_zy_1,real );
                  }
                  else
                    if( ( real > data_2000_zy_1 )&&( real <= data_3000_zy_1 ) )//  2.000mv--3.000mv
                    {
                      ScreenValue = RealValue_to_ScreenValue( SCREEN_3000,SCREEN_2000,data_3000_zy_1,data_2000_zy_1,real );
                    }
                    else
                      if( ( real > data_3000_zy_1 )&&( real <= data_4000_zy_1 ) )//  3.000mv--4.000mv
                      {
                        ScreenValue = RealValue_to_ScreenValue( SCREEN_4000,SCREEN_3000,data_4000_zy_1,data_3000_zy_1,real );
                      }
                      else
                        if( ( real > data_4000_zy_1 )&&( real <= data_5000_zy_1 ) )//  4.000mv--5.000mv
                        {
                          ScreenValue = RealValue_to_ScreenValue( SCREEN_5000,SCREEN_4000,data_5000_zy_1,data_4000_zy_1,real );
                        }
                        else
                          if( ( real > data_5000_zy_1 )&&( real <= data_6000_zy_1 ) )//  5.000mv--6.000mv
                          {
                            ScreenValue = RealValue_to_ScreenValue( SCREEN_6000,SCREEN_5000,data_6000_zy_1,data_5000_zy_1,real );
                          }
                          else
                            if( ( real > data_6000_zy_1 )&&( real <= data_7000_zy_1 ) )//  6.000mv--7.000mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_7000,SCREEN_6000,data_7000_zy_1,data_6000_zy_1,real );
                            }
                            else
                              if( ( real > data_7000_zy_1 )&&( real <= data_8000_zy_1 ) )//  7.000mv--8.000mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_8000,SCREEN_7000,data_8000_zy_1,data_7000_zy_1,real );
                              }
                              else
                                if( ( real > data_8000_zy_1 )&&( real <= data_9000_zy_1 ) )//  8.000mv--9.000mv
                                {
                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_9000,SCREEN_8000,data_9000_zy_1,data_8000_zy_1,real );
                                }
                                else
                                  if( ( real > data_9000_zy_1 )&&( real <= data_10000_zy_1 ) )//  9.000mv--10.000mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_10000,SCREEN_9000,data_10000_zy_1,data_9000_zy_1,real );
                                  }
                                  else
                                    if( ( real > data_10000_zy_1 )&&( real <= data_20000_zy_1 ) )//  10.000mv--20.000mv
                                    {
                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_20000,SCREEN_10000,data_20000_zy_1,data_10000_zy_1,real );
                                    }
                                    else
                                      if( ( real > data_20000_zy_1 )&&( real <= data_30000_zy_1 ) )//  20.000mv--30.000mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_30000,SCREEN_20000,data_30000_zy_1,data_20000_zy_1,real );
                                      }
                                      else
                                        if( ( real > data_30000_zy_1 )&&( real <= data_40000_zy_1 ) )//  30.000mv--40.000mv
                                        {
                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_40000,SCREEN_30000,data_40000_zy_1,data_30000_zy_1,real );
                                        }
                                        else
                                          if( ( real > data_40000_zy_1 )&&( real <= data_50000_zy_1 ) )//  40.000mv--50.000mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_50000,SCREEN_40000,data_50000_zy_1,data_40000_zy_1,real );
                                          }
                                          else
                                            if( ( real > data_50000_zy_1 )&&( real <= data_100000_zy_1 ) )//  50.000mv--100.000mv
                                            {
                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_100000,SCREEN_50000,data_100000_zy_1,data_50000_zy_1,real );
                                            }
                                            else
                                              if( ( real > data_100000_zy_1 )&&( real <= data_200000_zy_1 ) )//  100.000mv--200.000mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_200000,SCREEN_100000,data_200000_zy_1,data_100000_zy_1,real );
                                              }
                                              else
                                                if( ( real > data_200000_zy_1 )&&( real <= data_300000_zy_1 ) )//  200.000mv--300.000mv
                                                {
                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_300000,SCREEN_200000,data_300000_zy_1,data_200000_zy_1,real );
                                                }
                                                else
                                                  if( ( real > data_300000_zy_1 )&&( real <= data_400000_zy_1 ) )//  300.000mv--400.000mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_400000,SCREEN_300000,data_400000_zy_1,data_300000_zy_1,real );
                                                  }
                                                  else
                                                    if( ( real > data_400000_zy_1 )&&( real <= data_500000_zy_1 ) )//  400.000mv--500.000mv
                                                    {
                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_500000,SCREEN_400000,data_500000_zy_1,data_400000_zy_1,real );
                                                    }
                                                    else
                                                      if( ( real > data_500000_zy_1 )&&( real <= data_550000_zy_1 ) )//  500.000mv--550.000mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_550000,SCREEN_500000,data_550000_zy_1,data_500000_zy_1,real );
                                                      }
  //
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long ScreenValueForZengYi_2( signed long real )
函数功能： 计算得到屏显数据，增益2
说    明： 1、real --- AD7711计算得到的真实电压值 ，未经过修正
创建时间： 2010-07-10
修改时间： 2010-07-10
*******************************************************************************/
signed long ScreenValueForZengYi_2( signed long real )
{
  signed long ScreenValue = 0;

  if( real <= data_0000_zy_2 )// 0
  {
    ScreenValue = 0;
  }
  else
    if( ( real > data_0000_zy_2 )&&( real <= data_00001_zy_2 ) )//  0.001mv--0.000mv
    {
      ScreenValue = RealValue_to_ScreenValue( SCREEN_00001,SCREEN_0,data_00001_zy_2,data_0000_zy_2,real );
    }
    else
      if( ( real > data_00001_zy_2 )&&( real <= data_00005_zy_2 ) )//  0.001mv--0.005mv
      {
        ScreenValue = RealValue_to_ScreenValue( SCREEN_00005,SCREEN_00001,data_00005_zy_2,data_00001_zy_2,real );
      }
      else
        if( ( real > data_00005_zy_2 )&&( real <= data_00010_zy_2 ) )//  0.005mv--0.010mv
        {
          ScreenValue = RealValue_to_ScreenValue( SCREEN_00010,SCREEN_00005,data_00010_zy_2,data_00005_zy_2,real );
        }      
        else
                            if( ( real > data_00010_zy_2 )&&( real <= data_00015_zy_2 ) )//  0.010mv--0.015mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00015,SCREEN_00010,data_00015_zy_2,data_00010_zy_2,real );
                            }
                            else
                              if( ( real > data_00015_zy_2 )&&( real <= data_00020_zy_2 ) )//  0.015mv--0.020mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00020,SCREEN_00015,data_00020_zy_2,data_00015_zy_2,real );
                              }                              
                                else
                                  if( ( real > data_00020_zy_2 )&&( real <= data_00040_zy_2 ) )//  0.020mv--0.040mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00040,SCREEN_00020,data_00040_zy_2,data_00020_zy_2,real );
                                  }                                  
                                    else
                                      if( ( real > data_00040_zy_2 )&&( real <= data_00060_zy_2 ) )//  0.040mv--0.060mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00060,SCREEN_00040,data_00060_zy_2,data_00040_zy_2,real );
                                      }
                                      else                                        
                                          if( ( real > data_00060_zy_2 )&&( real <= data_00080_zy_2 ) )//  0.060mv--0.080mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_00080,SCREEN_00060,data_00080_zy_2,data_00060_zy_2,real );
                                          }
                                          else                                            
                                              if( ( real > data_00080_zy_2 )&&( real <= data_00100_zy_2 ) )//  0.080mv--0.100mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00100,SCREEN_00080,data_00100_zy_2,data_00080_zy_2,real );
                                              }
                                              else                                                
                                                  if( ( real > data_00100_zy_2 )&&( real <= data_00120_zy_2 ) )//  0.100mv--0.120mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00120,SCREEN_00100,data_00120_zy_2,data_00100_zy_2,real );
                                                  }
                                                  else                                                    
                                                      if( ( real > data_00120_zy_2 )&&( real <= data_00140_zy_2 ) )//  0.120mv--0.140mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00140,SCREEN_00120,data_00140_zy_2,data_00120_zy_2,real );
                                                      }
                                                      else                                                                                                                 
                                                            if( ( real > data_00140_zy_2 )&&( real <= data_00190_zy_2 ) )//  0.140mv--0.190mv
                                                            {
                                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00190,SCREEN_00140,data_00190_zy_2,data_00140_zy_2,real );
                                                            }
                                                            else                                                              
                                                                if( ( real > data_00190_zy_2 )&&( real <= data_00250_zy_2 ) )//  0.190mv--0.250mv
                                                                {
                                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_00250,SCREEN_00190,data_00250_zy_2,data_00190_zy_2,real );
                                                                }
                                                                else                                                                  
                                                                    if( ( real > data_00250_zy_2 )&&( real <= data_00330_zy_2 ) )//  0.250mv--0.330mv
                                                                    {
                                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_00330,SCREEN_00250,data_00330_zy_2,data_00250_zy_2,real );
                                                                    }
                                                                    else                                                                      
                                                                        if( ( real > data_00330_zy_2 )&&( real <= data_0400_zy_2 ) )//  0.330mv--0.400mv
                                                                        {
                                                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_0400,SCREEN_00330,data_0400_zy_2,data_00330_zy_2,real );
                                                                        }
                                                                        else
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////         
      if( ( real > data_0400_zy_2 )&&( real <= data_0500_zy_2 ) )//  0.400mv--0.500mv
      {
        ScreenValue = SCREEN_0500 - (signed long)((SCREEN_0500 - SCREEN_0400)*(data_0500_zy_2 - real))/(data_0500_zy_2 - data_0400_zy_2);
      }
      else        
        if( ( real > data_0500_zy_2 )&&( real <= data_0600_zy_2 ) )//  0.500mv--0.600mv
        {
          ScreenValue = SCREEN_0600 - (signed long)((SCREEN_0600 - SCREEN_0500)*(data_0600_zy_2 - real))/(data_0600_zy_2 - data_0500_zy_2);
        }
        else
          if( ( real > data_0600_zy_2 )&&( real <= data_0700_zy_2 ) )//  0.600mv--0.700mv
          {
            ScreenValue = SCREEN_0700 - (signed long)((SCREEN_0700 - SCREEN_0600)*(data_0700_zy_2 - real))/(data_0700_zy_2 - data_0600_zy_2);
          }
          else
            if( ( real > data_0700_zy_2 )&&( real <= data_0800_zy_2 ) )//  0.700mv--0.800mv
            {
              ScreenValue = SCREEN_0800 - (signed long)((SCREEN_0800 - SCREEN_0700)*(data_0800_zy_2 - real))/(data_0800_zy_2 - data_0700_zy_2);
            }
            else
              if( ( real > data_0800_zy_2 )&&( real <= data_0900_zy_2 ) )//  0.800mv--0.900mv
              {
                ScreenValue = SCREEN_0900 - (signed long)((SCREEN_0900 - SCREEN_0800)*(data_0900_zy_2 - real))/(data_0900_zy_2 - data_0800_zy_2);
              }
              else
                if( ( real > data_0900_zy_2 )&&( real <= data_1000_zy_2 ) )//  0.900mv--1.000mv
                {
                  ScreenValue = SCREEN_1000 - (signed long)((SCREEN_1000 - SCREEN_0900)*(data_1000_zy_2 - real))/(data_1000_zy_2 - data_0900_zy_2);
                }
                else            
                  if( ( real > data_1000_zy_2 )&&( real <= data_2000_zy_2 ) )//  1.000mv--2.000mv
                  {
                    ScreenValue = RealValue_to_ScreenValue( SCREEN_2000,SCREEN_1000,data_2000_zy_2,data_1000_zy_2,real );
                  }
                  else
                    if( ( real > data_2000_zy_2 )&&( real <= data_3000_zy_2 ) )//  2.000mv--3.000mv
                    {
                      ScreenValue = RealValue_to_ScreenValue( SCREEN_3000,SCREEN_2000,data_3000_zy_2,data_2000_zy_2,real );
                    }
                    else
                      if( ( real > data_3000_zy_2 )&&( real <= data_4000_zy_2 ) )//  3.000mv--4.000mv
                      {
                        ScreenValue = RealValue_to_ScreenValue( SCREEN_4000,SCREEN_3000,data_4000_zy_2,data_3000_zy_2,real );
                      }
                      else
                        if( ( real > data_4000_zy_2 )&&( real <= data_5000_zy_2 ) )//  4.000mv--5.000mv
                        {
                          ScreenValue = RealValue_to_ScreenValue( SCREEN_5000,SCREEN_4000,data_5000_zy_2,data_4000_zy_2,real );
                        }
                        else
                          if( ( real > data_5000_zy_2 )&&( real <= data_6000_zy_2 ) )//  5.000mv--6.000mv
                          {
                            ScreenValue = RealValue_to_ScreenValue( SCREEN_6000,SCREEN_5000,data_6000_zy_2,data_5000_zy_2,real );
                          }
                          else
                            if( ( real > data_6000_zy_2 )&&( real <= data_7000_zy_2 ) )//  6.000mv--7.000mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_7000,SCREEN_6000,data_7000_zy_2,data_6000_zy_2,real );
                            }
                            else
                              if( ( real > data_7000_zy_2 )&&( real <= data_8000_zy_2 ) )//  7.000mv--8.000mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_8000,SCREEN_7000,data_8000_zy_2,data_7000_zy_2,real );
                              }
                              else
                                if( ( real > data_8000_zy_2 )&&( real <= data_9000_zy_2 ) )//  8.000mv--9.000mv
                                {
                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_9000,SCREEN_8000,data_9000_zy_2,data_8000_zy_2,real );
                                }
                                else
                                  if( ( real > data_9000_zy_2 )&&( real <= data_10000_zy_2 ) )//  9.000mv--10.000mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_10000,SCREEN_9000,data_10000_zy_2,data_9000_zy_2,real );
                                  }
                                  else
                                    if( ( real > data_10000_zy_2 )&&( real <= data_20000_zy_2 ) )//  10.000mv--20.000mv
                                    {
                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_20000,SCREEN_10000,data_20000_zy_2,data_10000_zy_2,real );
                                    }
                                    else
                                      if( ( real > data_20000_zy_2 )&&( real <= data_30000_zy_2 ) )//  20.000mv--30.000mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_30000,SCREEN_20000,data_30000_zy_2,data_20000_zy_2,real );
                                      }
                                      else
                                        if( ( real > data_30000_zy_2 )&&( real <= data_40000_zy_2 ) )//  30.000mv--40.000mv
                                        {
                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_40000,SCREEN_30000,data_40000_zy_2,data_30000_zy_2,real );
                                        }
                                        else
                                          if( ( real > data_40000_zy_2 )&&( real <= data_50000_zy_2 ) )//  40.000mv--50.000mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_50000,SCREEN_40000,data_50000_zy_2,data_40000_zy_2,real );
                                          }
                                          else
                                            if( ( real > data_50000_zy_2 )&&( real <= data_100000_zy_2 ) )//  50.000mv--100.000mv
                                            {
                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_100000,SCREEN_50000,data_100000_zy_2,data_50000_zy_2,real );
                                            }
                                            else
                                              if( ( real > data_100000_zy_2 )&&( real <= data_200000_zy_2 ) )//  100.000mv--200.000mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_200000,SCREEN_100000,data_200000_zy_2,data_100000_zy_2,real );
                                              }
                                              else
                                                if( ( real > data_200000_zy_2 )&&( real <= data_300000_zy_2 ) )//  200.000mv--300.000mv
                                                {
                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_300000,SCREEN_200000,data_300000_zy_2,data_200000_zy_2,real );
                                                }
                                                else
                                                  if( ( real > data_300000_zy_2 )&&( real <= data_400000_zy_2 ) )//  300.000mv--400.000mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_400000,SCREEN_300000,data_400000_zy_2,data_300000_zy_2,real );
                                                  }
                                                  else
                                                    if( ( real > data_400000_zy_2 )&&( real <= data_500000_zy_2 ) )//  400.000mv--500.000mv
                                                    {
                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_500000,SCREEN_400000,data_500000_zy_2,data_400000_zy_2,real );
                                                    }
                                                    else
                                                      if( ( real > data_500000_zy_2 )&&( real <= data_550000_zy_2 ) )//  500.000mv--550.000mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_550000,SCREEN_500000,data_550000_zy_2,data_500000_zy_2,real );
                                                      }
  //
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long ScreenValueForZengYi_3( signed long real )
函数功能： 计算得到屏显数据，增益 3
说    明： 1、real --- AD7711计算得到的真实电压值 ，未经过修正
创建时间： 2010-07-10
修改时间： 2010-07-10
*******************************************************************************/
signed long ScreenValueForZengYi_3( signed long real )
{
  signed long ScreenValue = 0;

  if( real <= data_0000_zy_3 )// 0
  {
    ScreenValue = 0;
  }
  else
    if( ( real > data_0000_zy_3 )&&( real <= data_00001_zy_3 ) )//  0.001mv--0.000mv
    {
      ScreenValue = RealValue_to_ScreenValue( SCREEN_00001,SCREEN_0,data_00001_zy_3,data_0000_zy_3,real );
    }
    else
      if( ( real > data_00001_zy_3 )&&( real <= data_00005_zy_3 ) )//  0.001mv--0.005mv
      {
        ScreenValue = RealValue_to_ScreenValue( SCREEN_00005,SCREEN_00001,data_00005_zy_3,data_00001_zy_3,real );
      }
      else
        if( ( real > data_00005_zy_3 )&&( real <= data_00010_zy_3 ) )//  0.005mv--0.010mv
        {
          ScreenValue = RealValue_to_ScreenValue( SCREEN_00010,SCREEN_00005,data_00010_zy_3,data_00005_zy_3,real );
        }      
        else
                            if( ( real > data_00010_zy_3 )&&( real <= data_00015_zy_3 ) )//  0.010mv--0.015mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00015,SCREEN_00010,data_00015_zy_3,data_00010_zy_3,real );
                            }
                            else
                              if( ( real > data_00015_zy_3 )&&( real <= data_00020_zy_3 ) )//  0.015mv--0.020mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00020,SCREEN_00015,data_00020_zy_3,data_00015_zy_3,real );
                              }                              
                                else
                                  if( ( real > data_00020_zy_3 )&&( real <= data_00040_zy_3 ) )//  0.020mv--0.040mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00040,SCREEN_00020,data_00040_zy_3,data_00020_zy_3,real );
                                  }                                  
                                    else
                                      if( ( real > data_00040_zy_3 )&&( real <= data_00060_zy_3 ) )//  0.040mv--0.060mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00060,SCREEN_00040,data_00060_zy_3,data_00040_zy_3,real );
                                      }
                                      else                                        
                                          if( ( real > data_00060_zy_3 )&&( real <= data_00080_zy_3 ) )//  0.060mv--0.080mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_00080,SCREEN_00060,data_00080_zy_3,data_00060_zy_3,real );
                                          }
                                          else                                            
                                              if( ( real > data_00080_zy_3 )&&( real <= data_00100_zy_3 ) )//  0.080mv--0.100mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00100,SCREEN_00080,data_00100_zy_3,data_00080_zy_3,real );
                                              }
                                              else                                                
                                                  if( ( real > data_00100_zy_3 )&&( real <= data_00120_zy_3 ) )//  0.100mv--0.120mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00120,SCREEN_00100,data_00120_zy_3,data_00100_zy_3,real );
                                                  }
                                                  else                                                    
                                                      if( ( real > data_00120_zy_3 )&&( real <= data_00140_zy_3 ) )//  0.120mv--0.140mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00140,SCREEN_00120,data_00140_zy_3,data_00120_zy_3,real );
                                                      }
                                                      else                                                                                                                 
                                                            if( ( real > data_00140_zy_3 )&&( real <= data_00190_zy_3 ) )//  0.140mv--0.190mv
                                                            {
                                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00190,SCREEN_00140,data_00190_zy_3,data_00140_zy_3,real );
                                                            }
                                                            else                                                              
                                                                if( ( real > data_00190_zy_3 )&&( real <= data_00250_zy_3 ) )//  0.190mv--0.250mv
                                                                {
                                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_00250,SCREEN_00190,data_00250_zy_3,data_00190_zy_3,real );
                                                                }
                                                                else                                                                  
                                                                    if( ( real > data_00250_zy_3 )&&( real <= data_00330_zy_3 ) )//  0.250mv--0.330mv
                                                                    {
                                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_00330,SCREEN_00250,data_00330_zy_3,data_00250_zy_3,real );
                                                                    }
                                                                    else                                                                      
                                                                        if( ( real > data_00330_zy_3 )&&( real <= data_0400_zy_3 ) )//  0.330mv--0.400mv
                                                                        {
                                                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_0400,SCREEN_00330,data_0400_zy_3,data_00330_zy_3,real );
                                                                        }
                                                                        else
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
      if( ( real > data_0400_zy_3 )&&( real <= data_0500_zy_3 ) )//  0.400mv--0.500mv
      {
        ScreenValue = SCREEN_0500 - (signed long)((SCREEN_0500 - SCREEN_0400)*(data_0500_zy_3 - real))/(data_0500_zy_3 - data_0400_zy_3);
      }
      else        
        if( ( real > data_0500_zy_3 )&&( real <= data_0600_zy_3 ) )//  0.500mv--0.600mv
        {
          ScreenValue = SCREEN_0600 - (signed long)((SCREEN_0600 - SCREEN_0500)*(data_0600_zy_3 - real))/(data_0600_zy_3 - data_0500_zy_3);
        }
        else
          if( ( real > data_0600_zy_3 )&&( real <= data_0700_zy_3 ) )//  0.600mv--0.700mv
          {
            ScreenValue = SCREEN_0700 - (signed long)((SCREEN_0700 - SCREEN_0600)*(data_0700_zy_3 - real))/(data_0700_zy_3 - data_0600_zy_3);
          }
          else
            if( ( real > data_0700_zy_3 )&&( real <= data_0800_zy_3 ) )//  0.700mv--0.800mv
            {
              ScreenValue = SCREEN_0800 - (signed long)((SCREEN_0800 - SCREEN_0700)*(data_0800_zy_3 - real))/(data_0800_zy_3 - data_0700_zy_3);
            }
            else
              if( ( real > data_0800_zy_3 )&&( real <= data_0900_zy_3 ) )//  0.800mv--0.900mv
              {
                ScreenValue = SCREEN_0900 - (signed long)((SCREEN_0900 - SCREEN_0800)*(data_0900_zy_3 - real))/(data_0900_zy_3 - data_0800_zy_3);
              }
              else
                if( ( real > data_0900_zy_3 )&&( real <= data_1000_zy_3 ) )//  0.900mv--1.000mv
                {
                  ScreenValue = SCREEN_1000 - (signed long)((SCREEN_1000 - SCREEN_0900)*(data_1000_zy_3 - real))/(data_1000_zy_3 - data_0900_zy_3);
                }
                else            
                  if( ( real > data_1000_zy_3 )&&( real <= data_2000_zy_3 ) )//  1.000mv--2.000mv
                  {
                    ScreenValue = RealValue_to_ScreenValue( SCREEN_2000,SCREEN_1000,data_2000_zy_3,data_1000_zy_3,real );
                  }
                  else
                    if( ( real > data_2000_zy_3 )&&( real <= data_3000_zy_3 ) )//  2.000mv--3.000mv
                    {
                      ScreenValue = RealValue_to_ScreenValue( SCREEN_3000,SCREEN_2000,data_3000_zy_3,data_2000_zy_3,real );
                    }
                    else
                      if( ( real > data_3000_zy_3 )&&( real <= data_4000_zy_3 ) )//  3.000mv--4.000mv
                      {
                        ScreenValue = RealValue_to_ScreenValue( SCREEN_4000,SCREEN_3000,data_4000_zy_3,data_3000_zy_3,real );
                      }
                      else
                        if( ( real > data_4000_zy_3 )&&( real <= data_5000_zy_3 ) )//  4.000mv--5.000mv
                        {
                          ScreenValue = RealValue_to_ScreenValue( SCREEN_5000,SCREEN_4000,data_5000_zy_3,data_4000_zy_3,real );
                        }
                        else
                          if( ( real > data_5000_zy_3 )&&( real <= data_6000_zy_3 ) )//  5.000mv--6.000mv
                          {
                            ScreenValue = RealValue_to_ScreenValue( SCREEN_6000,SCREEN_5000,data_6000_zy_3,data_5000_zy_3,real );
                          }
                          else
                            if( ( real > data_6000_zy_3 )&&( real <= data_7000_zy_3 ) )//  6.000mv--7.000mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_7000,SCREEN_6000,data_7000_zy_3,data_6000_zy_3,real );
                            }
                            else
                              if( ( real > data_7000_zy_3 )&&( real <= data_8000_zy_3 ) )//  7.000mv--8.000mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_8000,SCREEN_7000,data_8000_zy_3,data_7000_zy_3,real );
                              }
                              else
                                if( ( real > data_8000_zy_3 )&&( real <= data_9000_zy_3 ) )//  8.000mv--9.000mv
                                {
                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_9000,SCREEN_8000,data_9000_zy_3,data_8000_zy_3,real );
                                }
                                else
                                  if( ( real > data_9000_zy_3 )&&( real <= data_10000_zy_3 ) )//  9.000mv--10.000mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_10000,SCREEN_9000,data_10000_zy_3,data_9000_zy_3,real );
                                  }
                                  else
                                    if( ( real > data_10000_zy_3 )&&( real <= data_20000_zy_3 ) )//  10.000mv--20.000mv
                                    {
                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_20000,SCREEN_10000,data_20000_zy_3,data_10000_zy_3,real );
                                    }
                                    else
                                      if( ( real > data_20000_zy_3 )&&( real <= data_30000_zy_3 ) )//  20.000mv--30.000mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_30000,SCREEN_20000,data_30000_zy_3,data_20000_zy_3,real );
                                      }
                                      else
                                        if( ( real > data_30000_zy_3 )&&( real <= data_40000_zy_3 ) )//  30.000mv--40.000mv
                                        {
                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_40000,SCREEN_30000,data_40000_zy_3,data_30000_zy_3,real );
                                        }
                                        else
                                          if( ( real > data_40000_zy_3 )&&( real <= data_50000_zy_3 ) )//  40.000mv--50.000mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_50000,SCREEN_40000,data_50000_zy_3,data_40000_zy_3,real );
                                          }
                                          else
                                            if( ( real > data_50000_zy_3 )&&( real <= data_100000_zy_3 ) )//  50.000mv--100.000mv
                                            {
                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_100000,SCREEN_50000,data_100000_zy_3,data_50000_zy_3,real );
                                            }
                                            else
                                              if( ( real > data_100000_zy_3 )&&( real <= data_200000_zy_3 ) )//  100.000mv--200.000mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_200000,SCREEN_100000,data_200000_zy_3,data_100000_zy_3,real );
                                              }
                                              else
                                                if( ( real > data_200000_zy_3 )&&( real <= data_300000_zy_3 ) )//  200.000mv--300.000mv
                                                {
                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_300000,SCREEN_200000,data_300000_zy_3,data_200000_zy_3,real );
                                                }
                                                else
                                                  if( ( real > data_300000_zy_3 )&&( real <= data_400000_zy_3 ) )//  300.000mv--400.000mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_400000,SCREEN_300000,data_400000_zy_3,data_300000_zy_3,real );
                                                  }
                                                  else
                                                    if( ( real > data_400000_zy_3 )&&( real <= data_500000_zy_3 ) )//  400.000mv--500.000mv
                                                    {
                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_500000,SCREEN_400000,data_500000_zy_3,data_400000_zy_3,real );
                                                    }
                                                    else
                                                      if( ( real > data_500000_zy_3 )&&( real <= data_550000_zy_3 ) )//  500.000mv--550.000mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_550000,SCREEN_500000,data_550000_zy_3,data_500000_zy_3,real );
                                                      }
  //
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long ScreenValueForZengYi_4( signed long real )
函数功能： 计算得到屏显数据，增益 4
说    明： 1、real --- AD7711计算得到的真实电压值 ，未经过修正
创建时间： 2010-07-10
修改时间： 2010-07-10
*******************************************************************************/
signed long ScreenValueForZengYi_4( signed long real )
{
  signed long ScreenValue = 0;
  
  if( real <= data_0000_zy_4 )// 0
  {
    ScreenValue = 0;
  }
  else
    if( ( real > data_0000_zy_4 )&&( real <= data_00001_zy_4 ) )//  0.001mv--0.000mv
    {
      ScreenValue = RealValue_to_ScreenValue( SCREEN_00001,SCREEN_0,data_00001_zy_4,data_0000_zy_4,real );
    }
    else
      if( ( real > data_00001_zy_4 )&&( real <= data_00005_zy_4 ) )//  0.001mv--0.005mv
      {
        ScreenValue = RealValue_to_ScreenValue( SCREEN_00005,SCREEN_00001,data_00005_zy_4,data_00001_zy_4,real );
      }
      else
        if( ( real > data_00005_zy_4 )&&( real <= data_00010_zy_4 ) )//  0.005mv--0.010mv
        {
          ScreenValue = RealValue_to_ScreenValue( SCREEN_00010,SCREEN_00005,data_00010_zy_4,data_00005_zy_4,real );
        }      
        else
                            if( ( real > data_00010_zy_4 )&&( real <= data_00015_zy_4 ) )//  0.010mv--0.015mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00015,SCREEN_00010,data_00015_zy_4,data_00010_zy_4,real );
                            }
                            else
                              if( ( real > data_00015_zy_4 )&&( real <= data_00020_zy_4 ) )//  0.015mv--0.020mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00020,SCREEN_00015,data_00020_zy_4,data_00015_zy_4,real );
                              }                              
                                else
                                  if( ( real > data_00020_zy_4 )&&( real <= data_00040_zy_4 ) )//  0.020mv--0.040mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00040,SCREEN_00020,data_00040_zy_4,data_00020_zy_4,real );
                                  }                                  
                                    else
                                      if( ( real > data_00040_zy_4 )&&( real <= data_00060_zy_4 ) )//  0.040mv--0.060mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00060,SCREEN_00040,data_00060_zy_4,data_00040_zy_4,real );
                                      }
                                      else                                        
                                          if( ( real > data_00060_zy_4 )&&( real <= data_00080_zy_4 ) )//  0.060mv--0.080mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_00080,SCREEN_00060,data_00080_zy_4,data_00060_zy_4,real );
                                          }
                                          else                                            
                                              if( ( real > data_00080_zy_4 )&&( real <= data_00100_zy_4 ) )//  0.080mv--0.100mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00100,SCREEN_00080,data_00100_zy_4,data_00080_zy_4,real );
                                              }
                                              else                                                
                                                  if( ( real > data_00100_zy_4 )&&( real <= data_00120_zy_4 ) )//  0.100mv--0.120mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00120,SCREEN_00100,data_00120_zy_4,data_00100_zy_4,real );
                                                  }
                                                  else                                                    
                                                      if( ( real > data_00120_zy_4 )&&( real <= data_00140_zy_4 ) )//  0.120mv--0.140mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00140,SCREEN_00120,data_00140_zy_4,data_00120_zy_4,real );
                                                      }
                                                      else                                                                                                                 
                                                            if( ( real > data_00140_zy_4 )&&( real <= data_00190_zy_4 ) )//  0.140mv--0.190mv
                                                            {
                                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00190,SCREEN_00140,data_00190_zy_4,data_00140_zy_4,real );
                                                            }
                                                            else                                                              
                                                                if( ( real > data_00190_zy_4 )&&( real <= data_00250_zy_4 ) )//  0.190mv--0.250mv
                                                                {
                                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_00250,SCREEN_00190,data_00250_zy_4,data_00190_zy_4,real );
                                                                }
                                                                else                                                                  
                                                                    if( ( real > data_00250_zy_4 )&&( real <= data_00330_zy_4 ) )//  0.250mv--0.330mv
                                                                    {
                                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_00330,SCREEN_00250,data_00330_zy_4,data_00250_zy_4,real );
                                                                    }
                                                                    else                                                                      
                                                                        if( ( real > data_00330_zy_4 )&&( real <= data_0400_zy_4 ) )//  0.330mv--0.400mv
                                                                        {
                                                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_0400,SCREEN_00330,data_0400_zy_4,data_00330_zy_4,real );
                                                                        }
                                                                        else
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
      if( ( real > data_0400_zy_4 )&&( real <= data_0500_zy_4 ) )//  0.400mv--0.500mv
      {
        ScreenValue = SCREEN_0500 - (signed long)((SCREEN_0500 - SCREEN_0400)*(data_0500_zy_4 - real))/(data_0500_zy_4 - data_0400_zy_4);
      }
      else        
        if( ( real > data_0500_zy_4 )&&( real <= data_0600_zy_4 ) )//  0.500mv--0.600mv
        {
          ScreenValue = SCREEN_0600 - (signed long)((SCREEN_0600 - SCREEN_0500)*(data_0600_zy_4 - real))/(data_0600_zy_4 - data_0500_zy_4);
        }
        else
          if( ( real > data_0600_zy_4 )&&( real <= data_0700_zy_4 ) )//  0.600mv--0.700mv
          {
            ScreenValue = SCREEN_0700 - (signed long)((SCREEN_0700 - SCREEN_0600)*(data_0700_zy_4 - real))/(data_0700_zy_4 - data_0600_zy_4);
          }
          else
            if( ( real > data_0700_zy_4 )&&( real <= data_0800_zy_4 ) )//  0.700mv--0.800mv
            {
              ScreenValue = SCREEN_0800 - (signed long)((SCREEN_0800 - SCREEN_0700)*(data_0800_zy_4 - real))/(data_0800_zy_4 - data_0700_zy_4);
            }
            else
              if( ( real > data_0800_zy_4 )&&( real <= data_0900_zy_4 ) )//  0.800mv--0.900mv
              {
                ScreenValue = SCREEN_0900 - (signed long)((SCREEN_0900 - SCREEN_0800)*(data_0900_zy_4 - real))/(data_0900_zy_4 - data_0800_zy_4);
              }
              else
                if( ( real > data_0900_zy_4 )&&( real <= data_1000_zy_4 ) )//  0.900mv--1.000mv
                {
                  ScreenValue = SCREEN_1000 - (signed long)((SCREEN_1000 - SCREEN_0900)*(data_1000_zy_4 - real))/(data_1000_zy_4 - data_0900_zy_4);
                }
                else            
                  if( ( real > data_1000_zy_4 )&&( real <= data_2000_zy_4 ) )//  1.000mv--2.000mv
                  {
                    ScreenValue = RealValue_to_ScreenValue( SCREEN_2000,SCREEN_1000,data_2000_zy_4,data_1000_zy_4,real );
                  }
                  else
                    if( ( real > data_2000_zy_4 )&&( real <= data_3000_zy_4 ) )//  2.000mv--3.000mv
                    {
                      ScreenValue = RealValue_to_ScreenValue( SCREEN_3000,SCREEN_2000,data_3000_zy_4,data_2000_zy_4,real );
                    }
                    else
                      if( ( real > data_3000_zy_4 )&&( real <= data_4000_zy_4 ) )//  3.000mv--4.000mv
                      {
                        ScreenValue = RealValue_to_ScreenValue( SCREEN_4000,SCREEN_3000,data_4000_zy_4,data_3000_zy_4,real );
                      }
                      else
                        if( ( real > data_4000_zy_4 )&&( real <= data_5000_zy_4 ) )//  4.000mv--5.000mv
                        {
                          ScreenValue = RealValue_to_ScreenValue( SCREEN_5000,SCREEN_4000,data_5000_zy_4,data_4000_zy_4,real );
                        }
                        else
                          if( ( real > data_5000_zy_4 )&&( real <= data_6000_zy_4 ) )//  5.000mv--6.000mv
                          {
                            ScreenValue = RealValue_to_ScreenValue( SCREEN_6000,SCREEN_5000,data_6000_zy_4,data_5000_zy_4,real );
                          }
                          else
                            if( ( real > data_6000_zy_4 )&&( real <= data_7000_zy_4 ) )//  6.000mv--7.000mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_7000,SCREEN_6000,data_7000_zy_4,data_6000_zy_4,real );
                            }
                            else
                              if( ( real > data_7000_zy_4 )&&( real <= data_8000_zy_4 ) )//  7.000mv--8.000mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_8000,SCREEN_7000,data_8000_zy_4,data_7000_zy_4,real );
                              }
                              else
                                if( ( real > data_8000_zy_4 )&&( real <= data_9000_zy_4 ) )//  8.000mv--9.000mv
                                {
                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_9000,SCREEN_8000,data_9000_zy_4,data_8000_zy_4,real );
                                }
                                else
                                  if( ( real > data_9000_zy_4 )&&( real <= data_10000_zy_4 ) )//  9.000mv--10.000mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_10000,SCREEN_9000,data_10000_zy_4,data_9000_zy_4,real );
                                  }
                                  else
                                    if( ( real > data_10000_zy_4 )&&( real <= data_20000_zy_4 ) )//  10.000mv--20.000mv
                                    {
                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_20000,SCREEN_10000,data_20000_zy_4,data_10000_zy_4,real );
                                    }
                                    else
                                      if( ( real > data_20000_zy_4 )&&( real <= data_30000_zy_4 ) )//  20.000mv--30.000mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_30000,SCREEN_20000,data_30000_zy_4,data_20000_zy_4,real );
                                      }
                                      else
                                        if( ( real > data_30000_zy_4 )&&( real <= data_40000_zy_4 ) )//  30.000mv--40.000mv
                                        {
                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_40000,SCREEN_30000,data_40000_zy_4,data_30000_zy_4,real );
                                        }
                                        else
                                          if( ( real > data_40000_zy_4 )&&( real <= data_50000_zy_4 ) )//  40.000mv--50.000mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_50000,SCREEN_40000,data_50000_zy_4,data_40000_zy_4,real );
                                          }
                                          else
                                            if( ( real > data_50000_zy_4 )&&( real <= data_100000_zy_4 ) )//  50.000mv--100.000mv
                                            {
                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_100000,SCREEN_50000,data_100000_zy_4,data_50000_zy_4,real );
                                            }
                                            else
                                              if( ( real > data_100000_zy_4 )&&( real <= data_200000_zy_4 ) )//  100.000mv--200.000mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_200000,SCREEN_100000,data_200000_zy_4,data_100000_zy_4,real );
                                              }
                                              else
                                                if( ( real > data_200000_zy_4 )&&( real <= data_300000_zy_4 ) )//  200.000mv--300.000mv
                                                {
                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_300000,SCREEN_200000,data_300000_zy_4,data_200000_zy_4,real );
                                                }
                                                else
                                                  if( ( real > data_300000_zy_4 )&&( real <= data_400000_zy_4 ) )//  300.000mv--400.000mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_400000,SCREEN_300000,data_400000_zy_4,data_300000_zy_4,real );
                                                  }
                                                  else
                                                    if( ( real > data_400000_zy_4 )&&( real <= data_500000_zy_4 ) )//  400.000mv--500.000mv
                                                    {
                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_500000,SCREEN_400000,data_500000_zy_4,data_400000_zy_4,real );
                                                    }
                                                    else
                                                      if( ( real > data_500000_zy_4 )&&( real <= data_550000_zy_4 ) )//  500.000mv--550.000mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_550000,SCREEN_500000,data_550000_zy_4,data_500000_zy_4,real );
                                                      }
  //
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long ScreenValueForZengYi_5( signed long real )
函数功能： 计算得到屏显数据，增益 5
说    明： 1、real --- AD7711计算得到的真实电压值 ，未经过修正
创建时间： 2010-07-10
修改时间： 2010-07-10
*******************************************************************************/
signed long ScreenValueForZengYi_5( signed long real )
{
  signed long ScreenValue = 0;
  
  if( real <= data_0000_zy_5 )// 0
  {
    ScreenValue = 0;
  }
  else
    if( ( real > data_0000_zy_5 )&&( real <= data_00001_zy_5 ) )//  0.001mv--0.000mv
    {
      ScreenValue = RealValue_to_ScreenValue( SCREEN_00001,SCREEN_0,data_00001_zy_5,data_0000_zy_5,real );
    }
    else
      if( ( real > data_00001_zy_5 )&&( real <= data_00005_zy_5 ) )//  0.001mv--0.005mv
      {
        ScreenValue = RealValue_to_ScreenValue( SCREEN_00005,SCREEN_00001,data_00005_zy_5,data_00001_zy_5,real );
      }
      else
        if( ( real > data_00005_zy_5 )&&( real <= data_00010_zy_5 ) )//  0.005mv--0.010mv
        {
          ScreenValue = RealValue_to_ScreenValue( SCREEN_00010,SCREEN_00005,data_00010_zy_5,data_00005_zy_5,real );
        }      
        else
                            if( ( real > data_00010_zy_5 )&&( real <= data_00015_zy_5 ) )//  0.010mv--0.015mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00015,SCREEN_00010,data_00015_zy_5,data_00010_zy_5,real );
                            }
                            else
                              if( ( real > data_00015_zy_5 )&&( real <= data_00020_zy_5 ) )//  0.015mv--0.020mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00020,SCREEN_00015,data_00020_zy_5,data_00015_zy_5,real );
                              }                              
                                else
                                  if( ( real > data_00020_zy_5 )&&( real <= data_00040_zy_5 ) )//  0.020mv--0.040mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00040,SCREEN_00020,data_00040_zy_5,data_00020_zy_5,real );
                                  }                                  
                                    else
                                      if( ( real > data_00040_zy_5 )&&( real <= data_00060_zy_5 ) )//  0.040mv--0.060mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00060,SCREEN_00040,data_00060_zy_5,data_00040_zy_5,real );
                                      }
                                      else                                        
                                          if( ( real > data_00060_zy_5 )&&( real <= data_00080_zy_5 ) )//  0.060mv--0.080mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_00080,SCREEN_00060,data_00080_zy_5,data_00060_zy_5,real );
                                          }
                                          else                                            
                                              if( ( real > data_00080_zy_5 )&&( real <= data_00100_zy_5 ) )//  0.080mv--0.100mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00100,SCREEN_00080,data_00100_zy_5,data_00080_zy_5,real );
                                              }
                                              else                                                
                                                  if( ( real > data_00100_zy_5 )&&( real <= data_00120_zy_5 ) )//  0.100mv--0.120mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00120,SCREEN_00100,data_00120_zy_5,data_00100_zy_5,real );
                                                  }
                                                  else                                                    
                                                      if( ( real > data_00120_zy_5 )&&( real <= data_00140_zy_5 ) )//  0.120mv--0.140mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00140,SCREEN_00120,data_00140_zy_5,data_00120_zy_5,real );
                                                      }
                                                      else                                                                                                                 
                                                            if( ( real > data_00140_zy_5 )&&( real <= data_00190_zy_5 ) )//  0.140mv--0.190mv
                                                            {
                                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00190,SCREEN_00140,data_00190_zy_5,data_00140_zy_5,real );
                                                            }
                                                            else                                                              
                                                                if( ( real > data_00190_zy_5 )&&( real <= data_00250_zy_5 ) )//  0.190mv--0.250mv
                                                                {
                                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_00250,SCREEN_00190,data_00250_zy_5,data_00190_zy_5,real );
                                                                }
                                                                else                                                                  
                                                                    if( ( real > data_00250_zy_5 )&&( real <= data_00330_zy_5 ) )//  0.250mv--0.330mv
                                                                    {
                                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_00330,SCREEN_00250,data_00330_zy_5,data_00250_zy_5,real );
                                                                    }
                                                                    else                                                                      
                                                                        if( ( real > data_00330_zy_5 )&&( real <= data_0400_zy_5 ) )//  0.330mv--0.400mv
                                                                        {
                                                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_0400,SCREEN_00330,data_0400_zy_5,data_00330_zy_5,real );
                                                                        }
                                                                        else
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////           
      if( ( real > data_0400_zy_5 )&&( real <= data_0500_zy_5 ) )//  0.400mv--0.500mv
      {
        ScreenValue = SCREEN_0500 - (signed long)((SCREEN_0500 - SCREEN_0400)*(data_0500_zy_5 - real))/(data_0500_zy_5 - data_0400_zy_5);
      }
      else        
        if( ( real > data_0500_zy_5 )&&( real <= data_0600_zy_5 ) )//  0.500mv--0.600mv
        {
          ScreenValue = SCREEN_0600 - (signed long)((SCREEN_0600 - SCREEN_0500)*(data_0600_zy_5 - real))/(data_0600_zy_5 - data_0500_zy_5);
        }
        else
          if( ( real > data_0600_zy_5 )&&( real <= data_0700_zy_5 ) )//  0.600mv--0.700mv
          {
            ScreenValue = SCREEN_0700 - (signed long)((SCREEN_0700 - SCREEN_0600)*(data_0700_zy_5 - real))/(data_0700_zy_5 - data_0600_zy_5);
          }
          else
            if( ( real > data_0700_zy_5 )&&( real <= data_0800_zy_5 ) )//  0.700mv--0.800mv
            {
              ScreenValue = SCREEN_0800 - (signed long)((SCREEN_0800 - SCREEN_0700)*(data_0800_zy_5 - real))/(data_0800_zy_5 - data_0700_zy_5);
            }
            else
              if( ( real > data_0800_zy_5 )&&( real <= data_0900_zy_5 ) )//  0.800mv--0.900mv
              {
                ScreenValue = SCREEN_0900 - (signed long)((SCREEN_0900 - SCREEN_0800)*(data_0900_zy_5 - real))/(data_0900_zy_5 - data_0800_zy_5);
              }
              else
                if( ( real > data_0900_zy_5 )&&( real <= data_1000_zy_5 ) )//  0.900mv--1.000mv
                {
                  ScreenValue = SCREEN_1000 - (signed long)((SCREEN_1000 - SCREEN_0900)*(data_1000_zy_5 - real))/(data_1000_zy_5 - data_0900_zy_5);
                }
                else            
                  if( ( real > data_1000_zy_5 )&&( real <= data_2000_zy_5 ) )//  1.000mv--2.000mv
                  {
                    ScreenValue = RealValue_to_ScreenValue( SCREEN_2000,SCREEN_1000,data_2000_zy_5,data_1000_zy_5,real );
                  }
                  else
                    if( ( real > data_2000_zy_5 )&&( real <= data_3000_zy_5 ) )//  2.000mv--3.000mv
                    {
                      ScreenValue = RealValue_to_ScreenValue( SCREEN_3000,SCREEN_2000,data_3000_zy_5,data_2000_zy_5,real );
                    }
                    else
                      if( ( real > data_3000_zy_5 )&&( real <= data_4000_zy_5 ) )//  3.000mv--4.000mv
                      {
                        ScreenValue = RealValue_to_ScreenValue( SCREEN_4000,SCREEN_3000,data_4000_zy_5,data_3000_zy_5,real );
                      }
                      else
                        if( ( real > data_4000_zy_5 )&&( real <= data_5000_zy_5 ) )//  4.000mv--5.000mv
                        {
                          ScreenValue = RealValue_to_ScreenValue( SCREEN_5000,SCREEN_4000,data_5000_zy_5,data_4000_zy_5,real );
                        }
                        else
                          if( ( real > data_5000_zy_5 )&&( real <= data_6000_zy_5 ) )//  5.000mv--6.000mv
                          {
                            ScreenValue = RealValue_to_ScreenValue( SCREEN_6000,SCREEN_5000,data_6000_zy_5,data_5000_zy_5,real );
                          }
                          else
                            if( ( real > data_6000_zy_5 )&&( real <= data_7000_zy_5 ) )//  6.000mv--7.000mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_7000,SCREEN_6000,data_7000_zy_5,data_6000_zy_5,real );
                            }
                            else
                              if( ( real > data_7000_zy_5 )&&( real <= data_8000_zy_5 ) )//  7.000mv--8.000mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_8000,SCREEN_7000,data_8000_zy_5,data_7000_zy_5,real );
                              }
                              else
                                if( ( real > data_8000_zy_5 )&&( real <= data_9000_zy_5 ) )//  8.000mv--9.000mv
                                {
                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_9000,SCREEN_8000,data_9000_zy_5,data_8000_zy_5,real );
                                }
                                else
                                  if( ( real > data_9000_zy_5 )&&( real <= data_10000_zy_5 ) )//  9.000mv--10.000mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_10000,SCREEN_9000,data_10000_zy_5,data_9000_zy_5,real );
                                  }
                                  else
                                    if( ( real > data_10000_zy_5 )&&( real <= data_20000_zy_5 ) )//  10.000mv--20.000mv
                                    {
                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_20000,SCREEN_10000,data_20000_zy_5,data_10000_zy_5,real );
                                    }
                                    else
                                      if( ( real > data_20000_zy_5 )&&( real <= data_30000_zy_5 ) )//  20.000mv--30.000mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_30000,SCREEN_20000,data_30000_zy_5,data_20000_zy_5,real );
                                      }
                                      else
                                        if( ( real > data_30000_zy_5 )&&( real <= data_40000_zy_5 ) )//  30.000mv--40.000mv
                                        {
                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_40000,SCREEN_30000,data_40000_zy_5,data_30000_zy_5,real );
                                        }
                                        else
                                          if( ( real > data_40000_zy_5 )&&( real <= data_50000_zy_5 ) )//  40.000mv--50.000mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_50000,SCREEN_40000,data_50000_zy_5,data_40000_zy_5,real );
                                          }
                                          else
                                            if( ( real > data_50000_zy_5 )&&( real <= data_100000_zy_5 ) )//  50.000mv--100.000mv
                                            {
                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_100000,SCREEN_50000,data_100000_zy_5,data_50000_zy_5,real );
                                            }
                                            else
                                              if( ( real > data_100000_zy_5 )&&( real <= data_200000_zy_5 ) )//  100.000mv--200.000mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_200000,SCREEN_100000,data_200000_zy_5,data_100000_zy_5,real );
                                              }
                                              else
                                                if( ( real > data_200000_zy_5 )&&( real <= data_300000_zy_5 ) )//  200.000mv--300.000mv
                                                {
                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_300000,SCREEN_200000,data_300000_zy_5,data_200000_zy_5,real );
                                                }
                                                else
                                                  if( ( real > data_300000_zy_5 )&&( real <= data_400000_zy_5 ) )//  300.000mv--400.000mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_400000,SCREEN_300000,data_400000_zy_5,data_300000_zy_5,real );
                                                  }
                                                  else
                                                    if( ( real > data_400000_zy_5 )&&( real <= data_500000_zy_5 ) )//  400.000mv--500.000mv
                                                    {
                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_500000,SCREEN_400000,data_500000_zy_5,data_400000_zy_5,real );
                                                    }
                                                    else
                                                      if( ( real > data_500000_zy_5 )&&( real <= data_550000_zy_5 ) )//  500.000mv--550.000mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_550000,SCREEN_500000,data_550000_zy_5,data_500000_zy_5,real );
                                                      }
  //
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long ScreenValueForZengYi_6( signed long real )
函数功能： 计算得到屏显数据，增益 6
说    明： 1、real --- AD7711计算得到的真实电压值 ，未经过修正
创建时间： 2010-07-10
修改时间： 2010-07-10
*******************************************************************************/
signed long ScreenValueForZengYi_6( signed long real )
{
  signed long ScreenValue = 0;
  if( real <= data_0000_zy_6 )// 0
  {
    ScreenValue = 0;
  }
  else
    if( ( real > data_0000_zy_6 )&&( real <= data_00001_zy_6 ) )//  0.001mv--0.000mv
    {
      ScreenValue = RealValue_to_ScreenValue( SCREEN_00001,SCREEN_0,data_00001_zy_6,data_0000_zy_6,real );
    }
    else
      if( ( real > data_00001_zy_6 )&&( real <= data_00005_zy_6 ) )//  0.001mv--0.005mv
      {
        ScreenValue = RealValue_to_ScreenValue( SCREEN_00005,SCREEN_00001,data_00005_zy_6,data_00001_zy_6,real );
      }
      else
        if( ( real > data_00005_zy_6 )&&( real <= data_00010_zy_6 ) )//  0.005mv--0.010mv
        {
          ScreenValue = RealValue_to_ScreenValue( SCREEN_00010,SCREEN_00005,data_00010_zy_6,data_00005_zy_6,real );
        }      
        else
                            if( ( real > data_00010_zy_6 )&&( real <= data_00015_zy_6 ) )//  0.010mv--0.015mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00015,SCREEN_00010,data_00015_zy_6,data_00010_zy_6,real );
                            }
                            else
                              if( ( real > data_00015_zy_6 )&&( real <= data_00020_zy_6 ) )//  0.015mv--0.020mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00020,SCREEN_00015,data_00020_zy_6,data_00015_zy_6,real );
                              }                              
                                else
                                  if( ( real > data_00020_zy_6 )&&( real <= data_00040_zy_6 ) )//  0.020mv--0.040mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00040,SCREEN_00020,data_00040_zy_6,data_00020_zy_6,real );
                                  }                                  
                                    else
                                      if( ( real > data_00040_zy_6 )&&( real <= data_00060_zy_6 ) )//  0.040mv--0.060mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00060,SCREEN_00040,data_00060_zy_6,data_00040_zy_6,real );
                                      }
                                      else                                        
                                          if( ( real > data_00060_zy_6 )&&( real <= data_00080_zy_6 ) )//  0.060mv--0.080mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_00080,SCREEN_00060,data_00080_zy_6,data_00060_zy_6,real );
                                          }
                                          else                                            
                                              if( ( real > data_00080_zy_6 )&&( real <= data_00100_zy_6 ) )//  0.080mv--0.100mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00100,SCREEN_00080,data_00100_zy_6,data_00080_zy_6,real );
                                              }
                                              else                                                
                                                  if( ( real > data_00100_zy_6 )&&( real <= data_00120_zy_6 ) )//  0.100mv--0.120mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00120,SCREEN_00100,data_00120_zy_6,data_00100_zy_6,real );
                                                  }
                                                  else                                                    
                                                      if( ( real > data_00120_zy_6 )&&( real <= data_00140_zy_6 ) )//  0.120mv--0.140mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00140,SCREEN_00120,data_00140_zy_6,data_00120_zy_6,real );
                                                      }
                                                      else                                                                                                                 
                                                            if( ( real > data_00140_zy_6 )&&( real <= data_00190_zy_6 ) )//  0.140mv--0.190mv
                                                            {
                                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00190,SCREEN_00140,data_00190_zy_6,data_00140_zy_6,real );
                                                            }
                                                            else                                                              
                                                                if( ( real > data_00190_zy_6 )&&( real <= data_00250_zy_6 ) )//  0.190mv--0.250mv
                                                                {
                                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_00250,SCREEN_00190,data_00250_zy_6,data_00190_zy_6,real );
                                                                }
                                                                else                                                                  
                                                                    if( ( real > data_00250_zy_6 )&&( real <= data_00330_zy_6 ) )//  0.250mv--0.330mv
                                                                    {
                                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_00330,SCREEN_00250,data_00330_zy_6,data_00250_zy_6,real );
                                                                    }
                                                                    else                                                                      
                                                                        if( ( real > data_00330_zy_6 )&&( real <= data_0400_zy_6 ) )//  0.330mv--0.400mv
                                                                        {
                                                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_0400,SCREEN_00330,data_0400_zy_6,data_00330_zy_6,real );
                                                                        }
                                                                        else
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
      if( ( real > data_0400_zy_6 )&&( real <= data_0500_zy_6 ) )//  0.400mv--0.500mv
      {
        ScreenValue = SCREEN_0500 - (signed long)((SCREEN_0500 - SCREEN_0400)*(data_0500_zy_6 - real))/(data_0500_zy_6 - data_0400_zy_6);
      }
      else        
        if( ( real > data_0500_zy_6 )&&( real <= data_0600_zy_6 ) )//  0.500mv--0.600mv
        {
          ScreenValue = SCREEN_0600 - (signed long)((SCREEN_0600 - SCREEN_0500)*(data_0600_zy_6 - real))/(data_0600_zy_6 - data_0500_zy_6);
        }
        else
          if( ( real > data_0600_zy_6 )&&( real <= data_0700_zy_6 ) )//  0.600mv--0.700mv
          {
            ScreenValue = SCREEN_0700 - (signed long)((SCREEN_0700 - SCREEN_0600)*(data_0700_zy_6 - real))/(data_0700_zy_6 - data_0600_zy_6);
          }
          else
            if( ( real > data_0700_zy_6 )&&( real <= data_0800_zy_6 ) )//  0.700mv--0.800mv
            {
              ScreenValue = SCREEN_0800 - (signed long)((SCREEN_0800 - SCREEN_0700)*(data_0800_zy_6 - real))/(data_0800_zy_6 - data_0700_zy_6);
            }
            else
              if( ( real > data_0800_zy_6 )&&( real <= data_0900_zy_6 ) )//  0.800mv--0.900mv
              {
                ScreenValue = SCREEN_0900 - (signed long)((SCREEN_0900 - SCREEN_0800)*(data_0900_zy_6 - real))/(data_0900_zy_6 - data_0800_zy_6);
              }
              else
                if( ( real > data_0900_zy_6 )&&( real <= data_1000_zy_6 ) )//  0.900mv--1.000mv
                {
                  ScreenValue = SCREEN_1000 - (signed long)((SCREEN_1000 - SCREEN_0900)*(data_1000_zy_6 - real))/(data_1000_zy_6 - data_0900_zy_6);
                }
                else            
                  if( ( real > data_1000_zy_6 )&&( real <= data_2000_zy_6 ) )//  1.000mv--2.000mv
                  {
                    ScreenValue = RealValue_to_ScreenValue( SCREEN_2000,SCREEN_1000,data_2000_zy_6,data_1000_zy_6,real );
                  }
                  else
                    if( ( real > data_2000_zy_6 )&&( real <= data_3000_zy_6 ) )//  2.000mv--3.000mv
                    {
                      ScreenValue = RealValue_to_ScreenValue( SCREEN_3000,SCREEN_2000,data_3000_zy_6,data_2000_zy_6,real );
                    }
                    else
                      if( ( real > data_3000_zy_6 )&&( real <= data_4000_zy_6 ) )//  3.000mv--4.000mv
                      {
                        ScreenValue = RealValue_to_ScreenValue( SCREEN_4000,SCREEN_3000,data_4000_zy_6,data_3000_zy_6,real );
                      }
                      else
                        if( ( real > data_4000_zy_6 )&&( real <= data_5000_zy_6 ) )//  4.000mv--5.000mv
                        {
                          ScreenValue = RealValue_to_ScreenValue( SCREEN_5000,SCREEN_4000,data_5000_zy_6,data_4000_zy_6,real );
                        }
                        else
                          if( ( real > data_5000_zy_6 )&&( real <= data_6000_zy_6 ) )//  5.000mv--6.000mv
                          {
                            ScreenValue = RealValue_to_ScreenValue( SCREEN_6000,SCREEN_5000,data_6000_zy_6,data_5000_zy_6,real );
                          }
                          else
                            if( ( real > data_6000_zy_6 )&&( real <= data_7000_zy_6 ) )//  6.000mv--7.000mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_7000,SCREEN_6000,data_7000_zy_6,data_6000_zy_6,real );
                            }
                            else
                              if( ( real > data_7000_zy_6 )&&( real <= data_8000_zy_6 ) )//  7.000mv--8.000mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_8000,SCREEN_7000,data_8000_zy_6,data_7000_zy_6,real );
                              }
                              else
                                if( ( real > data_8000_zy_6 )&&( real <= data_9000_zy_6 ) )//  8.000mv--9.000mv
                                {
                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_9000,SCREEN_8000,data_9000_zy_6,data_8000_zy_6,real );
                                }
                                else
                                  if( ( real > data_9000_zy_6 )&&( real <= data_10000_zy_6 ) )//  9.000mv--10.000mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_10000,SCREEN_9000,data_10000_zy_6,data_9000_zy_6,real );
                                  }
                                  else
                                    if( ( real > data_10000_zy_6 )&&( real <= data_20000_zy_6 ) )//  10.000mv--20.000mv
                                    {
                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_20000,SCREEN_10000,data_20000_zy_6,data_10000_zy_6,real );
                                    }
                                    else
                                      if( ( real > data_20000_zy_6 )&&( real <= data_30000_zy_6 ) )//  20.000mv--30.000mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_30000,SCREEN_20000,data_30000_zy_6,data_20000_zy_6,real );
                                      }
                                      else
                                        if( ( real > data_30000_zy_6 )&&( real <= data_40000_zy_6 ) )//  30.000mv--40.000mv
                                        {
                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_40000,SCREEN_30000,data_40000_zy_6,data_30000_zy_6,real );
                                        }
                                        else
                                          if( ( real > data_40000_zy_6 )&&( real <= data_50000_zy_6 ) )//  40.000mv--50.000mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_50000,SCREEN_40000,data_50000_zy_6,data_40000_zy_6,real );
                                          }
                                          else
                                            if( ( real > data_50000_zy_6 )&&( real <= data_100000_zy_6 ) )//  50.000mv--100.000mv
                                            {
                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_100000,SCREEN_50000,data_100000_zy_6,data_50000_zy_6,real );
                                            }
                                            else
                                              if( ( real > data_100000_zy_6 )&&( real <= data_200000_zy_6 ) )//  100.000mv--200.000mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_200000,SCREEN_100000,data_200000_zy_6,data_100000_zy_6,real );
                                              }
                                              else
                                                if( ( real > data_200000_zy_6 )&&( real <= data_300000_zy_6 ) )//  200.000mv--300.000mv
                                                {
                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_300000,SCREEN_200000,data_300000_zy_6,data_200000_zy_6,real );
                                                }
                                                else
                                                  if( ( real > data_300000_zy_6 )&&( real <= data_400000_zy_6 ) )//  300.000mv--400.000mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_400000,SCREEN_300000,data_400000_zy_6,data_300000_zy_6,real );
                                                  }
                                                  else
                                                    if( ( real > data_400000_zy_6 )&&( real <= data_500000_zy_6 ) )//  400.000mv--500.000mv
                                                    {
                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_500000,SCREEN_400000,data_500000_zy_6,data_400000_zy_6,real );
                                                    }
                                                    else
                                                      if( ( real > data_500000_zy_6 )&&( real <= data_550000_zy_6 ) )//  500.000mv--550.000mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_550000,SCREEN_500000,data_550000_zy_6,data_500000_zy_6,real );
                                                      }
  //
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long ScreenValueForZengYi_7( signed long real )
函数功能： 计算得到屏显数据，增益 7
说    明： 1、real --- AD7711计算得到的真实电压值 ，未经过修正
创建时间： 2010-07-10
修改时间： 2010-07-10
*******************************************************************************/
signed long ScreenValueForZengYi_7( signed long real )
{
  signed long ScreenValue = 0;
  
  if( real <= data_0000_zy_7 )// 0
  {
    ScreenValue = 0;
  }
  else
    if( ( real > data_0000_zy_7 )&&( real <= data_00001_zy_7 ) )//  0.001mv--0.000mv
    {
      ScreenValue = RealValue_to_ScreenValue( SCREEN_00001,SCREEN_0,data_00001_zy_7,data_0000_zy_7,real );
    }
    else
      if( ( real > data_00001_zy_7 )&&( real <= data_00005_zy_7 ) )//  0.001mv--0.005mv
      {
        ScreenValue = RealValue_to_ScreenValue( SCREEN_00005,SCREEN_00001,data_00005_zy_7,data_00001_zy_7,real );
      }
      else
        if( ( real > data_00005_zy_7 )&&( real <= data_00010_zy_7 ) )//  0.005mv--0.010mv
        {
          ScreenValue = RealValue_to_ScreenValue( SCREEN_00010,SCREEN_00005,data_00010_zy_7,data_00005_zy_7,real );
        }      
        else
                            if( ( real > data_00010_zy_7 )&&( real <= data_00015_zy_7 ) )//  0.010mv--0.015mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00015,SCREEN_00010,data_00015_zy_7,data_00010_zy_7,real );
                            }
                            else
                              if( ( real > data_00015_zy_7 )&&( real <= data_00020_zy_7 ) )//  0.015mv--0.020mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00020,SCREEN_00015,data_00020_zy_7,data_00015_zy_7,real );
                              }                              
                                else
                                  if( ( real > data_00020_zy_7 )&&( real <= data_00040_zy_7 ) )//  0.020mv--0.040mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00040,SCREEN_00020,data_00040_zy_7,data_00020_zy_7,real );
                                  }                                  
                                    else
                                      if( ( real > data_00040_zy_7 )&&( real <= data_00060_zy_7 ) )//  0.040mv--0.060mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00060,SCREEN_00040,data_00060_zy_7,data_00040_zy_7,real );
                                      }
                                      else                                        
                                          if( ( real > data_00060_zy_7 )&&( real <= data_00080_zy_7 ) )//  0.060mv--0.080mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_00080,SCREEN_00060,data_00080_zy_7,data_00060_zy_7,real );
                                          }
                                          else                                            
                                              if( ( real > data_00080_zy_7 )&&( real <= data_00100_zy_7 ) )//  0.080mv--0.100mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00100,SCREEN_00080,data_00100_zy_7,data_00080_zy_7,real );
                                              }
                                              else                                                
                                                  if( ( real > data_00100_zy_7 )&&( real <= data_00120_zy_7 ) )//  0.100mv--0.120mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00120,SCREEN_00100,data_00120_zy_7,data_00100_zy_7,real );
                                                  }
                                                  else                                                    
                                                      if( ( real > data_00120_zy_7 )&&( real <= data_00140_zy_7 ) )//  0.120mv--0.140mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00140,SCREEN_00120,data_00140_zy_7,data_00120_zy_7,real );
                                                      }
                                                      else                                                                                                                 
                                                            if( ( real > data_00140_zy_7 )&&( real <= data_00190_zy_7 ) )//  0.140mv--0.190mv
                                                            {
                                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00190,SCREEN_00140,data_00190_zy_7,data_00140_zy_7,real );
                                                            }
                                                            else                                                              
                                                                if( ( real > data_00190_zy_7 )&&( real <= data_00250_zy_7 ) )//  0.190mv--0.250mv
                                                                {
                                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_00250,SCREEN_00190,data_00250_zy_7,data_00190_zy_7,real );
                                                                }
                                                                else                                                                  
                                                                    if( ( real > data_00250_zy_7 )&&( real <= data_00330_zy_7 ) )//  0.250mv--0.330mv
                                                                    {
                                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_00330,SCREEN_00250,data_00330_zy_7,data_00250_zy_7,real );
                                                                    }
                                                                    else                                                                      
                                                                        if( ( real > data_00330_zy_7 )&&( real <= data_0400_zy_7 ) )//  0.330mv--0.400mv
                                                                        {
                                                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_0400,SCREEN_00330,data_0400_zy_7,data_00330_zy_7,real );
                                                                        }
                                                                        else
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
      if( ( real > data_0400_zy_7 )&&( real <= data_0500_zy_7 ) )//  0.400mv--0.500mv
      {
        ScreenValue = SCREEN_0500 - (signed long)((SCREEN_0500 - SCREEN_0400)*(data_0500_zy_7 - real))/(data_0500_zy_7 - data_0400_zy_7);
      }
      else        
        if( ( real > data_0500_zy_7 )&&( real <= data_0600_zy_7 ) )//  0.500mv--0.600mv
        {
          ScreenValue = SCREEN_0600 - (signed long)((SCREEN_0600 - SCREEN_0500)*(data_0600_zy_7 - real))/(data_0600_zy_7 - data_0500_zy_7);
        }
        else
          if( ( real > data_0600_zy_7 )&&( real <= data_0700_zy_7 ) )//  0.600mv--0.700mv
          {
            ScreenValue = SCREEN_0700 - (signed long)((SCREEN_0700 - SCREEN_0600)*(data_0700_zy_7 - real))/(data_0700_zy_7 - data_0600_zy_7);
          }
          else
            if( ( real > data_0700_zy_7 )&&( real <= data_0800_zy_7 ) )//  0.700mv--0.800mv
            {
              ScreenValue = SCREEN_0800 - (signed long)((SCREEN_0800 - SCREEN_0700)*(data_0800_zy_7 - real))/(data_0800_zy_7 - data_0700_zy_7);
            }
            else
              if( ( real > data_0800_zy_7 )&&( real <= data_0900_zy_7 ) )//  0.800mv--0.900mv
              {
                ScreenValue = SCREEN_0900 - (signed long)((SCREEN_0900 - SCREEN_0800)*(data_0900_zy_7 - real))/(data_0900_zy_7 - data_0800_zy_7);
              }
              else
                if( ( real > data_0900_zy_7 )&&( real <= data_1000_zy_7 ) )//  0.900mv--1.000mv
                {
                  ScreenValue = SCREEN_1000 - (signed long)((SCREEN_1000 - SCREEN_0900)*(data_1000_zy_7 - real))/(data_1000_zy_7 - data_0900_zy_7);
                }
                else            
                  if( ( real > data_1000_zy_7 )&&( real <= data_2000_zy_7 ) )//  1.000mv--2.000mv
                  {
                    ScreenValue = RealValue_to_ScreenValue( SCREEN_2000,SCREEN_1000,data_2000_zy_7,data_1000_zy_7,real );
                  }
                  else
                    if( ( real > data_2000_zy_7 )&&( real <= data_3000_zy_7 ) )//  2.000mv--3.000mv
                    {
                      ScreenValue = RealValue_to_ScreenValue( SCREEN_3000,SCREEN_2000,data_3000_zy_7,data_2000_zy_7,real );
                    }
                    else
                      if( ( real > data_3000_zy_7 )&&( real <= data_4000_zy_7 ) )//  3.000mv--4.000mv
                      {
                        ScreenValue = RealValue_to_ScreenValue( SCREEN_4000,SCREEN_3000,data_4000_zy_7,data_3000_zy_7,real );
                      }
                      else
                        if( ( real > data_4000_zy_7 )&&( real <= data_5000_zy_7 ) )//  4.000mv--5.000mv
                        {
                          ScreenValue = RealValue_to_ScreenValue( SCREEN_5000,SCREEN_4000,data_5000_zy_7,data_4000_zy_7,real );
                        }
                        else
                          if( ( real > data_5000_zy_7 )&&( real <= data_6000_zy_7 ) )//  5.000mv--6.000mv
                          {
                            ScreenValue = RealValue_to_ScreenValue( SCREEN_6000,SCREEN_5000,data_6000_zy_7,data_5000_zy_7,real );
                          }
                          else
                            if( ( real > data_6000_zy_7 )&&( real <= data_7000_zy_7 ) )//  6.000mv--7.000mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_7000,SCREEN_6000,data_7000_zy_7,data_6000_zy_7,real );
                            }
                            else
                              if( ( real > data_7000_zy_7 )&&( real <= data_8000_zy_7 ) )//  7.000mv--8.000mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_8000,SCREEN_7000,data_8000_zy_7,data_7000_zy_7,real );
                              }
                              else
                                if( ( real > data_8000_zy_7 )&&( real <= data_9000_zy_7 ) )//  8.000mv--9.000mv
                                {
                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_9000,SCREEN_8000,data_9000_zy_7,data_8000_zy_7,real );
                                }
                                else
                                  if( ( real > data_9000_zy_7 )&&( real <= data_10000_zy_7 ) )//  9.000mv--10.000mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_10000,SCREEN_9000,data_10000_zy_7,data_9000_zy_7,real );
                                  }
                                  else
                                    if( ( real > data_10000_zy_7 )&&( real <= data_20000_zy_7 ) )//  10.000mv--20.000mv
                                    {
                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_20000,SCREEN_10000,data_20000_zy_7,data_10000_zy_7,real );
                                    }
                                    else
                                      if( ( real > data_20000_zy_7 )&&( real <= data_30000_zy_7 ) )//  20.000mv--30.000mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_30000,SCREEN_20000,data_30000_zy_7,data_20000_zy_7,real );
                                      }
                                      else
                                        if( ( real > data_30000_zy_7 )&&( real <= data_40000_zy_7 ) )//  30.000mv--40.000mv
                                        {
                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_40000,SCREEN_30000,data_40000_zy_7,data_30000_zy_7,real );
                                        }
                                        else
                                          if( ( real > data_40000_zy_7 )&&( real <= data_50000_zy_7 ) )//  40.000mv--50.000mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_50000,SCREEN_40000,data_50000_zy_7,data_40000_zy_7,real );
                                          }
                                          else
                                            if( ( real > data_50000_zy_7 )&&( real <= data_100000_zy_7 ) )//  50.000mv--100.000mv
                                            {
                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_100000,SCREEN_50000,data_100000_zy_7,data_50000_zy_7,real );
                                            }
                                            else
                                              if( ( real > data_100000_zy_7 )&&( real <= data_200000_zy_7 ) )//  100.000mv--200.000mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_200000,SCREEN_100000,data_200000_zy_7,data_100000_zy_7,real );
                                              }
                                              else
                                                if( ( real > data_200000_zy_7 )&&( real <= data_300000_zy_7 ) )//  200.000mv--300.000mv
                                                {
                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_300000,SCREEN_200000,data_300000_zy_7,data_200000_zy_7,real );
                                                }
                                                else
                                                  if( ( real > data_300000_zy_7 )&&( real <= data_400000_zy_7 ) )//  300.000mv--400.000mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_400000,SCREEN_300000,data_400000_zy_7,data_300000_zy_7,real );
                                                  }
                                                  else
                                                    if( ( real > data_400000_zy_7 )&&( real <= data_500000_zy_7 ) )//  400.000mv--500.000mv
                                                    {
                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_500000,SCREEN_400000,data_500000_zy_7,data_400000_zy_7,real );
                                                    }
                                                    else
                                                      if( ( real > data_500000_zy_7 )&&( real <= data_550000_zy_7 ) )//  500.000mv--550.000mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_550000,SCREEN_500000,data_550000_zy_7,data_500000_zy_7,real );
                                                      }
  //
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long ScreenValueForZengYi_8( signed long real )
函数功能： 计算得到屏显数据，增益 8
说    明： 1、real --- AD7711计算得到的真实电压值 ，未经过修正
创建时间： 2010-07-10
修改时间： 2010-07-10
*******************************************************************************/
signed long ScreenValueForZengYi_8( signed long real )
{
  signed long ScreenValue = 0;
  
  if( real <= data_0000_zy_8 )// 0
  {
    ScreenValue = 0;
  }
  else
    if( ( real > data_0000_zy_8 )&&( real <= data_00001_zy_8 ) )//  0.001mv--0.000mv
    {
      ScreenValue = RealValue_to_ScreenValue( SCREEN_00001,SCREEN_0,data_00001_zy_8,data_0000_zy_8,real );
    }
    else
      if( ( real > data_00001_zy_8 )&&( real <= data_00005_zy_8 ) )//  0.001mv--0.005mv
      {
        ScreenValue = RealValue_to_ScreenValue( SCREEN_00005,SCREEN_00001,data_00005_zy_8,data_00001_zy_8,real );
      }
      else
        if( ( real > data_00005_zy_8 )&&( real <= data_00010_zy_8 ) )//  0.005mv--0.010mv
        {
          ScreenValue = RealValue_to_ScreenValue( SCREEN_00010,SCREEN_00005,data_00010_zy_8,data_00005_zy_8,real );
        }      
        else
                            if( ( real > data_00010_zy_8 )&&( real <= data_00015_zy_8 ) )//  0.010mv--0.015mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00015,SCREEN_00010,data_00015_zy_8,data_00010_zy_8,real );
                            }
                            else
                              if( ( real > data_00015_zy_8 )&&( real <= data_00020_zy_8 ) )//  0.015mv--0.020mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00020,SCREEN_00015,data_00020_zy_8,data_00015_zy_8,real );
                              }                              
                                else
                                  if( ( real > data_00020_zy_8 )&&( real <= data_00040_zy_8 ) )//  0.020mv--0.040mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00040,SCREEN_00020,data_00040_zy_8,data_00020_zy_8,real );
                                  }                                  
                                    else
                                      if( ( real > data_00040_zy_8 )&&( real <= data_00060_zy_8 ) )//  0.040mv--0.060mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00060,SCREEN_00040,data_00060_zy_8,data_00040_zy_8,real );
                                      }
                                      else                                        
                                          if( ( real > data_00060_zy_8 )&&( real <= data_00080_zy_8 ) )//  0.060mv--0.080mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_00080,SCREEN_00060,data_00080_zy_8,data_00060_zy_8,real );
                                          }
                                          else                                            
                                              if( ( real > data_00080_zy_8 )&&( real <= data_00100_zy_8 ) )//  0.080mv--0.100mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00100,SCREEN_00080,data_00100_zy_8,data_00080_zy_8,real );
                                              }
                                              else                                                
                                                  if( ( real > data_00100_zy_8 )&&( real <= data_00120_zy_8 ) )//  0.100mv--0.120mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00120,SCREEN_00100,data_00120_zy_8,data_00100_zy_8,real );
                                                  }
                                                  else                                                    
                                                      if( ( real > data_00120_zy_8 )&&( real <= data_00140_zy_8 ) )//  0.120mv--0.140mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00140,SCREEN_00120,data_00140_zy_8,data_00120_zy_8,real );
                                                      }
                                                      else                                                                                                                 
                                                            if( ( real > data_00140_zy_8 )&&( real <= data_00190_zy_8 ) )//  0.140mv--0.190mv
                                                            {
                                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00190,SCREEN_00140,data_00190_zy_8,data_00140_zy_8,real );
                                                            }
                                                            else                                                              
                                                                if( ( real > data_00190_zy_8 )&&( real <= data_00250_zy_8 ) )//  0.190mv--0.250mv
                                                                {
                                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_00250,SCREEN_00190,data_00250_zy_8,data_00190_zy_8,real );
                                                                }
                                                                else                                                                  
                                                                    if( ( real > data_00250_zy_8 )&&( real <= data_00330_zy_8 ) )//  0.250mv--0.330mv
                                                                    {
                                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_00330,SCREEN_00250,data_00330_zy_8,data_00250_zy_8,real );
                                                                    }
                                                                    else                                                                      
                                                                        if( ( real > data_00330_zy_8 )&&( real <= data_0400_zy_8 ) )//  0.330mv--0.400mv
                                                                        {
                                                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_0400,SCREEN_00330,data_0400_zy_8,data_00330_zy_8,real );
                                                                        }
                                                                        else
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
      if( ( real > data_0400_zy_8 )&&( real <= data_0500_zy_8 ) )//  0.400mv--0.500mv
      {
        ScreenValue = SCREEN_0500 - (signed long)((SCREEN_0500 - SCREEN_0400)*(data_0500_zy_8 - real))/(data_0500_zy_8 - data_0400_zy_8);
      }
      else        
        if( ( real > data_0500_zy_8 )&&( real <= data_0600_zy_8 ) )//  0.500mv--0.600mv
        {
          ScreenValue = SCREEN_0600 - (signed long)((SCREEN_0600 - SCREEN_0500)*(data_0600_zy_8 - real))/(data_0600_zy_8 - data_0500_zy_8);
        }
        else
          if( ( real > data_0600_zy_8 )&&( real <= data_0700_zy_8 ) )//  0.600mv--0.700mv
          {
            ScreenValue = SCREEN_0700 - (signed long)((SCREEN_0700 - SCREEN_0600)*(data_0700_zy_8 - real))/(data_0700_zy_8 - data_0600_zy_8);
          }
          else
            if( ( real > data_0700_zy_8 )&&( real <= data_0800_zy_8 ) )//  0.700mv--0.800mv
            {
              ScreenValue = SCREEN_0800 - (signed long)((SCREEN_0800 - SCREEN_0700)*(data_0800_zy_8 - real))/(data_0800_zy_8 - data_0700_zy_8);
            }
            else
              if( ( real > data_0800_zy_8 )&&( real <= data_0900_zy_8 ) )//  0.800mv--0.900mv
              {
                ScreenValue = SCREEN_0900 - (signed long)((SCREEN_0900 - SCREEN_0800)*(data_0900_zy_8 - real))/(data_0900_zy_8 - data_0800_zy_8);
              }
              else
                if( ( real > data_0900_zy_8 )&&( real <= data_1000_zy_8 ) )//  0.900mv--1.000mv
                {
                  ScreenValue = SCREEN_1000 - (signed long)((SCREEN_1000 - SCREEN_0900)*(data_1000_zy_8 - real))/(data_1000_zy_8 - data_0900_zy_8);
                }
                else            
                  if( ( real > data_1000_zy_8 )&&( real <= data_2000_zy_8 ) )//  1.000mv--2.000mv
                  {
                    ScreenValue = RealValue_to_ScreenValue( SCREEN_2000,SCREEN_1000,data_2000_zy_8,data_1000_zy_8,real );
                  }
                  else
                    if( ( real > data_2000_zy_8 )&&( real <= data_3000_zy_8 ) )//  2.000mv--3.000mv
                    {
                      ScreenValue = RealValue_to_ScreenValue( SCREEN_3000,SCREEN_2000,data_3000_zy_8,data_2000_zy_8,real );
                    }
                    else
                      if( ( real > data_3000_zy_8 )&&( real <= data_4000_zy_8 ) )//  3.000mv--4.000mv
                      {
                        ScreenValue = RealValue_to_ScreenValue( SCREEN_4000,SCREEN_3000,data_4000_zy_8,data_3000_zy_8,real );
                      }
                      else
                        if( ( real > data_4000_zy_8 )&&( real <= data_5000_zy_8 ) )//  4.000mv--5.000mv
                        {
                          ScreenValue = RealValue_to_ScreenValue( SCREEN_5000,SCREEN_4000,data_5000_zy_8,data_4000_zy_8,real );
                        }
                        else
                          if( ( real > data_5000_zy_8 )&&( real <= data_6000_zy_8 ) )//  5.000mv--6.000mv
                          {
                            ScreenValue = RealValue_to_ScreenValue( SCREEN_6000,SCREEN_5000,data_6000_zy_8,data_5000_zy_8,real );
                          }
                          else
                            if( ( real > data_6000_zy_8 )&&( real <= data_7000_zy_8 ) )//  6.000mv--7.000mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_7000,SCREEN_6000,data_7000_zy_8,data_6000_zy_8,real );
                            }
                            else
                              if( ( real > data_7000_zy_8 )&&( real <= data_8000_zy_8 ) )//  7.000mv--8.000mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_8000,SCREEN_7000,data_8000_zy_8,data_7000_zy_8,real );
                              }
                              else
                                if( ( real > data_8000_zy_8 )&&( real <= data_9000_zy_8 ) )//  8.000mv--9.000mv
                                {
                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_9000,SCREEN_8000,data_9000_zy_8,data_8000_zy_8,real );
                                }
                                else
                                  if( ( real > data_9000_zy_8 )&&( real <= data_10000_zy_8 ) )//  9.000mv--10.000mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_10000,SCREEN_9000,data_10000_zy_8,data_9000_zy_8,real );
                                  }
                                  else
                                    if( ( real > data_10000_zy_8 )&&( real <= data_20000_zy_8 ) )//  10.000mv--20.000mv
                                    {
                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_20000,SCREEN_10000,data_20000_zy_8,data_10000_zy_8,real );
                                    }
                                    else
                                      if( ( real > data_20000_zy_8 )&&( real <= data_30000_zy_8 ) )//  20.000mv--30.000mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_30000,SCREEN_20000,data_30000_zy_8,data_20000_zy_8,real );
                                      }
                                      else
                                        if( ( real > data_30000_zy_8 )&&( real <= data_40000_zy_8 ) )//  30.000mv--40.000mv
                                        {
                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_40000,SCREEN_30000,data_40000_zy_8,data_30000_zy_8,real );
                                        }
                                        else
                                          if( ( real > data_40000_zy_8 )&&( real <= data_50000_zy_8 ) )//  40.000mv--50.000mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_50000,SCREEN_40000,data_50000_zy_8,data_40000_zy_8,real );
                                          }
                                          else
                                            if( ( real > data_50000_zy_8 )&&( real <= data_100000_zy_8 ) )//  50.000mv--100.000mv
                                            {
                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_100000,SCREEN_50000,data_100000_zy_8,data_50000_zy_8,real );
                                            }
                                            else
                                              if( ( real > data_100000_zy_8 )&&( real <= data_200000_zy_8 ) )//  100.000mv--200.000mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_200000,SCREEN_100000,data_200000_zy_8,data_100000_zy_8,real );
                                              }
                                              else
                                                if( ( real > data_200000_zy_8 )&&( real <= data_300000_zy_8 ) )//  200.000mv--300.000mv
                                                {
                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_300000,SCREEN_200000,data_300000_zy_8,data_200000_zy_8,real );
                                                }
                                                else
                                                  if( ( real > data_300000_zy_8 )&&( real <= data_400000_zy_8 ) )//  300.000mv--400.000mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_400000,SCREEN_300000,data_400000_zy_8,data_300000_zy_8,real );
                                                  }
                                                  else
                                                    if( ( real > data_400000_zy_8 )&&( real <= data_500000_zy_8 ) )//  400.000mv--500.000mv
                                                    {
                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_500000,SCREEN_400000,data_500000_zy_8,data_400000_zy_8,real );
                                                    }
                                                    else
                                                      if( ( real > data_500000_zy_8 )&&( real <= data_550000_zy_8 ) )//  500.000mv--550.000mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_550000,SCREEN_500000,data_550000_zy_8,data_500000_zy_8,real );
                                                      }
  //
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long ScreenValueForZengYi_9( signed long real )
函数功能： 计算得到屏显数据，增益 9
说    明： 1、real --- AD7711计算得到的真实电压值 ，未经过修正
创建时间： 2010-07-10
修改时间： 2010-07-10
*******************************************************************************/
signed long ScreenValueForZengYi_9( signed long real )
{
  signed long ScreenValue = 0;
  
  if( real <= data_0000_zy_9 )// 0
  {
    ScreenValue = 0;
  }
  else
    if( ( real > data_0000_zy_9 )&&( real <= data_00001_zy_9 ) )//  0.001mv--0.000mv
    {
      ScreenValue = RealValue_to_ScreenValue( SCREEN_00001,SCREEN_0,data_00001_zy_9,data_0000_zy_9,real );
    }
    else
      if( ( real > data_00001_zy_9 )&&( real <= data_00005_zy_9 ) )//  0.001mv--0.005mv
      {
        ScreenValue = RealValue_to_ScreenValue( SCREEN_00005,SCREEN_00001,data_00005_zy_9,data_00001_zy_9,real );
      }
      else
        if( ( real > data_00005_zy_9 )&&( real <= data_00010_zy_9 ) )//  0.005mv--0.010mv
        {
          ScreenValue = RealValue_to_ScreenValue( SCREEN_00010,SCREEN_00005,data_00010_zy_9,data_00005_zy_9,real );
        }      
        else
                            if( ( real > data_00010_zy_9 )&&( real <= data_00015_zy_9 ) )//  0.010mv--0.015mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00015,SCREEN_00010,data_00015_zy_9,data_00010_zy_9,real );
                            }
                            else
                              if( ( real > data_00015_zy_9 )&&( real <= data_00020_zy_9 ) )//  0.015mv--0.020mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00020,SCREEN_00015,data_00020_zy_9,data_00015_zy_9,real );
                              }                              
                                else
                                  if( ( real > data_00020_zy_9 )&&( real <= data_00040_zy_9 ) )//  0.020mv--0.040mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00040,SCREEN_00020,data_00040_zy_9,data_00020_zy_9,real );
                                  }                                  
                                    else
                                      if( ( real > data_00040_zy_9 )&&( real <= data_00060_zy_9 ) )//  0.040mv--0.060mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00060,SCREEN_00040,data_00060_zy_9,data_00040_zy_9,real );
                                      }
                                      else                                        
                                          if( ( real > data_00060_zy_9 )&&( real <= data_00080_zy_9 ) )//  0.060mv--0.080mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_00080,SCREEN_00060,data_00080_zy_9,data_00060_zy_9,real );
                                          }
                                          else                                            
                                              if( ( real > data_00080_zy_9 )&&( real <= data_00100_zy_9 ) )//  0.080mv--0.100mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_00100,SCREEN_00080,data_00100_zy_9,data_00080_zy_9,real );
                                              }
                                              else                                                
                                                  if( ( real > data_00100_zy_9 )&&( real <= data_00120_zy_9 ) )//  0.100mv--0.120mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_00120,SCREEN_00100,data_00120_zy_9,data_00100_zy_9,real );
                                                  }
                                                  else                                                    
                                                      if( ( real > data_00120_zy_9 )&&( real <= data_00140_zy_9 ) )//  0.120mv--0.140mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_00140,SCREEN_00120,data_00140_zy_9,data_00120_zy_9,real );
                                                      }
                                                      else                                                                                                                 
                                                            if( ( real > data_00140_zy_9 )&&( real <= data_00190_zy_9 ) )//  0.140mv--0.190mv
                                                            {
                                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_00190,SCREEN_00140,data_00190_zy_9,data_00140_zy_9,real );
                                                            }
                                                            else                                                              
                                                                if( ( real > data_00190_zy_9 )&&( real <= data_00250_zy_9 ) )//  0.190mv--0.250mv
                                                                {
                                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_00250,SCREEN_00190,data_00250_zy_9,data_00190_zy_9,real );
                                                                }
                                                                else                                                                  
                                                                    if( ( real > data_00250_zy_9 )&&( real <= data_00330_zy_9 ) )//  0.250mv--0.330mv
                                                                    {
                                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_00330,SCREEN_00250,data_00330_zy_9,data_00250_zy_9,real );
                                                                    }
                                                                    else                                                                      
                                                                        if( ( real > data_00330_zy_9 )&&( real <= data_0400_zy_9 ) )//  0.330mv--0.400mv
                                                                        {
                                                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_0400,SCREEN_00330,data_0400_zy_9,data_00330_zy_9,real );
                                                                        }
                                                                        else
  ///////////////////////////////////////////////////////////////////////////////////////////////////
      if( ( real > data_0400_zy_9 )&&( real <= data_0500_zy_9 ) )//  0.400mv--0.500mv
      {
        ScreenValue = SCREEN_0500 - (signed long)((SCREEN_0500 - SCREEN_0400)*(data_0500_zy_9 - real))/(data_0500_zy_9 - data_0400_zy_9);
      }
      else        
        if( ( real > data_0500_zy_9 )&&( real <= data_0600_zy_9 ) )//  0.500mv--0.600mv
        {
          ScreenValue = SCREEN_0600 - (signed long)((SCREEN_0600 - SCREEN_0500)*(data_0600_zy_9 - real))/(data_0600_zy_9 - data_0500_zy_9);
        }
        else
          if( ( real > data_0600_zy_9 )&&( real <= data_0700_zy_9 ) )//  0.600mv--0.700mv
          {
            ScreenValue = SCREEN_0700 - (signed long)((SCREEN_0700 - SCREEN_0600)*(data_0700_zy_9 - real))/(data_0700_zy_9 - data_0600_zy_9);
          }
          else
            if( ( real > data_0700_zy_9 )&&( real <= data_0800_zy_9 ) )//  0.700mv--0.800mv
            {
              ScreenValue = SCREEN_0800 - (signed long)((SCREEN_0800 - SCREEN_0700)*(data_0800_zy_9 - real))/(data_0800_zy_9 - data_0700_zy_9);
            }
            else
              if( ( real > data_0800_zy_9 )&&( real <= data_0900_zy_9 ) )//  0.800mv--0.900mv
              {
                ScreenValue = SCREEN_0900 - (signed long)((SCREEN_0900 - SCREEN_0800)*(data_0900_zy_9 - real))/(data_0900_zy_9 - data_0800_zy_9);
              }
              else
                if( ( real > data_0900_zy_9 )&&( real <= data_1000_zy_9 ) )//  0.900mv--1.000mv
                {
                  ScreenValue = SCREEN_1000 - (signed long)((SCREEN_1000 - SCREEN_0900)*(data_1000_zy_9 - real))/(data_1000_zy_9 - data_0900_zy_9);
                }
                else            
                  if( ( real > data_1000_zy_9 )&&( real <= data_2000_zy_9 ) )//  1.000mv--2.000mv
                  {
                    ScreenValue = RealValue_to_ScreenValue( SCREEN_2000,SCREEN_1000,data_2000_zy_9,data_1000_zy_9,real );
                  }
                  else
                    if( ( real > data_2000_zy_9 )&&( real <= data_3000_zy_9 ) )//  2.000mv--3.000mv
                    {
                      ScreenValue = RealValue_to_ScreenValue( SCREEN_3000,SCREEN_2000,data_3000_zy_9,data_2000_zy_9,real );
                    }
                    else
                      if( ( real > data_3000_zy_9 )&&( real <= data_4000_zy_9 ) )//  3.000mv--4.000mv
                      {
                        ScreenValue = RealValue_to_ScreenValue( SCREEN_4000,SCREEN_3000,data_4000_zy_9,data_3000_zy_9,real );
                      }
                      else
                        if( ( real > data_4000_zy_9 )&&( real <= data_5000_zy_9 ) )//  4.000mv--5.000mv
                        {
                          ScreenValue = RealValue_to_ScreenValue( SCREEN_5000,SCREEN_4000,data_5000_zy_9,data_4000_zy_9,real );
                        }
                        else
                          if( ( real > data_5000_zy_9 )&&( real <= data_6000_zy_9 ) )//  5.000mv--6.000mv
                          {
                            ScreenValue = RealValue_to_ScreenValue( SCREEN_6000,SCREEN_5000,data_6000_zy_9,data_5000_zy_9,real );
                          }
                          else
                            if( ( real > data_6000_zy_9 )&&( real <= data_7000_zy_9 ) )//  6.000mv--7.000mv
                            {
                              ScreenValue = RealValue_to_ScreenValue( SCREEN_7000,SCREEN_6000,data_7000_zy_9,data_6000_zy_9,real );
                            }
                            else
                              if( ( real > data_7000_zy_9 )&&( real <= data_8000_zy_9 ) )//  7.000mv--8.000mv
                              {
                                ScreenValue = RealValue_to_ScreenValue( SCREEN_8000,SCREEN_7000,data_8000_zy_9,data_7000_zy_9,real );
                              }
                              else
                                if( ( real > data_8000_zy_9 )&&( real <= data_9000_zy_9 ) )//  8.000mv--9.000mv
                                {
                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_9000,SCREEN_8000,data_9000_zy_9,data_8000_zy_9,real );
                                }
                                else
                                  if( ( real > data_9000_zy_9 )&&( real <= data_10000_zy_9 ) )//  9.000mv--10.000mv
                                  {
                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_10000,SCREEN_9000,data_10000_zy_9,data_9000_zy_9,real );
                                  }
                                  else
                                    if( ( real > data_10000_zy_9 )&&( real <= data_20000_zy_9 ) )//  10.000mv--20.000mv
                                    {
                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_20000,SCREEN_10000,data_20000_zy_9,data_10000_zy_9,real );
                                    }
                                    else
                                      if( ( real > data_20000_zy_9 )&&( real <= data_30000_zy_9 ) )//  20.000mv--30.000mv
                                      {
                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_30000,SCREEN_20000,data_30000_zy_9,data_20000_zy_9,real );
                                      }
                                      else
                                        if( ( real > data_30000_zy_9 )&&( real <= data_40000_zy_9 ) )//  30.000mv--40.000mv
                                        {
                                          ScreenValue = RealValue_to_ScreenValue( SCREEN_40000,SCREEN_30000,data_40000_zy_9,data_30000_zy_9,real );
                                        }
                                        else
                                          if( ( real > data_40000_zy_9 )&&( real <= data_50000_zy_9 ) )//  40.000mv--50.000mv
                                          {
                                            ScreenValue = RealValue_to_ScreenValue( SCREEN_50000,SCREEN_40000,data_50000_zy_9,data_40000_zy_9,real );
                                          }
                                          else
                                            if( ( real > data_50000_zy_9 )&&( real <= data_100000_zy_9 ) )//  50.000mv--100.000mv
                                            {
                                              ScreenValue = RealValue_to_ScreenValue( SCREEN_100000,SCREEN_50000,data_100000_zy_9,data_50000_zy_9,real );
                                            }
                                            else
                                              if( ( real > data_100000_zy_9 )&&( real <= data_200000_zy_9 ) )//  100.000mv--200.000mv
                                              {
                                                ScreenValue = RealValue_to_ScreenValue( SCREEN_200000,SCREEN_100000,data_200000_zy_9,data_100000_zy_9,real );
                                              }
                                              else
                                                if( ( real > data_200000_zy_9 )&&( real <= data_300000_zy_9 ) )//  200.000mv--300.000mv
                                                {
                                                  ScreenValue = RealValue_to_ScreenValue( SCREEN_300000,SCREEN_200000,data_300000_zy_9,data_200000_zy_9,real );
                                                }
                                                else
                                                  if( ( real > data_300000_zy_9 )&&( real <= data_400000_zy_9 ) )//  300.000mv--400.000mv
                                                  {
                                                    ScreenValue = RealValue_to_ScreenValue( SCREEN_400000,SCREEN_300000,data_400000_zy_9,data_300000_zy_9,real );
                                                  }
                                                  else
                                                    if( ( real > data_400000_zy_9 )&&( real <= data_500000_zy_9 ) )//  400.000mv--500.000mv
                                                    {
                                                      ScreenValue = RealValue_to_ScreenValue( SCREEN_500000,SCREEN_400000,data_500000_zy_9,data_400000_zy_9,real );
                                                    }
                                                    else
                                                      if( ( real > data_500000_zy_9 )&&( real <= data_550000_zy_9 ) )//  500.000mv--550.000mv
                                                      {
                                                        ScreenValue = RealValue_to_ScreenValue( SCREEN_550000,SCREEN_500000,data_550000_zy_9,data_500000_zy_9,real );
                                                      }
  //
  return ScreenValue ;
}

/*******************************************************************************
函数名称： signed long RealValue_to_ScreenValue( long ScreenValueBig,long ScreenValueSmall,long RealValueBig,long RealValueSmall ,long RealValueMiddle )
函数功能： 利用 AD7711 计算得到的真实值，进而转化得到修正值（屏显值）
说    明： 1、函数返回值 Result ，是一个介于 ScreenValueBig 与 ScreenValueSmall 之间的值；
           2、ScreenValueBig --- 函数返回值 Result 最近的左右两个端点中，值最大者；
           3、ScreenValueSmall-- 函数返回值 Result 最近的左右两个端点中，值最小者；
           4、RealValueBig   --- 屏显值ScreenValueBig 对应的真实值；
           5、RealValueSmall --- 屏显值 ScreenValueSmall 对应的真实值；
           6、RealValueMiddle--- 真实值，介于 RealValueBig 与 RealValueSmall 之间。
创建时间： 2010-07-11
修改时间： 2010-07-11
*******************************************************************************/
signed long RealValue_to_ScreenValue( long ScreenValueBig,long ScreenValueSmall,long RealValueBig,long RealValueSmall,long RealValueMiddle )
{
  signed long Result = 0 ;
  Result = ScreenValueBig - ( signed long )( (double)( ( (double)(ScreenValueBig - ScreenValueSmall) )*( (double)(RealValueBig - RealValueMiddle) ) )/( (double)(RealValueBig - RealValueSmall) ) );
  return Result ;
}