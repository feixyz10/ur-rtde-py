SpeedJ
======

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example will use the **speedJ** command to move the robot, where the first 2 joints are speeding continuously
in a 500Hz control loop for 2 seconds.

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../../examples/py/speedj_example.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../../examples/cpp/speedj_example.cpp
         :language: c++

Intended movement:

.. image:: ../../../_static/speedj_example.gif

