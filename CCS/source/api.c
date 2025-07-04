#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"


int st1_counter_int=0;
char st1_counter_str[17];
int Nadc=0;
//-------------------------------------------------------------
//            state1 function
//-------------------------------------------------------------
void st1(){
	lcd_clear();     //clear LCD
	lcd_home();      //bring cursor to start of first line
	state1TIMERconfig();
	while (state == state1){ 
		sprintf(st1_counter_str, "%d", st1_counter_int);  
		lcd_puts(st1_counter_str);		
		DelayOfXms();
		st1_counter_int++;
		lcd_clear();
		lcd_home();
		}
	}
//--------------------------------------------------------------------
//            state2 function
//--------------------------------------------------------------------            
void st2(){
	state2TIMERconfig();
	while(state==state2){
		SetFreq_st2(1000);
		DelayOfXms();
		SetFreq_st2(1250);
		DelayOfXms();	
		SetFreq_st2(1500);
		DelayOfXms();
		SetFreq_st2(1750);
		DelayOfXms();
		SetFreq_st2(2000);
		DelayOfXms();
		SetFreq_st2(2250);
		DelayOfXms();
		SetFreq_st2(2500);
		DelayOfXms();		
	}
	haltTA1_st2();
}
//--------------------------------------------------------------------
//            state3 function
//--------------------------------------------------------------------            
void st3(){
	command_flag=0;       
	LPM0;         // wait for reading data(X value)
	LPM0;         // X value accepted, wait for next state
}
//--------------------------------------------------------------------
//            state4 function
//--------------------------------------------------------------------            
 void st4(){
	state4ADCconfig();
	while(state==state4){
		sampleADC();            //start single sample
		LPM0;                   //enter LPM0 mode
		Nadc = readADC10MEM();
		PrintVoltageToLCD(Nadc);	
	}
	haltADC();
}
//--------------------------------------------------------------------
//            state5 function
//--------------------------------------------------------------------      
void st5(){
	enablePB1_IE();
	LPM0; //wait for next state (push PB1 to show string)
	disablePB1_IE();
}
//--------------------------------------------------------------------
//            state6 function
//--------------------------------------------------------------------      
void st6(){
	lcd_clear();
	st1_counter_int=0;
	LPM0;
}
//--------------------------------------------------------------------
//            state7 function
//--------------------------------------------------------------------      
void st7(){
	LPM0;         //print MENU on PC side   
}










 