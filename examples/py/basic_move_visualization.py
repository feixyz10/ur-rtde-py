import threading
import time
import numpy as np
from rtde_control import RTDEControlInterface as RTDEControl
from rtde_receive import RTDEReceiveInterface as RTDEReceive

from typing import Callable, Optional, Sequence

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
    """Start a viser viewer and return (update_callback, clear_trajectory_fn, add_replay_button_fn)."""
    try:
        import viser
        from viser.extras import ViserUrdf
        from robot_descriptions.loaders.yourdfpy import load_robot_description
    except ImportError as exc:
        print(f"Missing dependency for visualization: {exc}")
        print("Run: pip install viser robot_descriptions yourdfpy")
        noop = lambda *a, **kw: None
        return noop, noop, noop

    description_name = _MODEL_TO_DESCRIPTION.get(robot_model)
    if description_name is None:
        print(f"No robot description available for model: {robot_model}")
        noop = lambda *a, **kw: None
        return noop, noop, noop

    server = viser.ViserServer(port=8080)

    server.scene.add_frame("/robot_base", wxyz=(0.0, 0.0, 0.0, 1.0), position=(0.0, 0.0, 0.0), show_axes=False)

    @server.on_client_connect
    def _(client: viser.ClientHandle) -> None:
        client.camera.position = (-0.902, 0.944, 0.692)
        client.camera.look_at = (-0.018, 0.049, 0.374)

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
    trajectory_handle = None

    print("Viser running at http://localhost:8080")

    def update_visualization(joint_q: Sequence[float], tcp_xyz: Optional[Sequence[float]] = None) -> None:
        nonlocal last_update_time, update_count, trajectory_handle
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
        trajectory_handle = server.scene.add_line_segments(
            "/trajectory/basic_move",
            points=segments,
            colors=colors,
            line_width=3.0,
        )

    def clear_trajectory() -> None:
        nonlocal trajectory_handle
        trajectory_points.clear()
        if trajectory_handle is not None:
            trajectory_handle.remove()
            trajectory_handle = None

    def add_replay_button(replay_fn: Callable[[], None]) -> None:
        replay_btn = server.gui.add_button("Replay")
        clear_btn = server.gui.add_button("Clear Trajectory")

        @replay_btn.on_click
        def _(_) -> None:
            clear_trajectory()
            threading.Thread(target=replay_fn, daemon=True).start()

        @clear_btn.on_click
        def _(_) -> None:
            clear_trajectory()

    return update_visualization, clear_trajectory, add_replay_button


rtde_c = RTDEControl("192.168.56.101")
rtde_r = RTDEReceive("192.168.56.101")

update_visualization, clear_trajectory, add_replay_button = visualize_trajectory(robot_model="UR5e")

home_q = [-np.pi/2, -np.pi/2, -np.pi/2, -np.pi/2, np.pi/2, 0.0]

def run_motion() -> None:
    """Execute the basic move sequence."""
    rtde_c.moveJ(home_q, 0.5, 0.5)

    tcp_pose = rtde_r.getActualTCPPose()
    print("TCP pose at home:", tcp_pose[:3])

    pose_down = tcp_pose[:]
    pose_down[2] -= 0.15
    rtde_c.moveL(pose_down, 0.25, 0.5)

    pose_side = pose_down[:]
    pose_side[0] += 0.10
    rtde_c.moveL(pose_side, 0.25, 0.5)

    rtde_c.moveL(tcp_pose, 0.25, 0.5)
    rtde_c.moveJ(home_q, 0.5, 0.5)

add_replay_button(run_motion)

# Background thread: continuously polls robot state and feeds the visualizer.
_poll_active = True

def _poll_loop() -> None:
    while _poll_active:
        joint_q = rtde_r.getActualQ()
        tcp_pose = rtde_r.getActualTCPPose()
        update_visualization(joint_q, tcp_pose[:3])
        time.sleep(1.0 / 60.0)

poll_thread = threading.Thread(target=_poll_loop, daemon=True)
poll_thread.start()

# Run the motion once on startup
run_motion()

# Keep running so viser stays alive and the replay button works
print("Motion complete. Press Ctrl+C to exit.")
try:
    while True:
        time.sleep(1.0)
except KeyboardInterrupt:
    pass

_poll_active = False
rtde_c.stopScript()

