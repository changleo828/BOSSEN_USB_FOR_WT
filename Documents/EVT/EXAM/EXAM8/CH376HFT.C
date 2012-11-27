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
/* ��������ʾ������д,�޸��ļ�����,��ѯ����ʣ��ռ�,
   ���ڽ�ADCģ���ɼ����������ӵ�U���ļ�MY_ADC.TXT��,Ϊ����ٶȶ�������Ϊ��λ��дU���ļ�,��֧��SD��,
   ��������ʾ������ģʽ�´�����������,ͬʱ��˲�������ͽϸ��ٶ�,��Ҫ��Ƭ��ϵͳ�ṩ�㹻��RAM��Ϊ�ļ����ݻ�����FILE_DATA_BUF */

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
#define		EN_DISK_QUERY		1	/* ���ô��̲�ѯ */
#define		EN_SECTOR_ACCESS	1	/* ����������д */
#include "..\FILE_SYS.H"
#include "..\FILE_SYS.C"

UINT8		idata	buf[64];

UINT16	total;		/* ��¼��ǰ������FILE_DATA_BUF�е����ݳ��� */
UINT32	NewSize;	/* ��ʱ���� */
UINT8	xdata	FileDataBuf[0x4000];	/* �ļ�������,̫С���ٶ��� */

/* ��׼��д��U�̵��������ݽ��м��л���,��ϳɴ����ݿ�ʱ��ͨ��CH376����д��U�� */
/* �������ĺô���: ����ٶ�(��Ϊ�����ݿ�д��ʱЧ�ʸ�), ����U�����(U���ڲ����ڴ���������,����Ƶ����д) */
void	mFlushBufferToDisk( UINT8 force )
/* force = 0 ���Զ�ˢ��(��黺�����е����ݳ���,����д��,��������ʱ���ڻ�������), force != 0 ��ǿ��ˢ��(���ܻ������е������ж��ٶ�д��,ͨ����ϵͳ�ػ�ǰӦ��ǿ��д��) */
{
	UINT8	s;
	if ( force ) {  /* ǿ��ˢ�� */
		s = ( total + DEF_SECTOR_SIZE - 1 ) / DEF_SECTOR_SIZE;  /* ���������е��ֽ���ת��Ϊ������(����DEF_SECTOR_SIZE),���ȼ���DEF_SECTOR_SIZE-1����ȷ��д��������ͷ���� */
		if ( s ) {  /* ������ */
			s = CH376SecWrite( FileDataBuf, s, NULL );  /* ������Ϊ��λ���ļ�д������,д�뻺�����е���������,��������ͷ */
			mStopIfError( s );
/* ��ЩU�̿��ܻ�Ҫ����д���ݺ�ȴ�һ����ܼ�������,����,�����ĳЩU���з������ݶ�ʧ����,������ÿ��д�����ݺ�������ʱ�ټ��� */
			mDelaymS( 1 );  /* д����ʱ,��ѡ��,�����U�̲���Ҫ */
			memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );  /* ���ղ���д��U�̵���ͷ���ݸ��Ƶ���������ͷ�� */
			total &= DEF_SECTOR_SIZE - 1;  /* ��������ֻʣ�¸ղ���д��U�̵���ͷ����,���������ڻ���������Ϊ�˷����Ժ��������׷������ */
		}
		NewSize = CH376GetFileSize( );  /* ��ȡ��ǰ�ļ�����,���û����ͷ����,��ô�ļ�������DEF_SECTOR_SIZE�ı��� */
		if ( total ) NewSize -= DEF_SECTOR_SIZE - total;  /* ������Ϊ��λ,����ͷ����,������������ļ�����(��Ч���ݵĳ���) */
		CH376WriteVar32( VAR_FILE_SIZE, NewSize );  /* ����ȷ�ĵ�ǰ�ļ�����д��CH376�ڴ� */
		printf( "Current file size is %ld\n", NewSize );
		s = CH376SecWrite( FileDataBuf, 0, NULL );  /* д0����,ʵ����ˢ���ļ�����,��CH376�ڴ��е�ǰ�ļ�������Ϣ����д��U�̻���SD���� */
		mStopIfError( s );
		s = CH376SecLocate( 0xFFFFFFFF );  /* ���»ص�ԭ�ļ���β��,������Ϊ��λ,���Ի�����ļ�β������ͷ����,���������д�����ݽ�����β����ͷ����,��������ͷ������һ�ݸ��������ڻ�������,��������� */
		mStopIfError( s );
	}
	else if ( total >= sizeof( FileDataBuf ) - DEF_SECTOR_SIZE ) {  /* �������е����ݿ�Ҫ����,����Ӧ���Ƚ��������е�ԭ������д��U�� */
		s = CH376SecWrite( FileDataBuf, (UINT8)( total / DEF_SECTOR_SIZE ), NULL );  /* ���������е��ֽ���ת��Ϊ������(����DEF_SECTOR_SIZE),������ͷ�����Ȳ���,������Ϊ��λ���ļ�д������ */
		mStopIfError( s );
		memcpy( FileDataBuf, & FileDataBuf[ total & ~ ( DEF_SECTOR_SIZE - 1 ) ], total & ( DEF_SECTOR_SIZE - 1 ) );  /* ���ղ�δд��U�̵���ͷ���ݸ��Ƶ���������ͷ�� */
		total &= DEF_SECTOR_SIZE - 1;  /* ��������ֻʣ�¸ղ�δд��U�̵���ͷ���� */
/* ע��,�˴�U�̻���SD���е��ļ�������Ȼ����ǰ��ֵ,����ʱͻȻ�ϵ�,��ô��Ȼ�����ϵ���������U��/SD����,���ļ�����û�а�����Щ����,���³���Ӧ�ó����޷�ȡ������ */
	}
}

