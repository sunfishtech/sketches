CosaCanvasHistogram
===============

Cosa demonstration of Canvas device driver for ST7735, 262K Color
Single-Chip TFT Controller, and monitoring of analog pins as a histogram.
Use the analog pin(0..MAX-1) as the probe pins.

Circuit
-------
Using a HY-1.8 SPI TFT 128*160 board connect as follows (Arduino ==>
HY-1.8 SPI):

![HY-1.8 SPI Board](http://img04.taobaocdn.com/imgextra/i4/39773402/T2dYOmXflaXXXXXXXX_!!39773402.jpg)

       GND ==> GND(1)  
       VCC(5V) ==> VCC(2)  
       RST ==> RESET(6)  
       Pin(9) ==> A0(7)  
       MOSI/Pin(11) ==> SDA(8)  
       SCK/Pin(13) ==> SCK(9)  
       SS/Pin(10) ==> CS(10)  
       VCC(5V) ==> LED+(15)  
       GND ==> LED-(16)    



