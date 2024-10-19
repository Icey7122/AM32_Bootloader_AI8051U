/* Bootloader */

#define BOOTLOADER_VERSION 10

#define USE_P01
/* Includes ------------------------------------------------------------------*/
#include "nstdbool.h"
#include "main.h"

//#define USE_ADC_INPUT      // will go right to application and ignore eeprom

#include "nstdint.h"
#include <stdlib.h>
#include <string.h>
#include "bootloader.h"

#define STC32_FLASH_START 0x00000000
#define FIRMWARE_RELATIVE_START 0x0000
#define EEPROM_RELATIVE_START 0x0000

uint8_t bootloader_version = BOOTLOADER_VERSION;

typedef void (*pFunction)(void);

#define APPLICATION_ADDRESS     (uint32_t)(STC32_FLASH_START + FIRMWARE_RELATIVE_START) // 4k

#define EEPROM_START_ADD         (uint32_t)(STC32_FLASH_START + EEPROM_RELATIVE_START)
#define FLASH_END_ADD           (uint32_t)(STC32_FLASH_START + 0xFFFF)               // 32 k


#define CMD_RUN             0x00
#define CMD_PROG_FLASH      0x01
#define CMD_ERASE_FLASH     0x02
#define CMD_READ_FLASH_SIL  0x03
#define CMD_VERIFY_FLASH    0x03
#define CMD_VERIFY_FLASH_ARM 0x04
#define CMD_READ_EEPROM     0x04
#define CMD_PROG_EEPROM     0x05
#define CMD_READ_SRAM       0x06
#define CMD_READ_FLASH_ATM  0x07
#define CMD_KEEP_ALIVE      0xFD
#define CMD_SET_ADDRESS     0xFF
#define CMD_SET_BUFFER      0xFE


#ifdef USE_P01

#define input_pin        P01
#define input_port       P0
#define PIN_NUMBER       0
#define PORT_LETTER      0

#endif

#ifdef USE_PB4
// #define input_pin       LL_GPIO_PIN_4
// #define input_port        GPIOB
// #define PIN_NUMBER        4
// #define PORT_LETTER       1
#endif


uint16_t low_pin_count = 0;
char receviedByte;
int receivedCount;
int count = 0;
char messagereceived = 0;
uint16_t invalid_command = 0;
uint16_t address_expected_increment;
int cmd = 0;
char eeprom_req = 0;
int received;
uint8_t port_letter;


uint8_t pin_code = PORT_LETTER << 4 | PIN_NUMBER;
uint8_t deviceInfo[9] = { 0 };      // stm32 device info

size_t str_len;
char connected = 0;
uint8_t rxBuffer[258];
uint8_t payLoadBuffer[256];
char rxbyte=0;
uint32_t address;
int tick = 0;

typedef union {
    uint8_t bytes[2];
    uint16_t word;
} uint8_16_u;
uint16_t len;
uint8_t received_crc_low_byte;
uint8_t received_crc_high_byte;
uint8_t calculated_crc_low_byte;
uint8_t calculated_crc_high_byte;
uint16_t payload_buffer_size;
char incoming_payload_no_command = 0;

char bootloaderactive = 1;

uint32_t JumpAddress;
// pFunction JumpToApplication;
#define JumpToApplication() 	IAP_CONTR = 0x20


void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
static void PWMB_Timer_Init(void);

/* USER CODE BEGIN PFP */
static void GPIO_INPUT_INIT(void);

void processmessage(void);
void serialwriteChar(char dat);
void sendString(uint8_t dat[], int len);
void recieveBuffer();

#define BAUDRATE              19200
#define BITTIME          1000000/BAUDRATE
#define HALFBITTIME       500000/BAUDRATE

//待改（使用定时器做延时函数）
void delayMicroseconds(uint32_t micros){
	// TIM2->CNT = 0;
	PWMB_CNTRH = 0x00;
	PWMB_CNTRL = 0x00;
	while (((uint16_t)PWMB_CNTRH << 8 | PWMB_CNTRL) < micros){
	}
}

