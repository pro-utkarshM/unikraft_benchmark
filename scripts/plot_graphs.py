import csv
import matplotlib.pyplot as plt

csv_file = "results/benchmark_results.csv"

benchmarks = []
values = []
units = []

with open(csv_file, "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        benchmarks.append(f"{row['Benchmark']} ({row['Metric']})")
        values.append(float(row["Value"]))
        units.append(row["Unit"])

plt.figure(figsize=(10, 6))
bars = plt.bar(benchmarks, values, color="skyblue", edgecolor="black")

for bar, unit in zip(bars, units):
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2.0, yval + 0.1, f'{yval} {unit}',
             ha='center', va='bottom', fontsize=10)

plt.title("Unikraft Benchmark Results")
plt.ylabel("Measured Values")
plt.xticks(rotation=30, ha="right")
plt.tight_layout()
plt.grid(axis='y', linestyle='--', alpha=0.7)

plt.savefig("results/benchmark_graph.png")
plt.show()
