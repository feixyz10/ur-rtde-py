# ur-rtde-py — User Guide

`ur-rtde-py` is a Python interface for controlling and receiving data from **Universal Robots**
over the robot's **Real-Time Data Exchange (RTDE)** interface. It is a packaging-focused fork of
[ur_rtde](https://gitlab.com/sdurobotics/ur_rtde) `v1.6.5`: the C++ core and Python API are
unchanged, while the bindings now use [nanobind](https://github.com/wjakob/nanobind) and the
wheels use the CPython **stable ABI**.

## Highlights

- 🤖 Control and monitor all CB-Series (CB3 software ≥ 3.3) and all e-Series robots
- 🚀 Real-time control at up to 500 Hz (`servoJ` / `servoL` / `servoC`)
- 📦 One `cp310-abi3` wheel per platform — works on every CPython ≥ 3.10
- 🧩 Same module names and API as upstream `ur_rtde` — existing code keeps working
- 🖥️ Linux, macOS and Windows support

## Installation

```bash
pip install ur-rtde-py
```

or:

```bash
uv pip install ur-rtde-py
```

**Requirements**

- CPython 3.10 or newer (3.10 – 3.14 and future 3.x are covered by one abi3 wheel per platform)
- Linux (x86_64, aarch64), macOS (x86_64, Apple Silicon) or Windows x64
- Network access from your computer to the robot (ports 30001–30004, 29999 by default)

> ⚠️ **Do not install together with the upstream `ur_rtde` package.** Both provide the same
> top-level modules (`rtde_control`, `rtde_receive`, …). If you previously installed
> `ur_rtde`, remove it first:
>
> ```bash
> pip uninstall ur_rtde
> pip install ur-rtde-py
> ```

## Quick start

Replace `192.168.56.101` with your robot's IP address.

### Read robot state

```python
import rtde_receive

rtde_r = rtde_receive.RTDEReceive("192.168.56.101")

print(rtde_r.getActualQ())        # actual joint positions [rad]
print(rtde_r.getActualTCPPose())  # actual TCP pose [x, y, z, rx, ry, rz]
print(rtde_r.getActualTCPForce()) # external TCP force [N, N, N, Nm, Nm, Nm]
print(rtde_r.getRobotMode())

rtde_r.disconnect()
```

### Move the robot

```python
import rtde_control

rtde_c = rtde_control.RTDEControlInterface("192.168.56.101")

# moveJ(q, speed [rad/s], acceleration [rad/s²], asynchronous)
rtde_c.moveJ([0, -1.57, 0, -1.57, 0, 0], 1.05, 1.4)

rtde_c.disconnect()
```

> ⚠️ Real robots move. Make sure the workspace is clear, the speed is safe, and you are
> familiar with the robot's emergency stop before running motion commands.

### Asynchronous moves

```python
import rtde_control

rtde_c = rtde_control.RTDEControlInterface("192.168.56.101")

rtde_c.moveJ([0, -1.57, 0, -1.57, 0, 0], 1.05, 1.4, asynchronous=True)

status = rtde_c.getAsyncOperationProgressEx()
print(status.isAsyncOperationRunning(), status.progress())

rtde_c.disconnect()
```

### Digital / analog IO

```python
import rtde_io

rtde_io_i = rtde_io.RTDEIOInterface("192.168.56.101")

rtde_io_i.setStandardDigitalOut(0, True)
rtde_io_i.setAnalogOutputVoltage(1, 0.5)  # 0.5 V on AO1

rtde_io_i.disconnect()
```

### Dashboard (power, brakes, programs)

```python
import dashboard_client

db = dashboard_client.DashboardClient("192.168.56.101")
db.connect()

print(db.polyscopeVersion())
print(db.robotmode())

db.powerOn()
db.brakeRelease()

db.loadURP("/programs/my_program.urp")
db.play()

db.disconnect()
```

### Blended paths (`moveJ` / `moveL` with a path)

```python
import rtde_control as c

rtde_c = c.RTDEControlInterface("192.168.56.101")

path = [
    [0.00, -1.57, 0.00, -1.57, 0.00, 0.00],  # start
    [0.20, -1.57, 0.00, -1.57, 0.00, 0.00],  # waypoint
    [0.40, -1.57, 0.00, -1.57, 0.00, 0.00],  # end
]

# move through the path with a 0.2 rad/s blend radius
rtde_c.moveJ(path, asynchronous=False)
rtde_c.disconnect()
```

For fine-grained control per waypoint (speed, acceleration, blend), use
`rtde_control.Path` / `rtde_control.PathEntry` — see the upstream API docs linked below.

## Modules

| Module | Purpose |
|---|---|
| `rtde_control` | Motion control: `moveJ/L/C`, `servoJ/L/C`, `speedJ/L`, force mode, jogging, … |
| `rtde_receive` | State: joint/TCP positions, velocities, currents, IO, safety, … |
| `rtde_io` | Set digital/analog outputs and input registers |
| `dashboard_client` | Power on/off, brake release, load/run programs, popups, … |
| `script_client` | Send raw URScript to port 30003 |

## Compatibility with upstream `ur_rtde`

- Same module names: `rtde_control`, `rtde_receive`, `rtde_io`, `script_client`, `dashboard_client`
- Same class names, method names, arguments and defaults
- Same C++ core (upstream `v1.6.5`)

Scripts written for `ur_rtde` run unchanged; only the package installed via `pip` differs.

## Building from source

If no wheel matches your platform, `pip` falls back to a source build. You will need:

- A C++17 compiler (GCC ≥ 8, Clang ≥ 7, MSVC 2019+)
- CMake ≥ 3.15
- Boost headers (header-only usage; e.g. `sudo apt install libboost-dev` on Ubuntu,
  `brew install boost` on macOS)

```bash
pip install .
```

For development, use uv:

```bash
uv sync            # create .venv + build bindings
uv run python ci/smoke_test.py
uv build           # sdist + cp310-abi3 wheel
```

## Troubleshooting

- **`pip install ur-rtde-py` keeps the old behavior** — make sure upstream `ur_rtde`
  is uninstalled (`pip uninstall ur_rtde`); both packages provide the same import modules.
- **Connection refused / timeout** — check the robot IP, that the robot is running, and that
  ports 30001–30004 (RTDE) and 29999 (dashboard) are reachable.
- **No prebuilt wheel for your platform** — pip will build from source; install a C++17
  compiler, CMake and Boost headers first (see above).
- **PyPy / free-threaded Python** — not supported by the stable-ABI wheels; use standard CPython.

## Credits & license

The RTDE interface was developed by **Anders Prier Lindvig / SDU Robotics** and is licensed
under the MIT license. `ur-rtde-py` is a packaging fork and contains the same MIT-licensed
core — see [LICENSE](LICENSE).
