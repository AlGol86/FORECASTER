#include "stm8s.h"
#include "main.h"
#include "iic_embedded_tx.h"
#include "oled.h"
#include "bmp_180.h"

Calibration_bmp180 calib_data;
OledDigitBuffer oledBuffer;
T_P tp;

//graphic buffer
char graphicBuf[128];

main() { 
  int cyclCntr = 0;
char i;

  del(10000);
	init_iic_emb_tx();
	init_ssd1306();
	init_bmp180();

  while (1){
		get_bmp180_T_P_average(&tp);
		printPressureValue();
		if(cyclCntr==100) {
			shiftGraphicBufferAndInsertNewValue((tp.P-73000)/30);
		  print_graphic();
			cyclCntr=0;
	  } 
		cyclCntr++;
}
}

void shiftGraphicBufferAndInsertNewValue(int val){
	char i;
	for(i=0;i<128;i++) {
		graphicBuf[i]=graphicBuf[i+1];
	}
	graphicBuf[127]=val;
}

void printPressureValue(void) {
		oled_print_XXXnumber((tp.P)/100, 0, 1);
		oled_print_giga_char('.',48);
		oled_print_XXnumber((tp.P)%100, 64, 1);
}