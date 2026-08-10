.. _record-data-example:

Record Data
===========

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example shows how to record the robot data to a (.csv) file of your choice. You
can set the frequency at which the data is recorded. Optionally, you can decide to only
record a subset of the available robot data. You do this by passing a std::vector<std::string> to
startFileRecording() that contains the names of the variables that you want to record. eg.

.. code-block:: c++

   ...
   std::vector<std::string> record_variables = {"timestamp", "actual_q", "actual_TCP_pose"};
   rtde_receive.startFileRecording("robot_data.csv", record_variables);
   ...

by default all variables are recorded, and you are not required to pass the variables argument.

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../../examples/py/record_data_example.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../../examples/cpp/record_data_example.cpp
         :language: c++

