/*
 * EEP.h
 *
 * Created: 11/21/2026 2:16:14 AM
 *  Author: Zeinab
 */ 


#ifndef EEP_H_
#define EEP_H_

#include "../../SERV/STD_TYPES.h"

void EEPROM_write(uint16 address, uint8 data);
uint8 EEPROM_read(uint16 address);


#endif /* EEP_H_ */