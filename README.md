# rgb-lcu
RGB-Laser Control Unit

This software is intended to control a red-green-blue diode laser module via an Arduino Uno.
The module's power is controlled via the Arduino as well as the red, green and blue laser diodes intensities via PWM.

The Arduino itself is controlled via the serial monitor, using the Arduino Commandline library (Commandline v2.1.0 by Bas Stottelaar (basilfx)) https://github.com/basilfx/Arduino-CommandLine

This software is written by Philippe Kirsch, 01/04/2017, Version 0.12
The used laser unit has a 100mW red laser diode, a 50mW green laser diode an 150mW blue laser diode.
