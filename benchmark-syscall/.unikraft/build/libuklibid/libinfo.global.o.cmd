""gcc -nostdlib -U __linux__ -U __FreeBSD__ -U __sun__ -nostdinc -fno-stack-protector -Wall -Wextra -D __Unikraft__ -DUK_CODENAME="Helene" -DUK_VERSION=0.18 -DUK_FULLVERSION=0.18.0 -fno-tree-sra -fno-split-stack -fcf-protection=none -O2 -fno-omit-frame-pointer -fno-PIC -fno-tree-loop-distribute-patterns   -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/arch/x86/x86_64/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/lib/syscall_shim/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/lib/syscall_shim/arch/x86_64/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/lib/uklibid/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/libuklibid/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/lib/ukintctlr/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/lib/uklibparam/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/lib/ukreloc/arch/x86_64/include/uk -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/lib/ukreloc/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/drivers/ukintctlr/xpic/include     -D__X86_64__ -m64 -mno-red-zone -fno-asynchronous-unwind-tables -fno-reorder-blocks -mtune=generic -DCC_VERSION=13.3 -D__ASSEMBLY__ -D__LIBUKLIBID_COMPILER__="GCC 13.3.0"     -D__GLOBALINFO__ -DUK_UTCDATE="2025-04-07T21:48:30+00:00"  -g3 -D__LIBNAME__=libuklibid -D__BASENAME__=libinfo.S -D__VARIANT__=global -c /home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/unikraft/lib/uklibid/libinfo.S -o /home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/libuklibid/libinfo.global.o -Wp,-MD,/home/gilfoyle/Documents/unikraft-benchmark/benchmark-syscall/.unikraft/build/libuklibid/.libinfo.global.o.d
