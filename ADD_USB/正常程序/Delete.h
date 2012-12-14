



//////////////////////////////////// 函数声明 /////////////////////////////////////
unsigned int DeleteOneRecord(void);
void DeleteControl(void);
////////////////////////////////////// END ////////////////////////////////////////

/*********************************************************************************
函数名称：void DeleteOneRecord(void)
函数功能：删除一条记录
创建时间：2010-05-17
说    明：1、返回删除的记录条数，第几条记录
*********************************************************************************/
unsigned int DeleteOneRecord(void)
{  
  uint  offset_start = FatBuffer.OffsetInBuffer_Start_RecordNumber;//0--511,数组 FAT32_Buffer 的下标
  //uint  offset_end = FatBuffer.OffsetInBuffer_RecordNumber;
  //uchar offsetCount_FromZero = 0;
  //首先判断要删除的那条记录是否处于两个扇区里
  if( FatBuffer.TheFirstPartForDemand_Sector==FatBuffer.TheSecondPartForDemand_Sector )//一个扇区里
  {
      FAT32_ReadSector((((FatBuffer.TheFirstPartForDemand_Cluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector + FatBuffer.TheFirstPartForDemand_Sector,FAT32_Buffer);//
      for( ;offset_start<FatBuffer.OffsetInBuffer_RecordNumber;offset_start++ )
        FAT32_Buffer[offset_start] = Space;//注：FAT32_Buffer[FatBuffer.OffsetInBuffer_RecordNumber]里的换行符保存
      ///将新内容重新写入扇区
      FAT32_WriteSector( (((FatBuffer.TheFirstPartForDemand_Cluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector + FatBuffer.TheFirstPartForDemand_Sector, FAT32_Buffer );
  }
  else//两个扇区里
  {
      //首先对记录的前半部分操作
      FAT32_ReadSector((((FatBuffer.TheFirstPartForDemand_Cluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector + FatBuffer.TheFirstPartForDemand_Sector,FAT32_Buffer);//
      for( ;offset_start<Init_Arg.BytesPerSector; offset_start++ )
        FAT32_Buffer[offset_start] = Space;
      FAT32_WriteSector( (((FatBuffer.TheFirstPartForDemand_Cluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector + FatBuffer.TheFirstPartForDemand_Sector, FAT32_Buffer );
      //对记录的 后半 部分操作
      FAT32_ReadSector((((FatBuffer.TheSecondPartForDemand_Cluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector + FatBuffer.TheSecondPartForDemand_Sector,FAT32_Buffer);//
      for( offset_start=0;offset_start<FatBuffer.OffsetInBuffer_RecordNumber;offset_start++ )
        FAT32_Buffer[offset_start] = Space;
      FAT32_WriteSector( (((FatBuffer.TheSecondPartForDemand_Cluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector + FatBuffer.TheSecondPartForDemand_Sector, FAT32_Buffer );
  }
  return FatBuffer.RecordNumber;
}

/*********************************************************************************
函数名称：void DeleteControl(void)
函数功能：
创建时间：2010-05-17
说    明：1、按压“删除”后，屏幕点亮“删除”；-----在查询的显示里实现
          2、屏幕点亮“删除”时，要显示操作员正在查询的记录信息；
          3、屏幕“删除”灭掉后，就显示 FatBuffer.RecordNumber - 1 的记录
*********************************************************************************/
void DeleteControl(void)
{
  if( Delete_flag == 1 )//刚按压了删除键
  {     
      DemandControl();//删除是在查询的基础上的操作，所以，一定要保证查询先运行
      DeleteOneRecord();
      Delete_flag = 2;
  }
}