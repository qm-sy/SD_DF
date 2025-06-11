#include "modbus_rtu.h"

MODBUS_INFO modbus;

/**
 * @brief	modbus接收函数，接收并判断Function后转到相应Function函数进行处理
 * 
 * @param   buf：待发送数组首地址           
 * @param   len：数组长度           
 * 
  @return  void
 */
void Modbus_Event( void )
{
    uint16_t crc,rccrc;

    /*1.接收完毕                                           */
    if( rs485.reflag == 1 )
    { 
        rs485.reflag = 0;

        /*2.CRC校验                                         */
        crc = MODBUS_CRC16(rs485.rcvbuf, rs485.recount-2);
        rccrc = (rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1]);
        if ( crc == rccrc)
        {
            if( rs485.rcvbuf[0] == SLAVE_ADDR )
            {
                switch (rs485.rcvbuf[1])
                {         
                    case 0x03:		Modbus_Fun3();		break;
                    case 0x04:		Modbus_Fun4();      break;            

                    default:						    break;
                }
                    /* RX允许继续接收，开启超时接收计时               */
                    rs485.recount = 0;
            }
        }
    }
}

/**
 * @brief	modbus接收处理-03
 * 
 * @param  void                 
 * 
  @return  void
 */
void Modbus_Fun3( void )
{
   uint8_t start_addr_03 = 3;              //Slave reply  DATA1_H address

   for( uint16_t i = 0; i < 6; i++)
   {
       switch (i)
       {

        /*      40001  风扇档位      */
        case 0:
            gui_info.fan_level = rs485.rcvbuf[start_addr_03 + 1];   

            break;

        /*      40002  LED开关状态      */
        case 1:
            gui_info.led_switch = rs485.rcvbuf[start_addr_03 + 1];

            break;

        /*      40003  220V三路通道开关状态及功率大小      */
        case 2:
            gui_info.power_level = rs485.rcvbuf[start_addr_03];    
            gui_info.channel_num = rs485.rcvbuf[start_addr_03 + 1];
            
            break; 

        /*      40004  同步开关状态      */
        case 3:
            gui_info.sync_switch = rs485.rcvbuf[start_addr_03 + 1];

            break;

        /*      40005  模式信息      */
        case 4:
            gui_info.mode_num = rs485.rcvbuf[start_addr_03 + 1];

            break;   

        /*      40006  高温报警温度      */
        case 5:
            gui_info.temp_alarm_value = rs485.rcvbuf[start_addr_03 + 1];

            break;   

        default:
            break;
       }
       start_addr_03 += 2;
   }

   gui_info.connect_flag = true;         //如果03收到回复，说明与从机已建立通讯
}

/**
 * @brief	modbus接收处理-04
 * 
 * @param  void                 
 * 
  @return  void
 */
void Modbus_Fun4( void )
{
    uint8_t start_addr_04 = 3;              //Slave reply  DATA1_H address

    for( uint16_t i = 0; i < 5; i++)
    {
        switch (i)
        {
            /*      30001  实时温度      */
            case 0:
                gui_info.temp_value = rs485.rcvbuf[start_addr_04 + 1];

                break;

            /*      30002  dht11 环境温湿度      */
            case 1:
                gui_info.envir_humidity = rs485.rcvbuf[start_addr_04]; 
                gui_info.envir_temp     = rs485.rcvbuf[start_addr_04 + 1];     

                break;

            /*      30003  累计耗电时间:分钟部分      */
            case 2:
                gui_info.capacity_min =    rs485.rcvbuf[start_addr_04 + 1];      

                break;

            /*      30003  累计耗电时间:小时部分      */    
            case 3:
                gui_info.capacity_h = ((rs485.rcvbuf[start_addr_04] << 8) |rs485.rcvbuf[start_addr_04 + 1]);      

                break;

            default:
                break;
        }
        start_addr_04 += 2;
    }
}

/**
 * @brief	读输出寄存器-03
 *
 * @param   void
 *
 * @return  void
 */
void read_slave_03( void )
{
    uint8_t send_buf[8];
    uint16_t crc;

    TX1_485;                        //使能发送
    delay_ms(2);                    //等待电平稳定

    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_03;           //Fun

    /*   Value_H  && Value_L    */
    send_buf[2] = START_REG_H_03;
    send_buf[3] = START_REG_L_03;
    send_buf[4] = REG_NUM_H_03;
    send_buf[5] = REG_NUM_L_03;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,6);
    send_buf[6] = crc >> 8;
    send_buf[7] = crc;

    /*   发送，后使能接收    */
    memcpy(modbus.send_buf,send_buf,8);

    modbus_send_data(modbus.send_buf,8); 
    RX1_485;
}

