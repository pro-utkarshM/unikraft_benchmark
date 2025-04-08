import os
import csv
import re

result_dir = "results"
output_csv = os.path.join(result_dir, "benchmark_results.csv")

fields = ["File", "Benchmark", "Metric", "Value", "Unit"]

def parse_line(line, file_name):
    line = line.strip()
    if "[MALLOC]" in line:
        # Example: [MALLOC] Throughput: 843.12 Kops/sec
        match = re.search(r"Throughput:\s*([\d.]+)\s*Kops/sec", line)
        if match:
            return (file_name, "Malloc", "Malloc Ops/Sec", match.group(1), "Kops/sec")
    elif "SYSCALL_LATENCY" in line:
        # Example: SYSCALL_LATENCY: 5.26
        match = re.search(r"SYSCALL_LATENCY:\s*([\d.]+)", line)
        if match:
            return (file_name, "Syscall", "Syscall Latency", match.group(1), "us")
    elif "TCP_THROUGHPUT" in line:
        # Example: TCP_THROUGHPUT: 16.91
        match = re.search(r"TCP_THROUGHPUT:\s*([\d.]+)", line)
        if match:
            return (file_name, "TCP", "Throughput", match.group(1), "MB/s")
    return None

def extract_boot_time_from_log(lines, file_name):
    timestamps = []
    for line in lines:
        match = re.search(r'\[\s*(\d+\.\d+)\]', line)
        if match:
            timestamps.append(float(match.group(1)))
    if len(timestamps) >= 2:
        boot_time = timestamps[-1] - timestamps[0]
        return (file_name, "Boot", "Boot Time", f"{boot_time * 1000:.3f}", "ms")
    return None

rows = []

for file in os.listdir(result_dir):
    if not file.endswith(".txt"):
        continue
    file_path = os.path.join(result_dir, file)
    with open(file_path) as f:
        lines = f.readlines()
        # Extract boot time
        boot_data = extract_boot_time_from_log(lines, file)
        if boot_data:
            rows.append(boot_data)
        # Parse other metrics
        for line in lines:
            data = parse_line(line, file)
            if data:
                rows.append(data)

# Write to CSV
with open(output_csv, "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(fields)
    writer.writerows(rows)

print(f"[+] Parsed results written to {output_csv}")
