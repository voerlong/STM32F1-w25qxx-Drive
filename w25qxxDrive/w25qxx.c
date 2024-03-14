/************************************************
	��2023.11.22����
	������SPI����W25q32����
	���ߣ�-Сè��-
************************************************/

/************************************************
	ͷ�ļ�
************************************************/
#include "w25qxx.h"



/************************************************
	������
************************************************/
#if SPI_SORF_OR_HARD

#else
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
#endif
/*******************************************************************************
 ����: SPI_UserInit
 ����: SPI��ʼ������
 ����: ��
 ����: ��
*******************************************************************************/
void SPI_UserInit(void)
{
	#if	SPI_SORF_OR_HARD
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(SPI_RCC_GPIO, ENABLE);
	if(SPI_RCC_SPIx == RCC_APB2Periph_SPI1){
		RCC_APB2PeriphClockCmd(SPI_RCC_SPIx, ENABLE);
	}
	else{
		RCC_APB1PeriphClockCmd(SPI_RCC_SPIx ,ENABLE);
	}
	
	GPIO_InitStructure.GPIO_Pin = SPI_PIN;  //���ݺ�ʱ�ӹܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;	//CS�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_CS_GPIO,&GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					//SPI����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;			//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;					//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����		
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//fpclk 2��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		//��λ����
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPIx ,&SPI_InitStructure);
	SPI_Cmd(SPIx, ENABLE);//ʹ��SPI
	
	SPI_FLASH_CS(Low);
	SPI_FLASH_CS(High);
	#else
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);	 //ʹ�ܶ˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = SPI_GPIO_CSN|SPI_GPIO_SCK|SPI_GPIO_MOSI;				 //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(SPI_GPIOx, &GPIO_InitStructure);					 //�����趨������ʼ��
	
	SPI_FLASH_CS(High);      //Ƭѡ��ѡ��
    
	GPIO_InitStructure.GPIO_Pin = SPI_GPIO_MOSO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIOx, &GPIO_InitStructure);
	#endif
}


///*******************************************************************************
// ����: w25q32_Init
// ����: ��ʼ��w25q32
// ����: ��
// ����: ��
//*******************************************************************************/
//void w25q32_Init(void)
//{
//	SPI_FLASH_SectorErase(Block0 & Sector0);
//	SPI_FLASH_SectorErase(Block0 & Sector1);
//	SPI_FLASH_SectorErase(Block0 & Sector2);
//	SPI_FLASH_SectorErase(Block0 & Sector3);
//	SPI_FLASH_SectorErase(Block0 & Sector4);
//}


/*******************************************************************************
 ����: SPI_FlashSendByte
 ����: ͨ��SPI���߷���һ���ֽڵ�����
 ����: 
		byt����Ҫ���͵ĵ����ֽ�
 ����: ����������
*******************************************************************************/
uint8_t SPI_FlashSendByte(uint8_t byte)
{
	#if	SPI_SORF_OR_HARD
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPIx, byte);
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_I2S_ReceiveData(SPIx);
	#else
	for(uint8_t bit_ctr=0;bit_ctr<8;bit_ctr++) 
	{
		if(byte & 0x80) MOSI_H;
		else MOSI_L;
		byte = (byte << 1);
		SCK_H; 
		Delay(0xff);
		if(READ_MISO)   byte |= 0x01; 
		SCK_L; 
		Delay(0xff);
	}
	return(byte); 
	#endif
}




/*******************************************************************************
 ����: SPI_FlashPowerdown
 ����: SPI_FLASH�������ģʽ
 ����: ��
 ����: ��
*******************************************************************************/
void SPI_FlashPowerdown(void)   
{ 
  SPI_FLASH_CS(Low);
  SPI_FlashSendByte(W25X_PowerDown);//���͵�������
  SPI_FLASH_CS(High);
}   



