.. _basic-move-example:

Basic Move
==========

.. raw:: html

   <div style="width:60%; max-width:512px; cursor:pointer; border-radius:6px; overflow:hidden;"
        title="Click to pause / resume"
        onclick="var v=this.querySelector('video'); v.paused ? v.play() : v.pause();">
     <video autoplay loop muted playsinline
            style="width:100%; display:block; border-radius:6px;">
       <source src="../../../_static/basic_move.webm" type="video/webm">
     </video>
   </div>

This example demonstrates the fundamental use of ur_rtde by moving the robot
using :func:`moveJ` (joint space) and :func:`moveL` (Cartesian space). It also
uses ``RTDEReceiveInterface`` to read the actual TCP pose after the joint move,
so all Cartesian targets are computed relative to the robot's real position,
making the example safe and correct on any UR model.

The example:

1. Moves to a home position in joint space using **moveJ**.
2. Reads the current TCP pose using **getActualTCPPose**.
3. Moves 15 cm down in Z using **moveL**.
4. Moves 10 cm in X using **moveL**.
5. Returns to the home TCP pose and then the home joint configuration.

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../../examples/py/basic_move.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../../examples/cpp/basic_move.cpp
         :language: c++

.. note::
   The example connects to ``192.168.56.101`` which is the default IP for docker URSim running locally, if you 
   followed the :ref:`Simulator Setup <simulator-setup>` guide. Change this to your robot's IP address when running on a real robot.
