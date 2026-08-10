.. _quick_start:

Quick Start
***********

This guide walks you through the three core interfaces of ur_rtde and shows
runnable examples you can try against the
:ref:`Docker-based UR simulator <simulator-setup>`
before touching a real robot.

.. note::
   All examples use IP ``192.168.56.101``, which is the IP assigned to the simulator
   in :ref:`Simulator Setup <simulator-setup>`. If you are connecting to a real robot,
   replace this with your robot's IP address.

The three interfaces
====================

ur_rtde exposes three independent interfaces, each connecting to the robot
separately:

.. list-table::
   :widths: 25 75
   :header-rows: 1

   * - Interface
     - Purpose
   * - ``RTDEControlInterface``
     - Send motion commands to the robot (``moveJ``, ``moveL``, ``servoJ``, etc.)
   * - ``RTDEReceiveInterface``
     - Read robot state at high frequency (joint positions, TCP pose, forces, …)
   * - ``RTDEIOInterface``
     - Set and read digital/analog I/O signals

You can instantiate all three simultaneously - they each open their own
connection to the robot.

Moving the robot
================

.. raw:: html

   <div style="width:60%; max-width:512px; cursor:pointer; border-radius:6px; overflow:hidden;"
        title="Click to pause / resume"
        onclick="var v=this.querySelector('video'); v.paused ? v.play() : v.pause();">
     <video autoplay loop muted playsinline
            style="width:100%; display:block; border-radius:6px;">
       <source src="../../_static/basic_move.webm" type="video/webm">
     </video>
   </div>

The example below moves the robot to a home position in joint space with
**moveJ**, then to two Cartesian poses with **moveL**, and returns home.

- ``moveJ`` takes joint angles in **radians**.
- ``moveL`` takes a Cartesian pose ``[x, y, z, rx, ry, rz]`` in **meters**
  and **axis-angle radians**.
- Both accept ``speed`` and ``acceleration`` as the next arguments.

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../examples/py/basic_move.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../examples/cpp/basic_move.cpp
         :language: c++


.. tip::
   ``moveJ`` and ``moveL`` are **blocking** by default — they return only after
   the robot has reached the target. Pass ``true`` / ``True`` as the last
   argument to make them non-blocking (asynchronous).

Reading robot state
===================

``RTDEReceiveInterface`` lets you read live data from the robot. The example
below prints the current joint positions and TCP pose.

.. tabs::

   .. code-tab:: python

      from rtde_receive import RTDEReceiveInterface as RTDEReceive

      rtde_r = RTDEReceive("192.168.56.101")

      # Get the current joint positions [rad]
      joint_q = rtde_r.getActualQ()
      print("Joint positions (rad):", joint_q)

      # Get the current TCP pose [x, y, z, rx, ry, rz]
      tcp_pose = rtde_r.getActualTCPPose()
      print("TCP position (m):            ", tcp_pose[:3])
      print("TCP orientation (axis-angle):", tcp_pose[3:])

   .. code-tab:: c++

      #include <ur_rtde/rtde_receive_interface.h>
      #include <iostream>
      #include <vector>

      using namespace ur_rtde;

      int main(int argc, char* argv[])
      {
        RTDEReceiveInterface rtde_receive("192.168.56.101");

        // Get the current joint positions [rad]
        std::vector<double> joint_q = rtde_receive.getActualQ();
        std::cout << "Joint positions (rad):" << std::endl;
        for (size_t i = 0; i < joint_q.size(); i++)
          std::cout << "  q[" << i << "] = " << joint_q[i] << std::endl;

        // Get the current TCP pose [x, y, z, rx, ry, rz]
        std::vector<double> tcp_pose = rtde_receive.getActualTCPPose();
        std::cout << "TCP position (m):              ["
                  << tcp_pose[0] << ", " << tcp_pose[1] << ", " << tcp_pose[2] << "]" << std::endl;
        std::cout << "TCP orientation (axis-angle):  ["
                  << tcp_pose[3] << ", " << tcp_pose[4] << ", " << tcp_pose[5] << "]" << std::endl;

        return 0;
      }


.. note::
   Data is received at **500 Hz** on e-Series robots and **125 Hz** on CB-series
   robots. This makes ``RTDEReceiveInterface`` suitable for real-time monitoring
   and closed-loop control.

Controlling I/O
===============

``RTDEIOInterface`` sets outputs. Use ``RTDEReceiveInterface`` to read them back, 
reading I/O state is part of the receive interface, not the IO interface.

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../examples/py/io_example.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../examples/cpp/io_example.cpp
         :language: c++


Next steps
==========

- See :doc:`../../examples/basic_motion/index` for more motion examples.
- See the :doc:`../../reference/index` for the full API.
