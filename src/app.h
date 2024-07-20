#ifndef APP_H
#define APP_H

// From ultrasonic_sensor.cpp
void ultrasonic_setup();
float ultrasonic_get_distance();

// From camera.cpp 

// From connection.cpp
void checkWifiConnection();
void checkMqttConnection();

#endif
