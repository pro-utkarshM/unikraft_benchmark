#!/bin/bash

echo "[*] Measuring syscall latency..."

qemu-system-x86_64 -kernel benchmark-syscall/build/syscall.elf \
  -nographic -serial mon:stdio | \
  grep "SYSCALL_LATENCY" | tee results/syscall_latency.txt
