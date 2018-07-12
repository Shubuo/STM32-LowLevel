/*****************************************************************************
 *   SHT11.h:  Kaynak Dosya 
 *
 *   www.Expkits.com
 *   
 *	 Yazan:
 *   Coskun ERGAN
 *   13-Mart-2011  
 *
******************************************************************************/
#ifndef __I2C_H 
#define __I2C_H

void start(void);
char sht11_oku(float *isi, float *nem); 
void reset_sht11(void);
char sht11_yaz_config(unsigned char status);

#endif 
/****************************************************************************
**                            End 
*****************************************************************************/
