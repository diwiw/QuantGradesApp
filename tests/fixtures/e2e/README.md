
Each subdirectory groups fixtures by the area of the system they are used to test.
# Test Fixtures – QuantGradesApp

This directory contains **small, deterministic test fixtures** used across unit, integration, and end-to-end (E2E) tests.

Fixtures are intentionally minimal, human-readable datasets designed to reliably reproduce specific test scenarios.
They are not meant to represent real-world market datasets (those will live in `tests/data/` starting from milestone 1.1.5).

---

## Structure

tests/
├── data/
│   └── test_http.csv            <-- temporarily
│
├── e2e/
│   ├── CMakeLists.txt
│   ├── test_main_e2e.cpp
│   └── test_cli_e2e.cpp        <-- milestone 0.8.0 core
│
├── fixtures/
│   └── e2e/
│       ├── config/             <-- future
│       ├── domain/
│       ├── README.md
│       └── sample_short.csv
│
├── CMakeLists.txt
└── test_*.cpp

- **`e2e/`** – minimal CSV inputs and configs used by CLI end-to-end tests
- **`domain/`** – deterministic datasets for domain logic tests (quotes, bars, mappers)
- **`config/`** – small JSON/INI config files used during configuration tests

Additional sections may be added as the project grows.

---

## General Rules

1. **Fixtures must be deterministic.**
   No randomness, timestamps, or values that change over time.

2. **Fixtures must remain small.**
   Typically under **5–10 KB**. Large datasets belong in `tests/data/`.

3. **Fixtures must be easy to understand.**
   A developer should be able to open them and immediately see what they represent.

4. **Fixtures are never modified at runtime.**
   Tests that need to write data should do so in a temporary directory (`build/e2e_tmp` etc.).

5. **Fixtures should represent edge cases whenever needed.**
   Examples:
   - malformed rows,
   - missing fields,
   - duplicated timestamps,
   - out-of-order entries.

---

## Existing Fixtures

### `e2e/sample_short.csv`
A short 10-row OHLCV dataset used for CLI end-to-end tests.
Rows are strictly ordered, consistent, and kept intentionally small to run fast on CI.

Contents include:
- timestamp
- open / high / low / close
- volume

Used in milestone **0.8.0 E2E tests**.

---

## Future Additions (Milestone ≥ 1.1.5)

Starting from milestone 1.1.5 the fixture directory will be extended to include:

- edge cases for DataIngest (malformed CSV, missing columns)
- fixture configs for strategy tests
- micro datasets used for backtesting logic
- fixtures supporting SQLite persistence tests

Larger or realistic datasets (e.g., 5k–50k OHLC rows) will be added separately under `tests/data/`.

---

## Contribution Guidelines

If you add a new fixture:

1. Keep it small.
2. Keep it deterministic.
3. Add a short comment at the top of the file if needed.
4. Document it in this README.

Fixtures that violate these rules will be moved or removed during the next cleanup milestone.

---
