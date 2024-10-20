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

#include "bootloader.h"
#include <string.h>
#include "stdio.h"


#define page_size 0x200                   // 512 bytes for STC8051U

extern void delayMicroseconds(uint32_t micros);

void save_flash_nolib(uint8_t *dat, int length, uint32_t add){

	IAP_ENABLE();                       	
	// unlock flash
	// erase page if address even divisable by 512
	if((add % page_size) == 0){

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

		while(CMD_FAIL)
		{
			printf("CMD_FAIL\n");
		}                  
	}

	IAP_WRITE();  			//宏调用, 送字节写命令
	do
    {
        IAP_ADDRE = (uint8_t)(add >> 16); 
        IAP_ADDRH = (uint8_t)(add >> 8);  
        IAP_ADDRL = (uint8_t)add;         
        IAP_DATA  = *dat;      

		IAP_TRIG = 0x5A;
		IAP_TRIG = 0xA5;                   
		_nop_();   
		_nop_();
		_nop_();
		_nop_();

		while(CMD_FAIL)
		{
			printf("CMD_FAIL\n");
		}

        add++;                     //下一个地址
        dat++;                    //下一个数据
    }
    while(--length);                    //直到结束


	IAP_DISABLE();                      //关闭IAP
}

void read_flash_bin(uint8_t*  dat , uint32_t add , int out_buff_len){
	int i;
	IAP_ENABLE();                           //设置等待时间，允许IAP操作，送一次就够
    IAP_READ();                             //送字节读命令，命令不需改变时，不需重新送命令
	for (i = 0; i < out_buff_len ; i ++){
		CMD_FAIL = 0;
        IAP_ADDRE = (uint8_t)((add+i) >> 16); //送地址高字节（地址需要改变时才需重新送地址）
        IAP_ADDRH = (uint8_t)((add+i) >> 8);  //送地址中字节（地址需要改变时才需重新送地址）
        IAP_ADDRL = (uint8_t)(add+i);         //送地址低字节（地址需要改变时才需重新送地址）

		IAP_TRIG = 0x5A;
		IAP_TRIG = 0xA5;                   
		_nop_();   
		_nop_();
		_nop_();
		_nop_();
		while(CMD_FAIL);

        dat[i] = IAP_DATA;            //读出的数据送往
	}

	IAP_DISABLE();                      
}


