# Christmas_Light_Tester

This is a 12v/5v multi-protocol device used to test most Christmas lights. The heart of the device is an ESP8266-12F. The ESP8266 WiFi Module is a self contained SOC with integrated TCP/IP protocol stack that can access to your WiFi network. The chip can be programmed using Arduino. 

Current Functions:
E1.31 output: controls the Espixelstick and outputs streaming DMX.
3 Wire: Adafruit's DotStars - AKA the APA102, Adafruit's Neopixel - aka the WS2812B (also WS2811/WS2812/WS2813, also supported in lo-speed mode) - a 3 wire addressable led chipset, TM1809/4 - 3 wire chipset, cheaply available on aliexpress.com, TM1803 - 3 wire chipset, sold by radio shack, CS1903 - another 3 wire led chipset, cheap, GW6205 - another 3 wire led chipset, LPD8806 - SPI based chpiset, very high speed, SM16716 - SPI based chipset, APA102 - SPI based chipset P9813 - aka Cool Neon's Total Control Lighting
DMX - send rgb data out over DMX using arduino DMX libraries.
WS2801 output: 4 wire (data & clock) driven using Renard protocol through a PIC16F1825 microchip programmed to be a PX1.
Rebard output: designed by Phil Short. Serial protocol that is parsed by microchips to control lights.
sACN Viwer: operates in multicast or unicast to monitor stream for Universe, number of packets and errors.

Thanks to:
Phil Short for Renard. 
Shelby Merrick for the work on E131.h. 
Daniel Garcia for FastLed.
