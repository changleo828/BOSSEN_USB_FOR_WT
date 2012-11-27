/* 2008.10.18
****************************************
**  Copyright  (C)  W.ch  1999-2009   **
**  Web:  http://www.winchiphead.com  **
****************************************
**  USB Host File Interface for CH376 **
**  TC2.0@PC, KC7.0@MCS51             **
****************************************
*/
/* CH376 �����ļ�ϵͳ�ӿ� */

/* MCS-51��Ƭ��C���Ե�U���ļ���дʾ������ */
/* ������������ʾ�г�ָ��Ŀ¼�µ������ļ����Լ���������/ö���ļ��� */

/* C51   CH376HFT.C */
/* LX51  CH376HFT.OBJ */
/* OHX51 CH376HFT */

#include <reg52.h>
#include <stdio.h>
#include <string.h>

#include "..\HAL.H"
#include "..\HAL_BASE.C"
#include "..\DEBUG.H"
#include "..\DEBUG.C"
#include "..\PARA_HW.C"		/* Ӳ�����߲������ӷ�ʽ */
//#include "..\PARA_SW.C"		/* ����ģ�Ⲣ�����ӷ�ʽ */
//#include "..\SPI_HW.C"		/* Ӳ��SPI���ӷ�ʽ */
//#include "..\SPI_SW.C"		/* ����ģ��SPI��ʽ */
//#include "..\UART_HW.C"		/* Ӳ���첽�������ӷ�ʽ */
#include "..\FILE_SYS.H"
#include "..\FILE_SYS.C"

typedef struct _FILE_NAME {
	UINT32	DirStartClust;				/* �ļ�����Ŀ¼����ʼ�غ� */
//	UINT32	Size;						/* �ļ����� */
	UINT8	Name[8+1+3+1];				/* �ļ���,��8+3�ֽ�,�ָ���,������,��Ϊδ�����ϼ�Ŀ¼�����������·�� */
	UINT8	Attr;						/* �ļ����� */
} FILE_NAME;
#define		MAX_FILE_COUNT		40
FILE_NAME	xdata	FileNameBuffer[ MAX_FILE_COUNT ];	/* �ļ����ṹ */
UINT16		FileCount;
UINT8		idata	buf[64];

