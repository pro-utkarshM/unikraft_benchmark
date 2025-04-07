/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/* Taken from Mini-OS */

#include <stdint.h>
#include <stddef.h>
#include <uk/print.h>
#include <xen/memory.h>
#include <xen/xen.h>
#include <libfdt.h>
#include <xen/grant_table.h>
#include <common/hypervisor.h>
#include <xen-arm/os.h>
#include <xen-arm/mm.h>

extern lpae_t fixmap_pgtable[512];

/* Get Xen's suggested physical page assignments for the grant table. */
static paddr_t get_gnttab_base(void)
{
	int hypervisor;
	int len = 0;
	const uint64_t *regs;
	paddr_t gnttab_base;

	hypervisor =
		fdt_node_offset_by_compatible(HYPERVISOR_dtb, -1, "xen,xen");
	if (hypervisor < 0)
		BUG();

	regs = fdt_getprop(HYPERVISOR_dtb, hypervisor, "reg", &len);
	/* The property contains the address and size, 8-bytes each. */
	if (regs == NULL || len < 16) {
		uk_pr_debug("Bad 'reg' property: %p %d\n", regs, len);
		BUG();
	}

	gnttab_base = fdt64_ld(regs);

	uk_pr_debug("FDT suggests grant table base %llx\n",
				(unsigned long long) gnttab_base);

	return gnttab_base;
}

static paddr_t map_gnttab(paddr_t phys)
{
	uk_pr_debug("%s, phys = 0x%lx\n", __func__, phys);

	set_pgt_entry(&fixmap_pgtable[l2_pgt_idx(FIX_GNT_START)],
		      ((phys & L2_MASK) | BLOCK_DEF_ATTR | L2_BLOCK));

	return (paddr_t)(FIX_GNT_START + (phys & L2_OFFSET));
}

grant_entry_v1_t *gnttab_arch_init(int nr_grant_frames)
{
	struct xen_add_to_physmap xatp;
	paddr_t gnttab_table;
	int i;

	gnttab_table = get_gnttab_base();

	for (i = 0; i < nr_grant_frames; i++) {
		xatp.domid = DOMID_SELF;
		xatp.size = 0;     // Seems to be unused
		xatp.space = XENMAPSPACE_grant_table;
		xatp.idx = i;
		xatp.gpfn = (gnttab_table >> PAGE_SHIFT) + i;
		if (HYPERVISOR_memory_op(XENMEM_add_to_physmap, &xatp) != 0)
			BUG();
	}

	return (grant_entry_v1_t *)map_gnttab(gnttab_table);
}
