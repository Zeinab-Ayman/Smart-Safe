/*
 * Smart_Safe.c
 *
 * Created: 11/21/2026 12:49:19 AM
 *  Author: Zeinab
 */ 

#include "HAL/LCD/lcd.h"
#include "HAL/KPD/KPD.h"
#include "MCAL/EEPROM/EEP.h"
#include <util/delay.h>

#define PASS_LEN      4
#define MAX_TRIES     3
#define LOCK_SEC      30

#define FLAG_ADD   (uint8)0x00
#define PASS_SADD  (uint8)0x01
#define TRIES_ADD  (uint8)0x10
#define FLAG_VAL   (uint8)0xA5

#define CLR    (uint8)'C'
#define RESET  (uint8)'C'

static void ReadPass(uint8 *pass){
	uint8 i = 0, key;
	LCD_vMovecursor(2,6);
	while(i < PASS_LEN){
		key = KPD_u8GetPressedKey();
		if(key == 0xFF) continue;
		
		if(key == CLR){
			if(i > 0){
				i--;
				LCD_vMovecursor(2,6+i);
				LCD_vWriteChar(' ');
			}
		}
		else {
			pass[i] = key;
			LCD_vMovecursor(2,6+i);
			LCD_vWriteChar(key);
			_delay_ms(50);
			LCD_vMovecursor(2,6+i);
			LCD_vWriteChar('*');
			i++;
		}            
	}
}                                      

static void SavePass(const uint8 *pass){
	uint8 i;
	EEPROM_write(FLAG_ADD, 0xFF);
	for(i = 0; i < PASS_LEN; i++)
	EEPROM_write(PASS_SADD + i, pass[i]);
	EEPROM_write(FLAG_ADD, FLAG_VAL);
}

static void Lockout(void){
	sint8 s;
	LCD_vWriteCMD(CLEAR_DIS);
	LCD_vWriteString((const uint8 *)"Locked! Wait...");
	for(s = LOCK_SEC; s > 0; s--){
		LCD_vMovecursor(2,7);
		LCD_vWriteString((const uint8 *)"   ");
		LCD_vMovecursor(2,7);
		LCD_vWriteInt(s);
		_delay_ms(1000);
	}
	EEPROM_write(TRIES_ADD, 0);
}

static void SetNewPass(uint8 *pass){
	LCD_vWriteCMD(CLEAR_DIS);
	LCD_vWriteString((const uint8 *)"Set Password:");
	ReadPass(pass);
	SavePass(pass);
	EEPROM_write(TRIES_ADD, 0);
	LCD_vWriteCMD(CLEAR_DIS);
	LCD_vWriteString((const uint8 *)"Password Saved");
	_delay_ms(1000);
}

int main(void)
{
	Init_vLCD4bit();
	Init_vKPD();

	uint8 pass[PASS_LEN], saved[PASS_LEN];
	uint8 tries, flag, i, k;

	/* first time */
	if(EEPROM_read(FLAG_ADD) != FLAG_VAL){ 
		SetNewPass(pass);
	}
	
	/* power drops */
	tries = EEPROM_read(TRIES_ADD);
	if(tries == 0xFF) tries = 0; 
	if(tries >= MAX_TRIES){
		Lockout();
		tries = 0;
	}

	while(1)
	{
		LCD_vWriteCMD(CLEAR_DIS);
		LCD_vWriteString((const uint8 *)"Enter Password:");
		ReadPass(pass);
	
		flag = 1;
		for(i = 0; i < PASS_LEN; i++){
			saved[i] = EEPROM_read(PASS_SADD + i);
			if(pass[i] != saved[i]) flag = 0;   
		}
	
		LCD_vWriteCMD(CLEAR_DIS);
		if(flag)
		{
			if(tries != 0){                 
				tries = 0;
				EEPROM_write(TRIES_ADD, 0);
			}
			LCD_vWriteString((const uint8 *)"Safe Opened");
			LCD_vMovecursor(2,1);
			LCD_vWriteString((const uint8 *)"C:RESET Password");
	
			for(i = 0; i < 200; i++){          
				k = KPD_u8GetPressedKey();
				if(k == RESET){
					SetNewPass(pass);
					break;
				}
				_delay_ms(10);
			}
		}
		else{
			tries++;
			EEPROM_write(TRIES_ADD, tries);
			LCD_vWriteString((const uint8 *)"Wrong Password");
			_delay_ms(1000);
			
			if(tries >= MAX_TRIES){
				Lockout();
				tries = 0;
			}
		}
	}
}