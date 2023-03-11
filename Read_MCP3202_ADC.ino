/*

                        Vernon Billingsley c2023
                          Raspberry Pi Pico
                         Arduino IDE 1.8.13

                   Setup the SPI and get a reading from the
                   MCP3202 ADC...      

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission
    notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

                Raspberry Pi Pico

                -----------------
               |      |USB|      |
  TXD 0 GPIO0 -|1              40|-  VBUS
  RXD 0 GPIO1 -|2              39|-  Vsys
        GND   -|3              38|-  GND
        GPIO2 -|4              37|-  EN
        GPIO3 -|5              36|-  3.3v Out
        GPIO4 -|6              35|-  ADC Ref
        GPIO5 -|7              34|-  GPIO28  ADC2
        GND   -|8              33|-  GND AGND
        GPIO6 -|9              32|-  GPIO27  ACD1
        GPIO7 -|10             31|-  GPIO26
        GPIO8 -|11             30|-  Run/Reset
        GPIO9 -|12             29|-  GPIO22
        GND   -|13             28|-  GND
       GPIO10 -|14             27|-  GPIO21
       GPIO11 -|15             26|-  GPIO20
       GPIO12 -|16             25|-  GPIO19
       GPIO13 -|17             24|-  GPIO18
        GND   -|18             23|-  GND
       GPIO14 -|19             22|-  GPIO17
       GPIO15 -|20             21|-  GPIO16
                -----------------

  Board json..
  https://github.com/earlephilhower/arduino-pico

  Board Settings:
  Raspberry Pi Pico
  2MB (on FS)
  133 mHz
  Small (-Os) (standard)
  RTTI: disabled
  Stack Protector: disabled
  C++ Exceptions: disabled
  Debug Port: disabled
  Debug Level: none
  USB Stack: "Pico SDK"
  IP Stack: "IPv4 Only"
  Upload Method: "Default (UF2)"


                 MCP3202
                ---------
 GPIO5    CS --|1   U   8|-- Vdd/Vref
         CH0 --|2       7|-- CLK    GPIO6
         CH1 --|3       6|-- Dout   GPIO4
         GND --|4       5|-- Din    GPIO7
                ---------

           Suggested dac input, datasheet page 14
                              2k2
           0 to 5 vdc -------/\/\/-------- CHx
                                    |
                                    _
                                    -   472n
                                    |
                                   GND


*/
/************************ Includes ********************************/
#include <SPI.h>

/************************* Defines ********************************/
#if !defined(LED_BUILTIN)
#define LED_BUILTIN     25
#endif

#define DEBUG 0

#if DEBUG > 0
#define dprint(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
#define dshow(expression) Serial.print( expression )
#else
#define dprint(expression)
#define dshow(expression)
#endif

/* Chip Select 0 GPIO5, Chip pin 7 */
#define CS0   5
/* SPI0 Clock GPIO6, Chip pin 9 */
#define SCK0 6
/* SPI0 Data Out GPIO7, Chip pin 10 */
#define MOSI0 7
/* SPI0 Data In GPIO4, Chip pin 6 */
#define MISO0 4

/* MCP3202 Defines */
/* Single channel read on chaanel 0 */
#define SingleChannel0 0b10100000
/* Single channel read on chaanel 1 */
#define SingleChannel1 0b11100000
/* Differential channel read CH0 as +ve, CH1 as -ve */
#define DiffChannel0 0b00100000
/* Differential channel read CH0 as -ve, CH1 as +ve */
#define DiffChannel1 0b01100000


/************************** Init Includes *************************/

/************************** Variables *****************************/

/************************** Variables1 ****************************/
volatile boolean flip0 = false;


/**************************  Functions ****************************/
uint16_t read_mcp3202(uint8_t mode) {
  uint8_t HIGH_BYTE = 0;
  uint8_t LOW_BYTE = 0;
  uint16_t out_word = 0;
  /* Stop interrupts */
  cli();
  /* Begin transaction */
  /* 1.5 mHz clock speed, datasheet page 3 */
  SPI.beginTransaction(SPISettings(1500000, MSBFIRST, SPI_MODE3));
  /* Take the chip select LOW to begin */
  digitalWrite(CS0, LOW);
  /* Send 7 leading Zeros and a Start bit, datasheet page 17 */
  LOW_BYTE = SPI.transfer(0x01);
  /* Read the HIGH byte from the '3202 */
  HIGH_BYTE = SPI.transfer(mode);
  /* Read the LOW byte */
  LOW_BYTE = SPI.transfer(0);
  /* Take the chip select HIGH */
  digitalWrite(CS0, HIGH);
  /* End the transaction */
  SPI.endTransaction();
  /* Restore interrupts */
  sei();
  /* Get the HIGH nibble and place it in the out word */
  out_word = (HIGH_BYTE & 0x0f) << 8;
  /* Place the LOW byte in the out word */
  out_word = out_word | LOW_BYTE;
  /* Return the out word */
  return out_word;
}

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG > 0) {
    Serial.begin(115200);
    while (!Serial);
    Serial.print(F(BOARD_NAME)); Serial.print(" @ "); Serial.print(F_CPU); Serial.println(" mHz");
  }

  pinMode(CS0, OUTPUT);
  digitalWrite(CS0, HIGH);

  /* Set SPI0 to the alternate pins */
  SPI.setCS(CS0);
  SPI.setSCK(SCK0);
  SPI.setTX(MOSI0);
  SPI.setRX(MISO0);

  /* Start with the boolean true, sketch handle the CS pin */
  SPI.begin(true);

}/**************************  End Setup **************************/

/******************************************************************/
/*************************** Setup1 *******************************/
/******************************************************************/
void setup1() {

  pinMode(LED_BUILTIN, OUTPUT);

}/**************************  End Setup1 **************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  uint16_t this_x = read_mcp3202(SingleChannel0);

  Serial.println(this_x);

  delay(25);


}/*************************** End Loop *****************************/

/******************************************************************/
/**************************** Loop1 *******************************/
/******************************************************************/
void loop1() {

  /* Change the boolean */
  flip0 = !flip0;
  /* Write to the pin */
  digitalWrite(LED_BUILTIN, flip0);
  delay(125);

}/*************************** End Loop1 ****************************/
