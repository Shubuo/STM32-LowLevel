/*
 * Dosya    : main.c
 * Yazar    : Expkits - www.expkits.com
 * Uygulama : SD Kart Uygulaması
 * Sürüm    : v1.0
 * Tarih    : 05.04.2013
 * MCU      : STM32F10x
 * Kristal  : 8Mhz (MCU FOSC 72Mhz)
 * Kit      : EXS03
 * Açıklama : Kit üzerindeki SD Kart soketine takılan microSD kart
 *						kapasite bilgisini LCD' de gösterir.
 *						SD kart içeriğinin listesini UART üzerinden PC' ye gönderir.
 * Uyarı    : Keil uVision v4.70 ile derlenmiştir.
 *					  EXS03 Kullanıcıların için;
 *					  -SWDIP1 SW1-2-3 hariç hepsi ON konumunda olmalıdır.
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "ff.h"
#include "SPI_MSD_Driver.h"
#include "exs03.h"
#include "lcd.h"

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   " Expkits EXS-03 "
#define MESSAGE2   "   STM32F10x    "

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t Timingdelay_ms;
/* Private variables ---------------------------------------------------------*/
FATFS fs;         /* Work area (file system object) for logical drive */
FIL fsrc;         /* file objects */
FRESULT res;
UINT br;

char path[512]="0:";
char str[100];

/* Private function prototypes -----------------------------------------------*/
int SD_TotalSize(void);
FRESULT scan_files (char *path);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    delay_init();
    EXS03_initGPIO();
    GPIOC->ODR&=~(1<<8);// EXS-03 BUZZER OFF
    EXS03_initUSART();
    MSD_SPI_Configuration(); //MicroSD i�in SPI1'i konfigure et
    lcd_init();
    lcd_yaz(MESSAGE1);
    delay_ms(10);
    lcd_gotoxy(2,1);
    lcd_yaz(MESSAGE2);
    delay_ms(1500);
    printf("\r\n");
    printf("*************************************************\r\n");
    printf("*                                               *\r\n");
    printf("*     www.Expkits.com  - SD Kart Uygulamasi     *\r\n");
    printf("*                                               *\r\n");
    printf("************************************ ************\r\n");
    if( _card_insert() == 0 )
    {
        printf("-- SD kart bulundu! \r\n");
        lcd_gotoxy(1,1);
        lcd_yaz("SD kart bulundu!");
    }
    else
    {
        printf("-- Lutfen SD Karti Yerlestirin \r\n");
        lcd_gotoxy(1,1);
        lcd_yaz("SD kart yok...! ");
        while( _card_insert() != 0 );
        printf("-- SD Kart Baglanti hatasi \r\n");
        lcd_gotoxy(1,1);
        lcd_yaz("SD kart hatasi! ");
        while(1);
    }
    f_mount(0,&fs);	//KART'i erisimi aktif et
    delay_ms(500);
    lcd_gotoxy(1,1);
    lcd_yaz("Okunuyor...     ");
    scan_files(path); // Karttaki dosyalari tek tek tara
    lcd_gotoxy(1,1);
    lcd_yaz("Toplam Kapasite:");
    SD_TotalSize();	   // Karttaki toplam kapasite ve bos alani goster
    /* Infinite loop */
    while (1)
    {
        /* Insert 1000 ms delay_ms */
        GPIOA->ODR|=1;
        delay_ms(500); //500ms
        GPIOA->ODR&=0;
        delay_ms(500); //500ms
    }
}

/***********Karttaki Dosyalari Tarama Icin SCAN Fomksiyonu*******************/
FRESULT scan_files (char *path)
{
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;
#if _USE_LFN
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif
    res = f_opendir(&dir, path);
    if (res == FR_OK)
    {
        i = strlen(path);
        for (;;)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fname[0] == '.') continue;
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR)
            {
                sprintf(&path[i], "/%s", fn);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            }
            else
            {
                printf("%s/%s \r\n", path, fn);
                lcd_gotoxy(2,1);
                sprintf(str,"%s                ",fn);
                lcd_yaz(str);
                delay_ms(500);
            }
        }
    }
    return res;
}


int SD_TotalSize(void)
{
    FATFS *fs;
    DWORD fre_clust;
    res = f_getfree("0:", &fre_clust, &fs);
    if ( res==FR_OK )
    {
        printf("\r\n%d MB toplam disk alani!\r\n"
               "%d MB bos alan!.\r\n",
               ( (fs->n_fatent - 2) * fs->csize ) / 2 /1024 , (fre_clust * fs->csize) / 2 /1024 );
        sprintf(str,"%dMB/%dMB ",( (fs->n_fatent - 2) * fs->csize ) / 2 /1024 , (fre_clust * fs->csize) / 2 /1024 );
        lcd_gotoxy(2,1);
        lcd_yaz(str);
        return ENABLE;
    }
    else
        return DISABLE;
}


PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}

    return ch;
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

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
