#include  "../header/halGPIO.h"     // private library - HAL layer
#include <stdlib.h>

int x=500;
unsigned int voltage_int=0;
unsigned int voltage_int2=0;
char voltage_str[4];

char input_str[6];
int input_index = 0;
int command_flag = 1; 
const char st5_str[] = "I love my Negev \n";
unsigned int TXindex = 0;
//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	PWMconfig();
	ADCconfig();
	lcd_init();
	UARTconfig();
}
//--------------------------------------------------------------------
//				Read value of 4-bit SWs array 
//--------------------------------------------------------------------
/* unsigned char readSWs(void){
	unsigned char ch;
	
	ch = SWsArrPort;
	ch &= SWmask;     // mask the least 1-bit
	return ch;
} */
//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;
	
	for(i=t; i>0; i--);
}
//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03) 
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}
//---------------------------state1--------------------------------
void state1TIMERconfig(void){
	TA0CTL = TASSEL_2+ID_3+MC_0;            //Smclk+Div8+disableTA
}
void DelayOfXms(){
	int temp = x;
  	while(temp > 500){
		TA0CCR0 = 0xFFFF;      // value of 0.5 sec
		TA0R=0x0000;
    	startTA0_st1();
		LPM0;
		haltTA0_st1();
    	temp -= 500;
 		}
  	if (temp != 0){
      	temp = temp<<7;    //multiply temp by 1024 
      	TA0CCR0 = temp;
		TA0R=0x0000;
    	startTA0_st1();
		LPM0;
		haltTA0_st1();
  		}
}
void startTA0_st1(void){
	TA0CTL |= MC_1;    //Up Mode
	TA0CCTL0 |= CCIE;
}
void haltTA0_st1(void){
	TA0CTL &= ~MC_1; 
	TA0CCTL0 &= ~CCIE;
}
//---------------------------state2--------------------------------
void state2TIMERconfig(){
	TA1CTL = TASSEL_2+MC_1;            //Smclk+UpMode
	TA1CCTL2 = OUTMOD_6;               //Toggle/Set
	TA1CCR0 = 1056;					   //Period of 1kHz PWM
	TA1CCR2 = 1056>>1;					   //DC of 50% PWM

	TA0CTL = TASSEL_2+ID_3+MC_0;            //Smclk+Div8+disableTA
}
void SetFreq_st2(int Freq){
	if (Freq==1000){
		TA1CCR0 = 1056;					   //Period of 1kHz PWM
		TA1CCR2 = 1056>>1;					   //DC of 50% PWM
		}
	if (Freq==1250){
		TA1CCR0 = 1320;					   
		TA1CCR2 = 1320>>1;					  
		}
	if (Freq==1500){
		TA1CCR0 = 1584;					  
		TA1CCR2 = 1584>>1;					   
		}	
	if (Freq==1750){
		TA1CCR0 = 1848;					   
		TA1CCR2 = 1848>>1;					   
		}
	if (Freq==2000){
		TA1CCR0 = 2112;					 
		TA1CCR2 = 2112>>1;					   
		}
	if (Freq==2250){
		TA1CCR0 = 2376;					  
		TA1CCR2 = 2376>>1;					   
		}
	if (Freq==2500){
		TA1CCR0 = 2640;					  
		TA1CCR2 = 2640>>1;					  
		}					
}
void haltTA1_st2(void){
	TA1CTL &= ~(MC_1+TAIE);		//halt clock+disable TA0IE
}
//---------------------------state3--------------------------------


