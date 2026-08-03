SpeedJ Example
==============

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example will use the **speedJ** command to move the robot, where the first 2 joints are speeding continuously
in a 500Hz control loop for 2 seconds.

You can find the source code of this example under :file:`examples/cpp/speedj_example.cpp`, if you compiled
ur_rtde with examples you can run this example from the *bin* folder. If you want to run the python example
navigate to :file:`examples/py/` and run :bash:`python3 speedj_example.py`.

C++:

.. code-block:: c++

   #include <ur_rtde/rtde_control_interface.h>
   #include <thread>
   #include <chrono>

   using namespace ur_rtde;
   using namespace std::chrono;

   int main(int argc, char* argv[])
   {
     RTDEControlInterface rtde_control("127.0.0.1");

     // Parameters
     double acceleration = 0.5;
     double dt = 1.0/500; // 2ms
     std::vector<double> joint_q = {-1.54, -1.83, -2.28, -0.59, 1.60, 0.023};
     std::vector<double> joint_speed = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

     // Move to initial joint position with a regular moveJ
     rtde_control.moveJ(joint_q);

     // Execute 500Hz control loop for 2 seconds, each cycle is ~2ms
     for (unsigned int i=0; i<1000; i++)
     {
       steady_clock::time_point t_start = rtde_control.initPeriod();
       rtde_control.speedJ(joint_speed, acceleration, dt);
       joint_speed[0] += 0.0005;
       joint_speed[1] += 0.0005;
       rtde_control.waitPeriod(t_start);
     }

     rtde_control.speedStop();
     rtde_control.stopScript();

     return 0;
   }

Python:

.. code-block:: python

   import rtde_control

   rtde_c = rtde_control.RTDEControlInterface("127.0.0.1")

   # Parameters
   acceleration = 0.5
   dt = 1.0/500  # 2ms
   joint_q = [-1.54, -1.83, -2.28, -0.59, 1.60, 0.023]
   joint_speed = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

   # Move to initial joint position with a regular moveJ
   rtde_c.moveJ(joint_q)

   # Execute 500Hz control loop for 2 seconds, each cycle is 2ms
   for i in range(1000):
       t_start = rtde_c.initPeriod()
       rtde_c.speedJ(joint_speed, acceleration, dt)
       joint_speed[0] += 0.0005
       joint_speed[1] += 0.0005
       rtde_c.waitPeriod(t_start)

   rtde_c.speedStop()
   rtde_c.stopScript()


Intended movement:

.. image:: ../_static/speedj_example.gif

