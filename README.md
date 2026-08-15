# KlyroDB

## Documentation
For comprehensive guides, tutorials, and full API references, please check our official documentation.

## Overview
KlyroDB is an embedded, local relational database written in modern C++20. It is engineered to be a highly-concurrent alternative to SQLite. By implementing Multi-Version Concurrency Control (MVCC) and fine-grained page locking, KlyroDB allows multiple readers and multiple writers simultaneously without deadlocks or database-level locks. It provides a rich set of data types including JSON, ARRAY, and DICT, similar in philosophy to PostgreSQL but completely local and network-independent.

## Why KlyroDB?
Traditional embedded databases like SQLite use file-level or database-level locks, which can block readers when a writer is active. KlyroDB solves this concurrency bottleneck. It is designed for massive local concurrency, making it ideal for high-throughput Python asyncio applications, background ingestion scripts, and local microservices that need to persist state concurrently.

## Core Features
- **Embedded & Serverless**: No database server, no network connection required. Data is contained safely in a local `.klyro` binary file.
- **Concurrent R/W**: Multiple readers and multiple writers simultaneously.
- **ACID Compliant**: Transactions with Write-Ahead Logging (WAL) and crash recovery.
- **Rich Types**: Strict PostgreSQL-inspired data types (UUID, JSON, ARRAY, DICT, etc.).
- **Python Native**: A unified Sync/Async Python API that maps seamlessly to C++.

## Architecture
The system is divided into several layers:
- **Parser & Planner**: A SQLite-like SQL dialect parsed and optimized via a cost-based optimizer.
- **Execution Engine**: Vectorized/row-based executor.
- **Storage Engine**: B+ Tree based storage using slotted pages.
- **Transaction Manager**: Manages MVCC snapshots, read/write sets, and logical locking.
- **Native Bridges**: Zero-copy Python bindings via PyBind11.

## Concurrency Model
KlyroDB uses thread-safe native data structures. The C++ core handles all locking and isolation.
- **Database thread safety**: The `Database` instance can be shared across all threads.
- **Connection thread safety**: `Connection` objects are bound to single threads/tasks. Do not share a connection across threads concurrently.
- **Async safety**: The `asyncio` SDK dispatches blocking I/O to a background thread pool, meaning the Python event loop is never blocked.

## Storage Engine
Data is stored in optimized Slotted Pages on disk for extremely fast `O(log N)` lookups. The storage engine bypasses the OS page cache where appropriate to manage its own buffer pool.

## Transactions
KlyroDB provides full ACID transactions with `BEGIN`, `COMMIT`, and `ROLLBACK` semantics. Nested transactions (savepoints) are supported. If a transaction fails, it automatically rolls back. 

## MVCC
Multi-Version Concurrency Control ensures that readers do not block writers, and writers do not block readers. When a row is updated, a new version is created in a version chain. Readers read from a snapshot isolated to the moment their transaction started.

## WAL and Recovery
Write-Ahead Logging (WAL) ensures your database survives process crashes and power loss. During initialization, if the database did not shut down cleanly, the WAL is automatically replayed to restore consistency before the first connection is accepted.

## Locking
KlyroDB employs fine-grained logical locking. Deadlocks are structurally avoided where possible, but if an unresolvable deadlock occurs, a `TransactionError` is raised. 

## SQL Compatibility
KlyroDB supports a SQLite-like dialect with standard relational features.
**Supported SQL**:
- `CREATE DATABASE` / file opening
- `CREATE TABLE`, `ALTER TABLE`, `DROP TABLE`
- `INSERT`, `SELECT`, `UPDATE`, `DELETE`
- `WHERE`, `ORDER BY`, `GROUP BY`, `HAVING`, `LIMIT`, `OFFSET`
- `JOIN`
- Aggregate functions, Indexes, PRAGMA
- Transactions and savepoints.

## Data Types

| Type | Description | Python |
|------|-------------|--------|
| NULL | SQL null | None |
| BOOLEAN | Boolean | bool |
| SMALLINT | 16-bit integer | int |
| INTEGER | 32-bit integer | int |
| BIGINT | 64-bit integer | int |
| REAL | 32-bit float | float |
| DOUBLE | 64-bit float | float |
| NUMERIC | High-precision numeric | decimal.Decimal |
| TEXT | UTF-8 text | str |
| BLOB | Binary data | bytes |
| DATE | Date | datetime.date |
| TIME | Time | datetime.time |
| TIMESTAMP | Timestamp | datetime.datetime |
| UUID | UUID | uuid.UUID |
| ARRAY | Ordered collection | list |
| DICT | Key/value mapping | dict |
| JSON | JSON document | dict/list/scalar |

*(Note: KlyroDB uses JSON. KlyroDB does not expose JSONB.)*

## ARRAY
KlyroDB natively supports arrays (`ARRAY`).
- **Syntax**: `[1, 2, 3]`
- **Construction**: Arrays map directly to Python `list`.
- **NULL behavior**: Array operations on NULL return NULL.

