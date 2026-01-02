# peltier-educational-kit
This repository presents an educational device kit for control education. The kit is composed of simple and low-cost components, making it easy to replicate. It allows students to implement and experimentally validate control algorithms.

The main idea of the device is to provide a small-scale thermal control system consisting of temperature sensors, a Peltier cell as the actuator, and interface circuits. 
It serves as a practical platform for studying temperature regulation, testing control strategies, and observing the effects of disturbances in a controlled environment.



## Overview
The kit consists of two main components: the interface circuit and the Peltier cell (process). The image below shows the operating principle of the kit:

<img src="images/block%20diagram2.jpg" alt="Kit principle" width="500">

The computer hosts the controller implemented in MATLAB/Simulink. Communication with the hardware interface is achieved via Bluetooth serial communication, which manages the sensors and actuators.



The Peltier cell is housed in a 3D-printed case. Inside the case, in addition to the cell, there are two fans, a power resistor, and two temperature sensors (one digital and one analog). One of the two fans, aided by a heat sink, is used to keep the uncontrolled side of the cell cool. This fan is always on. The other fan can be conveniently used to apply a load disturbance on the controlled side of the cell. The power resistor introduces another source of disturbance on the load but it creates the opposite effect (the fan removes heat, while the resistor generates heat). To improve heat conduction between the cell, the sensors, and the resistor, a thin metal plate has been inserted. The digital temperature sensor used is the DS18B20, a digital sensor that communicates via the 1-Wire protocol. It offers high accuracy, with maximum resolution 0.0625°C. This sensor has virtually no measurement noise. In order to have some measurement noise and therefore to test the capability of a controller to handle this issue, we have introduced another analog temperature sensor, namely, the LM35sensor, by intentionally omitting the filter capacitor. This allows the user to decide which sensor to use, with noise or without noise, so that he/she can face the problem of measurement noise in designing the control strategy, dealing with a typical issue in industrial environments.

<img src="images/3D exploded.jpg" alt="3D exploded" width="500">





## License

This project is released under the  
**Creative Commons Attribution–NonCommercial–ShareAlike 4.0 International License (CC BY-NC-SA 4.0)**.
https://creativecommons.org/licenses/by-nc-sa/4.0/


Use of this project is permitted exclusively for educational, study, and research purposes.  
Any commercial use is strictly prohibited without explicit authorization from the author.
