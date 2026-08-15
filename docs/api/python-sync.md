# Python Sync API Reference

The synchronous Python API executes immediately on the calling thread. It is distributed via `klyrodb-sync`.

## `klyrodb.open(path: str) -> Database`
Opens or creates a database at the specified path.

## `Database`
- `connect_sync() -> SyncConnection`
- `create_pool_sync(max_connections: int = 16) -> SyncConnectionPool`
- `close()`

## `SyncConnection`
A thread-bound connection to the database. Do not share across threads concurrently.
- `execute(sql: str, parameters: Optional[Sequence[object]] = None, timeout: float = 0.0) -> SyncResult`
- `executemany(sql: str, parameters_list: Sequence[Sequence[object]])`
- `prepare(sql: str) -> SyncPreparedStatement`
- `transaction() -> SyncTransaction`: Returns a context manager for RAII transaction handling.
- `close()`

## `SyncResult`
An iterator over rows.
- `fetchone() -> Optional[tuple]`
- `fetchmany(size: int) -> list[tuple]`
- `fetchall() -> list[tuple]`

## `SyncPreparedStatement`
- `execute(*parameters: object) -> SyncResult`

## `SyncTransaction`
Managed via `with` blocks. Commits on successful exit, rolls back on exception.

## `SyncConnectionPool`
- `acquire() -> SyncConnection`: Returns a managed connection that is returned to the pool on close.
- `close()`: Drains the pool.
