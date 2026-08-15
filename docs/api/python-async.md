# Python Async API Reference

The asynchronous Python API integrates flawlessly with Python's `asyncio` event loop. All blocking database I/O is dispatched to a native background thread pool. Distributed via `klyrodb-async`.

## `klyrodb.open(path: str) -> Database`
*(Note: Use `async with await klyrodb.open(...) as db:`)*

## `Database`
- `connect_async() -> AsyncConnection`
- `create_pool_async(max_connections: int = 16) -> AsyncConnectionPool`
- `close()`

## `AsyncConnection`
- `await execute(sql: str, parameters: Optional[Sequence[object]] = None, timeout: float = 0.0) -> AsyncResult`
- `await executemany(sql: str, parameters_list: Sequence[Sequence[object]])`
- `await prepare(sql: str) -> AsyncPreparedStatement`
- `transaction() -> AsyncTransaction`: Returns an async context manager for RAII transaction handling.
- `await close()`

## `AsyncResult`
An asynchronous iterator over rows. Support `async for row in result:`.
- `await fetchone() -> Optional[tuple]`
- `await fetchmany(size: int) -> list[tuple]`
- `await fetchall() -> list[tuple]`

## `AsyncPreparedStatement`
- `await execute(*parameters: object) -> AsyncResult`

## `AsyncTransaction`
Managed via `async with` blocks. Commits on successful exit, rolls back on exception.

## `AsyncConnectionPool`
- `await acquire() -> AsyncConnection`: Returns a managed connection that is returned to the pool on close.
- `await close()`: Drains the pool.
