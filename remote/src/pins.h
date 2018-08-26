/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 Multiprotocol is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with Multiprotocol.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * LEONARDO    usb
 *          __Micro__
 *   D1    -|       |- RAW
 *   D0    -|       |- GND
 *   GND   -|       |- RESET
 *   GND   -|       |- VCC
 *   D2    -|       |- A3
 *   D3    -|       |- A2
 *   D4/A6 -|       |- A1
 *   D5    -|       |- A0
*   D6/A7  -|       |- D15
 *   D7    -|       |- D14
 *   D8/A8 -|       |- D16
 *   D9/A9 -|       |- D10/A10
 *          '''''''''
 */
//*******************
//***   Pinouts   ***
//*******************
/* i2c clock */
#define I2C_SLC_PIN                 3
#define I2C_SLC_PORT                PORTD
#define I2C_SLC_IPR                 PIND
#define I2C_SLC_DDR                 DDRD
#define I2C_SLC_ON                  I2C_SLC_PORT |=  _BV(I2C_SLC_PIN)
#define I2C_SLC_OFF                 I2C_SLC_PORT &= ~_BV(I2C_SLC_PIN)
#define I2C_SLC_OUTPUT              I2C_SLC_DDR  |=  _BV(I2C_SLC_PIN)
#define I2C_SLC_INPUT               I2C_SLC_DDR  &= ~_BV(I2C_SLC_PIN)
#define I2C_SLC_IS_HIGH             ((I2C_SLC_IPR &  _BV(I2C_SLC_PIN)) != 0x00)
#define I2C_SLC_IS_LOW              ((I2C_SLC_IPR &  _BV(I2C_SLC_PIN)) == 0x00)

/* i2c data */
#define I2C_SDA_PIN                 2
#define I2C_SDA_PORT                PORTD
#define I2C_SDA_IPR                 PIND
#define I2C_SDA_DDR                 DDRD
#define I2C_SDA_ON                  I2C_SDA_PORT |=  _BV(I2C_SDA_PIN)
#define I2C_SDA_OFF                 I2C_SDA_PORT &= ~_BV(I2C_SDA_PIN)
#define I2C_SDA_OUTPUT              I2C_SDA_DDR  |=  _BV(I2C_SDA_PIN)
#define I2C_SDA_INPUT               I2C_SDA_DDR  &= ~_BV(I2C_SDA_PIN)
#define I2C_SDA_IS_HIGH             ((I2C_SDA_IPR &  _BV(I2C_SDA_PIN)) != 0x00)
#define I2C_SDA_IS_LOW              ((I2C_SDA_IPR &  _BV(I2C_SDA_PIN)) == 0x00)

/* switch flight mode */
#define FLIGHT_MODE_PIN             0
#define FLIGHT_MODE_PORT            PORTD
#define FLIGHT_MODE_IPR             PIND
#define FLIGHT_MODE_DDR             DDRD
#define FLIGHT_MODE_ON              FLIGHT_MODE_PORT |=  _BV(FLIGHT_MODE_PIN)
#define FLIGHT_MODE_OFF             FLIGHT_MODE_PORT &= ~_BV(FLIGHT_MODE_PIN)
#define FLIGHT_MODE_OUTPUT          FLIGHT_MODE_DDR  |=  _BV(FLIGHT_MODE_PIN)
#define FLIGHT_MODE_INPUT           FLIGHT_MODE_DDR  &= ~_BV(FLIGHT_MODE_PIN)
#define FLIGHT_MODE_IS_HIGH         ((FLIGHT_MODE_IPR &  _BV(FLIGHT_MODE_PIN)) != 0x00)
#define FLIGHT_MODE_IS_LOW          ((FLIGHT_MODE_IPR &  _BV(FLIGHT_MODE_PIN)) == 0x00)

/* switch arm */
#define ARM_PIN                     1
#define ARM_PORT                    PORTD
#define ARM_IPR                     PIND
#define ARM_DDR                     DDRD
#define ARM_ON                      ARM_PORT |=  _BV(ARM_PIN)
#define ARM_OFF                     ARM_PORT &= ~_BV(ARM_PIN)
#define ARM_OUTPUT                  ARM_DDR  |=  _BV(ARM_PIN)
#define ARM_INPUT                   ARM_DDR  &= ~_BV(ARM_PIN)
#define ARM_IS_HIGH                 ((ARM_IPR &  _BV(ARM_PIN)) != 0x00)
#define ARM_IS_LOW                  ((ARM_IPR &  _BV(ARM_PIN)) == 0x00)

/* power amplifier */
#define POWER_AMP_PIN               5
#define POWER_AMP_PORT              PORTD
#define POWER_AMP_IPR               PIND
#define POWER_AMP_DDR               DDRD
#define POWER_AMP_ON                POWER_AMP_PORT |=  _BV(POWER_AMP_PIN)
#define POWER_AMP_OFF               POWER_AMP_PORT &= ~_BV(POWER_AMP_PIN)
#define POWER_AMP_OUTPUT            POWER_AMP_DDR  |=  _BV(POWER_AMP_PIN)
#define POWER_AMP_INPUT             POWER_AMP_DDR  &= ~_BV(POWER_AMP_PIN)
#define POWER_AMP_IS_HIGH           ((POWER_AMP_IPR &  _BV(POWER_AMP_PIN)) != 0x00)
#define POWER_AMP_IS_LOW            ((POWER_AMP_IPR &  _BV(POWER_AMP_PIN)) == 0x00)

/* low noise amplifier */
#define LOW_NOISE_AMP_PIN           5
#define LOW_NOISE_AMP_PORT          PORTD
#define LOW_NOISE_AMP_IPR           PIND
#define LOW_NOISE_AMP_DDR           DDRD
#define LOW_NOISE_AMP_ON            LOW_NOISE_AMP_PORT |=  _BV(LOW_NOISE_AMP_PIN)
#define LOW_NOISE_AMP_OFF           LOW_NOISE_AMP_PORT &= ~_BV(LOW_NOISE_AMP_PIN)
#define LOW_NOISE_AMP_OUTPUT        LOW_NOISE_AMP_DDR  |=  _BV(LOW_NOISE_AMP_PIN)
#define LOW_NOISE_AMP_INPUT         LOW_NOISE_AMP_DDR  &= ~_BV(LOW_NOISE_AMP_PIN)
#define LOW_NOISE_AMP_IS_HIGH       ((LOW_NOISE_AMP_IPR &  _BV(LOW_NOISE_AMP_PIN)) != 0x00)
#define LOW_NOISE_AMP_IS_LOW        ((LOW_NOISE_AMP_IPR &  _BV(LOW_NOISE_AMP_PIN)) == 0x00)

/* buzzer */
#define BUZZER_PIN                  8
#define BUZZER_PORT                 PORTD
#define BUZZER_IPR                  PIND
#define BUZZER_DDR                  DDRD
#define BUZZER_ON                   BUZZER_PORT |=  _BV(BUZZER_PIN)
#define BUZZER_OFF                  BUZZER_PORT &= ~_BV(BUZZER_PIN)
#define BUZZER_OUTPUT               BUZZER_DDR  |=  _BV(BUZZER_PIN)
#define BUZZER_INPUT                BUZZER_DDR  &= ~_BV(BUZZER_PIN)
#define BUZZER_IS_HIGH              ((BUZZER_IPR &  _BV(BUZZER_PIN)) != 0x00)
#define BUZZER_IS_LOW               ((BUZZER_IPR &  _BV(BUZZER_PIN)) == 0x00)

/* chip select cc2500 */
#define CC2500_CS_PIN               8
#define CC2500_CS_PORT              PORTD
#define CC2500_CS_IPR               PIND
#define CC2500_CS_DDR               DDRD
#define CC2500_CS_ON                CC2500_CS_PORT |=  _BV(CC2500_CS_PIN)
#define CC2500_CS_OFF               CC2500_CS_PORT &= ~_BV(CC2500_CS_PIN)
#define CC2500_CS_OUTPUT            CC2500_CS_DDR  |=  _BV(CC2500_CS_PIN)
#define CC2500_CS_INPUT             CC2500_CS_DDR  &= ~_BV(CC2500_CS_PIN)
#define CC2500_CS_IS_HIGH           ((CC2500_CS_IPR &  _BV(CC2500_CS_PIN)) != 0x00)
#define CC2500_CS_IS_LOW            ((CC2500_CS_IPR &  _BV(CC2500_CS_PIN)) == 0x00)

/* Analog right Joystick X direction */
#define AD_R_X_JOY_PIN              3
#define AD_R_X_JOY_PORT             PORTA
#define AD_R_X_JOY_IPR              PORTA
#define AD_R_X_JOY_DDR              DDRA
#define AD_R_X_JOY_ON               AD_R_X_JOY_PORT |=  _BV(AD_R_X_JOY_PIN)
#define AD_R_X_JOY_OFF              AD_R_X_JOY_PORT &= ~_BV(AD_R_X_JOY_PIN)
#define AD_R_X_JOY_OUTPUT           AD_R_X_JOY_DDR  |=  _BV(AD_R_X_JOY_PIN)
#define AD_R_X_JOY_INPUT            AD_R_X_JOY_DDR  &= ~_BV(AD_R_X_JOY_PIN)
#define AD_R_X_JOY_IS_HIGH          ((AD_R_X_JOY_IPR &  _BV(AD_R_X_JOY_PIN)) != 0x00)
#define AD_R_X_JOY_IS_LOW           ((AD_R_X_JOY_IPR &  _BV(AD_R_X_JOY_PIN)) == 0x00)

/* Analog right Joystick y direction */
#define AD_R_Y_JOY_PIN              2
#define AD_R_Y_JOY_PORT             PORTA
#define AD_R_Y_JOY_IPR              PORTA
#define AD_R_Y_JOY_DDR              DDRA
#define AD_R_Y_JOY_ON               AD_R_Y_JOY_PORT |=  _BV(AD_R_Y_JOY_PIN)
#define AD_R_Y_JOY_OFF              AD_R_Y_JOY_PORT &= ~_BV(AD_R_Y_JOY_PIN)
#define AD_R_Y_JOY_OUTPUT           AD_R_Y_JOY_DDR  |=  _BV(AD_R_Y_JOY_PIN)
#define AD_R_Y_JOY_INPUT            AD_R_Y_JOY_DDR  &= ~_BV(AD_R_Y_JOY_PIN)
#define AD_R_Y_JOY_IS_HIGH          ((AD_R_Y_JOY_IPR &  _BV(AD_R_Y_JOY_PIN)) != 0x00)
#define AD_R_Y_JOY_IS_LOW           ((AD_R_Y_JOY_IPR &  _BV(AD_R_Y_JOY_PIN)) == 0x00)

/* right joystick middle button */
#define BUTTON_R_PIN                8
#define BUTTON_R_PORT               PORTD
#define BUTTON_R_IPR                PIND
#define BUTTON_R_DDR                DDRD
#define BUTTON_R_ON                 BUTTON_R_PORT |=  _BV(BUTTON_R_PIN)
#define BUTTON_R_OFF                BUTTON_R_PORT &= ~_BV(BUTTON_R_PIN)
#define BUTTON_R_OUTPUT             BUTTON_R_DDR  |=  _BV(BUTTON_R_PIN)
#define BUTTON_R_INPUT              BUTTON_R_DDR  &= ~_BV(BUTTON_R_PIN)
#define BUTTON_R_IS_HIGH            ((BUTTON_R_IPR &  _BV(BUTTON_R_PIN)) != 0x00)
#define BUTTON_R_IS_LOW             ((BUTTON_R_IPR &  _BV(BUTTON_R_PIN)) == 0x00)

/* Analog left Joystick X direction */
#define AD_L_X_JOY_PIN              1
#define AD_L_X_JOY_PORT             PORTA
#define AD_L_X_JOY_IPR              PORTA
#define AD_L_X_JOY_DDR              DDRA
#define AD_L_X_JOY_ON               AD_L_X_JOY_PORT |=  _BV(AD_L_X_JOY_PIN)
#define AD_L_X_JOY_OFF              AD_L_X_JOY_PORT &= ~_BV(AD_L_X_JOY_PIN)
#define AD_L_X_JOY_OUTPUT           AD_L_X_JOY_DDR  |=  _BV(AD_L_X_JOY_PIN)
#define AD_L_X_JOY_INPUT            AD_L_X_JOY_DDR  &= ~_BV(AD_L_X_JOY_PIN)
#define AD_L_X_JOY_IS_HIGH          ((AD_L_X_JOY_IPR &  _BV(AD_L_X_JOY_PIN)) != 0x00)
#define AD_L_X_JOY_IS_LOW           ((AD_L_X_JOY_IPR &  _BV(AD_L_X_JOY_PIN)) == 0x00)

/* Analog left Joystick y direction */
#define AD_L_Y_JOY_PIN              0
#define AD_L_Y_JOY_PORT             PORTA
#define AD_L_Y_JOY_IPR              PORTA
#define AD_L_Y_JOY_DDR              DDRA
#define AD_L_Y_JOY_ON               AD_L_Y_JOY_PORT |=  _BV(AD_L_Y_JOY_PIN)
#define AD_L_Y_JOY_OFF              AD_L_Y_JOY_PORT &= ~_BV(AD_L_Y_JOY_PIN)
#define AD_L_Y_JOY_OUTPUT           AD_L_Y_JOY_DDR  |=  _BV(AD_L_Y_JOY_PIN)
#define AD_L_Y_JOY_INPUT            AD_L_Y_JOY_DDR  &= ~_BV(AD_L_Y_JOY_PIN)
#define AD_L_Y_JOY_IS_HIGH          ((AD_L_Y_JOY_IPR &  _BV(AD_L_Y_JOY_PIN)) != 0x00)
#define AD_L_Y_JOY_IS_LOW           ((AD_L_Y_JOY_IPR &  _BV(AD_L_Y_JOY_PIN)) == 0x00)

/* left joystick middle button */
#define BUTTON_L_PIN                8
#define BUTTON_L_PORT               PORTD
#define BUTTON_L_IPR                PIND
#define BUTTON_L_DDR                DDRD
#define BUTTON_L_ON                 BUTTON_L_PORT |=  _BV(BUTTON_L_PIN)
#define BUTTON_L_OFF                BUTTON_L_PORT &= ~_BV(BUTTON_L_PIN)
#define BUTTON_L_OUTPUT             BUTTON_L_DDR  |=  _BV(BUTTON_L_PIN)
#define BUTTON_L_INPUT              BUTTON_L_DDR  &= ~_BV(BUTTON_L_PIN)
#define BUTTON_L_IS_HIGH            ((BUTTON_L_IPR &  _BV(BUTTON_L_PIN)) != 0x00)
#define BUTTON_L_IS_LOW             ((BUTTON_L_IPR &  _BV(BUTTON_L_PIN)) == 0x00)

/* Analog akku power read */
#define AD_AKKUPOWER_PIN            10
#define AD_AKKUPOWER_PORT           PORTA
#define AD_AKKUPOWER_IPR            PORTA
#define AD_AKKUPOWER_DDR            DDRA
#define AD_AKKUPOWER_ON             AD_AKKUPOWER_PORT |=  _BV(AD_AKKUPOWER_PIN)
#define AD_AKKUPOWER_OFF            AD_AKKUPOWER_PORT &= ~_BV(AD_AKKUPOWER_PIN)
#define AD_AKKUPOWER_OUTPUT         AD_AKKUPOWER_PORT |=  _BV(AD_AKKUPOWER_PIN)
#define AD_AKKUPOWER_INPUT          AD_AKKUPOWER_PORT &= ~_BV(AD_AKKUPOWER_PIN)
#define AD_AKKUPOWER_IS_HIGH        ((AD_AKKUPOWER_IPR &  _BV(AD_AKKUPOWER_PIN)) != 0x00)
#define AD_AKKUPOWER_IS_LOW         ((AD_AKKUPOWER_IPR &  _BV(AD_AKKUPOWER_PIN)) == 0x00)

/* SPI clock */
#define SPI_SCLK_PIN                15
#define SPI_SCLK_PORT               PORTD
#define SPI_SCLK_IPR                PIND
#define SPI_SCLK_DDR                DDRD
#define SPI_SCLK_ON                 SPI_SCLK_PORT |=  _BV(SPI_SCLK_PIN)
#define SPI_SCLK_OFF                SPI_SCLK_PORT &= ~_BV(SPI_SCLK_PIN)
#define SPI_SCLK_OUTPUT             SPI_SCLK_DDR  |=  _BV(SPI_SCLK_PIN)
#define SPI_SCLK_INPUT              SPI_SCLK_DDR  &= ~_BV(SPI_SCLK_PIN)
#define SPI_SCLK_IS_HIGH            ((SPI_SCLK_IPR &  _BV(SPI_SCLK_PIN)) != 0x00)
#define SPI_SCLK_IS_LOW             ((SPI_SCLK_IPR &  _BV(SPI_SCLK_PIN)) == 0x00)

/* SPI master in slave out */
#define SPI_MISO_PIN                14
#define SPI_MISO_PORT               PORTD
#define SPI_MISO_IPR                PIND
#define SPI_MISO_DDR                DDRD
#define SPI_MISO_ON                 SPI_MISO_PORT |=  _BV(SPI_MISO_PIN)
#define SPI_MISO_OFF                SPI_MISO_PORT &= ~_BV(SPI_MISO_PIN)
#define SPI_MISO_OUTPUT             SPI_MISO_DDR  |=  _BV(SPI_MISO_PIN)
#define SPI_MISO_INPUT              SPI_MISO_DDR  &= ~_BV(SPI_MISO_PIN)
#define SPI_MISO_IS_HIGH            ((SPI_MISO_IPR &  _BV(SPI_MISO_PIN)) != 0x00)
#define SPI_MISO_IS_LOW             ((SPI_MISO_IPR &  _BV(SPI_MISO_PIN)) == 0x00)

/* SPI master out slave in*/
#define SPI_MOSI_PIN                14
#define SPI_MOSI_PORT               PORTD
#define SPI_MOSI_IPR                PIND
#define SPI_MOSI_DDR                DDRD
#define SPI_MOSI_ON                 SPI_MOSI_PORT |=  _BV(SPI_MOSI_PIN)
#define SPI_MOSI_OFF                SPI_MOSI_PORT &= ~_BV(SPI_MOSI_PIN)
#define SPI_MOSI_OUTPUT             SPI_MOSI_DDR  |=  _BV(SPI_MOSI_PIN)
#define SPI_MOSI_INPUT              SPI_MOSI_DDR  &= ~_BV(SPI_MOSI_PIN)
#define SPI_MOSI_IS_HIGH            ((SPI_MOSI_IPR &  _BV(SPI_MOSI_PIN)) != 0x00)
#define SPI_MOSI_IS_LOW             ((SPI_MOSI_IPR &  _BV(SPI_MOSI_PIN) == 0x00)

#define NOP                         __asm__("nop");
