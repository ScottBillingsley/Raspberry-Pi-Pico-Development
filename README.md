# Raspberry-Pi-Pico-Development
A place for my development files for the Raspberry Pi Pico..<br>
Raspberry Pi Pico<br>
Arduino IDE 1.8.13<br>
<br>
Board json..<br>
  https://github.com/earlephilhower/arduino-pico<br>
<br>
  Board Settings:<br>
  Raspberry Pi Pico<br>
  2MB (on FS)<br>
  133 mHz<br>
  Small (-Os) (standard)<br>
  RTTI: disabled<br>
  Stack Protector: disabled<br>
  C++ Exceptions: disabled<br>
  Debug Port: disabled<br>
  Debug Level: none<br>
  USB Stack: "Pico SDK"<br>
  IP Stack: "IPv4 Only"<br>
  Upload Method: "Default (UF2)"<br>
  
  ## Files:

ADC Read Test<br>
&emsp; &emsp;Setup and read the ADC on core0 and control<br>
&emsp; &emsp;the bllink delay of LED on core 1..<br>
ADC Read Test 2<br>
&emsp; &emsp;Setup and read the ADC on core0 and control<br>
&emsp; &emsp;the blink delay on core1 by passing a byte<br>
&emsp; &emsp;between cores...<br>
Dual Core Test<br>
&emsp; &emsp;Test writing to an array using core1 and read from and printing the array using core0,<br>
&emsp; &emsp;pausing core0 during the write cycle...<br>
Dual Core Test 2<br>
&emsp; &emsp;Test writing to an array using core1 and read from and printing the array using core0..<br>
&emsp; &emsp;Use the FIFO to pause core0 during the write cycle...<br>
Dual Core Test 3<br>
&emsp; &emsp;Use RPi_Pico_TimerInterrupt to blink an LED<br>
&emsp; &emsp;One on core0 and one on core1...<br>
&emsp; &emsp;https://github.com/khoih-prog/RPI_PICO_TimerInterrupt<br>
MCP4921 Dual Core Test<br>
&emsp; &emsp;Setup the Pico in dual core mode, setup <br>
&emsp; &emsp;the MCP4921 DAC on core1 and send a full <br>
&emsp; &emsp;speed 1024 sample sine wave to the dac...<br>
&emsp; &emsp;Blink LED on core0 to confirm dual core..<br>
Random Number Test<br>
&emsp; &emsp; Seup the random number generator using the ring oscillator to generate a random seed..<br>
Serial Wombat Encoder Test<br>
&emsp; &emsp;https://broadwellconsultinginc.github.io/SerialWombatArdLib/ <br>
&emsp; &emsp;Setup and test the Serial Wombat in the encoder mode on I2C...<br>
&emsp; &emsp;Display the results incrementing and decrementing a variable... <br>