## DICT
KlyroDB natively supports string-keyed dictionaries (`DICT`).
- **Construction**: Maps directly to Python `dict`.
- **Duplicate-key behavior**: Overwrites existing keys on insert.
- **NULL behavior**: Dict operations on NULL return NULL.

## JSON
KlyroDB uses the `JSON` type for schema-less data.
- **Parsing**: Automatically parsed when inserting a Python `dict`/`list`/scalar if the column is typed as `JSON`.
- **Serialization**: Fetched as Python native structures.
- **NULL behavior**: JSON SQL operations on NULL return NULL.

## SQL Functions
| Function | Arguments | Return | Description |
|----------|-----------|--------|-------------|
| `count` | `(column)` | `INTEGER` | Counts non-null rows. |
| `sum` | `(column)` | `NUMERIC` | Sums numeric values. |
| `avg` | `(column)` | `REAL` | Averages numeric values. |

## Collection Operations
| Function | Arguments | Return | Description |
|----------|-----------|--------|-------------|
| `array_length` | `(ARRAY)` | `INTEGER` | Returns array length. |
| `array_append` | `(ARRAY, val)` | `ARRAY` | Appends value to array. |
| `array_prepend` | `(ARRAY, val)` | `ARRAY` | Prepends value. |
| `array_pop` | `(ARRAY)` | `ARRAY` | Removes last element. |
| `array_remove` | `(ARRAY, val)` | `ARRAY` | Removes occurrences of value. |
| `array_contains` | `(ARRAY, val)` | `BOOLEAN` | Checks if array contains value. |
| `array_position` | `(ARRAY, val)` | `INTEGER` | Finds index of value. |
| `array_slice` | `(ARRAY, start, end)` | `ARRAY` | Slices array. |
| `array_concat` | `(ARRAY, ARRAY)` | `ARRAY` | Concatenates two arrays. |
| `dict_get` | `(DICT, key)` | `ANY` | Gets value by key. |
| `dict_set` | `(DICT, key, val)` | `DICT` | Sets key to value. |
| `dict_insert`| `(DICT, key, val)` | `DICT` | Inserts if key doesn't exist. |
| `dict_remove`| `(DICT, key)` | `DICT` | Removes key. |
| `dict_contains`| `(DICT, key)` | `BOOLEAN` | Checks if key exists. |
| `dict_keys` | `(DICT)` | `ARRAY` | Returns all keys. |
| `dict_values`| `(DICT)` | `ARRAY` | Returns all values. |
| `dict_items` | `(DICT)` | `ARRAY` | Returns tuples of items. |
| `dict_size` | `(DICT)` | `INTEGER` | Returns number of keys. |
| `json_get` | `(JSON, path)` | `ANY` | Extracts value at path. |
| `json_set` | `(JSON, path, val)` | `JSON` | Sets value at path. |
| `json_remove`| `(JSON, path)` | `JSON` | Removes value at path. |
| `json_type` | `(JSON, path)` | `TEXT` | Returns type of value at path. |
| `json_array_length`| `(JSON, path)` | `INTEGER` | Returns length of JSON array. |
| `json_keys` | `(JSON, path)` | `ARRAY` | Returns keys of JSON object. |

## C++ API
KlyroDB offers a rich native C++ API using RAII for memory and transaction safety. The Python SDK delegates to this C++ engine via PyBind11.

## Python SDK
The Python SDK exposes a unified interface containing both synchronous and asynchronous APIs: `import klyrodb`.

Installed via `pip install klyrodb`.
This single package provides both synchronous database interactions (perfect for scripts and standard data ingestion tasks) and `asyncio` compatible operations using native worker threads.

## Logging
KlyroDB integrates fully with Python's standard `logging` module. 
By default, the library is entirely quiet. No logs are emitted, and no basic configuration is forced. 

### Diagnostic Loggers
- `klyrodb`
- `klyrodb.database`
- `klyrodb.connection`
- `klyrodb.transaction`
- `klyrodb.pool`
- `klyrodb.async`
- `klyrodb.performance`

**Parameter Privacy**: SQL parameters are NEVER logged by default to protect PII, passwords, and tokens.
**Query Logging**: You can enable query execution timings and slow query detection.

```python
import logging
import klyrodb

# Configure Python logging however you prefer
logging.basicConfig(level=logging.INFO)

# Enable KlyroDB diagnostics
klyrodb.configure_logging(
    level=logging.DEBUG,
    query_logging=True,
    slow_query_threshold_ms=50.0
)
```

## Diagnostics
Connection IDs and Transaction IDs are generated automatically and attached to logs as structured fields. Use a JSON formatter or similar standard Python logging utility to extract `duration_ms`, `conn_id`, and `operation`.

## Connection Pooling
Instead of opening a new connection for every request, use `klyrodb` connection pools for safe concurrency throttling.

