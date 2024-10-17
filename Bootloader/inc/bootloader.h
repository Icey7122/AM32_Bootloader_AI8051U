/*
 * bootloader.h
 *
 *  Created on: Mar. 25, 2020
 *      Author: Alka
 */
#include "main.h"


#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_
#endif /* INC_BOOTLOADER_H_ */

#define		IAP_STANDBY()	IAP_CMD = 0		
#define		IAP_READ()		IAP_CMD = 1		
#define		IAP_WRITE()		IAP_CMD = 2		
#define		IAP_ERASE()		IAP_CMD = 3		

#define	IAP_ENABLE()		IAPEN = 1; IAP_TPS = 48
#define	IAP_DISABLE()		IAP_CONTR = 0; IAP_CMD = 0; IAP_TRIG = 0; IAP_ADDRH = 0xFF; IAP_ADDRL = 0xFF

void read_flash_bin(uint8_t*  dat , uint32_t add ,int  out_buff_len);
void save_flash_nolib(uint8_t *dat, int length, uint32_t add);