void jump(){

	uint8_t value;

	EA = 0;

	IAP_ENABLE();                           //设置等待时间，允许IAP操作，送一次就够
    IAP_READ();                             

	IAP_ADDRE = (uint8_t)(EEPROM_START_ADD >> 16); 
	IAP_ADDRH = (uint8_t)(EEPROM_START_ADD >> 8);  
	IAP_ADDRL = (uint8_t)EEPROM_START_ADD;         
	IAP_TRIG = 0x5A;
	IAP_TRIG = 0xA5;                   
	_nop_();   
	_nop_();
	_nop_();
	_nop_();
	while(CMD_FAIL);

	value = IAP_DATA;            //读出的数据送往

	IAP_DISABLE();

#ifdef USE_ADC_INPUT
#else
	if (value != 0x01){      // check first byte of eeprom to see if its programmed, if not do not jump
		invalid_command = 0;
		return;
	}
#endif
   JumpToApplication();
}


void makeCrc(uint8_t* pBuff, uint16_t length){
	int i;
	uint8_t xb;
	static uint8_16_u CRC_16;
	
	CRC_16.word=0;
	for(i = 0; i < length; i++) {
		uint8_t j;
		xb = pBuff[i];
		for ( j = 0; j < 8; j++)
		{
			if (((xb & 0x01) ^ (CRC_16.word & 0x0001)) !=0 ) {
				CRC_16.word = CRC_16.word >> 1;
				CRC_16.word = CRC_16.word ^ 0xA001;
			} else {
				CRC_16.word = CRC_16.word >> 1;
			}
			xb = xb >> 1;
		}
	}
	calculated_crc_low_byte = CRC_16.bytes[0];
	calculated_crc_high_byte = CRC_16.bytes[1];
}

char checkCrc(uint8_t* pBuff, uint16_t length){

	char received_crc_low_byte2 = pBuff[length];          // one higher than len in buffer
	char received_crc_high_byte2 = pBuff[length+1];
	makeCrc(pBuff,length);
	if((calculated_crc_low_byte==received_crc_low_byte2)   && (calculated_crc_high_byte==received_crc_high_byte2)){
		return 1;
	}else{
		return 0;
	}

}

//待改（接收引脚初始化）上拉输入
void setReceive(void){

	GPIO_INPUT_INIT();
	received = 0;

}
//待改（发送引脚初始化）
void setTransmit(void){
	// LL_GPIO_SetPinMode(input_port, input_pin, LL_GPIO_MODE_OUTPUT);       // set as reciever // clear bits and set receive bits..
	P0M0 |= 0x02; P0M1 &= ~0x02; 	//推挽输出
	P0PU &= ~0x02; P0PD &= ~0x02; 	//无上下拉
}

void send_ACK(void){
    setTransmit();
    serialwriteChar(0x30);             // good ack!
	setReceive();
}

void send_BAD_ACK(void){
	setTransmit();
	serialwriteChar(0xC1);                // bad command message.
	setReceive();
}

void send_BAD_CRC_ACK(){
    setTransmit();
	serialwriteChar(0xC2);                // bad command message.
	setReceive();
}

void sendDeviceInfo(){
	setTransmit();
	sendString(deviceInfo,9);
	setReceive();
}

bool checkAddressWritable(uint32_t address) {
	return address >= APPLICATION_ADDRESS;
}

