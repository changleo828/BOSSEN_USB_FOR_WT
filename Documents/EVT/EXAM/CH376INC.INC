;/* ASM Define for CH376      */
;/* Website:  http://wch.cn   */
;/* Email:    tech@wch.cn     */
;/* Author:   W.ch 2008.10    */
;/* V1.0 for CH376            */

;/********************************************************************************************************************** */
;/*常用类型和常量定义 */


;#ifndef		TRUE
TRUE				EQU		1
FALSE				EQU		0
;#endif
;#ifndef		NULL
NULL				EQU		0
;#endif

;/********************************************************************************************************************** */
;/*硬件特性 */

CH376_DAT_BLOCK_LEN	EQU		040H			;/*USB单个数据包, 数据块的最大长度, 默认缓冲区的长度 */

;/********************************************************************************************************************** */
;/*命令代码 */
;/*部分命令兼容CH375芯片, 但是输入数据或者输出数据的可能局部不同) */
;/*一个命令操作顺序包含:
;          一个命令码(对于串口方式,命令码之前还需要两个同步码),
;          若干个输入数据(可以是0个),
;          产生中断通知 或者 若干个输出数据(可以是0个), 二选一, 有中断通知则一定没有输出数据, 有输出数据则一定不产生中断
;       仅CMD01_WR_REQ_DATA命令例外, 顺序包含: 一个命令码, 一个输出数据, 若干个输入数据
;   命令码起名规则: CMDxy_NAME
;       其中的x和y都是数字, x说明最少输入数据个数(字节数), y说明最少输出数据个数(字节数), y如果是H则说明产生中断通知,
;       有些命令能够实现0到多个字节的数据块读写, 数据块本身的字节数未包含在上述x或y之内 */
;/*本文件默认会同时提供与CH375芯片命令码兼容的命令码格式（即去掉x和y之后), 如果不需要, 那么可以定义_NO_CH375_COMPATIBLE_禁止 */

;/********************************************************************************************************************** */
;/*主要命令(手册一), 常用 */

CMD01_GET_IC_VER	EQU		001H			;/*获取芯片及固件版本 */
;/*输出: 版本号( 位7为0, 位6为1, 位5~位0为版本号 ) */
;/*          CH376返回版本号的值为041H即版本号为01H */

CMD21_SET_BAUDRATE	EQU		002H			;/*串口方式: 设置串口通讯波特率(上电或者复位后的默认波特率为9600bps,由D4/D5/D6引脚选择) */
;/*输入: 波特率分频系数, 波特率分频常数 */
;/*输出: 操作状态( CMD_RET_SUCCESS或CMD_RET_ABORT, 其它值说明操作未完成 ) */

CMD00_ENTER_SLEEP	EQU		003H			;/*进入睡眠状态 */

CMD00_RESET_ALL		EQU		005H			;/*执行硬件复位 */

CMD11_CHECK_EXIST	EQU		006H			;/*测试通讯接口和工作状态 */
;/*输入: 任意数据 */
;/*输出: 输入数据的按位取反 */

CMD20_CHK_SUSPEND	EQU		00BH			;/*设备方式: 设置检查USB总线挂起状态的方式 */
;/*输入: 数据10H, 检查方式 */
;/*          00H=不检查USB挂起, 04H=以50mS为间隔检查USB挂起, 05H=以10mS为间隔检查USB挂起 */

CMD20_SET_SDO_INT	EQU		00BH			;/*SPI接口方式: 设置SPI的SDO引脚的中断方式 */
;/*输入: 数据16H, 中断方式 */
;/*          10H=禁止SDO引脚用于中断输出,在SCS片选无效时三态输出禁止, 90H=SDO引脚在SCS片选无效时兼做中断请求输出 */

CMD14_GET_FILE_SIZE	EQU		00CH			;/*主机文件模式: 获取当前文件长度 */
;/*输入: 数据68H */
;/*输出: 当前文件长度(总长度32位,低字节在前) */

CMD50_SET_FILE_SIZE	EQU		00DH			;/*主机文件模式: 设置当前文件长度 */
;/*输入: 数据68H, 当前文件长度(总长度32位,低字节在前) */

CMD11_SET_USB_MODE	EQU		015H			;/*设置USB工作模式 */
;/*输入: 模式代码 */
;/*      00H=未启用的设备方式, 01H=已启用的设备方式并且使用外部固件模式(串口不支持), 02H=已启用的设备方式并且使用内置固件模式 */
;/*      03H=SD卡主机模式/未启用的主机模式,用于管理和存取SD卡中的文件 */
;/*      04H=未启用的主机方式, 05H=已启用的主机方式, 06H=已启用的主机方式并且自动产生SOF包, 07H=已启用的主机方式并且复位USB总线 */
;/*输出: 操作状态( CMD_RET_SUCCESS或CMD_RET_ABORT, 其它值说明操作未完成 ) */

CMD01_GET_STATUS	EQU		022H			;/*获取中断状态并取消中断请求 */
;/*输出: 中断状态 */

CMD00_UNLOCK_USB	EQU		023H			;/*设备方式: 释放当前USB缓冲区 */

CMD01_RD_USB_DATA0	EQU		027H			;/*从当前USB中断的端点缓冲区或者主机端点的接收缓冲区读取数据块 */
;/*输出: 长度, 数据流 */

CMD01_RD_USB_DATA	EQU		028H			;/*设备方式: 从当前USB中断的端点缓冲区读取数据块, 并释放缓冲区, 相当于 CMD01_RD_USB_DATA0 + CMD00_UNLOCK_USB */
;/*输出: 长度, 数据流 */

CMD10_WR_USB_DATA7	EQU		02BH			;/*设备方式: 向USB端点2的发送缓冲区写入数据块 */
;/*输入: 长度, 数据流 */

CMD10_WR_HOST_DATA	EQU		02CH			;/*向USB主机端点的发送缓冲区写入数据块 */
;/*输入: 长度, 数据流 */

CMD01_WR_REQ_DATA	EQU		02DH			;/*向内部指定缓冲区写入请求的数据块 */
;/*输出: 长度 */
;/*输入: 数据流 */

CMD20_WR_OFS_DATA	EQU		02EH			;/*向内部缓冲区指定偏移地址写入数据块 */
;/*输入: 偏移, 长度, 数据流 */

CMD10_SET_FILE_NAME	EQU		02FH			;/*主机文件模式: 设置将要操作的文件的文件名 */
;/*输入: 以0结束的字符串(含结束符0在内长度不超过14个字符) */

;/********************************************************************************************************************** */
;/*主要命令(手册一), 常用, 以下命令总是在操作结束时产生中断通知, 并且总是没有输出数据 */

CMD0H_DISK_CONNECT	EQU		030H			;/*主机文件模式/不支持SD卡: 检查磁盘是否连接 */
;/*输出中断 */

CMD0H_DISK_MOUNT	EQU		031H			;/*主机文件模式: 初始化磁盘并测试磁盘是否就绪 */
;/*输出中断 */

CMD0H_FILE_OPEN		EQU		032H			;/*主机文件模式: 打开文件或者目录(文件夹),或者枚举文件和目录(文件夹) */
;/*输出中断 */

CMD0H_FILE_ENUM_GO	EQU		033H			;/*主机文件模式: 继续枚举文件和目录(文件夹) */
;/*输出中断 */

CMD0H_FILE_CREATE	EQU		034H			;/*主机文件模式: 新建文件,如果文件已经存在那么先删除 */
;/*输出中断 */

CMD0H_FILE_ERASE	EQU		035H			;/*主机文件模式: 删除文件,如果已经打开则直接删除,否则对于文件会先打开再删除,子目录必须先打开 */
;/*输出中断 */

