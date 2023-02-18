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
 
Random Number Test<br>
&emsp; &emsp; Seup the random number generator using the ring oscillator to generate a random seed..<br>
Dual Core Test<br>
&emsp; &emsp;Test writing to an array using core1 and read from and printing the array using core0, pausing core0 during the write cycle...<br>
Dual Core Test 2<br>
&emsp; &emsp;Test writing to an array using core1 and read from and printing the array using core0..<br>
&emsp; &emsp;Use the FIFO to pause core0 during the write cycle...<br>


