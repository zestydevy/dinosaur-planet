// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include "libnaudio/n_synthInternals.h"
#include "libnaudio/n_unkfuncs.h"

f32 mp3_func_80077900(f32 arg0, f32 arg1) {
	s32 arg1int = arg1;
	f32 sp30;
	register s32 s0;

	if (arg0 != 0 && arg1 == 0) {
		return 1;
	}

	if (arg0 == 0) {
		return 0;
	}

	if ((arg0 == 0 && arg1 == 0) || (arg0 < 0 && arg1int != arg1)) {
		return 0;
	}

	if (arg1int != arg1) {
		arg0 = mp3_func_8007BEB0(mp3_func_8007C080(arg0) * arg1);
	} else if (arg1 > 0) {
		arg1--;
		sp30 = arg0;

		for (s0 = (arg1-- != 0); s0; s0 = (arg1-- != 0)) {
			arg0 *= sp30;
		}
	} else {
		sp30 = 1;

		for (s0 = (arg1++ != 0); s0; s0 = (arg1++ != 0)) {
			sp30 /= arg0;
		}

		arg0 = sp30;
	}

	return arg0;
}
