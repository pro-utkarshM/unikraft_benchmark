deps_config := \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/app.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/vfscore/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukvmem/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uktimeconv/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uktest/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukstreambuf/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukstore/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uksp/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uksglist/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukschedcoop/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uksched/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukrust/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukring/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukreloc/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukrandom/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukofw/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uknofault/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uknetdev/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukmpi/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukmmap/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uklock/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uklibparam/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uklibid/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukintctlr/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukgcov/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukfile/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukfallocbuddy/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukfalloc/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukdebug/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukcpio/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukconsole/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukbus/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukboot/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukblkdev/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukbitops/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukbinfmt/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukatomic/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukargparse/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukallocstack/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukallocregion/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukallocpool/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukallocbbuddy/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ukalloc/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/uk9p/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ubsan/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/syscall_shim/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/ramfs/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-user/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-unixsocket/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-tty/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-timerfd/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-time/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-sysinfo/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-socket/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-process/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-poll/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-pipe/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-mmap/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-libdl/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-futex/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-fdtab/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-fdio/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-fd/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-eventfd/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/posix-environ/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/nolibc/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/isrlib/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/fdt/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/devfs/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib/9pfs/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/libs.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/xen/xenheaders/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/xen/xengnttab/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/xen/xenemgcons/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/xen/xencons/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/xen/xenbus/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/xen/netfront/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/xen/blkfront/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/xen/9pfront/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/drivers-xen.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/virtio/ring/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/virtio/pci/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/virtio/net/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/virtio/mmio/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/virtio/bus/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/virtio/blk/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/virtio/9p/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/drivers-virtio.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukconsole/vgacons/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukconsole/pl011/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukconsole/ns16550/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/drivers-console.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukrandom/lcpu/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/drivers-random.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukps2/kbd/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/drivers-ukps2.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukintctlr/xpic/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukintctlr/gic/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/drivers-intctlr.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukbus/platform/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukbus/pci/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/drivers-bus.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/virtio/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukrtc/pl031/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukrtc/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers/ukps2/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/drivers.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/plat/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/plat/xen/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/plat/kvm/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/plats.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/arch/arm/arm64/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/arch/arm/arm/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/arch/x86/x86_64/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/arch/Config.uk \
	/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/Config.uk

/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: \
	$(deps_config)

ifneq "$(UK_FULLVERSION)" "0.18.0~8b39d45"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(UK_CODENAME)" "Helene"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(UK_ARCH)" "x86_64"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(HOST_ARCH)" "x86_64"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(UK_BASE)" "/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(UK_APP)" "/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(UK_NAME)" "benchmark-malloc"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(KCONFIG_DIR)" "/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(KCONFIG_PLAT_BASE)" "/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/plat"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(KCONFIG_EPLAT_DIRS)" ""
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(KCONFIG_EXCLUDEDIRS)" ""
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(KCONFIG_DRIV_BASE)" "/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/drivers"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(KCONFIG_LIB_BASE)" "/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/unikraft/lib"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(KCONFIG_ELIB_DIRS)" ""
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif
ifneq "$(KCONFIG_EAPP_DIR)" "/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc"
/home/gilfoyle/Documents/unikraft-benchmark/benchmark-malloc/.unikraft/build/kconfig/auto.conf: FORCE
endif

$(deps_config): ;
