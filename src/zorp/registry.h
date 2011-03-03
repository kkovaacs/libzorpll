/***************************************************************************
 *
 * This file is covered by a dual licence. You can choose whether you
 * want to use it according to the terms of the GNU GPL version 2, or
 * under the terms of Zorp Professional Firewall System EULA located
 * on the Zorp installation CD.
 *
 * $Id: registry.h,v 1.10 2003/04/08 13:32:29 sasa Exp $
 *
 ***************************************************************************/

#ifndef ZORP_REGISTRY_H_INCLUDED
#define ZORP_REGISTRY_H_INCLUDED

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PROXY_NAME 32

#define ZR_NONE      0
#define ZR_PROXY     1
#define ZR_PYPROXY   2
/* deprecated
#define ZR_DPROXY    3
*/
#define ZR_CONNTRACK 4
#define ZR_OTHER     5
#define ZR_MODULE    6
#define MAX_REGISTRY_TYPE 16

void z_registry_init(void);
void z_registry_destroy(void);
void z_registry_add(const gchar *name, gint type, gpointer value);
gpointer z_registry_get(const gchar *name, gint *type);
guint z_registry_has_key(const gchar *name);
void z_registry_foreach(gint type, GHFunc func, gpointer user_data);

#ifdef __cplusplus
}
#endif

#ifdef G_OS_WIN32

#include <windows.h>
#include <winreg.h>

gboolean z_reg_key_write_dword(HKEY root, gchar *key, gchar *name, DWORD value);
gboolean z_reg_key_write_string(HKEY root, gchar *key, gchar *name, gchar *value);

gboolean z_reg_key_read_dword(HKEY root, gchar *key, gchar *name, DWORD *value);
gboolean z_reg_key_read_string(HKEY root, gchar *key, gchar *name, gchar **value);
gboolean z_reg_key_delete(HKEY root, gchar *key, gchar *name);
gboolean z_sid_to_text( PSID ps, char *buf, int bufSize );
#endif

#endif
