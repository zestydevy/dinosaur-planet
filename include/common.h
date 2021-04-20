#ifndef _COMMON_H
#define _COMMON_H

#include "ultra64.h"
#include "common_structs.h"
#include "functions.h"
#include "variables.h"
#include "constants.h"

// XXX: These macros are ordinarily defined by <gbi.h> when using #define F3DEX_GBI_2x.
// However, our version of gbi.h fails if F3DEX_GBI_2x is defined.
// The following macros are used as a workaround.

#define rare_G_MV_VIEWPORT  8

#define rare_gMoveWd(pkt, index, offset, data)  \
    gDma1p((pkt), G_MOVEWORD, data, offset, index)
#define rare_gSPPerspNormalize(pkt, s)   \
    rare_gMoveWd(pkt, G_MW_PERSPNORM, 0, (s))
#define rare_gSPMatrix(pkt, m, p)  \
    gDma2p((pkt),G_MTX,(m),sizeof(Mtx),(p)^G_MTX_PUSH,0)
// XXX: this seems to be unique to Dinosaur Planet...
#define rare_gSPViewport(pkt,v) gDma1p(pkt,G_MOVEMEM,(v),sizeof(Vp)/2 /* 8 why????? */,rare_G_MV_VIEWPORT)

#endif
