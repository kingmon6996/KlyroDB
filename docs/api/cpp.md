# C++ API Reference

KlyroDB is engineered from the ground up in modern C++20. The C++ engine manages storage, MVCC, query planning, execution, and locking.

*Note: The C++ Native API is used directly when embedding KlyroDB into a C++ application. The Python SDK delegates to this API automatically.*

## Database
- `Database open(const std::string& path)`: Opens or creates a database at `path`.

## Connection
- `Connection Database::connect()`: Acquires a thread-bound connection to the database.
- `Result Connection::execute(const std::string& sql, const std::vector<Value>& params, double timeout_ms)`: Executes a SQL statement.
- `void Connection::executemany(const std::string& sql, const std::vector<std::vector<Value>>& params)`: Batch executes a SQL statement.
- `PreparedStatement Connection::prepare(const std::string& sql)`: Prepares a SQL statement for rapid execution.
- `void Connection::close()`: Closes the connection.

## PreparedStatement
- `Result PreparedStatement::execute(const std::vector<Value>& params)`: Executes the prepared statement.

## Result
- `std::optional<Row> Result::fetchone()`: Fetches the next row.
- `std::vector<Row> Result::fetchmany(size_t size)`: Fetches up to `size` rows.
- `std::vector<Row> Result::fetchall()`: Fetches all remaining rows.

## ConnectionPool
- `ConnectionPool Database::create_pool(size_t max_connections)`: Creates a managed thread-safe pool.
- `Connection ConnectionPool::acquire()`: Borrows a connection.
- `void ConnectionPool::close()`: Closes the pool and drains connections.
