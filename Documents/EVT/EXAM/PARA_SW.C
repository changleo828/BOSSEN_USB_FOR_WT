/* CH376оƬ ����ģ��8λ�������ӵ�Ӳ������� V1.0 */
/* �ṩI/O�ӿ��ӳ��� */

#include	"HAL.H"

/* �����е�Ӳ�����ӷ�ʽ����(ʵ��Ӧ�õ�·���Բ����޸��������弰�ӳ���) */
/* ��Ƭ��������    CH376оƬ������
      P2.0                 A0
      P2.6                 CS#    ���ģ����Ĳ�����ֻ��CH376,��ôCS#����ֱ�ӽӵ͵�ƽ,ǿ��Ƭѡ
      P3.6                 WR#
      P3.7                 RD#
      P0(8λ�˿�)         D7-D0       */
sbit	CH376_A0	=	P2^0;
sbit	CH376_CS	=	P2^6;
sbit	CH376_WR	=	P3^6;
sbit	CH376_RD	=	P3^7;
#define	CH376_DATA_DAT_OUT( d )	{ P0 = d; }		/* �򲢿�������� */
#define	CH376_DATA_DAT_IN( )	( P0 )			/* �Ӳ����������� */
#define	CH376_DATA_DIR_OUT( )					/* ���ò��ڷ���Ϊ��� */
#define	CH376_DATA_DIR_IN( )	{ P0 = 0xFF; }	/* ���ò��ڷ���Ϊ���� */

#define CH376_INT_WIRE			INT0	/* �ٶ�CH376��INT#����,���δ������ôҲ����ͨ����ѯ״̬�˿�ʵ�� */

void	CH376_PORT_INIT( void )  /* ����ʹ��ͨ��I/Oģ�Ⲣ�ڶ�дʱ��,���Խ��г�ʼ�� */
{
	CH376_CS = 1;
	CH376_WR = 1;
	CH376_RD = 1;
	CH376_A0 = 0;
	CH376_DATA_DIR_IN( );  /* ���ò������� */
}

//void	mDelay0_5uS( void )  /* ������ʱ0.5uS,���ݵ�Ƭ����Ƶ���� */
//{
//}

#define	xEndCH376Cmd( )  /* ����CH376����,������SPI�ӿڷ�ʽ */

UINT8	xReadCH376Status( void )  /* ��CH376��״̬,�����ڲ��ڷ�ʽ */
{
	UINT8	mData;
	CH376_DATA_DIR_IN( );  /* ���ò��ڷ���Ϊ���� */
	CH376_A0 = 1;
	CH376_CS = 0;
	CH376_RD = 0;  /* �����Ч�������ź�, ��CH376оƬ��״̬�˿� */
	CH376_CS = 0;  /* �ò���������,������ʱ,CH376Ҫ���д������ȴ���40nS,ǿ�ҽ���˴�ִ��һ����ָ����ʱ��ȷ���������㹻ʱ���������� */
	mData = CH376_DATA_DAT_IN( );  /* ��CH376�Ĳ����������� */
	CH376_RD = 1;  /* �����Ч�Ŀ����ź�, ��ɲ���CH376оƬ */
	CH376_CS = 1;
	CH376_A0 = 0;
	return( mData );
}

void	xWriteCH376Cmd( UINT8 mCmd )  /* ��CH376д���� */
{
	CH376_DATA_DAT_OUT( mCmd );  /* ��CH376�Ĳ���������� */
	CH376_DATA_DIR_OUT( );  /* ���ò��ڷ���Ϊ��� */
	CH376_A0 = 1;
	CH376_CS = 0;
	CH376_WR = 0;  /* �����Чд�����ź�, дCH376оƬ������˿� */
//	CH376_CS = 0;  /* �ò���������,������ʱ,CH376Ҫ���д������ȴ���40nS */
	CH376_WR = 1;  /* �����Ч�Ŀ����ź�, ��ɲ���CH376оƬ */
	CH376_CS = 1;
	CH376_A0 = 0;
	CH376_DATA_DIR_IN( );  /* ��ֹ������� */
/*	mDelay0_5uS( ); mDelay0_5uS( ); mDelay0_5uS( );*/  /* ��ʱ1.5uSȷ����д���ڴ���1.5uS,������״̬��ѯ���� */
}

