<div align="center">

<img width="25%" src="doc/_static/ur_rtde_logo.png" alt="ur-rtde-py">

# ur-rtde-py

> Modernized Python packaging of **ur_rtde** — Universal Robots RTDE control with nanobind bindings and one stable-ABI wheel per platform for all CPython ≥ 3.10.

[![CI](../../actions/workflows/ci.yml/badge.svg)](../../actions/workflows/ci.yml)
[![PyPI](https://badgen.net/pypi/v/ur-rtde-py)](https://pypi.org/project/ur-rtde-py/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

</div>

**ur-rtde-py** is a modernized packaging fork of [ur_rtde](https://gitlab.com/sdurobotics/ur_rtde),
the C++ / Python interface for controlling Universal Robots over the RTDE protocol.
It keeps the C++ core and the Python API of upstream `v1.6.5` unchanged, while replacing the
Python build and packaging stack with [nanobind](https://github.com/wjakob/nanobind),
[scikit-build-core](https://github.com/scikit-build/scikit-build-core) and [uv](https://docs.astral.sh/uv/).

## Relationship to ur_rtde

| | upstream [`ur_rtde`](https://gitlab.com/sdurobotics/ur_rtde) | this fork `ur-rtde-py` |
|---|---|---|
| C++ core | ✅ identical (based on `v1.6.5`) | ✅ identical |
| Python API | `rtde_control`, `rtde_receive`, `rtde_io`, `script_client`, `dashboard_client` | same module names, same signatures |
| Binding framework | pybind11 | nanobind |
| Wheel type | one wheel per Python version | **one `cp310-abi3` wheel per platform, all CPython ≥ 3.10** |
| Build backend | setuptools + custom CMake glue | scikit-build-core |
| Dev workflow | pip / setup.py | `uv sync` / `uv build` |
| CI | GitLab CI | GitHub Actions |

In short: **same robot API, modern packaging.** Code written for upstream `ur_rtde` keeps
working with this fork without changes (`import rtde_control`, `import rtde_receive`, …).

> ⚠️ Because both distributions install the same top-level modules, `ur_rtde` (upstream) and
> `ur-rtde-py` **must not be installed in the same environment**. Uninstall one before
> installing the other.

## Why this fork

- **Works out of the box on modern platforms.** Upstream `ur_rtde` does not publish wheels for
  macOS Apple Silicon or for newer CPython releases (3.13 / 3.14), so users there must compile
  from source. This fork ships prebuilt `cp310-abi3` wheels for Apple Silicon and every
  CPython ≥ 3.10 — no compiler, CMake or Boost needed.
- **One wheel per platform, not per Python version.** Wheels are tagged `cp310-abi3`
  (stable ABI): a single Linux x86_64 wheel, for example, works on CPython 3.10, 3.11, 3.12,
  3.13, 3.14 and future releases — no rebuilds when a new Python comes out.
- **nanobind bindings.** Smaller binaries and faster runtime dispatch than pybind11,
  with the same expressive C++ binding API.
- **Reproducible one-command builds.** `uv sync` creates the dev environment and builds the
  bindings; `uv build` produces the sdist and the abi3 wheel.
- **Cross-platform wheels via cibuildwheel.** Linux (manylinux x86_64 / aarch64),
  macOS (x86_64 / Apple Silicon) and Windows x64, built by GitHub Actions.
- **Unchanged core.** The battle-tested C++ implementation, robot compatibility and Python
  API of upstream ur_rtde are kept as-is.

## Installation

```bash
pip install ur-rtde-py
```

or with uv:

```bash
uv pip install ur-rtde-py
```

Requires CPython ≥ 3.10. See the **[user guide](USER_GUIDE.md)** for a full usage walkthrough,
code examples and troubleshooting.

## Supported platforms

| Platform | Wheels | Source build |
|---|---|---|
| Linux x86_64 (manylinux) | ✅ | ✅ |
| Linux aarch64 (manylinux) | ✅ | ✅ |
| macOS x86_64 | ✅ | ✅ |
| macOS Apple Silicon | ✅ | ✅ |
| Windows x64 | ✅ | ✅ |

PyPy and free-threaded CPython (`3.13t` / `3.14t`) are not supported by the abi3 wheels.

## Development

```bash
uv sync          # create .venv, build and install the bindings
uv run python ci/smoke_test.py
uv build         # sdist + cp310-abi3 wheel
```

Building from source requires a C++17 compiler, CMake ≥ 3.15 and Boost headers
(header-only parts are sufficient).

## Documentation

- [User guide](USER_GUIDE.md) — installation, examples, troubleshooting
- Upstream API docs: <https://sdurobotics.gitlab.io/ur_rtde/>

## Credits & license

The robot interface was created by **Anders Prier Lindvig / SDU Robotics** and is published
under the MIT license — see [LICENSE](LICENSE). This fork only modernizes the Python
packaging and CI; all credit for the RTDE implementation belongs to the original authors.
