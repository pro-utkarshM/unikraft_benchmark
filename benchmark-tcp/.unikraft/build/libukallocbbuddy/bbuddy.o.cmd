""gcc -nostdlib -U __linux__ -U __FreeBSD__ -U __sun__ -nostdinc -fno-stack-protector -Wall -Wextra -D __Unikraft__ -DUK_CODENAME="Helene" -DUK_VERSION=0.18 -DUK_FULLVERSION=0.18.0~8b39d45 -fno-tree-sra -fno-split-stack -fcf-protection=none -O2 -fno-omit-frame-pointer -fno-PIC -fno-tree-loop-distribute-patterns       -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/build/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/arch/x86/x86_64/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/devfs/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/isrlib/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/nolibc/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/nolibc/arch/x86_64 -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/nolibc/musl-imported/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/nolibc/musl-imported/arch/generic -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/nolibc/musl-imported/arch/x86_64 -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/posix-user/musl-imported/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/syscall_shim/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/syscall_shim/arch/x86_64/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukalloc/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukallocbbuddy/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukargparse/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukatomic/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukbitops/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukbitops/arch/x86_64/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukboot/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukconsole/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukdebug/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/uklibid/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/build/libuklibid/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukintctlr/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/uklibparam/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/uksched/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukschedcoop/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/uksp/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukstore/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/uktimeconv/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukvmem/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/vfscore/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukreloc/arch/x86_64/include/uk -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukreloc/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukallocstack/include -I/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/drivers/ukintctlr/xpic/include -D__X86_64__ -m64 -mno-red-zone -fno-asynchronous-unwind-tables -fno-reorder-blocks -mtune=generic -DCC_VERSION=13.3 -fno-builtin-printf -fno-builtin-fprintf -fno-builtin-sprintf -fno-builtin-snprintf -fno-builtin-vprintf -fno-builtin-vfprintf -fno-builtin-vsprintf -fno-builtin-vsnprintf -fno-builtin-scanf -fno-builtin-fscanf -fno-builtin-sscanf -fno-builtin-vscanf -fno-builtin-vfscanf -fno-builtin-vsscanf -DCONFIG_UK_NETDEV_SCRATCH_SIZE=0       -g3 -D__LIBNAME__=libukallocbbuddy -D__BASENAME__=bbuddy.c -c /home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/unikraft/lib/ukallocbbuddy/bbuddy.c -o /home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/build/libukallocbbuddy/bbuddy.o -Wp,-MD,/home/gilfoyle/Documents/unikraft-benchmark/benchmark-tcp/.unikraft/build/libukallocbbuddy/.bbuddy.o.d
