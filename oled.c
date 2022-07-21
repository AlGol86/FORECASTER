#include "stm8s.h"
#include "iic_embedded_tx.h"
#include "oled.h"
#include "main.h"


void sendCmd(char cmd) {
	i2c_wr_reg(oled_32_128_adr, 0x80, &cmd, 1);
}

void sendData(char* data, char count) {
	i2c_wr_reg(oled_32_128_adr, 0x40, data, count);
}

void init_ssd1306() {  
   char i;                                                                     //conrtast=7f//
   char initCmds[] = {0xAE,0x20,0x00,0xC0,0xA0,  0xB0,0x00,0x10,0x40,0x81,     0x7F,                0xA6,0xA8,0x3F,0xA4,
                      0xD3,0x00,0xD5,0XF0,0xD9,  0x22,0xDA,0x12,0xDB,0x20,  0x8D, 0x14, 0xAF};
																												// 0x02 for 128*32
   for(i=0;i<28;i++) sendCmd(initCmds[i]);
   oled_Clear_Screen();
}

void set_brightness_ssd1306(char brightness) {
  if (brightness==0) {
    sendCmd(0xAE);
		return;
	} 
	
	if (brightness==1) {
	  sendCmd(0xAF);
		return;
	} 
	
	sendCmd(0x81);
	sendCmd(brightness >> 1);
}

void blankScreen(char n) {
  send_repeat_data(0x00,n);
}

void oled_Clear_Screen(void)
{ 
  extern OledDigitBuffer oledBuffer;
	char i;
  oled_print_giga_space(0, 128);
  for(i = 0; i < DIG_BUF_SIZE; i++) {oledBuffer.position[i] = EMPTY;}
}

void ff_string(char n) {
  send_repeat_data(0xff,n);
}

void send_repeat_data(char data_byte, char n) { 
	i2c_write_repeated(oled_32_128_adr, 0x40, data_byte, n);
}

void set_cursor(char x, char y) {   
  sendCmd(0x0f&x); 
  sendCmd(0x10|(0x0f&(x>>4)));
  sendCmd(0xb0|y); 
}

void oled_print_giga_digit(char d, char x, char transparent) {
	if(transparent) oled_print_giga_transparent_digit(d, x);
	else          oled_print_giga_solid_digit(d, x);
}

void oled_print_giga_solid_digit(char d, char x) {
  const char c=4;
	char i=0;
  const char const_dig[10][9]={{0xff,0x0f,0xff, 0xff,0x00,0xff, 0x0f,0x0f,0x0f},  //0
                               {0x00,0x00,0xff, 0x00,0x00,0xff, 0x00,0x00,0x0f},  //1
                               {0x0f,0x0f,0xff, 0xff,0x0f,0x0f, 0x0f,0x0f,0x0f},  //2
                               {0x0f,0x0f,0xff, 0x0f,0x0f,0xff, 0x0f,0x0f,0x0f},  //3
                               {0xff,0x00,0xff, 0x0f,0x0f,0xff, 0x00,0x00,0x0f},  //4
                               {0xff,0x0f,0x0f, 0x0f,0x0f,0xff, 0x0f,0x0f,0x0f},  //5
                               {0xff,0x0f,0x0f, 0xff,0x0f,0xff, 0x0f,0x0f,0x0f},  //6
                               {0x0f,0x0f,0xff, 0x00,0x00,0xff, 0x00,0x00,0x0f},  //7
                               {0xff,0x0f,0xff, 0xff,0x0f,0xff, 0x0f,0x0f,0x0f},  //8
                               {0xff,0x0f,0xff, 0x0f,0x0f,0xff, 0x0f,0x0f,0x0f}}; //9
  char dig[9];
  for (i=0; i<9; i++) dig[i]=const_dig[d][i]; 
  
  set_cursor(x,0);
  for (i=0 ;i<3; i++) send_repeat_data(dig[i],c);
  set_cursor(x,1);
  for (i=3; i<6; i++) send_repeat_data(dig[i],c);
  set_cursor(x,2);
  for (i=6; i<9; i++) send_repeat_data(dig[i],c);
}
	
