# SQL Compatibility

KlyroDB supports a core subset of relational SQL designed to be intuitive and highly performant.

## DDL (Data Definition Language)
- `CREATE TABLE [IF NOT EXISTS] table_name (column defs)`
- `ALTER TABLE table_name [ADD COLUMN ...]`
- `DROP TABLE [IF EXISTS] table_name`
- `CREATE INDEX [IF NOT EXISTS] index_name ON table_name (columns)`

## DML (Data Manipulation Language)
- `INSERT INTO table_name (cols) VALUES (?, ?)`
- `SELECT cols FROM table_name [WHERE condition] [ORDER BY cols] [LIMIT n OFFSET m]`
- `UPDATE table_name SET col = val WHERE condition`
- `DELETE FROM table_name WHERE condition`

## Functions

### Aggregate Functions
- `count(column)`: Counts non-null rows.
- `sum(column)`: Sums numeric values.
- `avg(column)`: Averages numeric values.

### ARRAY Collection Functions
- `array_length(ARRAY) -> INTEGER`
- `array_append(ARRAY, val) -> ARRAY`
- `array_prepend(ARRAY, val) -> ARRAY`
- `array_pop(ARRAY) -> ARRAY`
- `array_remove(ARRAY, val) -> ARRAY`
- `array_contains(ARRAY, val) -> BOOLEAN`
- `array_position(ARRAY, val) -> INTEGER`
- `array_slice(ARRAY, start, end) -> ARRAY`
- `array_concat(ARRAY, ARRAY) -> ARRAY`

### DICT Collection Functions
- `dict_get(DICT, key) -> ANY`
- `dict_set(DICT, key, val) -> DICT`
- `dict_insert(DICT, key, val) -> DICT`
- `dict_remove(DICT, key) -> DICT`
- `dict_contains(DICT, key) -> BOOLEAN`
- `dict_keys(DICT) -> ARRAY`
- `dict_values(DICT) -> ARRAY`
- `dict_items(DICT) -> ARRAY`
- `dict_size(DICT) -> INTEGER`

### JSON Collection Functions
- `json_get(JSON, path) -> ANY`
- `json_set(JSON, path, val) -> JSON`
- `json_remove(JSON, path) -> JSON`
- `json_type(JSON, path) -> TEXT`
- `json_array_length(JSON, path) -> INTEGER`
- `json_keys(JSON, path) -> ARRAY`