void decodeInput(){
	if(incoming_payload_no_command){
		len = payload_buffer_size;
	//	received_crc_low_byte = rxBuffer[len];          // one higher than len in buffer
	//	received_crc_high_byte = rxBuffer[len+1];
		if(checkCrc(rxBuffer,len)){
			int i;
			memset(payLoadBuffer, 0, sizeof(payLoadBuffer));             // reset buffer

			for(i = 0; i < len; i++){
				payLoadBuffer[i]= rxBuffer[i];
			}
			send_ACK();
			incoming_payload_no_command = 0;
			return;
		}else{
			send_BAD_CRC_ACK();
			return;
		}
	}

	cmd = rxBuffer[0];

	if(rxBuffer[16] == 0x7d){
		if(rxBuffer[8] == 13 && rxBuffer[9] == 66){
			sendDeviceInfo();
			rxBuffer[20]= 0;

		}
		return;
	}

	if(rxBuffer[20] == 0x7d){
		if(rxBuffer[12] == 13 && rxBuffer[13] == 66){
			sendDeviceInfo();
			rxBuffer[20]= 0;
			return;
		}

	}
	if(rxBuffer[40] == 0x7d){
		if(rxBuffer[32] == 13 && rxBuffer[33] == 66){
			sendDeviceInfo();
			rxBuffer[20]= 0;
			return;
		}
	}

	if(cmd == CMD_RUN){         // starts the main app
		if((rxBuffer[1] == 0) && (rxBuffer[2] == 0) && (rxBuffer[3] == 0)){
			invalid_command = 101;
		}
	}

	if(cmd == CMD_PROG_FLASH){
		len = 2;
		if (!checkCrc((uint8_t*)rxBuffer, len)) {
			send_BAD_CRC_ACK();

			return;
		}

		if (!checkAddressWritable(address)) {
			send_BAD_ACK();

			return;
		}

		save_flash_nolib((uint8_t*)payLoadBuffer, payload_buffer_size,address);
		send_ACK();

	 	return;
	}

	if(cmd == CMD_SET_ADDRESS){             //  command set addressinput format is: CMD, 00 , High byte address, Low byte address, crclb ,crchb
		len = 4;  // package without 2 byte crc
		if (!checkCrc((uint8_t*)rxBuffer, len)) {
			send_BAD_CRC_ACK();

			return;
		}


	    // will send Ack 0x30 and read input after transfer out callback
		invalid_command = 0;
		address = STC32_FLASH_START + (rxBuffer[2] << 8 | rxBuffer[3]);
		send_ACK();

		return;
	}

	if(cmd == CMD_SET_BUFFER){        // for writing buffer rx buffer 0 = command byte.  command set address, input , format is CMD, 00 , 00 or 01 (if buffer is 256), buffer_size,
		len = 4;  // package without 2 byte crc
		if (!checkCrc((uint8_t*)rxBuffer, len)) {
			send_BAD_CRC_ACK();

			return;
		}

        // no ack with command set buffer;
       	if(rxBuffer[2] == 0x01){
       		payload_buffer_size = 256;                          // if nothing in this buffer
       	}else{
	        payload_buffer_size = rxBuffer[3];
        }
	    incoming_payload_no_command = 1;
	    address_expected_increment = 256;
        setReceive();

        return;
	}

	if(cmd == CMD_KEEP_ALIVE){
		len = 2;
		if (!checkCrc((uint8_t*)rxBuffer, len)) {
			send_BAD_CRC_ACK();

			return;
		}

	   	setTransmit();
	 	serialwriteChar(0xC1);                // bad command message.
		setReceive();

		return;
	}

	if(cmd == CMD_ERASE_FLASH){
		len = 2;
		if (!checkCrc((uint8_t*)rxBuffer, len)) {
			send_BAD_CRC_ACK();

			return;
		}

		if (!checkAddressWritable(address)) {
			send_BAD_ACK();

			return;
		}

		send_ACK();
		return;
	}

	if(cmd == CMD_READ_EEPROM){
		eeprom_req = 1;
	}

	if(cmd == CMD_READ_FLASH_SIL){     // for sending contents of flash memory at the memory location set in bootloader.c need to still set memory with data from set mem command
		uint16_t out_buffer_size;
		uint8_t *read_data;
		len = 2;
		if (!checkCrc((uint8_t*)rxBuffer, len)) {
			send_BAD_CRC_ACK();

			return;
		}

		count++;
		out_buffer_size = rxBuffer[1];//
		if(out_buffer_size == 0){
			out_buffer_size = 256;
		}
		address_expected_increment = 128;
		read_data = (uint8_t*)malloc(out_buffer_size + 3);
		setTransmit();														// make buffer 3 larger to fit CRC and ACK
		memset(read_data, 0, sizeof(read_data));
        //    read_flash((uint8_t*)read_data , address);                 	// make sure read_flash reads two less than buffer.
		read_flash_bin((uint8_t*)read_data , address, out_buffer_size);

        makeCrc(read_data,out_buffer_size);
        read_data[out_buffer_size] = calculated_crc_low_byte;
        read_data[out_buffer_size + 1] = calculated_crc_high_byte;
        read_data[out_buffer_size + 2] = 0x30;
        sendString(read_data, out_buffer_size+3);

		setReceive();
		free(read_data);
		return;
	}

    setTransmit();

	serialwriteChar(0xC1);                // bad command message.
	invalid_command++;
 	setReceive();
}

