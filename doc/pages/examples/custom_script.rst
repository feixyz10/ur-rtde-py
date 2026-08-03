Use with custom script
======================

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

The rtde control script is uploaded to the robot by default. However if you want
to modify the script and execute it as a part of a program on the controller, you
have the option of *not* uploading the default rtde control script. This means that ur_rtde expects a
script to be running on the controller that you have set up manually eg. copied to the controller from USB or
scp over the network.

What you can do, is to split the rtde_control script into two separate scripts: rtde_init.script and rtde_control.script.
where rtde_init.script contains the header and rtde_control.script contains the control loop.
You then create a new program in Polyscope and add a BeforeStart sequence to this new program. Simply add the
rtde_init.script to the BeforeStart sequence and the rtde_control.script to the Robot Program. See picture below:

.. image:: ../_static/ur_rtde_manual_program.png

Remember! that if you copy the rtde_control.script, please remove the $MAJOR.MINOR tags, that is used for removing
lines that are not compatible with specific controller versions. Also set the appropriate offset for reg_offset_float and
reg_offset_int either 0 or 24.

The benefit of the approach is that you have access to any functionality installed on the robot such as
functions from a gripper UR cap etc. (the same is true, when using the ExternalControl UR Cap). You must simply specify
that you want to use a custom script in the RTDEControlInterface constructor with the FLAG_CUSTOM_SCRIPT:

for Python:

.. code-block:: python

   from rtde_control import RTDEControlInterface as RTDEControl
   rtde_c = RTDEControl("127.0.0.1", RTDEControl.FLAG_CUSTOM_SCRIPT)
   rtde_c.moveL([-0.143, -0.435, 0.20, -0.001, 3.12, 0.04], 0.5, 0.3)

for C++:

.. code-block:: c++

   RTDEControlInterface rtde_control("127.0.0.1", RTDEControlInterface::FLAG_CUSTOM_SCRIPT);
   rtde_control.moveL({-0.143, -0.435, 0.20, -0.001, 3.12, 0.04}, 0.5, 0.2);

When you execute your ur_rtde application it will simply wait for you to press play on the controller in order
to start, unless you use the FLAG_NO_WAIT, in which case the interface will be initialized, but cannot be
used before the program is running on the controller. Finally make sure the robot is in remote control.


.. _record-data-example:

