# QGC Mission Manager — Agent Guidance

This directory implements QGroundControl's mission planning engine:
mission items, survey patterns, camera control, and plan file I/O.

## Context

- **Parent guidance:** `qgroundcontrol/AGENTS.md` (vendor submodule — read + build + test)
- This module reads/writes `.plan` files and uploads missions to the autopilot via MAVLink.

## Key files

| File | Purpose |
|------|---------|
| `MissionController.*` | Top-level mission management |
| `SimpleMissionItem.*` | Single waypoint / command items |
| `ComplexMissionItem.*` | Survey patterns, corridors |
| `PlanManager.*` | Plan upload/download to vehicle |
| `CameraCalc.*` | Camera footprint calculations |

## Relevance to px4-sim-suite

- `tests/qgc_plans/takeoff_land.plan` is a `.plan` file consumed by this module
- `simtest qgc autoplan` triggers headless mission upload and execution through this code path
- `HeadlessMissionRunner` (in `qgroundcontrol/src/`) drives this module without the GUI

## What agents should do here

- **Read** to understand `.plan` file format when creating new QGC mission plans
- **Reference** mission item types when adding scenarios to `tests/qgc_plans/`
- **Trace** upload logic when debugging `simtest qgc autoplan` failures

## What agents must not do

- Do not modify mission manager source code
- Do not change plan file format handling