void serialreadChar()
{
	int bits_to_read;
	rxbyte=0;
	// PWMB_PSCRH = 0x00;
	PWMB_PSCRL = 0x03; // set to 1/4mhz
	while(~(input_pin)){ // wait for rx to go high
		if(((uint16_t)PWMB_CNTRH << 8 | PWMB_CNTRL) > 50000){
				invalid_command = 101;
				return;
		}
	}
	PWMB_PSCRL = 0x00; // set to 1/4mhz
	while(input_pin){   // wait for it go go low
		if(((uint16_t)PWMB_CNTRH << 8 | PWMB_CNTRL) > 250 && messagereceived){
			return;
		}
	}

	delayMicroseconds(HALFBITTIME);//wait to get the center of bit time

	bits_to_read = 0;
	while (bits_to_read < 8) {
		delayMicroseconds(BITTIME);
		rxbyte = rxbyte | ((uint8_t)(input_pin) >> PIN_NUMBER) << bits_to_read;
	bits_to_read++;
	}

	delayMicroseconds(HALFBITTIME); //wait till the stop bit time begins
	messagereceived = 1;
	receviedByte = rxbyte;
	//return rxbyte;

}

void serialwriteChar(char dat)
{

	//BRR 只写寄存器：只能改变管脚状态为低电平，对寄存器 管脚对于位写 1 相应管脚会为低电平。写 0 无动作。
	// input_port->BRR = input_pin;; //initiate start bit
	char bits_to_read = 0;

	input_pin = 0;					//initiate start bit

	
	while (bits_to_read < 8) {

		delayMicroseconds(BITTIME);

		if (dat & 0x01) {
			// input_port->BSRR = input_pin;
			input_pin = 1;
		}else{
			// input_port->BRR = input_pin;
			input_pin = 0;
		}
		bits_to_read++;
		dat = dat >> 1;
	}

	delayMicroseconds(BITTIME);

	// input_port->BSRR = input_pin; //write the stop bit


	input_pin = 1; 					//write the stop bit


	// if more than one byte a delay is needed after stop bit,
	//if its the only one no delay, the sendstring function adds delay after each bit

	//if(cmd == 255 || cmd == 254 || cmd == 1  || incoming_payload_no_command){
	//
	//}else{
	//	delayMicroseconds(BITTIME);
	//}
}

void sendString(uint8_t *dat, int len){
	int i;
	for(i = 0; i < len; i++){
		serialwriteChar(dat[i]);
		delayMicroseconds(BITTIME);
	}
}

void recieveBuffer(void){

	int i = 0;
	count = 0;
	messagereceived = 0;
	memset(rxBuffer, 0, sizeof(rxBuffer));


	for(i = 0; i < sizeof(rxBuffer); i++){
		serialreadChar();
		if(incoming_payload_no_command){
			if(count == payload_buffer_size+2){
				break;
			}
			rxBuffer[i] = rxbyte;
			count++;
		}else{
			if(((uint16_t)PWMB_CNTRH << 8 | PWMB_CNTRL) > 250){
			count = 0;
			break;
			}else{
			rxBuffer[i] = rxbyte;
			if(i == 257){
				invalid_command+=20;       // needs one hundred to trigger a jump but will be reset on next set address commmand

				}
			}
		}
	}
	decodeInput();
}

void update_EEPROM(void){
	read_flash_bin(rxBuffer , EEPROM_START_ADD , 48);
	if(BOOTLOADER_VERSION != rxBuffer[2]){
		if (rxBuffer[2] == 0xFF || rxBuffer[2] == 0x00){
			return;
		}
		rxBuffer[2] = BOOTLOADER_VERSION;
		save_flash_nolib(rxBuffer, 48, EEPROM_START_ADD);
	}
}

void checkForSignal(void){
	//uint8_t floating_or_signal= 0;
	// LL_GPIO_SetPinPull(input_port, input_pin, LL_GPIO_PULL_DOWN);
	int i;

	P0PU &= ~0x02; //关闭上拉电阻
	P0PD |= 0x02; //开启下拉电阻

	delayMicroseconds(500);

	for(i = 0 ; i < 500; i ++){
		if(~input_pin){
			low_pin_count++;
		}else{
	//	 high_pin_count++;
		}
		delayMicroseconds(10);
	}

	if(low_pin_count == 0){
		return;           // all high while pin is pulled low, bootloader signal
	}

	low_pin_count = 0;

	P0PU &= ~0x02; //关闭上拉电阻
	P0PD &= ~0x02; //关闭下拉电阻 
	// LL_GPIO_SetPinPull(input_port, input_pin, LL_GPIO_PULL_NO);
	delayMicroseconds(500);

	for(i = 0 ; i < 500; i ++){
		if(~input_pin){
			low_pin_count++;
		}
		delayMicroseconds(10);
	}

	if(low_pin_count == 0){
		return;            // when floated all
	}

	if(low_pin_count > 0){
		jump();
	}
}