CMD1H_FILE_CLOSE	EQU		036H			;/*主机文件模式: 关闭当前已经打开的文件或者目录(文件夹) */
;/*输入: 是否允许更新文件长度 */
;/*         00H=禁止更新长度, 01H=允许更新长度 */
;/*输出中断 */

CMD1H_DIR_INFO_READ	EQU		037H			;/*主机文件模式: 读取文件的目录信息 */
;/*输入: 指定需要读取的目录信息结构在扇区内的索引号 */
;/*          索引号范围为00H~0FH, 索引号0FFH则为当前已经打开的文件 */
;/*输出中断 */

CMD0H_DIR_INFO_SAVE	EQU		038H			;/*主机文件模式: 保存文件的目录信息 */
;/*输出中断 */

CMD4H_BYTE_LOCATE	EQU		039H			;/*主机文件模式: 以字节为单位移动当前文件指针 */
;/*输入: 偏移字节数(总长度32位,低字节在前) */
;/*输出中断 */

CMD2H_BYTE_READ		EQU		03AH			;/*主机文件模式: 以字节为单位从当前位置读取数据块 */
;/*输入: 请求读取的字节数(总长度16位,低字节在前) */
;/*输出中断 */

CMD0H_BYTE_RD_GO	EQU		03BH			;/*主机文件模式: 继续字节读 */
;/*输出中断 */

CMD2H_BYTE_WRITE	EQU		03CH			;/*主机文件模式: 以字节为单位向当前位置写入数据块 */
;/*输入: 请求写入的字节数(总长度16位,低字节在前) */
;/*输出中断 */

CMD0H_BYTE_WR_GO	EQU		03DH			;/*主机文件模式: 继续字节写 */
;/*输出中断 */

CMD0H_DISK_CAPACITY	EQU		03EH			;/*主机文件模式: 查询磁盘物理容量 */
;/*输出中断 */

CMD0H_DISK_QUERY	EQU		03FH			;/*主机文件模式: 查询磁盘空间信息 */
;/*输出中断 */

CMD0H_DIR_CREATE	EQU		040H			;/*主机文件模式: 新建目录(文件夹)并打开,如果目录已经存在那么直接打开 */
;/*输出中断 */

CMD4H_SEC_LOCATE	EQU		04AH			;/*主机文件模式: 以扇区为单位移动当前文件指针 */
;/*输入: 偏移扇区数(总长度32位,低字节在前) */
;/*输出中断 */

CMD1H_SEC_READ		EQU		04BH			;/*主机文件模式/不支持SD卡: 以扇区为单位从当前位置读取数据块 */
;/*输入: 请求读取的扇区数 */
;/*输出中断 */

CMD1H_SEC_WRITE		EQU		04CH			;/*主机文件模式/不支持SD卡: 以扇区为单位在当前位置写入数据块 */
;/*输入: 请求写入的扇区数 */
;/*输出中断 */

CMD0H_DISK_BOC_CMD	EQU		050H			;/*主机方式/不支持SD卡: 对USB存储器执行BulkOnly传输协议的命令 */
;/*输出中断 */

CMD5H_DISK_READ		EQU		054H			;/*主机方式/不支持SD卡: 从USB存储器读物理扇区 */
;/*输入: LBA物理扇区地址(总长度32位, 低字节在前), 扇区数(01H~FFH) */
;/*输出中断 */

CMD0H_DISK_RD_GO	EQU		055H			;/*主机方式/不支持SD卡: 继续执行USB存储器的物理扇区读操作 */
;/*输出中断 */

CMD5H_DISK_WRITE	EQU		056H			;/*主机方式/不支持SD卡: 向USB存储器写物理扇区 */
;/*输入: LBA物理扇区地址(总长度32位, 低字节在前), 扇区数(01H~FFH) */
;/*输出中断 */

CMD0H_DISK_WR_GO	EQU		057H			;/*主机方式/不支持SD卡: 继续执行USB存储器的物理扇区写操作 */
;/*输出中断 */

;/********************************************************************************************************************** */
;/*辅助命令(手册二), 不太常用或者是为了与CH375和CH372兼容 */

CMD10_SET_USB_SPEED	EQU		004H			;/*设置USB总线速度, 在每次CMD11_SET_USB_MODE设置USB工作模式时会自动恢复到12Mbps全速 */
;/*输入: 总线速度代码 */
;/*          00H=12Mbps全速FullSpeed（默认值）, 01H=1.5Mbps（仅修改频率）, 02H=1.5Mbps低速LowSpeed */

CMD11_GET_DEV_RATE	EQU		00AH			;/*主机方式: 获取当前连接的USB设备的数据速率类型 */
;/*输入: 数据07H */
;/*输出: 数据速率类型 */
;/*          位4为1则是1.5Mbps低速USB设备, 否则是12Mbps全速USB设备 */

CMD11_GET_TOGGLE	EQU		00AH			;/*获取OUT事务的同步状态 */
;/*输入: 数据1AH */
;/*输出: 同步状态 */
;/*          位4为1则OUT事务同步, 否则OUT事务不同步 */

CMD11_READ_VAR8		EQU		00AH			;/*读取指定的8位文件系统变量 */
;/*输入: 变量地址 */
;/*输出: 数据 */

;/*CMD11_GET_MAX_LUN	= CMD11_READ_VAR8( VAR_UDISK_LUN )*/	;/*主机方式: 获取USB存储器最大和当前逻辑单元号 */

CMD20_SET_RETRY		EQU		00BH			;/*主机方式: 设置USB事务操作的重试次数 */
;/*输入: 数据25H, 重试次数 */
;/*          位7为0则收到NAK时不重试, 位7为1位6为0则收到NAK时无限重试, 位7为1位6为1则收到NAK时最多重试3秒, 位5~位0为超时后的重试次数 */

CMD20_WRITE_VAR8	EQU		00BH			;/*设置指定的8位文件系统变量 */
;/*输入: 变量地址, 数据 */

;/*CMD20_SET_DISK_LUN	= CMD20_WRITE_VAR8( VAR_UDISK_LUN )*/	;/*主机方式: 设置USB存储器的当前逻辑单元号 */

CMD14_READ_VAR32	EQU		00CH			;/*读取指定的32位文件系统变量 */
;/*输入: 变量地址 */
;/*输出: 数据(总长度32位,低字节在前) */

CMD50_WRITE_VAR32	EQU		00DH			;/*设置指定的32位文件系统变量 */
;/*输入: 变量地址, 数据(总长度32位,低字节在前) */

CMD01_DELAY_100US	EQU		00FH			;/*延时100uS(串口不支持) */
;/*输出: 延时期间输出0,延时结束输出非0 */

CMD40_SET_USB_ID	EQU		012H			;/*设备方式: 设置USB厂商VID和产品PID */
;/*输入: 厂商ID低字节, 厂商ID高字节, 产品ID低字节, 产品ID高字节 */

CMD10_SET_USB_ADDR	EQU		013H			;/*设置USB地址 */
;/*输入: 地址值 */

CMD01_TEST_CONNECT	EQU		016H			;/*主机方式/不支持SD卡: 检查USB设备连接状态 */
;/*输出: 状态( USB_INT_CONNECT或USB_INT_DISCONNECT或USB_INT_USB_READY, 其它值说明操作未完成 ) */

CMD00_ABORT_NAK		EQU		017H			;/*主机方式: 放弃当前NAK的重试 */

