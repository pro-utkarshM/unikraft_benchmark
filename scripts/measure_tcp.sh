#!/bin/bash

echo "[*] Measuring TCP throughput..."

# Start server in background
qemu-system-x86_64 -kernel benchmark-tcp/build/server.elf \
  -nographic -serial mon:stdio &

sleep 2

# Run client
qemu-system-x86_64 -kernel benchmark-tcp/build/client.elf \
  -nographic -serial mon:stdio | \
  grep "TCP_THROUGHPUT" | tee results/tcp_throughput.txt

# Kill background server
pkill -f server.elf
