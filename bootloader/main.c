/*
  bootloader for AM32 ESC firmware

  based on https://github.com/AlkaMotors/AT32F051_AM32_Bootloader
 */
#include "main.h"
#include <stdio.h>

#include "version.h"

/* Includes ------------------------------------------------------------------*/
#include "nstdbool.h"
#include "nstdint.h"

//#pragma optimize("O0")

#include "eeprom.h"

//#define USE_ADC_INPUT      // will go right to application and ignore eeprom
//#define UPDATE_EEPROM_ENABLE

// use this to check the clock config for the MCU (with a logic
// analyser on the input pin)
//#define BOOTLOADER_TEST_CLOCK

// use this to check the string output code. When enabled
// the string HELLO_WORLD is output every 10ms
//#define BOOTLOADER_TEST_STRING

// when there is no app fw yet, disable jump()
//#define DISABLE_JUMP

// optionally enable stats on serial bit-banging
//#define SERIAL_STATS

/*
  enable checking for software reset for jump.
  generally on a software reset we do want to stay in the bootloader
  if the signal pin is floating, but disabling this can be useful for
  CAN testing
*/
#define CHECK_SOFTWARE_RESET 1

/*
  enable checking for eeprom configured before jump
  disabling this can be useful for CAN development
*/
#define CHECK_EEPROM_BEFORE_JUMP 1

/*
  should we update the bootloader version in eeprom?
 */
#define UPDATE_EEPROM_ENABLE 1

#include <string.h>

#ifndef MCU_FLASH_START
#define MCU_FLASH_START 0x0000
#endif

#ifndef FIRMWARE_RELATIVE_START
#define FIRMWARE_RELATIVE_START 0x1000
#endif

#define USE_P00

#ifdef USE_P00
#define input_pin        P00
#define input_port       P0
#define PIN_NUMBER       0
#define PORT_LETTER      0
#elif defined(USE_P02)
#define input_pin         P02
#define input_port        P0
#define PIN_NUMBER        2
#define PORT_LETTER       0
#elif defined(USE_P04)
#define input_pin         P04
#define input_port        P0
#define PIN_NUMBER        4
#define PORT_LETTER       0
#elif defined(USE_P06)
#define input_pin         P06
#define input_port        P0
#define PIN_NUMBER        6
#define PORT_LETTER       0
#elif defined(USE_P10)
#define input_pin         P10
#define input_port        P1
#define PIN_NUMBER        0
#define PORT_LETTER       0
#elif defined(USE_P12)
#define input_pin         P12
#define input_port        P1
#define PIN_NUMBER        2
#define PORT_LETTER       0
#elif defined(USE_P14)
#define input_pin         P14
#define input_port        P1
#define PIN_NUMBER        4
#define PORT_LETTER       0
#elif defined(USE_P16)
#define input_pin         P16
#define input_port        P1
#define PIN_NUMBER        6
#define PORT_LETTER       0
#elif defined(USE_P20)
#define input_pin         P20
#define input_port        P2
#define PIN_NUMBER        0
#define PORT_LETTER       0
#elif defined(USE_P22)
#define input_pin         P22
#define input_port        P2
#define PIN_NUMBER        2
#define PORT_LETTER       0
#elif defined(USE_P24)
#define input_pin         P24
#define input_port        P2
#define PIN_NUMBER        4
#define PORT_LETTER       0
#elif defined(USE_P26)
#define input_pin         P26
#define input_port        P2
#define PIN_NUMBER        6
#define PORT_LETTER       0
#else
#error "Bootloader comms pin not defined"
#endif

static uint16_t invalid_command;

#include "blutil.h"

#ifndef BOARD_FLASH_SIZE
#error "must define BOARD_FLASH_SIZE"
#endif

#define PIN_CODE (PORT_LETTER << 4 | PIN_NUMBER)

/*
  currently only support 32, 64 or 128 k flash
 */
#if BOARD_FLASH_SIZE == 32
#define EEPROM_START_ADD (MCU_FLASH_START+0x7c00)
#define FLASH_SIZE_CODE 0x1f
#define ADDRESS_SHIFT 0

