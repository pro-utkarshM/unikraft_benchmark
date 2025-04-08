import os
import csv
import re

# Directory where the benchmark files are
RESULT_DIR = "./results"
OUTPUT_CSV = os.path.join(RESULT_DIR, "parsed_benchmark_results.csv")
FIELDS = ["test_name", "result", "units"]

def parse_boot_log(file_path):
    with open(file_path) as f:
        timestamps = []
        for line in f:
            match = re.search(r"\[\s*(\d+\.\d+)\]", line)
            if match:
                timestamps.append(float(match.group(1)))
        if len(timestamps) >= 2:
            boot_time = (timestamps[-1] - timestamps[0]) * 1000
            return [("boot_time", f"{boot_time:.3f}", "ms")]
    return []

def parse_malloc_log(file_path):
    with open(file_path) as f:
        for line in f:
            if "MALLOC_OPS" in line:
                parts = line.strip().split(":")
                return [("malloc_ops_per_sec", parts[1].strip(), "ops/sec")]
    return []

def parse_syscall_log(file_path):
    with open(file_path) as f:
        for line in f:
            if "SYSCALL_LATENCY" in line:
                parts = line.strip().split(":")
                return [("syscall_latency", parts[1].strip(), "us")]
    return []

def parse_tcp_log(file_path):
    with open(file_path) as f:
        for line in f:
            if "TCP_THROUGHPUT" in line:
                parts = line.strip().split(":")
                return [("tcp_throughput", parts[1].strip(), "MB/s")]
    return []

# File-to-parser map
PARSERS = {
    "benchmark-boot.txt": parse_boot_log,
    "benchmark-malloc.txt": parse_malloc_log,
    "benchmark-syscall.txt": parse_syscall_log,
    "benchmark-tcp.txt": parse_tcp_log
}

rows = []
for filename, parser in PARSERS.items():
    path = os.path.join(RESULT_DIR, filename)
    if os.path.exists(path):
        result = parser(path)
        rows.extend(result)
    else:
        print(f"[!] Missing file: {filename}")

# Write to CSV
with open(OUTPUT_CSV, "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(FIELDS)
    writer.writerows(rows)

print(f"[+] Wrote results to {OUTPUT_CSV}")