CMD10_SET_ENDP2		EQU		018H			;/*设备方式(串口不支持): 设置USB端点0的接收器 */
;/*输入: 工作方式 */
;/*          位7为1则位6为同步触发位, 否则同步触发位不变 */
;/*          位3~位0为事务响应方式:  0000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

CMD10_SET_ENDP3		EQU		019H			;/*设备方式(串口不支持): 设置USB端点0的发送器 */
;/*输入: 工作方式 */
;/*          位7为1则位6为同步触发位, 否则同步触发位不变 */
;/*          位3~位0为事务响应方式:  0000~1000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

CMD10_SET_ENDP4		EQU		01AH			;/*设备方式(串口不支持): 设置USB端点1的接收器 */
;/*输入: 工作方式 */
;/*          位7为1则位6为同步触发位, 否则同步触发位不变 */
;/*          位3~位0为事务响应方式:  0000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

CMD10_SET_ENDP5		EQU		01BH			;/*设备方式(串口不支持): 设置USB端点1的发送器 */
;/*输入: 工作方式 */
;/*          位7为1则位6为同步触发位, 否则同步触发位不变 */
;/*          位3~位0为事务响应方式:  0000~1000-就绪ACK, 1110-正忙NAK, 1111-错误STALL */

CMD10_SET_ENDP6		EQU		01CH			;/*设置USB端点2/主机端点的接收器 */
;/*输入: 工作方式 */
;/*          位7为1则位6为同步触发位, 否则同步触发位不变 */
;/*          位3~位0为事务响应方式:  0000-就绪ACK, 1101-就绪但不返回ACK, 1110-正忙NAK, 1111-错误STALL */

CMD10_SET_ENDP7		EQU		01DH			;/*设置USB端点2/主机端点的发送器 */
;/*输入: 工作方式 */
;/*          位7为1则位6为同步触发位, 否则同步触发位不变 */
;/*          位3~位0为事务响应方式:  0000-就绪ACK, 1101-就绪但无须应答, 1110-正忙NAK, 1111-错误STALL */

CMD00_DIRTY_BUFFER	EQU		025H			;/*主机文件模式: 清除内部的磁盘和文件缓冲区 */

CMD10_WR_USB_DATA3	EQU		029H			;/*设备方式(串口不支持): 向USB端点0的发送缓冲区写入数据块 */
;/*输入: 长度, 数据流 */

CMD10_WR_USB_DATA5	EQU		02AH			;/*设备方式(串口不支持): 向USB端点1的发送缓冲区写入数据块 */
;/*输入: 长度, 数据流 */

;/********************************************************************************************************************** */
;/*辅助命令(手册二), 不太常用或者是为了与CH375和CH372兼容, 以下命令总是在操作结束时产生中断通知, 并且总是没有输出数据 */

CMD1H_CLR_STALL		EQU		041H			;/*主机方式: 控制传输-清除端点错误 */
;/*输入: 端点号 */
;/*输出中断 */

CMD1H_SET_ADDRESS	EQU		045H			;/*主机方式: 控制传输-设置USB地址 */
;/*输入: 地址值 */
;/*输出中断 */

CMD1H_GET_DESCR		EQU		046H			;/*主机方式: 控制传输-获取描述符 */
;/*输入: 描述符类型 */
;/*输出中断 */

CMD1H_SET_CONFIG	EQU		049H			;/*主机方式: 控制传输-设置USB配置 */
;/*输入: 配置值 */
;/*输出中断 */

CMD0H_AUTO_SETUP	EQU		04DH			;/*主机方式/不支持SD卡: 自动配置USB设备 */
;/*输出中断 */

CMD2H_ISSUE_TKN_X	EQU		04EH			;/*主机方式: 发出同步令牌, 执行事务, 该命令可代替 CMD10_SET_ENDP6/CMD10_SET_ENDP7 + CMD1H_ISSUE_TOKEN */
;/*输入: 同步标志, 事务属性 */
;/*          同步标志的位7为主机端点IN的同步触发位, 位6为主机端点OUT的同步触发位, 位5~位0必须为0 */
;/*          事务属性的低4位是令牌, 高4位是端点号 */
;/*输出中断 */

CMD1H_ISSUE_TOKEN	EQU		04FH			;/*主机方式: 发出令牌, 执行事务, 建议用CMD2H_ISSUE_TKN_X命令 */
;/*输入: 事务属性 */
;/*          低4位是令牌, 高4位是端点号 */
;/*输出中断 */

CMD0H_DISK_INIT		EQU		051H			;/*主机方式/不支持SD卡: 初始化USB存储器 */
;/*输出中断 */

CMD0H_DISK_RESET	EQU		052H			;/*主机方式/不支持SD卡: 控制传输-复位USB存储器 */
;/*输出中断 */

CMD0H_DISK_SIZE		EQU		053H			;/*主机方式/不支持SD卡: 获取USB存储器的容量 */
;/*输出中断 */

CMD0H_DISK_INQUIRY	EQU		058H			;/*主机方式/不支持SD卡: 查询USB存储器特性 */
;/*输出中断 */

CMD0H_DISK_READY	EQU		059H			;/*主机方式/不支持SD卡: 检查USB存储器就绪 */
;/*输出中断 */

CMD0H_DISK_R_SENSE	EQU		05AH			;/*主机方式/不支持SD卡: 检查USB存储器错误 */
;/*输出中断 */

CMD0H_RD_DISK_SEC	EQU		05BH			;/*主机文件模式: 从磁盘读取一个扇区的数据到内部缓冲区 */
;/*输出中断 */

CMD0H_WR_DISK_SEC	EQU		05CH			;/*主机文件模式: 将内部缓冲区的一个扇区的数据写入磁盘 */
;/*输出中断 */

CMD0H_DISK_MAX_LUN	EQU		05DH			;/*主机方式: 控制传输-获取USB存储器最大逻辑单元号 */
;/*输出中断 */

;/********************************************************************************************************************** */
;/*以下定义只是为了兼容CH375的INCLUDE文件中的命令名称格式 */

