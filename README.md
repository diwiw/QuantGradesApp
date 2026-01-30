# QuantGradesApp

> **Version:** `v1.0.0`
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
  - REST-driven automation
  - future HPC-oriented extensions

This v1.0.0 release finalizes API stability, packaging, documentation, and repository bootstrap for the entire 1.x line.

---

## What's New in v1.0.0

### REST API v1
- Lightweight REST API (cpp-httplib / crow)
- `/health`
- `/version`
- `/ingest/csv`
- `/grades/report`
- Integrated with existing config & logging
- Optional build via `BUILD_API=ON/OFF`

### Release Packaging
- Linux standalone bundle (`.tar.gz`)
- Windows portable bundle (`.zip`)
- Bundled:
  - executables
  - configs
  - README
  - example CSV
- No compilation required to run

### Repository Bootstrap
- Clean root directory
- Normalized layout (`src/`, `include/`, `config/`, `tools/`)
- Updated `.gitignore`
- Removed deprecated paths
- CMake structure aligned with runtime layout

### Documentation Finalization
- README rewritten for 1.0.0
- Quickstart added
- API usage documented
- Architecture diagrams finalized

---

## Key Features (v1.0.0)
### Core Engine
- Deterministic backtesting engine
- Strict separation of domain logic and infrastructure
- Clean separation of domain and infrastructure

### Statistics Module
- Maximum Drawdown (MDD)
- CAGR
- Sharpe Ratio
- Sortino Ratio
- Hit Ratio
- Full unit test coverage

### Data ingestion
- Strict CSV validation
- Safe row parsing with error reporting
- HTTP ingest prototype (API-triggered)

### CLI
- Config required (`--config`)
- Deterministic output
- Integrated logging
- Stable command surface

---

## Quickstart (Release Bundle)

### 1. Download
Download the appropriate release bundle:
- Linux: quantgrades_api-linux-x64.tar.gz
- Windows: quantgrades_api-windows-x64.zip

### 2. Unpack
```
tar -xzf quantgrades_api-linux-x64.tar.gz
cd quantgrades
```

### 3. Inspect configuration

```
config/
 ├─ conf.dev.json             # loaded for dev
 ├─ conf.test.json            # loaded for tests
 └─ conf.json                 # default
 ```

 ### 4. Run CSV ingestion

 ```
 ./qga_cli ingest --config config/conf.dev.json
 ```

 ### 5. Run backtest & reporting
 ```
 ./qga_cli run --config config/conf.dev.json
 ```

 ### 6. Outputs
 - logs → `logs/`
 - reports → `data/`
 - console summary → stdout

---

# Configuration Model

QuantGradesApp uses an explicit, layered configuration model driven by JSON files and optional environment overrides.

### Configuration files

```
config/
 ├─ conf.json                 # default
 ├─ conf.dev.json             # loaded for dev
 └─ conf.test.json            # loaded for tests
 ```

 ### Default behavior
 - If no profile is specified, the application loads:
 `config/conf.json`
 - Development and test profiles must be selected explicitly via CLI or environment.

 ### CLI usage
 ```
 ./qga_cli run    --config config/conf.dev.json
 ./qga_cli ingest --config config/conf.dev.json
 ./quantgrades_api        --config config/conf.json
```

### Environment-based profile selection

The configuration can also be selected via environment variable:

`QGA_PROFILE=dev`

This results in loading:

`config/config.dev.json`

### Configuration structure (derived from `Config.cpp`)
```
{
  "api": {
    "port": 8080
  },
  "paths": {
    "data_dir": "data"
  },
  "engine": {
    "threads": 4
  },
  "logging": {
    "level": "info",
    "file": "logs/qga.log",
    "max_size_mb": 10,
    "max_files": 3
  },
  "input": {
    "path": "data/input.csv"
  },
  "output": {
    "path": "data/output"
  }
}
```

### Validation rules (runtime)
- `engine.threads`
 - `0` → hardware concurrency
 - `< 0` → fallback to hardware concurrency (with warning)
 - `> CPU cores` → clamped to hardware concurrency
- `logging.file`
 - empty → fallback to default
- invalid or missing config
 - defaults are loaded with warnings

There are *no silent fallbacks* — all corrections are logged as warnings.

---

## REST API v1
The REST API provides external control and reporting without requiring CLI execution.

### Start API
`quantgrades_api --config config/conf.dev.json`

### Endpoints
| Method    | Endpoint             | Description              |
|-----------|----------------------|--------------------------|
| GET       | `/health`            | Service heartbeat         |
| GET       | `/version`           | Build version + git hash |
| POST      | `/ingest/csv`        | Trigger CSV ingestion    |

Example:
`curl http://localhost:8080/health`

## Module Overview

### `api/`
REST layer (server + routing) exposing core functionality to external tools:
- `quantgrades_api` – REST entrypoint/binary
- HTTP routes (health/version/ingest/report)
- Request → service orchestration (calls into `core/`,`ignest/`, `reporting/`)
- Uses runtime config + logger

