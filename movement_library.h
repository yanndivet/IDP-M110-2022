#ifndef MOVEMENT_LIBRARY_H
#define MOVEMENT_LIBRARY_H
#include <Arduino.h>
#include <my_library.h>

void initial_movement() {
	move_forward();
	turn_right();
}

void block_drop(bool magnetic, int right_counter) {
    // if block is magnetic, drop it in red region
    int counter_threshold;

    if (magnetic == true) {
        counter_threshold = 4;
    }

    else {
        counter_threshold = 6;
    }

    if (right_counter >= counter_threshold) {
        turn_right(90);
        move_forward(100);
        move_backward(100);
        turn_left(90);
    }

    // if dropped in red region, move in front of green region to start next mission
    // perhaps don't always run this, especially at the end? 
    if (magnetic = true) {
        move_forward(200);
    }

    // if block isn't magnetic, drop it in green region
    else {
        move_backward(200);
    }

}

bool magnetic_detection (double voltage, double upper_voltage_thresh = 2.4, double lower_voltage_thresh = 2.1) {
    // Return whether block is magnetic or not
    bool magnet_inside; 
    if (voltage > upper_voltage_thresh) {
        magnet_inside = true;
    }
    else if (voltage > lower_voltage_thresh) {
        magnet_inside = false;
    }
    else {
        Serial.println("Voltage is out of range");
    }
    return magnet_inside;
}

#endif
