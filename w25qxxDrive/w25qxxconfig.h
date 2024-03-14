/************************************************
	������w25qxx.h�������ļ�
	���ߣ�-Сè��-
************************************************/
#ifndef W25QXXCONFIG_H
#define W25QXXCONFIG_H

/************************************************
	�궨��
************************************************/
//�������Ӳ������
#define	SPI_SOFTWARE	0
#define	SPI_HARDWARE	1

//оƬ����
#define w25q80	0
#define	w25q16	1
#define w25q32	2
#define w25q64	3
#define	w25q128	4
#define	w25q256	5
#define	w25q512	6

#define	w25qxx	w25q32	//ѡ��оƬ

#define	SPI_SORF_OR_HARD	SPI_HARDWARE	//ѡ��������ƻ���Ӳ������SPI

#if	SPI_SORF_OR_HARD
#define	SPI_PIN	GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7//SPI��Ӧ��PIN��
#define	SPI_GPIO	GPIOA	//SPI��Ӧ��GPIO��
#define	SPI_CS_PIN	GPIO_Pin_4	//SPICS��Ӧ��PIN��
#define	SPI_CS_GPIO	GPIOA	//SPICS��Ӧ��GPIO��
#define	SPIx	SPI1	//ʹ�õ�SPI��
#define	SPI_RCC_GPIO	RCC_APB2Periph_GPIOA	//ʹ�õ�GPIO�ڶ�Ӧ��ʱ��
#define	SPI_RCC_SPIx	RCC_APB2Periph_SPI1		//ʹ�õ�SPIx��Ӧ��ʱ��

#else
#define         SPI_GPIOx    GPIOA	//ʹ�õ�GPIO��
#define         RCC_APB2Periph_GPIOx   RCC_APB2Periph_GPIOA//ʹ�õ�GPIO�ڵ�ʱ��
#define         SPI_GPIO_CSN     GPIO_Pin_4      //Ƭѡ
#define         SPI_GPIO_SCK     GPIO_Pin_5      //ʱ��
#define         SPI_GPIO_MOSO    GPIO_Pin_6      //����
#define         SPI_GPIO_MOSI    GPIO_Pin_7      //���
#endif

#define SPI_FLASH_PerWritePageSize 256	//һҳ�ĳ���

#endif
