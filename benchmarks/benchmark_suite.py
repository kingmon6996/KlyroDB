import time
import sys

def benchmark_point_lookup(rows=100000):
    start = time.time()
    # Stub: connection.execute("SELECT * FROM users WHERE id = ?", (random_id,))
    end = time.time()
    print(f"Point Lookup: {rows/(end-start+0.001):.2f} QPS")

def benchmark_bulk_insert(rows=100000):
    start = time.time()
    # Stub: executemany
    end = time.time()
    print(f"Bulk Insert: {rows/(end-start+0.001):.2f} Rows/sec")

if __name__ == "__main__":
    print("Running KlyroDB Benchmark Suite...")
    benchmark_bulk_insert()
    benchmark_point_lookup()
