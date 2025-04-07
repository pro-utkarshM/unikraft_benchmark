cmd_/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/libkvmplat/link64.lds := /bin/bash /home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/libkvmplat/link64.lds.cmd

source_/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/libkvmplat/link64.lds := /home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/plat/kvm/x86/link64.lds.S

deps_/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/libkvmplat/link64.lds := \
  /home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/include/uk/arch/limits.h \
  /home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/include/uk/config.h \
  /home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/arch/x86/x86_64/include/uk/asm/limits.h \
    $(wildcard include/config/stack/size/page/order.h) \
    $(wildcard include/config/cpu/except/stack/size/page/order.h) \
    $(wildcard include/config/auxstack/size/page/order.h) \
  /home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/plat/common/include/uk/plat/common/common.lds.h \

/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/libkvmplat/link64.lds: $(deps_/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/libkvmplat/link64.lds)

$(deps_/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/libkvmplat/link64.lds):
