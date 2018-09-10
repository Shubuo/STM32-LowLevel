#define LED_PIN  9

volatile uint8_t led_blink = 0;

ISR(SPI_STC_vect)
{
    uint8_t data_byte = SPDR;

    switch (data_byte)
    {
        case '0':
            led_blink = 0;
            SPDR = 0;  
        break;
        case '1':
            led_blink = 1;
            SPDR = 0;  
        break;
        case '?':
            // Place LED blinking status in SPDR register for next transfer
            SPDR = led_blink;  
        break;
    }  
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    // Set MISO pin as output
    pinMode(MISO, OUTPUT);
    // Turn on SPI in slave mode
    SPCR |= (1 << SPE);
    // Turn on interrupt
    SPCR |= (1 << SPIE);
}

void loop() 
{
    // If LED blink status is on, then blink LED for 250ms
    if (led_blink == 1)
    {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);  
    }
    else if (led_blink == 0)
    {
        digitalWrite(LED_PIN, LOW); 
    }
}
