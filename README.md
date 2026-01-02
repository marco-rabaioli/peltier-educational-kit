# peltier-educational-kit
This repository presents an educational device kit for control education. The kit is composed of simple and low-cost components, making it easy to replicate. It allows students to implement and experimentally validate control algorithms.

The main idea of the device is to provide a small-scale thermal control system consisting of temperature sensors, a Peltier cell as the actuator, and interface circuits. 
It serves as a practical platform for studying temperature regulation, testing control strategies, and observing the effects of disturbances in a controlled environment.



## Overview
The kit consists of two main components: the interface circuit and the Peltier cell (process). The image below shows the operating principle of the kit:

<img src="images/block%20diagram2.jpg" alt="Kit principle" width="500">

The computer hosts the controller implemented in MATLAB/Simulink. Communication with the hardware interface is achieved via Bluetooth serial communication, which manages the sensors and actuators.



The Peltier cell is housed in a 3D-printed case. Inside the case, in addition to the cell, there are two fans, a power resistor, and two temperature sensors (one digital and one analog). One of the two fans, aided by a heat sink, is used to keep the uncontrolled side of the cell cool. This fan is always on. The other fan can be conveniently used to apply a load disturbance on the controlled side of the cell. The power resistor introduces another source of disturbance on the load but it creates the opposite effect (the fan removes heat, while the resistor generates heat). To improve heat conduction between the cell, the sensors, and the resistor, a thin metal plate has been inserted. The digital temperature sensor used is the DS18B20, a digital sensor that communicates via the 1-Wire protocol. It offers high accuracy. This sensor has virtually no measurement noise. In order to have some measurement noise and therefore to test the capability of a controller to handle this issue, we have introduced another analog temperature sensor, namely, the LM35 sensor, by intentionally omitting the filter capacitor. This allows the user to decide which sensor to use, with noise or without noise, so that he/she can face the problem of measurement noise in designing the control strategy, dealing with a typical issue in industrial environments.

The image below shows the exploded 3D assembly in which all the components can be seen:

<img src="images/3D exploded.jpg" alt="3D exploded" width="500">

<ol>
  <li> Support </li>
  <li> Fan </li>
  <li> Power resistor </li>
  <li> Peltier cell </li>
  <li> Internal support </li>
  <li> Temperature sensors </li>
  <li> Heat-sink </li>
  <li> Housing </li>
</ol>

## Interface circuit

The purpose of the circuit is to interface sensors and actuators with a personal computer through serial wireless communication based on Bluetooth connectivity. We have chosen to use wireless communication between the PC and the circuit in order to protect the PC in case of a circuit failure, such as a short circuit. In addition, wireless communication is more user-friendly as it allows the user to achieve the same performance of a wired connection with more flexibility. The circuit is powered by 12V DC supply obtained from a standard power adapter. The working principle of the interface circuit, which is managed by an Arduino Nano, is shown in Figure below:

<img src="images/block diagram1.jpg" alt="3D exploded" width="500">

The digital logical signal is shown in blue, the power signal is in red, while the analog signal is represented in green. A simple communication protocol has been implemented into the firmware. By sending strings properly formatted, through serial communication, it is possible to exchange data between the software and the process. In this way, it is possible to pilot the actuators and read the sensors. With this simple communication protocol, it is possible to use the interface circuit with many softwares such as Java, Python or Matlab/Simulink. In general, any software capable of communicating with serial commands is able to interface with the circuit. In particular, with Matlab/Simulink the user does not need to install any library because, for the serial communication, native commands are used. To simplify the communication, the protocol has been implemented into appropriate Matlab functions that write and send strings to Arduino. So the user just needs to import the Matlab functions into their own workspace and call them when he/she needs to exchange data with the process. There is no need to know the communication protocol. The interface circuit is managed by an Arduino Nano. The board receives serial commands from the software and then controls the actuators. The Peltier cell is powered by a voltage modulated with PWM at high frequencies. To pilot the cell a MOSFET has been used. To achieve symmetrical control actions (heating and cooling) on the same side of the cell, an H-bridge has been implemented in the circuit using a mechanical relay. The relay inverts the voltage modulated by the MOSFET. Another MOSFET and a BJT allow the control of the power resistor and of the fan located on the controlled side. The power resistor can also be controlled with a PWM signal in order to have control of load disturbance (in order to test different scenarios). Arduino manages also the digital temperature sensor applied to the cell. When the software requests a temperature reading through a serial command, Arduino reads the sensor and sends the data back to the software running on the PC. The analog temperature sensor is read using an analog input. The firmware implements the factor conversion in order to convert the voltage read into a temperature value in Celsius, so the user does not need to handle this conversion. For serial Bluetooth communication, the HC-05 module has been used. This module is connected to Arduino in parallel at the RX and TX serial pins. If it is necessary to reprogram the Arduino firmware, the HC-05 module must be disconnected. In order to do it easily, a button has been added. It is necessary to press it when the firmware is uploaded through the Arduino IDE.

The circuit board is shown in figure below:

<img src="images/interface circuit.jpg" alt="3D exploded" width="500">


## License

This project is released under the  
**Creative Commons Attribution–NonCommercial–ShareAlike 4.0 International License (CC BY-NC-SA 4.0)**.
https://creativecommons.org/licenses/by-nc-sa/4.0/


Use of this project is permitted exclusively for educational, study, and research purposes.  
Any commercial use is strictly prohibited without explicit authorization from the author.
