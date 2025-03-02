#include "PR/os.h"
#include "PR/rcp.h"
#include "libultra/io/viint.h"

/**
 * PAL LAN1 Video Mode
 *
 * L = Low Resolution
 * A = Anti-Aliased
 * N = Non-Interlaced
 * 1 = 16-bit Framebuffer
 */
OSViMode osViModePalLan1 = {
    OS_VI_PAL_LAN1, // type
    {
        // comRegs
        VI_CTRL_TYPE_16 | VI_CTRL_GAMMA_DITHER_ON | VI_CTRL_GAMMA_ON | VI_CTRL_DIVOT_ON | VI_CTRL_ANTIALIAS_MODE_1 |
            VI_CTRL_PIXEL_ADV_3, // ctrl
        WIDTH(320),              // width
        BURST(58, 30, 4, 69),    // burst
        VSYNC(625),              // vSync
        HSYNC(3177, 23),         // hSync
        LEAP(3183, 3181),        // leap
        HSTART(128, 768),        // hStart
        SCALE(2, 0),             // xScale
        VCURRENT(0),             // vCurrent
    },
    { // fldRegs
      {
          // [0]
          ORIGIN(640),         // origin
          SCALE(1, 0),         // yScale
          HSTART(95, 569),     // vStart
          BURST(107, 2, 9, 0), // vBurst
          VINTR(2),            // vIntr
      },
      {
          // [1]
          ORIGIN(640),         // origin
          SCALE(1, 0),         // yScale
          HSTART(95, 569),     // vStart
          BURST(107, 2, 9, 0), // vBurst
          VINTR(2),            // vIntr
      } },
};

/**
 * MPAL LAN1 Video Mode
 *
 * L = Low Resolution
 * A = Anti-Aliased
 * N = Non-Interlaced
 * 1 = 16-bit Framebuffer
 */
OSViMode osViModeMpalLan1 = {
    OS_VI_MPAL_LAN1, // type
    {
        // comRegs
        VI_CTRL_TYPE_16 | VI_CTRL_GAMMA_DITHER_ON | VI_CTRL_GAMMA_ON | VI_CTRL_DIVOT_ON | VI_CTRL_ANTIALIAS_MODE_1 |
            VI_CTRL_PIXEL_ADV_3, // ctrl
        WIDTH(320),              // width
        BURST(57, 30, 5, 70),    // burst
        VSYNC(525),              // vSync
        HSYNC(3089, 4),          // hSync
        LEAP(3097, 3098),        // leap
        HSTART(108, 748),        // hStart
        SCALE(2, 0),             // xScale
        VCURRENT(0),             // vCurrent
    },
    { // fldRegs
      {
          // [0]
          ORIGIN(640),        // origin
          SCALE(1, 0),        // yScale
          HSTART(37, 511),    // vStart
          BURST(4, 2, 14, 0), // vBurst
          VINTR(2),           // vIntr
      },
      {
          // [1]
          ORIGIN(640),        // origin
          SCALE(1, 0),        // yScale
          HSTART(37, 511),    // vStart
          BURST(4, 2, 14, 0), // vBurst
          VINTR(2),           // vIntr
      } },
};

/**
 * NTSC LAN1 Video Mode
 *
 * L = Low Resolution
 * A = Anti-Aliased
 * N = Non-Interlaced
 * 1 = 16-bit Framebuffer
 */
OSViMode osViModeNtscLan1 = {
    OS_VI_NTSC_LAN1, // type
    {
        // comRegs
        VI_CTRL_TYPE_16 | VI_CTRL_GAMMA_DITHER_ON | VI_CTRL_GAMMA_ON | VI_CTRL_DIVOT_ON | VI_CTRL_ANTIALIAS_MODE_1 |
            VI_CTRL_PIXEL_ADV_3, // ctrl
        WIDTH(320),              // width
        BURST(57, 34, 5, 62),    // burst
        VSYNC(525),              // vSync
        HSYNC(3093, 0),          // hSync
        LEAP(3093, 3093),        // leap
        HSTART(108, 748),        // hStart
        SCALE(2, 0),             // xScale
        VCURRENT(0),             // vCurrent
    },
    { // fldRegs
      {
          // [0]
          ORIGIN(640),        // origin
          SCALE(1, 0),        // yScale
          HSTART(37, 511),    // vStart
          BURST(4, 2, 14, 0), // vBurst
          VINTR(2),           // vIntr
      },
      {
          // [1]
          ORIGIN(640),        // origin
          SCALE(1, 0),        // yScale
          HSTART(37, 511),    // vStart
          BURST(4, 2, 14, 0), // vBurst
          VINTR(2),           // vIntr
      } },
};
