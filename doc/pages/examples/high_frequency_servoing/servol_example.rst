ServoL
======

.. raw:: html

   <div style="width:60%; max-width:512px; cursor:pointer; border-radius:6px; overflow:hidden;"
        title="Click to pause / resume"
        onclick="var v=this.querySelector('video'); v.paused ? v.play() : v.pause();">
     <video autoplay loop muted playsinline
            style="width:100%; display:block; border-radius:6px;">
       <source src="../../../_static/servoL_example.webm" type="video/webm">
     </video>
   </div>

**servoL** streams Cartesian-space setpoints to the robot at 500 Hz, enabling smooth continuous motion
along pre-computed Cartesian trajectories. The host computes the next TCP pose each cycle and the robot
blends towards it using the ``lookahead_time`` and ``gain`` parameters, similar to servoJ but in task
space rather than joint space.

This example moves the TCP in a horizontal circle of 5 cm radius at 0.5 Hz (one revolution every 2 seconds),
running until **Ctrl+C** is pressed.

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../../examples/py/servol_example.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../../examples/cpp/servol_example.cpp
         :language: c++

.. note::
   For best results at 500 Hz, a real-time OS kernel is recommended to minimise jitter.
   See the :ref:`Real-time Setup Guide <realtime-setup-guide>`. It is not strictly required -
   the example runs fine on a standard kernel, but timing precision may be slightly lower.

.. note::
   servoL is Cartesian so it uses the robot's internal IK each cycle. Keep consecutive target poses
   close together to avoid large joint-space jumps. If the robot struggles to keep up, reduce the
   radius, reduce the frequency, or increase the ``gain`` parameter.
