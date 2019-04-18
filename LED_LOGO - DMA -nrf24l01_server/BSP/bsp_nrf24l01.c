#include    "bsp_nrf24l01.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/                              
/* 私有变量 ------------------------------------------------------------------*/
SPI_HandleTypeDef hspi_NRF24L01;
extern SPI_HandleTypeDef hspi1;
const uint8_t TX_ADDRESS[TX_ADR_WIDTH]= {0xb0,0x43,0x10,0x10,0x01}; //发送地址
const uint8_t RX_ADDRESS0[RX_ADR_WIDTH]={0xb0,0x43,0x10,0x10,0x01};	//接收地址

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/


/* SPI1 init function */
 void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}




/**
  * 函数功能: 往串行Flash读取写入一个字节数据并接收一个字节数据
  * 输入参数: byte：待发送数据
  * 返 回 值: uint8_t：接收到的数据
  * 说    明：无
  */
uint8_t SPIx_ReadWriteByte(SPI_HandleTypeDef* hspi,uint8_t byte)
{
  uint8_t d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(hspi,&d_send,&d_read,1,0xFF)!=HAL_OK)
  {
    d_read=0xFF;
  }
  return d_read; 
}

/**
  * 函数功能: 检测24L01是否存在
  * 输入参数: 无
  * 返 回 值: 0，成功;1，失败
  * 说    明：无          
  */ 
uint8_t NRF24L01_Check(SPI_HandleTypeDef* hspi)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	NRF24L01_Write_Buf(hspi,NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(hspi,TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}	 	 

/**
  * 函数功能: SPI写寄存器
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：reg:指定寄存器地址
  *           
  */ 
uint8_t NRF24L01_Write_Reg(SPI_HandleTypeDef* hspi,uint8_t reg,uint8_t value)
{
	uint8_t status;	
  NRF24L01_SPI_CS_ENABLE();                 //使能SPI传输
  status =SPIx_ReadWriteByte(hspi,reg);//发送寄存器号 
  SPIx_ReadWriteByte(hspi,value);      //写入寄存器的值
  NRF24L01_SPI_CS_DISABLE();                 //禁止SPI传输	   
  return(status);       			//返回状态值
}

/**
  * 函数功能: 读取SPI寄存器值
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：reg:要读的寄存器
  *           
  */ 
uint8_t NRF24L01_Read_Reg(SPI_HandleTypeDef* hspi,uint8_t reg)
{
	uint8_t reg_val;	    
 	NRF24L01_SPI_CS_ENABLE();          //使能SPI传输		
  SPIx_ReadWriteByte(hspi,reg);   //发送寄存器号
  reg_val=SPIx_ReadWriteByte(hspi,0XFF);//读取寄存器内容
  NRF24L01_SPI_CS_DISABLE();          //禁止SPI传输		    
  return(reg_val);           //返回状态值
}	

/**
  * 函数功能: 在指定位置读出指定长度的数据
  * 输入参数: 无
  * 返 回 值: 此次读到的状态寄存器值 
  * 说    明：无
  *           
  */ 
uint8_t NRF24L01_Read_Buf(SPI_HandleTypeDef* hspi,uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,uint8_t_ctr;	   
  
  NRF24L01_SPI_CS_ENABLE();           //使能SPI传输
  status=SPIx_ReadWriteByte(hspi,reg);//发送寄存器值(位置),并读取状态值   	   
 	for(uint8_t_ctr=0;uint8_t_ctr<len;uint8_t_ctr++)
  {
    pBuf[uint8_t_ctr]=SPIx_ReadWriteByte(hspi,0XFF);//读出数据
  }
  NRF24L01_SPI_CS_DISABLE();       //关闭SPI传输
  return status;        //返回读到的状态值
}

/**
  * 函数功能: 在指定位置写指定长度的数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：reg:寄存器(位置)  *pBuf:数据指针  len:数据长度
  *           
  */ 
uint8_t NRF24L01_Write_Buf(SPI_HandleTypeDef* hspi,uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,uint8_t_ctr;	    
 	NRF24L01_SPI_CS_ENABLE();          //使能SPI传输
  status = SPIx_ReadWriteByte(hspi,reg);//发送寄存器值(位置),并读取状态值
  for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
  {
    SPIx_ReadWriteByte(hspi,*pBuf++); //写入数据	 
  }
  NRF24L01_SPI_CS_DISABLE();       //关闭SPI传输
  return status;          //返回读到的状态值
}				   

/**
  * 函数功能: 启动NRF24L01发送一次数据
  * 输入参数: 无
  * 返 回 值: 发送完成状况
  * 说    明：txbuf:待发送数据首地址
  *           
  */ 
uint8_t NRF24L01_TxPacket(SPI_HandleTypeDef* hspi,uint8_t *txbuf)
{
	uint8_t sta;
	NRF24L01_CE_LOW();
  NRF24L01_Write_Buf(hspi, WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	NRF24L01_CE_HIGH();//启动发送	 
  
	while(NRF24L01_IRQ_PIN_READ()!=0);//等待发送完成
  
	sta=NRF24L01_Read_Reg(hspi,STATUS);  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(hspi,FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}

/**
  * 函数功能:启动NRF24L01接收一次数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  *           
  */ 
uint8_t NRF24L01_RxPacket(SPI_HandleTypeDef* hspi,uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	sta=NRF24L01_Read_Reg(hspi,STATUS);  //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(hspi,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(hspi,FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}					    

/**
  * 函数功能: 该函数初始化NRF24L01到RX模式
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  *           
  */ 
void NRF24L01_RX_Mode(SPI_HandleTypeDef* hspi)
{
	NRF24L01_CE_LOW();	  
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+CONFIG, 0x0F);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC 
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  	 
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+RF_CH,40);	     //设置RF通信频率		  
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
     
  NRF24L01_Write_Buf(hspi,NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS0,RX_ADR_WIDTH);//写RX节点地址
	
  NRF24L01_CE_HIGH(); //CE为高,进入接收模式 
  HAL_Delay(1);
}						 

/**
  * 函数功能: 该函数初始化NRF24L01到TX模式
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  *           
  */ 
void NRF24L01_TX_Mode(SPI_HandleTypeDef* hspi)
{														 
	NRF24L01_CE_LOW();	    
  NRF24L01_Write_Buf(hspi,NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  NRF24L01_Write_Buf(hspi,NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS0,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+SETUP_RETR,0xff);//设置自动重发间隔时间:4000us + 86us;最大自动重发次数:15次
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  NRF24L01_Write_Reg(hspi,NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF24L01_CE_HIGH();//CE为高,10us后启动发送
  HAL_Delay(1);
}
