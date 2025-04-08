import re
import csv

log_files = {
    "boot": "./results/benchmark-boot.txt",
    "malloc": "./results/benchmark-malloc.txt",
    "syscall": "./results/benchmark-syscall.txt",
    "tcp": "./results/benchmark-tcp.txt"
}

results = []

# Parse boot log
with open(log_files["boot"]) as f:
    start_time = end_time = None
    for line in f:
        if "boot benchmark: start" in line:
            print(f"Start Line: {line.strip()}")
            start_time = float(re.search(r"\[(.*?)\]", line).group(1))
        elif "boot benchmark: end" in line:
            print(f"End Line: {line.strip()}")
            end_time = float(re.search(r"\[(.*?)\]", line).group(1))
    if start_time and end_time:
        results.append({
            "benchmark": "boot",
            "operation": "boot duration",
            "detail": "system boot time",
            "value": round(end_time - start_time, 6),
            "unit": "seconds"
        })

# Parse malloc log
with open(log_files["malloc"]) as f:
    current_detail = ""
    for line in f:
        if "Allocating" in line:
            print(f"Malloc Alloc Line: {line.strip()}")
            match = re.search(r"Allocating (.*?) blocks of (.*?) bytes", line)
            if match:
                current_detail = f"{match.group(1)} blocks, {match.group(2)} bytes each"
        elif "Time taken" in line:
            print(f"Malloc Time Line: {line.strip()}")
            time = float(re.search(r"Time taken: (.*?) seconds", line).group(1))
            results.append({
                "benchmark": "malloc",
                "operation": "memory allocation",
                "detail": current_detail,
                "value": time,
                "unit": "seconds"
            })

# Parse syscall log
with open(log_files["syscall"]) as f:
    current_detail = ""
    for line in f:
        if "Invoking" in line:
            print(f"Syscall Invoking Line: {line.strip()}")
            current_detail = re.search(r"Invoking (.*?) \d+", line).group(1)
        elif "Time taken" in line:
            print(f"Syscall Time Line: {line.strip()}")
            time = float(re.search(r"Time taken: (.*?) seconds", line).group(1))
            results.append({
                "benchmark": "syscall",
                "operation": "syscall",
                "detail": current_detail,
                "value": time,
                "unit": "seconds"
            })

# Parse TCP log
with open(log_files["tcp"]) as f:
    for line in f:
        if "Sent" in line:
            print(f"TCP Sent Line: {line.strip()}")
            match = re.search(r"Sent (.*?) bytes in (.*?) seconds", line)
            if match:
                results.append({
                    "benchmark": "tcp",
                    "operation": "tcp send",
                    "detail": f"{match.group(1)} bytes",
                    "value": float(match.group(2)),
                    "unit": "seconds"
                })
        elif "Received" in line:
            print(f"TCP Received Line: {line.strip()}")
            match = re.search(r"Received (.*?) bytes in (.*?) seconds", line)
            if match:
                results.append({
                    "benchmark": "tcp",
                    "operation": "tcp receive",
                    "detail": f"{match.group(1)} bytes",
                    "value": float(match.group(2)),
                    "unit": "seconds"
                })

# Write to CSV
with open("parsed_benchmark_results.csv", "w", newline="") as csvfile:
    fieldnames = ["benchmark", "operation", "detail", "value", "unit"]
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()
    writer.writerows(results)

print("✅ Parsed benchmark results saved to 'parsed_benchmark_results.csv'")
