Environment Setup
*****************

It is recommended to test the example programs against a Universal Robots simulator before running them on a real robot.
A convenient way to do this is to run the simulator in a Docker container.
If you need to install Docker on Ubuntu, see https://docs.docker.com/engine/install/ubuntu/. For Windows
see `here <https://docs.docker.com/desktop/setup/install/windows-install/>`_ and for MacOS see
`here <https://docs.docker.com/desktop/setup/install/mac-install/>`_.

Contents
========

This section provides guidance for two common setup paths: using a simulator for local testing and preparing a physical robot for real-world use.

- :ref:`Simulator setup <simulator-setup>`
- :ref:`Real robot setup <real-robot-setup>`

.. _simulator-setup:

Simulator setup
===============

Step-by-step guide
------------------

1. Pull the Docker image:

   .. code-block:: bash

      docker pull universalrobots/ursim_e-series

2. Create a Docker network for the simulator:

   .. code-block:: bash

      docker network create --subnet=192.168.56.0/24 ursim_net

3. Start the simulator with a fixed IP address on that network.

   .. code-block:: bash

      docker run --rm -it --net ursim_net --ip 192.168.56.101 universalrobots/ursim_e-series

4. Optional: if you do not want to use the default UR5, add the ``ROBOT_MODEL`` environment variable.
   The available model options are ``UR3``, ``UR5``, ``UR7``, ``UR8LONG``, ``UR10``, ``UR12``, ``UR15``, ``UR16``, ``UR18``, ``UR20``, and ``UR30``.

   .. code-block:: bash

      docker run --rm -it --net ursim_net --ip 192.168.56.101 -e ROBOT_MODEL=UR10 universalrobots/ursim_e-series

For further information and options, see the official Universal Robots Docker Hub page:
https://hub.docker.com/r/universalrobots/ursim_e-series

For other simulator variants, use the corresponding image:

- CB-series: https://hub.docker.com/r/universalrobots/ursim_cb3
- Polyscope X: https://hub.docker.com/r/universalrobots/ursim_polyscopex

Once the simulator is running in the terminal it will print:

.. code-block:: bash

   IP address of the simulator

       192.168.56.101


   Access the robots user interface through this URL:

       http://192.168.56.101:6080/vnc.html?host=192.168.56.101&port=6080


   Access the robots user interface with a VNC application on this address:

       192.168.56.101:5900


   You can find documentation on how to use this container on dockerhub:

       https://hub.docker.com/r/universalrobots/ursim_e-series


   Press Crtl-C to exit

The easiest way to view the simulator is to just use the URL link provided. Otherwise,
use a VNC client like Remmina on Ubuntu.

.. _real-robot-setup:

Real robot setup
================

Before running the examples on a physical robot, make sure the robot is reachable on the network and that the required controller features are enabled.

Recommended checks
------------------

1. Verify that the robot controller and the computer running ur_rtde are on the same network.
2. Confirm that the robot IP address is known and reachable from the host machine.
3. Make sure the robot is in remote control mode when using the default RTDE control workflow.
4. If you are using the External Control UR Cap, install it on the robot controller and configure it for the correct host IP address.