void oled_print_giga_transparent_digit(char d, char x) {
	char i=0;
  const char const_dig[10][36]={{0xff,0x01,0x01,0xf9,  0x09,0x09,0x09,0x09,  0xf9,0x01,0x01,0xff,  
	                               0xff,0x00,0x00,0xff,  0x00,0x00,0x00,0x00,  0xff,0x00,0x00,0xff,
																 0x0f,0x08,0x08,0x09,  0x09,0x09,0x09,0x09,  0x09,0x08,0x08,0x0f},  //0
																 
                                {0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0xff,0x01,0x01,0xff,  
	                               0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0xff,0x00,0x00,0xff,
																 0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x0f,0x08,0x08,0x0f},  //1
																 
                                {0x0f,0x09,0x09,0x09,  0x09,0x09,0x09,0x09,  0xf9,0x01,0x01,0xff,  
	                               0xff,0x01,0x01,0xf9,  0x09,0x09,0x09,0x09,  0x09,0x08,0x08,0x0f,
																 0x0f,0x08,0x08,0x09,  0x09,0x09,0x09,0x09,  0x09,0x09,0x09,0x0f},  //2																 
																 
                                {0x0f,0x09,0x09,0x09,  0x09,0x09,0x09,0x09,  0xf9,0x01,0x01,0xff, 
	                               0x0f,0x09,0x09,0x09,  0x09,0x09,0x09,0x09,  0xf9,0x00,0x00,0xff,
																 0x0f,0x09,0x09,0x09,  0x09,0x09,0x09,0x09,  0x09,0x08,0x08,0x0f},  //3																 
																 
                                {0xff,0x01,0x01,0xff,  0x00,0x00,0x00,0x00,  0xff,0x01,0x01,0xff,  
	                               0x0f,0x08,0x08,0x09,  0x09,0x09,0x09,0x09,  0xf9,0x00,0x00,0xff,
																 0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x0f,0x08,0x08,0x0f},  //4																 
																 
																{0xff,0x01,0x01,0xf9,  0x09,0x09,0x09,0x09,  0x09,0x09,0x09,0x0f,  
	                               0x0f,0x08,0x08,0x09,  0x09,0x09,0x09,0x09,  0xf9,0x01,0x01,0xff,
																 0x0f,0x09,0x09,0x09,  0x09,0x09,0x09,0x09,  0x09,0x08,0x08,0x0f},  //5		

                                {0xff,0x01,0x01,0xf9,  0x09,0x09,0x09,0x09,  0x09,0x09,0x09,0x0f,  
	                               0xff,0x00,0x00,0xf9,  0x09,0x09,0x09,0x09,  0xf9,0x01,0x01,0xff,
																 0x0f,0x08,0x08,0x09,  0x09,0x09,0x09,0x09,  0x09,0x08,0x08,0x0f},  //6
																 
                                {0x0f,0x09,0x09,0x09,  0x09,0x09,0x09,0x09,  0xf9,0x01,0x01,0xff,  
	                               0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0xff,0x00,0x00,0xff,
																 0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,  0x0f,0x08,0x08,0x0f},  //7																 
																 
                                {0xff,0x01,0x01,0xf9,  0x09,0x09,0x09,0x09,  0xf9,0x01,0x01,0xff,  
	                               0xff,0x00,0x00,0xf9,  0x09,0x09,0x09,0x09,  0xf9,0x00,0x00,0xff,
																 0x0f,0x08,0x08,0x09,  0x09,0x09,0x09,0x09,  0x09,0x08,0x08,0x0f},  //8																 
																 
                                {0xff,0x01,0x01,0xf9,  0x09,0x09,0x09,0x09,  0xf9,0x01,0x01,0xff,  
	                               0x0f,0x08,0x08,0x09,  0x09,0x09,0x09,0x09,  0xf9,0x00,0x00,0xff,
																 0x0f,0x09,0x09,0x09,  0x09,0x09,0x09,0x09,  0x09,0x08,0x08,0x0f}}; //9				
																 
  set_cursor(x,0);
  i2c_wr_reg(oled_32_128_adr, 0x40, &const_dig[d][0],  12);
  set_cursor(x,1);
  i2c_wr_reg(oled_32_128_adr, 0x40, &const_dig[d][12], 12);
  set_cursor(x,2);
  i2c_wr_reg(oled_32_128_adr, 0x40, &const_dig[d][24], 12);
}

