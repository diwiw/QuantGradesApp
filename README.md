# QuantGradesApp

> **Version:** `v1.0.0-dev`
> **Build System:** CMake + Ninja
> **Standard:** C++23
> **Documentation:** Doxygen + Mermaid + Graphviz
> **Testing:** doctest (unit + E2E)
> **Platforms:** Linux, Windows (MSVC+vcpkg)
> **Architecture:** Modular, layered, Clean Architecture
> **Performance Tools:** perf + FlameGraph (Linux, profiling preset)

QuantGradesApp is a modular, extensible quantitative backtesting framework written in modern C++.
Originally built as a grades/statistics demo, it has evolved into a clean, layered architecture suitable for:
  - quantitative research
  - backtesting trading strategies
  - portfolio & risk analytics
  - data ingestion
  - reporting
  - building HPC-oriented extensions

This release v0.9.0 introduces major quality upgrades, new metrics, improved ingestion, test suite stabilization, and a full CPU profiling pipeline.
---
# What's New in v0.9.0
## New performance profiling tools

- perf_setup.sh
- perf_flamegraph.sh
- perf_hotspot.sh
- CMake preset: linux-profiling
- VSCode task: Build Profiling (vcpkg)

## Expanded Statistics module

- Maximum Drawdown (MDD)
- CAGR
- Sharpe Ratio
- Sortino Ratio
- Hit Ratio
- Additional validation and tests

## CLI improvements

- `--config` is now required
- Better error messages
- CLI tests rewritten and fixed

## Data ingestion improvements

- Strict CSV validation
- Fixed row parsing for malformed inputs
- Preparation for mock HTTP server tests (not yet used in CI)
- Improved error handling and reporting

## Test suite updates (v0.9.0)

- Updated fixtures for new Config schema
- Stabilized E2E tests (Linux)
- Added NullLogger for deterministic outputs
- Improved Statistics tests

## Formatting & code quality

- clang-format enforced on all modules
- clang-tidy baseline added
- CI workflows updated for formatting compliance

---

# Features (v0.9.0)
The 0.9.0 milestone includes:

### Profiling & Performance Tools
- perf integration
- Flamegraph & hotspots scripting
- Userspace-only profiling mode
- Unified profiling pipeline (tools/profiling/)

## Module Overview

### `core/`
Shared, foundational infrastructure:
- `Config` – JSON/ENV configuration
- `Statistics` – metrics calculations
- `Platform` – platform utilities
- `Version` – semantic versioning info

### `domain/`
Business logic and quantitative model:
- **backtest:** BarSeries, Engine, Execution, Portfolio, Order, Trade, Position, Result, Instrument, Quote
- **strategy:** IStrategy, Buy & Hold, Moving Average Crossover

### `ingest/`
Data acquisition layer:
- `DataIngest` orchestrates ingestion

### `io/`
Low-level file operations:
- `CsvLoader`
- `FileManager`
- `DataExporter`

### `persistence/`
Database abstraction:
- `IDataStore` (interface)
- `SQLiteStore`
- `Statement` wrapper
- `DatabaseWorker` (async)

### `reporting/`
Observer-based reporting:
- `IReporter`
- `ReporterManager`

### `utils/`
Technical utilities:
- `ILogger`, `SpdLogger`, `NullLogger`, `MockLogger`
- `LoggerFactory`

---

# Architecture Overview

````mermaid
flowchart TD
    A[BarSeries] --> B[Engine]
    B --> C[Strategy]
    C --> D[Execution]
    D --> E[Portfolio]
    E --> F[Result]
    F --> G[DataExporter]
````

---

# Class Diagram

See `docs/pages/class_diagram.md` for extended diagrams.

````mermaid
classDiagram
    class Engine
    class BarSeries
    class Strategy
    class Portfolio
    class Order
    class Trade
    class Execution

    Engine --> BarSeries
    Engine --> Strategy
    Portfolio --> Trade
    Portfolio --> Order
