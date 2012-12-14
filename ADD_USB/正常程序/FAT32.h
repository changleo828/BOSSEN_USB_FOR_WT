

#include <ctype.h>

struct PartRecord
{
 unsigned char Active;         //
 unsigned char StartHead;      
 unsigned char StartCylSect[2];
 unsigned char PartType;       
 unsigned char EndHead;        
 unsigned char EndCylSect[2];  
 unsigned char StartLBA[4];   
 unsigned char Size[4];        
};


struct PartSector
{
 unsigned char PartCode[446]; 
 struct PartRecord Part[4];      
 unsigned char BootSectSig0;
 unsigned char BootSectSig1;
};

struct FAT32_FAT_Item
{
 unsigned char Item[4];
};

struct FAT32_FAT
{
 struct FAT32_FAT_Item Items[128];
};


struct FAT32_BPB
{
 unsigned char BS_jmpBoot[3];     
 unsigned char BS_OEMName[8];     
 unsigned char BPB_BytesPerSec[2];
 unsigned char BPB_SecPerClus[1]; 
 unsigned char BPB_RsvdSecCnt[2]; 
 unsigned char BPB_NumFATs[1];    
 unsigned char BPB_RootEntCnt[2]; 
 unsigned char BPB_TotSec16[2];   
 unsigned char BPB_Media[1];      
 unsigned char BPB_FATSz16[2];    
 unsigned char BPB_SecPerTrk[2];  
 unsigned char BPB_NumHeads[2];  
 unsigned char BPB_HiddSec[4];    
 unsigned char BPB_TotSec32[4];   

 unsigned char BPB_FATSz32[4];    
 unsigned char BPB_ExtFlags[2];   
 unsigned char BPB_FSVer[2];     
 unsigned char BPB_RootClus[4];  
 unsigned char FSInfo[2];         
 unsigned char BPB_BkBootSec[2]; 
 unsigned char BPB_Reserved[12];  
 unsigned char BS_DrvNum[1];     
 unsigned char BS_Reserved1[1];  
 unsigned char BS_BootSig[1];     
 unsigned char BS_VolID[4];      
 unsigned char BS_FilSysType[11]; 
 unsigned char BS_FilSysType1[8]; 
};




// Structure of a dos directory entry.
struct direntry 
{
  unsigned char deName[8];      
  unsigned char deExtension[3]; 	
  unsigned char deAttributes;   	
  unsigned char deLowerCase;    	
  unsigned char deCHundredth;   	
  unsigned char deCTime[2];     	
  unsigned char deCDate[2];     	
  unsigned char deADate[2];     	
  unsigned char deHighClust[2];    
  unsigned char deMTime[2];     	
  unsigned char deMDate[2];     	
  unsigned char deLowCluster[2]; 	
  unsigned char deFileSize[4];      
  
  
};

// Stuctures
struct FileInfoStruct
{
 //unsigned char  FileName[12];
 char           FileName[12];
 unsigned long  FileStartCluster;			
 unsigned long  FileCurCluster;
 unsigned long  FileNextCluster;
 unsigned long  FileSize;					
 unsigned char  FileAttr;					
 unsigned short FileCreateTime;			
 unsigned short FileCreateDate;			
 unsigned short FileMTime;
 unsigned short FileMDate;
 unsigned long  FileSector;				    
 //unsigned int   FileOffset;				   
 unsigned long   FileOffset;	
};


struct FAT32_Init_Arg
{
 unsigned char BPB_Sector_No;   
 unsigned long Total_Size;            
 unsigned long FirstDirClust;       
 unsigned long FirstDataSector;	 
 unsigned int  BytesPerSector;	 
 unsigned int  FATsectors;           
 unsigned int  SectorsPerClust;	
 unsigned long FirstFATSector;	
 unsigned long FirstDirSector;	 
 unsigned long RootDirSectors;	 
 unsigned long RootDirCount;	
};



//unsigned char xdata FAT32_Buffer[512]; 
unsigned char FAT32_Buffer[512]; 
struct FAT32_Init_Arg Init_Arg;	      
struct FileInfoStruct FileInfo;       

/////////////////////////////////////////  函数声明  /////////////////////////////////////////////
void EmptyFileProcess_DirCluser( struct FileInfoStruct *pstru,unsigned long offsetTotalInFAT );
unsigned long FAT32_FindFreeCluster(void);
void FAT32_Create( char * dir,unsigned long size);
struct FileInfoStruct * FAT32_OpenFile(char *filepath);
void HeadingWrite(void);
uchar ADDFat( unsigned long iTotalSize_New,struct FileInfoStruct *pstru2,unsigned long ClusterSize );
///////////////////////////////////////////////////////////////////////////////////////////////////


