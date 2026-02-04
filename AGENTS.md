# QGroundControl — Agent Guidance

This is a **forked submodule** of the QGroundControl ground control station.
Agents operating in this directory must follow different rules than the parent repo.

---

## Ownership and authority

- **Upstream:** [mavlink/qgroundcontrol](https://github.com/mavlink/qgroundcontrol)
- **Fork:** lpurdy01/qgroundcontrol
- This is a large Qt/C++ application. Treat it as a dependency with targeted customizations.

## What agents must NOT do here

- **Do not commit directly** to this submodule
- Do not modify upstream QGC source without human approval
- Do not update Qt version or QGC internal submodules
- Do not alter the CMake build configuration without explicit instructions
- Do not add or remove QGC plugins

## What agents may do here

- **Read** source code for context (UI components, MAVLink handling, plugin architecture)
- **Build** QGC via CMake when orchestrated by `simtest qgc build`
- **Run** unit tests via `ctest` or `QGroundControl --unittest`
- **Reference** QGC internals when writing integration tests or MAVLink scripts
- **Propose changes** via the patch process defined in the root `AGENTS.md`

## Build context

- Build system: CMake + Ninja
- Qt version: provisioned via `aqtinstall` (see `tools/environment_manifest.json`)
- Test flag: `-DQGC_BUILD_TESTING=ON`
- Output: AppImage at `build/qgc-simtest/`
- Headless execution: via `xvfb-run` (virtual X framebuffer)

## Key directories for reference

| Path | Purpose |
|------|---------|
| `src/` | Main application source |
| `src/MAVLink/` | MAVLink message handling |
| `src/MissionManager/` | Mission planning logic |
| `src/Vehicle/` | Vehicle state and control |
| `deploy/linux/` | Linux packaging scripts |

## Testing from the parent repo

QGC testing is orchestrated via `simtest qgc`:
- `simtest qgc build` — compile with test harness
- `simtest qgc test` — run CTest + unittest harness
- `simtest qgc stub` — validate MAVLink handshake via virtual PX4
- `simtest qgc autoplan` — headless mission execution

## Cross-reference

- Parent repo rules: `/AGENTS.md` (root)
- Patch process: `/AGENTS.md` section 5
- Virtual PX4 stub: `/tools/qgc_virtual_px4.py`
- Virtual joystick guide: `/tools/setup_virtual_joystick.md`
