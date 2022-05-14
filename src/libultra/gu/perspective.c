#include <PR/ultratypes.h>
#include "libultra/gu/guint.h"

f32 fcos16_precise(s16 theta);
f32 fsin16_precise(s16 theta);

void guPerspectiveF(float mf[4][4], u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
	float	cot;
	int	i, j;
	int unused;

	guMtxIdentF(mf);

	//fovy *= 3.1415926f / 180.0f;
	//cot = fcos16_precise (fovy/2) / fsin16_precise (fovy/2);
	cot = fcos16_precise (fovy*91.022003173828125f) / fsin16_precise (fovy*91.022003173828125f);

	mf[0][0] = cot / aspect;
	mf[1][1] = cot;
	mf[2][2] = (near + far) / (near - far);
	mf[2][3] = -1;
	mf[3][2] = (2.0f * near * far) / (near - far);
	mf[3][3] = 0;

	for (i=0; i<4; i++)
	    for (j=0; j<4; j++)
		mf[i][j] *= scale;

	if (perspNorm != (u16 *) NULL) {
	    if (near+far<=2.0f) {
		*perspNorm = (u16) 0xFFFF;
	    } else  {
		*perspNorm = (u16) ((2.0f*65536.0f)/(near+far));
		if (*perspNorm<=0) 
		    *perspNorm = (u16) 0x0001;
	    }
	}
}

void guPerspective(Mtx *m, u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
	Matrix	mf;

	guPerspectiveF(mf, perspNorm, fovy, aspect, near, far, scale);

	guMtxF2L(mf, m);
}
