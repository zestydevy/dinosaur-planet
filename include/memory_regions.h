/** Static memory regions
 */
#ifndef _MEMORY_REGIONS_H
#define _MEMORY_REGIONS_H

#include "PR/ultratypes.h"
#include "constants.h"

/** Note: These should be a multiple of 64-bits so the framebuffer addresses are aligned. */
#define MEMORY_POOL_0_SIZE 0x3d4000
#define MEMORY_POOL_1_SIZE 0x1e7000

#define MEMORY_POOL_0_START (EXPANSION_RAM_END - MEMORY_POOL_0_SIZE)
#define MEMORY_POOL_1_START (MEMORY_POOL_0_START - MEMORY_POOL_1_SIZE)

/**
 * Framebuffer region must be large enough to hold two 640x480 color buffers.
 *
 * When running at 240p, this region will also hold three 320x240 color buffers
 * (double buffered + pause screenshot framebuffer) and a single 320x240 depth buffer.
 * Note: For PAL support, the game *always* allocates each color buffer with a size 
 * of 320x260, so technically this region needs to hold three buffers of that size.
 */
#define FRAMEBUFFER_REGION_SIZE (640 * 480 * sizeof(u16) * 2)

/** Placed at end of RAM. */
#define COLOR_BUFFERS_ADDR_NO_EXP_PAK (NO_EXPANSION_RAM_END - FRAMEBUFFER_REGION_SIZE)
/** Placed right before memory pool 1. */
#define COLOR_BUFFERS_ADDR_EXP_PAK    (MEMORY_POOL_1_START - FRAMEBUFFER_REGION_SIZE)
/** 
 * Placed within the framebuffer region but in a different 1MB bank than the color buffers 
 * for performance. Care must be taken to ensure this does not overlap with the color
 * buffers or the memory pools.
 */
#define DEPTH_BUFFER_ADDR 0x80200000

#endif