````

---

# Project Structure

```
QuantGradesApp/
├─ .github/                                 # CI/CD pipelines (Ubuntu/Windows)
├─ build/                                   # Out-of-source build directory
├─ changelog/                               # Version changelogs
├─ config/                                  # Runtime configuration profiles (dev/test/prod)
├─ data/                                    # Sample datasets (optional)
├─ docs/                                    # Doxygen, diagrams, developer notes
│ ├─ diagrams/
│ ├─ pages/
│ ├─ developer_notes.md
│ └─ *.md / .dox
├─ external/                                # Third-party libs (header-only, vendored)
│ └─ doctest.h
├─ include/                                 # Public headers (installed by project)
│ ├─ common/
│ ├─ core/
│ ├─ domain/
│ ├─ ingest/
│ ├─ io/
│ ├─ persistence/
│ ├─ reporting/
│ ├─ strategy/
│ └─ utils/
├─ logs/                                    # App/runtime logs
├─ sql/
│ └─ migrations/                            # SQLite schema migrations
├─ src/                                     # Implementation (.cpp)
│ ├─ cli/
│ ├─ core/
│ ├─ domain/
│ ├─ ingest/
│ ├─ io/
│ ├─ persistence/
│ ├─ reporting/
│ ├─ strategy/
│ └─ utils/
├─ tests/                                   # Unit / integration / E2E tests
│ ├─ e2e/
│ ├─ fixtures/
│ │ └─ e2e/
│ └─ unit/
│   └─ test_.cpp
├─ tools/                                   # Profiling tools / scripts + utilities
│ └─ profiling/
├─ vcpkg_triplets/                          # Custom vcpkg triplets
├─ CMakeLists.txt
└─ LICENSE.txt
```

---

# Building the Project

## Prerequisites

To build v0.9.0, install the following depending on your platform.

### Linux (recommended)

```
sudo apt install -y \
  cmake ninja-build g++ pkg-config \
  libcurl4-openssl-dev libsqlite3-dev libspdlog-dev libfmt-dev \
  ccache graphviz doxygen
```
Optionally Clang for TSAN:
```
sudo apt install clang
```
### Windows (MSVC + vcpkg)
## Required
- Install Visual Studio Build Tools or full Visual Studio (C++ Desktop).

- Install Ninja

- Clone vcpkg:
```
git clone https://github.com/microsoft/vcpkg
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
```
- Install dependencies:
```
vcpkg install fmt spdlog sqlite3 curl
```


---

## Linux (GCC/Clang)

```
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```
---

## Windows (MSVC + Ninja + vcpkg)

### Build
- Enable toolchain when configuring and build:

