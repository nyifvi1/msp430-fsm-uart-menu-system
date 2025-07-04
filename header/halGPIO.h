#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"    		// private library - BSP layer
#include  "../header/app.h"    		// private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern int x;
extern int command_flag;

extern void sysConfig(void);
extern unsigned char readSWs(void);
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void state1TIMERconfig(void);
extern void DelayOfXms();
extern void state2TIMERconfig();
extern void SetFreq_st2();
extern void haltTA1_st2(void);
extern void state4ADCconfig();
extern void sampleADC();
extern int readADC10MEM();
extern int PrintVoltageToLCD(int Nadc);
extern void haltADC();
extern void enablePB1_IE();
extern void disablePB1_IE();


extern void lcd_init(void);
extern void lcd_cmd(unsigned char);
extern void lcd_data(unsigned char);
extern void lcd_puts(const char * s);
extern void lcd_strobe();
extern void DelayMs(unsigned int);
extern void DelayUs(unsigned int);
extern __interrupt void PBs_handler(void);

#endif







