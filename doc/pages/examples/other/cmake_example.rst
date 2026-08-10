CMake Example
=============

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

Once you have installed the ur_rtde library. You can use the cmake command find_package() to locate the library.
Here is an example of how to find the library and link it against your C++ executable.

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.5)
   project(ur_rtde_cmake_example)

   find_package(ur_rtde REQUIRED)
   add_executable(ur_rtde_cmake_example main.cpp)
   target_link_libraries(ur_rtde_cmake_example PRIVATE ur_rtde::rtde)

if the library is not installed or installed to a none standard system path, use one of the following methods:

*  Call CMake with :bash:`-Dur_rtde_DIR=/path/to/ur_rtde`
*  Set the path in :bash:`find_package()`:

   find_package(ur_rtde REQUIRED PATHS "/a/possible/path/to/ur_rtde" "/another/possible/path/to/ur_rtde")

The path to ur_rtde is the one where ur_rtdeTargets.cmake can be found. For a none install it should be
:file:`/path/to/ur_rtde/Build/ur_rtde`. For an install it is :file:`/path/to/lib/cmake/ur_rtde`.

