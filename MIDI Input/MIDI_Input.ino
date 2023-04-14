/*
                              MIDI Input
                        Vernon Billingsley c2023
                          Raspberry Pi Pico
                         Arduino IDE 1.8.13

                  Some simple routines for MIDI input on the
                  Raspberry Pi Pico....
                  Printed to the Serial Monitor for the demo...

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


  Serial Uart  MIDI Input: 
                                                      3.3 volt
                                             6N138       |
                                            ---U----     |  
                                         o-| 1     8 |-o------ 470 --|
      |---------- 220 -------------------o-| 2     7 |-o----|        |
    1 |                   |          ----o-| 3     6 |-o--- | ------------ RX Out
    o |                   |         |    o-| 4     5 |-o-|  | 
      o 4                 |         |       --------     |  | 
 >      o 2               --        |                    |  1K 
      o 5                 /\ 1N4148 |                    |  |
    o |                   |         |                    |--|
    3 |                   |         |                    |
      |------------------------------                   GND 
  5 pin DIN
      
 Development software @:
 https://github.com/ScottBillingsley/Raspberry-Pi-Pico-Development
 

*/
/************************ Includes ********************************/


/************************* Defines ********************************/
#if !defined(LED_BUILTIN)
#define LED_BUILTIN     25
#endif

#define DEBUG 1

#if DEBUG > 0
#define dprint(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
#define dshow(expression) Serial.print( expression )
#else
#define dprint(expression)
#define dshow(expression)
#endif

/* Define the UART */
#define UART_ID uart0
#define BAUD_RATE 31250
#define DATA_BITS 8
#define STOP_BITS 2
#define PARITY    UART_PARITY_NONE
#define UART_TX_PIN 0
#define UART_RX_PIN 1
/* Define standard midi numbers */
#define MIDI_NOTE_OFF 0x80      /* 0x8c, note #, velocity */
#define MIDI_NOTE_ON 0x90       /* 0x9c, note #, vleocity */
#define NOTE_AFTERTOUCH 0xA0    /* 0xAc, note #, amount   */
#define CONTROL_EVENT 0xB0      /* 0xBc, controller #, value */
#define PROGRAM_CHANGE 0xC0     /* 0xCc, program #        */
#define CHANNEL_AFTERTOUCH 0xD0 /* 0xDc, amount           */
#define PITCH_BEND 0xE0         /* 0xEc, value LSB, value MSB */


/************************** Init Includes *************************/


/************************** Variables *****************************/
uint8_t midi_buffer[4];

/************************** Variables1 ****************************/

/**************************  Functions ****************************/
void Pico_MIDI_begin() {
  /* Set up the UART with a basic baud rate */
  uart_init(UART_ID, 2400);
  /*  Set the TX and RX pins */
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  /* Set the baudrate for MIDI input */
  int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);
  /* No flow control */
  uart_set_hw_flow(UART_ID, false, false);
  /* Enable the FIFOs */
  uart_set_fifo_enabled(UART_ID, false);
}

