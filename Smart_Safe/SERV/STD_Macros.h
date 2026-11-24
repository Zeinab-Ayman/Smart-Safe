/*
 * STD_Macros.h
 *
 * Created: 11/21/2026 2:23:55 AM
 *  Author: Zeinab
 */ 


#ifndef STD_MACROS_H_
#define STD_MACROS_H_


#define SET_BIT(reg,bit)	reg |= (1<<bit)
#define CLR_BIT(reg,bit)	reg &= (~(1<<bit))
#define TOG_BIT(reg,bit)	reg ^= (1<<bit)
#define GET_BIT(reg,bit)	(reg>>bit) &1


#endif /* STD_MACROS_H_ */