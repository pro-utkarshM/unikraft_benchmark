""strip -s -R .uk_tracepoints_list -R .uk_trace_keyvals /home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/build/benchmark-tcp_qemu-x86_64.dbg -o /home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/build/benchmark-tcp_qemu-x86_64 2>&1 | { grep -Ev "Empty loadable segment detected|section.*lma.*adjusted to.*" || true; }
