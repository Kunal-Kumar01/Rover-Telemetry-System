# Rover Telemetry System

A mini project for Adelaide Rover Team.

## Sensor Node
Simulates the sensor readings of an actual rover, inlcuding battery level, motor temperature, and wheel speed. Publishes the readings to the monitor node every half a second. Resets the system when the sensor readings reach critical levels whenever emergency stop service is triggered.

## Monitor Node
Subscribes to the rover telemetry topic. Monitors sensor readings corresponding to the safety thresholds. Logs the alert levels, including NOMINAL, WARNING, and CRITICAL. Automatically calls the emergency stop service whenever the critical threshold is breached. 

## Setup

```bash
git clone https://github.com/Kunal-Kumar01/Rover-Telemetry-System.git

cd Rover-Telemetry-System

source /opt/ros/humble/setup.bash

colcon build

source install/setup.bash
```
