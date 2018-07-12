/*
 * Dosya    : main.c
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : SHT11 Sensor Uygulamasi
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8Mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açıklama : Sensirion SHT1x sensorlerini okuyarak LCD ekranda sicaklik ve nem
 *            bilgisi gosterir.
 * Uyarı    : Keils uVision v4.70 ile derlenmiştir.
 *            EX8 Kullanıcıları için;
 *            - SENSÖRÜ GELİŞTİRME KİTİNE TAKARKEN BESLEME VOLTAJININ DOĞRU
 *              OLDUĞUNDAN EMİN OLUNUZ. AKSİ HALDE SENSÖR ZARAR GÖREBİLİR.
 *			  - SWDIP1 üzerindeki tüm anahtarların kapalı olduğundan emin olun.
 * 			  - SENSÖR OKUMALARINDA NEM(RH) OLMASI GEREKEN DEĞERLERİ VERMEMESİ
 *				KOD İLE ALAKALI DEĞİLDİR. BU DURUM SENSÖRÜN KALİBRASYONU İLE
 *				ALAKALIDIR. KOD CRC KONTROLÜ YAPTIĞINDAN SENSÖRDEN OKUNAN VERİNİN
 *				HATALI OKUNMASI DİYE BİR DURUM YOKTUR.
 *				KALİBRASYON İÇİN SENSÖRÜN DÖKÜMANINDA YAZAN ADIMLAR ŞUNLARDIR;
 *				1.Adım : Sensörünüzü 50~60°C, Nem <10%RH olan bir ortamda 2 saat bekletiniz.
 *				2.Adım : Sensörünüzü 20~30°C, Nem >70%RH olan bir ortamda 5 saat bekletiniz.
 *				Bundan sonra sensör okumalarınız daha etkili olacaktır. 
 */
 
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "stdio.h"
#include "string.h"
#include "exs03.h"

float  isi,nem;
char isi_bilgisi[16];
char nem_bilgisi[16];
char lcdbuf[32];

/* Makro tanimlamalari  */
#ifndef DHT11
#define DHT11_IO  GPIOB
#define DHT11_PIN  GPIO_Pin_8
#endif

/* Global degiskenler   */
char dht_dat[10]; // Output data will be here
unsigned char GlobalErr = 0;

/* Fonksiyon prototipleri */
void dht_init(void);
unsigned char dht_poll(void);
void read_dht(void);

/* Fonksiyonlar         */
void dht_init()
{
    delay_ms(1); // 1ms
    GPIO_SetBits(DHT11_IO,DHT11_PIN);
}

unsigned char dht_poll()
{
    unsigned char i = 0;
    unsigned char result = 0;
    DHT11_IO->CRH &=~(0x0FUL);
    DHT11_IO->CRH |=0x04;
    for (i = 0; i < 8; i++)
    {
        while (GPIO_ReadInputDataBit(DHT11_IO,DHT11_PIN) == 0);
        delay_us(30); //30us
        if (GPIO_ReadInputDataBit(DHT11_IO,DHT11_PIN) == 1)
        {
            result |= (1 << (7 - i));
        }
        while (GPIO_ReadInputDataBit(DHT11_IO,DHT11_PIN) == 1);
    }
    DHT11_IO->CRH &=~(0x0FUL);
    DHT11_IO->CRH |=0x03;
    return result;
}

void read_dht()
{
    unsigned char dht_in;
    unsigned char i;
    GlobalErr = 0;
    dht_init();
    GPIO_ResetBits(DHT11_IO,DHT11_PIN);
    delay_ms(18); // 18ms
    GPIO_SetBits(DHT11_IO,DHT11_PIN);
    DHT11_IO->CRH &=~(0x0FUL);
    DHT11_IO->CRH |=0x04;
    delay_us(30); // 20-40us
    dht_in = DHT11_IO->IDR  & DHT11_PIN;
    if (dht_in)
    {
        GlobalErr = 1;
        return;
    }
    delay_us(80); // 80us
    dht_in = GPIO_ReadInputDataBit(DHT11_IO,DHT11_PIN);
    if (!dht_in)
    {
        GlobalErr = 2;
        return;
    }
    delay_us(80); // 80us
    for (i = 0; i < 5; i++)
    {
        dht_dat[i] = dht_poll();
    }
    DHT11_IO->CRH &=~(0x0FUL);
    DHT11_IO->CRH |=0x03;
    GPIO_SetBits(DHT11_IO,DHT11_PIN); //DHT11_PIN = 1;
}


/*************************Ana Program*************************/
int main(void)
{
    unsigned char dht11_crc = 0;
    EXS03_initGPIO();//Giris Cikilari Ayarla
    delay_init();  //Delay icin konfigurasyonlari yap
    GPIO_ResetBits(GPIOC , GPIO_Pin_8);	 // Buzzer Sussun
    lcd_init();
    lcd_yaz("     DHT11      ");
    lcd_gotoxy(2,1);
    lcd_yaz("www.ExpKits.com ");
    delay_ms(1500);
    // dht_init();
    lcd_clear();
    while(1)
    {
        memset(dht_dat, 0x0, sizeof (dht_dat));
        read_dht(); // Check dht_dat variable after this funciton
        dht11_crc = dht_dat[0] + dht_dat[1] + dht_dat[2] + dht_dat[3]; // check check_sum
        if (dht_dat[4] != dht11_crc || GlobalErr>0)
        {
            lcd_gotoxy(1, 1);
            sprintf(lcdbuf,"CRC ERR :%02x!%02X   ", dht_dat[4], dht11_crc);
            lcd_yaz(lcdbuf);
            lcd_gotoxy(2, 1);
            sprintf(lcdbuf,"Resp. Err:%d   ", GlobalErr);
            lcd_yaz(lcdbuf);
        }
        else
        {
            lcd_gotoxy(1, 1);
            sprintf(nem_bilgisi,"HUMI :%%%2.2fRH   ",(float) dht_dat[0]);
            lcd_yaz(nem_bilgisi);
            lcd_gotoxy(2, 1);
            sprintf(isi_bilgisi,"TEMP :%2.2f %cC  ",(float) dht_dat[2], 223);
            lcd_yaz(isi_bilgisi);
        }
        delay_ms(1000);// 1sn min. ornekleme
        delay_ms(1000);// 2sn ideal gecikme
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
