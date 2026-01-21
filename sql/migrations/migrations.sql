-- ======================================================
-- QuantGradesApp Persistence Schema (v0.7.0 baseline)
-- ======================================================

PRAGMA foreign_keys = ON;

-- ======================
-- Quotes (tick-level or OHLC aggregated)
-- ======================
CREATE TABLE IF NOT EXISTS quotes (
    symbol      TEXT NOT NULL,
    ts          INTEGER NOT NULL,    -- Epoch ms
    open        REAL NOT NULL,
    high        REAL NOT NULL,
    low         REAL NOT NULL,
    close       REAL NOT NULL,
    volume      REAL NOT NULL,
    PRIMARY KEY (symbol, ts)
);

-- ======================
-- BarSeries (candlestick data, derived from quotes)
-- ======================
CREATE TABLE IF NOT EXISTS bar_series (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    symbol      TEXT NOT NULL,
    timeframe   TEXT NOT NULL,        -- e.g. "1m", "5m", "1h", "1d"
    ts          INTEGER NOT NULL,     -- Epoch ms start of bar
    open        REAL NOT NULL,
    high        REAL NOT NULL,
    low         REAL NOT NULL,
    close       REAL NOT NULL,
    volume      REAL NOT NULL,
    UNIQUE(symbol, timeframe, ts)
);

-- ======================
-- Portfolio (state of account at snapshot in time)
-- ======================
CREATE TABLE IF NOT EXISTS portfolios (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    name        TEXT NOT NULL,
    created_at  INTEGER NOT NULL DEFAULT (strftime('%s','now')*1000),
    initial_capital REAL NOT NULL,
    description TEXT
);

-- ======================
-- Trades (executed trades linked to portfolios)
-- ======================
CREATE TABLE IF NOT EXISTS trades (
    id            INTEGER PRIMARY KEY AUTOINCREMENT,
    portfolio_id  INTEGER NOT NULL,
    symbol        TEXT NOT NULL,
    ts            INTEGER NOT NULL,     -- Epoch ms
    side          TEXT NOT NULL CHECK (side IN ('BUY','SELL')),
    quantity      REAL NOT NULL,
    price         REAL NOT NULL,
    fee           REAL DEFAULT 0,
    FOREIGN KEY (portfolio_id) REFERENCES portfolios(id) ON DELETE CASCADE
);

-- ======================
-- Positions (current open positions per portfolio+symbol)
-- ======================
CREATE TABLE IF NOT EXISTS positions (
    id            INTEGER PRIMARY KEY AUTOINCREMENT,
    portfolio_id  INTEGER NOT NULL,
    symbol        TEXT NOT NULL,
    quantity      REAL NOT NULL,
    avg_price     REAL NOT NULL,
    updated_at    INTEGER NOT NULL DEFAULT (strftime('%s','now')*1000),
    FOREIGN KEY (portfolio_id) REFERENCES portfolios(id) ON DELETE CASCADE,
    UNIQUE(portfolio_id, symbol)
);

-- ======================
-- Indexes for faster queries
-- ======================
CREATE INDEX IF NOT EXISTS idx_quotes_symbol_ts
    ON quotes(symbol, ts);

CREATE INDEX IF NOT EXISTS idx_bar_series_symbol_tf_ts
    ON bar_series(symbol, timeframe, ts);

CREATE INDEX IF NOT EXISTS idx_trades_portfolio
    ON trades(portfolio_id, ts);

CREATE INDEX IF NOT EXISTS idx_positions_portfolio
    ON positions(portfolio_id, symbol);
