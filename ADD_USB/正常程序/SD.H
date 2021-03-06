
#define   SPI_DI_L    ( P5OUT &=~ BIT1 )
#define   SPI_DI_H    ( P5OUT |= BIT1 )
#define   SPI_DO_L    ( P5OUT &=~ BIT2 )
#define   SPI_DO_H    ( P5OUT |= BIT2 )
#define   SPI_SCL_L   ( P5OUT &=~ BIT3 )
#define   SPI_SCL_H   ( P5OUT |= BIT3 )
#define   SPI_CS_L    ( P5OUT &=~ BIT4 )
#define   SPI_CS_H    ( P5OUT |= BIT4 )

#define   SPI_DO      ( P5IN & BIT2 )

//------------------------------------------------------------
// Error define
//-------------------------------------------------------------
#define INIT_CMD0_ERROR         0x01
#define INIT_CMD1_ERROR		0x02
#define WRITE_BLOCK_ERROR	0x03
#define READ_BLOCK_ERROR   	0x04 
//-------------------------------------------------------------

// data type
//-------------------------------------------------------------   
// this structure holds info on the MMC card currently inserted 
typedef struct MMC_VOLUME_INFO
{ //MMC/SD Card info
  unsigned int  size_MB;
  unsigned char sector_multiply;
  unsigned int  sector_count;
  unsigned char name[6];
} VOLUME_INFO_TYPE; 

typedef struct STORE 
{ 
  unsigned char dat[100]; 
} BUFFER_TYPE; //256 bytes, 128 words

BUFFER_TYPE sectorBuffer; //512 bytes for sector buffer

//--------------------------------------------------------------
unsigned int  readPos=0;
unsigned char sectorPos=0;
unsigned char LBA_Opened=0; //Set to 1 when a sector is opened.
unsigned char Init_Flag;    //Set it to 1 when Init is processing.
//---------------------------------------------------------------

void delay(unsigned int time)
{
 while(time--);
}

//****************************************************************************
// Port Init
void MMC_Port_Init()
//****************************************************************************
{
   SPI_SCL_H;
   SPI_DO_H;
   SPI_CS_H;

}

//****************************************************************************
//Routine for sending a byte to MMC/SD-Card
void Write_Byte_MMC(unsigned char value)
//****************************************************************************
{ 
   unsigned char i; 
   
   //SPI_BY=0; 
   //Software SPI
   for (i=0;i<8;i++) 
   {  //write a byte
      if (((value>>(7-i))&0x01)==0x01) //
     //if( value&0x01==0x01 )
	   SPI_DI_H; //Send bit by bit(MSB First)
      else SPI_DI_L;
       SPI_SCL_L; //set Clock Impuls low
       
       ///value=value>>1;  //��������
       
      if(Init_Flag) 
	   delay(8); 
      SPI_SCL_H; //set Clock Impuls High
      if(Init_Flag) 
	   delay(8);     
   }//write a byte
   //MMC_BUSY_LED=1;
}

//****************************************************************************
//Routine for reading a byte from MMC/SD-Card
unsigned char Read_Byte_MMC()
//****************************************************************************
{ 
   unsigned char temp=0;
   unsigned char i;

   //SPI_BY=0;
   //Software SPI
   for (i=0;i<8;i++) //MSB First
   {
      //SPI_DO_H;
      SPI_SCL_L; //Clock Impuls (Low)
      
      P5DIR	&=	~BIT2; //����
      
      if(Init_Flag)
	   delay(8);      
      //temp=(temp<<1)+(unsigned char)SPI_DO; //read mmc data out pin 
      if(SPI_DO==0)
      {
        temp=(temp<<1)+0; //read mmc data out pin 
      }
      else
      {
        temp=(temp<<1)+1; //read mmc data out pin 
      }
      P5DIR	|=	BIT2; //���
      SPI_SCL_H; //set Clock Impuls High
      if(Init_Flag) 
	   delay(8);	
   }
   //SPI_BY=1;
   return (temp);
}