## Error Handling
KlyroDB maps native exceptions to standard Python exceptions:
| Exception | Meaning | Typical Cause |
|-----------|---------|---------------|
| `KlyroError` | Base error | - |
| `DatabaseError` | Storage/Engine error | Corruption, IO failure |
| `OperationalError` | Runtime error | Out of memory |
| `IntegrityError` | Constraint violation | Duplicate primary key |
| `ProgrammingError` | Syntax/Logic error | Invalid SQL |
| `TransactionError` | Transaction failed | Deadlock |
| `ConcurrencyError` | Concurrency violation | Serialization failure |
| `TimeoutError` | Query timeout | Query exceeded threshold |

## Examples

**Sync Quickstart**
```python
import klyrodb

with klyrodb.open("example.klyro") as db:
    with db.connect_sync() as conn:
        conn.execute("CREATE TABLE users (id INTEGER, name TEXT)")
        conn.execute("INSERT INTO users VALUES (?, ?)", (1, "Alice"))
        
        for row in conn.execute("SELECT * FROM users"):
            print(row)
```

**Async Quickstart**
```python
import asyncio
import klyrodb

async def main():
    async with await klyrodb.open("example.klyro") as db:
        async with db.connect_async() as conn:
            await conn.execute("CREATE TABLE users (id INTEGER, name TEXT)")
            await conn.execute("INSERT INTO users VALUES (?, ?)", (1, "Alice"))
            
            result = await conn.execute("SELECT * FROM users")
            async for row in result:
                print(row)

asyncio.run(main())
```

## Performance
*Benchmark results: NOT YET MEASURED*
The architecture is designed to rival in-memory databases while providing safe disk persistence.

## DSA/DAA
- **B+ Tree**: Used for primary storage and index lookups (O(log N)).
- **MVCC Version Chains**: Maintains historical row states for concurrent snapshot isolation.
- **Worker Pool**: Maintains active thread workers to prevent thread-creation overhead.
- **Connection Pool**: Reuses allocated connection resources and native memory arenas.

## File Format
The `.klyro` file encapsulates pages, metadata, and collection serialization in a unified portable format.

## Limitations
- KlyroDB is strictly an embedded database. It cannot act as a distributed cluster database out of the box.
- JSON type is stored dynamically; large JSON payloads may incur parsing overhead.

## Testing
To run tests locally:
```bash
# Run python SDK tests
python -m unittest discover tests/python
```

## Build
Uses CMake for the C++ engine.
```bash
mkdir build && cd build
cmake ..
make
```

## Installation
```bash
pip install klyrodb
```

## License
MIT License. Created by Pravanjan Roy.

---

## Complete Function Index

### Database
- `open(path: str) -> Database`
- `Database.connect_sync() -> SyncConnection`
- `Database.create_pool_sync(max_connections: int = 16) -> SyncConnectionPool`
- `Database.connect_async() -> AsyncConnection`
- `Database.create_pool_async(max_connections: int = 16) -> AsyncConnectionPool`
- `Database.close()`

### Connection (Sync)
- `SyncConnection.execute(sql: str, parameters: Optional[Sequence[object]] = None, timeout: float = 0.0) -> SyncResult`
- `SyncConnection.executemany(sql: str, parameters_list: Sequence[Sequence[object]])`
- `SyncConnection.prepare(sql: str) -> SyncPreparedStatement`
- `SyncConnection.transaction() -> SyncTransaction`
- `SyncConnection.close()`

### Result (Sync)
- `SyncResult.fetchone() -> Optional[tuple]`
- `SyncResult.fetchmany(size: int) -> list[tuple]`
- `SyncResult.fetchall() -> list[tuple]`

### PreparedStatement (Sync)
- `SyncPreparedStatement.execute(*parameters: object) -> SyncResult`

### ConnectionPool (Sync)
- `SyncConnectionPool.acquire() -> SyncConnection`
- `SyncConnectionPool.close()`

### Connection (Async)
- `AsyncConnection.execute(sql: str, parameters: Optional[Sequence[object]] = None, timeout: float = 0.0) -> AsyncResult`
- `AsyncConnection.executemany(sql: str, parameters_list: Sequence[Sequence[object]])`
- `AsyncConnection.prepare(sql: str) -> AsyncPreparedStatement`
- `AsyncConnection.transaction() -> AsyncTransaction`
- `AsyncConnection.close()`

### Result (Async)
- `AsyncResult.fetchone() -> Optional[tuple]`
- `AsyncResult.fetchmany(size: int) -> list[tuple]`
- `AsyncResult.fetchall() -> list[tuple]`

### PreparedStatement (Async)
- `AsyncPreparedStatement.execute(*parameters: object) -> AsyncResult`

### ConnectionPool (Async)
- `AsyncConnectionPool.acquire() -> AsyncConnection`
- `AsyncConnectionPool.close()`

### Logging
- `configure_logging(level=None, query_logging=None, slow_query_threshold_ms=None, log_parameters=False, redaction=None, native_logging=False)`
- `enable_debug()`
- `LoggingConfig` class
