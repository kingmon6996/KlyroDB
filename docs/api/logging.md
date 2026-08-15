# Logging API Reference

KlyroDB integrates fully with Python's standard `logging` library. By default, the library is entirely silent.

## Configuration

To enable logging, you must configure the Python `logging` module and explicitly instruct `klyrodb` to emit logs using `klyrodb.configure_logging`.

### `klyrodb.configure_logging(level=None, query_logging=None, slow_query_threshold_ms=None, log_parameters=False, redaction=None, native_logging=False)`

- `level`: The logging level (e.g. `logging.DEBUG`, `logging.INFO`).
- `query_logging`: If `True`, logs every query execution at `DEBUG` level.
- `slow_query_threshold_ms`: Emits a `WARNING` if a query exceeds this duration.
- `log_parameters`: If `True`, parameters are appended to logs.
- `redaction`: A custom callable `(value: Any) -> str` to redact parameters. By default, KlyroDB redacts all strings, blobs, and restricts the output of large collections.
- `native_logging`: Enable native C++ core logging (if supported).

### `klyrodb.enable_debug()`
A shorthand for setting `level=logging.DEBUG` and `query_logging=True`.

### `LoggingConfig`
A dataclass holding the active configuration, which you can retrieve via internal module state if necessary.

## Environment Variables
The logging configuration will also fallback to the following environment variables if not provided programmatically:
- `KLYRODB_LOG_LEVEL` (e.g., `INFO`, `DEBUG`)
- `KLYRODB_QUERY_LOGGING` (e.g., `true`, `1`)
- `KLYRODB_SLOW_QUERY_MS` (e.g., `100.0`)

## Loggers
The following child loggers are available for fine-grained filtering:
- `klyrodb`
- `klyrodb.database`
- `klyrodb.connection`
- `klyrodb.transaction`
- `klyrodb.pool`
- `klyrodb.async`
- `klyrodb.performance`

## Security
SQL parameters are **never** logged by default. Even when `log_parameters=True` is provided, KlyroDB attempts to redact string contents and binary blobs automatically to prevent exposing passwords, API keys, or raw data payloads in system logs.