#elif BOARD_FLASH_SIZE == 64
#define EEPROM_START_ADD (MCU_FLASH_START+0xf800)
#define FLASH_SIZE_CODE 0x35
#define ADDRESS_SHIFT 0

#elif BOARD_FLASH_SIZE == 128
#define EEPROM_START_ADD (MCU_FLASH_START+0x1f800)
#define FLASH_SIZE_CODE 0x2B
#define ADDRESS_SHIFT 2 // addresses from the bl client are shifted 2 bits before being used
#else
#error "unsupported BOARD_FLASH_SIZE"
#endif

/*
  the devinfo structure tells the configuration client our pin code,
  flash size and device type. It can also be used by the main firmware
  to confirm we have the right eeprom address and pin code. We have 2
  32bit magic values so the main firmware can confirm the bootloader
  supports this feature
 */
#define DEVINFO_MAGIC1 0x5925e3da
#define DEVINFO_MAGIC2 0x4eb863d9

static const struct {
    uint32_t magic1;
    uint32_t magic2;
    const uint8_t deviceInfo[9];
} devinfo = {
        {DEVINFO_MAGIC1},
        {DEVINFO_MAGIC2},
        {'4','7','1',PIN_CODE,FLASH_SIZE_CODE,0x06,0x06,0x01,0x30}
};

typedef void (*pFunction)(void);

#define APPLICATION_ADDRESS     (uint32_t)(MCU_FLASH_START + FIRMWARE_RELATIVE_START)


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


static uint16_t low_pin_count;
static char receiveByte;
static int count;
static char messagereceived;
static uint16_t address_expected_increment;
static int cmd;
static char eeprom_req;
static int received;


static uint8_t rxBuffer[258];
static uint8_t payLoadBuffer[256];
static char rxbyte;
static uint32_t address;

typedef union __attribute__ ((packed)) {
    uint8_t bytes[2];
    uint16_t word;
} uint8_16_u;

static uint16_t len;
static uint8_t calculated_crc_low_byte;
static uint8_t calculated_crc_high_byte;
static uint16_t payload_buffer_size;
static char incoming_payload_no_command;

/* USER CODE BEGIN PFP */
static void sendString(const uint8_t dat[], int len);
static void receiveBuffer();
static void serialwriteChar(uint8_t dat);

#define BAUDRATE      19200
#define BITTIME          52 // 1000000/BAUDRATE
#define HALFBITTIME      26 // 500000/BAUDRATE

static void delayMicroseconds(uint32_t micros)
{
    bl_timer_reset();
    while (bl_timer_us() < micros) {
    }
}

/*
  jump to the application firmware
 */
static void jump()
{
#ifndef DISABLE_JUMP
#if CHECK_EEPROM_BEFORE_JUMP
	uint8_t value;
#endif
#ifndef DISABLE_APP_HEADER_CHECKS
	const uint32_t *app = (unsigned char far*)(MCU_FLASH_START + FIRMWARE_RELATIVE_START);
    const uint32_t ram_start = RAM_BASE;
    const uint32_t ram_limit_kb = RAM_SIZE + XRAM_SIZE;
    const uint32_t ram_end = ram_start+ram_limit_kb*1024;
#endif
#if CHECK_EEPROM_BEFORE_JUMP

	IAP_ENABLE();                           //设置等待时间，允许IAP操作，送一次就够
    IAP_READ();                             
	CMD_FAIL = 0;
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

	if (value != 0x01) {      // check first byte of eeprom to see if its programmed, if not do not jump
	invalid_command = 0;
	return;
    }

#endif
#ifndef DISABLE_APP_HEADER_CHECKS
    /*
      first word of the app is the stack pointer - make sure that it is in range
     */
    if (app[0] < ram_start || app[0] > ram_end) {
	invalid_command = 0;
	return;
    }
    /*
      2nd word is the entry point of the main app. Ensure that is in range
     */
    const uint32_t flash_limit_kb = 256;
    if (app[1] < APPLICATION_ADDRESS || app[1] > APPLICATION_ADDRESS+flash_limit_kb*1024) {
	// outside a 256k range, really unlikely to be a valid
	// application, don't jump
	invalid_command = 0;
	return;
    }

#endif
   JumpToApplication();
#endif
}


