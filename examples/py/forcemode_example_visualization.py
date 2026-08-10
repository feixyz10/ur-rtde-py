from rtde_control import RTDEControlInterface as RTDEControl
from rtde_receive import RTDEReceiveInterface as RTDEReceive

from typing import Callable, Optional, Sequence
import time
import numpy as np

_MODEL_TO_DESCRIPTION: dict[str, str] = {
	"UR3":   "ur3_description",
	"UR3e":  "ur3e_description",
	"UR5":   "ur5_description",
	"UR5e":  "ur5e_description",
	"UR10":  "ur10_description",
	"UR10e": "ur10e_description",
	"UR16e": "ur16e_description",
}

def visualize_trajectory(robot_model: str = "UR5e"):
	"""Start a viser viewer and return (update_callback, server)."""
	try:
		import viser
		from viser.extras import ViserUrdf
		from robot_descriptions.loaders.yourdfpy import load_robot_description
	except ImportError as exc:
		print(f"Missing dependency for visualization: {exc}")
		print("Run: pip install viser robot_descriptions yourdfpy")
		return lambda _joint_q: None, None

	description_name = _MODEL_TO_DESCRIPTION.get(robot_model)
	if description_name is None:
		print(f"No robot description available for model: {robot_model}")
		return lambda _joint_q: None, None

	server = viser.ViserServer(port=8080)
	#server.scene.add_grid("/ground", width=3, height=3, cell_size=0.1)
	# Apply ROS-vs-real base convention correction: 180 deg yaw around base Z.
	server.scene.add_frame("/robot_base", wxyz=(0.0, 0.0, 0.0, 1.0), position=(0.0, 0.0, 0.0), show_axes=False)

	@server.on_client_connect
	def _(client: viser.ClientHandle) -> None:
		client.camera.position = (-0.902, 0.944, 0.692)
		client.camera.look_at = (-0.018,0.049,0.374)

	print(f"Fetching {robot_model} URDF model...")
	urdf = load_robot_description(description_name)
	robot_vis = ViserUrdf(server, urdf, root_node_name="/robot_base/robot")
	joint_names = urdf.actuated_joint_names
	n_joints = len(joint_names)

	last_update_time = 0.0
	vis_update_hz = 30.0
	trajectory_points: list[list[float]] = []
	trajectory_downsample = 2
	update_count = 0
	trajectory_color = np.array([0.05, 0.80, 0.95], dtype=np.float32)

	print("Viser running at http://localhost:8080")

	def update_visualization(joint_q: Sequence[float], tcp_xyz: Optional[Sequence[float]] = None) -> None:
		nonlocal last_update_time, update_count
		now = time.time()
		if now - last_update_time < (1.0 / vis_update_hz):
			return
		last_update_time = now
		update_count += 1

		if len(joint_q) < n_joints:
			return

		robot_vis.update_cfg({joint_names[j]: float(joint_q[j]) for j in range(n_joints)})

		if tcp_xyz is None or len(tcp_xyz) < 3 or (update_count % trajectory_downsample != 0):
			return

		trajectory_points.append([float(tcp_xyz[0]), float(tcp_xyz[1]), float(tcp_xyz[2])])
		if len(trajectory_points) < 2:
			return

		points = np.asarray(trajectory_points, dtype=np.float32)
		segments = np.stack([points[:-1], points[1:]], axis=1)
		colors = np.tile(trajectory_color, (segments.shape[0], 2, 1))
		server.scene.add_line_segments(
			"/trajectory/downward_spiral",
			points=segments,
			colors=colors,
			line_width=3.0,
		)

	return update_visualization, server

rtde_c = RTDEControl("192.168.56.101")
rtde_r = RTDEReceive("192.168.56.101")

# Task parameters
dt = 1.0/500 # 500Hz control loop
task_frame = [0, 0, 0, 0, 0, 0]
compliance_vector = [1, 1, 1, 0, 0, 0]
init_q = [-np.pi/2, -np.pi/2, -np.pi/2, -np.pi/2, np.pi/2, 0.0]

