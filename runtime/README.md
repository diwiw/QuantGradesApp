# QuantumGrades API (v1.0.0)

A high-performance C++ REST API designed for efficient grade processing and analytics. This release bundle is self-contained and ready to run.

## 🚀 Quick Start

### Windows
1. Unzip the package.
2. Open a terminal (CMD or PowerShell) in the extracted folder.
3. Ensure `config.json` is present in config catalog.
4. Run the application:
   ```powershell
   .\quantumgrades_api.exe
5. You should see: ```Starting API server on port 8080```.

### Linux
1. Extract the tarball.
2. Run in terminal:
```./quantumgrades_api```

## 🧪 How to Test (Windows Console)

You can interact with the API using PowerShell (recommended) or Curl without installing extra tools.

1. Check Health (GET)

Verifies if the service is up and running.

PowerShell:
```
irm http://localhost:8080/health
```

Curl:
```
curl -v http://localhost:8080/health
```

2. Check Version (GET)

Returns the current API version.

PowerShell:
```
irm http://localhost:8080/version
```
3. Trigger Ingest (POST)

Simulates a CSV data ingestion trigger (Currently a prototype stub).

PowerShell:
```
irm http://localhost:8080/ingest/csv -Method Post
```

Curl:
```
curl -X POST http://localhost:8080/ingest/csv
```

4. Get Report (GET)

Retrieves the calculated grades statistics.

PowerShell:
```
irm http://localhost:8080/grades/report
```

## 🧪 How to Test (Linux)

1. Check Health (GET)

Verifies if the service is up and running.

Curl:
```
curl -v http://localhost:8080/health
```

2. Check Version (GET)

Returns the current API version.

Curl:
```
curl http://localhost:8080/version
```
3. Trigger Ingest (POST)

Simulates a CSV data ingestion trigger (Currently a prototype stub).

Curl:
```
curl -X POST http://localhost:8080/ingest/csv
```

4. Get Report (GET)

Retrieves the calculated grades statistics.

Curl:
```
curl http://localhost:8080/grades/report
```

## ⚙️ Configuration

The application is controlled by the `config.json` file located in the root directory.

### Default Configuration
```json
{
    "api": {
        "port": 8080
    },
    "logging": {
        "level": "INFO",
        "file": "logs/quantgrades.log",
        "max_size_mb": 20,
        "max_files": 5
    },
    "paths": {
        "data_dir": "data",
        "output_dir": "output"
    },
    "engine": {
        "threads": 0
    }
}

### Settings Description

- api

  - port: The HTTP port the server listens on (Default: 8080).

- logging

  - level: Log verbosity. Options: DEBUG, INFO, WARN, ERROR.

  - file: Path to the log file. The directory will be created automatically.

  - max_size_mb: Maximum size of a single log file in Megabytes before rotation.

  - max_files: Number of old log files to keep (log rotation).

- paths

  - data_dir: Directory for input data and internal storage.

  - output_dir: Directory where generated reports and artifacts are saved.

- engine

  - threads: Number of worker threads for parallel processing.

   - Note: If set to 0 (default), the application automatically detects the number of available hardware threads supported by the CPU.

## 📂 Logs & Data

- logs/: Application logs are rotated automatically.

- data/: Directory for input CSV files and internal storage.
