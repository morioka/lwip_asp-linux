/* This file is generated from target_rename.def by genrename. */

/* This file is included only when target_rename.h has been included. */
#ifdef TOPPERS_TARGET_RENAME_H
#undef TOPPERS_TARGET_RENAME_H


/*
 *  target_config.c
 */
#undef hew_io_sim_putc
#undef hew_io_sim_snd_chr
#undef hew_io_sim_rcv_chr
#undef target_initialize
#undef target_exit

#ifdef TOPPERS_LABEL_ASM


/*
 *  target_config.c
 */
#undef _hew_io_sim_putc
#undef _hew_io_sim_snd_chr
#undef _hew_io_sim_rcv_chr
#undef _target_initialize
#undef _target_exit

#endif /* TOPPERS_LABEL_ASM */

#include "sh12a_gcc/prc_unrename.h"

#endif /* TOPPERS_TARGET_RENAME_H */
