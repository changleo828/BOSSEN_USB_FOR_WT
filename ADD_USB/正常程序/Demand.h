
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
unsigned int CalculateTheOffsetInSectorOfTheFile( struct FileInfoStruct *pstru,unsigned long Cluster,unsigned char iSectorInCluster  );
unsigned char OpenFile( char *filepath );
void ReadTheRecord(void);
void DemandControl(void);
////////////////////////////////////// END ////////////////////////////////////////

/*********************************************************************************
函数名称：void ReadTheRecord(void)
函数功能：读取 SD 的最后几条记录，最大容量为512个字节
创建时间：2010-05-02
说    明：
*********************************************************************************/
void ReadTheRecord(void)
{
  uchar ri=0;
  int RestOfBytes = 0;
  unsigned long BackToFore_Size=0;
  unsigned long iCluster=0;
  int  iSector=0;
  uint  TheLastSectorInFile = 0;  //  文件的最后一个扇区
  uint  OffsetInSectorOfTheFile = 0;  //  文件中某个扇区的字节数
  WriteANameForANewFile();//这里不是给新文件起名字，只是利用此函数给数组 FileName 赋值
  
  if( OpenFile(FileName)==1 ) //  打开文件成功
  {
    TheLastSectorInFile = FAT32_ReadFileTheLastCluster( &FileInfo );
    FatBuffer.TheLastSectorInFile = TheLastSectorInFile ;
    FatBuffer.TheLastClusterInFile = FileInfo.FileCurCluster;//在函数FAT32_ReadFileTheLastCluster( &FileInfo );中为  FileInfo.FileCurCluster赋值
    OffsetInSectorOfTheFile = CalculateTheOffsetInSectorOfTheFile( &FileInfo,FileInfo.FileCurCluster,TheLastSectorInFile  );
    FAT32_ReadSector((((FileInfo.FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+TheLastSectorInFile,FAT32_Buffer);//
    
    //FatBuffer.RecordNumber = 3;////////////////暂时的
    FatBuffer.RecordNumber_Bytes = FatBuffer.RecordNumber * DemandLength ;
    OffsetInSectorOfTheFile = 0;
    iCluster = FileInfo.FileCurCluster;//文件的最后一个簇
    RestOfBytes = FatBuffer.RecordNumber_Bytes - BackToFore_Size;
    for( ;FatBuffer.RecordNumber_Bytes>BackToFore_Size;BackToFore_Size+=OffsetInSectorOfTheFile )//第一次出现 <=BackToFore_Size 就读出扇区内容，并且只读一次
    {
        if(ri==0)//第一次读取最后一个扇区
        { //只有最后一个扇区可能不是512个字节，其他的都是
          OffsetInSectorOfTheFile = CalculateTheOffsetInSectorOfTheFile( &FileInfo,FileInfo.FileCurCluster,TheLastSectorInFile  );
        }
        else
        {
          OffsetInSectorOfTheFile = 512;
        }
        FatBuffer.BytesAfterTheSector = BackToFore_Size;/////注：
        iSector = TheLastSectorInFile-ri;
        ri++;
        if( iSector<0 )
        {
          iSector = 7;
          TheLastSectorInFile =7;
          ri=1;
          iCluster--;
        }
        if( RestOfBytes>=DemandLength )//检查是否可能发生“读取部分记录”
          RestOfBytes = FatBuffer.RecordNumber_Bytes - BackToFore_Size;
    }//接下来，就是读取记录数据所在的扇区----可能读的是 部分的记录数据
    FAT32_ReadSector((((iCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector + iSector,FAT32_Buffer);//
    /////将读取的记录（可能读的是 部分的记录数据）存入 Demand[ DemandLength ];
    OffsetInSectorOfTheFile = CalculateTheOffsetInSectorOfTheFile( &FileInfo,iCluster,iSector  );
    ri=0;
    FatBuffer.OffsetInBuffer_RecordNumber = OffsetInSectorOfTheFile-FatBuffer.RecordNumber*DemandLength+ri;
    FatBuffer.OffsetInBuffer_Start_RecordNumber = FatBuffer.OffsetInBuffer_RecordNumber;//赋值起始下标
    for( ri=0;(ri<DemandLength)&&(FatBuffer.OffsetInBuffer_RecordNumber<512);ri++ )
    {
      //if( OffsetInSectorOfTheFile-FatBuffer.RecordNumber*DemandLength+ri<512 )//防止数组下标溢出
        FatBuffer.OffsetInBuffer_RecordNumber = OffsetInSectorOfTheFile-(FatBuffer.RecordNumber*DemandLength -FatBuffer.BytesAfterTheSector)+ri;
        FatBuffer.Demand[ ri ] = FAT32_Buffer[ FatBuffer.OffsetInBuffer_RecordNumber ];
    }
    ///////至此，假设“查询”内容全部读出，我们要给此部分内容的簇号与扇区号做个记录
    FatBuffer.TheFirstPartForDemand_Sector = iSector;
    FatBuffer.TheFirstPartForDemand_Cluster = iCluster;
    FatBuffer.TheSecondPartForDemand_Sector = FatBuffer.TheFirstPartForDemand_Sector;
    FatBuffer.TheSecondPartForDemand_Cluster = FatBuffer.TheFirstPartForDemand_Cluster;
    //////////下面就检查是否存在“读的是 部分的记录数据”的问题，即数组FAT32_Buffer的下标>=512发生溢出
    //if( ri!=DemandLength )//发生溢出
    if( RestOfBytes < DemandLength )
    {
        //刚才读的扇区重新读取
        //FatBuffer.OffsetInBuffer_RecordNumber = 512 - ri ;
        //FatBuffer.OffsetInBuffer_RecordNumber = 512 - (DemandLength-RestOfBytes) ;
        FatBuffer.OffsetInBuffer_RecordNumber = 512 - (RestOfBytes) ;
        FatBuffer.OffsetInBuffer_Start_RecordNumber = FatBuffer.OffsetInBuffer_RecordNumber;//赋值起始下标
        for( ri=0;(FatBuffer.OffsetInBuffer_RecordNumber<512);ri++ )
        { 
            FatBuffer.Demand[ ri ] = FAT32_Buffer[ FatBuffer.OffsetInBuffer_RecordNumber ];
            FatBuffer.OffsetInBuffer_RecordNumber ++;
        }
        ///////将“查询”内容所在的前半部分所在的簇与扇区保存
        FatBuffer.TheFirstPartForDemand_Sector = iSector;
        FatBuffer.TheFirstPartForDemand_Cluster = iCluster;
        //没有读完的记录在下一个扇区里
        iSector ++;
        if( iSector>=8 )
        {
            iSector = 0;
            iCluster ++;
        }
        ///////将“查询”内容所在的 后半部分 所在的簇与扇区保存
        FatBuffer.TheSecondPartForDemand_Sector = iSector;
        FatBuffer.TheSecondPartForDemand_Cluster = iCluster;//////保存完毕，下面就读取“查询”内容的另一截
        FAT32_ReadSector((((iCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector + iSector,FAT32_Buffer);//
        ///将剩余记录数据放入数组 FatBuffer.Demand ，此时，一定是从数组 FAT32_Buffer 的最前面开始读取
        FatBuffer.OffsetInBuffer_RecordNumber = 0;
        for( ;ri<DemandLength;ri++ )
        {
            FatBuffer.Demand[ ri ] = FAT32_Buffer[ FatBuffer.OffsetInBuffer_RecordNumber ];
            FatBuffer.OffsetInBuffer_RecordNumber ++;//最后的值会比实际值大 1
        }          
    }
    //////检查是否读取了标题行
    if( (FatBuffer.Demand[DemandLength-1]=='\r')&&(FatBuffer.Demand[DemandLength-2]=='Z')&&(FatBuffer.Demand[DemandLength-3]=='H') )
    {//参考数组 Heading 的最后三个值，就可以知道是否读到了“标题行”
         //一旦读到 标题行 ，就表明已经读到了文件的开头处，需要返回文件结尾处重新读取。返回后，一定读到的是完整记录，不存在“一条记录在两个扇区里的情况”   
            //OffsetInSectorOfTheFile = CalculateTheOffsetInSectorOfTheFile( &FileInfo,iCluster,iSector  );
            TheLastSectorInFile = FAT32_ReadFileTheLastCluster( &FileInfo );
            OffsetInSectorOfTheFile = CalculateTheOffsetInSectorOfTheFile( &FileInfo,FileInfo.FileCurCluster,TheLastSectorInFile  );
            FatBuffer.RecordNumber = 1;//返回到 第一条 记录
            FatBuffer.RecordNumber_Bytes = FatBuffer.RecordNumber * DemandLength ;
            FatBuffer.OffsetInBuffer_RecordNumber = OffsetInSectorOfTheFile-FatBuffer.RecordNumber*DemandLength;
            FatBuffer.OffsetInBuffer_Start_RecordNumber = FatBuffer.OffsetInBuffer_RecordNumber;//赋值起始下标
            for( ri=0;(ri<DemandLength)&&(FatBuffer.OffsetInBuffer_RecordNumber<512);ri++ )
            {
                
                FatBuffer.OffsetInBuffer_RecordNumber = OffsetInSectorOfTheFile-FatBuffer.RecordNumber*DemandLength + ri;
                FatBuffer.Demand[ ri ] = FAT32_Buffer[ FatBuffer.OffsetInBuffer_RecordNumber ];
            }
    } 
    
    /////////////////////
  }  
  ri = 1;  
}

/*********************************************************************************
函数名称：unsigned char OpenFile( char *filepath )
函数功能：打开某个已存在文件文件，
创建时间：2010-05-02
说    明：
*********************************************************************************/
unsigned char OpenFile( char *filepath )
{
 unsigned char depth=0;
 unsigned char i,index;
 unsigned long iFileSec,iCurFileSec,iFile;
 unsigned long sum3=0;
 struct direntry *pFile;
 unsigned char len=strlen(filepath);
 for(i=0;i<len;i++)
 {
  if(filepath[i]=='\\')
  { 
   depth++;
   index=i+1;
  }
 } 
 //iCurFileSec=FAT32_EnterDir(filepath)/*Init_Arg.FirstDirSector*/; 
 iCurFileSec=Init_Arg.FirstDirSector; //  只允许在根目录下工作
 for(iFileSec=iCurFileSec;iFileSec<iCurFileSec+(Init_Arg.SectorsPerClust);iFileSec++)
 {
    FAT32_ReadSector(iFileSec,FAT32_Buffer);
    for(iFile=0;iFile<Init_Arg.BytesPerSector;iFile+=sizeof(struct direntry))
    {
      pFile=((struct direntry *)(FAT32_Buffer+iFile));
      if(FAT32_CompareName(filepath+index,pFile->deName))
      {    
        FileInfo.FileSize=lb2bb(pFile->deFileSize,4);
	strcpy(FileInfo.FileName,filepath+index);
	FileInfo.FileStartCluster=lb2bb(pFile->deLowCluster,2)+lb2bb(pFile->deHighClust,2)*65536;
	FileInfo.FileCurCluster=FileInfo.FileStartCluster;
	FileInfo.FileNextCluster=FAT32_GetNextCluster(FileInfo.FileCurCluster);
	FileInfo.FileOffset=0;	
	//return &FileInfo;   
        return 1;
      } 
      else
      {        
        for(i=0;i<32;i++)
          sum3 += FAT32_Buffer[ iFile + i ];
        if( sum3 == 0 )
        {            
	    return 0;
        }
        else
        {
          sum3 = 0;
        }
      }
    }
 }
 return 0;
}


/*********************************************************************************
函数名称：unsigned int CalculateTheOffsetInSectorOfTheFile( struct FileInfoStruct *pstru )
函数功能：计算文件所占用的某个扇区的被使用字节数
创建时间：2010-05-16
说    明：参数意义：1、struct FileInfoStruct *pstru 文件相关信息 ； 
                    2、unsigned long Cluster 文件所占用的某一簇 ； 
                    3、unsigned char iSectorInCluster 参数2所列出的簇里的某个扇区。
思    路：1、利用文件的总大小，算出文件的总簇，如果要求得扇区的簇是最后一个簇，
          那么，就具体计算是哪个扇区的，如果扇区也是最后一个扇区，则计算，否则，
          直接返回 512
*********************************************************************************/
unsigned int CalculateTheOffsetInSectorOfTheFile( struct FileInfoStruct *pstru,unsigned long Cluster,unsigned char iSectorInCluster  )
{
  uint  OffsetInTheSector=0;//该扇区中的字节数
  unsigned long TheLastClusterInFile = 0;//文件的最后一个簇
  unsigned long ClustersInFile; //  除了起始簇外，文件所占有的所有簇
  unsigned long FileSizeInTheLastCluster=0;//文件最后一个簇，所存有的字节数
  uchar TheLastSector = 0;//文件的最后一个扇区
  ///////////////////////////////////////////////////////////////////////////////////
  ClustersInFile = pstru->FileSize/Init_Arg.SectorsPerClust;
  ClustersInFile = ClustersInFile/Init_Arg.BytesPerSector;
  if( pstru->FileSize - ClustersInFile*(unsigned long)(Init_Arg.SectorsPerClust)*(unsigned long)(Init_Arg.BytesPerSector) == 0 )
  {
    ClustersInFile = ClustersInFile-1;
  }
  TheLastClusterInFile = pstru->FileStartCluster+ClustersInFile;////
  ///////////////////////////////////////////////////////////////////////////////////
  FileSizeInTheLastCluster = pstru->FileSize - ClustersInFile*(unsigned long)(Init_Arg.SectorsPerClust)*(unsigned long)(Init_Arg.BytesPerSector);
  TheLastSector = FileSizeInTheLastCluster/Init_Arg.BytesPerSector;
  if( FileSizeInTheLastCluster%Init_Arg.BytesPerSector==0 )
  {
      TheLastSector = TheLastSector - 1;
  }///////////
  ///////////////////////////////////////////////////////////////////////////////////
  if( Cluster==TheLastClusterInFile )
  {
    if( iSectorInCluster == TheLastSector )
    {
      OffsetInTheSector   = pstru->FileSize - ( TheLastClusterInFile - pstru->FileStartCluster )*Init_Arg.SectorsPerClust*Init_Arg.BytesPerSector;
      OffsetInTheSector   = OffsetInTheSector - iSectorInCluster * Init_Arg.BytesPerSector;
    }
    else
    {
      return 512;
    }
  }
  else
  {
    return 512;
  }
  return  OffsetInTheSector ;
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
  if( FatBuffer.RecordNumber > 0)//在查询状况下
  {
    WriteANameForANewFile();//这里不是给新文件起名字，只是利用此函数给数组 FileName 赋值
    if( ( OpenFile(FileName)==1 )&&( FatBuffer.NoRecord_flag_InFile==0 ) ) //  打开文件成功，并且文件中所有记录没有都删除
    {        
      if( Delete_flag==0 )//如果没有按下删除键
      {
        //for(i=0;( i<((unsigned long)(FileInfo.FileSize)/(unsigned long)(DemandLength)) )&&(del==0);i++)
        {   //有个跳出循环的机会，因为有可能都是删除的记录
            for( i=0,FatBuffer.Demand[0]=Space,FatBuffer.Demand[1]=Space,FatBuffer.Demand[2]=Space;(FatBuffer.Demand[0]==Space)&&(FatBuffer.Demand[1]==Space)&&(FatBuffer.Demand[2]==Space)&&( i<((unsigned long)(FileInfo.FileSize)/(unsigned long)(DemandLength)) )&&(del==0);i++ )
            {//加入这层循环，是为检查是否读到了删除的记录
                ReadTheRecord( );
                if( (FatBuffer.Demand[0]==Space)&&(FatBuffer.Demand[1]==Space)&&(FatBuffer.Demand[2]==Space) )
                {//读到了删除的记录
                   if( BackOrFore_Demand()==2 )//按压了 --设置减
                   {
                        FatBuffer.LastRecordNumber = FatBuffer.RecordNumber ;
                        FatBuffer.RecordNumber --;
                        if( FatBuffer.RecordNumber==0 )
                           FatBuffer.RecordNumber = 1;//暂时设为1，本应为最前面的那条记录                        
                   }
                   else//按压了 设置加 ，或者是刚按压了 查询
                   {
                        FatBuffer.LastRecordNumber = FatBuffer.RecordNumber ;
                        FatBuffer.RecordNumber ++;
                   }
                }
                else////表明读取成功，没有读到 删除的记录
                {
                  del=1;//及早跳出循环，否则显示上有延误
                }
            }
        }        
      }   
      if( i<((unsigned long)(FileInfo.FileSize)/(unsigned long)(DemandLength)) )  //此条件事用来判别记录是否被全部删除
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
    }
    else
    {
        NOTOpenFile();
    }
  }
}

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