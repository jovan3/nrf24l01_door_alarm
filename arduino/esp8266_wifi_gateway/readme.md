# ESP8266 WiFi nrf24l01 gateway

Connects to a WiFi AP, sends data received from nrf24l01 devices to an MQTT broker.

## Building

Requires espmake. The `libs` directory contains some required libraries. Some subdirs in `libs/RF24` needed to be deleted, because espmake tries to compile AVR code.

Run `espmake` with a the following variables set: mqtt-port, mqtt-broker, mqtt-user, mqtt-password, ssid, apsk (preshared key). For example:

```
espmake ssid=wifissid apsk=wifipresharedkey mqtt-broker=192.168.1.123 mqtt-port=1883 mqtt-user=someuser mqtt-password=foobar123 VERBOSE=1
```

## Installing

Run `espmake flash`
