#include "modbus_rtu.h"

MODBIS_INFO modbus;

/**
 * @brief	modbus_rtu  无奇偶校验
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Event( void )
{
    uint16_t crc,rccrc;
    
    /*1.接收完毕                                           */
    if( rs485.RX4_rev_end_Flag == 1 )
    {
        /*2.清空接收完毕标志位                              */    
        rs485.RX4_rev_end_Flag = 0;

        /*3.CRC校验                                         */
        crc = MODBUS_CRC16(rs485.RX4_buf, rs485.RX4_rev_cnt-2);
        rccrc = (rs485.RX4_buf[rs485.RX4_rev_cnt-1]) | (rs485.RX4_buf[rs485.RX4_rev_cnt-2]<<8);

        /*4.清空接收计数                                    */
        rs485.RX4_rev_cnt = 0; 

        /*5.CRC校验通过，进行地址域校验                      */
        if( crc == rccrc )
        {
            /*6.地址域校验通过，进入相应功能函数进行处理      */
            if( rs485.RX4_buf[0] == MY_ADDR )
            {
                switch ( rs485.RX4_buf[1] )
                {
                    case 0x03:
                        Modbus_Fun3();
                        break;

                    case 0x04:
                        Modbus_Fun4();
                        break;

                    case 0x06:
                        Modbus_Fun6();
                        break;  

                    case 0x10:  
                        Modbus_Fun16();

                    default:
                        break;
                }
            }
        }
    }
}


/**
 * @brief	读输入寄存器  03
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun3( void )
{
    uint16_t i;

    modbus.send_value_addr  = 3;                //DATA1 H 位置
    modbus.byte_cnt   = (rs485.RX4_buf[4]<<8 | rs485.RX4_buf[5]) *2;
    modbus.start_addr = rs485.RX4_buf[2]<<8 | rs485.RX4_buf[3];

    rs485.TX4_buf[0]  = MY_ADDR;                //Addr
    rs485.TX4_buf[1]  = 0x03;                   //Fun
    rs485.TX4_buf[2]  = modbus.byte_cnt;        //Byte Count

    for( i = modbus.start_addr; i < modbus.start_addr + modbus.byte_cnt/2; i++ )
    {
        /*    每次循环前初始化byte_info                       */
        modbus.byte_info_H = modbus.byte_info_L = 0X00;
        switch (i)
        {   
            /*  40001 风速1查询                     */
            case 0x00:
                modbus.byte_info_H = 0x00;
                modbus.byte_info_L = slave_06.fan1_level;
                
                break;

            /*  40002 风速2查询                     */    
            case 0x01:
                modbus.byte_info_H = 0X00;
                modbus.byte_info_L = slave_06.fan2_level;

                break;

            /*  40003 3路220V开关使能查询                         */
            case 0x02:  
                modbus.byte_info_H = slave_06.power_level;  
                modbus.byte_info_L = slave_06.channel_num; 
                
                break;

            /*  40004 同步状态查询              */
            case 0x03:    
                modbus.byte_info_H = 0X00;  
                modbus.byte_info_L = slave_06.sync_switch;
                
                break;

            /*  40005 工作模式查询                     */
            case 0x04:   
                modbus.byte_info_H = 0X00; 
                modbus.byte_info_L = slave_06.mode_num;

                break;

            /*  40006 报警温度查询                     */
            case 5:   
                modbus.byte_info_H = 0X00;           
                modbus.byte_info_L = slave_06.temp_alarm_value;
         
                break;
            default:
                break;
        }
        rs485.TX4_buf[modbus.send_value_addr++] = modbus.byte_info_H;
        rs485.TX4_buf[modbus.send_value_addr++] = modbus.byte_info_L;
    }
    slave_to_master(0x03,3 + modbus.byte_cnt);
}

/**
 * @brief	读输出寄存器  04
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun4( void )
{
    uint16_t i;

    modbus.send_value_addr  = 3;                 //DATA1 H 位置
    modbus.byte_cnt   = (rs485.RX4_buf[4]<<8 | rs485.RX4_buf[5]) *2;
    modbus.start_addr = rs485.RX4_buf[2]<<8 | rs485.RX4_buf[3];

    rs485.TX4_buf[0]  = MY_ADDR;                //Addr
    rs485.TX4_buf[1]  = 0x04;                   //Fun
    rs485.TX4_buf[2]  = modbus.byte_cnt;        //Byte Count

    for( i = modbus.start_addr; i < modbus.start_addr + modbus.byte_cnt/2; i++ )
    {
        /*    每次循环前初始化byte_info                       */
        modbus.byte_info_H = modbus.byte_info_L = 0X00;
        switch (i)
        {
            /*  30001  NTC1、NTC2温度查询                           */
            case 0x00:
                modbus.byte_info_H = 0x00;   
                modbus.byte_info_L = temp.NTC1_value;

                break;

            /*  30002  环境温湿度查询                */
            case 0x01:
                modbus.byte_info_H = temp.dht11_humidity;           
                modbus.byte_info_L = temp.dht11_temp;          

                break;

            /*  30003 运行时间（min）                   */
            case 0x02:
                modbus.byte_info_H = 0x00;           
                modbus.byte_info_L = capacity.capacity_min;          

                break;

            /*  30004 运行时间（h）                   */
            case 0x03:
                modbus.byte_info_H = capacity.capacity_h >> 8;           
                modbus.byte_info_L = capacity.capacity_h;          

                break;

            default:
                break;
        }
        rs485.TX4_buf[modbus.send_value_addr++] = modbus.byte_info_H;
        rs485.TX4_buf[modbus.send_value_addr++] = modbus.byte_info_L;
    }
    slave_to_master(0x04,3 + modbus.byte_cnt);
}

