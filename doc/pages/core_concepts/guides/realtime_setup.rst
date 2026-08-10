.. _realtime-setup-guide:

.. role:: bash(code)
   :language: bash


Real-time Setup Guide
=====================
ur_rtde makes it possible to specify a desired real-time priority for the RTDEControlInterface and RTDEReceiveInterface.
First you must make sure your OS supports a real-time kernel and install it, if not already available. In the following
sections you can find how to set it up on various commonly used operating systems.

.. tip::

    It is highly recommended that you use ur_rtde in combination with a real-time kernel, this makes sure that if the system
    experience a high load, ur_rtde would still be able to control the robot in a precise and safe manner.

Linux
-----
The linux kernel is not real-time capable by default, and therefore one has to be installed.

Ubuntu 22.04
~~~~~~~~~~~~
For Ubuntu 22.04 a real-time beta kernel is already installed, but it has to be enabled. See more
information here: `Real-time Ubuntu 22.04 LTS Beta <https://ubuntu.com/blog/real-time-ubuntu-released>`_

How to enable the real-time kernel beta
"""""""""""""""""""""""""""""""""""""""
The beta kernel is available for free for personal use via Ubuntu Advantage for Infrastructure (UA-I)
To attach your personal machine to a UA subscription, please run:

.. code-block:: shell

    ua attach <free token>

where the <free token> is found on your Ubuntu one account associated with the free UA subscription.

Make sure you have at least version 27.8 of ubuntu-advantage-tools. You can check the version with:

.. code-block:: shell

    ua version

To upgrade ubuntu-advantage-tools to 27.8 in Jammy Jellyfish, please run:

.. code-block:: shell

    sudo apt install ubuntu-advantage-tools=27.8~22.04.1

To enable the real-time beta kernel, run:

.. code-block:: shell

    ua enable realtime-kernel --beta

Now reboot the system and the realtime kernel should be loaded.

.. warning::
    Please note you will need to manually configure grub to revert back to your original kernel after enabling real-time.

    For more information: ua help realtime-kernel

Ubuntu 20.04 / 18.04 / 16.04
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To get real-time support into an ubuntu system, the following steps have to be performed:

1. Get the sources of a real-time kernel
2. Compile the real-time kernel
3. Install real-time kernel

.. note::
   NVIDIA Drivers are not supported on PREEMPT_RT kernels!

Install dependencies
""""""""""""""""""""
To build the kernel, you will need a couple of tools available on your system. You can install them using

.. code-block:: shell

   sudo apt-get install build-essential bc curl ca-certificates gnupg2 libssl-dev lsb-release libelf-dev zstd libncurses-dev dwarves gawk flex bison

To continue with this tutorial, please create a temporary folder and navigate into it.
You should have sufficient space (around 25GB) there, as the extracted kernel sources take much space.
After the new kernel is installed, you can delete this folder again.

In this example we will use a temporary folder inside our home folder:

.. code-block:: shell

   mkdir -p ${HOME}/rt_kernel_build
   cd ${HOME}/rt_kernel_build

Getting the sources for a real-time kernel
""""""""""""""""""""""""""""""""""""""""""
Then, you have to decide which kernel version to use. To find the one you are using currently, use :bash:`uname -r`.
Real-time patches are only available for select kernel versions,
see https://www.kernel.org/pub/linux/kernel/projects/rt/. We recommend choosing the version closest to the one you
currently use. If you choose a different version, simply substitute the numbers. Having decided on a version, use curl
to download the source files:


.. admonition:: Ubuntu 16.04
  :class: note

    Ubuntu 16.04 was tested with the kernel version 4.14.12:

    .. code-block:: shell

        curl -SLO https://www.kernel.org/pub/linux/kernel/v4.x/linux-4.14.12.tar.xz
        curl -SLO https://www.kernel.org/pub/linux/kernel/v4.x/linux-4.14.12.tar.sign
        curl -SLO https://www.kernel.org/pub/linux/kernel/projects/rt/4.14/older/patch-4.14.12-rt10.patch.xz
        curl -SLO https://www.kernel.org/pub/linux/kernel/projects/rt/4.14/older/patch-4.14.12-rt10.patch.sign

.. admonition:: Ubuntu 18.04
  :class: note

    Ubuntu 18.04 was tested with the kernel version 5.4.19:

    .. code-block:: shell

        curl -SLO https://www.kernel.org/pub/linux/kernel/v5.x/linux-5.4.19.tar.xz
        curl -SLO https://www.kernel.org/pub/linux/kernel/v5.x/linux-5.4.19.tar.sign
        curl -SLO https://www.kernel.org/pub/linux/kernel/projects/rt/5.4/older/patch-5.4.19-rt10.patch.xz
        curl -SLO https://www.kernel.org/pub/linux/kernel/projects/rt/5.4/older/patch-5.4.19-rt10.patch.sign

.. admonition:: Ubuntu 20.04
  :class: note

    Ubuntu 20.04 was tested with the kernel version 5.9.1:

    .. code-block:: shell

        curl -SLO https://www.kernel.org/pub/linux/kernel/v5.x/linux-5.9.1.tar.xz
        curl -SLO https://www.kernel.org/pub/linux/kernel/v5.x/linux-5.9.1.tar.sign
        curl -SLO https://www.kernel.org/pub/linux/kernel/projects/rt/5.9/patch-5.9.1-rt20.patch.xz
        curl -SLO https://www.kernel.org/pub/linux/kernel/projects/rt/5.9/patch-5.9.1-rt20.patch.sign

To unzip the downloaded files do

.. code-block:: shell

   xz -d *.xz

Verification
""""""""""""
.. note::
    This step is optional but recommended!

The .sign files can be used to verify that the downloaded files were not corrupted or tampered with. The steps shown
here are adapted from the `Linux Kernel Archive <https://www.kernel.org/signature.html>`_, see the linked page for more
details about the process.

You can use gpg2 to verify the .tar archives:

.. code-block:: shell

    gpg2 --verify linux-*.tar.sign
    gpg2 --verify patch-*.patch.sign

If your output is similar to the following:

.. code-block:: none

    $ gpg2 --verify linux-*.tar.sign
    gpg: assuming signed data in 'linux-4.14.12.tar'
    gpg: Signature made Fr 05 Jan 2018 06:49:11 PST using RSA key ID 6092693E
    gpg: Can't check signature: No public key

You have to first download the public key of the person who signed the above file. As you can see from the above output,
it has the ID 6092693E. You can obtain it from the key server:

.. code-block:: shell

    gpg2  --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 6092693E

Similarly for the patch:

.. code-block:: shell

    gpg2 --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 2872E4CC

Note that keys for other kernel version might have different IDs, you will have to adapt accordingly.

Having downloaded the keys, you can now verify the sources. Here is an example of a correct output:

.. code-block:: shell

    $ gpg2 --verify linux-*.tar.sign
    gpg: assuming signed data in 'linux-4.14.12.tar'
    gpg: Signature made Fr 05 Jan 2018 06:49:11 PST using RSA key ID 6092693E
    gpg: Good signature from "Greg Kroah-Hartman <gregkh@linuxfoundation.org>" [unknown]
    gpg:                 aka "Greg Kroah-Hartman <gregkh@kernel.org>" [unknown]
    gpg:                 aka "Greg Kroah-Hartman (Linux kernel stable release signing key) <greg@kroah.com>" [unknown]
    gpg: WARNING: This key is not certified with a trusted signature!
    gpg:          There is no indication that the signature belongs to the owner.
    Primary key fingerprint: 647F 2865 4894 E3BD 4571  99BE 38DB BDC8 6092 693E

See `Linux Kernel Archive <https://www.kernel.org/signature.html>`_ for more information about the warning.

Compilation
"""""""""""
Once you are sure the files were downloaded properly, you can extract the source code and apply the patch:

