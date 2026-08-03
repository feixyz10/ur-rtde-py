Move Until Contact
==================

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example will move the robot down in the Z-axis with a speed of 100mm/s until contact is detected. The robot is
automatically moved back to the initial point of contact. You can specify the speed vector as well as a direction
to check for contacts in, see the API for further details.

You can find the source code of this example under :file:`examples/cpp/move_until_contact.cpp`, if you compiled
ur_rtde with examples you can run this example from the *bin* folder. If you want to run the python example
navigate to :file:`examples/py/` and run :bash:`python3 move_until_contact.py`.

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
     std::vector<double> speed = {0, 0, -0.100, 0, 0, 0};
     rtde_control.moveUntilContact(speed);
     rtde_control.stopScript();
     return 0;
   }

Python:

.. code-block:: python

   import rtde_control

   rtde_c = rtde_control.RTDEControlInterface("127.0.0.1")
   speed = [0, 0, -0.100, 0, 0, 0]
   rtde_c.moveUntilContact(speed)

   rtde_c.stopScript()