unsigned char * FAT32_ReadSector(unsigned long LBA,unsigned char *buf) 
{
 MMC_get_data_LBA(LBA,512,buf);
 return buf;
}

unsigned char FAT32_WriteSector(unsigned long LBA,unsigned char *buf)
{
 return MMC_write_sector(LBA,buf);
}

unsigned long lb2bb(unsigned char *dat,unsigned char len) 
{
 unsigned long temp=0;
 unsigned long fact=1;
 unsigned char i=0;
 for(i=0;i<len;i++)
 {
  temp+=dat[i]*fact;
  fact*=256;
 }
 return temp;
}



unsigned char* bb2lb(unsigned long bb)
{
  unsigned char lb[4];
  //unsigned char i=0;
  unsigned int  it,j;
  
  it = bb/256;  
  lb[0] = bb-it*256;
  
  j=it;
  it = it/256;
  lb[1] = j-it*256;
  
  j=it;
  it = it/256;
  lb[2] = j-it*256;
  
  j=it;
  it = it/256;
  lb[3] = j-it*256;
  
  return lb;
}


unsigned long  FAT32_FindBPB()  
{
 
 FAT32_ReadSector(0,FAT32_Buffer);
 return lb2bb(((((struct PartSector *)(FAT32_Buffer))->Part[0]).StartLBA),4);
}

unsigned long FAT32_Get_Total_Size() //存储器的总容量，单位为M
{
 FAT32_ReadSector(Init_Arg.BPB_Sector_No,FAT32_Buffer);
 return ((float)(lb2bb((((struct FAT32_BPB *)(FAT32_Buffer))->BPB_TotSec32),4)))*0.0004883;
}

void FAT32_Init(struct FAT32_Init_Arg *arg)
{
 struct FAT32_BPB *bpb=(struct FAT32_BPB *)(FAT32_Buffer);             
 arg->BPB_Sector_No   =FAT32_FindBPB();                                             
 arg->Total_Size      =FAT32_Get_Total_Size();                                        
 arg->FATsectors      =lb2bb((bpb->BPB_FATSz32)    ,4);                       
 arg->FirstDirClust   =lb2bb((bpb->BPB_RootClus)   ,4);                        
 arg->BytesPerSector  =lb2bb((bpb->BPB_BytesPerSec),2);                   
 arg->SectorsPerClust =lb2bb((bpb->BPB_SecPerClus) ,1);                  
 arg->FirstFATSector  =lb2bb((bpb->BPB_RsvdSecCnt) ,2)+arg->BPB_Sector_No;
 arg->RootDirCount    =lb2bb((bpb->BPB_RootEntCnt) ,2);                  
 arg->RootDirSectors  =(arg->RootDirCount)*32>>9;                           
 arg->FirstDirSector  =(arg->FirstFATSector)+(bpb->BPB_NumFATs[0])*(arg->FATsectors); 
 arg->FirstDataSector =(arg->FirstDirSector)+(arg->RootDirSectors); 
}

void FAT32_EnterRootDir()
{
 unsigned long iRootDirSector;
 unsigned long iDir;
 struct direntry *pDir;
 for(iRootDirSector=(Init_Arg.FirstDirSector);iRootDirSector<(Init_Arg.FirstDirSector)+(Init_Arg.SectorsPerClust);iRootDirSector++)
 {
  FAT32_ReadSector(iRootDirSector,FAT32_Buffer);
  for(iDir=0;iDir<Init_Arg.BytesPerSector;iDir+=sizeof(struct direntry))
  {
   pDir=((struct direntry *)(FAT32_Buffer+iDir));
   if((pDir->deName)[0]!=0x00 /*无效目录项*/ && (pDir->deName)[0]!=0xe5 /*无效目录项*/ && (pDir->deName)[0]!=0x0f /*无效属性*/)
   {
    
   }
  }
 }
}

void FAT32_CopyName(unsigned char *Dname,unsigned char *filename)
{
 unsigned char i=0;
 for(;i<11;i++)
 {
  Dname[i]=filename[i];
 }
 Dname[i]=0;
}

unsigned long FAT32_EnterDir(char *path)
{
 unsigned long iDirSector;
 unsigned long iCurSector=Init_Arg.FirstDirSector;
 unsigned long iDir;
 struct direntry *pDir;
 //unsigned char DirName[12];
 unsigned char depth=0,i=0;
 while(path[i]!=0)
 {
  if(path[i]=='\\')
  { 
   depth++;
  }
  i++;
 }
 if(depth==1)
 {
  return iCurSector;    
 }
 for(iDirSector=iCurSector;iDirSector<(Init_Arg.FirstDirSector)+(Init_Arg.SectorsPerClust);iDirSector++)
 {
  FAT32_ReadSector(iDirSector,FAT32_Buffer);
  for(iDir=0;iDir<Init_Arg.BytesPerSector;iDir+=sizeof(struct direntry))
  {
   pDir=((struct direntry *)(FAT32_Buffer+iDir));
   if((pDir->deName)[0]!=0x00 /*无效目录项*/ && (pDir->deName)[0]!=0xe5 /*无效目录项*/ && (pDir->deName)[0]!=0x0f /*无效属性*/)
   {
    //Printf_File_Name(pDir->deName);
   }
  }
 }
}

