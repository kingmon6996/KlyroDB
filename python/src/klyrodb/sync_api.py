import logging
import uuid
from typing import Sequence, Optional, Iterator
from .logging import conn_logger, txn_logger, pool_logger, QueryContext

class SyncResult(Iterator):
    def __init__(self, native_result):
        self._native = native_result

    def fetchone(self) -> Optional[tuple]:
        return self._native.fetchone()

    def fetchmany(self, size: int) -> list[tuple]:
        return self._native.fetchmany(size)

    def fetchall(self) -> list[tuple]:
        return self._native.fetchall()

    def __iter__(self):
        return self

    def __next__(self) -> tuple:
        row = self.fetchone()
        if row is None:
            raise StopIteration
        return row

class SyncPreparedStatement:
    def __init__(self, native_stmt, conn_id: str):
        self._native = native_stmt
        self._conn_id = conn_id

    def execute(self, *parameters: object) -> SyncResult:
        with QueryContext("EXECUTE_PREPARED", "<prepared_statement>", parameters, self._conn_id):
            return SyncResult(self._native.execute(parameters))

class SyncTransaction:
    def __init__(self, connection):
        self._conn = connection
        self._id = uuid.uuid4().hex[:8]

    def __enter__(self):
        if txn_logger.isEnabledFor(logging.DEBUG):
            txn_logger.debug(f"Transaction {self._id} started", extra={"conn_id": self._conn._id, "txn_id": self._id})
        self._conn.execute("BEGIN TRANSACTION")
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type is None:
            if txn_logger.isEnabledFor(logging.DEBUG):
                txn_logger.debug(f"Transaction {self._id} commit", extra={"conn_id": self._conn._id, "txn_id": self._id})
            self._conn.execute("COMMIT")
        else:
            if txn_logger.isEnabledFor(logging.WARNING):
                txn_logger.warning(f"Transaction {self._id} rollback due to exception", extra={"conn_id": self._conn._id, "txn_id": self._id, "error": str(exc_val)})
            self._conn.execute("ROLLBACK")
        return False

class SyncConnection:
    def __init__(self, native_conn):
        self._native = native_conn
        self._id = uuid.uuid4().hex[:8]
        if conn_logger.isEnabledFor(logging.DEBUG):
            conn_logger.debug(f"Connection acquired", extra={"conn_id": self._id})

    def execute(self, sql: str, parameters: Optional[Sequence[object]] = None, timeout: float = 0.0) -> SyncResult:
        with QueryContext("EXECUTE", sql, parameters or (), self._id):
            return SyncResult(self._native.execute(sql, parameters or (), timeout))

    def executemany(self, sql: str, parameters_list: Sequence[Sequence[object]]):
        with QueryContext("EXECUTEMANY", sql, [], self._id):
            self._native.executemany(sql, parameters_list)

    def prepare(self, sql: str) -> SyncPreparedStatement:
        if conn_logger.isEnabledFor(logging.DEBUG):
            conn_logger.debug(f"Prepared statement created", extra={"conn_id": self._id})
        return SyncPreparedStatement(self._native.prepare(sql), self._id)

    def transaction(self) -> SyncTransaction:
        return SyncTransaction(self)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()

    def close(self):
        if conn_logger.isEnabledFor(logging.DEBUG):
            conn_logger.debug(f"Connection closed", extra={"conn_id": self._id})
        self._native.close()

class SyncConnectionPool:
    def __init__(self, db, max_connections: int = 16):
        self._db = db
        self._native = db._native.create_pool(max_connections)
        self._max_connections = max_connections
        if pool_logger.isEnabledFor(logging.INFO):
            pool_logger.info(f"Connection pool created", extra={"max_connections": max_connections})

    def acquire(self) -> SyncConnection:
        try:
            native_conn = self._native.acquire()
            return SyncConnection(native_conn)
        except Exception as e:
            pool_logger.warning("Connection pool exhaustion or acquisition failure", exc_info=True)
            raise

    def close(self):
        if pool_logger.isEnabledFor(logging.INFO):
            pool_logger.info(f"Connection pool closed")
        self._native.close()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()
