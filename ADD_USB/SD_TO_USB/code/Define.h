/*============================================ 头文件说明 ============================================================
全局变量定义
================================================ END ===============================================================*/

////////////////////////////////////////////// END ////////////////////////////////////////////////////////



#define         uchar     unsigned char
#define 	uint 	  unsigned int

#define		LED1ON	  P1OUT |= BIT4
#define		LED1OFF	  P1OUT &=~ BIT4
#define         LED1_Flicker  P1OUT ^= BIT4
/*
#define		A0_H	  P1OUT |= BIT0
#define		A0_L	  P1OUT &=~ BIT0
#define		A1_H	  P1OUT |= BIT1
#define		A1_L	  P1OUT &=~ BIT1
#define		A2_H	  P1OUT |= BIT2
#define		A2_L	  P1OUT &=~ BIT2
#define		A3_H	  P1OUT |= BIT3
#define		A3_L	  P1OUT &=~ BIT3
*/

//*功能选择脚舍掉不用。
/*
#define		FeatureSelection1_H	  P1OUT |= BIT2
#define		FeatureSelection1_L	  P1OUT &=~ BIT2
#define		FeatureSelection0_H	  P1OUT |= BIT3
#define		FeatureSelection0_L	  P1OUT &=~ BIT3
*/
#define		FeatureSelection1_H	  P1OUT |= BIT3
#define		FeatureSelection1_L	  P1OUT &=~ BIT3
#define		FeatureSelection0_H	  P1OUT |= BIT2
#define		FeatureSelection0_L	  P1OUT &=~ BIT2
//*/
#define		P15_H	  P1OUT |= BIT5
#define		P15_L	  P1OUT &=~ BIT5
#define		P16_H	  P1OUT |= BIT6
#define		P16_L	  P1OUT &=~ BIT6
#define         P15_Flicker   P1OUT ^= BIT5
#define         P32_Flicker   P3OUT ^= BIT2
#define         P33_Flicker   P3OUT ^= BIT3
//#define         P16_Flicker   P1OUT ^= BIT6
#define         WorkOnControl_H   P1OUT &=~ BIT6
#define         WorkOnControl_L   P1OUT |= BIT6

#define		FrequencyPointSelection1_H	  P5OUT |= BIT5
#define		FrequencyPointSelection1_L	  P5OUT &=~ BIT5
#define		FrequencyPointSelection0_H	  P5OUT |= BIT6
#define		FrequencyPointSelection0_L	  P5OUT &=~ BIT6

#define         AVR_1_H     P5OUT |= BIT5
#define         AVR_1_L     P5OUT &=~ BIT5
#define         AVR_2_H     P5OUT |= BIT6
#define         AVR_2_L     P5OUT &=~ BIT6

#define         QIANYABAOHU         P6OUT &=~ BIT7
#define         QIANYABAOHU_Normal  P6OUT |= BIT7   //  欠压保护定义

/*
#define         GUOLIU              P6OUT &=~ BIT6
#define         GUOLIU_Normal       P6OUT |= BIT6   //  过流保护定义
*/
#define         GUOLIU              P6OUT |= BIT6
#define         GUOLIU_Normal       P6OUT &=~ BIT6   //  过流保护定义
uchar           GUOLIU_Flag = 0;  //  发生过流的标识，0：正常， 1：过流。
//////////////////////////////////////////////////////////////////////////////////////////////

uchar           MCU_Start_flag      =     0;//单片机开机标志

///////////////////////////////////////////////   定时器A   /////////////////////////////////////////////////////////
uint            AcquisitionTime     =       0;      //  “采集”换屏显示的计时
uint            WorkOnTime          =       0;      //  "工作开"的换屏显示计时
uint 		time	            =       0;  //中断次数计数
uchar 		second	            =	    0;  // 秒 的计数
uint		transmit_count      =       0;  //  频率发射器计时
uchar           WorkOn_Transmit     =       0;  //  按压"工作开"后,打频时间计数
uchar           keylong_flag        =       0;  //  长按键标志
#define		keylongtime    600

