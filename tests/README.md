# QuantGradesApp — Test Architecture

## Test Types

- `unit/` — Fast, isolated unit tests (GTest).
- `integration/` — Module-level tests (e.g., HTTP, persistence).
- `system/` — CLI / end-to-end system flows.
- `e2e/` — Full pipeline tests (disabled for now).
- `perf/` — Performance benchmarks.

---

## Logger Test Doubles

Two different test doubles exist for `ILogger`:

### 1. MockLoggerCapture (state-based)
Location: `tests/fixtures/MockLoggerCapture.hpp`

Use when:
- You want to verify stored log messages.
- You assert on content, order, filtering.
- You need access to entries(), count(), contains(), last().

Preferred for most unit tests.

---

### 2. MockLoggerGMock (interaction-based)
Location: `tests/mocks/MockLoggerGMock.hpp`

Use when:
- You need `EXPECT_CALL`.
- You want to verify call counts.
- You want strict argument matching.

Use this for behavior verification tests.

---

## Running Unit Tests

```bash
cmake --preset linux-debug
cmake --build --preset build-linux-debug --target qga_tests_gtest
ctest --preset test-unit-linux
