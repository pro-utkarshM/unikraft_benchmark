""gcc -nostdlib -U __linux__ -U __FreeBSD__ -U __sun__ -nostdinc -fno-stack-protector -Wall -Wextra -D __Unikraft__ -DUK_CODENAME="Helene" -DUK_VERSION=0.18 -DUK_FULLVERSION=0.18.0~9b10442 -fno-tree-sra -fno-split-stack -fcf-protection=none -O2 -fno-omit-frame-pointer -fno-PIC -fno-tree-loop-distribute-patterns   -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/arch/x86/x86_64/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/syscall_shim/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/syscall_shim/arch/x86_64/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uklibid/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/libuklibid/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukintctlr/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uklibparam/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukreloc/arch/x86_64/include/uk -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukreloc/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukintctlr/xpic/include  -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/plat/kvm/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/plat/common/include   -D__X86_64__ -m64 -mno-red-zone -fno-asynchronous-unwind-tables -fno-reorder-blocks -mtune=generic -DCC_VERSION=13.3 -D__ASSEMBLY__ -D__LIBUKLIBID_COMPILER__="GCC 13.3.0"  -DKVMPLAT -DUK_USE_SECTION_SEGMENTS     -g3 -D__LIBNAME__=libkvmplat -D__BASENAME__=lcpu_start.S -c /home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/plat/kvm/x86/lcpu_start.S -o /home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/libkvmplat/lcpu_start.o -Wp,-MD,/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/libkvmplat/.lcpu_start.o.d