////////////////////////////////////////////////   END   ////////////////////////////////////////////////////////////

/////////////////////////////////////////////// 矩阵式键盘 ////////////////////////////////////////////////////////
uchar	keyboard_value	        =	0; 	//存储按键值，表示第几个按键
uchar	key_save		=	0;	//
uchar	keyon			=	0;	//按键一旦存在，此值就为1
uint   	keytime			=	0;	//长短按键计时
uint   	keytime_save	        =	0;	//长短按键计时存储
uchar   keytime_start	        =	0;	//按键计时开始标志。0：停止，1：开始   
uchar	Rst_flag		=	0;	//”复位“按键，0-未按下，1-按下
uchar	ZengYi_flag		=	0;	//”增益“按键，0-未按下，1-按下；并且，按下除了相关的三个按键外，该变量变为0
char	ZengYi_disp		=	0;	//增益，屏显数值
uchar	Setting_flag	        =	0;	//按键 设置。0:未按下；1：点亮1/2AB；2：点亮1/2MN；3：点亮"功能选择"，；4：点亮"频点选择"，；5：点亮"时窗设置"，；6：点亮"点号"，。
uchar	Position_flag	        =	0;	//按键 --点号
uint	AB_data			=	60;	//点亮1/2AB，默认值0006.000 m  
uchar	AB_shanshuo		=	4;
uchar   AB_shanshuo_cmp         =       4;      //为防止屏幕闪烁位同时出现两个，此值的初值和 AB_shanshuo 一样
uchar   MN_shanshuo_cmp         =       4;
uint	MN_data			=	40;	//点亮1/2MN，默认值0003.000 m
uchar	FeatureSelection_data   =       0;      //点亮"功能选择"，默认值0000.000
uchar   FrequencyPointSelection_data=0;         //点亮"频点选择"，默认值0000.000；；；该值范围0--3
uchar	TimeWindow_data         =       0;      //  0 1 2 3 
signed char    Position_data[7]={0,0,0,1,0,0,1};//此数组仅供“点号”的显示用，并非存储的点号值----前面3个是"点号"，后面3个是"桩号"
                                                //从左往右数，第一个是巷道号，
                                                //功能选择==0时，7个数字从左往右依次是：巷道号、数字0、O点号十位、O点号个位、数字0、数字0、A点号； O点号范围1-99，A点号范围1-3 。
struct PositionRecord
{
  uchar PositionOu_InFeatureSelection_0;      //点号O（这个是字母，不是数字）在功能选择0时的临时存储值；范围 1-99
  uchar PositionA_InFeatureSelection_0;       //点号A在功能选择0时的临时存储值；范围 1，2，3.
  uint  PositionOu_InFeatureSelection_1;      //点号O（这个是字母，不是数字）在功能选择1时的临时存储值；范围 1-999
  uint  PositionA_InFeatureSelection_1;       //点号A在功能选择1时的临时存储值；范围 1-999
  uint  PositionOu_InFeatureSelection_2;
  uint  PositionA_InFeatureSelection_2; 
  uint  PositionOu_InFeatureSelection_3;
  uint  PositionA_InFeatureSelection_3; 
  uchar NumberOfXiangDao;                     //巷道号，范围 0-9
};
struct PositionRecord PositionRecordRun;      //此变量应在系统上电后进行初始化赋值。
#define Displacement              2           //单击“点号”按键，表示闪烁数字移位
#define ResumeRecordForPosition   3           //恢复上次关机时的点号记录----长按“点号”按键