;#ifndef	_NO_CH375_COMPATIBLE_
CMD_GET_IC_VER		EQU		CMD01_GET_IC_VER
CMD_SET_BAUDRATE	EQU		CMD21_SET_BAUDRATE
CMD_ENTER_SLEEP		EQU		CMD00_ENTER_SLEEP
CMD_RESET_ALL		EQU		CMD00_RESET_ALL
CMD_CHECK_EXIST		EQU		CMD11_CHECK_EXIST
CMD_CHK_SUSPEND		EQU		CMD20_CHK_SUSPEND
CMD_SET_SDO_INT		EQU		CMD20_SET_SDO_INT
CMD_GET_FILE_SIZE	EQU		CMD14_GET_FILE_SIZE
CMD_SET_FILE_SIZE	EQU		CMD50_SET_FILE_SIZE
CMD_SET_USB_MODE	EQU		CMD11_SET_USB_MODE
CMD_GET_STATUS		EQU		CMD01_GET_STATUS
CMD_UNLOCK_USB		EQU		CMD00_UNLOCK_USB
CMD_RD_USB_DATA0	EQU		CMD01_RD_USB_DATA0
CMD_RD_USB_DATA		EQU		CMD01_RD_USB_DATA
CMD_WR_USB_DATA7	EQU		CMD10_WR_USB_DATA7
CMD_WR_HOST_DATA	EQU		CMD10_WR_HOST_DATA
CMD_WR_REQ_DATA		EQU		CMD01_WR_REQ_DATA
CMD_WR_OFS_DATA		EQU		CMD20_WR_OFS_DATA
CMD_SET_FILE_NAME	EQU		CMD10_SET_FILE_NAME
CMD_DISK_CONNECT	EQU		CMD0H_DISK_CONNECT
CMD_DISK_MOUNT		EQU		CMD0H_DISK_MOUNT
CMD_FILE_OPEN		EQU		CMD0H_FILE_OPEN
CMD_FILE_ENUM_GO	EQU		CMD0H_FILE_ENUM_GO
CMD_FILE_CREATE		EQU		CMD0H_FILE_CREATE
CMD_FILE_ERASE		EQU		CMD0H_FILE_ERASE
CMD_FILE_CLOSE		EQU		CMD1H_FILE_CLOSE
CMD_DIR_INFO_READ	EQU		CMD1H_DIR_INFO_READ
CMD_DIR_INFO_SAVE	EQU		CMD0H_DIR_INFO_SAVE
CMD_BYTE_LOCATE		EQU		CMD4H_BYTE_LOCATE
CMD_BYTE_READ		EQU		CMD2H_BYTE_READ
CMD_BYTE_RD_GO		EQU		CMD0H_BYTE_RD_GO
CMD_BYTE_WRITE		EQU		CMD2H_BYTE_WRITE
CMD_BYTE_WR_GO		EQU		CMD0H_BYTE_WR_GO
CMD_DISK_CAPACITY	EQU		CMD0H_DISK_CAPACITY
CMD_DISK_QUERY		EQU		CMD0H_DISK_QUERY
CMD_DIR_CREATE		EQU		CMD0H_DIR_CREATE
CMD_SEC_LOCATE		EQU		CMD4H_SEC_LOCATE
CMD_SEC_READ		EQU		CMD1H_SEC_READ
CMD_SEC_WRITE		EQU		CMD1H_SEC_WRITE
CMD_DISK_BOC_CMD	EQU		CMD0H_DISK_BOC_CMD
CMD_DISK_READ		EQU		CMD5H_DISK_READ
CMD_DISK_RD_GO		EQU		CMD0H_DISK_RD_GO
CMD_DISK_WRITE		EQU		CMD5H_DISK_WRITE
CMD_DISK_WR_GO		EQU		CMD0H_DISK_WR_GO
CMD_SET_USB_SPEED	EQU		CMD10_SET_USB_SPEED
CMD_GET_DEV_RATE	EQU		CMD11_GET_DEV_RATE
CMD_GET_TOGGLE		EQU		CMD11_GET_TOGGLE
CMD_READ_VAR8		EQU		CMD11_READ_VAR8
CMD_SET_RETRY		EQU		CMD20_SET_RETRY
CMD_WRITE_VAR8		EQU		CMD20_WRITE_VAR8
CMD_READ_VAR32		EQU		CMD14_READ_VAR32
CMD_WRITE_VAR32		EQU		CMD50_WRITE_VAR32
CMD_DELAY_100US		EQU		CMD01_DELAY_100US
CMD_SET_USB_ID		EQU		CMD40_SET_USB_ID
CMD_SET_USB_ADDR	EQU		CMD10_SET_USB_ADDR
CMD_TEST_CONNECT	EQU		CMD01_TEST_CONNECT
CMD_ABORT_NAK		EQU		CMD00_ABORT_NAK
CMD_SET_ENDP2		EQU		CMD10_SET_ENDP2
CMD_SET_ENDP3		EQU		CMD10_SET_ENDP3
CMD_SET_ENDP4		EQU		CMD10_SET_ENDP4
CMD_SET_ENDP5		EQU		CMD10_SET_ENDP5
CMD_SET_ENDP6		EQU		CMD10_SET_ENDP6
CMD_SET_ENDP7		EQU		CMD10_SET_ENDP7
CMD_DIRTY_BUFFER	EQU		CMD00_DIRTY_BUFFER
CMD_WR_USB_DATA3	EQU		CMD10_WR_USB_DATA3
CMD_WR_USB_DATA5	EQU		CMD10_WR_USB_DATA5
CMD_CLR_STALL		EQU		CMD1H_CLR_STALL
CMD_SET_ADDRESS		EQU		CMD1H_SET_ADDRESS
CMD_GET_DESCR		EQU		CMD1H_GET_DESCR
CMD_SET_CONFIG		EQU		CMD1H_SET_CONFIG
CMD_AUTO_SETUP		EQU		CMD0H_AUTO_SETUP
CMD_ISSUE_TKN_X		EQU		CMD2H_ISSUE_TKN_X
CMD_ISSUE_TOKEN		EQU		CMD1H_ISSUE_TOKEN
CMD_DISK_INIT		EQU		CMD0H_DISK_INIT
CMD_DISK_RESET		EQU		CMD0H_DISK_RESET
CMD_DISK_SIZE		EQU		CMD0H_DISK_SIZE
CMD_DISK_INQUIRY	EQU		CMD0H_DISK_INQUIRY
CMD_DISK_READY		EQU		CMD0H_DISK_READY
CMD_DISK_R_SENSE	EQU		CMD0H_DISK_R_SENSE
CMD_RD_DISK_SEC		EQU		CMD0H_RD_DISK_SEC
CMD_WR_DISK_SEC		EQU		CMD0H_WR_DISK_SEC
CMD_DISK_MAX_LUN	EQU		CMD0H_DISK_MAX_LUN
;#endif

;/********************************************************************************************************************** */
;/*并口方式, 状态端口(读命令端口)的位定义 */
;#ifndef	PARA_STATE_INTB
PARA_STATE_INTB		EQU		080H			;/*并口方式状态端口的位7: 中断标志,低有效 */
PARA_STATE_BUSY		EQU		010H			;/*并口方式状态端口的位4: 忙标志,高有效 */
;#endif

;/********************************************************************************************************************** */
;/*串口方式, 操作命令前的引导同步码 */
;#ifndef	SER_CMD_TIMEOUT
SER_CMD_TIMEOUT		EQU		020H			;/*串口命令超时时间, 单位为mS, 同步码之间及同步码与命令码之间的间隔应该尽量短, 超时后的处理方式为丢弃 */
SER_SYNC_CODE1		EQU		057H			;/*启动操作的第1个串口同步码 */
SER_SYNC_CODE2		EQU		0ABH			;/*启动操作的第2个串口同步码 */
;#endif

;/********************************************************************************************************************** */
;/*操作状态 */

;#ifndef	CMD_RET_SUCCESS
CMD_RET_SUCCESS		EQU		051H			;/*命令操作成功 */
CMD_RET_ABORT		EQU		05FH			;/*命令操作失败 */
;#endif

;/********************************************************************************************************************** */
;/*USB中断状态 */

;#ifndef	USB_INT_EP0_SETUP

;/*以下状态代码为特殊事件中断, 如果通过CMD20_CHK_SUSPEND启用USB总线挂起检查, 那么必须处理USB总线挂起和睡眠唤醒的中断状态 */
USB_INT_USB_SUSPEND	EQU		005H			;/*USB总线挂起事件 */
USB_INT_WAKE_UP		EQU		006H			;/*从睡眠中被唤醒事件 */

;/*以下状态代码0XH用于USB设备方式 */
;/*  内置固件模式下只需要处理: USB_INT_EP1_OUT, USB_INT_EP1_IN, USB_INT_EP2_OUT, USB_INT_EP2_IN */
;/*  位7-位4为0000 */
;/*  位3-位2指示当前事务, 00=OUT, 10=IN, 11=SETUP */
;/*  位1-位0指示当前端点, 00=端点0, 01=端点1, 10=端点2, 11=USB总线复位 */
USB_INT_EP0_SETUP	EQU		00CH			;/*USB端点0的SETUP */
USB_INT_EP0_OUT		EQU		000H			;/*USB端点0的OUT */
USB_INT_EP0_IN		EQU		008H			;/*USB端点0的IN */
USB_INT_EP1_OUT		EQU		001H			;/*USB端点1的OUT */
USB_INT_EP1_IN		EQU		009H			;/*USB端点1的IN */
USB_INT_EP2_OUT		EQU		002H			;/*USB端点2的OUT */
USB_INT_EP2_IN		EQU		00AH			;/*USB端点2的IN */
;/*USB_INT_BUS_RESET	EQU		00000XX11B */		;/*USB总线复位 */
USB_INT_BUS_RESET1	EQU		003H			;/*USB总线复位 */
USB_INT_BUS_RESET2	EQU		007H			;/*USB总线复位 */
USB_INT_BUS_RESET3	EQU		00BH			;/*USB总线复位 */
USB_INT_BUS_RESET4	EQU		00FH			;/*USB总线复位 */

