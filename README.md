# Cooler Guard: IoT Medical Refrigerator Monitor

## Introduction
The objective of this project is to build an active, internet-connected alarm system for a medicine or vaccine refrigerator. Medicines like vaccines must be stored continuously between 2°C and 8°C to be safe. If the temperature goes out of this range, the medicine can degrade. This upgraded Cooler Guard project uses an ESP32 microcontroller to monitor the temperature. It provides local alerts using an LCD, LEDs, and a buzzer, and also connects to Wi-Fi to send instant emergency notifications to a doctor's phone via a Telegram bot.

## Components List
To simulate this IoT project in Wokwi, the following components were used:

*   **ESP32 Microcontroller:** The main brain, chosen because it has a built-in Wi-Fi chip.
*   **NTC Temperature Sensor:** The analog probe used to detect heat changes inside the fridge.
*   **16x2 LCD Screen:** To show the current system status and temperature locally.
*   **Piezo Buzzer:** An active buzzer to make a loud sound if the temperature is unsafe.
*   **Red and Green LEDs:** To show visual status alerts (Green = Safe, Red = Danger).
*   **Breadboard and Jumper Wires:** For connecting all the parts.
*   **Resistors:** Two 220-ohm resistors for the LEDs.

## Wiring Connections

**Power Rails:**
*   Connecting the 3V3 pin from the ESP32 to the red (+) rail on the breadboard.
*   Connecting the GND pin to the black/blue (-) rail.

**NTC Temperature Sensor:**
*   **VCC** connects to 3V3 (+).
*   **GND** connects to GND (-).
*   **OUT (Signal)** connects to Analog Pin 35 on the ESP32.

**LED Indicators and Buzzer:**
*   **Green LED:** Negative leg to GND (-). Positive leg connects through a 220-ohm resistor to Pin 12.
*   **Red LED:** Negative leg to GND (-). Positive leg connects through a 220-ohm resistor to Pin 14.
*   **Piezo Buzzer:** Negative leg to GND (-). Positive leg connects directly to Pin 27.

**16x2 LCD Screen:**
*   **GND**, **V0**, and **RW** to GND (-).
*   **VCC** and **A** to 3V3 (+).
*   **K** to GND (-).
*   **RS** to Pin 22, and **E** to Pin 23.
*   **D4, D5, D6, D7** to Pins 5, 18, 19, and 21 on the ESP32.

   <img width="834" height="633" alt="image" src="https://github.com/user-attachments/assets/e84c9297-e5b0-4229-80e2-de53bce1457e" />


## Mathematical Conversion Logic
The ESP32 gets a raw analog reading from the NTC sensor. Because it is a thermistor, the relationship between temperature and resistance is not a simple straight line. We use the Steinhart-Hart equation (using a Beta parameter of 3950) to calculate the exact Celsius temperature.

$$
Temperature (^\circ C) = \frac{1}{\left(\frac{1}{\beta}\right) \times \ln\left(\frac{4095}{\text{AnalogValue}} - 1\right) + \frac{1}{298.15}} - 273.15
$$

## System Logic and IoT Integration
The system checks the temperature every second. To prevent sending thousands of spam messages, the code uses "Edge Detection" (state tracking). It remembers if it is currently in danger, and only sends a Telegram message when the state changes.

| Condition | Local Output (Hardware) | IoT Action (Telegram) |
| :--- | :--- | :--- |
| **Between 2°C and 8°C** | Green LED ON, Red OFF, Buzzer OFF. LCD: "SAFE" | Sends an "All Clear / Safe" message ONLY if the fridge was previously in danger. |
| **Under 2°C or Over 8°C** | Green LED OFF, Red ON, Buzzer ON (1000Hz). LCD: "SPOIL!" | Sends an emergency "SPOIL ALERT" text to the doctor's phone. Locks to prevent spam. |

Displaying current fridge condition on LCD screen:


<img width="595" height="395" alt="image" src="https://github.com/user-attachments/assets/b1990809-9c64-4ea5-923a-1e62ee81451c" />




Receiving alarm notifications through a telegram bot:

<img width="420" height="716" alt="image" src="https://github.com/user-attachments/assets/9d7f41cc-bfb4-455a-8617-f3f1c68c0f5e" />



## Conclusion
The Cooler Guard project was successfully accomplished as a full IoT device. The system was simulated in Wokwi, where it successfully connected to a virtual Wi-Fi gateway and used HTTPS REST APIs to push real-time alerts to a mobile phone via Telegram. This proves that cheap, accessible hardware can be used to build professional-grade remote monitoring tools to protect sensitive medical supplies.
