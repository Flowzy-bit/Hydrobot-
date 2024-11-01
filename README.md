This project is for the Autonomous Navigation and Swarming Robotics Integration for a River cleaning robot.

A/N : This project is 70% completed with Swarming Algorithm will be uploaded soon.


Hydrobot's concept has 3 modes of operation: Autonomous, Pattern, and Swarming. To achieve this, there will be a Master Control Unit (MCU) and Slave Units (SU).
It runs on ESP32 for communication and Arduino Nano for motor control and behavior control.
3 ESP32s will be used. One as master and two as slaves.
It utilizes ESP-NOW Protocol to send and receive data and commands.

OM9 is the Master Control Unit for the project. It has a 2.8" TFT Display for display of GPS Coordinates and the status of mode of operation.
  *OM9 sends commands for what type of operation to be executed (Autonomous, Pattern, Swarming) through a push button.
  *OM9 also receive the GPS datas of the 2 Slave Units and display it.
  *Even if one of the slaves stops transmitting data, it displays its last known location.
  
1S9 and 2S9 are the Slave Units. They both possess a Ublox NEO-6M GPS Module.
  *These Slaves send their GPS Data (Latitude and Longitude) to the MCU.
  *They also receive command for what type of operation to execute and command the Arduino NANO through GPIOs.
