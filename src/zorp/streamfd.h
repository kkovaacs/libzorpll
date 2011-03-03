/***************************************************************************
 *
 * This file is covered by a dual licence. You can choose whether you
 * want to use it according to the terms of the GNU GPL version 2, or
 * under the terms of Zorp Professional Firewall System EULA located
 * on the Zorp installation CD.
 *
 * $Id: streamfd.h,v 1.8 2003/05/14 16:40:23 bazsi Exp $
 *
 ***************************************************************************/

#ifndef ZORP_STREAMFD_H_INCLUDED
#define ZORP_STREAMFD_H_INCLUDED

#include <zorp/zorplib.h>
#include <zorp/stream.h>

#ifdef __cplusplus
extern "C" {
#endif

ZStream *z_stream_fd_new(gint fd, const gchar *name);

#ifdef __cplusplus
}
#endif

#endif