//unsigned char FAT32_CompareName(unsigned char *sname,unsigned char *dname)
unsigned char FAT32_CompareName( char *sname,unsigned char *dname)
{
 unsigned char i,j=8;
 unsigned char name_temp[12];
 for(i=0;i<11;i++) name_temp[i]=0x20;
 name_temp[11]=0;
 i=0;
 while(sname[i]!='.')
 {
  name_temp[i]=sname[i];
  i++;
 }
 i++;
 while(sname[i]!=0)
 {
  name_temp[j++]=sname[i];
  i++;
 }
 //Printf(name_temp,0);
 for(i=0;i<11;i++)
 {
  if(name_temp[i]!=dname[i]) return 0;
 }
  //Printf(name_temp,0);
 return 1;
}

unsigned long FAT32_GetNextCluster(unsigned long LastCluster)//簇
{
 unsigned long temp;
 struct FAT32_FAT *pFAT;
 struct FAT32_FAT_Item *pFAT_Item;
 //struct FAT32_FAT_Item pFAT_Item;
 temp=( (LastCluster/128)+Init_Arg.FirstFATSector );
 FAT32_ReadSector(temp,FAT32_Buffer);
 pFAT=(struct FAT32_FAT *)FAT32_Buffer;
 pFAT_Item=&((pFAT->Items)[LastCluster%128]);
 //return lb2bb(pFAT_Item,4);
 return lb2bb(pFAT_Item->Item,4);
}


struct FileInfoStruct * FAT32_OpenFile(char *filepath)
//struct FileInfoStruct * FAT32_OpenFile(char filepath[9])
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
	return &FileInfo;        
      } 
      else
      {        
        for(i=0;i<32;i++)
          sum3 += FAT32_Buffer[ iFile + i ];
        if( sum3 == 0 )
        {
            FAT32_Create(filepath,0);
            FirstCreatTheFile_flag  = 1;  //  表明第一次生成该文件
            FAT32_ReadSector(iFileSec,FAT32_Buffer);
            pFile=((struct direntry *)(FAT32_Buffer+iFile));
            FileInfo.FileSize=lb2bb(pFile->deFileSize,4);
	    strcpy(FileInfo.FileName,filepath+index);
	    FileInfo.FileStartCluster=lb2bb(pFile->deLowCluster,2)+lb2bb(pFile->deHighClust,2)*65536;
	    FileInfo.FileCurCluster=FileInfo.FileStartCluster;
	    FileInfo.FileNextCluster=FAT32_GetNextCluster(FileInfo.FileCurCluster);
	    FileInfo.FileOffset=0;	
	    return &FileInfo;
        }
        else
        {
          sum3 = 0;
        }
      }
    }
 }
 
}



