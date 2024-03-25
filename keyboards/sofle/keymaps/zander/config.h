#pragma once

/* The way how "handedness" is decided (which half is which),
see https://docs.qmk.fm/#/feature_split_keyboard?id=setting-handedness
for more options.
*/
//#define NO_DEBUG
//#define NO_PRINT
#define NO_ACTION_ONESHOT
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_A, USER_SYNC_B, USER_SYNC_C
#define SPLIT_HAND_MATRIX_GRID B3, B5
#define SPLIT_WATCHDOG_ENABLE

