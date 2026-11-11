#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <ur_rtde/dashboard_client.h>
#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_control_interface_doc.h>
#include <ur_rtde/rtde_io_interface.h>
#include <ur_rtde/rtde_io_interface_doc.h>
#include <ur_rtde/rtde_receive_interface.h>
#include <ur_rtde/rtde_receive_interface_doc.h>
#include <ur_rtde/script_client.h>

namespace nb = nanobind;
using namespace ur_rtde;

namespace rtde_control
{
NB_MODULE(rtde_control, m)
{
  m.doc() = "RTDE Control Interface";

  nb::class_<PathEntry> pentry(m, "PathEntry");
  pentry.def(nb::init<PathEntry::eMoveType, PathEntry::ePositionType, std::vector<double>>(), nb::arg("move_type"), nb::arg("position_type"), nb::arg("parameters"))
      .def("toScriptCode", &PathEntry::toScriptCode, "", nb::call_guard<nb::gil_scoped_release>())
      .def("__repr__", [](const PathEntry &a) { return "<rtde_control.PathEntry>"; });
  nb::enum_<PathEntry::eMoveType>(pentry, "eMoveType")
      .value("MoveJ", PathEntry::eMoveType::MoveJ)
      .value("MoveL", PathEntry::eMoveType::MoveL)
      .value("MoveP", PathEntry::eMoveType::MoveP)
      .value("MoveC", PathEntry::eMoveType::MoveC)
      .export_values();
  nb::enum_<PathEntry::ePositionType>(pentry, "ePositionType")
      .value("PositionTcpPose", PathEntry::ePositionType::PositionTcpPose)
      .value("PositionJoints", PathEntry::ePositionType::PositionJoints)
      .export_values();

  nb::class_<Path>(m, "Path")
      .def(nb::init<>())
      .def("addEntry", &Path::addEntry, "", nb::arg("entry"), nb::call_guard<nb::gil_scoped_release>())
      .def("clear", &Path::clear, "", nb::call_guard<nb::gil_scoped_release>())
      .def("size", &Path::size, "", nb::call_guard<nb::gil_scoped_release>())
      .def("waypoints", &Path::waypoints, "", nb::call_guard<nb::gil_scoped_release>())
      .def("appendMovelPath", &Path::appendMovelPath, "", nb::arg("path"), nb::call_guard<nb::gil_scoped_release>())
      .def("appendMovejPath", &Path::appendMovejPath, "", nb::arg("path"), nb::call_guard<nb::gil_scoped_release>())
      .def("toScriptCode", &Path::toScriptCode, "", nb::call_guard<nb::gil_scoped_release>())
      .def("__repr__", [](const Path &a) { return "<rtde_control.Path>"; });

  nb::class_<AsyncOperationStatus>(m, "AsyncOperationStatus")
      .def(nb::init<int>())
      .def("value", &AsyncOperationStatus::value, "", nb::call_guard<nb::gil_scoped_release>())
      .def("isAsyncOperationRunning", &AsyncOperationStatus::isAsyncOperationRunning, "", nb::call_guard<nb::gil_scoped_release>())
      .def("operationId", &AsyncOperationStatus::operationId, "", nb::call_guard<nb::gil_scoped_release>())
      .def("changeCount", &AsyncOperationStatus::changeCount, "", nb::call_guard<nb::gil_scoped_release>())
      .def("progress", &AsyncOperationStatus::progress, "", nb::call_guard<nb::gil_scoped_release>())
      .def("equals", &AsyncOperationStatus::equals, nb::arg("other"), nb::call_guard<nb::gil_scoped_release>())
      .def("__repr__", [](const AsyncOperationStatus &a) { return "<rtde_control.AsyncOperationStatus>"; });

  nb::class_<RTDEControlInterface> control(m, "RTDEControlInterface");
  nb::enum_<RTDEControlInterface::Flags>(control, "Flags", nb::is_arithmetic())
      .value("FLAG_UPLOAD_SCRIPT", RTDEControlInterface::Flags::FLAG_UPLOAD_SCRIPT)
      .value("FLAG_USE_EXT_UR_CAP", RTDEControlInterface::Flags::FLAG_USE_EXT_UR_CAP)
      .value("FLAG_VERBOSE", RTDEControlInterface::Flags::FLAG_VERBOSE)
      .value("FLAG_UPPER_RANGE_REGISTERS", RTDEControlInterface::Flags::FLAG_UPPER_RANGE_REGISTERS)
      .value("FLAG_NO_WAIT", RTDEControlInterface::Flags::FLAG_NO_WAIT)
      .value("FLAG_CUSTOM_SCRIPT", RTDEControlInterface::Flags::FLAG_CUSTOM_SCRIPT)
      .value("FLAG_NO_EXT_FT", RTDEControlInterface::Flags::FLAG_NO_EXT_FT)
      .value("FLAG_DISABLE_REMOTE_CONTROL_CHECK", RTDEControlInterface::Flags::FLAG_DISABLE_REMOTE_CONTROL_CHECK)
      .value("FLAGS_DEFAULT", RTDEControlInterface::Flags::FLAGS_DEFAULT)
      .export_values();

  nb::enum_<RTDEControlInterface::Feature>(control, "Feature")
      .value("FEATURE_BASE", RTDEControlInterface::Feature::FEATURE_BASE)
      .value("FEATURE_TOOL", RTDEControlInterface::Feature::FEATURE_TOOL)
      .value("FEATURE_CUSTOM", RTDEControlInterface::Feature::FEATURE_CUSTOM)
      .export_values();

  control.def(nb::init<std::string, double, uint16_t, int, int>(), nb::arg("hostname"),
              nb::arg("frequency") = -1.0,
              nb::arg("flags") = RTDEControlInterface::Flags::FLAGS_DEFAULT,
              nb::arg("ur_cap_port") = 50002, nb::arg("rt_priority") = 0);
  control.def("disconnect", &RTDEControlInterface::disconnect, DOC(ur_rtde, RTDEControlInterface, disconnect), nb::call_guard<nb::gil_scoped_release>());
  control.def("reconnect", &RTDEControlInterface::reconnect, DOC(ur_rtde, RTDEControlInterface, reconnect),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("isConnected", &RTDEControlInterface::isConnected, DOC(ur_rtde, RTDEControlInterface, isConnected),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("sendCustomScriptFunction", &RTDEControlInterface::sendCustomScriptFunction,
              DOC(ur_rtde, RTDEControlInterface, sendCustomScriptFunction), nb::call_guard<nb::gil_scoped_release>());
  control.def("sendCustomScript", &RTDEControlInterface::sendCustomScript,
              DOC(ur_rtde, RTDEControlInterface, sendCustomScript), nb::call_guard<nb::gil_scoped_release>());
  control.def("sendCustomScriptFile", &RTDEControlInterface::sendCustomScriptFile,
              DOC(ur_rtde, RTDEControlInterface, sendCustomScriptFile), nb::call_guard<nb::gil_scoped_release>());
  control.def("setCustomScriptFile", &RTDEControlInterface::setCustomScriptFile,
              DOC(ur_rtde, RTDEControlInterface, setCustomScriptFile), nb::call_guard<nb::gil_scoped_release>());
  control.def("stopScript", &RTDEControlInterface::stopScript, DOC(ur_rtde, RTDEControlInterface, stopScript),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("reuploadScript", &RTDEControlInterface::reuploadScript, DOC(ur_rtde, RTDEControlInterface, reuploadScript),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("moveJ",
              (bool (RTDEControlInterface::*)(const std::vector<std::vector<double>> &path, bool asynchronous)) &RTDEControlInterface::moveJ,
              DOC(ur_rtde, RTDEControlInterface, moveJ_2), nb::arg("path"), nb::arg("asynchronous") = false, nb::call_guard<nb::gil_scoped_release>());
  control.def("movePath",
              (bool (RTDEControlInterface::*)(const Path &path, bool asynchronous)) &RTDEControlInterface::movePath, DOC(ur_rtde, RTDEControlInterface, movePath),
              nb::arg("path"), nb::arg("asynchronous") = false, nb::call_guard<nb::gil_scoped_release>());
  control.def("moveJ",
              (bool (RTDEControlInterface::*)(const std::vector<double> &q, double speed, double acceleration, bool asynchronous)) &
                  RTDEControlInterface::moveJ,
              DOC(ur_rtde, RTDEControlInterface, moveJ), nb::arg("q"), nb::arg("speed") = 1.05,
              nb::arg("acceleration") = 1.4, nb::arg("asynchronous") = false, nb::call_guard<nb::gil_scoped_release>());
  control.def("moveJ_IK", &RTDEControlInterface::moveJ_IK, DOC(ur_rtde, RTDEControlInterface, moveJ_IK), nb::arg("pose"),
              nb::arg("speed") = 1.05, nb::arg("acceleration") = 1.4, nb::arg("asynchronous") = false, nb::call_guard<nb::gil_scoped_release>());
  control.def("moveL",
              (bool (RTDEControlInterface::*)(const std::vector<std::vector<double>> &path, bool asynchronous)) &RTDEControlInterface::moveL,
              DOC(ur_rtde, RTDEControlInterface, moveL_2), nb::arg("path"), nb::arg("asynchronous") = false, nb::call_guard<nb::gil_scoped_release>());
  control.def("moveL",
              (bool (RTDEControlInterface::*)(const std::vector<double> &pose, double speed, double acceleration, bool asynchronous)) &
                  RTDEControlInterface::moveL,
              DOC(ur_rtde, RTDEControlInterface, moveL), nb::arg("pose"), nb::arg("speed") = 0.25,
              nb::arg("acceleration") = 1.2, nb::arg("asynchronous") = false, nb::call_guard<nb::gil_scoped_release>());
  control.def("moveL_FK", &RTDEControlInterface::moveL_FK, DOC(ur_rtde, RTDEControlInterface, moveL_FK), nb::arg("q"),
              nb::arg("speed") = 0.25, nb::arg("acceleration") = 1.2, nb::arg("asynchronous") = false, nb::call_guard<nb::gil_scoped_release>());
  control.def("speedJ", &RTDEControlInterface::speedJ, DOC(ur_rtde, RTDEControlInterface, speedJ), nb::arg("qd"),
              nb::arg("acceleration") = 0.5, nb::arg("time") = 0.0, nb::call_guard<nb::gil_scoped_release>());
  control.def("speedL", &RTDEControlInterface::speedL, DOC(ur_rtde, RTDEControlInterface, speedL), nb::arg("xd"),
              nb::arg("acceleration") = 0.25, nb::arg("time") = 0.0, nb::call_guard<nb::gil_scoped_release>());
  control.def("speedStop", &RTDEControlInterface::speedStop, nb::arg("a") = 10.0, DOC(ur_rtde, RTDEControlInterface, speedStop),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("servoJ", &RTDEControlInterface::servoJ, DOC(ur_rtde, RTDEControlInterface, servoJ),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("servoL", &RTDEControlInterface::servoL, DOC(ur_rtde, RTDEControlInterface, servoL),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("servoC", &RTDEControlInterface::servoC, DOC(ur_rtde, RTDEControlInterface, servoC), nb::arg("pose"),
              nb::arg("speed") = 0.25, nb::arg("acceleration") = 1.2, nb::arg("blend") = 0.0,
              nb::call_guard<nb::gil_scoped_release>());
  control.def("servoStop", &RTDEControlInterface::servoStop, nb::arg("a") = 10.0, DOC(ur_rtde, RTDEControlInterface, servoStop),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("forceMode", &RTDEControlInterface::forceMode, DOC(ur_rtde, RTDEControlInterface, forceMode),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("forceModeStop", &RTDEControlInterface::forceModeStop, DOC(ur_rtde, RTDEControlInterface, forceModeStop),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("forceModeSetDamping", &RTDEControlInterface::forceModeSetDamping,
              DOC(ur_rtde, RTDEControlInterface, forceModeSetDamping), nb::call_guard<nb::gil_scoped_release>());
  control.def("toolContact", &RTDEControlInterface::toolContact, DOC(ur_rtde, RTDEControlInterface, toolContact),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("getTargetWaypoint", &RTDEControlInterface::getTargetWaypoint,
              DOC(ur_rtde, RTDEControlInterface, getTargetWaypoint), nb::call_guard<nb::gil_scoped_release>());
  control.def("getActualJointPositionsHistory", &RTDEControlInterface::getActualJointPositionsHistory,
              DOC(ur_rtde, RTDEControlInterface, getActualJointPositionsHistory), nb::call_guard<nb::gil_scoped_release>());
  control.def("getStepTime", &RTDEControlInterface::getStepTime, DOC(ur_rtde, RTDEControlInterface, getStepTime),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("teachMode", &RTDEControlInterface::teachMode, DOC(ur_rtde, RTDEControlInterface, teachMode),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("endTeachMode", &RTDEControlInterface::endTeachMode, DOC(ur_rtde, RTDEControlInterface, endTeachMode),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("isProgramRunning", &RTDEControlInterface::isProgramRunning, DOC(ur_rtde, RTDEControlInterface, isProgramRunning),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("forceModeSetGainScaling", &RTDEControlInterface::forceModeSetGainScaling,
              DOC(ur_rtde, RTDEControlInterface, forceModeSetGainScaling), nb::call_guard<nb::gil_scoped_release>());
  control.def("zeroFtSensor", &RTDEControlInterface::zeroFtSensor, DOC(ur_rtde, RTDEControlInterface, zeroFtSensor),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("setPayload", &RTDEControlInterface::setPayload, DOC(ur_rtde, RTDEControlInterface, setPayload),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("setTcp", &RTDEControlInterface::setTcp, DOC(ur_rtde, RTDEControlInterface, setTcp),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("getInverseKinematics", &RTDEControlInterface::getInverseKinematics,
              DOC(ur_rtde, RTDEControlInterface, getInverseKinematics),
              nb::arg("x"), nb::arg("qnear") = std::vector<double>(),
              nb::arg("max_position_error") = 1e-10, nb::arg("max_orientation_error") = 1e-10,
              nb::call_guard<nb::gil_scoped_release>());
  control.def("poseTrans", &RTDEControlInterface::poseTrans,
              DOC(ur_rtde, RTDEControlInterface, poseTrans), nb::arg("p_from"), nb::arg("p_from_to"),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("triggerProtectiveStop", &RTDEControlInterface::triggerProtectiveStop,
              DOC(ur_rtde, RTDEControlInterface, triggerProtectiveStop), nb::call_guard<nb::gil_scoped_release>());
  control.def("stopL", &RTDEControlInterface::stopL,
              DOC(ur_rtde, RTDEControlInterface, stopL), nb::arg("a") = 10.0, nb::arg("asynchronous") = false, nb::call_guard<nb::gil_scoped_release>());
  control.def("stopJ", &RTDEControlInterface::stopJ,
              DOC(ur_rtde, RTDEControlInterface, stopJ), nb::arg("a") = 2.0, nb::arg("asynchronous") = false, nb::call_guard<nb::gil_scoped_release>());
  control.def("setWatchdog", &RTDEControlInterface::setWatchdog,
              DOC(ur_rtde, RTDEControlInterface, setWatchdog), nb::arg("min_frequency") = 10.0,
              nb::call_guard<nb::gil_scoped_release>());
  control.def("kickWatchdog", &RTDEControlInterface::kickWatchdog,
              DOC(ur_rtde, RTDEControlInterface, kickWatchdog), nb::call_guard<nb::gil_scoped_release>());
  control.def("isPoseWithinSafetyLimits", &RTDEControlInterface::isPoseWithinSafetyLimits,
              DOC(ur_rtde, RTDEControlInterface, isPoseWithinSafetyLimits), nb::call_guard<nb::gil_scoped_release>());
  control.def("isJointsWithinSafetyLimits", &RTDEControlInterface::isJointsWithinSafetyLimits,
              DOC(ur_rtde, RTDEControlInterface, isJointsWithinSafetyLimits), nb::call_guard<nb::gil_scoped_release>());
  control.def("getJointTorques", &RTDEControlInterface::getJointTorques,
              DOC(ur_rtde, RTDEControlInterface, getJointTorques), nb::call_guard<nb::gil_scoped_release>());
  control.def("getTCPOffset", &RTDEControlInterface::getTCPOffset, DOC(ur_rtde, RTDEControlInterface, getTCPOffset),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("getForwardKinematics", &RTDEControlInterface::getForwardKinematics, DOC(ur_rtde, RTDEControlInterface, getForwardKinematics),
              nb::arg("q") = std::vector<double>(), nb::arg("tcp_offset") = std::vector<double>(),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("isSteady", &RTDEControlInterface::isSteady, DOC(ur_rtde, RTDEControlInterface, isSteady), nb::call_guard<nb::gil_scoped_release>());
  control.def("moveUntilContact", &RTDEControlInterface::moveUntilContact, DOC(ur_rtde, RTDEControlInterface, moveUntilContact), nb::arg("xd"),
              nb::arg("direction") = std::vector<double>{0, 0, 0, 0, 0, 0}, nb::arg("acceleration") = 1.2,
              nb::call_guard<nb::gil_scoped_release>());
  control.def("jogStart", &RTDEControlInterface::jogStart, DOC(ur_rtde, RTDEControlInterface, jogStart), nb::arg("speeds"),
              nb::arg("feature") = RTDEControlInterface::Feature::FEATURE_BASE,
              nb::arg("acc") = 0.5,
              nb::arg("custom_frame") = std::vector<double>{0, 0, 0, 0, 0, 0},
              nb::call_guard<nb::gil_scoped_release>());
  control.def("jogStop", &RTDEControlInterface::jogStop, DOC(ur_rtde, RTDEControlInterface, jogStop), nb::call_guard<nb::gil_scoped_release>());
  control.def("freedriveMode", &RTDEControlInterface::freedriveMode, nb::arg("free_axes") = std::vector<int>{1, 1, 1, 1, 1, 1},
              nb::arg("feature") = std::vector<double>{0, 0, 0, 0, 0, 0}, nb::call_guard<nb::gil_scoped_release>());
  control.def("endFreedriveMode", &RTDEControlInterface::endFreedriveMode, nb::call_guard<nb::gil_scoped_release>());
  control.def("getFreedriveStatus", &RTDEControlInterface::getFreedriveStatus, nb::call_guard<nb::gil_scoped_release>());
  control.def("ftRtdeInputEnable", &RTDEControlInterface::ftRtdeInputEnable, nb::arg("enable"),
              nb::arg("sensor_mass") = 0.0,
              nb::arg("sensor_measuring_offset") = std::vector<double>{0.0, 0.0, 0.0},
              nb::arg("sensor_cog") = std::vector<double>{0.0, 0.0, 0.0},
              nb::call_guard<nb::gil_scoped_release>());
  control.def("enableExternalFtSensor", &RTDEControlInterface::enableExternalFtSensor, nb::arg("enable"),
              nb::arg("sensor_mass") = 0.0,
              nb::arg("sensor_measuring_offset") = std::vector<double>{0.0, 0.0, 0.0},
              nb::arg("sensor_cog") = std::vector<double>{0.0, 0.0, 0.0},
              nb::call_guard<nb::gil_scoped_release>());
  control.def("setExternalForceTorque", &RTDEControlInterface::setExternalForceTorque, nb::call_guard<nb::gil_scoped_release>());
  control.def("getAsyncOperationProgress", &RTDEControlInterface::getAsyncOperationProgress, DOC(ur_rtde, RTDEControlInterface, getAsyncOperationProgress), nb::call_guard<nb::gil_scoped_release>());
  control.def("getAsyncOperationProgressEx", &RTDEControlInterface::getAsyncOperationProgressEx, DOC(ur_rtde, RTDEControlInterface, getAsyncOperationProgressEx), nb::call_guard<nb::gil_scoped_release>());
  control.def("getRobotStatus", &RTDEControlInterface::getRobotStatus, DOC(ur_rtde, RTDEControlInterface, getRobotStatus), nb::call_guard<nb::gil_scoped_release>());
  control.def("getActualToolFlangePose", &RTDEControlInterface::getActualToolFlangePose, nb::call_guard<nb::gil_scoped_release>());
  control.def("setGravity", &RTDEControlInterface::setGravity, nb::call_guard<nb::gil_scoped_release>());
  control.def("initPeriod", &RTDEControlInterface::initPeriod, nb::call_guard<nb::gil_scoped_release>());
  control.def("waitPeriod", &RTDEControlInterface::waitPeriod, nb::call_guard<nb::gil_scoped_release>());
  control.def("getInverseKinematicsHasSolution", &RTDEControlInterface::getInverseKinematicsHasSolution,
              nb::arg("x"), nb::arg("qnear") = std::vector<double>(),
              nb::arg("max_position_error") = 1e-10, nb::arg("max_orientation_error") = 1e-10,
              nb::call_guard<nb::gil_scoped_release>());
  control.def("setTargetPayload", &RTDEControlInterface::setTargetPayload,
              nb::arg("mass"), nb::arg("cog") = std::vector<double>(),
              nb::arg("inertia") = std::vector<double>{0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
              nb::call_guard<nb::gil_scoped_release>());
  control.def("directTorque", &RTDEControlInterface::directTorque,
              nb::arg("torque"),
              nb::arg("viscous_scale") = std::vector<double>{0.9, 0.9, 0.8, 0.9, 0.9, 0.9},
              nb::arg("coulomb_scale") = std::vector<double>{0.8, 0.8, 0.7, 0.8, 0.8, 0.8},
              nb::call_guard<nb::gil_scoped_release>());
  control.def("getMassMatrix", &RTDEControlInterface::getMassMatrix,
              nb::arg("q") = std::vector<double>(), nb::arg("include_rotors_inertia") = false,
              nb::call_guard<nb::gil_scoped_release>());
  control.def("getCoriolisAndCentrifugalTorques", &RTDEControlInterface::getCoriolisAndCentrifugalTorques,
              nb::arg("q") = std::vector<double>(), nb::arg("qd") = std::vector<double>(),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("getTargetJointAccelerations", &RTDEControlInterface::getTargetJointAccelerations,
              nb::call_guard<nb::gil_scoped_release>());
  control.def("getJacobian", &RTDEControlInterface::getJacobian,
              nb::arg("pos") = std::vector<double>(), nb::arg("tcp") = std::vector<double>(),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("getJacobianTimeDerivative", &RTDEControlInterface::getJacobianTimeDerivative,
              nb::arg("pos") = std::vector<double>(), nb::arg("vel") = std::vector<double>(), nb::arg("tcp") = std::vector<double>(),
              nb::call_guard<nb::gil_scoped_release>());
  control.def("__repr__", [](const RTDEControlInterface &a) { return "<rtde_control.RTDEControlInterface>"; });
  control.def("startContactDetection", &RTDEControlInterface::startContactDetection, nb::arg("direction") = std::vector<double>{0, 0, 0, 0, 0, 0}, nb::call_guard<nb::gil_scoped_release>());
  control.def("readContactDetection", &RTDEControlInterface::readContactDetection, nb::call_guard<nb::gil_scoped_release>());
  control.def("stopContactDetection", &RTDEControlInterface::stopContactDetection, nb::call_guard<nb::gil_scoped_release>());
}
};  // namespace rtde_control

namespace rtde_receive
{
NB_MODULE(rtde_receive, m)
{
  m.doc() = "RTDE Receive Interface";
  nb::class_<RTDEReceiveInterface>(m, "RTDEReceiveInterface")
      .def(nb::init<std::string, double, std::vector<std::string>, bool, bool, int>(), nb::arg("hostname"),
           nb::arg("frequency") = -1.0,
           nb::arg("variables") = std::vector<std::string>(), nb::arg("verbose") = false,
           nb::arg("use_upper_range_registers") = false,
           nb::arg("rt_priority") = 0)
      .def("disconnect", &RTDEReceiveInterface::disconnect, nb::call_guard<nb::gil_scoped_release>())
      .def("reconnect", &RTDEReceiveInterface::reconnect, DOC(ur_rtde, RTDEReceiveInterface, reconnect),
           nb::call_guard<nb::gil_scoped_release>())
      .def("startFileRecording", &RTDEReceiveInterface::startFileRecording, nb::arg("filename"),
           nb::arg("variables") = std::vector<std::string>(), nb::call_guard<nb::gil_scoped_release>())
      .def("stopFileRecording", &RTDEReceiveInterface::stopFileRecording, nb::call_guard<nb::gil_scoped_release>())
      .def("isConnected", &RTDEReceiveInterface::isConnected, DOC(ur_rtde, RTDEReceiveInterface, isConnected),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getTimestamp", &RTDEReceiveInterface::getTimestamp, DOC(ur_rtde, RTDEReceiveInterface, getTimestamp),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getTargetQ", &RTDEReceiveInterface::getTargetQ, DOC(ur_rtde, RTDEReceiveInterface, getTargetQ),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getTargetQd", &RTDEReceiveInterface::getTargetQd, DOC(ur_rtde, RTDEReceiveInterface, getTargetQd),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getTargetQdd", &RTDEReceiveInterface::getTargetQdd, DOC(ur_rtde, RTDEReceiveInterface, getTargetQdd),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getTargetCurrent", &RTDEReceiveInterface::getTargetCurrent,
           DOC(ur_rtde, RTDEReceiveInterface, getTargetCurrent), nb::call_guard<nb::gil_scoped_release>())
      .def("getTargetMoment", &RTDEReceiveInterface::getTargetMoment,
           DOC(ur_rtde, RTDEReceiveInterface, getTargetMoment), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualQ", &RTDEReceiveInterface::getActualQ, DOC(ur_rtde, RTDEReceiveInterface, getActualQ),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getActualQd", &RTDEReceiveInterface::getActualQd, DOC(ur_rtde, RTDEReceiveInterface, getActualQd),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getActualCurrent", &RTDEReceiveInterface::getActualCurrent,
           DOC(ur_rtde, RTDEReceiveInterface, getActualCurrent), nb::call_guard<nb::gil_scoped_release>())
      .def("getJointControlOutput", &RTDEReceiveInterface::getJointControlOutput,
           DOC(ur_rtde, RTDEReceiveInterface, getJointControlOutput), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualTCPPose", &RTDEReceiveInterface::getActualTCPPose,
           DOC(ur_rtde, RTDEReceiveInterface, getActualTCPPose), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualTCPSpeed", &RTDEReceiveInterface::getActualTCPSpeed,
           DOC(ur_rtde, RTDEReceiveInterface, getActualTCPSpeed), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualTCPForce", &RTDEReceiveInterface::getActualTCPForce,
           DOC(ur_rtde, RTDEReceiveInterface, getActualTCPForce), nb::call_guard<nb::gil_scoped_release>())
      .def("getTargetTCPPose", &RTDEReceiveInterface::getTargetTCPPose,
           DOC(ur_rtde, RTDEReceiveInterface, getTargetTCPPose), nb::call_guard<nb::gil_scoped_release>())
      .def("getTargetTCPSpeed", &RTDEReceiveInterface::getTargetTCPSpeed,
           DOC(ur_rtde, RTDEReceiveInterface, getTargetTCPSpeed), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualDigitalInputBits", &RTDEReceiveInterface::getActualDigitalInputBits,
           DOC(ur_rtde, RTDEReceiveInterface, getActualDigitalInputBits), nb::call_guard<nb::gil_scoped_release>())
      .def("getDigitalInState", &RTDEReceiveInterface::getDigitalInState, nb::call_guard<nb::gil_scoped_release>())
      .def("getJointTemperatures", &RTDEReceiveInterface::getJointTemperatures,
           DOC(ur_rtde, RTDEReceiveInterface, getJointTemperatures), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualExecutionTime", &RTDEReceiveInterface::getActualExecutionTime,
           DOC(ur_rtde, RTDEReceiveInterface, getActualExecutionTime), nb::call_guard<nb::gil_scoped_release>())
      .def("getRobotMode", &RTDEReceiveInterface::getRobotMode, DOC(ur_rtde, RTDEReceiveInterface, getRobotMode),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getJointMode", &RTDEReceiveInterface::getJointMode, DOC(ur_rtde, RTDEReceiveInterface, getJointMode),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getSafetyMode", &RTDEReceiveInterface::getSafetyMode, DOC(ur_rtde, RTDEReceiveInterface, getSafetyMode),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getSafetyStatusBits", &RTDEReceiveInterface::getSafetyStatusBits,
           DOC(ur_rtde, RTDEReceiveInterface, getSafetyStatusBits),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getActualToolAccelerometer", &RTDEReceiveInterface::getActualToolAccelerometer,
           DOC(ur_rtde, RTDEReceiveInterface, getActualToolAccelerometer), nb::call_guard<nb::gil_scoped_release>())
      .def("getSpeedScaling", &RTDEReceiveInterface::getSpeedScaling,
           DOC(ur_rtde, RTDEReceiveInterface, getSpeedScaling), nb::call_guard<nb::gil_scoped_release>())
      .def("getTargetSpeedFraction", &RTDEReceiveInterface::getTargetSpeedFraction,
           DOC(ur_rtde, RTDEReceiveInterface, getTargetSpeedFraction), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualMomentum", &RTDEReceiveInterface::getActualMomentum,
           DOC(ur_rtde, RTDEReceiveInterface, getActualMomentum), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualMainVoltage", &RTDEReceiveInterface::getActualMainVoltage,
           DOC(ur_rtde, RTDEReceiveInterface, getActualMainVoltage), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualRobotVoltage", &RTDEReceiveInterface::getActualRobotVoltage,
           DOC(ur_rtde, RTDEReceiveInterface, getActualRobotVoltage), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualRobotCurrent", &RTDEReceiveInterface::getActualRobotCurrent,
           DOC(ur_rtde, RTDEReceiveInterface, getActualRobotCurrent), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualJointVoltage", &RTDEReceiveInterface::getActualJointVoltage,
           DOC(ur_rtde, RTDEReceiveInterface, getActualJointVoltage), nb::call_guard<nb::gil_scoped_release>())
      .def("getActualDigitalOutputBits", &RTDEReceiveInterface::getActualDigitalOutputBits,
           DOC(ur_rtde, RTDEReceiveInterface, getActualDigitalOutputBits), nb::call_guard<nb::gil_scoped_release>())
      .def("getDigitalOutState", &RTDEReceiveInterface::getDigitalOutState, nb::call_guard<nb::gil_scoped_release>())
      .def("getRuntimeState", &RTDEReceiveInterface::getRuntimeState,
           DOC(ur_rtde, RTDEReceiveInterface, getRuntimeState), nb::call_guard<nb::gil_scoped_release>())
      .def("getRobotStatus", &RTDEReceiveInterface::getRobotStatus,
           DOC(ur_rtde, RTDEReceiveInterface, getRobotStatus), nb::call_guard<nb::gil_scoped_release>())
      .def("getStandardAnalogInput0", &RTDEReceiveInterface::getStandardAnalogInput0,
           DOC(ur_rtde, RTDEReceiveInterface, getStandardAnalogInput0), nb::call_guard<nb::gil_scoped_release>())
      .def("getStandardAnalogInput1", &RTDEReceiveInterface::getStandardAnalogInput1,
           DOC(ur_rtde, RTDEReceiveInterface, getStandardAnalogInput1), nb::call_guard<nb::gil_scoped_release>())
      .def("getStandardAnalogOutput0", &RTDEReceiveInterface::getStandardAnalogOutput0,
           DOC(ur_rtde, RTDEReceiveInterface, getStandardAnalogOutput0), nb::call_guard<nb::gil_scoped_release>())
      .def("getStandardAnalogOutput1", &RTDEReceiveInterface::getStandardAnalogOutput1,
           DOC(ur_rtde, RTDEReceiveInterface, getStandardAnalogOutput1), nb::call_guard<nb::gil_scoped_release>())
      .def("isProtectiveStopped", &RTDEReceiveInterface::isProtectiveStopped,
           DOC(ur_rtde, RTDEReceiveInterface, isProtectiveStopped), nb::call_guard<nb::gil_scoped_release>())
      .def("isEmergencyStopped", &RTDEReceiveInterface::isEmergencyStopped,
           DOC(ur_rtde, RTDEReceiveInterface, isEmergencyStopped), nb::call_guard<nb::gil_scoped_release>())
      .def("getOutputIntRegister", &RTDEReceiveInterface::getOutputIntRegister,
           DOC(ur_rtde, RTDEReceiveInterface, getOutputIntRegister),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getOutputDoubleRegister", &RTDEReceiveInterface::getOutputDoubleRegister,
           DOC(ur_rtde, RTDEReceiveInterface, getOutputDoubleRegister),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getSpeedScalingCombined", &RTDEReceiveInterface::getSpeedScalingCombined,
           DOC(ur_rtde, RTDEReceiveInterface, getSpeedScalingCombined),
           nb::call_guard<nb::gil_scoped_release>())
      .def("getFtRawWrench", &RTDEReceiveInterface::getFtRawWrench,
           nb::call_guard<nb::gil_scoped_release>())
      .def("getPayload", &RTDEReceiveInterface::getPayload,
           nb::call_guard<nb::gil_scoped_release>())
      .def("getPayloadCog", &RTDEReceiveInterface::getPayloadCog,
           nb::call_guard<nb::gil_scoped_release>())
      .def("getPayloadInertia", &RTDEReceiveInterface::getPayloadInertia,
           nb::call_guard<nb::gil_scoped_release>())
      .def("initPeriod", &RTDEReceiveInterface::initPeriod,
           nb::call_guard<nb::gil_scoped_release>())
      .def("waitPeriod", &RTDEReceiveInterface::waitPeriod,
           nb::call_guard<nb::gil_scoped_release>())
      .def("__repr__", [](const RTDEReceiveInterface &a) { return "<rtde_receive.RTDEReceiveInterface>"; });
}
};  // namespace rtde_receive

namespace rtde_io
{
NB_MODULE(rtde_io, m)
{
  m.doc() = "RTDE IO Interface";
  nb::class_<RTDEIOInterface>(m, "RTDEIOInterface")
      .def(nb::init<std::string, bool, bool>(), nb::arg("hostname"), nb::arg("verbose") = false,
           nb::arg("use_upper_range_registers") = false)
      .def("reconnect", &RTDEIOInterface::reconnect, DOC(ur_rtde, RTDEIOInterface, reconnect),
           nb::call_guard<nb::gil_scoped_release>())
      .def("disconnect", &RTDEIOInterface::disconnect, nb::call_guard<nb::gil_scoped_release>())
      .def("isConnected", &RTDEIOInterface::isConnected, nb::call_guard<nb::gil_scoped_release>())
      .def("setStandardDigitalOut", &RTDEIOInterface::setStandardDigitalOut,
           DOC(ur_rtde, RTDEIOInterface, setStandardDigitalOut), nb::call_guard<nb::gil_scoped_release>())
      .def("setToolDigitalOut", &RTDEIOInterface::setToolDigitalOut, DOC(ur_rtde, RTDEIOInterface, setToolDigitalOut),
           nb::call_guard<nb::gil_scoped_release>())
      .def("setSpeedSlider", &RTDEIOInterface::setSpeedSlider, DOC(ur_rtde, RTDEIOInterface, setSpeedSlider),
           nb::call_guard<nb::gil_scoped_release>())
      .def("setAnalogOutputVoltage", &RTDEIOInterface::setAnalogOutputVoltage,
           DOC(ur_rtde, RTDEIOInterface, setAnalogOutputVoltage), nb::call_guard<nb::gil_scoped_release>())
      .def("setAnalogOutputCurrent", &RTDEIOInterface::setAnalogOutputCurrent,
           DOC(ur_rtde, RTDEIOInterface, setAnalogOutputCurrent), nb::call_guard<nb::gil_scoped_release>())
      .def("setConfigurableDigitalOut", &RTDEIOInterface::setConfigurableDigitalOut,
           DOC(ur_rtde, RTDEIOInterface, setConfigurableDigitalOut),
           nb::call_guard<nb::gil_scoped_release>())
      .def("setInputIntRegister", &RTDEIOInterface::setInputIntRegister,
           DOC(ur_rtde, RTDEIOInterface, setInputIntRegister),
           nb::call_guard<nb::gil_scoped_release>())
      .def("setInputDoubleRegister", &RTDEIOInterface::setInputDoubleRegister,
           DOC(ur_rtde, RTDEIOInterface, setInputDoubleRegister),
           nb::call_guard<nb::gil_scoped_release>())
      .def("__repr__", [](const RTDEIOInterface &a) { return "<rtde_io.RTDEIOInterface>"; });
}
};  // namespace rtde_io

namespace script_client
{
NB_MODULE(script_client, m)
{
  m.doc() = "Script Client";
  nb::class_<ScriptClient>(m, "ScriptClient")
      .def(nb::init<std::string, uint32_t, uint32_t, int, bool>(), nb::arg("hostname"),
           nb::arg("major_control_version"), nb::arg("minor_control_version"), nb::arg("port") = 30003,
           nb::arg("verbose") = false)
      .def("connect", &ScriptClient::connect, nb::call_guard<nb::gil_scoped_release>())
      .def("isConnected", &ScriptClient::isConnected, nb::call_guard<nb::gil_scoped_release>())
      .def("disconnect", &ScriptClient::disconnect, nb::call_guard<nb::gil_scoped_release>())
      .def("setScriptFile", (bool (ScriptClient::*)()) &ScriptClient::setScriptFile,
           nb::call_guard<nb::gil_scoped_release>())
      .def("sendScript", (bool (ScriptClient::*)()) &ScriptClient::sendScript,
           nb::call_guard<nb::gil_scoped_release>())
      .def("sendScript", (bool (ScriptClient::*)(const std::string &file_name)) &ScriptClient::sendScript,
           nb::call_guard<nb::gil_scoped_release>())
      .def("sendScriptCommand", &ScriptClient::sendScriptCommand, nb::call_guard<nb::gil_scoped_release>())
      .def("getScript", &ScriptClient::getScript, nb::call_guard<nb::gil_scoped_release>())
      .def("__repr__", [](const ScriptClient &a) { return "<script_client.ScriptClient>"; });
}
};  // namespace script_client

namespace dashboard_client
{
NB_MODULE(dashboard_client, m)
{
  m.doc() = "Dashboard Client";
  nb::class_<DashboardClient>(m, "DashboardClient")
      .def(nb::init<std::string, int, bool>(), nb::arg("hostname"), nb::arg("port") = 29999, nb::arg("verbose") = false)
      .def("connect", &DashboardClient::connect, nb::arg("timeout_ms") = 2000, nb::call_guard<nb::gil_scoped_release>())
      .def("isConnected", &DashboardClient::isConnected, nb::call_guard<nb::gil_scoped_release>())
      .def("disconnect", &DashboardClient::disconnect, nb::call_guard<nb::gil_scoped_release>())
      .def("send", &DashboardClient::send, nb::call_guard<nb::gil_scoped_release>())
      .def("receive", &DashboardClient::receive, nb::call_guard<nb::gil_scoped_release>())
      .def("loadURP", &DashboardClient::loadURP, nb::call_guard<nb::gil_scoped_release>())
      .def("play", &DashboardClient::play, nb::call_guard<nb::gil_scoped_release>())
      .def("stop", &DashboardClient::stop, nb::call_guard<nb::gil_scoped_release>())
      .def("pause", &DashboardClient::pause, nb::call_guard<nb::gil_scoped_release>())
      .def("quit", &DashboardClient::quit, nb::call_guard<nb::gil_scoped_release>())
      .def("shutdown", &DashboardClient::shutdown, nb::call_guard<nb::gil_scoped_release>())
      .def("running", &DashboardClient::running, nb::call_guard<nb::gil_scoped_release>())
      .def("popup", &DashboardClient::popup, nb::call_guard<nb::gil_scoped_release>())
      .def("closePopup", &DashboardClient::closePopup, nb::call_guard<nb::gil_scoped_release>())
      .def("closeSafetyPopup", &DashboardClient::closeSafetyPopup, nb::call_guard<nb::gil_scoped_release>())
      .def("powerOn", &DashboardClient::powerOn, nb::call_guard<nb::gil_scoped_release>())
      .def("powerOff", &DashboardClient::powerOff, nb::call_guard<nb::gil_scoped_release>())
      .def("brakeRelease", &DashboardClient::brakeRelease, nb::call_guard<nb::gil_scoped_release>())
      .def("unlockProtectiveStop", &DashboardClient::unlockProtectiveStop, nb::call_guard<nb::gil_scoped_release>())
      .def("restartSafety", &DashboardClient::restartSafety, nb::call_guard<nb::gil_scoped_release>())
      .def("polyscopeVersion", &DashboardClient::polyscopeVersion, nb::call_guard<nb::gil_scoped_release>())
      .def("programState", &DashboardClient::programState, nb::call_guard<nb::gil_scoped_release>())
      .def("robotmode", &DashboardClient::robotmode, nb::call_guard<nb::gil_scoped_release>())
      .def("getRobotModel", &DashboardClient::getRobotModel, nb::call_guard<nb::gil_scoped_release>())
      .def("getLoadedProgram", &DashboardClient::getLoadedProgram, nb::call_guard<nb::gil_scoped_release>())
      .def("safetymode", &DashboardClient::safetymode, nb::call_guard<nb::gil_scoped_release>())
      .def("safetystatus", &DashboardClient::safetystatus, nb::call_guard<nb::gil_scoped_release>())
      .def("addToLog", &DashboardClient::addToLog, nb::call_guard<nb::gil_scoped_release>())
      .def("isProgramSaved", &DashboardClient::isProgramSaved, nb::call_guard<nb::gil_scoped_release>())
      .def("isInRemoteControl", &DashboardClient::isInRemoteControl, nb::call_guard<nb::gil_scoped_release>())
      .def("setUserRole", &DashboardClient::setUserRole, nb::call_guard<nb::gil_scoped_release>())
      .def("getSerialNumber", &DashboardClient::getSerialNumber, nb::call_guard<nb::gil_scoped_release>())
      .def("__repr__", [](const DashboardClient &a) { return "<dashboard_client.DashboardClient>"; });
}
};  // namespace dashboard_client
