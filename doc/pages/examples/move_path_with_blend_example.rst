MoveL Path With Blending Example
================================

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example will use the **moveL** command with a path, where each joint pose in the path has a defined velocity,
acceleration and blend. The joint poses in the path are defined by a 9-dimensional vector, where the first six
values constitutes the joint pose, followed by the last three values *velocity*, *acceleration* and *blend*.

You can find the source code of this example under :file:`examples/cpp/movel_path_with_blend_example.cpp`, if you compiled
ur_rtde with examples you can run this example from the *bin* folder. If you want to run the python example
navigate to :file:`examples/py/` and run :bash:`python3 movel_path_with_blend_example.py`.

C++:

.. code-block:: c++

   #include <ur_rtde/rtde_control_interface.h>

   using namespace ur_rtde;

   int main(int argc, char* argv[])
   {
     RTDEControlInterface rtde_control("127.0.0.1");

     double velocity = 0.5;
     double acceleration = 0.5;
     double blend_1 = 0.0;
     double blend_2 = 0.02;
     double blend_3 = 0.0;
     std::vector<double> path_pose1 = {-0.143, -0.435, 0.20, -0.001, 3.12, 0.04, velocity, acceleration, blend_1};
     std::vector<double> path_pose2 = {-0.143, -0.51, 0.21, -0.001, 3.12, 0.04, velocity, acceleration, blend_2};
     std::vector<double> path_pose3 = {-0.32, -0.61, 0.31, -0.001, 3.12, 0.04, velocity, acceleration, blend_3};

     std::vector<std::vector<double>> path;
     path.push_back(path_pose1);
     path.push_back(path_pose2);
     path.push_back(path_pose3);

     // Send a linear path with blending in between - (currently uses separate script)
     rtde_control.moveL(path);
     rtde_control.stopScript();

     return 0;
   }

Python:

.. code-block:: python

   import rtde_control

   rtde_c = rtde_control.RTDEControlInterface("127.0.0.1")

   velocity = 0.5
   acceleration = 0.5
   blend_1 = 0.0
   blend_2 = 0.02
   blend_3 = 0.0
   path_pose1 = [-0.143, -0.435, 0.20, -0.001, 3.12, 0.04, velocity, acceleration, blend_1]
   path_pose2 = [-0.143, -0.51, 0.21, -0.001, 3.12, 0.04, velocity, acceleration, blend_2]
   path_pose3 = [-0.32, -0.61, 0.31, -0.001, 3.12, 0.04, velocity, acceleration, blend_3]
   path = [path_pose1, path_pose2, path_pose3]

   # Send a linear path with blending in between - (currently uses separate script)
   rtde_c.moveL(path)
   rtde_c.stopScript()


Intended movement:

.. image:: ../_static/movel_path_blend.gif

