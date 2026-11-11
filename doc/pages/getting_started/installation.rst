.. _installation:

************
Installation
************

There are several ways to use ur_rtde, if you want to try the C++ examples simply
follow the steps outlined in the :ref:`Building ur_rtde <building_ur_rtde>` section.

.. note::
    It is not necessary to install the library to be able to run the examples, this is only relevant
    if you want to use ur_rtde as a library that you link to your own C++ application.

If you are interested in using the Python module you can follow the instructions given in the
:ref:`Quick install <quick_install>` section.

.. _quick_install:

Quick install
=============
The easiest way if you want to try out ur_rtde, is to install the python module from PyPI.

.. code-block:: bash

   pip install ur_rtde

or, when working from a source checkout with `uv <https://docs.astral.sh/uv/>`_:

.. code-block:: bash

   uv sync   # create a virtual environment and build the bindings
   uv build  # build an sdist and a stable-ABI (cp310-abi3) wheel


.. _building_ur_rtde:

Building ur_rtde
================
This section describes how to build ur_rtde from source on different platforms. The dependencies of ur_rtde are:

* `Boost <https://www.boost.org/>`_
* `nanobind <https://github.com/wjakob/nanobind>`_

.. note::
   The nanobind dependency is optional and is only required if ur_rtde is to be used with Python (3.10 or newer).
   The ur_rtde library depends on Boost for networking.

Linux (Ubuntu) and macOS
------------------------

Dependencies
~~~~~~~~~~~~
You can install Boost on Ubuntu using:

.. code-block:: shell

   sudo apt-get install libboost-all-dev

On macOS you can install Boost with:

.. code-block:: shell

   brew install boost

Build
~~~~~

.. code-block:: shell

    git clone <repository-url>
    cd ur_rtde
    mkdir build
    cd build
    cmake ..
    make
    sudo make install


.. role:: bash(code)
   :language: bash

.. note::
    The Python bindings require Python 3.10 or newer. If you do not want to use Python at all, please
    use :bash:`cmake -DPYTHON_BINDINGS:BOOL=OFF ..`

Windows
-------

Dependencies
~~~~~~~~~~~~

Boost precompiled libraries can be found at:
https://sourceforge.net/projects/boost/files/boost-binaries/

.. note::
    If you are using Microsoft Visual Studio make sure you download a version of
    Boost that matches your Microsoft Visual Studio version.

Depending on your setup it may be required to add the Boost library path
to the system environment variables.

.. code-block:: shell

    git clone <repository-url>

Visual Studio Build
~~~~~~~~~~~~~~~~~~~

If you are using Microsoft Visual Studio with CMake support (supported since 2017 version).
You should be able to navigate to the folder you cloned the repository to and open the ur_rtde
as a CMake project. See the image below.

.. image:: ../../_static/open_cmake_project.png
  :target: https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019
  :alt: Open CMake Project

You can find more information about CMake projects in visual studio `Here
<https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019>`_.

Once you have loaded the project navigate to the CMake Settings through 'Project->CMake Settings for ur_rtde'.
Set the following CMake Command Arguments in order for the project to find Boost and disable the Python bindings:

.. code-block:: shell

    -DBOOST_ROOT=<YOUR_BOOST_ROOT> -DBOOST_LIBRARYDIR=<YOUR_BOOST_LIBRARY_DIR> -DPYTHON_BINDINGS:BOOL=OFF

Commandline Build
~~~~~~~~~~~~~~~~~

A quick way to compile the interface on Windows, is to do everything from the command line.
The following commands can be executed from the command prompt after Boost has been installed.

.. code-block:: shell

    git clone <repository-url>
    cd ur_rtde
    mkdir Build
    cd Build
    cmake -DBOOST_ROOT="<Path:\to\boost_<version>>" -DBOOST_LIBRARYDIR="<Path:\to\boost_<VERSION>\<COMPILER>>" -DPYTHON_BINDINGS=OFF ..
    msbuild ur_rtde.sln /property:Configuration=Release /maxcpucount:<NUMBER_OF_CORES>

filling out the system specific variables the setup looks like this for a computer with 8 cores,
using visual studio 2019 and boost 1.71.0.

.. code-block:: shell

    cmake -DBOOST_ROOT="C:\local\boost_1_71_0" -DBOOST_LIBRARYDIR="C:\local\boost_1_71_0\lib64-msvc-14.2" -DPYTHON_BINDINGS=OFF ..
    msbuild ur_rtde.sln /property:Configuration=Release /maxcpucount:8

The example above is the basic setup for compiling ur_rtde below you'll find a list of other commands and what they do.

- -G "Visual Studio 15 2017 Win64"
    - This command forces the generator to vs 2017 64 bit. On some 64 bit systems when running cmake boost will
      look for 32bit libraries which are not there.
    - The equivalent for Visual studio 2019 is: -G "Visual Studio 16 2019" -A x64

- -DBUILD_STATIC=[ON|OFF]
    - The default value is "OFF", which means that a dll library is built.
      This means, that when trying to link against this library runtime.
      The .dll file must either be moved to the same directory as .exe file.
      Or the system PATH variable must be set to include Build\\<config>
    - If it is set to ON the libraries will be build statically.
      This will make the compile time linking libraries a little longer and the output .exe or .lib a little larger.

- -DBoost_USE_STATIC_LIBS=[ON|OFF]
    - The default is OFF. If set to on it will link statically to Boost libraries.
      This will give the same benefits/drawbacks as -DBUILD_STATIC, just for the Boost libraries instead.

