import asyncio
import logging
import uuid
from typing import Sequence, Optional, AsyncIterator
from .logging import conn_logger, txn_logger, pool_logger, async_logger, QueryContext

class AsyncResult(AsyncIterator):
    def __init__(self, native_result, loop):
        self._native = native_result
        self._loop = loop

    async def fetchone(self) -> Optional[tuple]:
        return await self._loop.run_in_executor(None, self._native.fetchone)

    async def fetchmany(self, size: int) -> list[tuple]:
        return await self._loop.run_in_executor(None, self._native.fetchmany, size)

    async def fetchall(self) -> list[tuple]:
        return await self._loop.run_in_executor(None, self._native.fetchall)

    def __aiter__(self):
        return self

    async def __anext__(self) -> tuple:
        row = await self.fetchone()
        if row is None:
            raise StopAsyncIteration
        return row

class AsyncPreparedStatement:
    def __init__(self, native_stmt, loop, conn_id: str):
        self._native = native_stmt
        self._loop = loop
        self._conn_id = conn_id

    async def execute(self, *parameters: object) -> AsyncResult:
        with QueryContext("EXECUTE_PREPARED_ASYNC", "<prepared_statement>", parameters, self._conn_id, is_async=True):
            native_result = await self._loop.run_in_executor(None, self._native.execute, parameters)
            return AsyncResult(native_result, self._loop)

class AsyncTransaction:
    def __init__(self, connection):
        self._conn = connection
        self._id = uuid.uuid4().hex[:8]

    async def __aenter__(self):
        if txn_logger.isEnabledFor(logging.DEBUG):
            txn_logger.debug(f"Async Transaction {self._id} started", extra={"conn_id": self._conn._id, "txn_id": self._id})
        await self._conn.execute("BEGIN TRANSACTION")
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        if exc_type is None:
            if txn_logger.isEnabledFor(logging.DEBUG):
                txn_logger.debug(f"Async Transaction {self._id} commit", extra={"conn_id": self._conn._id, "txn_id": self._id})
            await self._conn.execute("COMMIT")
        else:
            if txn_logger.isEnabledFor(logging.WARNING):
                txn_logger.warning(f"Async Transaction {self._id} rollback due to exception", extra={"conn_id": self._conn._id, "txn_id": self._id, "error": str(exc_val)})
            await self._conn.execute("ROLLBACK")
        return False

class AsyncConnection:
    def __init__(self, native_conn, loop):
        self._native = native_conn
        self._loop = loop
        self._id = uuid.uuid4().hex[:8]
        if conn_logger.isEnabledFor(logging.DEBUG):
            conn_logger.debug(f"Async Connection acquired", extra={"conn_id": self._id})

    async def execute(self, sql: str, parameters: Optional[Sequence[object]] = None, timeout: float = 0.0) -> AsyncResult:
        with QueryContext("EXECUTE_ASYNC", sql, parameters or (), self._id, is_async=True):
            task = self._loop.run_in_executor(None, self._native.execute, sql, parameters or (), timeout)
            try:
                native_result = await task
                return AsyncResult(native_result, self._loop)
            except asyncio.CancelledError:
                if async_logger.isEnabledFor(logging.DEBUG):
                    async_logger.debug("Async task cancelled, cancelling native operation")
                self._native.cancel()
                raise

    async def executemany(self, sql: str, parameters_list: Sequence[Sequence[object]]):
        with QueryContext("EXECUTEMANY_ASYNC", sql, [], self._id, is_async=True):
            task = self._loop.run_in_executor(None, self._native.executemany, sql, parameters_list)
            try:
                await task
            except asyncio.CancelledError:
                if async_logger.isEnabledFor(logging.DEBUG):
                    async_logger.debug("Async executemany task cancelled, cancelling native operation")
                self._native.cancel()
                raise

    async def prepare(self, sql: str) -> AsyncPreparedStatement:
        if conn_logger.isEnabledFor(logging.DEBUG):
            conn_logger.debug(f"Async Prepared statement created", extra={"conn_id": self._id})
        native_stmt = await self._loop.run_in_executor(None, self._native.prepare, sql)
        return AsyncPreparedStatement(native_stmt, self._loop, self._id)

    def transaction(self) -> AsyncTransaction:
        return AsyncTransaction(self)

    async def __aenter__(self):
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        await self.close()

    async def close(self):
        if conn_logger.isEnabledFor(logging.DEBUG):
            conn_logger.debug(f"Async Connection closed", extra={"conn_id": self._id})
        await self._loop.run_in_executor(None, self._native.close)

class AsyncConnectionPool:
    def __init__(self, db, max_connections: int = 16):
        self._db = db
        self._loop = asyncio.get_running_loop()
        self._native = db._native.create_pool(max_connections)
        self._max_connections = max_connections
        if pool_logger.isEnabledFor(logging.INFO):
            pool_logger.info(f"Async Connection pool created", extra={"max_connections": max_connections})

    async def acquire(self) -> AsyncConnection:
        try:
            native_conn = await self._loop.run_in_executor(None, self._native.acquire)
            return AsyncConnection(native_conn, self._loop)
        except Exception as e:
            pool_logger.warning("Async Connection pool exhaustion or acquisition failure", exc_info=True)
            raise

    async def close(self):
        if pool_logger.isEnabledFor(logging.INFO):
            pool_logger.info(f"Async Connection pool closed")
        await self._loop.run_in_executor(None, self._native.close)

    async def __aenter__(self):
        return self

    async def __aexit__(self, exc_type, exc_val, exc_tb):
        await self.close()