/**
 * @brief	写单个输出寄存器  06
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun6( void )
{
    switch (rs485.RX4_buf[3])
    {
        /*  40001  风速1设置                 */
        case 0x00:    
            slave_06.fan1_level = rs485.RX4_buf[5];     

            fan1_ctrl(slave_06.fan1_level);

            break;

        /*  40002   风速2设置                          */
        case 0x01:                                         
            slave_06.fan2_level = rs485.RX4_buf[5];      

            fan2_ctrl(slave_06.fan2_level);

            break;

        /*  40003 三路220V输出使能设置                          */
        case 0x02:                                         
            slave_06.power_level = rs485.RX4_buf[4];
            slave_06.channel_num = rs485.RX4_buf[5];

            AC_channel_ctrl(slave_06.channel_num);
            AC_level_ctrl(rs485.RX4_buf[4]);

            break;  
            
        /*  40004  同步状态设置                   */
        case 0x03:                                         
            slave_06.sync_switch = rs485.RX4_buf[5];

            sync_ctrl();

            break;

        /*  40005  工作模式设置                   */
        case 0x04:                                         
            slave_06.mode_num = rs485.RX4_buf[5];

            mode_ctrl(slave_06.mode_num);

            break;

        /*  40006  报警温度设置                   */
        case 0x05:                                         
            slave_06.temp_alarm_value = rs485.RX4_buf[5];

            break;

        /*  40007  总开关设置                   */
        case 0x06:    
            slave_06.power_switch = rs485.RX4_buf[5];

            power_switch_ctrl(slave_06.power_switch);

        default:
            break;   
    }

    slave_to_master(0x06,8);

    eeprom_data_record();
}

/**
 * @brief	写多个输出寄存器  16
 *
 * @param   void
 *
 * @return  void 
**/
void Modbus_Fun16( void )
{
    uint16_t i;

    modbus.rcv_value_addr = 7;                  //DATA1 H位置
    modbus.byte_cnt   = rs485.RX4_buf[6];
    modbus.start_addr = rs485.RX4_buf[2]<<8 | rs485.RX4_buf[3];

    
    for( i = modbus.start_addr; i < modbus.start_addr + modbus.byte_cnt/2; i++)
    {
        modbus.byte_info_H = rs485.RX4_buf[modbus.rcv_value_addr];
        modbus.byte_info_L = rs485.RX4_buf[modbus.rcv_value_addr + 1];
        switch (i)
        {
            /*  40001  风速1设置                 */
            case 0x00:
                slave_06.fan1_level = modbus.byte_info_L;

                fan1_ctrl(modbus.byte_info_L);

                break;
            
            /*  40002  风速2设置                          */
            case 0x01:
                slave_06.fan2_level = modbus.byte_info_L;

                fan2_ctrl(slave_06.fan2_level);

                break;

            /*  40003 三路220V输出使能设置                          */
            case 0x02:
                slave_06.power_level = modbus.byte_info_H;
                slave_06.channel_num = modbus.byte_info_L;

                AC_channel_ctrl(slave_06.channel_num);
                AC_level_ctrl(slave_06.power_level);

                break;

            /*  40004  同步状态设置                   */
            case 0x03:
                slave_06.sync_switch = modbus.byte_info_L;

                sync_ctrl();

                break;

            /*  40005  工作模式设置                   */
            case 0x04:                                         
                slave_06.mode_num = modbus.byte_info_L;

                mode_ctrl(slave_06.mode_num);

                break;

            /*  40006  报警温度设置                   */
            case 0x05:                                         
                slave_06.temp_alarm_value = modbus.byte_info_L;
                
                break;
                
            default:
                break;
        }
        modbus.rcv_value_addr += 2;         //从Value1_H →→ 从Value2_H
    }

    slave_to_master(0x10,8);

    eeprom_data_record();                      //记录更改后的值
}


/**
 * @brief	从机回复主机
 *  
 * @param   code_num:功能码       
 * @param   length:数据长度        
 * 
  @return  crc16:crc校验的值 2byte
 */
void slave_to_master(uint8_t code_num,uint8_t length)
{
    uint16_t crc;

    switch (code_num)
    {
        case 0x03:
            crc = MODBUS_CRC16(rs485.TX4_buf,length);

            rs485.TX4_buf[length+1] = crc;             //CRC H
            rs485.TX4_buf[length] = crc>>8;            //CRC L

            rs485.TX4_send_bytelength = length + 2;
            
            break;
        case 0x04:
            crc = MODBUS_CRC16(rs485.TX4_buf,length);

            rs485.TX4_buf[length+1] = crc;              //CRC H
            rs485.TX4_buf[length] = crc>>8;             //CRC L

            rs485.TX4_send_bytelength = length + 2;
            
            break;    

        case 0x06:
            memcpy(rs485.TX4_buf,rs485.RX4_buf,8);

            rs485.TX4_send_bytelength = length;
            
            break;   

        case 0x10:
            memcpy(rs485.TX4_buf,rs485.RX4_buf,6);
        
            crc = MODBUS_CRC16(rs485.TX4_buf,6);

            rs485.TX4_buf[7] = crc;                 //CRC H
            rs485.TX4_buf[6] = crc>>8;              //CRC L
        
            rs485.TX4_send_bytelength = length;
            
            break;         

        default:
            break;
    }

    DR_485 = 1;                                 //485可以发送
    delay_ms(2);
    S4CON |= S4TI;                              //开始发送
    delay_ms(1);
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