uchar	Position_shanshuo       =       3;
uchar	Position_shanshuo_cmp   =       3;
uchar   Setting_up		=	0;
uchar   Setting_down	        =	0;  //短按为1，长按为2
uchar   Acquisition_flag        =       0;      //  采集 标志位。0：停止，1：开始
uchar   Vol_Calculate_One_Time  =       0;  //  “采集”或者“工作开”时，显示电压只要第一次采的值
uchar   Cur_Calculate_One_Time  =       0;  //  “工作开”时，显示电流只要第一次采的值
uchar   WorkOn_flag             =       0;      //  按键”工作开“。0：停止，1：开始
uchar   WorkOn_Transmit_STA     =       0;      //  频率发射开始标志
uchar   Save_WorkStop_flag      =       0;  //按压“存储”，要行使“工作停”的功能。

////////////////////////////////////////////////   END ////////////////////////////////////////////////////////////

///////////////////////////////////////// 12位AD变量定义 ///////////////////////////////////////////////////
//单片机内部AD
uint    A2result                =       0;      //  存储 A2通道AD的平均值
uint    A4result                =       0;
uint    A0result                =       0; //信号溢出检测 第 1 路
uint    A1result                =       0; //信号溢出检测 第 2 路



////////////////////////////////////////////////  END //////////////////////////////////////////////////////

///////////////////////////////////////// 屏幕变量定义 ////////////////////////////////////////////////////
const uchar disp_data[10][2]={//数字显示,适用于屏幕中间1～7个大的“8”
{5,15},	 //--0	 {E+F+G ，A+B+C+D}
{0,6},	 //--1
{3,13},	 //--2
{2,15},	 //--3
{6,6},	 //--4
{6,11},	 //--5
{7,11},	 //--6
{0,14},	 //--7
{7,15},	 //--8
{6,15} 	 //--9
};
const uchar disp_data_addr[7][2]={//	屏幕上每个“8”的地址，一共7个
{1,2},	 //--第1个	{低地址,高地址}
{3,4},	 //--2
{5,6},	 //--3
{7,8},	 //--4
{19,18}, //--5 {高地址,低地址}
{17,16}, //--6
{15,14}	 //--7 
};
////////////////////////////////////////////// END ////////////////////////////////////////////////////////

///////////////////////////////////////// 存储器变量定义 //////////////////////////////////////////////////
uchar     Save_Flag       =   0;  //  0：未按压“保存”按键；1：按压“保存”按键 。
uint      SaveTimeCount   =   0;  //  存储器存储计时

#define   Parameter       0x30
#define   SaveLength      26

uchar     SaveBuffer[SaveLength]; //  每次按压“存储”键，存储的内容；
#define   Demand_end_flag  0x08 //数组存储的结束标志
#define   DemandLength    SaveLength
//uchar     Demand[DemandLength]={Demand_end_flag,};  //  查询的内容
struct BufferFlagForDemand  //  查询
{
  unsigned int  RecordNumber;//"查询"屏幕上所对对应的第几条记录，"查询"按键标识 。若为 0 ，则无"查询"按键
  unsigned long RecordNumber_Bytes;//第几条记录所对应的字节总数. 1条记录对应的字节总数为 1*DemandLength，2条为 2*DemandLength 。
  unsigned int  LastRecordNumber;//是 RecordNumber 的上一次记录，可以大于等于或者小于 RecordNumber
  unsigned int  OffsetInSectorOfTheFile;//文件的某个扇区中的字节数
  unsigned int  OffsetInBuffer_Start_RecordNumber;//每条记录所对应的 数组FAT32_Buffer 的下标；---最开始的那个下标
  unsigned int  OffsetInBuffer_RecordNumber;//每条记录所对应的 数组FAT32_Buffer 的下标；
  unsigned int  TheSecondPartForDemand_Sector;//屏幕上显示的“查询”内容所占用的第二个个扇区
  unsigned long TheSecondPartForDemand_Cluster;//
  unsigned int  TheFirstPartForDemand_Sector;//屏幕上显示的“查询”内容所占用的第1个扇区
  unsigned long TheFirstPartForDemand_Cluster;//
  //unsigned char TheLastSectorInFile;//文件的最后一个 扇区//逻辑扇区，并非物理扇区。。范围：0-7
  //unsigned long TheLastClusterInFile;//文件的最后一个 簇
  //unsigned long BytesAfterTheSector;//此扇区后面的所有扇区的占用字节总数
  unsigned char NoRecord_flag_InFile;//该文件中已无有效记录。刚按压查询按键时，此值为0；. 0--查询文件有效记录没有全部删除；1--全部删除
  unsigned char Demand[ DemandLength ];
};
struct BufferFlagForDemand FatBuffer;
uchar   Delete_flag = 0;  //删除按键标识----在按键函数里赋值为1，在删除的相关函数里清0
                          //任何按键不可以对其清零


