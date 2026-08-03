.. _move-asynchronous-example:

Move Asynchronous Example
=========================

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example will perform two asynchronous movements, first one by **moveJ**, followed by a movement with **moveL**.
Both movements are stopped before reaching the targets with **stopJ** and **stopL** respectively.

C++:

.. code-block:: c++

   #include <ur_rtde/rtde_control_interface.h>
   #include <ur_rtde/rtde_receive_interface.h>

   #include <thread>
   #include <chrono>

   using namespace ur_rtde;
   using namespace std::chrono;

   int main(int argc, char* argv[])
   {
     RTDEControlInterface rtde_control("127.0.0.1");
     RTDEReceiveInterface rtde_receive("127.0.0.1");
     std::vector<double> init_q = rtde_receive.getActualQ();

     // Target in the robot base
     std::vector<double> new_q = init_q;
     new_q[0] += 0.2;

     /**
      * Move asynchronously in joint space to new_q, we specify asynchronous behavior by setting the async parameter to
      * 'true'. Try to set the async parameter to 'false' to observe a default synchronous movement, which cannot be
      * stopped by the stopJ function due to the blocking behaviour.
      */
     rtde_control.moveJ(new_q, 1.05, 1.4, true);
     std::this_thread::sleep_for(std::chrono::milliseconds(200));
     // Stop the movement before it reaches new_q
     rtde_control.stopJ(0.5);

     // Target 10 cm up in the Z-Axis of the TCP
     std::vector<double> target = rtde_receive.getActualTCPPose();
     target[2] += 0.10;

     /**
      * Move asynchronously in cartesian space to target, we specify asynchronous behavior by setting the async parameter
      * to 'true'. Try to set the async parameter to 'false' to observe a default synchronous movement, which cannot be
      * stopped by the stopL function due to the blocking behaviour.
      */
     rtde_control.moveL(target, 0.25, 0.5, true);
     std::this_thread::sleep_for(std::chrono::milliseconds(200));
     // Stop the movement before it reaches target
     rtde_control.stopL(0.5);

     // Move to initial joint position with a regular moveJ
     rtde_control.moveJ(init_q);

     // Stop the RTDE control script
     rtde_control.stopScript();
     return 0;
   }

Python:

.. code-block:: python

   import rtde_control
   import rtde_receive
   import time

   rtde_c = rtde_control.RTDEControlInterface("127.0.0.1")
   rtde_r = rtde_receive.RTDEReceiveInterface("127.0.0.1")
   init_q = rtde_r.getActualQ()

   # Target in the robot base
   new_q = init_q[:]
   new_q[0] += 0.20

   # Move asynchronously in joint space to new_q, we specify asynchronous behavior by setting the async parameter to
   # 'True'. Try to set the async parameter to 'False' to observe a default synchronous movement, which cannot be stopped
   # by the stopJ function due to the blocking behaviour.
   rtde_c.moveJ(new_q, 1.05, 1.4, True)
   time.sleep(0.2)
   # Stop the movement before it reaches new_q
   rtde_c.stopJ(0.5)

   # Target in the Z-Axis of the TCP
   target = rtde_r.getActualTCPPose()
   target[2] += 0.10

   # Move asynchronously in cartesian space to target, we specify asynchronous behavior by setting the async parameter to
   # 'True'. Try to set the async parameter to 'False' to observe a default synchronous movement, which cannot be stopped
   # by the stopL function due to the blocking behaviour.
   rtde_c.moveL(target, 0.25, 0.5, True)
   time.sleep(0.2)
   # Stop the movement before it reaches target
   rtde_c.stopL(0.5)

   # Move back to initial joint configuration
   rtde_c.moveJ(init_q)

   # Stop the RTDE control script
   rtde_c.stopScript()

You can find the source code of this example under :file:`examples/cpp/move_async_example.cpp`, if you compiled
ur_rtde with examples you can run this example from the *bin* folder. If you want to run the python example
navigate to :file:`examples/py/` and run :bash:`python3 move_async_example.py`.

