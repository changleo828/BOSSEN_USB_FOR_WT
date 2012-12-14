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
void MemoryInit(void);
////////////////////////////////////// END ////////////////////////////////////////

/*********************************************************************************
函数名称：void SaveControl(void)
函数功能：存储控制
创建时间：2010-05-02
说    明：1、Save_Flag 是在定时器 TA0 里清零
*********************************************************************************/
char FileName[9]={0,0,0,0,0,0X2E,0,0,0}; 
void SaveControl(void)
{  
  //if( (Save_Flag==1)&&(SaveTimeCount<5) )//由于 Save_Flag 是在定时器 TA0 里清零的，所以，在这里使用 &&(SaveTimeCount<5)，是为了防止被多次写入  
  if( (Save_Flag==1) )
  {          
    ZhuangHaoDianHao();
    _DINT();
    //(FAT32_OpenFile("\\TEST.TXT")) ; 
    /*
    memset( FileName,0,9 );    
    FileName[0] = 0x5C;
    FileName[1] = 0x5C;  
    FileName[2] = Position_data[4]+Parameter;
    FileName[3] = Position_data[5]+Parameter;
    FileName[4] = Position_data[6]+Parameter;
    FileName[5] = 0X2E;
    FileName[6] = 68;
    FileName[7] = 65;
    FileName[8] = 84;  
    */
    /*
    memset( FileName,0,7 );    
    FileName[0] = 0x5C;
    FileName[1] = 0x5C;  
    FileName[2] = Position_data[0]+Parameter;     
    FileName[3] = 0X2E;
    FileName[4] = 68;
    FileName[5] = 65;
    FileName[6] = 84;  
    */
    WriteANameForANewFile();
    FAT32_OpenFile(FileName) ;  
    FAT32_WriteFile( &FileInfo,SaveBuffer, SaveLength );
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
void MemoryInit(void)
{
  MMC_Init();
  MMC_get_volume_info();
  FAT32_Init(&Init_Arg);
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
  //uint second,hour,day,month,year;
  //uchar abc[7];
  
  SaveBuffer[0] = Position_data[1] + Parameter; //  缓存点号
  SaveBuffer[1] = Position_data[2] + Parameter;
  SaveBuffer[2] = Position_data[3] + Parameter;
  SaveBuffer[3] = ',';
  SaveBuffer[4] = Position_data[4] + Parameter; //  缓存 桩号
  SaveBuffer[5] = Position_data[5] + Parameter;
  SaveBuffer[6] = Position_data[6] + Parameter;
  SaveBuffer[7] = ','; 
  
  for( i=0;i<7;i++ )  // 缓存电压
  {
    SaveBuffer[8+i] = mmmm[6-i] + Parameter ;
  }
  SaveBuffer[15] = ','; 
  
  for( i=0;i<7;i++ )  //  缓存 电流
  {
    SaveBuffer[16+i] = mmma[6-i] + Parameter ;
  }
  /*
  SaveBuffer[23] = ','; 
  //////////////////////////////////////////
  R1302T_String( abc );//
  day = ((abc[3]&0xF0)>>4)*10 + (abc[3]&0x0F);
  month = ((abc[4]&0xF0)>>4)*10 + (abc[4]&0x0F);
  year = ((abc[6]&0xF0)>>4)*10 + (abc[6]&0x0F); 
  second=((abc[1]&0xF0)>>4)*10 + (abc[1]&0x0F); 
  hour=((abc[2]&0xF0)>>4)*10 + (abc[2]&0x0F);   
  day = 27;
  month = 02;
  year = 10;
  second = 23;
  hour = 12;
  SaveBuffer[24] = year/10 + Parameter; 
  SaveBuffer[25] = year%10 + Parameter;
  SaveBuffer[26] = '-';
  SaveBuffer[27] = month/10 + Parameter; 
  SaveBuffer[28] = month%10 + Parameter; 
  SaveBuffer[29] = '-';
  SaveBuffer[30] = day/10 + Parameter; 
  SaveBuffer[31] = day%10 + Parameter; 
  SaveBuffer[32] = ','; 
  SaveBuffer[33] = hour/10 + Parameter; 
  SaveBuffer[34] = hour%10 + Parameter; 
  SaveBuffer[35] = ':'; 
  SaveBuffer[36] = second/10 + Parameter; 
  SaveBuffer[37] = second%10 + Parameter; 
  //////////////////////////////////////
  //SaveBuffer[24] = '\n'; // 回车
  SaveBuffer[38] = '\r'; // 换行
  */
  SaveBuffer[23] = '\r'; // 换行
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
}

#endif