//---------------------------state4--------------------------------
void state4ADCconfig(){
	ADC10CTL0 = ADC10SHT_2+ADC10ON+ADC10IE;            //16xADC10CLKS+ADC10ON+ADC10IE
	ADC10CTL1 = INCH_3+ADC10SSEL_0+CONSEQ_0;           //ChannelA3+ADC10OSCreference+SingleChannelSingleConversion
	ADC10AE0 =  BIT3;            //enable A3 Channel
}
void sampleADC(){
	ADC10CTL0 |= ADC10SC+ENC;
}
int readADC10MEM(){
	return ADC10MEM;
}
int PrintVoltageToLCD(int Nadc){
	voltage_int = (Nadc*45);        // (358/1023)=> (Q9.7)=> 45
	voltage_int = voltage_int / 128;	  
	voltage_str[0] = '0' + (voltage_int / 100);             
	voltage_str[1] = '0' + (voltage_int / 10 % 10);         
	voltage_str[2] = '0' + (voltage_int % 10);  
	DelayMs(500);            
	lcd_clear();
	lcd_home();
	lcd_data(voltage_str[0]);
	lcd_data('.');
	lcd_data(voltage_str[1]);
	lcd_data(voltage_str[2]);
}
void haltADC(){
	ADC10CTL0 &= ~(ADC10ON+ENC+ADC10IE);  
}
//---------------------------state5--------------------------------
void enablePB1_IE(){
	P1IE |= PB0;
}
void disablePB1_IE(){
	P1IE &= ~PB0;
}
//******************************************************************
// initialize the LCD
//******************************************************************
void lcd_init(){
  
	char init_value;

	if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
        else init_value = 0x3F;
	
	LCD_RS_DIR(OUTPUT_PIN);
	LCD_EN_DIR(OUTPUT_PIN);
	LCD_RW_DIR(OUTPUT_PIN);
        LCD_DATA_DIR |= OUTPUT_DATA;
        LCD_RS(0);
	LCD_EN(0);
	LCD_RW(0);
        
	DelayMs(15);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayMs(5);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayUs(200);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	
	if (LCD_MODE == FOURBIT_MODE){
		LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET; // Set 4-bit mode
		lcd_strobe();
		lcd_cmd(0x28); // Function Set
	}
        else lcd_cmd(0x3C); // 8bit,two lines,5x10 dots 
	
	lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
	lcd_cmd(0x1); //Display Clear
	lcd_cmd(0x6); //Entry Mode
	lcd_cmd(0x80); //Initialize DDRAM address to zero
}
//******************************************************************
// send a command to the LCD
//******************************************************************
void lcd_cmd(unsigned char c){
  
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	if (LCD_MODE == FOURBIT_MODE)
	{
		LCD_DATA_WRITE &= ~OUTPUT_DATA;// clear bits before new write
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
		lcd_strobe();
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
    		LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
}
//******************************************************************
// send data to the LCD
//******************************************************************
void lcd_data(unsigned char c){
        
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	LCD_DATA_WRITE &= ~OUTPUT_DATA;       
	LCD_RS(1);
	if (LCD_MODE == FOURBIT_MODE)
	{
    		LCD_DATA_WRITE &= ~OUTPUT_DATA;
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;  
		lcd_strobe();		
                LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET; 
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
          
	LCD_RS(0);   
}
//******************************************************************
// write a string of chars to the LCD
//******************************************************************
void lcd_puts(const char * s){
  
	while(*s)
		lcd_data(*s++);
}
//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){
  
	unsigned char i;
        for(i=cnt ; i>0 ; i--) asm(" nop"); // tha command asm("nop") takes raphly 1usec
	
}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(unsigned int cnt){
  
	unsigned char i;
        for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec
	
}
//******************************************************************
// lcd strobe functions
//******************************************************************
void lcd_strobe(){
  LCD_EN(1);
  asm(" nop");
  asm(" nop");
  LCD_EN(0);
}
//*********************************************************************
//            Port1 Interrupt Service Routine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){

    delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
    if(PBsArrIntPend & PB0){
      PBsArrIntPend &= ~PB0;
      TXindex = 0;
      IE2 |= UCA0TXIE;                        // Enable USCI_A0 TX interrupt
      UCA0TXBUF = st5_str[TXindex];  // Start transmission
    }
}
//*********************************************************************
//            TA1 Interrupt Service Rotine
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) TIMER1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TA1IV,0x0A))
  {
      case  TA1IV_NONE: break;              // Vector  0:  No interrupt
      case  TA1IV_TACCR1: break;            // Vector  2:  TACCR1 CCIFG
      case  TA1IV_TACCR2: break;            // Vector  4:  TACCR2 CCIFG
      case  TA1IV_TAIFG: break;             // Vector 10:  TAIFG
      default: 	break;
  }
} 
//*********************************************************************
//            TimerA0 Interrupt Service Rotine
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  LPM0_EXIT;
}
//---------------------------------------------------------------------
//            USCI A0/B0 Transmit ISR
//---------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
	if (st5_str[TXindex+1] == '\n'){           // TX over?
	UCA0TXBUF = st5_str[TXindex+1];
    IE2 &= ~UCA0TXIE;                       // Disable USCI_A0 TX interrupt
  }
  else{
	TXindex++;
	UCA0TXBUF = st5_str[TXindex];  
}
}
//---------------------------------------------------------------------
//            USCI A0/B0 Receive ISR
//---------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  input_str[input_index] = UCA0RXBUF;
  input_index++;
  if (input_str[input_index-1] == '\n')                     
  {
      if(command_flag){                     //command_flag='1' (command)      command_flag='0' (data)

          switch(input_str[0]){
              case '1' :
                  state = state1;
                  break;
              case '2' :
                  state = state2;
                  break;
              case '3' :
                  state = state3;
                  break;
              case '4' :
                  state = state4;
                  break;
              case '5' :
                  state = state5;
                  break;
              case '6' :
                  state = state6;
                  break;
              case '7' :
                  state = state7;
                  break;
              case '8' :
                  state = state8;
                  break;
              default:
                  break;
          }
      }
      else{                                   
          x = atoi(input_str);          //convert input_str to int
          command_flag = 1;
      	}
    input_index = 0;
    //IE2 |= UCA0TXIE;                        // Enable USCI_A0 TX interrupt
//---------------------------------------------------------------------
//            Exit from a given LPM
//---------------------------------------------------------------------
      switch(lpm_mode){
          case mode0:
           LPM0_EXIT; // must be called from ISR only
           break;

          case mode1:
           LPM1_EXIT; // must be called from ISR only
           break;

          case mode2:
           LPM2_EXIT; // must be called from ISR only
           break;

          case mode3:
           LPM3_EXIT; // must be called from ISR only
           break;

          case mode4:
           LPM4_EXIT; // must be called from ISR only
           break;
      }
  }
}
//*********************************************************************
//            ADC10 Interrupt Service Rotine
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
  LPM0_EXIT;
}
