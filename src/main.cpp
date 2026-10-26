#include <Arduino.h>
#include <avr/interrupt.h>

// LED pins: red on pad 10 (PB6), green on pad 6 (PD7), blue on pad 12 (PD6, always off).
// Sensor A0 goes through a 10k/15k voltage divider into pad 9 (PB5, ADC12).

#define RED_LED 6
#define GREEN_LED 7
#define BLUE_LED 6
#define SENSOR_INPUT 5

#define TIMER_TOP 31250
#define UPDATE_DELAY 1000
#define NORMAL_LIMIT 40
#define ALARM_LIMIT 100

void SetupIO()
{
    DDRB |= (1 << RED_LED);
    DDRD |= (1 << GREEN_LED);
    DDRD |= (1 << BLUE_LED);

    PORTB &= ~(1 << RED_LED);
    PORTD &= ~(1 << GREEN_LED);
    PORTD &= ~(1 << BLUE_LED);
}

void SetupTimer()
{
    TCCR1B = 0;
    TCCR1A = 0;
    TCNT1 = 0;
    OCR1A = TIMER_TOP;
    TIFR1 = (1 << TOV1);
    TIMSK1 = (1 << TOIE1);

    // 500 ms at 8 MHz with prescaler 64
    TCCR1A = 0b00000001;
    TCCR1B = 0b00010011;
}

void SetupADC()
{
    ADCSRA = 0;

    DDRB &= ~(1 << SENSOR_INPUT);
    PORTB &= ~(1 << SENSOR_INPUT);

    ADMUX = 0b01000100;
    ADCSRB = 0b00100110;
    DIDR2 |= (1 << ADC12D);

    ADCSRA = 0b10100010;
    ADCSRA |= (1 << ADSC);
}

ISR(TIMER1_OVF_vect)
{
}

void UpdateLED()
{
    unsigned int COValue = ADCW;

    PORTB &= ~(1 << RED_LED);
    PORTD &= ~(1 << GREEN_LED);

    if (COValue > ALARM_LIMIT)
    {
        PORTB |= (1 << RED_LED);
    }
    else if (COValue > NORMAL_LIMIT)
    {
        PORTB |= (1 << RED_LED);
        PORTD |= (1 << GREEN_LED);
    }
    else
    {
        PORTD |= (1 << GREEN_LED);
    }
}

void setup()
{
    cli();

    SetupIO();
    SetupTimer();
    SetupADC();

    sei();
}

void loop()
{
    UpdateLED();
    delay(UPDATE_DELAY);
}
