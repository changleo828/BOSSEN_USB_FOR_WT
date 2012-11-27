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
/* ������������ʾ���U���Ƿ�д����,��ʾģ�������˵İ�ȫ�Ƴ�,Ҳ���Բο��������д����������� */

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

/* ���U���Ƿ�д����, ����USB_INT_SUCCESS˵������д,����0xFF˵��ֻ��/д����,��������ֵ˵���Ǵ������ */
/* ����BulkOnly����Э���������Բο�������Ӵ���,�޸�ǰ�����˽�USB MassStorage��SCSI�淶 */
UINT8	IsDiskWriteProtect( void )
{
	UINT8			s, SysBaseInfo;
	P_BULK_ONLY_CBW	pCbw;
	SysBaseInfo = CH376ReadVar8( VAR_SYS_BASE_INFO );  /* ��ǰϵͳ�Ļ�����Ϣ */
	pCbw = (P_BULK_ONLY_CBW)buf;
	for ( s = 0; s != sizeof( pCbw -> CBW_CB_Buf ); s ++ ) pCbw -> CBW_CB_Buf[ s ] = 0;  /* Ĭ����0 */
	pCbw -> CBW_DataLen0 = 0x10;  /* ���ݴ��䳤�� */
	pCbw -> CBW_Flag = 0x80;  /* ���䷽��Ϊ���� */
	if ( SysBaseInfo & 0x40 ) {  /* SubClass-Code������6 */
		pCbw -> CBW_CB_Len = 10;  /* �����ĳ��� */
		pCbw -> CBW_CB_Buf[0] = 0x5A;  /* ��������ֽ�, MODE SENSE(10) */
		pCbw -> CBW_CB_Buf[2] = 0x3F;
		pCbw -> CBW_CB_Buf[8] = 0x10;
	}
	else {  /* SubClass-Code�����Ϊ6 */
		pCbw -> CBW_CB_Len = 6;  /* �����ĳ��� */
		pCbw -> CBW_CB_Buf[0] = 0x1A;  /* ��������ֽ�, MODE SENSE(6) */
		pCbw -> CBW_CB_Buf[2] = 0x3F;
		pCbw -> CBW_CB_Buf[4] = 0x10;
	}
	CH376WriteHostBlock( (PUINT8)pCbw, sizeof( BULK_ONLY_CBW ) );  /* ��USB�����˵�ķ��ͻ�����д�����ݿ�,ʣ�ಿ��CH376�Զ�� */
	s = CH376SendCmdWaitInt( CMD0H_DISK_BOC_CMD );  /* ��U��ִ��BulkOnly����Э�� */
	if ( s == USB_INT_SUCCESS ) {
		s = CH376ReadBlock( buf );  /* �ӵ�ǰ�����˵�Ľ��ջ�������ȡ���ݿ�,���س��� */
		if ( s > 3 ) {  /* MODE SENSE��������ݵĳ�����Ч */
			if ( SysBaseInfo & 0x40 ) s = buf[3];  /* MODE SENSE(10), device specific parameter */
			else s = buf[2];  /* MODE SENSE(6), device specific parameter */
			if ( s & 0x80 ) return( 0xFF );  /* U��д���� */
			else return( USB_INT_SUCCESS );  /* U��û��д���� */
		}
		return( USB_INT_DISK_ERR );
	}
	CH376DiskReqSense( );  /* ���USB�洢������ */
	return( s );
}

