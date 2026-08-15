#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>
#include "klyro/api/database.hpp"
#include "klyro/api/connection.hpp"
#include "klyro/api/prepared_statement.hpp"
#include "klyro/api/transaction.hpp"
#include "klyro/api/result.hpp"
#include "klyro/api/row.hpp"

namespace py = pybind11;
using namespace klyro::api;

PYBIND11_MODULE(_klyro, m) {
    m.doc() = "KlyroDB native extension";

    // Exceptions
    static py::exception<std::runtime_error> KlyroError(m, "KlyroError");
    static py::exception<std::runtime_error> KlyroDatabaseError(m, "KlyroDatabaseError", KlyroError.ptr());
    static py::exception<std::runtime_error> KlyroOperationalError(m, "KlyroOperationalError", KlyroDatabaseError.ptr());
    static py::exception<std::runtime_error> KlyroIntegrityError(m, "KlyroIntegrityError", KlyroDatabaseError.ptr());
    static py::exception<std::runtime_error> KlyroProgrammingError(m, "KlyroProgrammingError", KlyroDatabaseError.ptr());
    static py::exception<std::runtime_error> KlyroTransactionError(m, "KlyroTransactionError", KlyroDatabaseError.ptr());
    static py::exception<std::runtime_error> KlyroConcurrencyError(m, "KlyroConcurrencyError", KlyroDatabaseError.ptr());
    static py::exception<std::runtime_error> KlyroTimeoutError(m, "KlyroTimeoutError", KlyroDatabaseError.ptr());
    static py::exception<std::runtime_error> KlyroCancelledError(m, "KlyroCancelledError", KlyroDatabaseError.ptr());
    static py::exception<std::runtime_error> KlyroRecoveryError(m, "KlyroRecoveryError", KlyroDatabaseError.ptr());

    // Map klyro::api::DatabaseConfig
    py::class_<klyro::DatabaseConfig>(m, "DatabaseConfig")
        .def(py::init<>())
        .def_readwrite("buffer_pool_size", &klyro::DatabaseConfig::buffer_pool_size)
        .def_readwrite("worker_count", &klyro::DatabaseConfig::worker_count)
        .def_readwrite("connection_pool_size", &klyro::DatabaseConfig::connection_pool_size);
        
    py::class_<ConnectionConfig>(m, "ConnectionConfig")
        .def(py::init<>())
        .def_readwrite("read_only", &ConnectionConfig::read_only);
        
    py::class_<QueryStatistics>(m, "QueryStatistics")
        .def_readonly("planning_time_ms", &QueryStatistics::planning_time_ms)
        .def_readonly("execution_time_ms", &QueryStatistics::execution_time_ms)
        .def_readonly("rows_returned", &QueryStatistics::rows_returned)
        .def_readonly("rows_scanned", &QueryStatistics::rows_scanned)
        .def_readonly("pages_read", &QueryStatistics::pages_read)
        .def_readonly("pages_written", &QueryStatistics::pages_written)
        .def_readonly("buffer_hits", &QueryStatistics::buffer_hits)
        .def_readonly("memory_used_bytes", &QueryStatistics::memory_used_bytes);

    py::class_<ColumnMetadata>(m, "ColumnMetadata")
        .def_readonly("name", &ColumnMetadata::name)
        .def_readonly("type", &ColumnMetadata::type)
        .def_readonly("nullable", &ColumnMetadata::nullable)
        .def_readonly("table", &ColumnMetadata::table);

#include "klyro/types/value.hpp"
#include "klyro/types/array.hpp"
#include "klyro/types/dict.hpp"
#include "klyro/types/json.hpp"

namespace {
    py::object value_to_py(const klyro::types::Value& v) {
        if (v.is_null()) return py::none();
        switch (v.type()) {
            case klyro::types::TypeID::Boolean: return py::bool_(v.get<bool>());
            case klyro::types::TypeID::SmallInt: return py::int_(v.get<std::int16_t>());
            case klyro::types::TypeID::Integer: return py::int_(v.get<std::int32_t>());
            case klyro::types::TypeID::BigInt: return py::int_(v.get<std::int64_t>());
            case klyro::types::TypeID::Real: return py::float_(v.get<float>());
            case klyro::types::TypeID::Double: return py::float_(v.get<double>());
            case klyro::types::TypeID::Text:
            case klyro::types::TypeID::VarChar:
            case klyro::types::TypeID::Char: return py::str(v.get<std::string>());
            case klyro::types::TypeID::JSON: return py::str(v.get<klyro::types::Json>().to_string());
            case klyro::types::TypeID::Array: {
                py::list l;
                const auto& arr = v.get<klyro::types::Array>();
                for (size_t i = 0; i < arr.size(); ++i) {
                    l.append(value_to_py(arr.at(i)));
                }
                return l;
            }
            case klyro::types::TypeID::DICT: {
                py::dict d;
                const auto& dict = v.get<klyro::types::Dict>();
                for (const auto& k : dict.keys()) {
                    d[py::str(k)] = value_to_py(*dict.get(k));
                }
                return d;
            }
            default: return py::str(v.to_string()); // Fallback
        }
    }

    klyro::types::Value py_to_value(py::handle obj) {
        if (obj.is_none()) return klyro::types::Value();
        if (py::isinstance<py::bool_>(obj)) return klyro::types::Value(obj.cast<bool>());
        if (py::isinstance<py::int_>(obj)) return klyro::types::Value(obj.cast<std::int64_t>());
        if (py::isinstance<py::float_>(obj)) return klyro::types::Value(obj.cast<double>());
        if (py::isinstance<py::str>(obj)) return klyro::types::Value(obj.cast<std::string>(), klyro::types::TypeID::Text);
        if (py::isinstance<py::list>(obj)) {
            py::list l = obj.cast<py::list>();
            klyro::types::Array arr(klyro::types::TypeID::Invalid);
            for (auto item : l) {
                arr.push_back(py_to_value(item));
            }
            return klyro::types::Value(std::move(arr));
        }
        if (py::isinstance<py::dict>(obj)) {
            py::dict d = obj.cast<py::dict>();
            klyro::types::Dict dict;
            for (auto item : d) {
                std::string key = py::str(item.first);
                dict.set(key, py_to_value(item.second));
            }
            return klyro::types::Value(std::move(dict));
        }
        return klyro::types::Value();
    }
}

// ... inside PYBIND11_MODULE

    py::class_<Row>(m, "Row")
        .def("get_by_index", [](const Row& r, std::size_t i) -> py::object {
            return value_to_py(r.at(i));
        })
        .def("get_by_name", [](const Row& r, const std::string& name) -> py::object {
            return value_to_py(r.get(name));
        })
        .def("__len__", &Row::size);

    py::class_<Result>(m, "Result")
        .def("has_rows", &Result::has_rows)
        .def("affected_rows", &Result::affected_rows)
        .def("columns", &Result::columns)
        .def("statistics", &Result::statistics)
        .def("next", [](Result& self) -> std::unique_ptr<Row> {
            return self.next(); // Handled as std::unique_ptr returning None if nullptr
        });

    py::class_<Transaction>(m, "Transaction")
        .def("commit", [](Transaction& self) {
            auto res = self.commit();
            if (!res) throw std::runtime_error("Commit failed");
        })
        .def("rollback", [](Transaction& self) {
            auto res = self.rollback();
            if (!res) throw std::runtime_error("Rollback failed");
        });

    py::class_<PreparedStatement>(m, "PreparedStatement")
        .def("bind", [](PreparedStatement& self, std::size_t index, py::object value) {
            auto res = self.bind(index, py_to_value(value));
            if (!res) throw std::runtime_error("Bind failed");
        })
        .def("execute", [](PreparedStatement& self) -> Result {
            py::gil_scoped_release release;
            auto res = self.execute();
            if (!res) throw std::runtime_error("Execute failed");
            return std::move(res.value());
        });

    py::class_<Connection>(m, "Connection")
        .def("execute", [](Connection& self, const std::string& sql) -> Result {
            py::gil_scoped_release release;
            auto res = self.execute(sql);
            if (!res) throw std::runtime_error("Execute failed");
            return std::move(res.value());
        })
        .def("prepare", [](Connection& self, const std::string& sql) -> PreparedStatement {
            auto res = self.prepare(sql);
            if (!res) throw std::runtime_error("Prepare failed");
            return std::move(res.value());
        })
        .def("transaction", [](Connection& self) -> Transaction {
            auto res = self.transaction();
            if (!res) throw std::runtime_error("Transaction begin failed");
            return std::move(res.value());
        })
        .def("set_autocommit", &Connection::set_autocommit)
        .def("cancel", &Connection::cancel)
        .def("close", &Connection::close);

    py::class_<klyro::Database>(m, "Database")
        .def("connect", [](klyro::Database& self, const ConnectionConfig& config) -> Connection {
            auto res = self.connect(config);
            if (!res) throw std::runtime_error("Connect failed");
            return std::move(res.value());
        })
        .def("close", [](klyro::Database& self) {
            auto res = self.close();
            if (!res) throw std::runtime_error("Close failed");
        })
        .def_static("open", [](const std::string& path, const klyro::DatabaseConfig& config) -> klyro::Database {
            auto res = klyro::Database::open(path, config);
            if (!res) throw std::runtime_error("Open failed");
            return std::move(res.value());
        })
        .def_static("engine_version", &klyro::Database::engine_version);
}
