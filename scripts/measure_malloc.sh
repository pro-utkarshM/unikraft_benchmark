#!/bin/bash

echo "[*] Measuring malloc performance..."

qemu-system-x86_64 -kernel benchmark-malloc/build/malloc.elf \
  -nographic -serial mon:stdio | \
  grep "MALLOC_OPS" | tee results/malloc_ops.txt
