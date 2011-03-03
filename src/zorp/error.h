/***************************************************************************
 *
 * This file is covered by a dual licence. You can choose whether you
 * want to use it according to the terms of the GNU GPL version 2, or
 * under the terms of Zorp Professional Firewall System EULA located
 * on the Zorp installation CD.
 *
 * $Id: error.h,v 1.4 2004/05/21 13:58:32 abi Exp $
 *
 ***************************************************************************/

#ifndef _ZORP_ERROR_H_INCLUDED
#define _ZORP_ERROR_H_INCLUDED

#include <zorp/zorplib.h>

#ifdef G_OS_WIN32
#  include <winsock2.h>
#  define ETIMEDOUT WSAETIMEDOUT
#  define EINPROGRESS WSAEINPROGRESS
#  define ENOTCONN WSAENOTCONN 
#  define ENETUNREACH WSAENETUNREACH
#  define EADDRINUSE WSAEADDRINUSE
#  define ENOTSOCK WSAENOTSOCK
#endif

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

gboolean z_errno_is(int e);
int z_errno_get(void);
void z_errno_set(int e);

#ifdef __cplusplus
}
#endif

#endif
