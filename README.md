# Unikraft Benchmark Suite

This repository contains a set of benchmarking tools and scripts to evaluate the performance of [Unikraft](https://unikraft.org) unikernels across key system metrics.

## Overview

Unikraft is a fast, lightweight unikernel system optimized for running single-purpose applications with minimal resource overhead. This project aims to benchmark various aspects of Unikraft's performance to aid in optimization, research, and system tuning.

The benchmarking suite includes:

-  **Syscall latency**
-  **Memory allocation performance**
-  **TCP throughput**
-  **Boot time**
-  **CPU and memory usage**
-  **Disk I/O performance**

##  Repository Structure

```
.
├── benchmark-boot
│   ├── kraft.yaml
│   ├── main.c
│   └── Makefile.uk
├── benchmark-malloc
│   ├── kraft.yaml
│   ├── main.c
│   └── Makefile.uk
├── benchmark-syscall
│   ├── kraft.yaml
│   ├── main.c
│   └── Makefile.uk
├── benchmark-tcp
│   ├── client.c
│   ├── kraft.yaml
│   ├── Makefile.uk
│   └── server.c
├── parsed_benchmark_results.csv
├── README.md
├── results
│   ├── benchmark-boot.txt
│   ├── benchmark_graph.png
│   ├── benchmark-malloc.txt
│   ├── benchmark-syscall.txt
│   ├── benchmark-tcp.txt
│   └── parsed_benchmark_results.csv
└── scripts
    ├── measure_boot_time.sh
    ├── measure_malloc.sh
    ├── measure_syscall.sh
    ├── measure_tcp.sh
    ├── parse_results.py
    ├── plot_graphs.py
    └── run_all.sh

7 directories, 28 files
```

## ⚙️ Setup

### Prerequisites

- Linux-based system
- [Unikraft Tooling (kraftkit)](https://unikraft.org/docs/tools/kraftkit/)
- Docker (optional, for containerized benchmarking)
- QEMU or Firecracker (for running virtualized workloads)

### Install KraftKit

```bash
curl -sSfL https://get.kraftkit.sh | sh
```

Or follow the latest instructions at [kraftkit.sh](https://get.kraftkit.sh).

### Clone the Repo

```bash
git clone https://github.com/pro-utkarshM/unikraft_benchmark.git
cd unikraft_benchmark
```

## Usage


```bash
chmod +x ./scripts/run_all.sh
```

Benchmark-specific instructions can be found in their respective subfolders.

## Goals

This benchmarking suite is part of a broader effort under **Google Summer of Code 2025** to:

- Identify bottlenecks in Unikraft's performance
- Contribute performance improvements upstream
- Provide reproducible performance test cases
- Evaluate unikernels for production scenarios

## Benchmarks Under Development

- [ ] Boot Time Benchmark
- [ ] Syscall Latency Benchmark
- [ ] TCP Throughput Benchmark
- [ ] Memory Usage Tracker
- [ ] CPU Utilization Monitor
- [ ] Disk I/O Benchmark

## Contributing

Pull requests, issues, and ideas are welcome!

If you'd like to contribute:

1. Fork the repo
2. Create a new branch
3. Commit your changes
4. Open a pull request

## License

MIT License. See `LICENSE` file for details.

## Author

Made by [Utkarsh Maurya](https://github.com/pro-utkarshM)

This project is a part of GSoC 2025 with [Unikraft](https://unikraft.org).

---
