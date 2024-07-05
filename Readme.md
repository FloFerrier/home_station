# WEATHER_SENSORS PROJECT
## Summary
This project is an embedded platform to gather physical measurements in a smart home like :
- Temperature
- Pressure
- Humidity
The device which get data is a STM32 microcontroller and a gateway with a raspberry pi which store data on a database.
## Organization
This project is a monolithic repository which contains two distincts source code on subfolder :
- firmware : C source code to flash on a STM32 microcontroller.  
- service : python script to deploy on a raspberry pi device.
Each subfolder contains their own development environment and their own tools.
These are separate project that communicate through a custom protocol.
