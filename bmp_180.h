

#define BMP_addr  0x77

//calibr tabl
#define cAC1_m 0xaa
#define cAC1_l 0xab
#define cAC2_m 0xac
#define cAC2_l 0xad
#define cAC3_m 0xae
#define cAC3_l 0xaf
#define cAC4_m 0xb0
#define cAC4_l 0xb1
#define cAC5_m 0xb2
#define cAC5_l 0xb3
#define cAC6_m 0xb4
#define cAC6_l 0xb5
#define cB1_m  0xb6
#define cB1_l  0xb7
#define cB2_m  0xb8
#define cB2_l  0xb9
#define cMB_m  0xba
#define cMB_l  0xbb
#define cMC_m  0xbc
#define cMC_l  0xbd
#define cMD_m  0xbe
#define cMD_l  0xbf

typedef struct {
  int AC1; //0xaa
  int AC2; //0xac
  int AC3; //0xae
  unsigned int AC4; //0xb0
  unsigned int AC5; //0xb2
  unsigned int AC6; //0xb4
  int B1;  //0xb6
  int B2;  //0xb8
  int MB;  //0xba
  int MC;  //0xbc
  int MD; //0xbe
 } Calibration_bmp180;
 
 typedef struct {
	 int T;
	 long int P;
 } T_P;

void del(unsigned int del);
void init_bmp180(void);
long int get_bmp180_ADC(char cmd, unsigned  int delay, char n);
void get_bmp180_T_P ( T_P* tp );
void get_bmp180_T_P_average ( T_P* tp );
long int  altitude (long int p,long int p0, int t);
