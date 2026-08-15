import asyncio

class KlyroError(Exception):
    def __init__(self, message: str, code: int = 0):
        super().__init__(message)
        self.code = code
        self.message = message

class DatabaseError(KlyroError): pass
class OperationalError(DatabaseError): pass
class IntegrityError(DatabaseError): pass
class ProgrammingError(DatabaseError): pass
class TransactionError(DatabaseError): pass
class ConcurrencyError(DatabaseError): pass
class TimeoutError(OperationalError): pass
class CancelledError(asyncio.CancelledError, OperationalError): pass
