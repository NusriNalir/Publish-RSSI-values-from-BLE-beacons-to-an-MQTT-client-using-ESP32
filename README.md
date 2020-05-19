# Publish-RSSI-values-from-BLE-beacons-to-an-MQTT-client-using-ESP32
In this arduino code we use an ESP32 to scan and identify the RSSI values of BLE signals emited by Bluetooth beacons. Then this scanned values are published to an MQTT client.

This code was written as a part of data collection for the project "Indoor localization using BLE beacons". the RSSI values published by this code can be read by a python script and can create a data set.  then this data set was used to train a machine learning model and accuratly localize in an indoor environment.
