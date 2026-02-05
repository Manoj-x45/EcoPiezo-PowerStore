# EcoPiezo PowerStore 

**A Smart IoT-Enabled Footwear Energy Harvesting System**

##  Abstract
EcoPiezo PowerStore is an innovative energy harvesting system that generates electricity from footsteps using piezoelectric technology embedded in shoes. This project aims to provide a sustainable and portable power source for charging mobile devices. As the user walks, the mechanical pressure exerted on the piezoelectric sensors converts kinetic energy into electrical energy. [cite_start]The system is integrated with IoT monitoring to track voltage generation and step counts in real-time[cite: 2136].

##  Features
* [cite_start]**Energy Harvesting:** Converts foot impact (heel and toe) into DC voltage using PZT discs[cite: 2388].
* [cite_start]**IoT Monitoring:** ESP32 transmits real-time data to Google Firebase[cite: 2412].
* [cite_start]**Mobile Dashboard:** Android app (MIT App Inventor) displays voltage, step count, and battery status[cite: 2422].
* [cite_start]**RFID Authentication:** Secure access control to enable/disable the charging mechanism[cite: 2743].
* [cite_start]**Smart Storage:** Efficiently charges a Li-Ion battery (18650) via a rectification and smoothing circuit[cite: 2391].

##  Hardware Requirements
* [cite_start]**Microcontrollers:** Arduino Uno (Sensor Node), ESP32 DEVKIT V1 (IoT Gateway)[cite: 2701, 2724].
* [cite_start]**Sensors:** Piezoelectric Transducers (PZT Discs)[cite: 2758].
* [cite_start]**Modules:** RFID RC522, Relay Module, 16x2 LCD (I2C)[cite: 2741, 2799, 2774].
* [cite_start]**Power:** 18650 Li-ion Battery, Voltage Regulator, Bridge Rectifier (1N4007 diodes)[cite: 2788, 3014].

##  Software Stack
* **Firmware:** C++ (Arduino IDE)
* [cite_start]**Cloud Database:** Google Firebase Realtime Database[cite: 2851].
* [cite_start]**Mobile App:** MIT App Inventor[cite: 2872].

##  How It Works
1.  **Harvesting:** Piezo sensors in the shoe sole generate AC voltage upon impact.
2.  [cite_start]**Rectification:** A bridge rectifier converts AC to DC, and a capacitor smooths the ripples[cite: 3026].
3.  **Processing:** Arduino reads the voltage, handles RFID authentication, and displays status on the LCD.
4.  [cite_start]**IoT Transmission:** Arduino sends data via Serial UART to the ESP32, which uploads it to Firebase[cite: 2937].
5.  [cite_start]**Visualization:** The Mobile App listens to Firebase updates and shows live stats to the user[cite: 3001].


##  Author
 **Manoj S**