#include <PR/ultratypes.h>
#include "libultra/gu/guint.h"

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/gu/perspective/guPerspectiveF.s")

void guPerspective(Mtx *m, u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
	Matrix	mf;

	guPerspectiveF(mf, perspNorm, fovy, aspect, near, far, scale);

	guMtxF2L(mf, m);
}
