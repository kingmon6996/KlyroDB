# API Parity

Both `klyrodb-sync` and `klyrodb-async` are built atop the same C++ database engine. They offer exactly identical semantics, translating synchronous concepts into asynchronous contexts directly.

| Sync API | Async API |
|---|---|
| `db = klyrodb.open("app.klyro")` | `db = await klyrodb.open("app.klyro")` |
| `conn = db.connect()` | `conn = db.connect()` |
| `conn.execute(sql)` | `await conn.execute(sql)` |
| `for row in result:` | `async for row in result:` |
| `result.fetchone()` | `await result.fetchone()` |
| `result.fetchmany(n)` | `await result.fetchmany(n)` |
| `result.fetchall()` | `await result.fetchall()` |
| `with db.connect() as conn:` | `async with db.connect() as conn:` |
| `with conn.transaction():` | `async with conn.transaction():` |
| `conn.prepare(sql)` | `await conn.prepare(sql)` |
| `pool = klyrodb.ConnectionPool()` | `pool = klyrodb.ConnectionPool()` |
