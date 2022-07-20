#include "stm8s.h"
#include "bmp_180.h"
#include "iic_embedded_tx.h"


void del(unsigned  int del)//1=6us
{ while(del!=0){del--;}
}

void init_bmp180(void)
{
extern Calibration_bmp180 calib_data;
char calibrationArr[2];
char i;
int* pointer_calib;
pointer_calib= (int*)&calib_data;

for(i=cAC1_m;i<=cMD_l;i+=2) 
  {
	  calibrationArr[0]=0;
	  calibrationArr[1]=0;
	  i2c_wr_reg(BMP_addr, i, &i, 0);
	  i2c_read(BMP_addr, &calibrationArr[0], 2);

    *pointer_calib = calibrationArr[0];
    *pointer_calib<<=8;
    *pointer_calib += calibrationArr[1];
		pointer_calib++;
  }
}

long int get_bmp180_ADC(char cmd, unsigned  int delay, char n)
 {
	char adcArr[30]; 
  long int adc;
  i2c_wr_reg(BMP_addr, 0xf4, &cmd, 1);
  del(delay);
 
  i2c_wr_reg(BMP_addr, 0xf6, &cmd, 0);
 
  adcArr[0]=0;
  adcArr[1]=0;
  adcArr[2]=0;
  i2c_read(BMP_addr, &adcArr[0], n);
  adc = adcArr[0];
  adc<<=8;
  adc+= adcArr[1];
  adc<<=8;
  adc+= adcArr[2];
	
  return adc; 
 } 
	 
void get_bmp180_T_P ( T_P* tp ) 
	{ 
 extern Calibration_bmp180 calib_data;
 long int x1,x2,x3,b3,b5,b6;
 unsigned long int b4,b7;
 long int UT=0,UP=0;
 UT=(get_bmp180_ADC(0x2e,2000,2))>>8;
 UP=(get_bmp180_ADC(0xf4,15000,3))>>5;

 x1=UT-calib_data.AC6;
 x1*=calib_data.AC5;
 x1/=32768;
 x2=(long int)calib_data.MC*2048;
 x2/=(x1+calib_data.MD);
 
 b5=x1+x2;
 tp->T=(b5+8)>>4;
 
 b6=b5-4000;
 x1=(calib_data.B2*(b6*b6/4096))/2048;
 x2=(calib_data.AC2*b6)/2048;
 x3=x1+x2;
 b3=((((long int)calib_data.AC1*4+x3)<<3)+2)/4;
 x1=(calib_data.AC3*b6)/8192;
 x2=(calib_data.B1*((b6*b6)/4096))/65536;
 x3=((x1+x2)+2)/4;
 b4=(calib_data.AC4*(long int)(x3+32768))/32768;
 b7=((long int)UP-b3)*(50000>>3);
 if(b7<0x80000000) tp->P=(b7*2)/b4;
 else tp->P=(b7/b4)*2;
 x1=((tp->P)/256)*((tp->P)/256);
 x1=(x1*3038)/65536;
 x2=((-7357)*(tp->P))/65536;
 tp->P=(tp->P)+(x1+x2+3791)/16;
 /////////////////////////////////
 tp->P*=75;
 tp->P/=100;
 }
 
void get_bmp180_T_P_average ( T_P* tp )
{
 char i;
 T_P t_p;
 long int sum_P=0,sum_T=0;
 for(i=0;i<40;i++)
  {
  get_bmp180_T_P(&t_p );
  sum_P+=t_p.P;
	sum_T+=t_p.T;
  }
  t_p.P=sum_P/40;
	t_p.T=sum_T/40;
	tp->P=t_p.P;
	tp->T=t_p.T;
 }

long int altitude (long int p,long int p0, int t)
{
	signed long int a,b,c,d;
	a=((p0-p)*(p0-p)/10*77)/10;
	b=(p0-p)*114820;
	c=1000+(t/10-25)*3;
	d=(((a+b)/1000)*c)/100000;
	return (d);
}
