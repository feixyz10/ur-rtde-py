ServoJ
======

.. raw:: html

   <div style="width:60%; max-width:512px; cursor:pointer; border-radius:6px; overflow:hidden;"
        title="Click to pause / resume"
        onclick="var v=this.querySelector('video'); v.paused ? v.play() : v.pause();">
     <video autoplay loop muted playsinline
            style="width:100%; display:block; border-radius:6px;">
       <source src="../../../_static/servoJ_example.webm" type="video/webm">
     </video>
   </div>

**servoJ** streams joint-space setpoints to the robot at 500 Hz, allowing smooth continuous motion
along pre-computed trajectories. Each cycle the host sends the next target joint configuration and the
robot's internal interpolator blends towards it using the ``lookahead_time`` and ``gain`` parameters.

This example moves all six joints in independent sinusoidal patterns, creating a flowing multi-joint
motion that runs until **Ctrl+C** is pressed.

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../../examples/py/servoj_example.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../../examples/cpp/servoj_example.cpp
         :language: c++

.. note::
   For best results with high-frequency servoing, a real-time OS kernel is recommended to avoid
   jitter caused by OS scheduling. See the :ref:`Real-time Setup Guide <realtime-setup-guide>` for
   instructions. The example runs fine on a standard kernel, but timing precision may be slightly lower.

.. note::
   Keep consecutive target poses close together (dense trajectory). If the robot struggles to keep up,
   increase the ``gain`` parameter or reduce ``lookahead_time``.

