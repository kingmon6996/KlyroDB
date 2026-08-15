# Migrating from Sync to Async

Migrating your codebase to `klyrodb-async` is extremely straightforward due to our API parity rules.

### The Sync Way:
```python
import klyrodb

with klyrodb.open("app.klyro") as db:
    with db.connect() as conn:
        with conn.transaction():
            conn.execute("INSERT INTO test VALUES (?)", (1,))
            
        result = conn.execute("SELECT * FROM test")
        for row in result:
            print(row)
```

### The Async Way:
```python
import klyrodb

async def main():
    async with await klyrodb.open("app.klyro") as db:
        async with db.connect() as conn:
            async with conn.transaction():
                await conn.execute("INSERT INTO test VALUES (?)", (1,))
                
            result = await conn.execute("SELECT * FROM test")
            async for row in result:
                print(row)
```

**Key Differences to note:**
- `klyrodb.open()` is an awaitable in the async package, as checking/recovering WAL files on disk during initialization can block the event loop.
- Result sets use `async for` instead of fully buffering data.
- Do NOT use `asyncio.gather()` for multiple queries on the same connection. Instead, open a `ConnectionPool`.