/*******************************************************************************
 ����: SPI_FlashWakeup
 ����: SPI_FLASH�ӵ���ģʽ����
 ����: ��
 ����: ��
*******************************************************************************/
void SPI_FlashWakeup(void)   
{
  SPI_FLASH_CS(Low);
  SPI_FlashSendByte(W25X_ReleasePowerDown);//���ͻ�������
  SPI_FLASH_CS(High);      
}


/*******************************************************************************
 ����: SPI_FlashReadDeviceid
 ����: ��ȡFLASH����ID
 ����: ��
 ����: ��
*******************************************************************************/
uint32_t SPI_FlashReadDeviceid(void)
{
  uint32_t Temp = 0;

  SPI_FLASH_CS(Low);
  SPI_FlashSendByte(W25X_DeviceID);
  SPI_FlashSendByte(Dummy_Byte);
  SPI_FlashSendByte(Dummy_Byte);
  SPI_FlashSendByte(Dummy_Byte);
  
  Temp = SPI_FlashSendByte(Dummy_Byte);

  SPI_FLASH_CS(High);
  return Temp;
}



/*******************************************************************************
 ����: SPI_FlashReadJedecid
 ����: ��ȡjedecid
 ����: ��
 ����: ��
*******************************************************************************/
uint32_t SPI_FlashReadJedecid(void)
{
  uint32_t temp = 0, temp0 = 0, temp1 = 0, temp2 = 0;

  SPI_FLASH_CS(Low);
  SPI_FlashSendByte(W25X_JedecDeviceID);
  temp0 = SPI_FlashSendByte(Dummy_Byte);
  temp1 = SPI_FlashSendByte(Dummy_Byte);
  temp2 = SPI_FlashSendByte(Dummy_Byte);
  SPI_FLASH_CS(High);

  temp = (temp0 << 16) | (temp1 << 8) | temp2;

  return temp;
}



/*******************************************************************************
 ����: SPI_FlashWriteEnable
 ����: дʹ�ܺ���
 ����: ��
 ����: ��
*******************************************************************************/
void SPI_FlashWriteEnable(void)
{
  SPI_FLASH_CS(Low);
  SPI_FlashSendByte(W25X_WriteEnable);//дʹ������
  SPI_FLASH_CS(High);
}




/*******************************************************************************
 ����: SPI_FlashWaitForWriteEnd
 ����: flash��æ����(æ�ȴ���ʽ)
 ����: ��
 ����: ��
*******************************************************************************/
void SPI_FlashWaitForWriteEnd(void)
{
  uint8_t FLASH_Status=0;
  SPI_FLASH_CS(Low);
  SPI_FlashSendByte(W25X_ReadStatusReg);
  do
  {
    FLASH_Status=SPI_FlashSendByte(Dummy_Byte);
  }
  while((FLASH_Status & WIP_Flag) == SET);
  SPI_FLASH_CS(High);
}


/*******************************************************************************
 ����: SPI_FlashSendAddr
����: ��Flash������Ҫ�����ĵ�ַ
 ����: 
		Addr����ַ
 ����: ��
*******************************************************************************/
void	SPI_FlashSendAddr(uint32_t	Addr)
{
	#ifdef	ADDR3
  SPI_FlashSendByte((Addr & 0xFF0000) >> 16);
  SPI_FlashSendByte((Addr& 0xFF00) >> 8);
  SPI_FlashSendByte(Addr & 0xFF);
	#else
	SPI_FlashSendByte((Addr & 0xFF000000) >> 24);
	SPI_FlashSendByte((Addr & 0xFF0000) >> 16);
  SPI_FlashSendByte((Addr& 0xFF00) >> 8);
  SPI_FlashSendByte(Addr & 0xFF);
	#endif
}


/*******************************************************************************
 ����: SPI_FlashBufferRead
 ����: flash��ȡ
 ����: 
		ReadAddr����ȡ��flash��ַ
		NumByteToRead����ȡ���ٸ��ֽ�
 ����: 
		pBuffer����ȡ�����ݻ�����
*******************************************************************************/
void SPI_FlashBufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  SPI_FLASH_CS(Low);
  SPI_FlashSendByte(W25X_ReadData);
	SPI_FlashSendAddr(ReadAddr);
  while (NumByteToRead--) 
  {
    *pBuffer = SPI_FlashSendByte(Dummy_Byte);//����
    pBuffer++;
  }
  SPI_FLASH_CS(High);
}



