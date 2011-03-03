/***************************************************************************
 *
 * COPYRIGHTHERE
 *
 * $Id: streamfd.h,v 1.8 2003/05/14 16:40:23 bazsi Exp $
 *
 ***************************************************************************/

#ifndef ZORP_STREAMBLOB_H
#define ZORP_STREAMBLOB_H 1

#include <zorp/blob.h>

#include <zorp/stream.h>

#ifdef __cplusplus
extern "C" {
#endif

ZStream *z_stream_blob_new(ZBlob *blob, gchar *name);

#ifdef __cplusplus
}
#endif

#endif
