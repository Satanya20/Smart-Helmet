This project presents a Smart Helmet System designed to enhance the safety of motorcyclists by incorporating real-time accident detection and emergency communication features. The system integrates multiple sensors and a GSM communication module to automatically alert medical services in the event of a crash, providing precise location data for faster response.

## Project Objectives
- Enhance rider safety through automated fall detection.
- Trigger emergency alerts (SMS and phone calls) immediately after an accident.
- Detect helmet usage before ignition to enforce rider protection.
- Provide location-based assistance via GPS for timely help in case of emergencies.

## Technologies & Tools
- **Microcontroller**: ATmega328P 
- **Programming**: Arduino IDE (C/C++)
- **PCB Design**: EasyEDA
- **Simulation Tools**: Proteus (optional)
- **Modules**:
  - SIM900A GSM/GPRS Module
  - FSR Sensor (Force Sensitive Resistor)
  - Vibration Sensor
  - GPS Module (for location tracking)

## Key Features
#### Helmet Wear Detection
An FSR sensor embedded in the helmet padding detects whether the helmet is worn.
The system will only activate if the helmet is properly worn, ensuring accurate functioning.

#### Fall Detection
A vibration sensor continuously monitors the helmet for strong impacts or abrupt movements.
When an accident is detected, a timer ensures it's not a false alarm (e.g., sudden braking).

#### Emergency Alert System
Upon confirmed fall detection:
- The SIM900A module sends an SMS with a predefined message and GPS coordinates.
- It then initiates a phone call to emergency contacts or ambulance services.

#### PCB Design
The entire circuit was implemented on a custom PCB designed in EasyEDA for a compact and robust setup within the helmet.