void oled_print_giga_space(char pos, char n) {
  set_cursor(pos,0 );send_repeat_data(0,n);
  set_cursor(pos,1 );send_repeat_data(0,n);
  set_cursor(pos,2 );send_repeat_data(0,n);
  set_cursor(pos,3 );send_repeat_data(0,n);
	set_cursor(pos,4 );send_repeat_data(0,n);
  set_cursor(pos,5 );send_repeat_data(0,n);
  set_cursor(pos,6 );send_repeat_data(0,n);
  set_cursor(pos,7 );send_repeat_data(0,n);
}
char oled_print_giga_char(char c,char x)
{
  const char cc=3;
	char i;
  const char const_ch[6][12]={{0x00,0x38,0x38,0x00, 0x00,0xc0,0xc0,0x00, 0x00,0x01,0x01,0x00},  //0   ':'
                              {0x00,0x00,0x00,0x00, 0x06,0x06,0x06,0x06, 0x00,0x00,0x00,0x00},  //1   '-'
                              {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x0e,0x00,0x00},  //2   '.'
															{0xfc,0x03,0x03,0xfc, 0xff,0xf0,0xf0,0xff, 0x0f,0x00,0x00,0x0f},  //3   'A'
															{0xff,0x00,0x00,0xff, 0xff,0x00,0x00,0xff, 0x00,0x0f,0x0f,0x00},  //4   'V'
															{0xff,0x0f,0x0f,0x0f, 0xff,0x0f,0x0f,0x0f, 0x0f,0x0f,0x0f,0x0f}   //4   'E'
                              }; 
                              
  char pos;
  char ch[12];
	
  switch(c)
  {
  case ':':{c=0; pos=16; break;}
  case '-':{c=1; pos=16; break;}
  case '.':{c=2; pos=12; break;}
	case 'A':{c=3; pos=16; break;}
	case 'V':{c=4; pos=16; break;}
	case 'E':{c=5; pos=16; break;}
  }
  
  
  for (i=0;i<12;i++) ch[i]=const_ch[c][i]; 
  
  set_cursor(x,0);
	set_cursor(x,0);
  for (i=0;i<4;i++) send_repeat_data(ch[i],cc);
  set_cursor(x,1);
  for (i=4;i<8;i++) send_repeat_data(ch[i],cc);
  set_cursor(x,2);
  for (i=8;i<12;i++) send_repeat_data(ch[i],cc);
  return (pos+x);
  }

void oled_print_XXnumber(int n, char pos, char transparent) {
  extern OledDigitBuffer oledBuffer;
	char i;
  char space=0;
	char bufIndex = 0;
	
	
  for (i=0; i<DIG_BUF_SIZE; i++) {if (n == oledBuffer.value[i] && pos == oledBuffer.position[i] && transparent == oledBuffer.transperent_view[i]) return;}
	
	//following space if sign has dissapeared
  for (i=0; i<DIG_BUF_SIZE; i++) {if (n >= 0 && oledBuffer.value[i]<0 && pos == oledBuffer.position[i]) space=1;}  
  
  while(oledBuffer.position[bufIndex] != 255  && oledBuffer.position[bufIndex] != pos)
  {bufIndex++;}
  oledBuffer.position[bufIndex] = pos;
  oledBuffer.value[bufIndex] = n;
  oledBuffer.transperent_view[bufIndex] = transparent;
	 
  if (n<0) {oled_print_giga_char('-',pos);pos+=16;n=-n;} 
  if (n >= 10) {oled_print_giga_digit(n/10, pos, transparent);}
  else {oled_print_giga_digit(0, pos, transparent);}
  pos += 16;
  oled_print_giga_digit(n%10, pos, transparent);
  if(space){pos += 16; oled_print_giga_space(pos, 16);}
}
void oled_print_XXXnumber(int n, char pos, char transparent) {
  extern OledDigitBuffer oledBuffer;
	char i;
  char space=0;
	char bufIndex = 0;
	
	
  for (i=0; i<DIG_BUF_SIZE; i++) {if (n == oledBuffer.value[i] && pos == oledBuffer.position[i] && transparent == oledBuffer.transperent_view[i]) return;}
	
	//following space if sign has dissapeared
  for (i=0; i<DIG_BUF_SIZE; i++) {if (n >= 0 && oledBuffer.value[i]<0 && pos == oledBuffer.position[i]) space=1;}  
  
  while(oledBuffer.position[bufIndex] != 255  && oledBuffer.position[bufIndex] != pos)
  {bufIndex++;}
  oledBuffer.position[bufIndex] = pos;
  oledBuffer.value[bufIndex] = n;
  oledBuffer.transperent_view[bufIndex] = transparent;
	 
  if (n<0) {oled_print_giga_char('-',pos);pos+=16;n=-n;} 
  if (n >= 100) {oled_print_giga_digit(n/100, pos, transparent);pos += 16;}
  if (n >= 10)  {oled_print_giga_digit((n/10)%10, pos, transparent);pos += 16;}
  oled_print_giga_digit(n%10, pos, transparent);
  if(space){pos += 16; oled_print_giga_space(pos, 16);}
}

