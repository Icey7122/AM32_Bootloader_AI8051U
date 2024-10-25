/*
 * bootloader.c
 *
 *  Created on: Mar. 25, 2020
 *      Author: Alka
 *  Porting on: Mar. 13, 2024
 * 		Porting code from F051 to STC8051U
 * 	
 *
 */

#include "eeprom.h"
#include <string.h>

#define page_size 0x200                   // 512 bytes for STC8051U

bool save_flash_nolib(const uint8_t* dat, uint32_t length, uint32_t add)
{
	uint32_t i;

	IAP_ENABLE();                       	
	// unlock flash
	// erase page if address even divisable by 512
	if((add % page_size) == 0){
		CMD_FAIL = 0;

		IAP_ERASE();                        

		IAP_ADDRE = (uint8_t)(add >> 16); 
		IAP_ADDRH = (uint8_t)(add >> 8);  
		IAP_ADDRL = (uint8_t)add;         

		IAP_TRIG = 0x5A;
		IAP_TRIG = 0xA5;                   
		_nop_();   
		_nop_();
		_nop_();
		_nop_();

		while(CMD_FAIL);       
	}

	IAP_WRITE();  			//宏调用, 送字节写命令

	for (i = 0; i < length; i++)
	{
		CMD_FAIL = 0;

        IAP_ADDRE = (uint8_t)((add + i) >> 16); 
        IAP_ADDRH = (uint8_t)((add + i) >> 8);  
        IAP_ADDRL = (uint8_t)(add + i);         
        IAP_DATA  = *(dat + i);      

		IAP_TRIG = 0x5A;
		IAP_TRIG = 0xA5;                   
		_nop_();   
		_nop_();
		_nop_();
		_nop_();

		while(CMD_FAIL);
	}
	
	IAP_DISABLE();                      //关闭IAP

	return memcmp(dat, (unsigned char far *)add, length) == 0;
}

void read_flash_bin(uint8_t* dat, uint32_t add, int out_buff_len)
{
	memcpy(dat, (unsigned char far*)add, out_buff_len);               
}


