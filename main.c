#include "stm8s.h"
#include "main.h"
#include "iic_embedded_tx.h"
#include "oled.h"
#include "bmp_180.h"

Calibration_bmp180 calib_data;
OledDigitBuffer oledBuffer;
T_P tp;

//graphic buffer
@near int graphicBuf[128];

main() { 
  del(10000);
	init_iic_emb_tx();
	init_ssd1306();
	init_bmp180();
	
  while (1){
		get_bmp180_T_P_average(&tp);
		printPressureValue();
		print_graphic();
}
}

void printPressureValue(void) {
		oled_print_XXXnumber((tp.P)/100, 0, 1);
		oled_print_giga_char('.',48);
		oled_print_XXnumber((tp.P)%100, 64, 1);
}