void print_save(void) { 
  char i;
  oled_Clear_Screen();   
  oled_print_giga_digit(5,0,0);
	oled_print_giga_char('A',16);
	oled_print_giga_char('V',32);
	oled_print_giga_char('E',48);	
  
}

void print_graphic(void) { 
  extern char graphicBuf[128];
	char i,y;
  char shift;
	shift = calculateZeroShift();
	
  set_cursor(0, 3);
	send_repeat_data(0, 128);
	set_cursor(0, 4);
	send_repeat_data(0, 128);
	set_cursor(0, 5);
	send_repeat_data(0, 128);
	set_cursor(0, 6);
	send_repeat_data(0, 128);
	set_cursor(0, 7);
	send_repeat_data(0, 128);
	
	for(i=0;i<128;i++) {
		y=graphicBuf[i]-shift;
		if (y<8){
			set_cursor(i, 7);
			send_repeat_data(0x80 >> y, 1);
		} else if (y<16){
			set_cursor(i, 6);
			send_repeat_data(0x80 >> (y-8), 1);
    } else if (y<24){
			set_cursor(i, 5);
			send_repeat_data(0x80 >> (y-16), 1);
		} else if (y<32){	
		  set_cursor(i, 4);
			send_repeat_data(0x80 >> (y-24), 1);
	  } else if (y<40){
			set_cursor(i, 3);
			send_repeat_data(0x80 >> (y-32), 1);
		} 
	}
}

char calculateZeroShift(void) {
	extern char graphicBuf[128];
	char max;
	char min;
	char i;
	
	max = graphicBuf[0];
	min = graphicBuf[0];
	
	for (i=0; i<128; i++) {
		if (graphicBuf[i]>max) max=graphicBuf[i];
		else if ((graphicBuf[i]!=0) && (graphicBuf[i]<min)) min=graphicBuf[i];
	}
	
	if ( ((max-min) >= 40) && ((graphicBuf[127]-min) > (max-graphicBuf[127])) ) return (max-39);
	return min;
}
/*

    _/
  _/
_/

      set_cursor(0, 7);
			send_repeat_data(0x80, 1);
			set_cursor(5, 7);
			send_repeat_data(1, 1);
			
			set_cursor(10, 6);
			send_repeat_data(0x80, 1);
			set_cursor(15, 6);
			send_repeat_data(1, 1);
			
			set_cursor(20, 5);
			send_repeat_data(0x80, 1);
			set_cursor(25, 5);
			send_repeat_data(1, 1);
			
			set_cursor(30, 4);
			send_repeat_data(0x80, 1);
			set_cursor(35, 4);
			send_repeat_data(1, 1);
			
			set_cursor(40, 3);
			send_repeat_data(0x80, 1);
			set_cursor(45, 3);
			send_repeat_data(1, 1);
*/