void Pico_MIDI_check() {
  uint8_t this_buffer[2];
  if (uart_is_readable(UART_ID) > 0) {
    while (uart_is_readable(UART_ID)) {
      /* Get the first char and compare it to the different modes */
      uint8_t this_char = uart_getc(UART_ID);
      /* Check for Note On */
      if ((this_char & 0xf0) == MIDI_NOTE_ON) {
        /* Wait for 2 more byte and get */
        uart_read_blocking(UART_ID, this_buffer, 2);
        midi_buffer[0] = this_char & 0x0f; /* Channel  */
        midi_buffer[1] = this_char & 0xf0; /* Command  */
        midi_buffer[2] = this_buffer[0];   /* Note     */
        midi_buffer[3] = this_buffer[1];   /* Velocity */
        Midi_Note_On(midi_buffer[0], midi_buffer[2], midi_buffer[3]);
      }/* End Note On */
      /* Check for Note Off */
      if ((this_char & 0xf0) == MIDI_NOTE_OFF) {
        /* Wait for 2 more byte and get */
        uart_read_blocking(UART_ID, this_buffer, 2);
        midi_buffer[0] = this_char & 0x0f; /* Channel  */
        midi_buffer[1] = this_char & 0xf0; /* Command  */
        midi_buffer[2] = this_buffer[0];   /* Note     */
        midi_buffer[3] = this_buffer[1];   /* Velocity */
        Midi_Note_Off(midi_buffer[0], midi_buffer[2], midi_buffer[3]);
      }/* End Note Off */
      /* Check for control event */
      if ((this_char & 0xf0) == CONTROL_EVENT) {
        /* Wait for 2 more byte and get */
        uart_read_blocking(UART_ID, this_buffer, 2);
        /* Store the control event */
        midi_buffer[0] = this_char & 0x0f; /* Channel  */
        midi_buffer[1] = this_char & 0xf0; /* Command  */
        midi_buffer[2] = this_buffer[0];   /* Control  */
        midi_buffer[3] = this_buffer[1];   /* Value    */
        Midi_Control(midi_buffer[0], midi_buffer[1], midi_buffer[2], midi_buffer[3]);
      }/* End Control_Event */
      /* Check for pitch bend event */
      if ((this_char & 0xf0) == PITCH_BEND) {
        /* Wait for 2 more byte and get */
        uart_read_blocking(UART_ID, this_buffer, 2);
        /* Store the pitch bend event */
        midi_buffer[0] = this_char & 0x0f; /* Channel  */
        midi_buffer[1] = this_char & 0xf0; /* Command  */
        midi_buffer[2] = this_buffer[0];   /* Low Byte */
        midi_buffer[3] = this_buffer[1];   /* High Byte */
        Midi_PitchBend(midi_buffer[0], midi_buffer[1], midi_buffer[2], midi_buffer[3]);
      }/* End Pitch_Bend */

    }/* End while */
  }/* End uart check */
}/* End midi check */


void Midi_Note_On(uint8_t _channel, uint8_t note, uint8_t velocity) {
  Serial.print(_channel);
  Serial.print("\t");
  Serial.print("Note On  :");
  Serial.print(note);
  Serial.print("\t");
  Serial.println(velocity);  
}
void Midi_Note_Off(uint8_t _channel, uint8_t note, uint8_t velocity) {
  Serial.print(_channel);
  Serial.print("\t");
  Serial.print("Note Off :");
  Serial.print(note);
  Serial.print("\t");
  Serial.println(velocity);
}
void Midi_Control(uint8_t _channel, uint8_t command, uint8_t _control, uint8_t value) {
  Serial.print(_channel);
  Serial.print("\t");
  Serial.print("Command :");
  Serial.print(command);
  Serial.print("\t");
  Serial.print("Control :");
  Serial.print(_control);
  Serial.print("\t");
  Serial.print("Value :");
  Serial.println(value);
}
void Midi_PitchBend(uint8_t _channel, uint8_t command, uint8_t low_byte, uint8_t _hi_byte) {
  uint16_t this_value = (_hi_byte << 7) + low_byte;
  Serial.print(_channel);
  Serial.print("\t");
  Serial.print("Command :");
  Serial.print(command);
  Serial.print("\t");
  Serial.println(this_value);
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
  pinMode(LED_BUILTIN, OUTPUT);

  /* Setup the Uart */
  Pico_MIDI_begin();


}/**************************  End Setup **************************/

/******************************************************************/
/*************************** Setup1 *******************************/
/******************************************************************/
void setup1() {



}/**************************  End Setup1 **************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  /* Check the uart each loop */
  Pico_MIDI_check();




}/*************************** End Loop *****************************/

/******************************************************************/
/**************************** Loop1 *******************************/
/******************************************************************/
void loop1() {


}/*************************** End Loop1 ****************************/
