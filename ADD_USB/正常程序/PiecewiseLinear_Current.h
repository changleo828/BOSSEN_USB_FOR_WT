//*********************************************   函数声明   **********************************************
signed long ScreenValueForCurrent( void );
void CurrentFilter(void);
//*************************************************** END *************************************************

#define   Cur_SCREEN_0       0//电流表读数
#define   Cur_SCREEN_10       10000 //10.000mA
#define   Cur_SCREEN_20       20000
#define   Cur_SCREEN_30       30000
#define   Cur_SCREEN_40       40000
#define   Cur_SCREEN_50       50000
#define   Cur_SCREEN_60       60000
#define   Cur_SCREEN_70       70000 //70.000mA
#define   Cur_SCREEN_80       80000
#define   Cur_SCREEN_90       90000
#define   Cur_SCREEN_100      100000
#define   Cur_SCREEN_200      200000  //200.000mA

signed long ScreenValueForCurrent( void )
{
  signed long ScreenValue = 0;
  if( current<=data_current_000 )//0mA
  {
    ScreenValue = 0;
  }
  else
    if( (current>data_current_000)&&(current<=data_current_010) )
    {
      ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_10,Cur_SCREEN_0,data_current_010,data_current_000,current );
    }
    else
      if( (current>data_current_010)&&(current<=data_current_020) )
      {
        ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_20,Cur_SCREEN_10,data_current_020,data_current_010,current );
      }
      else
        if( (current>data_current_020)&&(current<=data_current_030) )
        {
          ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_30,Cur_SCREEN_20,data_current_030,data_current_020,current );
        }
        else
          if( (current>data_current_030)&&(current<=data_current_040) )
          {
            ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_40,Cur_SCREEN_30,data_current_040,data_current_030,current );
          }
          else
            if( (current>data_current_040)&&(current<=data_current_050) )
            {
              ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_50,Cur_SCREEN_40,data_current_050,data_current_040,current );
            }
            else
              if( (current>data_current_050)&&(current<=data_current_060) )
              {
                ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_60,Cur_SCREEN_50,data_current_060,data_current_050,current );
              }
              else
                if( (current>data_current_060)&&(current<=data_current_070) )
                {
                  ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_70,Cur_SCREEN_60,data_current_070,data_current_060,current );
                }
                else
                  if( (current>data_current_070)&&(current<=data_current_080) )
                  {
                    ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_80,Cur_SCREEN_70,data_current_080,data_current_070,current );
                  }
                  else
                    if( (current>data_current_080)&&(current<=data_current_090) )
                    {
                      ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_90,Cur_SCREEN_80,data_current_090,data_current_080,current );
                    }
                    else
                      if( (current>data_current_090)&&(current<=data_current_100) )
                      {
                         ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_100,Cur_SCREEN_90,data_current_100,data_current_090,current );
                      }
                      else
                        if( (current>data_current_100)&&(current<=data_current_200) )
                        {
                          ScreenValue = RealValue_to_ScreenValue( Cur_SCREEN_200,Cur_SCREEN_100,data_current_200,data_current_100,current );
                        }
                        else
                          ScreenValue = Cur_SCREEN_200;
  return ScreenValue ;
}

void CurrentFilter(void)
{
  iXiuZhengCount++; 
  sum_XiuZheng  =  sum_XiuZheng + ScreenValueForCurrent(  );
  if( iXiuZhengCount >= XiuZhengCount )
  {
      iXiuZhengCount  = 0;
      XiuZheng_Result = sum_XiuZheng/XiuZhengCount;
      sum_XiuZheng = 0;
  }
}
