from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('monitor_node'),
        'config',
        'threshold_params.yaml'
    )

    return LaunchDescription([
        Node(
            package='sensor_node',
            executable='sensor_node',
            name='sensor_node',
            output='screen'
        ),
        Node(
            package='monitor_node',
            executable='monitor_node',
            name='monitor_node',
            output='screen',
            parameters=[config]
        )
    ])