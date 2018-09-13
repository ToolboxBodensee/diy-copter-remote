#include "io_data.h"
#include "pins.h"
#include <Arduino.h>

IO_DATA::IO_DATA()
{
}
void IO_DATA::adc_init(void)
{
    // AREF = AVcc
    ADMUX = (0 << REFS1) | (1 << REFS0);
    Serial.print("ADMUX");
    Serial.print(ADMUX);
    Serial.print(" ADCSRA");
    Serial.print(ADCSRA);
    Serial.print("\n");

    // ADC Enable
    ADCSRA |= _BV(ADEN);
    // ADC prescaler of 128
    // 16000000/128 = 125000
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

uint16_t IO_DATA::adc_read(uint8_t ch)
{
    // ‘ch’ between 0 and 7
    //
    if (ch >= 0x08) {
        /* Set Mux5 */
        ADCSRB |= _BV(MUX5);
        ch &= ~0x08;
    } else {
        /* Clear Mux5 */
        ADCSRB &= ~_BV(MUX5);
    }

    ch = ch & 0b00000111;
    Serial.print("ch");
    Serial.print(ch);
    ADMUX |= ch;

    // start single convertion
    ADCSRA |= (1 << ADSC);

    // wait for conversion to complete
    delay(50);

    while (ADCSRA & (1 << ADSC)); // wait for conversion to complete

    /* while(!(ADCSRA & (1<<ADIF))) { */
    /*     /1* nothing *1/ */
    /* } */

    // clear bit
    ADCSRA |= (1 << ADIF);

    return (ADC);
}

void IO_DATA::init()
{
#if 0
    adc_init();
#endif
    BUTTON_R_INPUT;
    BUTTON_L_INPUT;
    ARM_INPUT;
    FLIGHT_MODE_INPUT;

    /* inital read */
    update();
}
void IO_DATA::update()
{
#if 0
    /* read joysticks */
    r_x = adc_read(AD_R_X_JOY_PIN);
    r_y = adc_read(AD_R_Y_JOY_PIN);


    l_x = adc_read(AD_L_X_JOY_PIN);
    l_y = adc_read(AD_L_Y_JOY_PIN);

    /* read akku power */
    akku_power = adc_read(AD_AKKUPOWER_PIN);
#else
    /* read joysticks */
    r_x = analogRead(AD_R_X_JOY_PIN);
    r_y = analogRead(AD_R_Y_JOY_PIN);

    l_x = analogRead(AD_L_X_JOY_PIN);
    l_y = analogRead(AD_L_Y_JOY_PIN);

    /* read akku power */
    akku_power = analogRead(AD_AKKUPOWER_PIN);
#endif

    r_sw = BUTTON_R_IS_HIGH;
    l_sw = BUTTON_L_IS_HIGH;

    /* read switches */
    arm_sw = ARM_IS_HIGH;
    flight_mode_sw = FLIGHT_MODE_IS_HIGH;

}
