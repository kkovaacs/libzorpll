/***************************************************************************
 *
 * This file is covered by a dual licence. You can choose whether you
 * want to use it according to the terms of the GNU GPL version 2, or
 * under the terms of Zorp Professional Firewall System EULA located
 * on the Zorp installation CD.
 *
 * $Id: thread.h,v 1.8 2003/07/02 14:21:06 bazsi Exp $
 *
 ***************************************************************************/

#ifndef ZORP_THREAD_H_INCLUDED
#define ZORP_THREAD_H_INCLUDED

#include <zorp/zorplib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_THREAD_NAME 128

/**
 * ZThread encapsulates a simple thread, and adds some meta information
 * like thread name, thread id etc.
 **/
typedef struct _ZThread
{
  GThread *thread;
  gint thread_id;
  gchar name[MAX_THREAD_NAME];
  GThreadFunc func;
  gpointer arg;
} ZThread;

void z_thread_register_start_callback(GFunc func, gpointer user_data);
void z_thread_register_stop_callback(GFunc func, gpointer user_data);

gboolean z_thread_new(gchar *name, GThreadFunc func, gpointer arg);

void z_thread_enable_threadpools(gint idle);
void z_thread_set_max_threads(gint max);
void z_thread_set_max_stack_size(gint stack_size);

void z_thread_init(void);
void z_thread_destroy(void);
ZThread *z_thread_self(void);

#ifdef __cplusplus
}
#endif

#endif