```
cmake -S . -B build -G "Ninja" ^
  -DCMAKE_TOOLCHAIN_FILE=%CD%/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-windows-static ^
  -DCMAKE_CXX_COMPILER_LAUNCHER=sccache

cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

---

## ThreadSanitizer (Linux only)

```
sudo apt install clang
cmake -S . -B build-tsan -DENABLE_TSAN=ON -DCMAKE_CXX_COMPILER=clang++
cmake --build build-tsan
ctest --test-dir build-tsan --output-on-failure
```
## Profiling (NEW in v0.9.0)
### Setup
```
./tools/profiling/perf_setup.sh
```
### Build profiling preset
```
cmake --preset linux-profiling
cmake --build --preset build-linux-profiling
```
### FlameGraph
```
./tools/profiling/perf_flamegraph.sh ./build/Profiling/bin/qga_cli config/perf_config.json
```
Output → ``` profiling_output/flamegraph.svg ```

### Hotspot report

```
./tools/profiling/perf_hotspot.sh ./build/Profiling/bin/qga_cli config/perf_config.json
```

---

# Running Tests

## Linux
./build/bin/qga_tests
./build/bin/qga_tests_e2e

## Windows (MSVC + Ninja)
# Ninja multi-config produces binaries under /Debug or /Release
./build/Debug/qga_tests.exe
./build/Debug/qga_tests_e2e.exe

### Run all tests:
```
ctest --test-dir build --output-on-failure
```

---

# Documentation (Doxygen)

```
cmake -S . -B build -DBUILD_DOCS=ON
cmake --build build --target docs
```
Output: `docs/html/index.html`

Install Graphviz for UML diagrams:
```
sudo apt install graphviz
```

---

> **Note:**
> Important:
> Milestone 0.9.0 does NOT fix CI — that is intentionally postponed to 1.0.5
> (Advanced Test Infrastructure milestone).

| Platform    | Build Status        | Cache              | Tests | Notes |
|-------------|---------------------|---------------------|-------|-------|
| **Ubuntu**  | ⚠️ Builds, ❌ Tests | ccache             | ❌    | Config schema changed; CLI requires `--config`; Statistics tests outdated; HTTP ingest missing mock server; async logger warning during teardown |
| **Windows** | ⚠️ Builds, ❌ Tests | sccache + vcpkg    | ❌    | CTest cannot find binaries (Ninja + MSVC outputs to `/Debug`); multi-config mismatch; fix postponed to `1.0.5` |
| **Docs**    | ✅                  | —                  | —     | Doxygen builds successfully |

---

## Known Issues (High Level)

### Profiling dataset is too small (demo-only)

v0.9.0 ships with a tiny ingest dataset (demo.csv) for stability and CI reproducibility.
Real performance profiling will become available in v2.0.0, once large datasets and optimized ingestion paths are added.

### Ubuntu (Linux)
Ubuntu CI fails due to test-suite mismatches caused by legitimate refactors done in 0.9.0:

- Old tests incompatible with new Config schema
- CLI now **requires `--config`**, so doctest auto-discovery fails
- Profile-based configs (`dev/test/prod`) changed — tests expect old values
- HTTP ingest tests require a real HTTP server (not available on CI runners)
- Statistics tests expect old validation logic (e.g. mean/median error cases)
- Async logger prints warnings when destroyed before thread pool flush

These failures are **not regressions in application logic** —
they reflect that tests must be updated to match the new architecture.

---

### Windows (MSVC + Ninja + vcpkg)
Windows CI fails because **CTest cannot find test executables**, e.g.:
- Ninja + MSVC output layout mismatch:
```
build/Debug/qga_tests.exe
build/Release/qga_tests.exe
```
but CTest expects:
```
build/bin/qga_tests.exe
```
- CTest paths will be refactored in 1.0.5
- Multi-config generators complicate test discovery

---

# Roadmap
### v0.9.0
- CPU profiling + hotspots
- Statistics expansion
- Tests stabilization (Linux only)
- Cleanup, formatting, clang-tidy baseline

### v1.0.0 (MVP Stable)

- REST API v1 (crow/cpp-httplib)
- Linux/Windows release bundles
- Documentation overhaul
- Coverage ≥70% (core modules)
- NO CI fix here

### v1.0.5 (Advanced Test Infrastructure)

- This is where CI finally becomes stable and green.
- Includes:
  - Test layout refactor (unit/module/system/perf)
  - MockLogger for deterministic outputs
  - Integration test suite
  - Performance test scaffold
  - Coverage reporting into CI
  - Windows test path unification
  - Fully rewritten CTest config

---

## Contributors
Project created and maintained by https://github.com/diwiw.

---

# License

QuantGradesApp is licensed under the **Business Source License (BSL) 1.1**.

- ❌ Commercial use is NOT permitted without a separate license
- ❌ SaaS / hosted offerings are NOT permitted
- ✔️ Personal, educational, and research use is allowed

See [LICENSE](./LICENSE) and [LEGAL_NOTICE.md](./LEGAL_NOTICE.md) for details.
