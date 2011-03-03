#ifndef ZCV_STREAMTEE_H
#define ZCV_STREAMTEE_H

#include <zorp/stream.h>

ZStream *z_stream_tee_new(ZStream *child, ZStream *fork, GIOCondition tee_direction);

#endif
