ServoJ Example
==============

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example will use the **servoJ** command to move the robot, where incremental changes are made to the base and
shoulder joint continuously in a 500Hz control loop for 2 seconds.

You can find the source code of this example under :file:`examples/cpp/servoj_example.cpp`, if you compiled
ur_rtde with examples you can run this example from the *bin* folder. If you want to run the python example
navigate to :file:`examples/py/` and run :bash:`python3 servoj_example.py`.

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
     double velocity = 0.5;
     double acceleration = 0.5;
     double dt = 1.0/500; // 2ms
     double lookahead_time = 0.1;
     double gain = 300;
     std::vector<double> joint_q = {-1.54, -1.83, -2.28, -0.59, 1.60, 0.023};

     // Move to initial joint position with a regular moveJ
     rtde_control.moveJ(joint_q);

     // Execute 500Hz control loop for 2 seconds, each cycle is ~2ms
     for (unsigned int i=0; i<1000; i++)
     {
       steady_clock::time_point t_start = rtde_control.initPeriod();
       rtde_control.servoJ(joint_q, velocity, acceleration, dt, lookahead_time, gain);
       joint_q[0] += 0.001;
       joint_q[1] += 0.001;
       rtde_control.waitPeriod(t_start);
     }

     rtde_control.servoStop();
     rtde_control.stopScript();

     return 0;
   }

Python:

.. code-block:: python

   import rtde_control

   rtde_c = rtde_control.RTDEControlInterface("127.0.0.1")

   # Parameters
   velocity = 0.5
   acceleration = 0.5
   dt = 1.0/500  # 2ms
   lookahead_time = 0.1
   gain = 300
   joint_q = [-1.54, -1.83, -2.28, -0.59, 1.60, 0.023]

   # Move to initial joint position with a regular moveJ
   rtde_c.moveJ(joint_q)

   # Execute 500Hz control loop for 2 seconds, each cycle is 2ms
   for i in range(1000):
       t_start = rtde_c.initPeriod()
       rtde_c.servoJ(joint_q, velocity, acceleration, dt, lookahead_time, gain)
       joint_q[0] += 0.001
       joint_q[1] += 0.001
       rtde_c.waitPeriod(t_start)

   rtde_c.servoStop()
   rtde_c.stopScript()


.. note::
   Remember that to allow for a fast control rate when servoing, the joint positions must be close to each other e.g.
   (dense trajectory). If the robot is not reaching the target fast enough try to increase the acceleration or the
   gain parameter.

Intended movement:

.. image:: ../_static/servoj_example.gif