//****************************************************************************
//Send a Command to MMC/SD-Card
//Return: the second byte of response register of MMC/SD-Card
unsigned char Write_Command_MMC(unsigned char *CMD)
//****************************************************************************
{
   unsigned char tmp;
   unsigned char retry=0;
   unsigned char i;

   //set MMC_Chip_Select to high (MMC/SD-Card disable) 
   SPI_CS_H;
   //send 8 Clock Impulse
   Write_Byte_MMC(0xFF);
   //set MMC_Chip_Select to low (MMC/SD-Card active)
   SPI_CS_L;

   //send 6 Byte Command to MMC/SD-Card
   for (i=0;i<0x06;i++) 
   { 
      Write_Byte_MMC(*CMD++);
   }
   
   //get 16 bit response
   Read_Byte_MMC(); //read the first byte,ignore it. 
   do 
   {  //Only last 8 bit is used here.Read it out. 
      tmp = Read_Byte_MMC();
      retry++;
   }
   while((tmp==0xff)&&(retry<100)); 
   return(tmp);
}

//****************************************************************************
//Routine for Init MMC/SD card(SPI-MODE)
unsigned char MMC_Init()
//****************************************************************************
{  
   unsigned char retry,temp;
   unsigned char i;
   unsigned char CMD[] = {0x40,0x00,0x00,0x00,0x00,0x95};
   //send_s("SD CARD Init!");
   MMC_Port_Init(); //Init SPI port  

   delay(200);
   
   Init_Flag=1; //Set the init flag

   for (i=0;i<0xff;i++) 
   {
      Write_Byte_MMC(0xff); //send 74 clock at least!!!
   }
	
   //Send Command CMD0 to MMC/SD Card
   //send_s("Send Command CMD0 to MMC/SD Card");
   retry=0;
   do
   { //retry 200 times to send CMD0 command 
     temp=Write_Command_MMC(CMD);
     retry++;
     if(retry==200) 
     { //time out
       return(INIT_CMD0_ERROR);//CMD0 Error!
     }
   } 
   while(temp!=1);
   
   //Send Command CMD1 to MMC/SD-Card
   //send_s("Send Command CMD1 to MMC/SD-Card");
   CMD[0] = 0x41; //Command 1
   CMD[5] = 0xFF;
   retry=0;
   do
   { //retry 100 times to send CMD1 command 
     temp=Write_Command_MMC(CMD);
     retry++;
     if(retry==100) 
     { //time out
       return(INIT_CMD1_ERROR);//CMD1 Error!
     }
   } 
   while(temp!=0);
   
   Init_Flag=0; //Init is completed,clear the flag 
   
   SPI_CS_H;  //set MMC_Chip_Select to high 
   //send_s("SD CARD Init Suc!!");
   return(0x55); //All commands have been taken.
} 

//****************************************************************************
//Routine for reading data Registers of MMC/SD-Card
//Return 0 if no Error.
unsigned char MMC_Read_Block(unsigned char *CMD,unsigned char *Buffer,unsigned int Bytes)
//****************************************************************************
{  
   unsigned int i; 
   unsigned retry,temp;
    
   //Send Command CMD to MMC/SD-Card
   retry=0;
   do
   {  //Retry 100 times to send command.
      temp=Write_Command_MMC(CMD);
      retry++;
      if(retry==100) 
      {
        return(READ_BLOCK_ERROR); //block write Error!
      }
   }
   while(temp!=0); 
   			
   //Read Start Byte form MMC/SD-Card (FEh/Start Byte)
   while (Read_Byte_MMC()!=0xfe);
	
   //Write blocks(normal 512Bytes) to MMC/SD-Card
   for (i=0;i<Bytes;i++)
   {
      *Buffer++ = Read_Byte_MMC();
   }
   
   //CRC-Byte
   Read_Byte_MMC();//CRC - Byte 
   Read_Byte_MMC();//CRC - Byte
	
   //set MMC_Chip_Select to high (MMC/SD-Card invalid)
   SPI_CS_H;
   return(0);
}

//***************************************************************************
//Routine for reading CSD Registers from MMC/SD-Card (16Bytes)
//Return 0 if no Error.
unsigned char Read_CSD_MMC(unsigned char *Buffer)
//***************************************************************************
{	
   //Command for reading CSD Registers
   unsigned char CMD[] = {0x49,0x00,0x00,0x00,0x00,0xFF};
   unsigned char temp;
   temp=MMC_Read_Block(CMD,Buffer,16); //read 16 bytes

   return(temp);
}

//***************************************************************************
//Routine for reading CID Registers from MMC/SD-Card (16Bytes) 
//Return 0 if no Error.
unsigned char Read_CID_MMC(unsigned char *Buffer)
//***************************************************************************
{
   //Command for reading CID Registers
   unsigned char CMD[] = {0x4A,0x00,0x00,0x00,0x00,0xFF}; 
   unsigned char temp;
   temp=MMC_Read_Block(CMD,Buffer,16); //read 16 bytes

   return(temp);
}

