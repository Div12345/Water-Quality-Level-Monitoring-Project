# Apogee-Project-2019
Smart Water Level and Quality Monitoring Framework

This is an automation project intended to reduce human errors and expenditure of human resources in Water System Management.

The components used are - 
1. NodeMCU x 3
2. Ultrasound Sensor HCSR-04 x 3
3. pH Sensor x 1
4. Temperature Sensor DS18B20 x 1
5. Turbidity Sensor x 1 
6. Relay
7. Submersible water pump x 3

The framework - 
The prototype shows 3 tanks each tank consisting of a ultrasound sensor to detect the water level in the tank. Further one tank has the temperature sensor, an other tank has the pH sensor and the other has the Turbidity sensor.
The tank with the temperature sensor and the one with the pH sensor are connected to a single NodeMCU, to display the ability of single NodeMCU to handle more than a single tank, corresponding to the multiple tanks present in a single hostel and the one with the Turbidity sensor is connected to a NodeMCU, representing a tank in another hostel, thus exhibiting the ability of the framework to manage multiple groups of tanks concurrently.
One NodeMCU is connected to a relay which controls the submersible water pumps, automatically switching them ON or OFF depending on the water level in the corresponding tank.
The NodeMCUs are connected to a WiFi network through which they transfer their data to the Adafruit IO server.
A mobile phone using the app MQTT Dash makes use of the MQTT communication protocol to recieve and send data to the server. 
Thus the instantaneous water level data as well as the pump's state is available to the supervisor.
The App also allows an override option in case the supervisor wants to control the pumps.
