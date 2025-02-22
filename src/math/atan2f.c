// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>
#include <PR/gu.h>
#include "sys/math.h"

f32 atan2f(f32 x, f32 z) {
	f32 result;
	f32 len;
    
	result = 0;

	if (x == 0) {
		if (z >= 0) {
			result = 0;
		} else {
			result = DEG_TO_RAD(180);
		}
	} else if (z == 0) {
		if (x > 0) {
			result = DEG_TO_RAD(90);
		} else {
			result = DEG_TO_RAD(270);
		}
	} else {
		len = sqrtf(x * x + z * z);

		if (x > z) {
			result = acosf(z / len);

			if (x < 0) {
				result = DEG_TO_RAD(360) - result;
			}
		} else {
			result = DEG_TO_RAD(90) - acosf(x / len);

			if (z < 0) {
				result = DEG_TO_RAD(180) - result;
			}

			if (result < 0) {
				result += DEG_TO_RAD(360);
			}
		}
	}

	return result;
}
