/*============================================ 头文件说明 ============================================================
头文件名称：
头文件用途：
================================================ END ===============================================================*/

#ifndef __MemoryManagement_H
#define __MemoryManagement_H

//////////////////////////////////// 函数声明 /////////////////////////////////////
void WriteANameForANewFile(void);
void ZhuangHaoDianHao(void);
void ScreenControlForMemory(void);
void SaveControl(void);
uchar MemoryInit(void);
////////////////////////////////////// END ////////////////////////////////////////
#define _EINT  __enable_interrupt
#define _DINT  __disable_interrupt
/*********************************************************************************
函数名称：void SaveControl(void)
函数功能：存储控制
创建时间：2010-05-02
说    明：1、Save_Flag 是在定时器 TA0 里清零
*********************************************************************************/
char FileName[10]={0,0,0,0,0,0X2E,0,0,0}; 
uchar USB_Disk_Connect_Flag;
void SaveControl(void)
{  
    uchar s;

  //if( (Save_Flag==1)&&(SaveTimeCount<5) )//由于 Save_Flag 是在定时器 TA0 里清零的，所以，在这里使用 &&(SaveTimeCount<5)，是为了防止被多次写入  
  if( (Save_Flag==1) )
  {          
    if( (USB_Disk_Connect_Flag == 0) || (CH376DiskConnect() == ERR_DISK_DISCON) ){
        if(MemoryInit() == 0){
            Save_Flag=0;
            return ;
        }
    }
    _DINT();

    WriteANameForANewFile();
    
    s = CH376FileOpenPath( FileName );
   
    if ( s == ERR_MISS_FILE ){
        s = CH376FileCreatePath( FileName );  /* 新建多级目录下的文件,支持多级目录路径,输入缓冲区必须在RAM中 */
        //mStopIfError( s );
        s = CH376ByteLocate(0xFFFFFFFF);
        //mStopIfError( s );
        HeadingWrite();
        s = CH376ByteWrite( Heading, strlen(Heading), NULL );  /* 以字节为单位向当前位置写入数据块 */
        //mStopIfError( s );
        
    }else{
        s = CH376ByteLocate(0xFFFFFFFF);
        //mStopIfError( s );
    }
    ZhuangHaoDianHao();
    s = CH376ByteWrite( SaveBuffer, strlen(SaveBuffer), NULL );  /* 以字节为单位向当前位置写入数据块 */
    //mStopIfError( s );
    s = CH376FileClose( TRUE );  /* 关闭文件,自动计算文件长度,以字节为单位写文件,建议让程序库关闭文件以便自动更新文件长度 */
    //mStopIfError( s );
    
    //FAT32_WriteFile( &FileInfo,SaveBuffer, SaveLength );
    ZhuangHaoAutomaticallyIncreases( );
    
    _EINT();
    Save_Flag = 2;
    
  }
}

/*********************************************************************************
函数名称：void ScreenControlForMemory(void)
函数功能：存储的屏显控制
创建时间：2010-05-02
说    明：
*********************************************************************************/
void ScreenControlForMemory(void)
{
  
}

/*********************************************************************************
函数名称：void MemoryInit(void)
函数功能：存储器初始化
创建时间：2010-05-02
说    明：
*********************************************************************************/
uchar MemoryInit(void)
{
    int i;
    uchar res;

    res = mInitCH376Host();
    if(res != USB_INT_SUCCESS){
        for(i = 0;i<30000;i++);
            res = mInitCH376Host();
        if(res != USB_INT_SUCCESS)
            goto MemoryInitRes;
    }
/* 查询U盘是否连接，返回USB_INT_SUCCESS则说明当前已连接 */ 
    res = CH376DiskConnect();
    if(res != USB_INT_SUCCESS){
        for(i = 0;i<30000;i++);
            res = CH376DiskConnect();
        if(res != USB_INT_SUCCESS)
            goto MemoryInitRes;
    }

/* 查询U盘或SD卡是否准备好，有些U盘可能需多次调用才能成功 */  
    res = CH376DiskMount();
    if(res != USB_INT_SUCCESS){
        for(i = 0;i<30000;i++);
            res = CH376DiskMount();
        if(res != USB_INT_SUCCESS)
            goto MemoryInitRes;
    }
    USB_Disk_Connect_Flag = 1;
    return USB_Disk_Connect_Flag;
MemoryInitRes:
    USB_Disk_Connect_Flag = 0;
    return USB_Disk_Connect_Flag;
}

/*********************************************************************************
函数名称：void ZhuangHaoDianHao(void)
函数功能：
创建时间：2010-05-02
说    明：
*********************************************************************************/
void ZhuangHaoDianHao(void)
{
    uchar i=0;

    SaveBuffer[0] = Position_data[1] + Parameter; //  缓存点号
    SaveBuffer[1] = Position_data[2] + Parameter;
    SaveBuffer[2] = Position_data[3] + Parameter;
    SaveBuffer[3] = ',';
    SaveBuffer[4] = Position_data[4] + Parameter; //  缓存 桩号
    SaveBuffer[5] = Position_data[5] + Parameter;
    SaveBuffer[6] = Position_data[6] + Parameter;
    SaveBuffer[7] = ','; 

    for( i=0;i<7;i++ ){  // 缓存电压
        SaveBuffer[8+i] = mmmm[6-i] + Parameter ;
    }
    SaveBuffer[15] = ','; 

    for( i=0;i<7;i++ ){  //  缓存 电流
    SaveBuffer[16+i] = mmma[6-i] + Parameter ;
    }

    SaveBuffer[23] = '\r'; // 换行
    SaveBuffer[24] = '\n'; // 换行
    SaveBuffer[25] = '\0';
}

/*********************************************************************************
函数名称：void WriteANameForANewFile(void)
函数功能：
创建时间：2010-05-02
说    明：
*********************************************************************************/
void WriteANameForANewFile(void)
{
    FileName[0] = 0x5C;                       
    FileName[1] = 0x5C;                       
    FileName[2] = Position_data[0] + Parameter; //巷道号  
    FileName[3] = 0x5F;                       //短下划线
    FileName[4] = FeatureSelection_data + Parameter;//功能选择
    FileName[5] = 0X2E;                       //.
    FileName[6] = 68;
    FileName[7] = 65;
    FileName[8] = 84;
    FileName[9] = '\0';
}

#endif
