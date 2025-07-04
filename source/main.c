#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;


void main(void){
  
  state = state8;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  
  while(1){
	switch(state){
	  case state8:
        enterLPM(lpm_mode);
		break;
		 
	  case state1:
		st1();
		break;
		 
	  case state2:
		st2();
		break;

	  case state3:
		st3();
	  	break;

	  case state4:
      	st4();
		break;

	  case state5:
      	st5();
		break;

	 case state6:
      	st6();
		break;

	 case state7:
      	st7();
		break;		
	}
  }
}
  
  
  
  
  
  