.. code-block:: shell

    tar xf linux-*.tar
    cd linux-*/
    patch -p1 < ../patch-*.patch

Next copy your currently booted kernel configuration as the default config for the new real time kernel:

.. code-block:: shell

    cp -v /boot/config-$(uname -r) .config

Now you can use this config as the default to configure the build:

.. code-block:: shell

    make olddefconfig
    make menuconfig

The second command brings up a terminal interface in which you can configure the preemption model.
For the preemption model select Fully Preemptible Kernel:

.. code-block:: shell

   Preemption Model
      1. No Forced Preemption (Server) (PREEMPT_NONE)
    > 2. Voluntary Kernel Preemption (Desktop) (PREEMPT_VOLUNTARY)
      3. Preemptible Kernel (Low-Latency Desktop) (PREEMPT__LL) (NEW)
      4. Preemptible Kernel (Basic RT) (PREEMPT_RTB) (NEW)
      5. Fully Preemptible Kernel (RT) (PREEMPT_RT_FULL) (NEW)
    choice[1-5]: 5

After that navigate to Cryptographic API > Certificates for signature checking (at the very bottom of the list) >
Provide system-wide ring of trusted keys > Additional X.509 keys for default system keyring

Remove the “debian/canonical-certs.pem” from the prompt and press Ok. Save this configuration to :bash:`.config` and
exit the TUI.

