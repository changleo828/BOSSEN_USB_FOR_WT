;/* 2008.10.18
;****************************************
;**  Copyright  (C)  W.ch  1999-2009   **
;**  Web:  http://www.winchiphead.com  **
;****************************************
;**  USB Host File Interface for CH376 **
;**  TC2.0@PC, KC7.0@MCS51             **
;****************************************
;*/
;/* CH376 主机文件系统接口 */

;/* MCS-51单片机ASM语言的U盘文件读写示例程序,支持89C2051,89C51,89C52等单片机 */
;/* 本程序演示字节读写,用于将ADC模数采集的数据添加到U盘或者SD卡文件MY_ADC.TXT中,
;   如果文件存在那么将数据添加到文件末尾,如果文件不存在那么新建文件后添加数据 */
; 
; /* A51   CH376HFT.ASM */
; /* L51   CH376HFT.OBJ */
; /* OHX51 CH376HFT */
;

; 对于89C2051等没有并口总线的单片机,可以选择软件模拟并口,硬件SPI或者软件模拟SPI,异步串口等硬件通讯方式,C语言有这些例子
CH376_CMD_PORT			EQU		0BDF1H		; CH376命令端口的I/O地址
CH376_DAT_PORT			EQU		0BCF0H		; CH376数据端口的I/O地址

$INCLUDE		(..\CH376INC.INC)			; ASM头文件,常用定义

LED_OUT					EQU		P1.4		; 调试状态指示输出

; 应用程序段定义
MY_CODE				SEGMENT		CODE
MY_DATA				SEGMENT		DATA

; 复位和中断向量
					CSEG	AT	0000H
					LJMP	ASM_MAIN		; 单片机复位后的主程序入口
;					CSEG	AT	0003H		; INT0中断向量
;					LJMP    InterruptHandle	; CH376的中断服务程序,如果使用"中断方式"则需根据中断引脚修改向量

; 应用程序中用到的常量,只定义大小,不指定地址,由链接程序自动分配地址
					RSEG	MY_CODE
FILE_NAME:			DB	'\MY_ADC.TXT',00H
STR_WRITE_1:		DB	'This is first string,\xd\xa',00H
STR_WRITE_2:		DB	'Second string 结束!\xd\xa',00H

; 应用程序中用到的变量,只定义大小,不指定地址,由链接程序自动分配地址
					RSEG	MY_DATA
MY_BUFFER:			DS	64

; 应用程序代码,不指定地址,由链接程序自动分配地址
					RSEG	MY_CODE
;
; 延时n微秒,不精确,24MHz主频
; 输入: A 延时微秒数
; 使用: A, R7
mDelayuS:			MOV		R7,A
mDelayuS_loop:		DJNZ	R7,mDelayuS_loop
					RET
;
; 延时n毫秒,不精确,24MHz主频
; 输入: A 延时毫秒数
; 使用: A, R7
mDelaymS:			MOV		R7,#0FAH
mDelaymS_loop:		NOP
					NOP
					NOP
					NOP
					NOP
					NOP
					DJNZ	R7,mDelaymS_loop
					DEC     A
					JNZ     mDelaymS
					RET
;
; 向CH376写命令
; 输入: A 命令
; 使用: A, DPTR
xWriteCH376Cmd:		MOV		DPTR,#CH376_CMD_PORT	;命令端口
					MOVX	@DPTR,A
					NOP								;延时确保读写周期大于1.5uS
					RET
; 向CH376写数据
; 输入: A 数据
; 使用: A, DPTR
xWriteCH376Data:	MOV		DPTR,#CH376_DAT_PORT
					MOVX	@DPTR,A
					RET
; 从CH376读数据
; 输出: A 数据
; 使用: A, DPTR
xReadCH376Data:		MOV		DPTR,#CH376_DAT_PORT
					MOVX	A,@DPTR
					RET
