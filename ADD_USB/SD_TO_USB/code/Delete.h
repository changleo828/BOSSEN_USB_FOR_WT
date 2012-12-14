



//////////////////////////////////// 函数声明 /////////////////////////////////////
unsigned int DeleteOneRecord(void);
void DeleteControl(void);
////////////////////////////////////// END ////////////////////////////////////////
#if 1
/*********************************************************************************
函数名称：void DeleteOneRecord(void)
函数功能：删除一条记录
创建时间：2010-05-17
说    明：1、返回删除的记录条数，第几条记录
*********************************************************************************/
unsigned int DeleteOneRecord(void)
{
    int i;
    char str[SaveLength+1];
    
    UINT32 RecordAddr;

    str[SaveLength] = '\0';
    
    if( ( CH376FileOpenPath( FileName )!= USB_INT_SUCCESS )||( CH376GetFileSize() <= strlen(Heading) ) ){
        //打开文件失败，或者文件中所有记录都删除
        NOTOpenFile();
        return FatBuffer.RecordNumber;
    }
    
    RecordAddr = CH376GetFileSize() - FatBuffer.RecordNumber * (SaveLength-1);

    for( i = 0;i < SaveLength ;i++){
        str[i] = ' ';
    }
    str[SaveLength - 3] = '\r';
    str[SaveLength - 2] = '\n';
    str[SaveLength - 1] = '\0';
    
    //Locate tail
    CH376ByteLocate(RecordAddr);
    CH376ByteWrite( str, strlen(str), NULL );  /* 以字节为单位向当前位置写入数据块 */

    CH376FileClose( TRUE );
    
    FatBuffer.RecordNumber --;
    return FatBuffer.RecordNumber;
}
#endif
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