/**
 * @brief	读输入寄存器-04
 *
 * @param   void
 *
 * @return  void
 */
void read_slave_04( void )
{
    uint8_t send_buf[8];
    uint16_t crc;

    TX1_485;                        //使能发送
    delay_ms(2);                    //等待电平稳定

    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_04;           //Fun

    /*   Value_H  && Value_L    */
    send_buf[2] = START_REG_H_04;
    send_buf[3] = START_REG_L_04;
    send_buf[4] = REG_NUM_H_04;
    send_buf[5] = REG_NUM_L_04;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,6);
    send_buf[6] = crc >> 8;
    send_buf[7] = crc;

    
    /*   发送，后使能接收    */
    memcpy(modbus.send_buf,send_buf,8);

    modbus_send_data(modbus.send_buf,8); 
    RX1_485;
}

/**
 * @brief	写单个输出寄存器-06
 *
 * @param   reg_addr：要写的寄存器地址
 *          reg_val： 要写的值
 *
 * @return  void
 */
void write_slave_06( uint16_t reg_addr, uint8_t reg_val_H, uint8_t reg_val_L)
{
    uint8_t send_buf[8];
    uint16_t crc;

    TX1_485;                        //使能发送
    delay_ms(2);                    //等待电平稳定

    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_06;           //Fun

    /*   Value_H  && Value_L    */
    send_buf[2] = reg_addr >> 8;
    send_buf[3] = reg_addr;
    send_buf[4] = reg_val_H ;
    send_buf[5] = reg_val_L;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,6);
    send_buf[6] = crc >> 8;
    send_buf[7] = crc;

    /*   发送，后使能接收    */
    memcpy(modbus.send_buf,send_buf,8);

    modbus_send_data(modbus.send_buf,8);
    RX1_485;        
}

/**
 * @brief	写多个输出寄存器-16
 *
 * @param   void
 *
 * @return  void
 */
void write_slave_16( void )
{
    uint8_t send_buf[21];
    uint16_t crc;

    TX1_485;
    delay_ms(2);

    send_buf[0] = SLAVE_ADDR;       //Addr
    send_buf[1] = FUN_16;           //Fun
    send_buf[2] = 0x00;             //Start reg H
    send_buf[3] = 0x00;             //Start reg L
    send_buf[4] = 0x00;             //Num H
    send_buf[5] = 0x06;             //Num L
    send_buf[6] = 0x0C;             //Byte count

    /*   Valuex_H  && Valuex_L    */
    send_buf[7] = 0x00;
    send_buf[8] = gui_info.fan_level;

    send_buf[9] = 0x00;
    send_buf[10] = gui_info.led_switch;

    send_buf[11] = gui_info.power_level;
    send_buf[12] = gui_info.channel_num;

    send_buf[13] = 0x00;
    send_buf[14] = gui_info.sync_switch;

    send_buf[15] = gui_info.mode_write_flag;
    send_buf[16] = gui_info.mode_num;

    send_buf[17] = 0x00;
    send_buf[18] = gui_info.temp_alarm_value;

    /*   crc    */
    crc = MODBUS_CRC16(send_buf,19);
    send_buf[19] = crc>>8;
    send_buf[20] = crc;

    /*   发送，后使能接收    */
    memcpy(modbus.send_buf,send_buf,21);

    modbus_send_data(modbus.send_buf,21);
    RX1_485;
}

/**
 * @brief	发送1帧数据
 * 
 * @param   buf：待发送数组首地址           
 * @param   len：数组长度           
 * 
  @return  void
 */
void modbus_send_data( uint8_t *buf , uint8_t len )
{
    HAL_UART_Transmit(&huart1,(uint8_t*)buf,len,1000);
    
    while (__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC) != SET);
}


/**
 * @brief	crc校验函数
 * 
 * @param   buf：  Address(1 byte) +Funtion(1 byte) ）+Data(n byte)   
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length)
{
	uint8_t	i;
	uint16_t	crc16;

    /* 1, 预置16位CRC寄存器为0xffff（即全为1）                          */
	crc16 = 0xffff;	

	do
	{
        /* 2, 把8位数据与16位CRC寄存器的低位相异或，把结果放于CRC寄存器     */        
		crc16 ^= (uint16_t)*buf;		//
		for(i=0; i<8; i++)		
		{
            /* 3, 如果最低位为1，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位 再异或0xA001    */
			if(crc16 & 1)
            {
                crc16 = (crc16 >> 1) ^ 0xA001;
            }
            /* 4, 如果最低位为0，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位                */
            else
            {
                crc16 >>= 1;
            }		
		}
		buf++;
	}while(--length != 0);

	return	(crc16);
}