//****************************************************************************
//returns the :
// 	size of the card in MB ( ret * 1024^2) == bytes
// 	sector count and multiplier MB are in u08 == C_SIZE / (2^(9-C_SIZE_MULT))
// 	name of the media 
void MMC_get_volume_info(void)
//****************************************************************************
{   //unsigned char i;
    //unsigned char c_temp[5];
    VOLUME_INFO_TYPE MMC_volume_Info,*vinf;
    //send_s("SD CARD Information Read!!");
    vinf=&MMC_volume_Info; //Init the pointoer;
	// read the CSD register
    Read_CSD_MMC(sectorBuffer.dat);
	// get the C_SIZE value. bits [73:62] of data
	// [73:72] == sectorBuffer.data[6] && 0x03
	// [71:64] == sectorBuffer.data[7]
	// [63:62] == sectorBuffer.data[8] && 0xc0
	vinf->sector_count = sectorBuffer.dat[6] & 0x03;
	vinf->sector_count <<= 8;
	vinf->sector_count += sectorBuffer.dat[7];
	vinf->sector_count <<= 2;
	vinf->sector_count += (sectorBuffer.dat[8] & 0xc0) >> 6;
		
	// get the val for C_SIZE_MULT. bits [49:47] of sectorBuffer.data
	// [49:48] == sectorBuffer.data[5] && 0x03
	// [47]    == sectorBuffer.data[4] && 0x80
	vinf->sector_multiply = sectorBuffer.dat[9] & 0x03;
	vinf->sector_multiply <<= 1;
	vinf->sector_multiply += (sectorBuffer.dat[10] & 0x80) >> 7;

	// work out the MBs
	// mega bytes in u08 == C_SIZE / (2^(9-C_SIZE_MULT))
	vinf->size_MB = vinf->sector_count >> (9-vinf->sector_multiply);
        //size = vinf->size_MB;   //��ʱ�洢
	// get the name of the card
	Read_CID_MMC(sectorBuffer.dat);
	vinf->name[0] = sectorBuffer.dat[3];
	vinf->name[1] = sectorBuffer.dat[4];
	vinf->name[2] = sectorBuffer.dat[5];
	vinf->name[3] = sectorBuffer.dat[6];
	vinf->name[4] = sectorBuffer.dat[7];
	vinf->name[5] = 0x00; //end flag
        ////////////////////////////////////////
        /*
        nn[0]=vinf->name[0];
        nn[1]=vinf->name[1];
        nn[2]=vinf->name[2];
        nn[3]=vinf->name[3];
        nn[4]=vinf->name[4];
        nn[5]=vinf->name[5];
        */
	//----------------------------------------------------------
	
    
	
}

//#define   EndWrite    '+'
//****************************************************************************
//Routine for writing a Block(512Byte) to MMC/SD-Card
//Return 0 if sector writing is completed.
unsigned char MMC_write_sector(unsigned long addr,unsigned char *Buffer)
//****************************************************************************
{  
   unsigned char tmp,retry;
   unsigned int i;
   //Command 24 is a writing blocks command for MMC/SD-Card.
   unsigned char CMD[] = {0x58,0x00,0x00,0x00,0x00,0xFF}; 
   //send_s("Write Sector Start!!");
   //EA=0; //clear all interrupt.
   //_DINT();
   addr = addr << 9; //addr = addr * 512
	
   CMD[1] = ((addr & 0xFF000000) >>24 );
   CMD[2] = ((addr & 0x00FF0000) >>16 );
   CMD[3] = ((addr & 0x0000FF00) >>8 );

   //Send Command CMD24 to MMC/SD-Card (Write 1 Block/512 Bytes)
   //send_s("Send Command CMD24 to MMC/SD-Card (Write 1 Block/512 Bytes)");
   retry=0;
   do
   {  //Retry 100 times to send command.
      tmp=Write_Command_MMC(CMD);
      retry++;
      if(retry==100) 
      { 
        return(tmp); //send commamd Error!
      }
   }
   while(tmp!=0); 
   
   //Before writing,send 100 clock to MMC/SD-Card
   //send_s("Before writing,send 100 clock to MMC/SD-Card");
   for (i=0;i<100;i++)
   {
      Read_Byte_MMC();
   }
	
   //Send Start Byte to MMC/SD-Card
   //send_s("Send Start Byte to MMC/SD-Card");
   Write_Byte_MMC(0xFE);	
	
   //Now send real data Bolck (512Bytes) to MMC/SD-Card
   //send_s("Now send real data Bolck (512Bytes) to MMC/SD-Card");
   for (i=0;i<512;i++)
   {
      Write_Byte_MMC(*Buffer++); //send 512 bytes to Card
   }
   //CRC-Byte 
   Write_Byte_MMC(0xFF); //Dummy CRC
   Write_Byte_MMC(0xFF); //CRC Code
   
    
   tmp=Read_Byte_MMC();   // read response
   if((tmp & 0x1F)!=0x05) // data block accepted ?
   {
     SPI_CS_H;
     return(WRITE_BLOCK_ERROR); //Error!
   }
   //Wait till MMC/SD-Card is not busy
   //send_s("Wait till MMC/SD-Card is not busy");
   while (Read_Byte_MMC()!=0xff){};
	
   //set MMC_Chip_Select to high (MMC/SD-Card Invalid)
   //send_s("set MMC_Chip_Select to high (MMC/SD-Card Invalid)");
   SPI_CS_H;
   //send_s("Write Sector suc!!");
   //_EINT();
   
   return(0);
} 

