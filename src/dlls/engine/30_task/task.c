#include "PR/ultratypes.h"
#include "dll.h"
#include "sys/main.h"
#include "variables.h"
#include "functions.h"

static u8 sRecentlyCompleted[5];
static u8 sCompletionIdx;
static s8 sRecentlyCompletedNextIdx;

void task_ctor(void *self) {

}

void task_dtor(void *self) {

}

void task_load_recently_completed() {
    s32 i;
    u8 val;

    for (i = 0; i != 5; i++) {
        val = get_gplay_bitstring(311 + i);
        sRecentlyCompleted[i] = val;

        if (val != 0) {
            sRecentlyCompletedNextIdx = i;
        }
    }

    val = get_gplay_bitstring(316);
    sCompletionIdx = val;
    if (val == 0) {
        sCompletionIdx = 1;
        sRecentlyCompletedNextIdx = -1;
    }
}

void task_mark_task_completed(u8 task) {
    s16 i;
    s16 bs_entry;
    s16 bit_idx;
    u32 bs_value;
    s16 bs_entry2;

    // Bail if already recently completed
    for (i = 0; i < 5; i++) {
        if (task == sRecentlyCompleted[i]) {
            return;
        }
    }

    // Add task to recently completed list
    if (sRecentlyCompletedNextIdx != 4) {
        // Append if there's room
        sRecentlyCompletedNextIdx++;
        sRecentlyCompleted[sRecentlyCompletedNextIdx] = task;

        set_gplay_bitstring(sRecentlyCompletedNextIdx + 311, task);
    } else {
        // Otherwise, shift everything down and add to the end
        for (i = 0; i < 4; i++) {
            sRecentlyCompleted[i] = sRecentlyCompleted[i + 1];
        }

        sRecentlyCompleted[4] = task;

        for (i = 0; i < 5; i++) {
            set_gplay_bitstring(311 + i, sRecentlyCompleted[i]);
        }
    }

    // Set bit for task in bitstring
    //
    // This is a 256-bit bitstring from gplay bitstring entry 303 to 315 (8 entries)
    bs_entry = (task / 32) + 303;

    bs_value = get_gplay_bitstring(bs_entry);
    bit_idx = task % 32;
    bs_value = (1 << (bit_idx)) | bs_value;

    set_gplay_bitstring(bs_entry, bs_value);

    // Determine new completion index
    if (sCompletionIdx == task) {
        do {
            sCompletionIdx++;

            bs_entry2 = (sCompletionIdx / 32) + 303;
            if (bs_entry2 != bs_entry) {
                bs_entry = bs_entry2;

                bs_value = get_gplay_bitstring(bs_entry2);
            }
            bit_idx = sCompletionIdx % 32;

        } while ((bs_value >> bit_idx) & 1);

        set_gplay_bitstring(316, sCompletionIdx);
    }

    // hmm
    if (!task) {
        gDLL_29_gplay->exports->func_958(NULL, 0, 1, func_80048498());
    }
}

u8 task_get_num_recently_completed() {
    return sRecentlyCompletedNextIdx + 1;
}

char *task_get_recently_completed_task_text(u8 idx) {
    return gDLL_21_gametext->exports->get_text(sRecentlyCompleted[idx] + 244, 0);
}

char *task_get_completion_task_text() {
    return gDLL_21_gametext->exports->get_text(sCompletionIdx + 244, 1);
}

s16 task_get_completion_percentage() {
    f32 tmp = sCompletionIdx / 206.0f;
    return tmp * 100.0f;
}
