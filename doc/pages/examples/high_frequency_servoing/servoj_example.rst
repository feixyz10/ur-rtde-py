ServoJ
======

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
   instructions. That said, servoJ works fine without a real-time kernel for most use cases — you may
   simply see slightly less precise timing at 500 Hz on a standard kernel.

.. note::
   Keep consecutive target poses close together (dense trajectory). If the robot struggles to keep up,
   increase the ``gain`` parameter or reduce ``lookahead_time``.