/* ����:�о�ָ����ŵ�Ŀ¼�µ������ļ� */
UINT8	ListFile( UINT8 index )
/* �������index��ָĿ¼�ڽṹ�е���� */
{
	UINT8			s;
	P_FAT_DIR_INFO	pDir;
	PUINT8			pNameBuf;
	UINT32			CurrentDirStartClust;  /* ���浱ǰĿ¼����ʼ�غ�,���ڼӿ��ļ�ö�ٺʹ��ٶ� */
	CH376WriteVar32( VAR_START_CLUSTER, FileNameBuffer[ index ].DirStartClust );  /* ����ǰĿ¼���ϼ�Ŀ¼����ʼ�غ�����Ϊ��ǰ�غ�,�൱�ڴ��ϼ�Ŀ¼ */
	printf( "List Directory: %s\n", FileNameBuffer[ index ].Name );  /* ��ʾ��ǰҪ�оٵ�Ŀ¼�� */
	s = CH376FileOpen( FileNameBuffer[ index ].Name );  /* ��Ŀ¼,��Ϊ�˻�ȡĿ¼����ʼ�غ�������ٶ� */
	if ( s == USB_INT_SUCCESS ) return( ERR_FOUND_NAME );  /* Ӧ���Ǵ���Ŀ¼,���Ƿ��ؽ���Ǵ����ļ� */
	else if ( s != ERR_OPEN_DIR ) return( s );
	if ( index ) CurrentDirStartClust = CH376ReadVar32( VAR_START_CLUSTER );  /* ���Ǹ�Ŀ¼,��ȡĿ¼����ʼ�غ� */
	else CurrentDirStartClust = 0;  /* �Ǹ�Ŀ¼ */
	CH376FileClose( FALSE );  /* ���ڸ�Ŀ¼һ��Ҫ�ر� */
	CH376WriteVar32( VAR_START_CLUSTER, CurrentDirStartClust );  /* ��ǰĿ¼����ʼ�غ�,�൱�ڴ򿪵�ǰĿ¼ */
	CH376SetFileName( "*" );  /* ���ý�Ҫ�������ļ����ļ���,ͨ���֧�������ļ�����Ŀ¼ */
	xWriteCH376Cmd( CMD0H_FILE_OPEN );  /* ö���ļ���Ŀ¼ */
	xEndCH376Cmd( );
	while ( 1 ) {
		s = Wait376Interrupt( );
		if ( s == USB_INT_DISK_READ ) {  /* �������ݶ��� */
/* ���ļ�ö�ٹ�����,����ִ���������ܲ����жϵĲ�������,����,�����Ҫ��ȡ���ļ���,��ô���Խ�ö�ٳ����ļ������沢��ö�ٽ������ȡ�䳤�ļ��� */
			CH376ReadBlock( buf );  /* ��ȡö�ٵ����ļ���FAT_DIR_INFO�ṹ,���س�������sizeof( FAT_DIR_INFO ) */
			xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* ����ö���ļ���Ŀ¼,�ȷ�����һ�������ٷ������ж��������ݿ�����CH376�뵥Ƭ���ֱ�ͬʱ����,����ٶ� */
			xEndCH376Cmd( );
			pDir = (P_FAT_DIR_INFO)buf;  /* ��ǰ�ļ�Ŀ¼��Ϣ */
			if ( pDir -> DIR_Name[0] != '.' ) {  /* ���Ǳ��������ϼ�Ŀ¼�������,������붪�������� */
				if ( pDir -> DIR_Name[0] == 0x05 ) pDir -> DIR_Name[0] = 0xE5;  /* �����ַ��滻 */
				if ( pDir -> DIR_Name[8] == 'H' && pDir -> DIR_Name[9] == ' '  /* �Ƚ��ļ���չ�������ļ����͵ķ��� */
					|| pDir -> DIR_Name[8] == 'E' && pDir -> DIR_Name[9] == 'X' && pDir -> DIR_Name[10] == 'E' ) {
					printf( "This is a .H or .EXE file\n" );
				}
				if ( FileCount < MAX_FILE_COUNT ) {  /* �ļ����ṹ�������㹻 */
					pNameBuf = & FileNameBuffer[ FileCount ].Name;  /* �ļ����ṹ�е��ļ��������� */
					for ( s = 0; s < 11; s ++ ) {  /* �����ļ���,����Ϊ11���ַ� */
						if ( pDir -> DIR_Name[ s ] != 0x20 ) {  /* ��Ч�ַ� */
							if ( s == 8 ) {  /* ������չ�� */
								*pNameBuf = '.';  /* �ָ��� */
								pNameBuf ++;
							}
							*pNameBuf = pDir -> DIR_Name[ s ];  /* �����ļ�����һ���ַ� */
							pNameBuf ++;
						}
					}
					*pNameBuf = 0;  /* ��ǰ�ļ�������·���Ľ����� */
					FileNameBuffer[ FileCount ].DirStartClust = CurrentDirStartClust;  /* ��¼��ǰĿ¼����ʼ�غ�,���ڼӿ��ļ����ٶ� */
					FileNameBuffer[ FileCount ].Attr = pDir -> DIR_Attr;  /* ��¼�ļ����� */
					if ( pDir -> DIR_Attr & ATTR_DIRECTORY ) printf( "Dir %4d#: %s\n", FileCount, FileNameBuffer[ FileCount ].Name );  /* �ж���Ŀ¼�� */
					else printf( "File%4d#: %s\n", FileCount, FileNameBuffer[ FileCount ].Name );  /* �ж����ļ��� */
					FileCount ++;  /* ��Ŀ¼���� */
				}
				else {  /* �ļ����ṹ������̫С,�ṹ�������� */
					printf( "FileName Structure Full\n" );
					s = Wait376Interrupt( );
					CH376EndDirInfo( );  /* ��ȡ��FAT_DIR_INFO�ṹ */
					break;  /* ǿ����ֹö�� */
				}
			}
		}
		else {
			if ( s == ERR_MISS_FILE ) s = USB_INT_SUCCESS;  /* û���ҵ������ƥ���ļ� */
			break;
		}
	}
/*	if ( s == USB_INT_SUCCESS ) return( s );*/  /* �����ɹ� */
	return( s );
}

