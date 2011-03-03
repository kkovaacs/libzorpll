/***************************************************************************
 *
 * This file is covered by a dual licence. You can choose whether you
 * want to use it according to the terms of the GNU GPL version 2, or
 * under the terms of Zorp Professional Firewall System EULA located
 * on the Zorp installation CD.
 *
 * $Id: zorplib.h,v 1.10 2003/04/08 13:32:29 sasa Exp $
 *
 ***************************************************************************/
#ifndef ZORP_STACKDUMP_H_INCLUDED
#define ZORP_STACKDUMP_H_INCLUDED

#include <zorp/zorplib.h>

#ifdef __cplusplus
extern "C" {
#endif

#if ZORPLIB_ENABLE_STACKDUMP
typedef struct sigcontext ZSignalContext;
#else
typedef void ZSignalContext;
#endif

/**
 * Return the current signal context used for stack dumping.
 * The variable passed as parameter must be declared in a signal handler.
 **/
#define z_stackdump_get_context(p) \
  (ZSignalContext *) (((char *) &p) + 16)

void z_stackdump_log(ZSignalContext *context);
#ifdef G_OS_WIN32
void z_enable_write_dump_file();
#endif
#ifdef __cplusplus
}
#endif

#endif
