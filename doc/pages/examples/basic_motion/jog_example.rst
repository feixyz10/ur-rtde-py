Jog
===

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example shows how you can use the **jogStart()** function of the RTDEControlInterface to jog the robot in the
tool frame, using the arrows on your keyboard.

.. tabs::

   .. tab:: C++

      .. literalinclude:: ../../../../examples/cpp/jog_example.cpp
         :language: c++

.. note::
   This example only works on Linux / UNIX at the moment, since it requires ncurses for registering key presses.
   It can fairly easy be adjusted to work for Windows, just use the *conio.h* header instead of ncurses.
