#!/bin/bash

# Go to the root of the project
ROOT_DIR=$(dirname "$(realpath "$0")")/..
cd "$ROOT_DIR" || exit

# Ensure results directory exists
mkdir -p results

# Array of benchmarks
BENCHMARKS=("benchmark-syscall" "benchmark-malloc" "benchmark-tcp" "benchmark-boot")

# Run each benchmark
for bench in "${BENCHMARKS[@]}"; do
  echo "🛠 Building and running $bench..."
  cd "$bench" || exit 1
  kraft build || { echo "❌ Build failed for $bench"; exit 1; }

  # Run and log output to results/
  kraft run > "../results/${bench}.txt" 2>&1
  cd "$ROOT_DIR" || exit
done

echo "✅ All benchmarks completed."
