/*
 * KPD.c
 *
 * Created: 11/20/2026 11:47:26 PM
 *  Author: Zeinab
 */ 


#include "../../MCAL/DIO/DIO.h"
#include "KPD.h"
#include "KPD_Config.h"
#include <util/delay.h>

#if ROW_NIBBLE == HIGHNIBBLE
#define ROW_START  (KPD_PORT * 8 + 4)
#define COL_START  (KPD_PORT * 8)
#else
#define ROW_START  (KPD_PORT * 8)
#define COL_START  (KPD_PORT * 8 + 4)
#endif

static const uint8 KPD_Keys[NOROWS][NOCOLS] = {
{'7','8','9','/'},
{'4','5','6','*'},
{'1','2','3','-'},
{'C','0','=','+'}
};


void Init_vKPD(void){
	#if ROW_NIBBLE == HIGHNIBBLE
		DIO_vSet_HIGHNIBBLE_Dir(KPD_PORT,OUTPUT_NIBBLE);
		DIO_vSet_HIGHNIBBLE_Val(KPD_PORT,HIGH_NIBBLE);
		
		DIO_vSet_LOWNIBBLE_Dir(KPD_PORT,INPUT_NIBBLE);
		DIO_vSet_LOWNIBBLE_Val(KPD_PORT,HIGH_NIBBLE);
	#else 
		DIO_vSet_LOWNIBBLE_Dir(KPD_PORT,OUTPUT_NIBBLE);
		DIO_vSet_LOWNIBBLE_Val(KPD_PORT,HIGH_NIBBLE);
		
		DIO_vSet_HIGHNIBBLE_Dir(KPD_PORT,INPUT_NIBBLE);
		DIO_vSet_HIGHNIBBLE_Val(KPD_PORT,HIGH_NIBBLE);
	#endif
}
uint8 KPD_u8GetPressedKey(void){
	uint8 PressedKey = 0xFF;
	uint8 j ,i;
	for(i=0; i<NOROWS; i++){
		DIO_vSetPinVal(ROW_START + i ,LOW_PIN);
		for(j=0; j<NOCOLS; j++){
			if(DIO_u8GetPinVal(COL_START + j) == LOW_PIN){
				_delay_ms(20);    
				while(DIO_u8GetPinVal(COL_START + j) == LOW_PIN);   
				_delay_ms(20);                               
				PressedKey = KPD_Keys[i][j];
				DIO_vSetPinVal(ROW_START + i, HIGH_PIN);
				return PressedKey;
			}
		}
		DIO_vSetPinVal(ROW_START + i ,HIGH_PIN);
	}
	return PressedKey;
}