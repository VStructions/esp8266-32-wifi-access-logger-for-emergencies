# ESP8266/32 WiFi access logger for emergencies (Proof Of Concept)

Log WiFi devices' MAC address in case of an emergency for aftermath evaluation, with an Espressif microprocessor. This requires the device to be logged, 
to connect to the software access point implemented on an ESP8266 or ESP32. 

This is a fully working project aimed to empower local authorities in the capacity of faster information gathering. In a human life threatening situation, people could connect 
once to the WiFI AP with their smartphones, to log their presence in the area. Then, this information can be gathered and assessed in order to find out whether someone is left 
behind. 
The cellular network can accomplish this task much better because it's subscribers are already connected and logged by the providing company. But this open source project enables 
anyone with an ESP8266 or ESP32 to contribute voluntarily, although for the contribution to matter, this project's purpose would have to gain popularity as people would have 
no reason to join this probably "weird in their eyes" WiFi network. Also, it is very unlikely of one that has no idea about a system like this, to check the local WiFi 
networks at times of emergency.

This project's functionality can be extended for security or less urgent matters. Like logging the presence of an employee in the workspace. In any case, using ESP 
microprocessor developer boards for this task is one of the cheapest and simplest ways to cover area, just by deploying many of them.

# Installation
This project was setup with the PlatformIO IDE for VS Code. If you are using:

* PlatformIO, simply clone this repository, open it as a PlatformIO project and build it for your ESP8266 / ESP32 device

* This project is built using the Arduino framework, so it should work with the Arduino IDE, but I haven't tested it. 

# Operation

* Check the serial output for the AP IP (should be 192.168.4.1) and other operation related messages
* Connect to the ESP with WiFi
* Go to AP IP using a browser to display the MAC archive
* Go to AP IP/clearArchive using a browser to clear the MAC archive

There is no security because this is a proof of concept, anyone with knowledge of the /clearArchive page can clear the archive.

# Tested on:

* ESP8266 NodeMCU 1.0 (ESP-12E Module)
* ESP32   NodeMCU-32S (ESP-WROOM-32 Module)

#

This software is under the MIT open source license
