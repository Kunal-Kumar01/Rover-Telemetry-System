# Rover Telemetry System

A mini project for Adelaide Rover Team. Its a rover telemetry and safety monitoring system. Utilizes sensors to collect data and checks readings against the safety thresholds. 

## Sensor Node
Simulates the sensor readings similar to of an actual rover, including battery level, motor temperature, and wheel speed. Publishes the readings to the monitor node every half a second. Hosts the emergency stop and reset services. Halts publishing when emergency stop is triggered and resumes only when manually reset. 

## Monitor Node
Subscribes to the rover telemetry topic. Monitors sensor readings corresponding to the safety thresholds. Logs the alert levels, including NOMINAL, WARNING, and CRITICAL. Automatically calls the emergency stop service whenever the critical threshold is breached. 

## Communication
- `/rover_telemetry` — topic, sensor_node publishes, monitor_node subscribes
- `/emergency_stop` — service, monitor_node calls, sensor_node hosts
- `/reset_rover` — service, operator calls manually, sensor_node hosts

## Custom Interfaces

### Custom Message
- `RoverTelemetry.msg` — bundles battery level, motor temperature, and wheel speed into a single telemetry message

### Custom Service
- `EmergencyStop.srv` — request contains a reason string, response contains a boolean acknowledgement

## Prerequisites
- ROS2 Humble
- Colcon
- Ubuntu 22.04

## Setup

```bash
git clone https://github.com/Kunal-Kumar01/Rover-Telemetry-System.git

cd Rover-Telemetry-System

source /opt/ros/humble/setup.bash

colcon build

source install/setup.bash
```

## Running the Nodes

### Manual 

Open the terminal and run the command to start the sensor node: 

```bash
ros2 run sensor_node sensor_node
```

Open another terminal and run the command to start the monitor node: 

```bash
ros2 run monitor_node monitor_node
```

### Using Launch file

Instead of manually launching each node separately, one can launch the both nodes simultaneously in one go. Open the terminal and run the command:

```bash 
ros2 launch monitor_node rover_launch.py
```

## Setting Parameters

There is an option to provide parameters for the safety level thresholds, if not provided, the thresholds fall back to the default level. 

There are 2 ways to provide the parameters:

1. If the nodes are launched through launch file, then parameters can be set in the `threshold_params.yaml` present in the `monitor_node/config` directory. 

2. Once the nodes are launched and the safety thresholds are to be changed mid-way, then it can be done through the terminal command.
Example: changing the battery threshold:
```bash
ros2 param set /monitor_node battery_warning 50.0
```

## Checking Parameters

To view current threshold values:
```bash
ros2 param list /monitor_node
ros2 param get /monitor_node battery_critical
```


## Emergency Stop

If the sensor readings suggest that the rover has crossed the safety levels, then the emergency stop service is automatically called by the monitor node and the rover comes to a halt. 

To restart the rover, the user has to run the following command in the terminal:

```bash
ros2 service call /reset_rover std_srvs/srv/Trigger
```




