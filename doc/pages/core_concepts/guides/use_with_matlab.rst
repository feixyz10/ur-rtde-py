.. _use-with-matlab:

Use with MATLAB
===============
MATLAB supports calling python library functions, please see
`this <https://se.mathworks.com/help/matlab/getting-started-with-python.html>`_ site for more information.

Here is an example of receiving the actual joint and tcp pose from the robot, and moving the robot
to some pre-defined cartesian position in MATLAB:

.. code-block:: matlab

    import py.rtde_receive.RTDEReceiveInterface
    import py.rtde_control.RTDEControlInterface

    rtde_r = RTDEReceiveInterface("localhost");
    rtde_c = RTDEControlInterface("localhost");

    actual_q = rtde_r.getActualQ();
    actual_tcp_pose = rtde_r.getActualTCPPose();

    % Convert to MATLAB array of double
    actual_q_array = cellfun(@double, cell(actual_q));
    actual_tcp_pose_array = cellfun(@double, cell(actual_tcp_pose));

    actual_q_array
    actual_tcp_pose_array

    position1 = [-0.343, -0.435, 0.50, -0.001, 3.12, 0.04];
    position2 = [-0.243, -0.335, 0.20, -0.001, 3.12, 0.04];

    rtde_c.moveL(position1);
    rtde_c.moveL(position2);
    rtde_c.stopRobot();
    clear

.. warning::
    Please notice, it is very important to include the 'clear' command and the end of execution, otherwise the ur_rtde
    threads will continue run in the background and you would not be able to execute the code again until the environment
    has been cleared.

.. note::
    Currently using the ur_rtde interface has only been tested with MATLAB R2019b using Python 2.7, since this seems
    to be the default interpreter of MATLAB R2019b. However, it should also work with Python 3.x
