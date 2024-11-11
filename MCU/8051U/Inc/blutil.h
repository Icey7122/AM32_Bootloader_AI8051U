/*
  MCU specific utility functions for the bootloader
 */
/*
  based on https://github.com/AlkaMotors/AM32_Bootloader_F051/blob/main/Core/
 */
#ifndef __BLUTIL_H
#define __BLUTIL_H

// /*
//   34k ram
//  */
#define RAM_BASE 0x0000
#define RAM_SIZE 34*1024

// /*
//   use 32k of flash
//  */
#define BOARD_FLASH_SIZE 64

#define GPIO_PIN(n) (1U<<(n))


// uint32_t SystemCoreClock = 8000000U;


/*
  initialise timer for 1us per tick
 */
#define bl_timer_init()\
{\
  PWMB_PSCRH = 0x00;\
  PWMB_PSCRL = 39;\
  PWMB_ARRH = 0xFF;\
  PWMB_ARRL = 0xFF;\
  PWMB_CNTRH = 0xFF;\
  PWMB_CNTRL = 0xFF;\
  PWMB_CR1 |= 0x01;\
}


/*
  disable timer ready for app start
 */
#define bl_timer_disable() PWMB_CR1 &= ~0x01

#define bl_timer_us() ((uint16_t)PWMB_CNTRH << 8 | PWMB_CNTRL)

#define bl_timer_reset() PWMB_EGR = 0x01

/*
  initialise clocks
 */
#define bl_clock_config()\
{\
  EA = 0;\
  CKCON = 0x00;\
  WTST = 0;\     	
  P_SW2 = 0x80;\	  
  IRCBAND |= 0x03;\
	VRTRIM = 26;\
	IRTRIM = 107;\
	USBCLK |= (1<<7);\
	USBCLK &= ~(3<<5);\
	USBCLK |= (2<<5);\
	NOP(5);\
	CLKSEL |= (1<<7);\
	CLKSEL |= (1<<6);\
	MCLKOCR = 0x01;\
	IRC48MCR = 0x80;\
	while(!(IRC48MCR&1));\
	IRCBAND &= ~(3<<6);\
	IRCBAND |= (2<<6);\
	EA = 1;\
}

// void bl_clock_config(void)
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

#define bl_gpio_init()  gpio_mode_set(input_port,mode_set_pin,GPIO_Mode_IPU)

/*
  return true if the MCU booted under a software reset
 */
#define bl_was_software_reset() RSTFLAG & 0x04

/*
  jump from the bootloader to the application code
 */
#define jump_to_application() ((void (far *)())(MCU_FLASH_START + FIRMWARE_RELATIVE_START))();



#endif // !__BLUTIL_H