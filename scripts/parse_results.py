import os
import csv
import re

result_dir = "results"
output_csv = os.path.join(result_dir, "benchmark_results.csv")

fields = ["Benchmark", "Metric", "Value", "Unit"]

def parse_line(line):
    if "MALLOC_OPS" in line:
        parts = line.strip().split(":")
        return ("Malloc", "Malloc Ops/Sec", parts[1].strip(), "ops/sec")
    elif "SYSCALL_LATENCY" in line:
        parts = line.strip().split(":")
        return ("Syscall", "Syscall Latency", parts[1].strip(), "us")
    elif "TCP_THROUGHPUT" in line:
        parts = line.strip().split(":")
        return ("TCP", "Throughput", parts[1].strip(), "MB/s")
    else:
        return None

def extract_boot_time_from_log(lines):
    timestamps = []
    for line in lines:
        match = re.search(r'\[\s*(\d+\.\d+)\]', line)
        if match:
            timestamps.append(float(match.group(1)))
    if len(timestamps) >= 2:
        boot_time = timestamps[-1] - timestamps[0]
        return ("Boot", "Boot Time", f"{boot_time * 1000:.3f}", "ms")  # Convert to ms
    return None

rows = []

for file in os.listdir(result_dir):
    if not file.endswith(".txt"):
        continue
    file_path = os.path.join(result_dir, file)
    with open(file_path) as f:
        lines = f.readlines()
        # Extract boot time first
        boot_data = extract_boot_time_from_log(lines)
        if boot_data:
            rows.append(boot_data)
        # Then parse other metrics
        for line in lines:
            data = parse_line(line)
            if data:
                rows.append(data)

with open(output_csv, "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(fields)
    writer.writerows(rows)

print(f"[+] Results written to {output_csv}")
