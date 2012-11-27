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
/* ������������ʾ�����ļ�Ŀ¼��FAT_DIR_INFO,����:�޸��ļ���,�����ļ��Ĵ������ں�ʱ��� */

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

UINT8		idata	buf[64];

/* �޸�ָ���ļ����ļ���,�����C�ļ����޸�ΪTXT�ļ� */
/* �������:   iOldName ԭʼ�ļ��� */
/* ����״̬��: USB_INT_SUCCESS = �޸��ļ����ɹ�,
               ����״̬��ο�CH376INC.H */
UINT8	RenameFileName( PUINT8 iOldName )
{
	UINT8			s;
	P_FAT_DIR_INFO	pDir;
	s = CH376FileOpen( iOldName );  /* ��ԭ�ļ� */
	if ( s == USB_INT_SUCCESS ) {
		/* �ļ���д������... */
		s = CH376DirInfoRead( );  /* ��ȡ��ǰ�ļ���Ŀ¼��ϢFAT_DIR_INFO,��������ݵ����ڴ��� */
		if ( s == USB_INT_SUCCESS ) {
			CH376ReadBlock( buf );  /* ���ڴ滺������ȡFAT_DIR_INFO���ݿ�,���س�������sizeof(FAT_DIR_INFO) */
			pDir = (P_FAT_DIR_INFO)buf;  /* ��ǰ�ļ�Ŀ¼��Ϣ */
			if ( pDir -> DIR_Name[8] == 'C' && pDir -> DIR_Name[9] == ' ' ) {  /* �ļ���չ����C */
				pDir -> DIR_Name[8] = 'T';  /* �޸��ļ���չ��ΪTXT */
				pDir -> DIR_Name[9] = 'X';  /* ͬ�����������޸��ļ����� */
				pDir -> DIR_Name[10] = 'T';
				s = CH376DirInfoRead( );  /* ��ȡ��ǰ�ļ���Ŀ¼��ϢFAT_DIR_INFO,��������ݵ����ڴ��� */
				if ( s == USB_INT_SUCCESS ) {  /* ��ǰ�ļ���Ŀ¼��ϢFAT_DIR_INFO�Ѿ������ڴ滺���� */
					CH376WriteOfsBlock( &buf[8], 8, 3 );  /* ���ڲ�������ƫ�Ƶ�ַ8(�������ļ���)д��3���ֽ�(�޸ĵ���չ��) */
					s = CH376DirInfoSave( );  /* �����ļ���Ŀ¼��Ϣ */
					if ( s == USB_INT_SUCCESS ) {  /* �ɹ��޸Ĳ����� */
						/* �ļ���д������... */
					}
				}
			}
			else {  /* �ļ���չ������C */
			}
		}
		CH376FileClose( FALSE );  /* �ر��ļ� */
	}
	return( s );
}

/* Ϊָ���ļ����ô������ں�ʱ�� */
/* �������:   ԭʼ�ļ�����iFileName��, �µĴ������ں�ʱ��: iCreateDate, iCreateTime */
/* ����״̬��: USB_INT_SUCCESS = ���óɹ�,
               ����״̬��ο�CH376INC.H */
UINT8	SetFileCreateTime( PUINT8 iFileName, UINT16 iCreateDate, UINT16 iCreateTime )
{
	UINT8	s;
	s = CH376FileOpen( iFileName );  /* ���ļ� */
	if ( s == USB_INT_SUCCESS ) {
		/* �ļ���д������... */
		s = CH376DirInfoRead( );  /* ��ȡ��ǰ�ļ���Ŀ¼��ϢFAT_DIR_INFO,��������ݵ����ڴ��� */
		if ( s == USB_INT_SUCCESS ) {
			buf[0] = (UINT8)iCreateTime;  /* �ļ�������ʱ��,��8λ��ǰ */
			buf[1] = (UINT8)(iCreateTime>>8);  /* �ļ�������ʱ��,��8λ�ں� */
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_CrtTime ), 2 );  /* ���ڲ�������ƫ�Ƶ�ַDIR_CrtTimeд��2���ֽ� */
			buf[0] = (UINT8)iCreateDate;  /* �ļ�����������,��8λ��ǰ */
			buf[1] = (UINT8)(iCreateDate>>8);  /* �ļ�����������,��8λ�ں� */
			CH376WriteOfsBlock( buf, STRUCT_OFFSET( FAT_DIR_INFO, DIR_CrtDate ), 2 );  /* ���ڲ�������ƫ�Ƶ�ַDIR_CrtDateд��2���ֽ� */
			s = CH376DirInfoSave( );  /* �����ļ���Ŀ¼��Ϣ */
			if ( s == USB_INT_SUCCESS ) {  /* �ɹ��޸Ĳ����� */
				/* �ļ���д������... */
			}
		}
		CH376FileClose( FALSE );  /* �ر��ļ� */
	}
	return( s );
}

main( ) {
	UINT8	s;

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

/* ���ڼ�⵽USB�豸��,���ȴ�10*50mS */
		for ( s = 0; s < 10; s ++ ) {  /* ��ȴ�ʱ��,10*50mS */
			mDelaymS( 50 );
			printf( "Ready ?\n" );
			if ( CH376DiskMount( ) == USB_INT_SUCCESS ) break;  /* ��ʼ�����̲����Դ����Ƿ���� */
		}
//		s = CH376ReadBlock( buf );  /* �����Ҫ,���Զ�ȡ���ݿ�CH376_CMD_DATA.DiskMountInq,���س��� */
//		if ( s == sizeof( INQUIRY_DATA ) ) {  /* U�̵ĳ��̺Ͳ�Ʒ��Ϣ */
//			buf[ s ] = 0;
//			printf( "UdiskInfo: %s\n", ((P_INQUIRY_DATA)buf) -> VendorIdStr );
//		}

		printf( "Open and rename CH376HFT.C to CH376HFT.TXT \n" );
		s = RenameFileName( "/CH376HFT.C" );  /* �޸��ļ���, ���ļ��ڸ�Ŀ¼��, C�ļ� => TXT�ļ� */
		if ( s == ERR_MISS_FILE ) printf( "File /CH376HFT.C not found !\n" );
		mStopIfError( s );
		printf( "Set file create date & time to 2004.06.08 15:39:20 \n" );
		s = SetFileCreateTime( "/CH376HFT.TXT", MAKE_FILE_DATE( 2004, 6, 8 ), MAKE_FILE_TIME( 15, 39, 20 ) );  /* Ϊָ���ļ����ô������ں�ʱ�� */
		mStopIfError( s );

		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* ���U���Ƿ�����,�ȴ�U�̰γ� */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );
	}
}