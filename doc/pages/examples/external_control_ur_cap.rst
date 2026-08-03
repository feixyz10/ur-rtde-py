Use with ExternalControl UR Cap
===============================

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

ur_rtde can be used together with UR's `ExternalControl UR Cap <https://github.com/UniversalRobots/Universal_Robots_ExternalControl_URCap>`_
which is also used for the Universal Robot's ROS driver.

You can download the UR Cap from
`here <https://github.com/UniversalRobots/Universal_Robots_ROS_Driver/blob/master/ur_robot_driver/resources/externalcontrol-1.0.4.urcap>`_.

To install it on your robot, please follow the instructions:

`Installing a URCap on a e-Series robot <https://github.com/UniversalRobots/Universal_Robots_ROS_Driver/blob/master/ur_robot_driver/doc/install_urcap_e_series.md>`_
(for an e-series robot)

or

`Installing a URCap on a CB3 robot <https://github.com/UniversalRobots/Universal_Robots_ROS_Driver/blob/master/ur_robot_driver/doc/install_urcap_cb3.md>`_
(for a CB-series robot).

In order to setup ur_rtde for using the ExternalControl UR Cap, all you have to do is to specify
this in the RTDEControlInterface constructor with the flag FLAG_USE_EXT_UR_CAP.

.. tip::
    Remember you can combine flags in the constructor using the bitwise OR operator both in Python
    (eg. RTDEControl.FLAG_VERBOSE | RTDEControl.FLAG_USE_EXT_UR_CAP) and in C++
    (eg. RTDEControlInterface::FLAG_NO_WAIT | RTDEControlInterface::FLAG_USE_EXT_UR_CAP)


for Python:

.. code-block:: python

   from rtde_control import RTDEControlInterface as RTDEControl
   rtde_frequency = 500.0
   rtde_c = RTDEControl("127.0.0.1", rtde_frequency, RTDEControl.FLAG_USE_EXT_UR_CAP)
   rtde_c.moveL([-0.143, -0.435, 0.20, -0.001, 3.12, 0.04], 0.5, 0.3)

for C++:

.. code-block:: c++

   RTDEControlInterface rtde_control("127.0.0.1", 500.0, RTDEControlInterface::FLAG_USE_EXT_UR_CAP);
   rtde_control.moveL({-0.143, -0.435, 0.20, -0.001, 3.12, 0.04}, 0.5, 0.2);

When you execute your ur_rtde application it will simply wait for you to press play on the controller in order
to start, unless you use the FLAG_NO_WAIT, in which case the interface will be initialized, but cannot be
used before the program is running on the controller. The port used for communicating with the UR Cap defaults to
50002 if this does not suit your application it can be changed in the constructor of the RTDEControlInterface right
after the flags. eg. RTDEControl("127.0.0.1", RTDEControl.FLAG_USE_EXT_UR_CAP, <SOME PORT>).

.. note::
   You must have the ExternalControl node as a part of the program and it must be setup with the correct IP of the
   computer that you want to control the robot from. This can be changed under:
   (Installation tab -> URCaps -> ExternalControl).