;#endif

;/*以下状态代码2XH-3XH用于USB主机方式的通讯失败代码 */
;/*  位7-位6为00 */
;/*  位5为1 */
;/*  位4指示当前接收的数据包是否同步 */
;/*  位3-位0指示导致通讯失败时USB设备的应答: 0010=ACK, 1010=NAK, 1110=STALL, 0011=DATA0, 1011=DATA1, XX00=超时 */
;/*USB_INT_RET_ACK	EQU		0001X0010B */H			;/*错误:对于IN事务返回ACK */
;/*USB_INT_RET_NAK	EQU		0001X1010B */H			;/*错误:返回NAK */
;/*USB_INT_RET_STALL	EQU		0001X1110B */		;/*错误:返回STALL */
;/*USB_INT_RET_DATA0	EQU		0001X0011B */		;/*错误:对于OUT/SETUP事务返回DATA0 */
;/*USB_INT_RET_DATA1	EQU		0001X1011B */		;/*错误:对于OUT/SETUP事务返回DATA1 */
;/*USB_INT_RET_TOUT	EQU		0001XXX00B */H			;/*错误:返回超时 */
;/*USB_INT_RET_TOGX	EQU		00010X011B */H			;/*错误:对于IN事务返回数据不同步 */
;/*USB_INT_RET_PID	EQU		0001XXXXXB */H			;/*错误:未定义 */

;/*以下状态代码1XH用于USB主机方式的操作状态代码 */
;#ifndef	USB_INT_SUCCESS
USB_INT_SUCCESS		EQU		014H			;/*USB事务或者传输操作成功 */
USB_INT_CONNECT		EQU		015H			;/*检测到USB设备连接事件, 可能是新连接或者断开后重新连接 */
USB_INT_DISCONNECT	EQU		016H			;/*检测到USB设备断开事件 */
USB_INT_BUF_OVER	EQU		017H			;/*USB传输的数据有误或者数据太多缓冲区溢出 */
USB_INT_USB_READY	EQU		018H			;/*USB设备已经被初始化(已经分配USB地址) */
USB_INT_DISK_READ	EQU		01DH			;/*USB存储器请求数据读出 */
USB_INT_DISK_WRITE	EQU		01EH			;/*USB存储器请求数据写入 */
USB_INT_DISK_ERR	EQU		01FH			;/*USB存储器操作失败 */
;#endif

;/*以下状态代码用于主机文件模式下的文件系统错误码 */
;#ifndef	ERR_DISK_DISCON
ERR_DISK_DISCON		EQU		082H			;/*磁盘尚未连接,可能磁盘已经断开 */
ERR_LARGE_SECTOR	EQU		084H			;/*磁盘的扇区太大,只支持每扇区512字节 */
ERR_TYPE_ERROR		EQU		092H			;/*磁盘分区类型不支持,只支持FAT12/FAT16/BigDOS/FAT32,需要由磁盘管理工具重新分区 */
ERR_BPB_ERROR		EQU		0A1H			;/*磁盘尚未格式化,或者参数错误,需要由WINDOWS采用默认参数重新格式化 */
ERR_DISK_FULL		EQU		0B1H			;/*磁盘文件太满,剩余空间太少或者已经没有,需要磁盘整理 */
ERR_FDT_OVER		EQU		0B2H			;/*目录(文件夹)内文件太多,没有空闲的目录项,FAT12/FAT16根目录下的文件数应该少于512个,需要磁盘整理 */
ERR_FILE_CLOSE		EQU		0B4H			;/*文件已经关闭,如果需要使用,应该重新打开文件 */
ERR_OPEN_DIR		EQU		041H			;/*指定路径的目录(文件夹)被打开 */
ERR_MISS_FILE		EQU		042H			;/*指定路径的文件没有找到,可能是文件名称错误 */
ERR_FOUND_NAME		EQU		043H			;/*搜索到相匹配的文件名,或者是要求打开目录(文件夹)而实际结果却打开了文件 */
;/* 以下文件系统错误码用于文件系统子程序 */
ERR_MISS_DIR		EQU		0B3H			;/*指定路径的某个子目录(文件夹)没有找到,可能是目录名称错误 */
ERR_LONG_BUF_OVER	EQU		048H			;/*长文件缓冲区溢出 */
ERR_LONG_NAME_ERR	EQU		049H			;/*短文件名没有对应的长文件名或者长文件名错误 */
ERR_NAME_EXIST		EQU		04AH			;/*同名的短文件已经存在,建议重新生成另外一个短文件名 */
;#endif

;/********************************************************************************************************************** */
;/*以下状态代码用于主机文件模式下的磁盘及文件状态, VAR_DISK_STATUS */
;#ifndef	DEF_DISK_UNKNOWN
DEF_DISK_UNKNOWN	EQU		000H			;/*尚未初始化,未知状态 */
DEF_DISK_DISCONN	EQU		001H			;/*磁盘没有连接或者已经断开 */
DEF_DISK_CONNECT	EQU		002H			;/*磁盘已经连接,但是尚未初始化或者无法识别该磁盘 */
DEF_DISK_MOUNTED	EQU		003H			;/*磁盘已经初始化成功,但是尚未分析文件系统或者文件系统不支持 */
DEF_DISK_READY		EQU		010H			;/*已经分析磁盘的文件系统并且能够支持 */
DEF_DISK_OPEN_ROOT	EQU		012H			;/*已经打开根目录,使用后必须关闭,注意FAT12/FAT16根目录是固定长度 */
DEF_DISK_OPEN_DIR	EQU		013H			;/*已经打开子目录目录(文件夹) */
DEF_DISK_OPEN_FILE	EQU		014H			;/*已经打开文件 */
;#endif

;/********************************************************************************************************************** */
;/*文件系统常用定义 */

;#ifndef	DEF_SECTOR_SIZE
DEF_SECTOR_SIZE		EQU		200H			;/*U盘或者SD卡默认的物理扇区的大小 */
;#endif

;#ifndef	DEF_WILDCARD_CHAR
DEF_WILDCARD_CHAR	EQU		02AH			;/*路径名的通配符 '*' */
DEF_SEPAR_CHAR1		EQU		05CH			;/*路径名的分隔符 '\' */
DEF_SEPAR_CHAR2		EQU		02FH			;/*路径名的分隔符 '/' */
DEF_FILE_YEAR		EQU		2004			;/*默认文件日期: 2004年 */
DEF_FILE_MONTH		EQU		1				;/*默认文件日期: 1月 */
DEF_FILE_DATE		EQU		1				;/*默认文件日期: 1日 */
;#endif

;#ifndef	ATTR_DIRECTORY

;/*FAT数据区中文件目录信息 */
;typedef struct _FAT_DIR_INFO {
;	UINT8	DIR_Name[11];					;/*00H,文件名,共11字节,不足处填空格 */
;	UINT8	DIR_Attr;						;/*0BH,文件属性,参考后面的说明 */
;	UINT8	DIR_NTRes;						;/*0CH */
;	UINT8	DIR_CrtTimeTenth;				;/*0DH,文件创建的时间,以0.1秒单位计数 */
;	UINT16	DIR_CrtTime;					;/*0EH,文件创建的时间 */
;	UINT16	DIR_CrtDate;					;/*10H,文件创建的日期 */
;	UINT16	DIR_LstAccDate;					;/*12H,最近一次存取操作的日期 */
;	UINT16	DIR_FstClusHI;					;/*14H */
;	UINT16	DIR_WrtTime;					;/*16H,文件修改时间,参考前面的宏MAKE_FILE_TIME */
;	UINT16	DIR_WrtDate;					;/*18H,文件修改日期,参考前面的宏MAKE_FILE_DATE */
;	UINT16	DIR_FstClusLO;					;/*1AH */
;	UINT32	DIR_FileSize;					;/*1CH,文件长度 */
;} FAT_DIR_INFO, *P_FAT_DIR_INFO;			;/*20H */
FAT_DIR_INF_Name		EQU		000H		;/*00H,文件名,共11字节,不足处填空格 */
FAT_DIR_INF_Attr		EQU		00BH		;/*0BH,文件属性,参考后面的说明 */
FAT_DIR_INF_NTRes		EQU		00CH		;/*0CH */
FAT_DIR_INF_CrtTimeTen	EQU		00DH		;/*0DH,文件创建的时间,以0.1秒单位计数 */
FAT_DIR_INF_CrtTime		EQU		00EH		;/*0EH,文件创建的时间 */
FAT_DIR_INF_CrtDate		EQU		010H		;/*10H,文件创建的日期 */
FAT_DIR_INF_LstAccDate	EQU		012H		;/*12H,最近一次存取操作的日期 */
FAT_DIR_INF_FstClusHI	EQU		014H		;/*14H */
FAT_DIR_INF_WrtTime		EQU		016H		;/*16H,文件修改时间,参考前面的宏MAKE_FILE_TIME */
FAT_DIR_INF_WrtDate		EQU		018H		;/*18H,文件修改日期,参考前面的宏MAKE_FILE_DATE */
FAT_DIR_INF_FstClusLO	EQU		01AH		;/*1AH */
FAT_DIR_INF_FileSize	EQU		01CH		;/*1CH,文件长度 */
FAT_DIR_INF_STRUCT_SIZE	EQU		020H		;结构大小

;/*文件属性 */
ATTR_READ_ONLY			EQU		001H		;/*文件为只读属性 */
ATTR_HIDDEN				EQU		002H		;/*文件为隐含属性 */
ATTR_SYSTEM				EQU		004H		;/*文件为系统属性 */
ATTR_VOLUME_ID			EQU		008H		;/*卷标 */
ATTR_DIRECTORY			EQU		010H		;/*子目录(文件夹) */
ATTR_ARCHIVE			EQU		020H		;/*文件为存档属性 */
ATTR_LONG_NAME			EQU		( ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID )	;/*长文件名属性 */
ATTR_LONG_NAME_MASK		EQU		( ATTR_LONG_NAME | ATTR_DIRECTORY | ATTR_ARCHIVE )
;/*文件属性 UINT8 */
;/*bit0 bit1 bit2 bit3 bit4 bit5 bit6 bit7 */
;/* 只   隐   系   卷   目   存   未定义   */
;/* 读   藏   统   标   录   档            */
;/*文件时间 UINT16 */
;/*Time = (Hour<<11) + (Minute<<5) + (Second>>1) */
;#define MAKE_FILE_TIME( h, m, s )	( (h<<11) + (m<<5) + (s>>1) )	;/*生成指定时分秒的文件时间数据 */
;/*文件日期 UINT16 */
;/*Date = ((Year-1980)<<9) + (Month<<5) + Day */
;#define MAKE_FILE_DATE( y, m, d )	( ((y-1980)<<9) + (m<<5) + d )	;/*生成指定年月日的文件日期数据 */

LONE_NAME_MAX_CHAR		EQU		(255*2)		;/*长文件名最多字符数/字节数 */
LONG_NAME_PER_DIR		EQU		(13*2)		;/*长文件名在每个文件目录信息结构中的字符数/字节数 */

;#endif

;/********************************************************************************************************************** */
;/*SCSI命令和数据输入输出结构 */

;#ifndef	SPC_CMD_INQUIRY

;/*SCSI命令码 */
SPC_CMD_INQUIRY			EQU		012H
SPC_CMD_READ_CAPACITY	EQU		025H
SPC_CMD_READ10			EQU		028H
SPC_CMD_WRITE10			EQU		02AH
SPC_CMD_TEST_READY		EQU		000H
SPC_CMD_REQUEST_SENSE	EQU		003H
SPC_CMD_MODESENSE6		EQU		01AH
SPC_CMD_MODESENSE10		EQU		05AH
SPC_CMD_START_STOP		EQU		01BH

;/*BulkOnly协议的命令块 */
;typedef struct _BULK_ONLY_CBW {
;	UINT32	CBW_Sig;
;	UINT32	CBW_Tag;
;	UINT8	CBW_DataLen0;					;/*08H,输入: 数据传输长度,对于输入数据其有效值是0到48,对于输出数据其有效值为0到33 */
;	UINT8	CBW_DataLen1;
;	UINT16	CBW_DataLen2;
;	UINT8	CBW_Flag;						;/*0CH,输入: 传输方向等标志,位7为1则输入数据,位为0则输出数据或者没有数据 */
;	UINT8	CBW_LUN;
;	UINT8	CBW_CB_Len;						;/*0EH,输入: 命令块的长度,有效值是1到16 */
;	UINT8	CBW_CB_Buf[16];					;/*0FH,输入: 命令块,该缓冲区最多为16个字节 */
;} BULK_ONLY_CBW, *P_BULK_ONLY_CBW;			;/*BulkOnly协议的命令块, 输入CBW结构 */
BO_CBW_DataLen0			EQU		008H		;/*08H,输入: 数据传输长度,对于输入数据其有效值是0到48,对于输出数据其有效值为0到33 */
BO_CBW_Flag				EQU		00CH		;/*0CH,输入: 传输方向等标志,位7为1则输入数据,位为0则输出数据或者没有数据 */
BO_CBW_CB_Len			EQU		00EH		;/*0EH,输入: 命令块的长度,有效值是1到16 */
BO_CBW_CB_Buf			EQU		00FH		;/*0FH,输入: 命令块,该缓冲区最多为16个字节 */
BO_CBW_STRUCT_SIZE		EQU		01FH		;结构大小

;/*INQUIRY命令的返回数据 */
;typedef struct _INQUIRY_DATA {
;	UINT8	DeviceType;					;/*00H, 设备类型 */
;	UINT8	RemovableMedia;				;/*01H, 位7为1说明是移动存储 */
;	UINT8	Versions;					;/*02H, 协议版本 */
;	UINT8	DataFormatAndEtc;			;/*03H, 指定返回数据格式 */
;	UINT8	AdditionalLength;			;/*04H, 后续数据的长度 */
;	UINT8	Reserved1;
;	UINT8	Reserved2;
;	UINT8	MiscFlag;					;/*07H, 一些控制标志 */
;	UINT8	VendorIdStr[8];				;/*08H, 厂商信息 */
;	UINT8	ProductIdStr[16];			;/*10H, 产品信息 */
;	UINT8	ProductRevStr[4];			;/*20H, 产品版本 */
;} INQUIRY_DATA, *P_INQUIRY_DATA;		;/*24H */
INQUIRY_DeviceType		EQU		000H
INQUIRY_RemovableMedia	EQU		001H
INQUIRY_VendorIdStr		EQU		008H
INQUIRY_ProductIdStr	EQU		010H
INQUIRY_ProductRevStr	EQU		020H
INQUIRY_STRUCT_SIZE		EQU		024H		;结构大小