Now you are ready to compile the kernel. As this is a lengthy process, set the multithreading option -j to the number
of your CPU cores:

.. code-block:: shell

    make -j$(nproc) deb-pkg


Installation
""""""""""""
After building, install the linux-headers and linux-image packages in the parent folder
(only the ones without the -dbg in the name).

.. code-block:: shell

    sudo dpkg -i ../linux-headers-*.deb ../linux-image-*.deb

Verifying the new kernel
""""""""""""""""""""""""
Restart your system. The Grub boot menu should now allow you to choose your newly installed kernel. To see which one is
currently being used, see the output of the :bash:`uname -a` command. It should contain the string PREEMPT RT and the version
number you chose. Additionally, :file:`/sys/kernel/realtime` should exist and contain the the number :bash:`1`.

Setup user privileges to use real-time scheduling
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To be able to set real-time priority on threads with user privileges you'll have to change the user's limits by
changing :file:`/etc/security/limits.conf` (See the manpage for details).

It is recommended to setup a group for real-time users instead of writing a fixed username into the config file:

.. code-block:: shell

    sudo groupadd realtime
    sudo usermod -aG realtime $(whoami)

Afterwards, add the following limits to the *realtime* group in :file:`/etc/security/limits.conf` contains:

.. code-block:: shell

    @realtime soft rtprio 99
    @realtime soft priority 99
    @realtime soft memlock 102400
    @realtime hard rtprio 99
    @realtime hard priority 99
    @realtime hard memlock 102400

You need to log out and in again or simply reboot in order for the new limits to take effect.

This part of the guide was inspired by the guide from the universal robots urcl client library git repository found
`here <https://github.com/UniversalRobots/Universal_Robots_Client_Library/blob/master/doc/real_time.md>`_. as well as
the guide from the Franka Control Interface documentation found here:
`Setting up the real-time kernel <https://frankaemika.github.io/docs/installation_linux.html#setting-up-the-real-time-kernel>`_.

Windows
-------
The Windows NT kernel is real-time capable by default, this means we do not need to setup anything special to
execute ur_rtde with real-time priority, although you might have to run your program as Administrator.
On Windows the real-time priorities are set differently than on Linux and in another range.

Learn more about the real-time priorities on windows here: `Scheduling Priorities <https://docs.microsoft.com/en-us/windows/win32/procthread/scheduling-priorities>`_

Setting a real-time priority
----------------------------
The real-time priority of the RTDEControl and RTDEReceiveInterface can be set with an integer specified
in the constructor of the interfaces. Like so:

.. code-block:: c++

    // ur_rtde real-time priorities
    int rt_receive_priority = 90;
    int rt_control_priority = 85;

    RTDEControlInterface rtde_control(robot_ip, rtde_frequency, flags, ur_cap_port, rt_control_priority);
    RTDEReceiveInterface rtde_receive(robot_ip, rtde_frequency, {}, true, false, rt_receive_priority);

    // Set application real-time priority
    RTDEUtility::setRealtimePriority(80);

On linux the priority range is (0-99) where 99 is the highest priority available. However be aware that a priority
of 99 might make the OS unstable. If no priority is specified the interface will default to the safe maximum priority
of 90. If a negative priority is specified, real-time priority will be disabled.

On Windows the REALTIME_PRIORITY_CLASS is set for the process, this means that the priority range is (16-31), where
the priority levels are:

+-------------------------------+---------------+
| Thread priority level         | Base priority |
+===============================+===============+
| THREAD_PRIORITY_IDLE          | 16            |
+-------------------------------+---------------+
| THREAD_PRIORITY_LOWEST        | 22            |
+-------------------------------+---------------+
| THREAD_PRIORITY_BELOW_NORMAL  | 23            |
+-------------------------------+---------------+
| THREAD_PRIORITY_NORMAL        | 24            |
+-------------------------------+---------------+
| THREAD_PRIORITY_ABOVE_NORMAL  | 25            |
+-------------------------------+---------------+
| THREAD_PRIORITY_HIGHEST       | 26            |
+-------------------------------+---------------+
| THREAD_PRIORITY_TIME_CRITICAL | 31            |
+-------------------------------+---------------+

Also see the more complete real-time control example:

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../../examples/py/realtime_control_example.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../../examples/cpp/realtime_control_example.cpp
         :language: c++