main( ) {
	UINT8	s;
	UINT8	month, date, hour;
	UINT16	year, adc;

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

/* ���U�̻���SD����ʣ��ռ� */
		printf( "DiskQuery: " );
		s = CH376DiskQuery( (PUINT32)buf );  /* ��ѯ����ʣ��ռ���Ϣ,������ */
		mStopIfError( s );
//		printf( "free cap = %ld MB\n", *(PUINT32)buf * DEF_SECTOR_SIZE / 1000000 );  /* δ������������������С��,����1M����Ϊ���ֽڵ�λ */
		printf( "free cap = %ld MB\n", *(PUINT32)buf / ( 1000000 / DEF_SECTOR_SIZE ) );  /* ����һ�еļ�����ܻ����,����˳����� */

/* ���MY_ADC.TXT�ļ��Ѿ��������������ݵ�β��,������������½��ļ� */
		printf( "Open\n" );
		s = CH376FileOpen( "/MY_ADC.TXT" );  /* ���ļ�,���ļ��ڸ�Ŀ¼�� */
		if ( s == USB_INT_SUCCESS ) {  /* �ļ����ڲ����Ѿ�����,�ƶ��ļ�ָ�뵽β���Ա��������� */
			NewSize = CH376GetFileSize( );  /* ��ȡ��ǰ�ļ�����,���û����ͷ����,��ô�ļ�������DEF_SECTOR_SIZE�ı��� */
			printf( "File size = %ld\n", NewSize );
			printf( "Locate tail\n" );
			s = CH376SecLocate( 0xFFFFFFFF );  /* ������Ϊ��λ�Ƶ��ļ�β��,����ļ�β���в���һ����������ͷ�����򱻺���,�������������ô��ͷ���ݽ����ܱ�д�����ݸ��� */
			mStopIfError( s );
			total = NewSize & ( DEF_SECTOR_SIZE - 1 );  /* �ϴα����ļ�ʱ���β������ͷ����,��ô��ȡ����ͷ�ֽ���,�����������ȵ��������� */
			printf( "Read last tail = %d Bytes\n", total );
			if ( total ) {  /* ԭβ������ͷ����,��ô�ȵ����ڴ�,�Ա������������ */
				s = CH376SecRead( FileDataBuf, 1, NULL );  /* ���ļ���ȡβ����ͷ���� */
				mStopIfError( s );
				s = CH376SecLocate( 0xFFFFFFFF );  /* ���»ص�ԭ�ļ���β��,�������д�����ݽ�����ԭβ����ͷ����,����ԭ��ͷ���ݸղ��Ѿ��������ڴ�,��������� */
				mStopIfError( s );
			}
		}
		else if ( s == ERR_MISS_FILE ) {  /* û���ҵ��ļ�,�����½��ļ� */
			printf( "Create\n" );
			s = CH376FileCreate( NULL );  /* �½��ļ�����,����ļ��Ѿ���������ɾ�������½�,�������ṩ�ļ���,�ղ��Ѿ��ṩ��CH376FileOpen */
			mStopIfError( s );
			total = 0;  /* ��ǰû����ͷ���� */
			NewSize = 0;
		}
		else mStopIfError( s );  /* ���ļ�ʱ���� */
		printf( "Write begin\n" );
		total += sprintf( FileDataBuf + total, "�ڱ�����������֮ǰ,���ļ��������ݵĳ����� %ld �ֽ�\xd\xa", NewSize );  /* �����������ӵ���������β��,�ۼƻ������ڵ����ݳ��� */
		mFlushBufferToDisk( 0 );  /* �Զ�ˢ�»�����,��黺�����Ƿ�����,����д�� */
		printf( "Write ADC data\n" );
		TR0=1;  /* �ö�ʱ��0�ļ���ֵ����ADC���� */
		for ( month = 1; month != 12; month ++ ) {  /* ��Ϊ���԰���û��ʵʱʱ��оƬ,������ѭ����ʽģ���·� */
			for ( date = 1; date != 30; date ++ ) {  /* ��Ϊ���԰���û��ʵʱʱ��оƬ,������ѭ����ʽģ������ */
				year = 2004;  /* �ٶ�Ϊ2004�� */
				hour = TL1 & 0x1F;  /* ��Ϊ���԰���û��ʵʱʱ��оƬ,�����ö�ʱ��1�ļ������������ʾ */
/*				adc = get_adc_data( ); */
				adc = ( (UINT16)TH0 << 8 ) | TL0;  /* ��Ϊ���԰���û��ADC,�����ö�ʱ��0�ļ�������ADC������ʾ */
				total += sprintf( FileDataBuf + total, "Year=%04d, Month=%02d, Date=%02d, Hour=%02d, ADC_data=%u\xd\xa", year, (UINT16)month, (UINT16)date, (UINT16)hour, adc );  /* �����������ݸ�ʽΪһ���ַ��� */
				if ( month == 6 && ( date & 0x0F ) == 0 ) mFlushBufferToDisk( 1 );  /* ǿ��ˢ�»�����,����ǿ��ˢ�»�����,������ͻȻ�ϵ����Լ������ݶ�ʧ */
				else mFlushBufferToDisk( 0 );  /* �Զ�ˢ�»�����,��黺�����Ƿ�����,����д�� */
				printf( "Current total is %d\n", total );  /* ���ڼ�ؼ�� */
			}
		}
		printf( "Write end\n" );
		total += sprintf( FileDataBuf + total, " ********************************* " );  /* �����������ӵ���������β��,�ۼƻ������ڵ����ݳ��� */
		total += sprintf( FileDataBuf + total, "��ε�ADC���ݵ��˽���,���򼴽��˳�\xd\xa" );  /* �����������ӵ���������β��,�ۼƻ������ڵ����ݳ��� */
		mFlushBufferToDisk( 1 );  /* ǿ��ˢ�»�����,��ΪϵͳҪ�˳���,���Ա���ǿ��ˢ�� */
		printf( "Close\n" );
		s = CH376FileClose( FALSE );  /* �ر��ļ�,����ģʽͨ�����Զ������ļ�����,��Ϊǿ��ˢ�»�����ʱ�Ѿ��������ļ�����,�������ﲻ��Ҫ�Զ������ļ����� */
		mStopIfError( s );

		printf( "Take out\n" );
		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* ���U���Ƿ�����,�ȴ�U�̰γ� */
			mDelaymS( 100 );
		}
		mDelaymS( 200 );
	}
}