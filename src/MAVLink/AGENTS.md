# QGC MAVLink Layer — Agent Guidance

This directory implements QGroundControl's MAVLink protocol handling:
message parsing, FTP, signing, stream configuration, and status reporting.

## Context

- **Parent guidance:** `qgroundcontrol/AGENTS.md` (vendor submodule — read + build + test)
- This is the communication layer between QGC and the autopilot.

## Key files

| File | Purpose |
|------|---------|
| `QGCMAVLink.h/cc` | Core MAVLink connection management |
| `MAVLinkFTP.h/cc` | File transfer over MAVLink |
| `MAVLinkSigning.h/cc` | Message authentication |
| `MAVLinkStreamConfig.h/cc` | Telemetry stream rate configuration |
| `StatusTextHandler.h/cc` | Vehicle status message parsing |
| `SysStatusSensorInfo.h/cc` | Sensor health bit decoding |

## Relevance to px4-sim-suite

- `tools/qgc_virtual_px4.py` validates the MAVLink handshake this layer performs
- `tools/mavlink_heartbeat.py` spoofs the GCS heartbeat this layer normally sends
- Scenario scripts in `tests/scenarios/` use the same MAVLink protocol via pymavlink

## What agents should do here

- **Read** to understand QGC's MAVLink message handling when debugging handshake issues
- **Reference** message IDs and field names when writing pymavlink scenario scripts
- **Trace** connection logic when diagnosing `qgc_virtual_px4.py` test failures

## What agents must not do

- Do not modify MAVLink handling code
- Do not change stream rates or signing behavior