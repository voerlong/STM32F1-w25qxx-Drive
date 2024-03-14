/************************************************
	描述：w25qxx.h的配置文件
	作者：-小猫巫-
************************************************/
#ifndef W25QXXCONFIG_H
#define W25QXXCONFIG_H

/************************************************
	宏定义
************************************************/
//软件还是硬件控制
#define	SPI_SOFTWARE	0
#define	SPI_HARDWARE	1

//芯片类型
#define w25q80	0
#define	w25q16	1
#define w25q32	2
#define w25q64	3
#define	w25q128	4
#define	w25q256	5
#define	w25q512	6

#define	w25qxx	w25q32	//选择芯片

#define	SPI_SORF_OR_HARD	SPI_HARDWARE	//选择软件控制还是硬件控制SPI

#if	SPI_SORF_OR_HARD
#define	SPI_PIN	GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7//SPI对应的PIN脚
#define	SPI_GPIO	GPIOA	//SPI对应的GPIO口
#define	SPI_CS_PIN	GPIO_Pin_4	//SPICS对应的PIN脚
#define	SPI_CS_GPIO	GPIOA	//SPICS对应的GPIO口
#define	SPIx	SPI1	//使用的SPI几
#define	SPI_RCC_GPIO	RCC_APB2Periph_GPIOA	//使用的GPIO口对应的时钟
#define	SPI_RCC_SPIx	RCC_APB2Periph_SPI1		//使用的SPIx对应的时钟

#else
#define         SPI_GPIOx    GPIOA	//使用的GPIO口
#define         RCC_APB2Periph_GPIOx   RCC_APB2Periph_GPIOA//使用的GPIO口的时钟
#define         SPI_GPIO_CSN     GPIO_Pin_4      //片选
#define         SPI_GPIO_SCK     GPIO_Pin_5      //时钟
#define         SPI_GPIO_MOSO    GPIO_Pin_6      //输入
#define         SPI_GPIO_MOSI    GPIO_Pin_7      //输出
#endif

#define SPI_FLASH_PerWritePageSize 256	//一页的长度

#endif
