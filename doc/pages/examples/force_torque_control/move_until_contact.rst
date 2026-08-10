Move Until Contact
==================

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example will move the robot down in the Z-axis with a speed of 100mm/s until contact is detected. The robot is
automatically moved back to the initial point of contact. You can specify the speed vector as well as a direction
to check for contacts in, see the API for further details.

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../../examples/py/move_until_contact.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../../examples/cpp/move_until_contact.cpp
         :language: c++


