IO Example
==========

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example will print out the state of a standard digital output, change the state of that output and print the
state again. Furthermore it will set the current ratio of an analog output.

You can find the source code of this example under :file:`examples/cpp/io_example.cpp`, if you compiled
ur_rtde with examples you can run this example from the *bin* folder. If you want to run the python example
navigate to :file:`examples/py/` and run :bash:`python3 io_example.py`.

C++:

.. code-block:: c++

   #include <ur_rtde/rtde_io_interface.h>
   #include <ur_rtde/rtde_receive_interface.h>
   #include <iostream>
   #include <thread>

   using namespace ur_rtde;

   int main(int argc, char* argv[])
   {
     RTDEIOInterface rtde_io("127.0.0.1");
     RTDEReceiveInterface rtde_receive("127.0.0.1");

     /** How-to set and get standard and tool digital outputs. Notice that we need the
       * RTDEIOInterface for setting an output and RTDEReceiveInterface for getting the state
       * of an output.
       */

     if (rtde_receive.getDigitalOutState(7))
       std::cout << "Standard digital out (7) is HIGH" << std::endl;
     else
       std::cout << "Standard digital out (7) is LOW" << std::endl;

     if (rtde_receive.getDigitalOutState(16))
       std::cout << "Tool digital out (16) is HIGH" << std::endl;
     else
       std::cout << "Tool digital out (16) is LOW" << std::endl;

     rtde_io.setStandardDigitalOut(7, true);
     rtde_io.setToolDigitalOut(0, true);
     std::this_thread::sleep_for(std::chrono::milliseconds(10));

     if (rtde_receive.getDigitalOutState(7))
       std::cout << "Standard digital out (7) is HIGH" << std::endl;
     else
       std::cout << "Standard digital out (7) is LOW" << std::endl;

     if (rtde_receive.getDigitalOutState(16))
       std::cout << "Tool digital out (16) is HIGH" << std::endl;
     else
       std::cout << "Tool digital out (16) is LOW" << std::endl;

     // How to set a analog output with a specified current ratio
     rtde_io.setAnalogOutputCurrent(1, 0.25);

     return 0;
   }

Python:

.. code-block:: python

   import rtde_io
   import rtde_receive
   import time

   rtde_io_ = rtde_io.RTDEIOInterface("127.0.0.1")
   rtde_receive_ = rtde_receive.RTDEReceiveInterface("127.0.0.1")

   # How-to set and get standard and tool digital outputs. Notice that we need the
   # RTDEIOInterface for setting an output and RTDEReceiveInterface for getting the state
   # of an output.

   if rtde_receive_.getDigitalOutState(7):
       print("Standard digital out (7) is HIGH")
   else:
       print("Standard digital out (7) is LOW")

   if rtde_receive_.getDigitalOutState(16):
       print("Tool digital out (16) is HIGH")
   else:
       print("Tool digital out (16) is LOW")

   rtde_io_.setStandardDigitalOut(7, True)
   rtde_io_.setToolDigitalOut(0, True)
   time.sleep(0.01)

   if rtde_receive_.getDigitalOutState(7):
       print("Standard digital out (7) is HIGH")
   else:
       print("Standard digital out (7) is LOW")

   if rtde_receive_.getDigitalOutState(16):
       print("Tool digital out (16) is HIGH")
   else:
       print("Tool digital out (16) is LOW")

   # How to set a analog output with a specified current ratio
   rtde_io_.setAnalogOutputCurrent(1, 0.25)

.. _robotiq-gripper-example:

