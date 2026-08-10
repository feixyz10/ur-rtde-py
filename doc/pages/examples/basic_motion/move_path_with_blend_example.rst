MoveL Path With Blending
========================

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example will use the **moveL** command with a path, where each joint pose in the path has a defined velocity,
acceleration and blend. The joint poses in the path are defined by a 9-dimensional vector, where the first six
values constitutes the joint pose, followed by the last three values *velocity*, *acceleration* and *blend*.

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../../examples/py/movel_path_with_blend_example.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../../examples/cpp/movej_path_with_blend_example.cpp
         :language: c++

Intended movement:

.. image:: ../../../_static/movel_path_blend.gif