void makeCrc(uint8_t* pBuff, uint16_t length){
	uint16_t i;
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
	calculated_crc_low_byte = CRC_16.bytes[1];				//C251是大端编译模式
	calculated_crc_high_byte = CRC_16.bytes[0];
}

char checkCrc(uint8_t* pBuff, uint16_t length){

	uint8_t received_crc_low_byte2 = pBuff[length];          // one higher than len in buffer
	uint8_t received_crc_high_byte2 = pBuff[length+1];
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
	P0PU &= ~0x02; P0PD &= ~0x02; 	//无上下拉
	P0M0 |= 0x02; P0M1 &= ~0x02; 	//推挽输出

	delayMicroseconds(BITTIME);
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
	sendString(devinfo.deviceInfo,sizeof(devinfo.deviceInfo));
	setReceive();
}

bool checkAddressWritable(uint32_t address) {
	return address >= APPLICATION_ADDRESS;
}

void decodeInput(void){
	if(incoming_payload_no_command){
		len = payload_buffer_size;
		if(checkCrc(rxBuffer,len)){
			uint16_t i;
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

	if(cmd == CMD_RUN){        
		// starts the main app
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
		address = MCU_FLASH_START + (rxBuffer[2] << 8 | rxBuffer[3]);
		send_ACK();

		return;
	}

	if(cmd == CMD_SET_BUFFER){   
		// for writing buffer rx buffer 0 = command byte.  command set
		// address, input , format is CMD, 00 , 00 or 01 (if buffer is 256),
		// buffer_size,
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

	if(cmd == CMD_READ_FLASH_SIL){     
		// for sending contents of flash memory at the memory location set in
		// bootloader.c need to still set memory with data from set mem
		// command
		uint16_t out_buffer_size;
		uint8_t xdata* read_data;
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
		read_data = (uint8_t xdata*)malloc(out_buffer_size + 3);
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

#ifdef SERIAL_STATS
// stats for debugging serial protocol
struct {
    uint32_t no_idle;
    uint32_t no_start;
    uint32_t bad_start;
    uint32_t bad_stop;
    uint32_t good;
} stats;
#endif

/*
  read one byte from the input pin, 19200, not inverted, one stop bit

  return false if we can't get a byte, or the byte has bad framing
 */

static bool serialreadChar()
{
	int bits_to_read;
	rxbyte=0;

    bl_timer_reset();

    // UART is idle high, wait for it to be in the idle state
    while (!gpio_read(input_pin)) { // wait for rx to go high
	if (bl_timer_us() > 20000) {
	    /*
	      if we don't get a command for 20ms then assume we should
	      be trying to boot the main firmware, invalid_command 101
	      triggers the jump immediately
	     */
	    invalid_command = 101;
#ifdef SERIAL_STATS
	    stats.no_idle++;
#endif
	    return false;
	}
    }


    // now we need to wait for the start bit leading edge, which is low
    bl_timer_reset();
    while (gpio_read(input_pin)) {
	if (bl_timer_us() > 5*BITTIME && messagereceived) {
	    // we've been waiting too long, don't allow for long gaps
	    // between bytes
#ifdef SERIAL_STATS
	    stats.no_start++;
#endif
	    return false;
	}
    }


    // wait to get the center of bit time. We want to sample at the
    // middle of each bit
    delayMicroseconds(HALFBITTIME);
    if (gpio_read(input_pin)) {
	// bad framing, we should be half-way through the start bit
	// which should still be low
#ifdef SERIAL_STATS
	stats.bad_start++;
#endif
	return false;
    }

    /*
      now sample the 8 data bits
     */
    bits_to_read = 0;
    while (bits_to_read < 8) {
	delayMicroseconds(BITTIME);
	rxbyte = rxbyte | gpio_read(input_pin) << bits_to_read;
	bits_to_read++;
    }

    // wait till middle of stop bit, so we can check that too
    delayMicroseconds(BITTIME);
    if (!gpio_read(input_pin)) {
	// bad framing, stop bit should be high
#ifdef SERIAL_STATS
	stats.bad_stop++;
#endif
	return false;
    }

    // we got a good byte
    messagereceived = 1;
    receiveByte = rxbyte;
#ifdef SERIAL_STATS
    stats.good++;
#endif
    return true;
}

static void serialwriteChar(char dat)
{
	uint8_t bits_written = 0;
    // start bit is low
    gpio_clear(input_pin);
    delayMicroseconds(BITTIME);

	
    while (bits_written < 8) {
	if (dat & 0x01) {
	    gpio_set(input_pin);
	} else {
	    // GPIO_BC(input_port) = input_pin;
	    gpio_clear(input_pin);
	}
	bits_written++;
	dat = dat >> 1;
	delayMicroseconds(BITTIME);
    }

    // send stop bit
    gpio_set(input_pin);

    /*
      note that we skip the delay by BITTIME for the full stop bit and
      do it in sendString() instead to ensure when sending an ACK
      immediately followed by a setReceive() on a slow MCU that we
      start on the receive as soon as possible.
    */
}

static void sendString(const uint8_t *dat, int len){
	int i;
	for(i = 0; i < len; i++){
		serialwriteChar(dat[i]);
		// for multi-byte writes we add the stop bit delay
		delayMicroseconds(BITTIME);
	}
}

static void receiveBuffer()
{
	uint32_t i;
    count = 0;
    messagereceived = 0;
    memset(rxBuffer, 0, sizeof(rxBuffer));

    setReceive();

    for(i = 0; i < sizeof(rxBuffer); i++){
	if (!serialreadChar()) {
	    break;
	}

	if(incoming_payload_no_command) {
	    if(count == payload_buffer_size+2){
		break;
	    }
	    rxBuffer[i] = rxbyte;
	    count++;
	} else {
	    if(bl_timer_us() > 250){
	
		count = 0;

		break;
	    } else {
		rxBuffer[i] = rxbyte;
		if(i == 257){
		    invalid_command+=20;       // needs one hundred to trigger a jump but will be reset on next set address commmand

		}
	    }
	}
    }
    if (messagereceived) {
	decodeInput();
    }
}

#ifdef UPDATE_EEPROM_ENABLE
static void update_EEPROM()
{
    read_flash_bin(rxBuffer , EEPROM_START_ADD , 48);
    if (BOOTLOADER_VERSION != rxBuffer[2]) {
	if (rxBuffer[2] == 0xFF || rxBuffer[2] == 0x00){
	    return;
	}
	rxBuffer[2] = BOOTLOADER_VERSION;
	save_flash_nolib(rxBuffer, 48, EEPROM_START_ADD);
    }
}
#endif // UPDATE_EEPROM_ENABLE

static void checkForSignal()
{
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


	P3M0 |= 0x03; P3M1 &= ~0x03; 
	P2M0 |= 0x03; P2M1 &= ~0x03; 
}



int main(void)
{

	//Prevent warnings
	static uint8_t xdata MEMPOOL[1024];		//内存池
	

  	SystemClock_Config();
	init_mempool(MEMPOOL, sizeof(MEMPOOL));	//初始化内存池

	IAP_TPS =48;	//设置IAP等待时间

	PWMB_Timer_Init();

   	GPIO_INPUT_INIT();     // init the pin with a pullup

	// Uart1_Init();

   	checkForSignal();
	
	P0PD &= ~0x02;
	P0PU |= 0x02;			//上拉输入

  	deviceInfo[3] = pin_code;

	update_EEPROM();
  	while (1)
  	{
	  	recieveBuffer();
	  	if (invalid_command > 100){
		  	jump();
	  	}
  	}
}







static void GPIO_INPUT_INIT(void)
{
#ifdef USE_PB4
#endif
#ifdef USE_PA2
#endif

	P0M0 = 0x00; P0M1 = 0xff; 

    P0IE |= 0x02; 
	P0PU |= 0x02; 
	//高阻上拉输入
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