UINT8	ListAll( void )  /* �Թ�����ȵ��㷨ö������U���е������ļ���Ŀ¼ */
{
	UINT8	s;
	UINT16	OldFileCount;
	UINT16	RealReadCount;
	FileNameBuffer[ 0 ].Name[0] = '/';  /* ��Ŀ¼,������·����,����Ŀ¼�Ǿ���·��֮�ⶼ�����·�� */
	FileNameBuffer[ 0 ].Name[1] = 0;
	FileNameBuffer[ 0 ].DirStartClust = 0;  /* ��Ŀ¼����ʼ�غ� */
	FileNameBuffer[ 0 ].Attr = ATTR_DIRECTORY;  /* ��Ŀ¼Ҳ��Ŀ¼,��Ϊ��һ����¼���� */
	for ( OldFileCount = 0, FileCount = 1; OldFileCount < FileCount; OldFileCount ++ ) {  /* ������ö�ٵ����ļ����ṹδ���з���,FileCount���ڱ仯֮�� */
		if ( FileNameBuffer[ OldFileCount ].Attr & ATTR_DIRECTORY ) {  /* ��Ŀ¼���������������� */
			s = ListFile( OldFileCount );  /* ö��Ŀ¼,��¼���浽�ṹ��,FileCount���ܻ�ı� */
			if ( s != USB_INT_SUCCESS ) return( s );
		}
	}

/* U���е��ļ���Ŀ¼ȫ��ö�����,���濪ʼ���ݽṹ��¼���δ��ļ� */
	printf( "Total file&dir = %d, Open every file:\n", FileCount );
	for ( OldFileCount = 0; OldFileCount < FileCount; OldFileCount ++ ) {
		if ( ( FileNameBuffer[ OldFileCount ].Attr & ATTR_DIRECTORY ) == 0 ) {  /* ���ļ����,Ŀ¼������ */
			printf( "Open file: %s\n", FileNameBuffer[ OldFileCount ].Name );
			CH376WriteVar32( VAR_START_CLUSTER, FileNameBuffer[ OldFileCount ].DirStartClust );  /* ����ǰ�ļ������ϼ�Ŀ¼����ʼ�غ�����Ϊ��ǰ�غ�,�൱�ڴ��ϼ�Ŀ¼ */
			s = CH376FileOpen( FileNameBuffer[ OldFileCount ].Name );  /* ���ļ� */
			if ( s == USB_INT_SUCCESS ) {  /* �ɹ������ļ� */
				s = CH376ByteRead( buf, sizeof(buf), &RealReadCount );  /* ���ֽ�Ϊ��λ�ӵ�ǰλ�ö�ȡ���ݿ� */
				if ( RealReadCount != sizeof(buf) ) printf( "File size = %d\n", RealReadCount );  /* �ļ����Ȳ���sizeof(buf) */
				CH376FileClose( FALSE );  /* ����д������������ر� */
			}
		}
	}
}

main( ) {
	UINT8	i, s;
	mDelaymS( 100 );  /* ��ʱ100���� */
	mInitSTDIO( );  /* Ϊ���ü����ͨ�����ڼ����ʾ���� */
	printf( "Start\n" );

	s = mInitCH376Host( );  /* ��ʼ��CH376 */
	mStopIfError( s );
/* ������·��ʼ�� */

	while ( 1 ) {
		printf( "Wait Udisk/SD\n" );
		while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) {  /* ���U���Ƿ�����,�ȴ�U�̲���,����SD��,�����ɵ�Ƭ��ֱ�Ӳ�ѯSD�����Ĳ��״̬���� */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );  /* ��ʱ,��ѡ����,�е�USB�洢����Ҫ��ʮ�������ʱ */

/* ���ڼ�⵽USB�豸��,���ȴ�100*50mS,��Ҫ�����ЩMP3̫��,���ڼ�⵽USB�豸��������DISK_MOUNTED��,���ȴ�5*50mS,��Ҫ���DiskReady������ */
		for ( i = 0; i < 100; i ++ ) {  /* ��ȴ�ʱ��,100*50mS */
			mDelaymS( 50 );
			printf( "Ready ?\n" );
			s = CH376DiskMount( );  /* ��ʼ�����̲����Դ����Ƿ���� */
			if ( s == USB_INT_SUCCESS ) break;  /* ׼���� */
			else if ( s == ERR_DISK_DISCON ) break;  /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
			if ( CH376GetDiskStatus( ) >= DEF_DISK_MOUNTED && i >= 5 ) break;  /* �е�U�����Ƿ���δ׼����,�������Ժ���,ֻҪ�佨������MOUNTED�ҳ���5*50mS */
		}
		if ( s == ERR_DISK_DISCON ) {  /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
			printf( "Device gone\n" );
			continue;
		}
		if ( CH376GetDiskStatus( ) < DEF_DISK_MOUNTED ) {  /* δ֪USB�豸,����USB���̡���ӡ���� */
			printf( "Unknown device\n" );
			goto UnknownUsbDevice;
		}
		i = CH376ReadBlock( buf );  /* �����Ҫ,���Զ�ȡ���ݿ�CH376_CMD_DATA.DiskMountInq,���س��� */
		if ( i == sizeof( INQUIRY_DATA ) ) {  /* U�̵ĳ��̺Ͳ�Ʒ��Ϣ */
			buf[ i ] = 0;
			printf( "UdiskInfo: %s\n", ((P_INQUIRY_DATA)buf) -> VendorIdStr );
		}
		mDelaymS( 20 );

		printf( "List all file \n" );
		s = ListAll( );  /* ö������U���е������ļ���Ŀ¼ */
		mStopIfError( s );

UnknownUsbDevice:
		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* ���U���Ƿ�����,�ȴ�U�̰γ� */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );
	}
}