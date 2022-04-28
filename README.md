# m5stack-UWB-test
Some basic tests for m5stack uwb

## About The Project

M5Core2 is the second generation core device in the M5Stack development kit series, which further enhances the functions of the original generation of cores.

UWB is a unit that employs nanosecond pulses to find objects and describe position and orientation. Decawave's DW1000 transceiver module is used in the Ai-ThinkerBU01 design. The inbuilt STM32 chip has a 10cm positional precision and allows AT command control. Indoor wireless asset tracking/range finding works by triangulating the location of the base station/s and tag (the base station resolves the position information and outputs it to the tag).

### Built With

* [Arduino](https://www.arduino.cc/)

## Getting Started

### Software Settings
Please follow this manual for environmental setup.

[Arduino IDE environment - M5Core](https://docs.m5stack.com/en/quick_start/m5core/arduino)

### Hardware 

The following diagram is M5Core2 pin diagram.

<img src="https://raw.githubusercontent.com/kuoyaoming/m5stack-UWB-test/main/Document/68747470733a2f2f7374617469632d63646e2e6d35737461636b2e636f6d2f7265736f757263652f646f63732f7374617469632f6173736574732f696d672f70726f647563745f706963732f636f72652f636f7265322f636f7265325f6d6275732e77656270.webp" width="500">

The following figure is the DW1000 pin diagram.

<img src="https://cdn.shopify.com/s/files/1/0056/7689/2250/products/8_298da73c-378c-4c53-bb40-4863337aade3_1200x1200.jpg?v=1608513606" width="500">

Use DuPont connectors to connect the following four pairs of contacts:

* GND to GND
* 5V to 5V
* DW1000 RXD to M5Core2 TXD2
* DW1000 TXD to M5Core2 RXD2

## Usage

Download each of the three files (`Distance measurement/M5andDW1000_distance_tag`,`Distance measurement/M5andDW1000_distance_anchor_0`,`Distance measurement/M5andDW1000_distance_anchor_1`) to the three sets of set-up machines

## Result

<img src="https://github.com/kuoyaoming/m5stack-UWB-test/blob/main/Document/Picture1_0.png" width="500">

<img src="https://github.com/kuoyaoming/m5stack-UWB-test/blob/main/Document/Picture2_0.png" width="500">

The test results showed that the device maintained a high level of accuracy over long distances.


