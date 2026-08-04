Quick Start
***********

You can use the ur_rtde interface in primarily two ways:

**1.** The RTDEControlInterface automatically uploads a script to the robot controller. (Requires the remote control feature to be enabled on the robot).

**2.** Install and use the External UR Cap on the robot controller see here, (Does not require remote control, but manual execution of the program on the controller).

**Option 1**. Is the default for PolyScope 3 (CB-series) and 5 (UR and e-Series) and does not require any additional configuration.

**Option 2**. Requires some extra flags in the
RTDEControlInterface constructor and installation of a the External Control UR Cap (This is the only option for PolyScopeX at the moment).
See the guide here :ref:`Use the ExternalControl UR Cap <external_control_urcap>`.

For an overview of the architecture of ur_rtde see :ref:`Architecture <architecture>`.

Basic use
=========
Simple example using the RTDEControlInterface to move the robot to a pose with the **moveL** command.

.. tabs::

   .. code-tab:: c++

      // The first argument of the constructor simply takes the IP address of the Robot
      RTDEControlInterface rtde_control("192.168.56.101");
      // First argument is the pose 6d vector followed by speed and acceleration
      rtde_control.moveL({-0.143, -0.435, 0.20, -0.001, 3.12, 0.04}, 0.5, 0.2);

   .. code-tab:: python

      import rtde_control
      rtde_c = rtde_control.RTDEControlInterface("192.168.56.101")
      rtde_c.moveL([-0.143, -0.435, 0.20, -0.001, 3.12, 0.04], 0.5, 0.3)


Simple example using the RTDE Receive Interface to get the joint positions of the robot

.. tabs::

   .. code-tab:: c++

      /* The constructor takes the IP address of the robot, by default all variables are
       * transmitted. Optionally only a subset of variables, specified by a vector, are transmitted.
       */
      RTDEReceiveInterface rtde_receive("192.168.56.101");
      std::vector<double> joint_positions = rtde_receive.getActualQ();

   .. code-tab:: python

      import rtde_receive
      rtde_r = rtde_receive.RTDEReceiveInterface("192.168.56.101")
      actual_q = rtde_r.getActualQ()

Simple example using the RTDE IO Interface to set a standard digital output.

.. tabs::

   .. code-tab:: c++

      RTDEIOInterface rtde_io("192.168.56.101");
      rtde_io.setStandardDigitalOut(7, true);

   .. code-tab:: python

      import rtde_io
      rtde_io = rtde_io.RTDEIOInterface("192.168.56.101")
      rtde_io.setStandardDigitalOut(7, True)

.. note::
   When using an UR or e-Series robot, data will be received at the maximum available frequency (500Hz), for a CB-series
   robot the frequency will be (125Hz).