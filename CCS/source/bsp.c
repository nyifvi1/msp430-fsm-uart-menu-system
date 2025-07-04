#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  // LEDs 8-bit Array Port configuration
  //LEDsArrPortSel &= ~0xFF;            // GPIO capability
  //LEDsArrPortDir |= 0xFF;             // output dir
  //LEDsArrPort = 0x00;				  // clear all LEDs
  
   // LCD Setup
  LCD_DATA_WRITE &= 0x0F;   //bic Px.7-Px.4
  LCD_DATA_DIR |= 0xF0;     //bis Px.7-Px.4
  LCD_DATA_SEL &= 0x0F;
  LCD_CNTL_SEL &= ~0xE0;    //bic Px.7-Px.5
  
  // Switches Setup
  //SWsArrPortSel &= ~SW0;  //bic PxSel SW0      
  //SWsArrPortDir &= ~SW0;  //bic PxDir SW0 
  
//   // PushButtons Setup          PB0-PB2 configuration
//   PBsArrPortSel &= 0xF8; 
//   PBsArrPortDir &= 0xF8;
//   PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
//   PBsArrIntEdgeSel &= ~0x04;         // pull-down mode
//   PBsArrIntEn |= 0x07;
//   PBsArrIntPend &= 0xF8;            // clear pending interrupts 
  

 //PB1 Setup          
  P1SEL &= ~PB0;
  P1DIR &= ~PB0;
  P1IES |= PB0;                // pull-up mode
  P1IE &= ~PB0;
  P1IFG &= ~PB0;
  

  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void PWMconfig(void){
        P2DIR |= 0x10;   //bis BIT4  P2.4 output comapre (PWM) Timer1_TACCR2
        P2SEL |= 0x10;   //bis BIT4
        P2SEL2 &= ~0x10; //bic BIT4
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
	ADC10AE0 |= BIT3;   //enable A3 as analog input
        ADC10CTL1 = INCH3;  // INCH3 - select A3 as input channel
        CAPD &= ~CAPD3;     //disable comparator port
} 

void UARTconfig(void){
  if (CALBC1_1MHZ==0xFF){					  // If calibration constant erased											
    while(1);                       // do not load, trap CPU!!	
  }
  DCOCTL = 0;                       // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;            // Set DCO
  DCOCTL = CALDCO_1MHZ;
  UARTArrPortSel |= BIT1 + BIT2;    // Select peripheral function for UART pins
  UARTArrPortSel2 |= BIT1 + BIT2;   // Select secondary peripheral function for UART pins
  UARTArrPortDir |= BIT1 + BIT2;    // Set UART pins as outputs
  UARTArrPortOUT &= ~(BIT1 + BIT2); // Clear output bits 
  UCA0CTL1 |= UCSSEL_2;             // SMCLK as clock source
  UCA0BR0 = 104;                    // Set baud rate to 9600 (assuming 1 MHz clock)
  UCA0MCTL = UCBRS0;                // Set modulation  
  UCA0BR1 = 0x00;                   // baud rate 9600 
  UCA0CTL1 &= ~UCSWRST;             // Initialize USCI 
  IE2 |= UCA0RXIE;                  // Enable USCI_A0 RX interrupt
}




           
             

 
             
             
            
  

