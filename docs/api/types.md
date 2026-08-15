# Supported Data Types

KlyroDB enforces strict, PostgreSQL-inspired data types at the storage layer.

| SQL Type | Python Type | Storage Size | Description |
| :--- | :--- | :--- | :--- |
| `SMALLINT` | `int` | 2 bytes | 16-bit signed integer. Capacity: -32768 to +32767. |
| `INTEGER` | `int` | 4 bytes | 32-bit signed integer. Capacity: -2 billion to +2 billion. |
| `BIGINT` | `int` | 8 bytes | 64-bit signed integer. Capacity: -9 quintillion to +9 quintillion. |
| `REAL` | `float` | 4 bytes | 32-bit IEEE 754 single-precision floating point number. |
| `DOUBLE` | `float` | 8 bytes | 64-bit IEEE 754 double-precision floating point number. |
| `NUMERIC(p,s)` / `DECIMAL` | `decimal.Decimal` | Variable | Exact numeric type for high-precision financial calculations. |
| `BOOLEAN` | `bool` | 1 byte | True/False boolean logic. Stored efficiently as a single byte. |
| `CHAR(n)` | `str` | Fixed `n` | Fixed-length string space-padded to `n` characters. |
| `VARCHAR(n)` | `str` | Variable | Variable-length UTF-8 text string. Enforces a maximum length of `n`. |
| `TEXT` | `str` | Variable | Unbounded variable-length UTF-8 text string. |
| `BYTEA` / `BLOB` | `bytes` | Variable | Raw binary data array. |
| `DATE` | `datetime.date` | 4 bytes | Calendar date (Year, Month, Day). |
| `TIME` | `datetime.time` | 8 bytes | Time of day without timezone, precise to microseconds. |
| `TIMESTAMP` | `datetime.datetime` | 8 bytes | Date and time without timezone, tracked as microseconds since Unix Epoch. |
| `TIMESTAMPTZ`| `datetime.datetime` | 8 bytes | Date and time **with** timezone awareness. |
| `INTERVAL` | `datetime.timedelta` | 16 bytes | Time duration (months, days, microseconds). |
| `UUID` | `uuid.UUID` | 16 bytes | Universally Unique Identifier, stored compactly in native binary. |
| `JSON` | `str`/`dict`/`list` | Variable | Text representation of JSON data. Parsed seamlessly using JSON path operations. *(KlyroDB strictly uses JSON and does not expose JSONB)*. |
| `DICT` | `dict` | Variable | First-class dictionary mapping string keys to arbitrary KlyroDB types. |
| `ARRAY` | `list` | Variable | A variable-length array of any other valid KlyroDB type (e.g., `INTEGER[]`). |
| `ENUM` | `enum.Enum` | 4 bytes | A custom enumerated type mapping string labels to highly efficient internal integer IDs. |