/* ��ȫ�Ƴ�U��, ����USB_INT_SUCCESS˵�����԰�ȫ�Ƴ�,����˵�����ܰ�ȫ�Ƴ�,ֻ��ǿ���Ƴ� */
/* �ڲ�����U��׼�����û��γ�U��ǰ����, ��ֹ�û�����γ�U�̶�ʧ���� */
UINT8	SafeRemoveDisk( void )
{
	UINT8	i, s;
	for ( i = 0; i < 20; i ++ ) {  /* �е�U�����Ƿ���δ׼����,�������Ա����� */
		mDelaymS( 50 );
		if ( CH376DiskMount( ) == USB_INT_SUCCESS ) break;  /* ��ѯ�����Ƿ�׼���� */
	}
	mDelaymS( 5 );
	s = CH376SendCmdDatWaitInt( CMD1H_SET_CONFIG, 0 );  /* ȡ������,�ᵼ�ºܶ�U�̵�LED���� */
	mDelaymS( 5 );
	if ( i < 20 && s == USB_INT_SUCCESS ) return( USB_INT_SUCCESS );  /* �����ɹ� */
	else return( 0xFF );
/* ����ȡ��SOF���ᵼ�¾������U�̵�LED���� */
/* ����˴�ȡ��SOF, ��ô��˶�Ӧ, �ڼ�⵽U�̲����Ӧ���ٻָ�Ϊģʽ6���ָ�SOF�� */
//	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* �豸USB����ģʽ */
//	xWriteCH376Data( 0x05 );  /* ֹͣ����SOF��,��⵽U�̲���������ģʽ6�ָ�SOF������ */
//	mDelaymS( 2 );
//	xReadCH376Data( );
//	xEndCH376Cmd( );
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
		printf( "Wait Udisk\n" );

//		while ( CH376DiskConnect( ) != USB_INT_SUCCESS ) {  /* ���U���Ƿ�����,�ȴ�U�̲���,����SD��,�����ɵ�Ƭ��ֱ�Ӳ�ѯSD�����Ĳ��״̬���� */
//			mDelaymS( 100 );
//		}
		while ( 1 ) {  /* ��ǰ���еķ�����ͬ,�˴��ǵȵ��ж�֪ͨ���ٲ�ѯUSB�豸���� */
			if ( Query376Interrupt( ) ) {  /* ��ѯCH376�ж�(INT#����Ϊ�͵�ƽ) */
				if ( CH376DiskConnect( ) == USB_INT_SUCCESS ) break;  /* USB�豸���� */
			}
			mDelaymS( 100 );
		}

		mDelaymS( 200 );  /* ��ʱ,��ѡ����,�е�USB�洢����Ҫ��ʮ�������ʱ */

/* ���ڼ�⵽USB�豸��,���ȴ�10*50mS */
		for ( s = 0; s < 10; s ++ ) {  /* ��ȴ�ʱ��,10*50mS */
			mDelaymS( 50 );
			printf( "Ready ?\n" );
			if ( CH376DiskMount( ) == USB_INT_SUCCESS ) break;  /* ��ʼ�����̲����Դ����Ƿ���� */
		}
		s = CH376ReadBlock( buf );  /* �����Ҫ,���Զ�ȡ���ݿ�CH376_CMD_DATA.DiskMountInq,���س��� */
		if ( s == sizeof( INQUIRY_DATA ) ) {  /* U�̵ĳ��̺Ͳ�Ʒ��Ϣ */
			buf[ s ] = 0;
			printf( "UdiskInfo: %s\n", ((P_INQUIRY_DATA)buf) -> VendorIdStr );
		}

		printf( "Check Disk Write Protect ? ...\n" );
		s = IsDiskWriteProtect( );  /* ���U���Ƿ�д����, ����USB_INT_SUCCESS˵������д,����0xFF˵��ֻ��/д����,��������ֵ˵���Ǵ������ */
		if ( s != USB_INT_SUCCESS && s != 0xFF ) {  /* ����ʧ�� */
			printf( "Again ...\n" );
			mDelaymS( 100 );
			s = IsDiskWriteProtect( );  /* ����һ�� */
		}
		if ( s == USB_INT_SUCCESS ) {  /* ����д */
			printf( "... No !\n" );
			printf( "Create a File\n" );
			s = CH376FileCreate( "\\NEWFILE.TXT" );  /* �ڸ�Ŀ¼���½��ļ�����,����ļ��Ѿ���������ɾ�������½� */
			mStopIfError( s );
			buf[0] = 'O';
			buf[1] = 'K';
			s = CH376ByteWrite( buf, 2, NULL );  /* ���ļ�д������ */
			mStopIfError( s );
			printf( "Close\n" );
			s = CH376FileClose( TRUE );  /* �Զ������ļ����� */
			mStopIfError( s );
			if ( SafeRemoveDisk( ) == USB_INT_SUCCESS ) {  /* ��ȫ�Ƴ�U�� */
				printf( "Safe Remove !\n" );
			}
			else {
				printf( "Unsafe Remove !\n" );
			}
		}
		else if ( s == 0xFF ) {  /* д���� */
			printf( "... Yes !\n" );
		}
		else {
			mStopIfError( s );  /* ��ʾ������� */
		}

		printf( "Take out\n" );

//		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* ���U���Ƿ�����,�ȴ�U�̰γ� */
//			mDelaymS( 100 );
//		}
		while ( 1 ) {  /* ��ǰ���еķ�����ͬ,�˴��ǵȵ��ж�֪ͨ���ٲ�ѯUSB�豸���� */
			if ( Query376Interrupt( ) ) {  /* ��ѯCH376�ж�(INT#����Ϊ�͵�ƽ) */
				if ( CH376DiskConnect( ) != USB_INT_SUCCESS ) break;  /* USB�豸�Ͽ� */
			}
			mDelaymS( 100 );
		}

		mDelaymS( 200 );

	}
}