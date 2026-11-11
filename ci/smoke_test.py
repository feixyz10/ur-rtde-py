import rtde_control
import rtde_receive
import rtde_io
import script_client
import dashboard_client


modules = (rtde_control, rtde_receive, rtde_io, script_client, dashboard_client)
assert all(module.__name__ for module in modules)

entry = rtde_control.PathEntry(
    rtde_control.PathEntry.eMoveType.MoveJ,
    rtde_control.PathEntry.ePositionType.PositionTcpPose,
    [0.1] * 6 + [1.05, 1.4, 0.0],
)
path = rtde_control.Path()
path.addEntry(entry)

assert path.size() == 1
assert len(path.waypoints()) == 1
assert "movej(p[0.1,0.1,0.1,0.1,0.1,0.1],a=1.4,v=1.05,r=0)" in entry.toScriptCode()

flags = rtde_control.RTDEControlInterface.Flags
assert int(flags.FLAG_UPLOAD_SCRIPT | flags.FLAG_VERBOSE) == 5

print("smoke test passed")
