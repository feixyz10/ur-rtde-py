import time
import numpy as np
from rtde_control import RTDEControlInterface as RTDEControl
rtde_frequency = 500.0
rtde_c = RTDEControl("localhost", rtde_frequency, RTDEControl.FLAG_VERBOSE | RTDEControl.FLAG_USE_EXT_UR_CAP)
mass_matrix = np.array(rtde_c.getMassMatrix()).reshape(6, 6)
print('Mass Matrix:', mass_matrix)