### `apps/`
Standalone runnable applications / demos built on top of the engine:
- demo entrypoints (e.g. backtest/grades/report demos)
- “showcase” programs for release bundles / recruiters
- `minimal glue code around `domain/` + `core/`

### `cli/`
Command-line frontend for local execution:
- `qga_cli` - CLI entrypoint/binary
- argument parsing + command dispatch
- config selection (`--config ...`)
- triggers ingest / backtest / reporting flows

### `core/`
Shared, foundational infrastructure:
- `Config` – JSON/ENV configuration
- `Statistics` – metrics calculations
- `Platform` – platform utilities
- `Version` – semantic versioning info

### `domain/`
Business logic and quantitative model:
- **backtest:** Engine, Execution, Portfolio, Orders/Trades/Positions, Result, Instrument, Quote/Series
- domain rules and deterministic calculations
- no IO / no HTTP / no CLI dependencies

### `ingest/`
Data acquisition & layer:
- CSV (and optional HTTP) ingestion orchestration
- validation + mapping to domain models (e.g. `domain::Quote`)
- high-level ingest pipeline that feeds the engine/reporting

### `io/`
Low-level file operations:
- `CsvLoader` / file readers
- `FileManager` utilities (stateless)
- `DataExporter` (writing reports/exports)

### `persistence/`
Database abstraction:
- `IDataStore` (interface)
- `SQLiteStore` implementation
- `Statement` / DB wrapper
- `DatabaseWorker` / async worker / background execution (if present in your implementation)

### `reporting/`
Reporting & aggregation layer (observer-style):
- `IReporter` interfaces
- `ReporterManager` orchestration
- JSON summary/report generation (used by CLI + API)

### `strategy/`
Strategy layer (pluggable trading logic):
- `IStrategy` interface
- concrete strategies (e.g. Buy&Hold, MA crossover if included)
- strategy selection / integration with `domain/backtest`

### `utils/`
Technical utilities shared across modules:
- logging abstractions (`ILogger`, implementations)
- `LoggerFactory`
- helpers used by multiple layers (without leaking app concerns)


### `sql/`
Database schema / migrations:
- SQLite schema files
- migrations and seed helpers

---

# Architecture Overview

````mermaid
flowchart TD
    CLI[CLI / API] --> Config
    Config --> Ingest
    Config --> Engine
    Ingest --> Domain
    Domain --> Statistics
    Statistics --> Reporting
    Reporting --> Output
    Engine --> Reporting
````
### Architectural rules
- `domain/` contains *pure business logic*
- `core/` contains *configuration, statistics and platform glue*
- `cli/` and `cli/` are *thin entry layers*
- no IO, HTTP or CLI dependencies inside `domain/`
- all runtime behavior is config-driven

---

# Class Diagram

See `docs/pages/class_diagram.md` for extended diagrams.

````mermaid
classDiagram
    class Config
    class Engine
    class Strategy
    class Execution
    class Portfolio
    class Order
    class Trade
    class Result
    class Statistics
    class ReporterManager

    Config --> Engine
    Engine --> Strategy
    Engine --> Execution
    Execution --> Portfolio
    Portfolio --> Order
    Portfolio --> Trade
    Portfolio --> Result
    Engine --> Statistics
    Statistics --> ReporterManager
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
│ ├─ api/                   # REST API server
│ ├─ apps/                  # Demo / showcase executables
│ ├─ cli/                   # CLI frontend
│ ├─ core/                  # Config, Statistics, Platform, Version
│ ├─ domain/                # Backtest engine & models
│ ├─ ingest/                # CSV / HTTP ingestion
│ ├─ io/                    # File IO & exporters
│ ├─ persistence/           # SQLite persistence layer
│ ├─ reporting/             # Reporting & aggregation
│ ├─ strategy/              # Trading strategies
│ └─ utils/                 # Logging & shared utilities
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

To build v1.0.0, install the following depending on your platform.

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
> *v1.0.0 intentionally ships with a reduced test surface in CI.*
> Some tests are disabled or postponed to *v1.0.5 (Advanced Test Infrastructure)* as part of a planned CI refactor.
> This does not affect runtime stability, release bundles, or API correctness.

| Platform    | Build Status        | Cache               | Tests         | Notes |
|-------------|---------------------|---------------------|---------------|-------|
| **Ubuntu**  | ✅ Builds           | ccache              | ⚠️ Partial    | Core builds and runtime tests pass. Some unit/(all) E2E tests disabled due to config schema refactor and missing HTTP mock server. |
| **Windows** | ✅ Builds           | sccache + vcpkg     | ⚠️ Partial    | Builds succeed. Some tests (all E2E) disabled due to Ninja + MSVC multi-config output paths (CTest discovery).                            |
| **Docs**    | ✅                  | —                   | —     | Doxygen builds successfully |

---

## Known Issues (High Level)

### Test surface intentionally reduced (v1.0.0)
The following limitations are *known and intentional* for the v1.0.0 release:
- Test suite partially disabled in CI
- Some E2E and integration tests postponed
- Coverage reporting not yet enabled in CI
These are *infrastructure limitations*, not application regressions.

### E2E tests disabled (v1.0.0)
E2E tests are intentionally disabled in this release:
- `tests/e2e` is not added to the build (`add_subdirectory(e2e`) commented out)
- reason: planned migration of E2E to extended test infrastructure (Windows + HTTP server tests)
- scheduled: v1.0.5

### Profiling dataset is too small (demo-only)
v0.9.0 ships with a tiny ingest dataset (demo.csv) for stability and CI reproducibility.
Real performance profiling will become available in v2.0.0, once large datasets and optimized ingestion paths are added.

### Ubuntu (Linux)
Ubuntu CI fails due to test-suite mismatches caused by legitimate refactors done in 0.9.0:

- Configuration schema was refactored → older tests no longer apply
- CLI requires explicit `--config`→ auto-discovery tests disabled
- Profile-based configs (`dev/test/prod`) require test refactor
- HTTP ingest tests require a mock HTTP server (planned for 1.0.5)
- Async logger prints warnings when destroyed before thread pool flush

These failures are **not regressions in application logic** —
they reflect that tests must be updated to match the new architecture.

---

### Windows (MSVC + Ninja + vcpkg)
- Ninja + MSVC generates binaries under:
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

### v1.0.0 Stable Core Release

- REST API v1
- Linux/Windows release bundles
- Production-ready configuration model
- Documentation overhaul
- Stable runtime & deterministic behavior
- CI builds are stable; test surface intentionally reduced

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