void FAT32_ReadFile(struct FileInfoStruct *pstru)
{
 unsigned long Sub=pstru->FileSize-pstru->FileOffset;
 unsigned long iSectorInCluster=0;
 unsigned long i=0;
 while(pstru->FileNextCluster!=0x0fffffff)  
 {
  for(iSectorInCluster=0;iSectorInCluster<Init_Arg.SectorsPerClust;iSectorInCluster++)  
  {
   FAT32_ReadSector((((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster),FAT32_Buffer);
   pstru->FileOffset+=Init_Arg.BytesPerSector;
   Sub=pstru->FileSize-pstru->FileOffset;
   for(i=0;i<Init_Arg.BytesPerSector;i++)
   {
    //send(FAT32_Buffer[i]);   //将数据发送到终端上显示
     //tryy11[i]= FAT32_Buffer[i]; ////////////////////////////try
   }
  }
  pstru->FileCurCluster=pstru->FileNextCluster;  
  pstru->FileNextCluster=FAT32_GetNextCluster(pstru->FileCurCluster);   
 }
 iSectorInCluster=0;
 while(Sub>=Init_Arg.BytesPerSector)   
 {
  FAT32_ReadSector((((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster++),FAT32_Buffer);
  pstru->FileOffset+=Init_Arg.BytesPerSector;
  Sub=pstru->FileSize-pstru->FileOffset;
  for(i=0;i<Init_Arg.BytesPerSector;i++)
  {
   //send(FAT32_Buffer[i]);
    //tryy11[i]= FAT32_Buffer[i]; ////////////////////////////try
  }
 }
 FAT32_ReadSector((((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster),FAT32_Buffer); //读取最后一个扇区
 for(i=0;i<Sub;i++)    
 {
  //send(FAT32_Buffer[i]);
   //tryy11[i]= FAT32_Buffer[i]; ////////////////////////////try，
 }	
}


unsigned int FAT32_ReadFileTheLastCluster(struct FileInfoStruct *pstru)
{
  unsigned long iSectorInCluster=0;
  //unsigned long Sub=pstru->FileSize-pstru->FileOffset;
  unsigned long ClustersInFile; 
  unsigned long FileSizeInTheLastCluster=0;
  if( pstru->FileSize != 0 )  
  {
    ClustersInFile = pstru->FileSize/Init_Arg.SectorsPerClust;
    ClustersInFile = ClustersInFile/Init_Arg.BytesPerSector;
    if( pstru->FileSize - ClustersInFile*(unsigned long)(Init_Arg.SectorsPerClust)*(unsigned long)(Init_Arg.BytesPerSector) == 0 )
    {
      ClustersInFile = ClustersInFile-1;
    }
    pstru->FileCurCluster = pstru->FileStartCluster+ClustersInFile;
  
    FileSizeInTheLastCluster = pstru->FileSize - ClustersInFile*(unsigned long)(Init_Arg.SectorsPerClust)*(unsigned long)(Init_Arg.BytesPerSector);
    iSectorInCluster = FileSizeInTheLastCluster/Init_Arg.BytesPerSector;
    if( FileSizeInTheLastCluster%Init_Arg.BytesPerSector==0 )
    {
      iSectorInCluster = iSectorInCluster - 1;
    }
    memset( FAT32_Buffer,0,512 );
    FAT32_ReadSector((((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster),FAT32_Buffer); //读取最后一个扇区
  }
  return iSectorInCluster;  
}


#define   FileStartCluser_H16 0X14  
#define   FileStartCluser_L16 0X1A  
void EmptyFileProcess_DirCluser( struct FileInfoStruct *pstru,unsigned long offsetTotalInFAT )
{
  uchar *p; 
  unsigned long iFileSec,iCurFileSec,iFile;
  struct direntry *pFile;
  //uchar   DIRbuffer[512]; 
  
  iCurFileSec = Init_Arg.FirstDirSector;  
  for(iFileSec=iCurFileSec;iFileSec<iCurFileSec+(Init_Arg.SectorsPerClust);iFileSec++)
  {
    FAT32_ReadSector(iFileSec,FAT32_Buffer);
    for(iFile=0;iFile<Init_Arg.BytesPerSector;iFile+=sizeof(struct direntry))
    {
      pFile=((struct direntry *)(FAT32_Buffer+iFile));
      if(FAT32_CompareName(pstru->FileName,pFile->deName))//
      {
        
        p=bb2lb( offsetTotalInFAT/4 );
        
        FAT32_Buffer[ iFile + FileStartCluser_L16 +0 ] = *( p+0 );
        FAT32_Buffer[ iFile + FileStartCluser_L16 +1 ] = *( p+1 );
        FAT32_Buffer[ iFile + FileStartCluser_H16 +0 ] = *( p+2 );
        FAT32_Buffer[ iFile + FileStartCluser_H16 +1 ] = *( p+3 ); 
        FAT32_WriteSector( iFileSec,FAT32_Buffer );
      }
    }
  }
  pstru->FileStartCluster = offsetTotalInFAT/4;
  pstru->FileCurCluster = pstru->FileStartCluster;
}

uchar EmptyFileProcess( struct FileInfoStruct *pstru )
{

  //查找 FAT 链接表，
  uint  iFATSec = 0;
  uint  iCurFATSec = 0;
  uint  OffsetInSector  = 0;
  //uint  OffsetInSector_i=0; //  临时存储
  unsigned long Cluster=0;
  if( pstru->FileStartCluster==0 )  //  表明是空文件
  {
    iCurFATSec = Init_Arg.FirstFATSector;  
    for( iFATSec=iCurFATSec;iFATSec<iCurFATSec+(Init_Arg.FATsectors);iFATSec++ )
    {
      FAT32_ReadSector(iFATSec,FAT32_Buffer);
      for( OffsetInSector=0;OffsetInSector<Init_Arg.BytesPerSector;OffsetInSector+=4 )
      {
          if( (FAT32_Buffer[OffsetInSector+0]==0XFF)&&(FAT32_Buffer[OffsetInSector+1]==0XFF)&&(FAT32_Buffer[OffsetInSector+2]==0XFF)&&(FAT32_Buffer[OffsetInSector+3]==0X0F) )
          {//找到 FAT 表 的最后一个使用簇
            if( (FAT32_Buffer[OffsetInSector+4]==0X00)&&(FAT32_Buffer[OffsetInSector+5]==0X00)&&(FAT32_Buffer[OffsetInSector+6]==0X00)&&(FAT32_Buffer[OffsetInSector+7]==0X00) )
            {
              OffsetInSector+=4;  
              FAT32_Buffer[OffsetInSector+0]=0XFF;
              FAT32_Buffer[OffsetInSector+1]=0XFF;
              FAT32_Buffer[OffsetInSector+2]=0XFF;
              FAT32_Buffer[OffsetInSector+3]=0X0F;
              FAT32_WriteSector( iFATSec,FAT32_Buffer );
              Cluster = ( iFATSec-Init_Arg.FirstFATSector )*(Init_Arg.BytesPerSector) + OffsetInSector;
              EmptyFileProcess_DirCluser( pstru, Cluster );  
              return 1;
            }
          }          
      }
    }
  }
  return 2;//不是空文件
}


#define   FileSizeStartOffset   0X1C  // 
void FAT32_WriteFile(struct FileInfoStruct *pstru,unsigned char *data1,unsigned long size)
{
  unsigned long   iSectorInCluster  = 0; //
  unsigned int    OffsetInSector    = 0; // 
  uchar WriteBuf[512]; //写入文件的临时数组
  unsigned int i,j,k;
  //////////////////////////////////////////////////////////////////////////
  unsigned long iFileSec,iCurFileSec,iFile;
  struct direntry *pFile;
  unsigned char *p;
  unsigned long iTotalSize_New=0; // 
  unsigned int  ClusterSize = 0;////  
  /////////////////////////////////////////////////////////////////////////
  
  ClusterSize = ((Init_Arg.SectorsPerClust)*(Init_Arg.BytesPerSector));//
  
  EmptyFileProcess( pstru );
  
  iSectorInCluster = FAT32_ReadFileTheLastCluster( pstru );//文件在该簇(最后一个簇)中已经占了几个扇区,最大值为7
  
  OffsetInSector   = pstru->FileSize - ( pstru->FileCurCluster - pstru->FileStartCluster )*Init_Arg.SectorsPerClust*Init_Arg.BytesPerSector;
  OffsetInSector   = OffsetInSector - iSectorInCluster * Init_Arg.BytesPerSector;
  memset( WriteBuf,0,512 );
  for( i=0;i<OffsetInSector;i++ )//先将该扇区的已有字节存入临时数组WriteBuf
  {
    WriteBuf[i]=FAT32_Buffer[i];
  }
  ///////////////////////////////////////////////////////////////////////////////////
  if( FirstCreatTheFile_flag  ==1 ) //  如果该文件是第一次生成
  {
    //FirstCreatTheFile_flag  = 0;
    OffsetInSector = HeadingLength ;
    //size
    HeadingWrite();
    for( i=0;i<OffsetInSector;i++ )//先将该扇区的已有字节存入临时数组WriteBuf
    {
      WriteBuf[i] = Heading[i];
    }
  }
  ////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////修改原文件大小
  //iCurFileSec = FAT32_EnterDir( pstru->FileName )/*Init_Arg.FirstDirSector*/;
  //kkkk = iCurFileSec;
  iCurFileSec = Init_Arg.FirstDirSector;
  for(iFileSec=iCurFileSec;iFileSec<iCurFileSec+(Init_Arg.SectorsPerClust);iFileSec++)
  {
    FAT32_ReadSector(iFileSec,FAT32_Buffer);
    for(iFile=0;iFile<Init_Arg.BytesPerSector;iFile+=sizeof(struct direntry))
    {
      pFile=((struct direntry *)(FAT32_Buffer+iFile));
      if(FAT32_CompareName(pstru->FileName,pFile->deName))//
      {
       
        if( FirstCreatTheFile_flag  ==1 ) //  
        {
          FirstCreatTheFile_flag  = 0;
          p=bb2lb( HeadingLength + size);
        }
        else
        {
          p=bb2lb( pstru->FileSize+size);
        }
        //////////
        FAT32_Buffer[iFile+FileSizeStartOffset+0] =  *( p+0 );
        FAT32_Buffer[iFile+FileSizeStartOffset+1] =  *( p+1 );
        FAT32_Buffer[iFile+FileSizeStartOffset+2] =  *( p+2 );
        FAT32_Buffer[iFile+FileSizeStartOffset+3] =  *( p+3 );
        //FAT32_WriteSector( Init_Arg.FirstDataSector,FAT32_Buffer );
        FAT32_WriteSector( iFileSec,FAT32_Buffer );
      }
    }
  }
  //////////////////////////////////////////////////////////////////////
  /// 检查再写入 size 大小的字节后，文件总大小是否超过1个簇？？？===修改FAT表的链接关系
  iTotalSize_New = pstru->FileSize + size;
  ADDFat(  iTotalSize_New,pstru,ClusterSize );
  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////
  for( i=OffsetInSector,j=0; ( i < OffsetInSector+size )&&( i<512 ); i++,j++ )  //加入新的内容
  {                                                                             //  数组的最大长度是512，一次写入不可以超过 512 个字节
    WriteBuf[i]=data1[j];
  }
  k = iSectorInCluster/Init_Arg.SectorsPerClust;  //  此时的 iSectorInCluster 有可能大于等于8，但是，在一个簇中，其值为0到7的整数，大于7，就表示到了另一簇了
  iSectorInCluster = iSectorInCluster - k*Init_Arg.SectorsPerClust;//在 pstru->FileCurCluster 的哪个扇区。
  FAT32_WriteSector( (((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster),WriteBuf );
  //超过一个扇区，但是没有超过一个簇的字节，再次写入
  if( (pstru->FileCurCluster) == (pstru->FileStartCluster + iTotalSize_New/ClusterSize ) )
  {
    if( OffsetInSector+size > 512 )
    {
      memset( WriteBuf,0,512 );
      for( ;i < OffsetInSector+size;i++,j++ )
      {
        WriteBuf[i-512]=data1[j];    
      }
      FAT32_WriteSector( (((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+(iSectorInCluster)+1,WriteBuf );
    }
  }
  if( (pstru->FileCurCluster) < (pstru->FileStartCluster + iTotalSize_New/ClusterSize ) )//超过一个簇
  {
    if( OffsetInSector+size > 512 )
    {
      memset( WriteBuf,0,512 );
      for( ;i < OffsetInSector+size;i++,j++ )
      {
        WriteBuf[i-512]=data1[j];    
      }
      pstru->FileCurCluster = iTotalSize_New/ClusterSize + pstru->FileStartCluster;
      FAT32_WriteSector( (((pstru->FileCurCluster)-2)*(Init_Arg.SectorsPerClust))+Init_Arg.FirstDataSector+0,WriteBuf );
    }
  }
}




#define ATTR_DIRECTORY  0x10           
#define ATTR_ARCHIVE    0x20            
#define LCASE_BASE      0x08           
#define LCASE_EXT       0x10           


void FAT32_Create( char * dir,unsigned long size)
{
	unsigned char name[11];
	//unsigned char *p=dir;
        char *p=dir;
	unsigned char deep=0;
	unsigned char i;
        unsigned long j;
	unsigned long cluster=0;
	//unsigned char *buffer;
        //uchar *b2l;
	//unsigned int sector;
	//struct direntry *item;
        unsigned long iDirSector=0;  //  根目录所在扇区
        uint  OffsetInSector=0;
	//if(*p != '\\')return 1;//invalid path
	while(*p)
	{
		if(*p == '\\')
		{
			deep++;
		}
		p++;
	}
	p=dir;
	for(i=0;i<deep-1;i++)
	{
		p++;
		for(j=0;j<11;j++)name[j]=0x20;
		j=0;
		while(*p != '\\')
		{
			if((*p) >= 'a' && (*p) <= 'z')name[j] = (*p++)-0x20;
			else name[j] = *p++;
			j++;
		}
                
                cluster = FileInfo.FileStartCluster;
	}
	p++;
	for(j=0;j<11;j++)
	 name[j]=0x20;
	j=0;
	while(*p != '.')
	{
		if(*p>='a' && *p<='z')name[j]=(*p++)-0x20;
		else name[j]=*p++;
		j++;
	}
	j=8;
	p++;
	while(*p)
	{
		if(*p>='a' && *p<='z')name[j]=(*p++)-0x20;
		else name[j]=*p++;
		j++;
	}       
        cluster = FAT32_FindFreeCluster();
	
        FAT32_ReadSector(  Init_Arg.FirstDirSector + iDirSector, FAT32_Buffer );
        OffsetInSector=0;
        i=1;
        do{
          if( OffsetInSector<511 )
          {
            
            for( j=0;(OffsetInSector<32*i);OffsetInSector++ )
            {
              j += FAT32_Buffer[ OffsetInSector ];
            }
            i++;
          }
          else
          {
            OffsetInSector = 0;
            i=1;
            iDirSector++;
            FAT32_ReadSector(  Init_Arg.FirstDirSector + iDirSector, FAT32_Buffer );
          }
        }while( j!=0 ); //  求得文件目录项的空白处
        if( j==0 )  //  找到
        {
          OffsetInSector = OffsetInSector  -32; //  地址复原
          
          for( i=0;i<11;i++ ) //  装入新文件的名字
            FAT32_Buffer[ OffsetInSector + i ] = name[i];
          /*
          ///装入文件起始簇          
          b2l=bb2lb( cluster );
          FAT32_Buffer[ OffsetInSector + FileStartCluser_L16+0 ] = *( b2l+0 );
          FAT32_Buffer[ OffsetInSector + FileStartCluser_L16+1 ] = *( b2l+1 );
          FAT32_Buffer[ OffsetInSector + FileStartCluser_H16+0 ] = *( b2l+2 );
          FAT32_Buffer[ OffsetInSector + FileStartCluser_H16+1 ] = *( b2l+3 );
          */
          /////文件大小为 0
          FAT32_WriteSector( Init_Arg.FirstDirSector + iDirSector,FAT32_Buffer );
        }
	//return cluster;//reutn the first cluster number
}

/*******************************************************************************
函数名称： 
函数功能： 寻找闲置簇，用以生成新文件时使用
时间	： 2010-3-31
说明    ： 
*******************************************************************************/
unsigned long FAT32_FindFreeCluster(void)
{
  //查找 FAT 链接表，
  uint  iFATSec = 0;
  uint  iCurFATSec = 0;
  uint  OffsetInSector  = 0;
  //uint  OffsetInSector_i=0; //  临时存储
  unsigned long Cluster=0;
  
  //FAT32_ReadSector(  Init_Arg.FirstFATSector + iFATSector, FAT32_Buffer );//读取第一个FAT表，装入FAT32_Buffer
  iCurFATSec = Init_Arg.FirstFATSector;  
  for( iFATSec=iCurFATSec;iFATSec<iCurFATSec+(Init_Arg.FATsectors);iFATSec++ )
  {
      FAT32_ReadSector(iFATSec,FAT32_Buffer);
      for( OffsetInSector=0;OffsetInSector<Init_Arg.BytesPerSector;OffsetInSector+=4 )
      {
          if( (FAT32_Buffer[OffsetInSector+0]==0XFF)&&(FAT32_Buffer[OffsetInSector+1]==0XFF)&&(FAT32_Buffer[OffsetInSector+2]==0XFF)&&(FAT32_Buffer[OffsetInSector+3]==0X0F) )
          {//找到 FAT 表 的最后一个使用簇
            if( (FAT32_Buffer[OffsetInSector+4]==0X00)&&(FAT32_Buffer[OffsetInSector+5]==0X00)&&(FAT32_Buffer[OffsetInSector+6]==0X00)&&(FAT32_Buffer[OffsetInSector+7]==0X00) )
            {
              OffsetInSector+=4;              
              Cluster = ( iFATSec-Init_Arg.FirstFATSector )*(Init_Arg.BytesPerSector) + OffsetInSector;
              Cluster = Cluster/4;
              return  Cluster;
            }
          }          
      }
  }
  
  return 0;
}

uchar ADDFat( unsigned long iTotalSize_New,struct FileInfoStruct *pstru2,unsigned long ClusterSize )
{
  uchar *ppt;
  //iTotalSize_New = pstru2->FileSize + size;
  uint  iFATSector=0;
  uint  OffsetInFatSector=0;
  if( (pstru2->FileCurCluster) < (pstru2->FileStartCluster + iTotalSize_New/ClusterSize ) )
  {
    iFATSector = (pstru2->FileCurCluster * 4 + 4)/Init_Arg.BytesPerSector + Init_Arg.FirstFATSector ;//此值如果无余数，表明是该扇区的最后一个链接表
    OffsetInFatSector = (pstru2->FileCurCluster * 4 + 4)-((pstru2->FileCurCluster * 4 + 4)/Init_Arg.BytesPerSector)*Init_Arg.BytesPerSector;
    if( (iFATSector>=1)&&(OffsetInFatSector==0) )//将原先的最后一簇改为倒数第二簇
    {
      FAT32_ReadSector( iFATSector-1,FAT32_Buffer );
      ppt=bb2lb( pstru2->FileCurCluster+1 );//指向最后一个链接簇
      FAT32_Buffer[508+0] = *(ppt+0);
      FAT32_Buffer[508+1] = *(ppt+1);
      FAT32_Buffer[508+2] = *(ppt+2);
      FAT32_Buffer[508+3] = *(ppt+3);
      FAT32_WriteSector( iFATSector-1,FAT32_Buffer );
    }
    else
      if( OffsetInFatSector!=0 )
      {
        FAT32_ReadSector( iFATSector,FAT32_Buffer );
        ppt=bb2lb( pstru2->FileCurCluster+1 );//指向最后一个链接簇
        FAT32_Buffer[OffsetInFatSector-4+0] = *(ppt+0);
        FAT32_Buffer[OffsetInFatSector-4+1] = *(ppt+1);
        FAT32_Buffer[OffsetInFatSector-4+2] = *(ppt+2);
        FAT32_Buffer[OffsetInFatSector-4+3] = *(ppt+3);
        FAT32_WriteSector( iFATSector,FAT32_Buffer );
      }
    
    for( ; iFATSector<Init_Arg.FirstFATSector + Init_Arg.FATsectors;iFATSector++,OffsetInFatSector=0 )
    {//写入新加入的最后一簇
      FAT32_ReadSector( iFATSector,FAT32_Buffer );
      //OffsetInFatSector=0;
      for( ;OffsetInFatSector<Init_Arg.BytesPerSector;OffsetInFatSector+=4 )
      {
        if( (FAT32_Buffer[OffsetInFatSector+0]==0X00)&&(FAT32_Buffer[OffsetInFatSector+1]==0X00)&&(FAT32_Buffer[OffsetInFatSector+2]==0X00)&&(FAT32_Buffer[OffsetInFatSector+3]==0X00) )
        {//找到 FAT 表 的最开始一个未被使用的簇
            //p=bb2lb( iTotalSize_New/ClusterSize + pstru2->FileStartCluster);
            FAT32_Buffer[OffsetInFatSector+0] = 0XFF;
            FAT32_Buffer[OffsetInFatSector+1] = 0XFF;
            FAT32_Buffer[OffsetInFatSector+2] = 0XFF;
            FAT32_Buffer[OffsetInFatSector+3] = 0X0F;
            FAT32_WriteSector( iFATSector,FAT32_Buffer );
            return 1;
        }
      }
    }    
  }
  //else
  //{
    return 2;
  //}
}

/***************************************************************************************
函数名称：void HeadingWrite(void)
函数功能：刚生成文件时，往文件里写入标题行
创建时间：
修改时间：
作    者：
说    明：
***************************************************************************************/
#define   Space   0X20
#define   Comma   0X2C
void HeadingWrite(void)
{
  uchar  *p;
  uchar i=0;
  
  Heading[0] = 'O';
  Heading[1] = Comma;
  Heading[2] = 'A';
  Heading[3] = Comma;
  Heading[4] = 'V';
  Heading[5] = 'V';
  Heading[6] = Comma;
  Heading[7] = 'V';
  Heading[8] = 'A';
  Heading[9] = Comma;
  Heading[10] = 'M';
  Heading[11] = 'N';
  Heading[12] = '=';
  //Heading[12] = MN_data/
  p=Decomposition( MN_data ); 
  for( i=0;i<4;i++ )
  {
      //Heading[i+13]= *(p+i) + Parameter;
      Heading[16-i]= *(p+i) + Parameter;
  }
  i = Heading[16];
  Heading[16] = '.';
  Heading[17] = i;
  Heading[18] = Comma;
  /////////////加入施工方法
  if( FeatureSelection_data==0 )
  {
    Heading[19] = 0XB3;
    Heading[20] = 0XAC;
    Heading[21] = 0XC7;
    Heading[22] = 0XB0;
    Heading[23] = 0XCC;
    Heading[24] = 0XBD;    
  }
  else
    if( FeatureSelection_data==1 )
    {
      Heading[19] = 0XB5;
      Heading[20] = 0XE7;
      Heading[21] = 0XB2;
      Heading[22] = 0XE2;
      Heading[23] = 0XC9;
      Heading[24] = 0XEE;    
    }
    else
      if( FeatureSelection_data==2 )
      {
        Heading[19] = 0XB5;
        Heading[20] = 0XE7;
        Heading[21] = 0XCD;
        Heading[22] = 0XB8;
        Heading[23] = 0XCA;
        Heading[24] = 0XC6;    
      }
      else
      {
        Heading[19] = 'X';
        Heading[20] = 'X';
        Heading[21] = 'X';
        Heading[22] = Space;
        Heading[23] = Space;
        Heading[24] = Space;
      }
  Heading[25] = Comma;
  ///频率
  if( FrequencyPointSelection_data==0 )
  {
    Heading[26] = Space;
    Heading[27] = Space;
    Heading[28] = 5 + Parameter;
  }
  else
    if( FrequencyPointSelection_data==1 )
    {
      Heading[26] = Space;
      Heading[27] = 1 + Parameter;
      Heading[28] = 5 + Parameter;
    }
    else
      if( FrequencyPointSelection_data==2 )
      {
        Heading[26] = 1 + Parameter;
        Heading[27] = 2 + Parameter;
        Heading[28] = 0 + Parameter;
      }
      else
      {
        Heading[26] = 4 + Parameter;
        Heading[27] = 0 + Parameter;
        Heading[28] = 0 + Parameter;
      }
  Heading[29] = 'H';
  Heading[30] = 'Z';
  Heading[31] = '\r'; // 换行
}