void	xWriteCH376Data( UINT8 mData )  /* ��CH376д���� */
{
	CH376_DATA_DAT_OUT( mData );  /* ��CH376�Ĳ���������� */
	CH376_DATA_DIR_OUT( );  /* ���ò��ڷ���Ϊ��� */
	CH376_A0 = 0;
	CH376_CS = 0;
	CH376_WR = 0;  /* �����Чд�����ź�, дCH376оƬ�����ݶ˿� */
//	CH376_CS = 0;  /* �ò���������,������ʱ,CH376Ҫ���д������ȴ���40nS */
	CH376_WR = 1;  /* �����Ч�Ŀ����ź�, ��ɲ���CH376оƬ */
	CH376_CS = 1;
	CH376_DATA_DIR_IN( );  /* ��ֹ������� */
//	mDelay0_5uS( );  /* ȷ����д���ڴ���0.6uS */
}

UINT8	xReadCH376Data( void )  /* ��CH376������ */
{
	UINT8	mData;
//	mDelay0_5uS( );  /* ȷ����д���ڴ���0.6uS */
	CH376_DATA_DIR_IN( );  /* ���ò��ڷ���Ϊ���� */
	CH376_A0 = 0;
	CH376_CS = 0;
	CH376_RD = 0;  /* �����Ч�������ź�, ��CH376оƬ�����ݶ˿� */
	CH376_CS = 0;  /* �ò���������,������ʱ,CH376Ҫ���д������ȴ���40nS,ǿ�ҽ���˴�ִ��һ����ָ����ʱ��ȷ���������㹻ʱ���������� */
	mData = CH376_DATA_DAT_IN( );  /* ��CH376�Ĳ����������� */
	CH376_RD = 1;  /* �����Ч�Ŀ����ź�, ��ɲ���CH376оƬ */
	CH376_CS = 1;
	return( mData );
}

/* ��ѯCH376�ж�(INT#�͵�ƽ) */
UINT8	Query376Interrupt( void )
{
#ifdef	CH376_INT_WIRE
	return( CH376_INT_WIRE ? FALSE : TRUE );  /* ���������CH376���ж�������ֱ�Ӳ�ѯ�ж����� */
#else
	return( xReadCH376Status( ) & PARA_STATE_INTB ? FALSE : TRUE );  /* ���δ����CH376���ж��������ѯ״̬�˿� */
#endif
}

UINT8	mInitCH376Host( void )  /* ��ʼ��CH376 */
{
	UINT8	res;
	CH376_PORT_INIT( );  /* �ӿ�Ӳ����ʼ�� */
	xWriteCH376Cmd( CMD11_CHECK_EXIST );  /* ���Ե�Ƭ����CH376֮���ͨѶ�ӿ� */
	xWriteCH376Data( 0x65 );
	res = xReadCH376Data( );
//	xEndCH376Cmd( );  // ���ڷ�ʽ����Ҫ
	if ( res != 0x9A ) return( ERR_USB_UNKNOWN );  /* ͨѶ�ӿڲ�����,����ԭ����:�ӿ������쳣,�����豸Ӱ��(Ƭѡ��Ψһ),���ڲ�����,һֱ�ڸ�λ,���񲻹��� */
	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* �豸USB����ģʽ */
	xWriteCH376Data( 0x06 );
	mDelayuS( 20 );
	res = xReadCH376Data( );
//	xEndCH376Cmd( );  // ���ڷ�ʽ����Ҫ
	if ( res == CMD_RET_SUCCESS ) return( USB_INT_SUCCESS );
	else return( ERR_USB_UNKNOWN );  /* ����ģʽ���� */
}