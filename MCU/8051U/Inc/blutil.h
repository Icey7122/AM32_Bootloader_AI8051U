/*
  MCU specific utility functions for the bootloader
 */
/*
  based on https://github.com/AlkaMotors/AM32_Bootloader_F051/blob/main/Core/
 */
#pragma once

/*
  2k ram + 32k xram
 */
#define RAM_BASE 0x0000
#define RAM_SIZE 2
#define XRAM_SIZE 32


/*
  use 64k of flash
 */
#define BOARD_FLASH_SIZE 64

#define GPIO_PIN(n) (1U<<(n))

#define GPIO_PULL_NONE 				LL_GPIO_PULL_NO
#define GPIO_PULL_UP   				LL_GPIO_PULL_UP
#define GPIO_PULL_DOWN 				LL_GPIO_PULL_DOWN

#define GPIO_OUTPUT_PUSH_PULL 		LL_GPIO_OUTPUT_PUSHPULL



#define gpio_set(input_pin)  	input_pin = 1

#define gpio_clear(input_pin)  	input_pin = 0

#define gpio_read(input_pin)  	input_pin


#define PWMB_Timer_Init()\
{\
	PWMB_ENO = 0x00;	\	
	PWMB_IOAUX = 0x00;	\		
	PWMB_ARRH = 0xFF;	\
	PWMB_ARRL = 0xFF;	\	
	PWMB_CNTRH = 0x00;	\
	PWMB_CNTRL = 0x00;	\		
	PWMB_PSCRH = 0x00;	\		
	PWMB_PSCRL = 47;	\	
	PWMB_IER = 0x00;	\		
	PWMB_CR1 = 0x01;	\		
}

// static void PWMB_Timer_Init(void)
// {
// //由于未开启高速PWM所以PWMB的时钟源为48Mhz

// 	PWMB_ENO = 0x00;		//禁止PWMB的PWM输出
// 	PWMB_IOAUX = 0x00;		//禁止PWMB

// 	PWMB_ARRH = 0xFF;
// 	PWMB_ARRL = 0xFF;		//设置PWMB周期为65535
// 	PWMB_CNTRH = 0x00;
// 	PWMB_CNTRL = 0x00;		//清零计数器
// 	PWMB_PSCRH = 0x00;		
// 	PWMB_PSCRL = 47;		//PWMB时钟源分频到1Mhz
// 	PWMB_IER = 0x00;		//禁止PWMB中断
// 	PWMB_CR1 = 0x01;		//使能计数器
// }

/*
  disable timer ready for app start
 */
#define bl_timer_disable()	PWMB_CR1 &= 0xFE 

#define bl_timer_us()		((uint16_t)PWMB_CNTRH << 8 | PWMB_CNTRL)

#define bl_timer_reset() 	PWMB_CNTRH = 0; PWMB_CNTRL = 0


#define SystemClock_Config()\
{\
	EA = 0;						\
	CKCON = 0x00;				\			      
	WTST = 1;    				\           	
	P_SW2 = 0x80;				\			    
	CLKDIV = 0x04;				\		
	IRTRIM = CHIPID12;    		\ 		
	HIRCSEL1 = 1;				\
	HIRCSEL0 = 0;				\
	HIRCCR = 0x80;				\
	while (!(HIRCCR & 0x01));	\
	CLKSEL = 0x40; 				\
	USBCLK &= 0x0F;				\
	USBCLK |= 0xA0;				\
	NOP(5);						\
	CLKDIV = 0X01;				\
	CLKSEL |= 0x08;				\
	HSCLKDIV = 0x01;			\
	USBCKS = 1;					\
	USBCKS2 = 0;				\
	EA = 1;						\
}


// void SystemClock_Config(void)
// {
// 	EA = 0;

// 	CKCON = 0x00;			      // 设置外部数据总线为最快
// 	WTST = 1;               	// 设置程序代码等待参数，赋值为0可将CPU执行程序的速度设置为最快
// 	P_SW2 = 0x80;			    // 开启特殊地址访问

// 	CLKDIV = 0x04;			//主时钟MCLK输出到系统时钟(SYSCLK)分频1
  
// 	IRTRIM = CHIPID12;     		//内部时钟源选择24M
// 	HIRCSEL1 = 1;
// 	HIRCSEL0 = 0;				//27Mhz频段

// 	HIRCCR = 0x80;
// 	while (!(HIRCCR & 0x01));
	
// 	// MCLKOCR = 72;         	//分频72,输出时钟的分频

// 	CLKSEL = 0x40; 			//PLL,高速IO，系统时钟源的相关设置(先选择内部IRC作为系统时钟)

// 	USBCLK &= 0x0F;
// 	USBCLK |= 0xA0;
// 	NOP(5);					//等待时钟稳定

// 	//PLL产生96Mhz时钟

// 	CLKDIV = 0X01;			//主时钟MCLK输出到系统时钟(SYSCLK)分频1

// 	CLKSEL |= 0x08;			//MCLK选择PLL/2为时钟源->48Mhz

// 	HSCLKDIV = 0x01;		//高速PWM,SPI,I2S,TFPU时钟96MHz

// 	USBCKS = 1;				
// 	USBCKS2 = 0;			//USB时钟选择48Mhz

// 	EA = 1;
// }

#define bl_was_software_reset()

#define JumpToApplication() 	IAP_CONTR = 0x20

