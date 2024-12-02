# Smart Home Project

## Introduction
This project is a smart home prototype that demonstrates automation and multi-task control using Arduino and FreeRTOS. The system integrates multiple features, including automatic door control, emergency handling, and dynamic lighting adjustment.

## Features
- **Automatic Door Control**: Detects and manages door open/close state based on specific triggers.
- **Emergency Button**: High-priority task monitoring for emergency responses.
- **State Switching**: Automatically toggles between open/close states based on environmental and user-defined triggers.
- **Dynamic LED Brightness Adjustment**: Adjusts LED brightness based on the ambient light using a photocell sensor.

## Hardware Components
- **Arduino Board**: Main microcontroller.
- **Photocell Sensor**: Detects ambient light levels.
- **LED**: Displays dynamic brightness adjustments.
- **Buttons**: Includes emergency and toggle buttons.
- **Motor/Servo**: Controls door movement.

## Software
- **Operating System**: FreeRTOS for task scheduling and multi-tasking.
- **Programming Language**: C/C++ for Arduino development.
- **Libraries Used**:
  - FreeRTOS
  - Arduino core libraries for GPIO control
