import os
import csv

result_dir = "results"
output_csv = os.path.join(result_dir, "benchmark_results.csv")

fields = ["Benchmark", "Metric", "Value", "Unit"]

def parse_line(line):
    if "BOOT_TIME" in line:
        parts = line.strip().split(":")
        return ("Boot", "Boot Time", parts[1].strip(), "ms")
    elif "MALLOC_OPS" in line:
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

rows = []

for file in os.listdir(result_dir):
    if not file.endswith(".txt"):
        continue
    with open(os.path.join(result_dir, file)) as f:
        for line in f:
            data = parse_line(line)
            if data:
                rows.append(data)

with open(output_csv, "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(fields)
    writer.writerows(rows)

print(f"[+] Results written to {output_csv}")