;
; 查询CH376中断(INT#低电平)
; 输出: NC 有中断, CY 无中断
; 使用: A, DPTR
Query376Interrupt:	MOV		DPTR,#CH376_CMD_PORT	;状态端口
					MOVX	A,@DPTR
					RLC		A
					RET
;
; 初始化CH376为USB主机工作模式
; 输出: NC 成功, CY 操作失败
; 使用: A, DPTR, R7
mInitCH376Host:		MOV		A,#CMD11_SET_USB_MODE
					LCALL	xWriteCH376Cmd
					MOV		A,#06H					;03H用于SD卡模式
					LCALL	xWriteCH376Data
					MOV		A,#20
					LCALL	xReadCH376Data
					SETB	C
					XRL		A,#CMD_RET_SUCCESS
					JNZ		mInitCH376Host0
					CLR		C
mInitCH376Host0:	RET
;
; 从内部缓冲区读取数据块
; 输入: R0 指向内部RAM缓冲区,接收数据
; 输出: R7 数据块长度
; 使用: A, DPTR, R0, R6, R7
CH376ReadBlock:		MOV		A,#CMD01_RD_USB_DATA0
					LCALL	xWriteCH376Cmd
					LCALL	xReadCH376Data			;长度
					MOV		R7,A
					JZ		CH376ReadBlock0			;0长度
					MOV		R6,A
CH376ReadBlock1:	LCALL	xReadCH376Data
					MOV		@R0,A
					INC		R0
					DJNZ	R6,CH376ReadBlock1
CH376ReadBlock0:	RET
;
; 向内部指定缓冲区写入请求的数据块
; 输入: R0 指向内部RAM缓冲区,待写数据
; 输出: R7 数据块长度
; 使用: A, DPTR, R0, R6, R7
CH376WriteReqBlock:	MOV		A,#CMD01_WR_REQ_DATA
					LCALL	xWriteCH376Cmd
					LCALL	xReadCH376Data			;长度
					MOV		R7,A
					JZ		CH376WriteReqBlk0		;0长度
					MOV		R6,A
CH376WriteReqBlk1:	MOV		A,@R0
					INC		R0
					LCALL	xWriteCH376Data
					DJNZ	R6,CH376WriteReqBlk1
CH376WriteReqBlk0:	RET
;
; 设置将要操作的文件的文件名
; 输入: DPTR 指向程序ROM中的文件名字符串,以0结束
; 使用: A, DPTR
CH376SetFileName:	PUSH	DPL
					PUSH	DPH
					MOV		A,#CMD10_SET_FILE_NAME
					LCALL	xWriteCH376Cmd
					POP		DPH
					POP		DPL
CH376SetFileName1:	CLR		A
					MOVC	A,@A+DPTR
					INC		DPTR
					PUSH	DPL
					PUSH	DPH
					LCALL	xWriteCH376Data
					POP		DPH
					POP		DPL
					JNZ		CH376SetFileName1
					RET
;
; 获取中断状态并取消中断请求
; 输出: A 中断状态码
; 使用: A, DPTR
CH376GetIntStatus:	MOV		A,#CMD01_GET_STATUS
					LCALL	xWriteCH376Cmd
					LCALL	xReadCH376Data
					RET
;
; 等待CH376中断(INT#低电平)，返回中断状态码
; 输出: A 中断状态码, NC 操作成功(中断状态码是USB_INT_SUCCESS), CY 操作失败
; 使用: A, DPTR
Wait376Interrupt:	LCALL	Query376Interrupt
					JC		Wait376Interrupt		;没有中断
					LCALL	CH376GetIntStatus		;获取中断状态并取消中断请求
					CJNE	A,#USB_INT_SUCCESS,Wait376Interrupt0	;比较中断状态码
					CLR		C
					RET
Wait376Interrupt0:	SETB	C
					RET
;
; 检查U盘是否连接,不支持SD卡
; 输出: A 中断状态码, NC 操作成功(中断状态码是USB_INT_SUCCESS), CY 操作失败
; 使用: A, DPTR
CH376DiskConnect:	MOV		A,#CMD0H_DISK_CONNECT
					LCALL	xWriteCH376Cmd
					LJMP	Wait376Interrupt
;
; 初始化磁盘并测试磁盘是否就绪
; 输出: A 中断状态码, NC 操作成功(中断状态码是USB_INT_SUCCESS), CY 操作失败
; 使用: A, DPTR
CH376DiskMount:		MOV		A,#CMD0H_DISK_MOUNT
					LCALL	xWriteCH376Cmd
					LJMP	Wait376Interrupt
;
; 在根目录或者当前目录下打开文件或者目录
; 输入: DPTR 指向程序ROM中的文件名字符串,以0结束
; 输出: A 中断状态码, NC 操作成功(中断状态码是USB_INT_SUCCESS), CY 操作失败
; 使用: A, DPTR
CH376FileOpen:		CLR		A
					MOVC	A,@A+DPTR
					PUSH	ACC
					LCALL	CH376SetFileName		;设置将要操作的文件的文件名
					POP		ACC
					CJNE    A,#DEF_SEPAR_CHAR1,CH376FileOpen1
					SJMP    CH376FileOpen0
CH376FileOpen1:		CJNE    A,#DEF_SEPAR_CHAR2,CH376FileOpen2
CH376FileOpen0:		MOV		A,#CMD50_WRITE_VAR32
					LCALL	xWriteCH376Cmd
					MOV		A,#VAR_CURRENT_CLUST
					LCALL	xWriteCH376Data
					CLR		A
					LCALL	xWriteCH376Data
					CLR		A
					LCALL	xWriteCH376Data
					CLR		A
					LCALL	xWriteCH376Data
					CLR		A
					LCALL	xWriteCH376Data
CH376FileOpen2:		MOV		A,#CMD0H_FILE_OPEN
					LCALL	xWriteCH376Cmd
					LJMP	Wait376Interrupt
;
; 在根目录或者当前目录下新建文件,如果文件已经存在那么先删除
; 输入: DPTR 指向程序ROM中的文件名字符串,以0结束
; 输出: A 中断状态码, NC 操作成功(中断状态码是USB_INT_SUCCESS), CY 操作失败
; 使用: A, DPTR
CH376FileCreate:	LCALL	CH376SetFileName		;设置将要操作的文件的文件名
					MOV		A,#CMD0H_FILE_CREATE
					LCALL	xWriteCH376Cmd
					LJMP	Wait376Interrupt
;
; 关闭当前已经打开的文件
; 输入: A 为0则禁止更新文件长度, 非0则允许自动更新文件长度
; 输出: A 中断状态码, NC 操作成功(中断状态码是USB_INT_SUCCESS), CY 操作失败
; 使用: A, DPTR
CH376FileClose:		PUSH	ACC
					MOV		A,#CMD1H_FILE_CLOSE
					LCALL	xWriteCH376Cmd
					POP		ACC
					LCALL	xWriteCH376Data
					LJMP	Wait376Interrupt
;
; 以字节为单位移动当前文件指针
; 输入: R7/R6/R5/R4 偏移字节数
; 输出: A 中断状态码, NC 操作成功(中断状态码是USB_INT_SUCCESS), CY 操作失败
; 使用: A, DPTR, R4, R5, R6, R7
CH376ByteLocate:	MOV		A,#CMD4H_BYTE_LOCATE
					LCALL	xWriteCH376Cmd
					MOV		A,R4
					LCALL	xWriteCH376Data
					MOV		A,R5
					LCALL	xWriteCH376Data
					MOV		A,R6
					LCALL	xWriteCH376Data
					MOV		A,R7
					LCALL	xWriteCH376Data
					LJMP	Wait376Interrupt
;
; 以字节为单位从当前位置读取数据块
; 输入: R0 指向内部RAM缓冲区,接收数据,
;       R7/R6 请求读取的字节数,从0到65535,R6为低字节,实际上使用内部RAM缓冲区只能支持几十字节,使用外部RAM缓冲区后才支持到65535字节
; 输出: A 中断状态码, NC 操作成功(中断状态码是USB_INT_SUCCESS), CY 操作失败,
;       R5/R4 实际读取的字节数,R4为低字节
; 使用: A, DPTR, R0, R4, R5, R6, R7
CH376ByteRead:		MOV		A,#CMD2H_BYTE_READ
					LCALL	xWriteCH376Cmd
					MOV		A,R6
					LCALL	xWriteCH376Data
					MOV		A,R7
					LCALL	xWriteCH376Data
					CLR		A
					MOV		R4,A
					MOV		R5,A
CH376ByteReadLoop:	LCALL	Wait376Interrupt
					JNC		CH376ByteReadEnd		;操作结束并且成功
					CJNE	A,#USB_INT_DISK_READ,CH376ByteReadErr
					LCALL	CH376ReadBlock			;从内部缓冲区读取数据块,缓冲区指针自动增加
					MOV		A,R4
					ADD		A,R7
					MOV		R4,A					;累加实际接收的字节数
					MOV		A,R5
					ADDC	A,#00H
					MOV		R5,A
					MOV		A,#CMD0H_BYTE_RD_GO
					LCALL	xWriteCH376Cmd
					SJMP    CH376ByteReadLoop		;继续读
CH376ByteReadErr:	SETB	C
CH376ByteReadEnd:	RET
;
; 以字节为单位向当前位置写入数据块
; 输入: R0 指向内部RAM缓冲区,待写数据,
;       R7/R6 请求写入的字节数,从0到65535,R6为低字节,实际上使用内部RAM缓冲区只能支持几十字节,使用外部RAM缓冲区后才支持到65535字节
; 输出: A 中断状态码, NC 操作成功(中断状态码是USB_INT_SUCCESS), CY 操作失败,
;       R5/R4 实际写入的字节数,R4为低字节
; 使用: A, DPTR, R0, R4, R5, R6, R7
CH376ByteWrite:		MOV		A,#CMD2H_BYTE_WRITE
					LCALL	xWriteCH376Cmd
					MOV		A,R6
					LCALL	xWriteCH376Data
					MOV		A,R7
					LCALL	xWriteCH376Data
					CLR		A
					MOV		R4,A
					MOV		R5,A
CH376ByteWriteLoop:	LCALL	Wait376Interrupt
					JNC		CH376ByteWriteEnd		;操作结束并且成功
					CJNE	A,#USB_INT_DISK_WRITE,CH376ByteWriteErr
					LCALL	CH376WriteReqBlock		;向内部指定缓冲区写入请求的数据块,缓冲区指针自动增加
					MOV		A,R4
					ADD		A,R7
					MOV		R4,A					;累加实际写入的字节数
					MOV		A,R5
					ADDC	A,#00H
					MOV		R5,A
					MOV		A,#CMD0H_BYTE_WR_GO
					LCALL	xWriteCH376Cmd
					SJMP    CH376ByteWriteLoop		;继续写
CH376ByteWriteErr:	SETB	C
CH376ByteWriteEnd:	RET
;
; 将程序ROM空间的字符串复制到内部RAM中
; 输入: DPTR 指向程序ROM空间的字符串源地址
;       R0 指向内部RAM的目的地址
; 使用: A, DPTR, R0
mCopyStringToIRAM:	CLR		A
					MOVC	A,@A+DPTR
					INC		DPTR
					MOV		@R0,A
					INC		R0
					JNZ		mCopyStringToIRAM
					RET
;
; 计算RAM中字符串的长度
; 输入: R0 指向内部RAM中的字符串起始地址
; 输出: A 字符串的长度
; 使用: A, R0
mGetStringLength:	MOV		A,#0FFH
					DEC		R0
mGetStringLength1:	INC		R0
					INC		A
					CJNE	@R0,#00H,mGetStringLength1
					RET
;
; 检查操作状态,如果错误则停机,实际应用程序需要另作处理
; 输入: A 状态码
mStopIfError:		JC		ERROR_FOUND			;错误
					RET							;操作成功则返回
ERROR_FOUND:		CLR		LED_OUT				;LED闪烁显示
					MOV		A,#100
					LCALL	mDelaymS
					SETB	LED_OUT
					MOV		A,#100
					LCALL	mDelaymS
					SJMP	ERROR_FOUND
;
;
; 应用程序主程序, 仅作演示
ASM_MAIN:			MOV		SP,#0CFH			;堆栈
					CLR		LED_OUT				;开机后LED亮一下以示工作
					MOV		A,#100
					LCALL	mDelaymS
					SETB	LED_OUT

					LCALL	mInitCH376Host		;初始化CH376为USB主机工作模式
					LCALL	mStopIfError

; 主循环, 等待U盘插入并处理, 实际上单片机可以做其它事, 当需要读写U盘时再查询U盘状态
WAIT_DISK_IN:		LCALL	CH376DiskConnect	;检查U盘是否连接,不支持SD卡,对于SD卡可以由单片机直接查询SD卡座的插拔状态引脚
					JNC		DISK_IN_NOW			;U盘已经插入,或者其它USB设备已经插入
					MOV		A,#100
					LCALL	mDelaymS			;单片机可以做其它事,无需频繁查询
					SJMP	WAIT_DISK_IN
DISK_IN_NOW:		MOV		A,#250
					LCALL	mDelaymS			;延时,可选操作,有的USB存储器需要几十毫秒或者更多的延时
					CLR  	LED_OUT				;LED亮

; 检查U盘是否准备好
					MOV		R2,#10
IS_DISK_READY:		LCALL	CH376DiskMount		;初始化磁盘并测试磁盘是否就绪
					JNC		DISK_READY_OK		;准备好了
					MOV		A,#50
					LCALL	mDelaymS
					DJNZ	R2,IS_DISK_READY	;继续等待,有的U盘总是返回未准备好,不过可以被忽略
DISK_READY_OK:

; 尝试打开文件
					MOV		DPTR,#FILE_NAME		;指向程序ROM中的文件名字符串
					LCALL	CH376FileOpen		;在根目录或者当前目录下打开文件
					JNC     FILE_OPEN_EXIST		;文件已经存在
					CJNE	A,#ERR_MISS_FILE,FILE_OPEN_ERR
; 文件不存在则新建文件
					MOV		DPTR,#FILE_NAME		;指向程序ROM中的文件名字符串
					LCALL	CH376FileCreate		;在根目录或者当前目录下新建文件
					LCALL	mStopIfError
					SJMP	FILE_OPEN_CREATE
; 文件打开操作失败
FILE_OPEN_ERR:		SETB	C
					LCALL	mStopIfError
; 文件已经存在,则移动当前文件指针到文件的尾部以便添加数据
FILE_OPEN_EXIST:

;					MOV		A,#30				;请求读取的字节数,不能超过内部缓冲区的大小
;					MOV		R2,A
;					MOV		R6,A
;					MOV		R7,#00H
;					MOV		R0,#MY_BUFFER		;指向内部RAM缓冲区,存放读出的数据块
;					LCALL	CH376ByteRead		;以字节为单位从当前位置读取数据块
;					LCALL	mStopIfError
;					MOV		A,R4				;实际读取的字节数
;					XRL		A,R2				;请求读取的字节数
;					JNZ		END_OF_FILE			;实际字节数不足说明到文件末尾了,文件结束
;					NOP							;文件尚未结束,取出数据

					MOV		R4,#0FFH			;移到文件的尾部
					MOV		R5,#0FFH
					MOV		R6,#0FFH
					MOV		R7,#0FFH
					LCALL	CH376ByteLocate		;以字节为单位移动当前文件指针
					LCALL	mStopIfError
FILE_OPEN_CREATE:

; 开始向文件中写入数据
					MOV		DPTR,#STR_WRITE_1
					MOV		R0,#MY_BUFFER
					LCALL	mCopyStringToIRAM	;将程序ROM空间的字符串复制到内部RAM中
					MOV		R0,#MY_BUFFER
					LCALL	mGetStringLength	;计算RAM中字符串的长度
					MOV		R6,A				;请求写入的字节数
					MOV		R7,#00H
					MOV		R0,#MY_BUFFER		;指向内部RAM缓冲区
					LCALL	CH376ByteWrite		;以字节为单位向当前位置写入数据块
					LCALL	mStopIfError
; 开始向文件中写入数据,第二次
					MOV		DPTR,#STR_WRITE_2
					MOV		R0,#MY_BUFFER
					LCALL	mCopyStringToIRAM	;将程序ROM空间的字符串复制到内部RAM中
					MOV		R0,#MY_BUFFER
					LCALL	mGetStringLength	;计算RAM中字符串的长度
					MOV		R6,A				;请求写入的字节数
					MOV		R7,#00H
					MOV		R0,#MY_BUFFER		;指向内部RAM缓冲区
					LCALL	CH376ByteWrite		;以字节为单位向当前位置写入数据块
					LCALL	mStopIfError
					MOV		A,#01H				;允许自动更新文件长度
					LCALL   CH376FileClose		;关闭当前已经打开的文件
					LCALL	mStopIfError

; 演示处理完毕,等待U盘拔出
					SETB 	LED_OUT				; LED灭
WAIT_DISK_OUT:		LCALL	CH376DiskConnect	;检查U盘是否连接,等待U盘拔出,对于SD卡可以由单片机直接查询SD卡座的插拔状态引脚
					JC		DISK_OUT_NOW		;U盘已经断开
					MOV		A,#100
					LCALL	mDelaymS			;单片机可以做其它事,无需频繁查询
					SJMP	WAIT_DISK_OUT

DISK_OUT_NOW:		MOV		A,#200
					LCALL	mDelaymS
					LJMP	WAIT_DISK_IN		;主循环,处理下一个U盘
;
END