/*******************************************************************************
 ����: SPI_FLASH_SectorErase
 ����: ��������
 ����: 
		SectorAddr����Ҫ������������ʼ��ַ
 ����: ��
*******************************************************************************/
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	SPI_FlashWriteEnable();
	SPI_FlashWaitForWriteEnd();
	SPI_FLASH_CS(Low);
	//��������,���е�һ���ֽ�Ϊ��������������루20h������������ΪҪ���в�����24λ��ʼ��ַ(������ʼ��ַ)��
	SPI_FlashSendByte(W25X_SectorErase);
	SPI_FlashSendAddr(SectorAddr);
	SPI_FLASH_CS(High);
	SPI_FlashWaitForWriteEnd();
}



/*******************************************************************************
 ����: SPI_FLASH_BlockErase
 ����: ��������
 ����: 
		SectorAddr����Ҫ�����Ŀ�����ʼ��ַ
 ����: ��
*******************************************************************************/
void SPI_FLASH_BlockErase(uint32_t BlockAddr)
{
	SPI_FlashWriteEnable();
	SPI_FlashWaitForWriteEnd();
	SPI_FLASH_CS(Low);
	//��������,���е�һ���ֽ�Ϊ��������������루D8h������������ΪҪ���в�����24λ��ʼ��ַ(������ʼ��ַ)��
	SPI_FlashSendByte(W25X_BlockErase);
	SPI_FlashSendAddr(BlockAddr);
	SPI_FLASH_CS(High);
	SPI_FlashWaitForWriteEnd();
}


/*******************************************************************************
 ����: SPI_FlashPageWrite
 ����: ҳ��д����
 ����: 
		pBuffer����Ҫд����ֽڻ������飬һ��д�벻�ܳ���һҳ������Ǵ�ͷ��ʼд�����ܳ���256��
		WriteAddr��д�����ʼ��ַ
		NumByteToWrite��д����ٸ��ֽ�
 ����: ��
*******************************************************************************/
void SPI_FlashPageWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	SPI_FlashWriteEnable();
	SPI_FlashWaitForWriteEnd();
	SPI_FLASH_CS(Low);
	SPI_FlashSendByte(W25X_PageProgram);	//ҳ���ָ��
	SPI_FlashSendAddr(WriteAddr);
	if(NumByteToWrite>SPI_FLASH_PerWritePageSize)
		NumByteToWrite=SPI_FLASH_PerWritePageSize;
	while(NumByteToWrite--)
	{
		SPI_FlashSendByte(*pBuffer);
		pBuffer++;
	}
	SPI_FLASH_CS(High);
	SPI_FlashWaitForWriteEnd();
}


/*******************************************************************************
 ����: SPI_FlashWriteNoCheck
 ����: flashд�뺯��(Ҫ��֤flash�Ǳ���������)(�Զ���ҳ)
 ����: 
		pBuffer����Ҫд����ֽڻ�������
		WriteAddr��д�����ʼ��ַ
		NumByteToWrite��д����ٸ��ֽ�
 ����: ��
*******************************************************************************/
void SPI_FlashWriteNoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain = 0;	  
	pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite <= pageremain)	pageremain = NumByteToWrite;//д����ֽ���С��һҳ  pageremain�͵���Ҫд����ֽ���
	while(1)
	{	   
		SPI_FlashPageWrite(pBuffer,WriteAddr,pageremain);//д�뵥ҳʣ����ֽ���
		if(NumByteToWrite==pageremain)	break;//д�������
	 	else //��Ҫд����ֽڴ��ڵ�ҳʣ����
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;
			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)	pageremain=256; //һ�ο���д��256���ֽ�
			else	pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	}	    
} 