void Uart1_Init(void)	//921600bps@48MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xF3;			//设置定时初始值
	T2H = 0xFF;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
}

#include <stdio.h>


int main(void)
{

	//Prevent warnings
	(void)bootloader_version;

  	// LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  	// LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  	// FLASH->ACR |= FLASH_ACR_PRFTBE;   // prefetch buffer enable

  	SystemClock_Config();
	Uart1_Init();

	P3M0 |= 0x03; P3M1 &= ~0x03; 
	P2M0 |= 0x03; P2M1 &= ~0x03; 

	PWMB_Timer_Init();


	while (1)
	{
		// printf("SystemClock_Config\n");
		// printf("%u\n",((uint16_t)PWMB_CNTRH << 8 | PWMB_CNTRL));
	}
	
	
  	
//   	// LL_TIM_EnableCounter(TIM2);

//    	GPIO_INPUT_INIT();     // init the pin with a pulldown

//    	checkForSignal();


// 	P0PD &= ~0x02;
// 	P0PU |= 0x02;			//上拉输入
//    	// LL_GPIO_SetPinPull(input_port, input_pin, LL_GPIO_PULL_UP);

// #ifdef USE_ADC_INPUT  // go right to application
//   	jump();
// #endif
//   	deviceInfo[3] = pin_code;
//   	update_EEPROM();

// //  sendDeviceInfo();
//   	while (1)
//   	{
// 	  	recieveBuffer();
// 	  	if (invalid_command > 100){
// 		  	jump();
// 	  	}
//   	}

}



void SystemClock_Config(void)
{
	EA = 0;

	CKCON = 0x00;			      // 设置外部数据总线为最快
	WTST = 1;               	// 设置程序代码等待参数，赋值为0可将CPU执行程序的速度设置为最快
	P_SW2 = 0x80;			    // 开启特殊地址访问

	CLKDIV = 0x04;			//主时钟MCLK输出到系统时钟(SYSCLK)分频1
       
	IRTRIM = CHIPID12;     		//内部时钟源选择24M
	HIRCSEL1 = 1;
	HIRCSEL0 = 0;

	HIRCCR = 0x80;
	while (!(HIRCCR & 0x01));
	
	// MCLKOCR = 72;         	//分频72,输出时钟的分频

	CLKSEL = 0x40; 			//PLL,高速IO，系统时钟源的相关设置(先选择内部IRC作为系统时钟)

	USBCLK &= 0x0F;
	USBCLK |= 0xA0;
	NOP(20);					//等待时钟稳定

	//PLL产生96Mhz时钟

	CLKDIV = 0X01;			//主时钟MCLK输出到系统时钟(SYSCLK)分频1

	CLKSEL |= 0x08;			//MCLK选择PLL/2为时钟源->48Mhz

	HSCLKDIV = 0x01;		//PWM,SPI,I2S,TFPU时钟96MHz

	USBCKS = 1;				
	USBCKS2 = 0;			//USB时钟选择48Mhz

	EA = 1;
}


static void PWMB_Timer_Init(void)
{
	PWMB_ENO = 0x00;		//禁止PWMB的PWM输出
	PWMB_IOAUX = 0x00;		//禁止PWMB

	PWMB_ARRH = 0x03;
	PWMB_ARRL = 0xE8;		//设置PWMB周期为65535
	PWMB_CNTRH = 0x00;
	PWMB_CNTRL = 0x00;		//清零计数器
	PWMB_PSCRH = 0xBB;		
	PWMB_PSCRL = 0x80;		//PWMB时钟源分频到1Mhz
	PWMB_IER = 0x01;		//禁止PWMB中断
	PWMB_CR1 = 0x01;		//使能计数器
}



static void GPIO_INPUT_INIT(void)
{
#ifdef USE_PB4
#endif
#ifdef USE_PA2
#endif

    P0M0 &= ~0x02; 
	P0M1 |= 0x02; 	
    P0NCS &= ~0x02; 
    P0IE |= 0x02; 
	P0PU |= 0x02; 
	//高阻上拉输入
}





void PWMB_Timer_Handler(void) interrupt PWMB_VECTOR
{
	PWMB_SR1 = 0x00;
	P20 = ~P20;
	printf("ADCC\n");
}

#pragma FUNCTIONS (static)
char putchar(char c)
{
	// serialwriteChar(c);
	SBUF = c;
	while (!TI);
	TI = 0;
	return c;
}