uchar     FirstCreatTheFile_flag  = 0;  //  第一次创建该文件
#define   HeadingLength   34  //  标题行的字符数，即数组 Heading[] 的长度
uchar     Heading[HeadingLength];
////////////////////////////////////////////// END ////////////////////////////////////////////////////////

//////////////////////////////////// “工作开”与“采集”变量定义 /////////////////////////////////////////
uchar  mmma[8];// 存储电流
uchar  mmmm[8];// 缓存电压
////////////////////////////////////////////// END ////////////////////////////////////////////////////////
/*
#define   BIAOXIAO        ( (P1IN&BIT7)&(0x80) )  //
uchar     BIAOXIAO_flag   =   0;  //标校按键标志位。0：无按键操作，1：第一次按下；2：第二次按下；0：第三次按下
#define   BIAOXIAO_save        0XBA00  //标校情况保存地址，也是本程序中，使用flash存储的起始地址。

#define   CalibrationCurrent_1st  1 //  进入电流标校：第一个点
#define   CalibrationCurrent_2nd  2
#define   CalibrationCurrent_3rd  3
#define   CalibrationCurrent_4th  4
#define   CalibrationVoltage      5 //  进入电压标校
#define   Calibration_TheLastFlag 5 //  标校状态的最后一个标识，如果 BIAOXIAO_flag 大于此值，则退出标校。

long biaojiao_current  = 0;  //  标校电流值
unsigned long biaojiao_current_staic  = 0;  //  标校电流值,静
uchar     biaojiao_cur_sign = 0;//标校电流符号
uint      biaojiao_cur_chazhi = 0;//标校电流的差值---单位：微安

signed long deta          =   0;  //  电压标校地绝对值
uchar     memory[64];                  // memory[0]----电压标校地正负号
                                       // memory[1]----电压标校差值绝对值，低两位，0-99
                                       // memory[2]----电压标校差值绝对值，高两位，0-99
*/
signed long XiuZheng_Result = 0;  //  经过函数变换后的结果，在AD中断里得到结果；此值又经过函数 void CurrentShow( signed long  current_xiuzheng ) 被使用。
#define XiuZhengCount   17   //  计算电流修正值的最大次数
uchar   iXiuZhengCount  = 0;//  每计算一次电流修正值，该值加 1
signed long long sum_XiuZheng  = 0;
#define   current    (long)( (((float)(MCU_Ref)*(float)(A4result)*1000.0)/((float)(4095)))/((float)(Current_R)) )  //  直接换算成电流


#define ZengYi_MAX  10  //增益的最多级数,0---9的屏显值，
struct AutoZengYi
{
  unsigned char which_grade_yichu ;//哪一级溢出
  unsigned char NO1_GAIN;//第一级增益，若无溢出，则为0，否则==1
  unsigned char NO2_GAIN;
  unsigned char NO3_GAIN;
  unsigned char loop;//循环
  unsigned char iloop;
  unsigned char ZengYi_Ever_Used ; // 1）、增益按键在开机后是否被按下，若无，则==0 ；按下，==1；
                                   // 2）、在开机状态下，一旦按压“复位”键，使 ZengYi_Ever_Used 必回0
};
struct AutoZengYi AutoZengYiRun;

/////////////////////////////////////////////////
//