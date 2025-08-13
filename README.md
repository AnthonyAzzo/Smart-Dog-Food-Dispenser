# Smart Dog Food Dispenser

An Arduino-based pet food dispenser with manual and scheduled feeding times, built with an A4988 stepper motor driver and a DS3231 Real-Time Clock. This project was created to learn about hardware integration and C++ programming with Arduino.

## Features

* **Timed Manual Dispensing:** Press a button to dispense food for a specific, configurable duration.
* **Scheduled Feedings:** Automatically dispenses food at set times in the morning and evening.
* **Adjustable Speed:** Buttons to increase or decrease the motor's maximum speed.
* **Smooth Operation:** Uses the AccelStepper library for smooth acceleration and deceleration of the motor.

## Hardware Components

* Arduino Nano
* A4988 Stepper Motor Driver
* NEMA 17 Stepper Motor
* DS3231 Real-Time Clock (RTC) Module
* 3x Push Buttons
* 12V DC Power Supply

## Hardware Design ⚡️

The circuit schematic and PCB layout were designed in KiCad. You can view the PDFs below, and all original design files are available in the `/hardware` folder.

* **[View Schematic PDF](hardware/Dog_food_dispense_Schematic.pdf)**
* **[View PCB Layout PDF](hardware/Dog_food_dispense_PCB-F_Cu.pdf)**
* **[View PCB Layout PDF](hardware/Dog_food_dispense_PCB-Edge_Cuts.pdf)**
* **[View PCB Layout PDF](hardware/Dog_food_dispense_PCB-F_Silkscreen.pdf)**
    
## Libraries Used

* [`AccelStepper`](https://www.airspayce.com/mikem/arduino/AccelStepper/) by Mike McCauley
* [`RTClib`](https://github.com/adafruit/RTClib) by Adafruit
