/*
 * EEP.c
 *
 * Created: 11/21/2026 2:16:00 AM
 *  Author: Zeinab
 */ 

#include "../../SERV/STD_Macros.h"
#include "EEP.h"
#include "EEP_REG.h"

void EEPROM_write(uint16 address, uint8 data){
	while (GET_BIT(EECR, EEWE) == 1);
	EEARL = (uint8)address;
	EEARH = (uint8)(address >> 8);
	EEDR  = data;
	SET_BIT(EECR, EEMWE);
	SET_BIT(EECR, EEWE);
}
uint8 EEPROM_read(uint16 address){
	while (GET_BIT(EECR, EEWE) == 1);
	EEARL = (uint8)address;
	EEARH = (uint8)(address >> 8);
	SET_BIT(EECR, EERE);
	return EEDR;
}