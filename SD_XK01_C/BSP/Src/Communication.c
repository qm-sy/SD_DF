#include "communication.h"

RS485 rs485;

/**
 * @brief	uart1回调函数 用于接收数据
 * 
 * @param   huart：标识触发回调函数的UART外设                     
 * 
  @return  void
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if( huart->Instance == USART1)
    {
        if ( rs485.reflag == 1 )
        {
            return;
        }
		rs485.rcvbuf[rs485.recount++] = (uint8_t)(huart1.Instance->DR & 0x00FF);
		rs485.timout = 0;
		if( rs485.recount == 1 )
		{
			rs485.timrun = 1;
		}

		HAL_UART_Receive_IT(&huart1,&rs485.rcvbuf[rs485.recount],1);
    }
}

void RS485_Init( void )
{
    RX1_485;
	rs485.reflag = 0;
	rs485.recount = 0;
    modbus.modbus_04_scan_flag  = 0;
    modbus.modbus_04_scan_allow = 1;

    HAL_UART_Receive_IT(&huart1,&rs485.rcvbuf[rs485.recount],1);
}
