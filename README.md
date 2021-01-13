# ESP8266_weather_station

This weather station has an ESP8266-12F in combination with DS18B20 and/or BME280 for sensing. I used a TP4056 module for 18650 Li-Ion battery charging/protection and TPS63900 buck-boost converter, which has incredibly low quiescent current combined with great efficiency at low power consumption (>90% at 10uA).

The station connects to my local Blynk server and sends the data every 5 minutes. During deep-sleep the whole circuit consumes about 25uA, which is fantastic. It should last about a year on one charge, since it connects to my server, sends the data and goes back to sleep in about 0.7s on average (I used Blynk API to reduce the connection time). So the overall power consumption is really low.
