#include "PR/gbi.h"
#include "PR/mbi.h"
#include "PR/os.h"
#include "sys/rsp_segment.h"

/**
 * Set the offset for the given address, so the RSP can get the correct physical address.
 * This is used for the colour and depth buffers.
 * Official name: segSetBase
 */
void rsp_segment(Gfx **gdl, u32 segment, void *base) {
	gSPSegment((*gdl)++, segment, OS_K0_TO_PHYSICAL(base));
}