//***************************************************************************
//Return: [0]-success or something error!
unsigned char MMC_Start_Read_Sector(unsigned long sector)
//***************************************************************************
{  
   unsigned char retry;
   //Command 16 is reading Blocks from MMC/SD-Card
   unsigned char CMD[] = {0x51,0x00,0x00,0x00,0x00,0xFF}; 
   unsigned char temp;
   
   //EA=0; //clear all interrupt.
   //Address conversation(logic block address-->byte address)  
   sector = sector << 9; //sector = sector * 512

   CMD[1] = ((sector & 0xFF000000) >>24 );
   CMD[2] = ((sector & 0x00FF0000) >>16 );
   CMD[3] = ((sector & 0x0000FF00) >>8 );
   //Send Command CMD to MMC/SD-Card
   retry=0;
   do
   {  //Retry 100 times to send command.
      temp=Write_Command_MMC(CMD);
      retry++;
      if(retry==100) 
      {
        return(READ_BLOCK_ERROR); //block write Error!
      }
   }
   while(temp!=0); 
   			
   //Read Start Byte form MMC/SD-Card (FEh/Start Byte)
   //Now data is ready,you can read it out.
   while (Read_Byte_MMC() != 0xfe);
   return 0; //Open a sector successfully!
}

//***************************************************************************
void MMC_get_data(unsigned int Bytes,unsigned char *buffer) 
//***************************************************************************
{
   unsigned int j;
   //EA=0; //clear all interrupt.
   for (j=0;((j<Bytes) && (readPos<512));j++)
   {	
      *buffer++ = Read_Byte_MMC();
      readPos++; //read a byte,increase read position
   }
   if (readPos==512)  
   {  //CRC-Bytes
      Read_Byte_MMC();//CRC - Byte 
      Read_Byte_MMC();//CRC - Byte
      readPos=0;      //reset sector read offset 
      sectorPos++;    //Need to read next sector
      LBA_Opened=0;   //Set to 1 when a sector is opened.
      //set MMC_Chip_Select to high (MMC/SD-Card invalid)
      SPI_CS_H;;  //MMC disable
   }
}

//***************************************************************************
void MMC_get_data_LBA(unsigned long lba, unsigned int Bytes,unsigned char *buffer)
//***************************************************************************
{ //get data from lba address of MMC/SD-Card
  //If a new sector has to be read then move head
  if (readPos==0) MMC_Start_Read_Sector(lba); 
  MMC_get_data(Bytes,buffer);
}

//***************************************************************************
void MMC_LBA_Close()
//***************************************************************************
{  
   unsigned char temp[1];
   while((readPos!=0x00)|(LBA_Opened==1))
   { //read MMC till readPos==0x00
     MMC_get_data(1, temp); //dummy read,temp is a valid data.
   }  
}

//***************************************************************************
void MMC_GotoSectorOffset(unsigned long LBA,unsigned int offset)
//***************************************************************************
{  
   //Find the offset in the sector
   unsigned char temp[1];
   MMC_LBA_Close(); //close MMC when read a new sector(readPos=0)
   while (readPos<offset) MMC_get_data_LBA(LBA,1,temp); //go to offset  
}