# Spiral search parameters
F_tangential      = 30.0   # N - tangential force that keeps the robot orbiting
growth_rate_mm_s  = 1.0    # mm/s - desired outward radial growth rate (easy to tune)
z_force           = -5.0   # N - downward force
v_max = [0.5, 0.5, 0.5, 0.2, 0.2, 0.2]

# Derive F_radial from the desired growth rate
F_radial = F_tangential * (growth_rate_mm_s / 1000.0) / v_max[0]

update_visualization, server = visualize_trajectory(robot_model="UR5e")

# Move to initial joint position after the visualizer is up so the robot
# pose in viser matches the real robot from the start
rtde_c.moveJ(init_q)

# Seed the visualizer with the actual pose so the virtual robot is correct
# before the user presses Start
actual_q = rtde_r.getActualQ()
update_visualization(actual_q, rtde_r.getActualTCPPose()[:3])

rtde_c.zeroFtSensor() # zero ft sensor
time.sleep(0.2)

# Record the starting TCP position — the spiral center
start_tcp = rtde_r.getActualTCPPose()
start_x, start_y = start_tcp[0], start_tcp[1]

# Wait for the user to press Start in the viser GUI
import threading
_start_event = threading.Event()
start_button = server.gui.add_button("Start")

@start_button.on_click
def _(_) -> None:
    _start_event.set()

print("Press 'Start' in the viser GUI to begin the spiral search.")
_start_event.wait()

try:
    while True:
        start = time.time()

        # Compute force direction based on current position relative to spiral center.
        actual_tcp_pose = rtde_r.getActualTCPPose()
        dx = actual_tcp_pose[0] - start_x
        dy = actual_tcp_pose[1] - start_y
        r  = np.sqrt(dx**2 + dy**2)

        if r > 0.001:
            cos_phi = dx / r
            sin_phi = dy / r
        else:
            cos_phi, sin_phi = 1.0, 0.0  # initial push in +X

        # Tangential (CCW orbit) + radial (outward expansion)
        Fx = F_tangential * (-sin_phi) + F_radial * cos_phi
        Fy = F_tangential * cos_phi  + F_radial * sin_phi
        Fz = z_force

        F_vec = [Fx, Fy, Fz, 0, 0, 0]

        rtde_c.forceMode(task_frame, compliance_vector, F_vec, 2, v_max)
        actual_q = rtde_r.getActualQ()
        update_visualization(actual_q, actual_tcp_pose[:3])

        end = time.time()
        duration = end - start
        if duration < dt:
            time.sleep(dt - duration)

except KeyboardInterrupt:
    print("Stopped spiral search")
    rtde_c.forceModeStop()

# task_frame = [0, 0, 0, 0, 0, 0]
# selection_vector = [0, 0, 1, 0, 0, 0]
# wrench_down = [0, 0, -10, 0, 0, 0]
# wrench_up = [0, 0, 10, 0, 0, 0]
# force_type = 2
# limits = [2, 2, 1.5, 1, 1, 1]
# joint_q = [-np.pi/2, -np.pi/2, -np.pi/2, -np.pi/2, np.pi/2, 0.0]

# # Move to initial joint position with a regular moveJ
# rtde_c.moveJ(joint_q)

# update_visualization = visualize_trajectory(robot_model="UR5e")

# # Execute 500Hz control loop for 4 seconds, each cycle is 2ms
# for i in range(2000):
#     t_start = rtde_c.initPeriod()
#     # First move the robot down for 2 seconds, then up for 2 seconds
#     if i > 1000:
#         rtde_c.forceMode(task_frame, selection_vector, wrench_up, force_type, limits)
#     else:
#         rtde_c.forceMode(task_frame, selection_vector, wrench_down, force_type, limits)
#     update_visualization(rtde_r.getActualQ())
#     rtde_c.waitPeriod(t_start)

# rtde_c.forceModeStop()
# rtde_c.stopScript()
