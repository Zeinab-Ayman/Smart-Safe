/*
 * EEP_REG.h
 *
 * Created: 11/21/2026 2:15:14 AM
 *  Author: Zeinab
 */ 


#ifndef EEP_REG_H_
#define EEP_REG_H_


#include "../../Serv/STD_TYPES.h"

#define EEAR	*((volatile uint16 *)(0x3E))
#define EEARL	*((volatile uint8 *)(0x3E))
#define EEARH	*((volatile uint8 *)(0x3F))

#define EEDR	*((volatile uint8 *)(0x3D))

#define EECR	*((volatile uint8 *)(0x3C))
#define EERE	0
#define EEWE	1
#define EEMWE	2
#define EERIE	3

#endif /* EEP_REG_H_ */