;/*REQUEST SENSE命令的返回数据 */
;typedef struct _SENSE_DATA {
;	UINT8	ErrorCode;					;/*00H, 错误代码及有效位 */
;	UINT8	SegmentNumber;
;	UINT8	SenseKeyAndEtc;				;/*02H, 主键码 */
;	UINT8	Information0;
;	UINT8	Information1;
;	UINT8	Information2;
;	UINT8	Information3;
;	UINT8	AdditSenseLen;				;/*07H, 后续数据的长度 */
;	UINT8	CmdSpecInfo[4];
;	UINT8	AdditSenseCode;				;/*0CH, 附加键码 */
;	UINT8	AddSenCodeQual;				;/*0DH, 详细的附加键码 */
;	UINT8	FieldReplaUnit;
;	UINT8	SenseKeySpec[3];
;} SENSE_DATA, *P_SENSE_DATA;			;/*12H */

;#endif

;/********************************************************************************************************************** */
;/*主机文件模式下的数据输入和输出结构 */

;#ifndef	MAX_FILE_NAME_LEN

MAX_FILE_NAME_LEN		EQU		(13+1)		;/*文件名最大长度,最大长度是1个根目录符+8个主文件名+1个小数点+3个类型名+结束符=14 */

;/*命令的输入数据和输出数据 */
;typedef union _CH376_CMD_DATA {
;	struct {
;		UINT8	mBuffer[ MAX_FILE_NAME_LEN ];
;	} Default;
;
;	INQUIRY_DATA	DiskMountInq;			;/*返回: INQUIRY命令的返回数据 */
;											;/*CMD0H_DISK_MOUNT: 初始化磁盘并测试磁盘是否就绪,首次执行时 */
;
;	FAT_DIR_INFO	OpenDirInfo;			;/*返回: 枚举到的文件目录信息 */
;											;/*CMD0H_FILE_OPEN: 枚举文件和目录(文件夹) */
;
;	FAT_DIR_INFO	EnumDirInfo;			;/*返回: 枚举到的文件目录信息 */
;											;/*CMD0H_FILE_ENUM_GO: 继续枚举文件和目录(文件夹) */
;
;	struct {
;		UINT8	mUpdateFileSz;				;/*输入参数: 是否允许更新文件长度, 0则禁止更新长度 */
;	} FileCLose;							;/*CMD1H_FILE_CLOSE: 关闭当前已经打开的文件 */
;
;	struct {
;		UINT8	mDirInfoIndex;				;/*输入参数: 指定需要读取的目录信息结构在扇区内的索引号, 0FFH则为当前已经打开的文件 */
;	} DirInfoRead;							;/*CMD1H_DIR_INFO_READ: 读取文件的目录信息 */
;
;	union {
;		UINT32	mByteOffset;				;/*输入参数: 偏移字节数,以字节为单位的偏移量(总长度32位,低字节在前) */
;		UINT32	mSectorLba;					;/*返回: 当前文件指针对应的绝对线性扇区号,0FFFFFFFFH则已到文件尾(总长度32位,低字节在前) */
;	} ByteLocate;							;/*CMD4H_BYTE_LOCATE: 以字节为单位移动当前文件指针 */
;
;	struct {
;		UINT16	mByteCount;					;/*输入参数: 请求读取的字节数(总长度16位,低字节在前) */
;	} ByteRead;								;/*CMD2H_BYTE_READ: 以字节为单位从当前位置读取数据块 */
;
;	struct {
;		UINT16	mByteCount;					;/*输入参数: 请求写入的字节数(总长度16位,低字节在前) */
;	} ByteWrite;							;/*CMD2H_BYTE_WRITE: 以字节为单位向当前位置写入数据块 */
;
;	union {
;		UINT32	mSectorOffset;				;/*输入参数: 偏移扇区数,以扇区为单位的偏移量(总长度32位,低字节在前) */
;		UINT32	mSectorLba;					;/*返回: 当前文件指针对应的绝对线性扇区号,0FFFFFFFFH则已到文件尾(总长度32位,低字节在前) */
;	} SectorLocate;							;/*CMD4H_SEC_LOCATE: 以扇区为单位移动当前文件指针 */
;
;	struct {
;		UINT8	mSectorCount;				;/*输入参数: 请求读取的扇区数 */
;											;/*返回: 允许读取的扇区数 */
;		UINT8	mReserved1;
;		UINT8	mReserved2;
;		UINT8	mReserved3;
;		UINT32	mStartSector;				;/*返回: 允许读取的扇区块的起始绝对线性扇区号(总长度32位,低字节在前) */
;	} SectorRead;							;/*CMD1H_SEC_READ: 以扇区为单位从当前位置读取数据块 */
;
;	struct {
;		UINT8	mSectorCount;				;/*输入参数: 请求写入的扇区数 */
;											;/*返回: 允许写入的扇区数 */
;		UINT8	mReserved1;
;		UINT8	mReserved2;
;		UINT8	mReserved3;
;		UINT32	mStartSector;				;/*返回: 允许写入的扇区块的起始绝对线性扇区号(总长度32位,低字节在前) */
;	} SectorWrite;							;/*CMD1H_SEC_WRITE: 以扇区为单位在当前位置写入数据块 */
;
;	struct {
;		UINT32	mDiskSizeSec;				;/*返回: 整个物理磁盘的总扇区数(总长度32位,低字节在前) */
;	} DiskCapacity;							;/*CMD0H_DISK_CAPACITY: 查询磁盘物理容量 */
;
;	struct {
;		UINT32	mTotalSector;				;/*返回: 当前逻辑盘的总扇区数(总长度32位,低字节在前) */
;		UINT32	mFreeSector;				;/*返回: 当前逻辑盘的剩余扇区数(总长度32位,低字节在前) */
;		UINT8	mDiskFat;					;/*返回: 当前逻辑盘的FAT类型,1-FAT12,2-FAT16,3-FAT32 */
;	} DiskQuery;							;/*CMD_DiskQuery, 查询磁盘信息 */
;
;	BULK_ONLY_CBW	DiskBocCbw;				;/*输入参数: CBW命令结构 */
;											;/*CMD0H_DISK_BOC_CMD: 对USB存储器执行BulkOnly传输协议的命令 */
;
;	struct {
;		UINT8	mMaxLogicUnit;				;/*返回: USB存储器的最大逻辑单元号 */
;	} DiskMaxLun;							;/*CMD0H_DISK_MAX_LUN: 控制传输-获取USB存储器最大逻辑单元号 */
;
;	INQUIRY_DATA	DiskInitInq;			;/*返回: INQUIRY命令的返回数据 */
;											;/*CMD0H_DISK_INIT: 初始化USB存储器 */
;
;	INQUIRY_DATA	DiskInqData;			;/*返回: INQUIRY命令的返回数据 */
;											;/*CMD0H_DISK_INQUIRY: 查询USB存储器特性 */
;
;	SENSE_DATA		ReqSenseData;			;/*返回: REQUEST SENSE命令的返回数据 */
;											;/*CMD0H_DISK_R_SENSE: 检查USB存储器错误 */
;
;	struct {
;		UINT32	mDiskSizeSec;				;/*返回: 整个物理磁盘的总扇区数(总长度32位,高字节在前) */
;	} DiskSize;								;/*CMD0H_DISK_SIZE: 获取USB存储器的容量 */
;
;	struct {
;		UINT32	mStartSector;				;/*输入参数: LBA扇区地址(总长度32位,低字节在前) */
;		UINT8	mSectorCount;				;/*输入参数: 请求读取的扇区数 */
;	} DiskRead;								;/*CMD5H_DISK_READ: 从USB存储器读数据块(以扇区为单位) */
;
;	struct {
;		UINT32	mStartSector;				;/*输入参数: LBA扇区地址(总长度32位,低字节在前) */
;		UINT8	mSectorCount;				;/*输入参数: 请求写入的扇区数 */
;	} DiskWrite;							;/*CMD5H_DISK_WRITE: 向USB存储器写数据块(以扇区为单位) */
;} CH376_CMD_DATA, *P_CH376_CMD_DATA;

