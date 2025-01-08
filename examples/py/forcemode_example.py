from rtde_control import RTDEControlInterface as RTDEControl
import time

rtde_c = RTDEControl("192.168.56.101")
mass = 1.0
cog = [1.0, 2.0, 3.0]
inertia = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6]
rtde_c.setTargetPayload(mass, cog, inertia)
time.sleep(10)

