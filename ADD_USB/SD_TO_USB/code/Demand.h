
/*
struct BufferFlag
{
  unsigned int RecordNumber;//"查询"屏幕上所对对应的第几条记录
  unsigned int OffsetInSectorOfTheFile;
};
struct BufferFlag FatBuffer;
*/
//////////////////////////////////// 函数声明 /////////////////////////////////////
uchar BackOrFore_Demand(void);
void NOTOpenFile(void);//打开文件失败
void Position_Demand(void);//点号---查询时候显示
void Vol_Demand(void);//电压---查询时候显示
void Current_Demand(void);////电流---查询时候使用

void DemandControl(void);
////////////////////////////////////// END ////////////////////////////////////////


void ReadTheRecord(void)
{
    int i;
    uchar str[DemandLength];
    UINT16     RealCount;
    UINT32 RecordAddr;
    
    
    
    RecordAddr = CH376GetFileSize() - FatBuffer.RecordNumber * (SaveLength-1);
        //Locate tail
    CH376ByteLocate(RecordAddr);
    
    CH376ByteRead( str, DemandLength, &RealCount );
    HeadingWrite();
    if( (str[DemandLength-3] == Heading[HeadingLength - 3])&&
        (str[DemandLength-5] == Heading[HeadingLength - 5])&&
        (str[DemandLength-7] == Heading[HeadingLength - 7]))
    {
        FatBuffer.RecordNumber = 1;
        FatBuffer.LastRecordNumber = 1;
        RecordAddr = CH376GetFileSize() - (SaveLength-1);
        CH376ByteLocate(RecordAddr);
        CH376ByteRead( str, DemandLength, &RealCount );
    }
    
    for( i = 0;i < DemandLength;i ++){
        FatBuffer.Demand [i] = str[i];
    }

}
void Position_Demand(void)//点号---查询时候显示
{
  Ht1621WrOneData( 10,0 );	//点亮“采集”图标--关闭
  Ht1621WrOneData( 13,0 );//单位：mV---关掉 
  
  ///////////首先显示--巷道号---这是本次查询的定值
  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ Position_data[0] ][0] );
  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ Position_data[0] ][1] );//
  
  /////////////点号
  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ FatBuffer.Demand[0]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ FatBuffer.Demand[0]-Parameter ][1] );//
  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ FatBuffer.Demand[1]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ FatBuffer.Demand[1]-Parameter ][1] );//
  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ FatBuffer.Demand[2]-Parameter ][0] + 8 ); // 点亮“点号”
  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ FatBuffer.Demand[2]-Parameter ][1] );//
    
  //////////////////桩号
    Ht1621WrOneData( disp_data_addr[4][0],disp_data[ FatBuffer.Demand[4]-Parameter ][0] );
    Ht1621WrOneData( disp_data_addr[4][1],disp_data[ FatBuffer.Demand[4]-Parameter ][1] );//
    if( Delete_flag==2 )
    {
      Ht1621WrOneData( disp_data_addr[5][0],disp_data[ FatBuffer.Demand[5]-Parameter ][0] +8);
      Ht1621WrOneData( disp_data_addr[5][1],disp_data[ FatBuffer.Demand[5]-Parameter ][1] );//
    }
    else
    {
      Ht1621WrOneData( disp_data_addr[5][0],disp_data[ FatBuffer.Demand[5]-Parameter ][0] );
      Ht1621WrOneData( disp_data_addr[5][1],disp_data[ FatBuffer.Demand[5]-Parameter ][1] );
    }
    Ht1621WrOneData( disp_data_addr[6][0],disp_data[ FatBuffer.Demand[6]-Parameter ][0] +8);////点亮“显示”
    Ht1621WrOneData( disp_data_addr[6][1],disp_data[ FatBuffer.Demand[6]-Parameter ][1] );//
    
    Ht1621WrOneData( 12,1+0 );	//小数点+  ma
}
void Vol_Demand(void)//电压---查询时候显示
{
  Ht1621WrOneData( 10,0 );	//点亮“采集”图标--关闭
  Ht1621WrOneData( 13,1 );//单位：mV
  
  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ FatBuffer.Demand[8]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ FatBuffer.Demand[8]-Parameter ][1] );//
  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ FatBuffer.Demand[9]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ FatBuffer.Demand[9]-Parameter ][1] );//
  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ FatBuffer.Demand[10]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ FatBuffer.Demand[10]-Parameter ][1] );//
  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ FatBuffer.Demand[11]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ FatBuffer.Demand[11]-Parameter ][1] );//
  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ FatBuffer.Demand[12]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ FatBuffer.Demand[12]-Parameter ][1] );//
  if( Delete_flag==2 )
  {
      Ht1621WrOneData( disp_data_addr[5][0],disp_data[ FatBuffer.Demand[13]-Parameter ][0] +8);
      Ht1621WrOneData( disp_data_addr[5][1],disp_data[ FatBuffer.Demand[13]-Parameter ][1] );//
  }
  else
  {
      Ht1621WrOneData( disp_data_addr[5][0],disp_data[ FatBuffer.Demand[13]-Parameter ][0] );
      Ht1621WrOneData( disp_data_addr[5][1],disp_data[ FatBuffer.Demand[13]-Parameter ][1] );//
  }
  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ FatBuffer.Demand[14]-Parameter ][0] +8);////点亮“显示”
  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ FatBuffer.Demand[14]-Parameter ][1] );//
    
  Ht1621WrOneData( 12,1+0 );	//小数点+  ma
}
void Current_Demand(void)////电流---查询时候使用
{
  Ht1621WrOneData( 10,0 );	//点亮“采集”图标--关闭
  Ht1621WrOneData( 13,0 );//单位：mV--关闭
  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ FatBuffer.Demand[16]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ FatBuffer.Demand[16]-Parameter ][1] );//
  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ FatBuffer.Demand[17]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ FatBuffer.Demand[17]-Parameter ][1] );//
  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ FatBuffer.Demand[18]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ FatBuffer.Demand[18]-Parameter ][1] );//
  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ FatBuffer.Demand[19]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ FatBuffer.Demand[19]-Parameter ][1] );//
  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ FatBuffer.Demand[20]-Parameter ][0] );
  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ FatBuffer.Demand[20]-Parameter ][1] );//
  if( Delete_flag==2 )
  {
    Ht1621WrOneData( disp_data_addr[5][0],disp_data[ FatBuffer.Demand[21]-Parameter ][0] +8);///点亮“删除”
    Ht1621WrOneData( disp_data_addr[5][1],disp_data[ FatBuffer.Demand[21]-Parameter ][1] );//
  }
  else
  {
    Ht1621WrOneData( disp_data_addr[5][0],disp_data[ FatBuffer.Demand[21]-Parameter ][0] );
    Ht1621WrOneData( disp_data_addr[5][1],disp_data[ FatBuffer.Demand[21]-Parameter ][1] );//
  }
  Ht1621WrOneData( disp_data_addr[6][0],disp_data[ FatBuffer.Demand[22]-Parameter ][0] +8);////点亮“显示”
  Ht1621WrOneData( disp_data_addr[6][1],disp_data[ FatBuffer.Demand[22]-Parameter ][1] );//
    
  Ht1621WrOneData( 12,1+2 );	//小数点+  ma
}
#if 1
/*********************************************************************************
函数名称：void DemandControl(void)
函数功能：按键“查询”控制
创建时间：2010-05-17
说    明：
*********************************************************************************/
void DemandControl(void)
{
    unsigned int i=0;
    uchar del=0;
    
    if( FatBuffer.RecordNumber == 0)//不在查询状况下
        return ;
    
    //在查询状况下
    //printk("在查询状况下\r\n");
    WriteANameForANewFile();//这里不是给新文件起名字，只是利用此函数给数组 FileName 赋值
    //USB 连接失败或者已拔出，尝试连接
    if( (USB_Disk_Connect_Flag == 0) || (CH376DiskConnect() == ERR_DISK_DISCON) ){
        if(MemoryInit() == 0){
            return ;
        }
    }

    if( ( CH376FileOpenPath( FileName )!= USB_INT_SUCCESS )||( CH376GetFileSize() <= strlen(Heading) ) ){
        //打开文件失败，或者文件中所有记录都删除
        NOTOpenFile();
        CH376FileClose( TRUE );
        return ;
    }
    
    //打开文件成功，并且文件中所有记录没有都删除       
    if( Delete_flag==0 )//如果没有按下删除键
    {
        //for(i=0;( i<((unsigned long)(FileInfo.FileSize)/(unsigned long)(DemandLength)) )&&(del==0);i++)
        //{   //有个跳出循环的机会，因为有可能都是删除的记录
            for( i=0,
                 FatBuffer.Demand[0]=Space,
                 FatBuffer.Demand[1]=Space,
                 FatBuffer.Demand[2]=Space;
                     (FatBuffer.Demand[0]==Space)&&
                     (FatBuffer.Demand[1]==Space)&&
                     (FatBuffer.Demand[2]==Space)&&
                     ( i<(CH376GetFileSize()/(unsigned long)(DemandLength)) )&&
                     (del==0);
                         i++ )
            {//加入这层循环，是为检查是否读到了删除的记录
                ReadTheRecord( );
                if( (FatBuffer.Demand[0]==Space)&&
                    (FatBuffer.Demand[1]==Space)&&
                    (FatBuffer.Demand[2]==Space) ){//读到了删除的记录
                   if( BackOrFore_Demand()==2 ){//按压了 --设置减
                        FatBuffer.LastRecordNumber = FatBuffer.RecordNumber ;
                        FatBuffer.RecordNumber --;
                        if( FatBuffer.RecordNumber==0 )
                           FatBuffer.RecordNumber = 1;//暂时设为1，本应为最前面的那条记录                        
                   }else{//按压了 设置加 ，或者是刚按压了 查询
                        FatBuffer.LastRecordNumber = FatBuffer.RecordNumber ;
                        FatBuffer.RecordNumber ++;
                   }
                }else{////表明读取成功，没有读到 删除的记录
                  del=1;//及早跳出循环，否则显示上有延误
                }
            }// end for
        //}        
    }   
    if( i<(CH376GetFileSize()/(unsigned long)(DemandLength)) )  //此条件事用来判别记录是否被全部删除
    {   
      Ht1621WrOneData( 0,8 );//点亮公司的标识  
      Ht1621WrOneData( 9,8 ); //点亮公司的标识  
      Ht1621WrOneData( 11,4 );		//点亮公司的标识  
      if( WorkOnTime<=Basic_Time )   Position_Demand();//显示点号                   
      if( (WorkOnTime>Basic_Time)&&(WorkOnTime<=Basic_Time*2) )  Current_Demand();//显示电流                  
      if( (WorkOnTime>Basic_Time*2)&&(WorkOnTime<=Basic_Time*3) )  Vol_Demand();//显示电压，24位AD   
    }
    else  //有效记录全部删除
    {
      FatBuffer.NoRecord_flag_InFile=1;//NOTOpenFile();
    }
    CH376FileClose( TRUE );
}
#endif
void NOTOpenFile(void)//打开文件失败
{
  //屏幕上“显示”图标闪动，七个数字处于静态的 0
  Ht1621WrOneData( 0,8 );//点亮公司的标识  
  Ht1621WrOneData( 9,8 ); //点亮公司的标识  
  Ht1621WrOneData( 11,4 );//点亮公司的标识  
  Ht1621WrOneData( disp_data_addr[0][0],disp_data[ 0 ][0] );//
  Ht1621WrOneData( disp_data_addr[0][1],disp_data[ 0 ][1] );//
  Ht1621WrOneData( disp_data_addr[1][0],disp_data[ 0 ][0] );//
  Ht1621WrOneData( disp_data_addr[1][1],disp_data[ 0 ][1] );//
  Ht1621WrOneData( disp_data_addr[2][0],disp_data[ 0 ][0] );//
  Ht1621WrOneData( disp_data_addr[2][1],disp_data[ 0 ][1] );//
  Ht1621WrOneData( disp_data_addr[3][0],disp_data[ 0 ][0] );//
  Ht1621WrOneData( disp_data_addr[3][1],disp_data[ 0 ][1] );//
  Ht1621WrOneData( disp_data_addr[4][0],disp_data[ 0 ][0] );//
  Ht1621WrOneData( disp_data_addr[4][1],disp_data[ 0 ][1] );//
  Ht1621WrOneData( disp_data_addr[5][0],disp_data[ 0 ][0] );//
  Ht1621WrOneData( disp_data_addr[5][1],disp_data[ 0 ][1] );//
  if( second%2==0 )
  {
      Ht1621WrOneData( disp_data_addr[6][0],disp_data[ 0 ][0] +8);////点亮“显示”
      Ht1621WrOneData( disp_data_addr[6][1],disp_data[ 0 ][1] );//
  }
  else
  {
    Ht1621WrOneData( disp_data_addr[6][0],disp_data[ 0 ][0] );////“显示”---关闭
    Ht1621WrOneData( disp_data_addr[6][1],disp_data[ 0 ][1] );//
  }
}

/*-------------------------------------------------------------------------------
函数名称：
函数功能：从最近一次按压“设置加”或者“设置减”来判断判断操作员的查询趋势：是向前还是向后
说    明：1、 返回参数 ： 1--最近一次按压了“设置加”
                      2--最近一次按压了“设置减”
                      0--文件中只有一条记录、
          2、如果是第一次按压查询按键，并且没有按压“设置加”或者“设置减”，也会被认为是0
-------------------------------------------------------------------------------*/
uchar BackOrFore_Demand(void)
{
  uchar flag=0;
  if( FatBuffer.LastRecordNumber > FatBuffer.RecordNumber )
  {
      if( FatBuffer.LastRecordNumber-FatBuffer.RecordNumber==1 )
        flag=2;
      else
        flag=1;
  }
  if( FatBuffer.LastRecordNumber < FatBuffer.RecordNumber )
  {
      if( FatBuffer.RecordNumber-FatBuffer.LastRecordNumber == 1 )
        flag=1;
      else
        flag=2;
  }
  if( FatBuffer.LastRecordNumber == FatBuffer.RecordNumber )
  {
      flag=0;
  }
  return flag;
}