;#endif

;/********************************************************************************************************************** */
;/*主机文件模式下的文件系统变量的地址 */

;#ifndef	VAR_FILE_SIZE

;/*8位/单字节变量 */
VAR_SYS_BASE_INFO	EQU		020H			;/*当前系统的基本信息 */
;/*          位6用于指示USB存储设备的子类别SubClass-Code, 位6为0则说明子类别为6, 位6为1则说明子类别是非6的其它值 */
;/*          位5用于指示USB设备方式下的USB配置状态和USB主机方式下的USB设备连接状态 */
;/*               USB设备方式下, 位5为1则USB配置完成, 位5位0则尚未配置 */
;/*               USB主机方式下, 位5为1则USB端口存在USB设备, 位5位0则USB端口没有USB设备 */
;/*          位4用于指示USB设备方式下的缓冲区锁定状态, 位4为1则说明USB缓冲区处于锁定状态, 位6为1则说明已经释放 */
;/*          其它位, 保留,请勿修改 */
VAR_RETRY_TIMES		EQU		025H			;/*USB事务操作的重试次数 */
;/*          位7为0则收到NAK时不重试, 位7为1位6为0则收到NAK时无限重试(可以用CMD_ABORT_NAK命令放弃重试), 位7为1位6为1则收到NAK时最多重试3秒 */
;/*          位5~位0为超时后的重试次数 */
VAR_FILE_BIT_FLAG	EQU		026H			;/*主机文件模式下的位标志 */
;/*          位1和位0, 逻辑盘的FAT文件系统标志, 00-FAT12, 01-FAT16, 10-FAT32, 11-非法 */
;/*          位2, 当前缓冲区中的FAT表数据是否被修改标志, 0-未修改, 1-已修改 */
;/*          位3, 文件长度需要修改标志, 当前文件被追加数据, 0-未追加无需修改, 1-已追加需要修改 */
;/*          其它位, 保留,请勿修改 */
VAR_DISK_STATUS		EQU		02BH			;/*主机文件模式下的磁盘及文件状态 */
VAR_SD_BIT_FLAG		EQU		030H			;/*主机文件模式下SD卡的位标志 */
;/*          位0, SD卡版本, 0-只支持SD第一版,1-支持SD第二版 */
;/*          位1, 自动识别, 0-SD卡, 1-MMC卡 */
;/*          位2, 自动识别, 0-标准容量SD卡, 1-大容量SD卡(HC-SD) */
;/*          位4, ACMD41命令超时 */
;/*          位5, CMD1命令超时 */
;/*          位6, CMD58命令超时 */
;/*          其它位, 保留,请勿修改 */
VAR_UDISK_TOGGLE	EQU		031H			;/*USB存储设备的BULK-IN/BULK-OUT端点的同步标志 */
;/*          位7, Bulk-In端点的同步标志 */
;/*          位6, Bulk-In端点的同步标志 */
;/*          位5~位0, 必须为0 */
VAR_UDISK_LUN		EQU		034H			;/*USB存储设备的逻辑单元号 */
;/*          位7~位4, USB存储设备的当前逻辑单元号,CH376初始化USB存储设备后,默认是访问0#逻辑单元 */
;/*          位3~位0, USB存储设备的最大逻辑单元号,加1后等于逻辑单元数 */
VAR_SEC_PER_CLUS	EQU		038H			;/*逻辑盘的每簇扇区数 */
VAR_FILE_DIR_INDEX	EQU		03BH			;/*当前文件目录信息在扇区内的索引号 */
VAR_CLUS_SEC_OFS	EQU		03CH			;/*当前文件指针在簇内的扇区偏移,为0xFF则指向文件末尾,簇结束 */

;/*32位/4字节变量 */
VAR_DISK_ROOT		EQU		044H			;/*对于FAT16盘为根目录占用扇区数,对于FAT32盘为根目录起始簇号(总长度32位,低字节在前) */
VAR_DSK_TOTAL_CLUS	EQU		048H			;/*逻辑盘的总簇数(总长度32位,低字节在前) */
VAR_DSK_START_LBA	EQU		04CH			;/*逻辑盘的起始绝对扇区号LBA(总长度32位,低字节在前) */
VAR_DSK_DAT_START	EQU		050H			;/*逻辑盘的数据区域的起始LBA(总长度32位,低字节在前) */
VAR_LBA_BUFFER		EQU		054H			;/*当前磁盘数据缓冲区的数据对应的LBA(总长度32位,低字节在前) */
VAR_LBA_CURRENT		EQU		058H			;/*当前读写的磁盘起始LBA地址(总长度32位,低字节在前) */
VAR_FAT_DIR_LBA		EQU		05CH			;/*当前文件目录信息所在的扇区LBA地址(总长度32位,低字节在前) */
VAR_START_CLUSTER	EQU		060H			;/*当前文件或者目录(文件夹)的起始簇号(总长度32位,低字节在前) */
VAR_CURRENT_CLUST	EQU		064H			;/*当前文件的当前簇号(总长度32位,低字节在前) */
VAR_FILE_SIZE		EQU		068H			;/*当前文件的长度(总长度32位,低字节在前) */
VAR_CURRENT_OFFSET	EQU		06CH			;/*当前文件指针,当前读写位置的字节偏移(总长度32位,低字节在前) */

;#endif

;/********************************************************************************************************************** */
;/*常用USB定义 */

;/*USB的包标识PID, 主机方式可能用到 */
;#ifndef	DEF_USB_PID_SETUP
DEF_USB_PID_NULL	EQU		000H			;/*保留PID, 未定义 */
DEF_USB_PID_SOF		EQU		005H
DEF_USB_PID_SETUP	EQU		00DH
DEF_USB_PID_IN		EQU		009H
DEF_USB_PID_OUT		EQU		001H
DEF_USB_PID_ACK		EQU		002H
DEF_USB_PID_NAK		EQU		00AH
DEF_USB_PID_STALL	EQU		00EH
DEF_USB_PID_DATA0	EQU		003H
DEF_USB_PID_DATA1	EQU		00BH
DEF_USB_PID_PRE		EQU		00CH
;#endif

;/*USB请求类型, 外置固件模式可能用到 */
;#ifndef	DEF_USB_REQ_TYPE
DEF_USB_REQ_READ	EQU		080H			;/*控制读操作 */
DEF_USB_REQ_WRITE	EQU		000H			;/*控制写操作 */
DEF_USB_REQ_TYPE	EQU		060H			;/*控制请求类型 */
DEF_USB_REQ_STAND	EQU		000H			;/*标准请求 */
DEF_USB_REQ_CLASS	EQU		020H			;/*设备类请求 */
DEF_USB_REQ_VENDOR	EQU		040H			;/*厂商请求 */
DEF_USB_REQ_RESERVE	EQU		060H			;/*保留请求 */
;#endif

;/*USB标准设备请求, RequestType的位6位5=00(Standard), 外置固件模式可能用到 */
;#ifndef	DEF_USB_GET_DESCR
DEF_USB_CLR_FEATURE	EQU		001H
DEF_USB_SET_FEATURE	EQU		003H
DEF_USB_GET_STATUS	EQU		000H
DEF_USB_SET_ADDRESS	EQU		005H
DEF_USB_GET_DESCR	EQU		006H
DEF_USB_SET_DESCR	EQU		007H
DEF_USB_GET_CONFIG	EQU		008H
DEF_USB_SET_CONFIG	EQU		009H
DEF_USB_GET_INTERF	EQU		00AH
DEF_USB_SET_INTERF	EQU		00BH
DEF_USB_SYNC_FRAME	EQU		00CH
;#endif

;/********************************************************************************************************************** */
