from rtde_control import RTDEControlInterface as RTDEControl
import numpy as np
import time
from typing import Callable, Sequence

_MODEL_TO_DESCRIPTION: dict[str, str] = {
	"UR3":   "ur3_description",
	"UR3e":  "ur3e_description",
	"UR5":   "ur5_description",
	"UR5e":  "ur5e_description",
	"UR10":  "ur10_description",
	"UR10e": "ur10e_description",
	"UR16e": "ur16e_description",
}

def visualize_trajectory(robot_model: str = "UR5e") -> Callable[[Sequence[float]], None]:
	"""Start a viser viewer and return a callback for live joint updates."""
	try:
		import viser
		from viser.extras import ViserUrdf
		from robot_descriptions.loaders.yourdfpy import load_robot_description
	except ImportError as exc:
		print(f"Missing dependency for visualization: {exc}")
		print("Run: pip install viser robot_descriptions yourdfpy")
		return lambda _joint_q: None

	description_name = _MODEL_TO_DESCRIPTION.get(robot_model)
	if description_name is None:
		print(f"No robot description available for model: {robot_model}")
		return lambda _joint_q: None

	server = viser.ViserServer(port=8080)
	#server.scene.add_grid("/ground", width=3, height=3, cell_size=0.1)

	@server.on_client_connect
	def _(client: viser.ClientHandle) -> None:
		client.camera.position = (-0.902, 0.944, 0.692)
		client.camera.look_at = (-0.018,0.049,0.374)

	print(f"Fetching {robot_model} URDF model...")
	urdf = load_robot_description(description_name)
	robot_vis = ViserUrdf(server, urdf, root_node_name="/robot")
	joint_names = urdf.actuated_joint_names
	n_joints = len(joint_names)

	last_update_time = 0.0
	vis_update_hz = 30.0

	print("Viser running at http://localhost:8080")

	def update_visualization(joint_q: Sequence[float]) -> None:
		nonlocal last_update_time
		now = time.time()
		if now - last_update_time < (1.0 / vis_update_hz):
			return
		last_update_time = now

		if len(joint_q) < n_joints:
			return

		robot_vis.update_cfg({joint_names[j]: float(joint_q[j]) for j in range(n_joints)})

	return update_visualization

rtde_c = RTDEControl("192.168.56.101")

# Parameters
dt = 1.0/500  # 2ms
lookahead_time = 0.1
gain = 800
joint_q = [-np.pi/2, -np.pi/2, -np.pi/2, -np.pi/2, np.pi/2, 0.0]

# Move to initial joint position with a regular moveJ
rtde_c.moveJ(joint_q)

update_visualization = visualize_trajectory(robot_model="UR5e")

# Continuous small sinusoidal motion on all 6 joints.
joint_q_start = np.array(joint_q, dtype=float)
amplitudes = np.array([0.03, 0.03, 0.03, 0.02, 0.02, 0.02], dtype=float)  # rad
frequencies = np.array([0.5, 0.6, 0.7, 0.8, 0.9, 1.0], dtype=float)  # Hz
phases = np.array([0.0, np.pi / 6.0, np.pi / 3.0, np.pi / 2.0, 2.0 * np.pi / 3.0, 5.0 * np.pi / 6.0], dtype=float)
motion_start_time = time.time()

try:
	while True:
		t_start = rtde_c.initPeriod()
		elapsed = time.time() - motion_start_time
		sine_argument = 2.0 * np.pi * frequencies * elapsed + phases
		joint_q = (joint_q_start + amplitudes * np.sin(sine_argument)).tolist()
		rtde_c.servoJ(joint_q, 0.0, 0.0, dt, lookahead_time, gain)
		update_visualization(joint_q)
		rtde_c.waitPeriod(t_start)
except KeyboardInterrupt:
	print("KeyboardInterrupt received. Stopping robot movement...")
finally:
	rtde_c.servoStop()
	rtde_c.stopScript()
