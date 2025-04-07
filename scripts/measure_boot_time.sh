#!/bin/bash

echo "[*] Measuring boot time..."

qemu-system-x86_64 -kernel benchmark-boot/build/boot.elf \
  -nographic -serial mon:stdio | \
  grep "BOOT_TIME" | tee results/boot_time.txt

