/***************************************************************************
 *
 * This file is covered by a dual licence. You can choose whether you
 * want to use it according to the terms of the GNU GPL version 2, or
 * under the terms of Zorp Professional Firewall System EULA located
 * on the Zorp installation CD.
 *
 * $Id: registry.c,v 1.11 2004/08/18 11:46:46 bazsi Exp $
 *
 * Author  : Bazsi
 * Auditor :
 * Last audited version:
 * Notes:
 *
 ***************************************************************************/

#include <zorp/registry.h>
#include <zorp/log.h>

#include <glib.h>
#include <string.h>

#ifdef G_OS_WIN32
  #include <windows.h>
  #include <winreg.h>
#endif

static GHashTable *registry[MAX_REGISTRY_TYPE];

#define MAX_REGISTRY_NAME 32

/**
 * ZRegistryEntry is an entry in the registry hash.
 *
 * An entry in the registry. The registry is a hash table indexed by a
 * string and storing an opaque pointer and an integer type
 * value. 
 */
typedef struct _ZRegistryEntry
{
  gint type;
  gchar name[MAX_REGISTRY_NAME];
  gpointer value;
} ZRegistryEntry;

/**
 * Initialize the registry.
 **/
void
z_registry_init(void)
{
  int i;
  for (i = 0; i < MAX_REGISTRY_TYPE; i++)
    registry[i] = g_hash_table_new(g_str_hash, g_str_equal);
}


/**
 * Deinitialize the registry subsystem.
 **/
void
z_registry_destroy(void)
{
  int i;
  for (i = 0; i<MAX_REGISTRY_TYPE; i++)
    g_hash_table_destroy(registry[i]);
}

/**
 * Add an entry to the registry with the given name and type.
 *
 * @param[in] name the key of this entry
 * @param[in] type the type of this entry
 * @param[in] value the pointer associated with this entry
 **/
void
z_registry_add(const gchar *name, gint type, gpointer value)
{
  ZRegistryEntry *ze = g_new0(ZRegistryEntry, 1);
  
  if (type < 0 || type > MAX_REGISTRY_TYPE)
    {
      /*LOG
        This message indicates that an internal error occurred,
        a buggy/incompatible loadable module wanted to register
        an unsupported module type. Please report this event to the 
	 Balabit QA Team (devel@balabit.com).
       */
      z_log(NULL, CORE_ERROR, 0, "Internal error, bad registry type; name='%s', type='%d'", name, type);
      return;
    }
  g_strlcpy(ze->name, name, sizeof(ze->name));
  ze->value = value;
  ze->type = type;
  g_hash_table_insert(registry[type], ze->name, ze);
}

/**
 * Fetch an item from the registry with the given name returning its
 * type and pointer.
 *
 * @param[in] name name of the entry to fetch
 * @param[in] type type of the entry
 *
 * @returns NULL if the entry was not found, the associated pointer otherwise
 **/
ZRegistryEntry *
z_registry_get_one(const gchar *name, gint type)
{
  ZRegistryEntry *ze = NULL;
  
  z_enter();
  ze = g_hash_table_lookup(registry[type], name);
  z_return(ze);
}

/**
 * This function returns an entry from the registry autoprobing for
 * different types.
 *
 * @param[in]      name name of the entry to fetch
 * @param[in, out] type contains the preferred entry type on input, contains the real type on output
 *
 * If type is NULL or the value pointed by type is 0,
 * then each possible entry type is checked, otherwise only the value
 * specified will be used.
 *
 * @returns the value stored in the registry or NULL if nothing is found
 **/
gpointer
z_registry_get(const gchar *name, gint *type)
{
  int i;
  ZRegistryEntry *ze = NULL;
  
  z_enter();
  if (type && (*type > MAX_REGISTRY_TYPE || *type < 0))
    z_return(NULL);

  if (type == NULL || *type == 0)
    {
      for (i = 0; i < MAX_REGISTRY_TYPE && ze == NULL; i++)
        {
          ze = z_registry_get_one(name, i);
        }
    }
  else
    {
      ze = z_registry_get_one(name, *type);
    }
    
  if (ze)
    {
      if (type) 
        *type = ze->type;
      z_return(ze->value);
    }
  z_return(NULL);
}

/**
 * This function checks whether the given name is found in the registry at
 * all, and returns the accompanying type value if found.
 *
 * @param[in] name name to search for
 *
 * @return the type value
 **/
guint
z_registry_has_key(const gchar *name)
{
  int i;
  ZRegistryEntry *ze = NULL;
  
  for (i = 0; (i < MAX_REGISTRY_TYPE) && (ze == NULL); i++)
    {
      ze = z_registry_get_one(name, i);
    }
  if (ze)
    {
      return i;
    }
  else
    {
      return 0;
    }
}

/**
 * This function iterates over the set of registry entries having the
 * type type.
 *
 * @param[in] type type of entries to iterate over
 * @param[in] func function to call for elements
 * @param[in] user_data pointer to be passed to func
 **/
void
z_registry_foreach(gint type, GHFunc func, gpointer user_data)
{
  g_hash_table_foreach(registry[type], func, user_data); 
}

#ifdef G_OS_WIN32

/**
 * This function stores data in the value field of an registry key.
 * 
 * @param[in] root The key that is opened or created by this function is a subkey of the key that is identified by this parameter.
 * @param[in] key Pointer to a null-terminated string that specifies the name of a subkey that this function opens or creates.
 * This is a subkey of the key that is identified by <TT>root</TT>.
 * @param[in] name Pointer to a string containing the name of the value to set.
 * @param[in] value The data to be stored with the specified value name.
*/
gboolean
z_reg_key_write_dword(HKEY root, gchar *key, gchar *name, DWORD value)
{
  HKEY hKey;

  if(RegCreateKeyEx(root, (LPCSTR)key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    {
      return FALSE;
    }

  if(RegSetValueEx(hKey, (LPCSTR)name, 0, REG_DWORD, (LPBYTE)&value, sizeof(DWORD)) == ERROR_SUCCESS)
    {
      RegFlushKey(hKey);
      RegCloseKey(hKey);
      return TRUE;
    }

  RegCloseKey(hKey);
  return FALSE;
}

/**
 * This function stores data in the value field of an registry key.
 * 
 * @param[in] root The key that is opened or created by this function is a subkey of the key that is identified by this parameter.
 * @param[in] key Pointer to a null-terminated string that specifies the name of a subkey that this function opens or creates.
 * This is a subkey of the key that is identified by <TT>root</TT>.
 * @param[in] name Pointer to a string containing the name of the value to set.
 * @param[in] value Pointer to a buffer that contains the data to be stored with the specified value name.
*/
gboolean
z_reg_key_write_string(HKEY root, gchar *key, gchar *name, gchar *value)
{
  HKEY hKey;

  if(RegCreateKeyEx(root, (LPCSTR)key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    return FALSE;

  if(RegSetValueEx(hKey, (LPCSTR)name, 0, REG_SZ, (LPBYTE)value, strlen(value)+1) == ERROR_SUCCESS)
    {
      RegFlushKey(hKey);
      RegCloseKey(hKey);
      return TRUE;
    }

  RegCloseKey(hKey);
  return FALSE;
}

/**
 * This function stores data in the value field of an registry key.
 * 
 * @param[in] root The key that is removed by this function is a subkey of the key that is identified by this parameter.
 * @param[in] key Pointer to a null-terminated string that specifies the name of a subkey that this function removes.
 * This is a subkey of the key that is identified by <TT>root</TT>.
 * @param[in] name Pointer to a null-terminated string that names the value to remove.
*/
gboolean
z_reg_key_delete(HKEY root, gchar *key, gchar *name)
{
  HKEY hKey;

  if(RegOpenKeyEx(root,(LPCSTR)key, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
    return FALSE;

  if(RegDeleteValue(hKey, (LPCSTR)name) != ERROR_SUCCESS)
    {
      RegFlushKey(hKey);
      RegCloseKey(hKey);
      return TRUE;
    }

  RegCloseKey(hKey);
  return FALSE;
}

/**
 * This function retrieves the data for a specified value associated with a specified registry key.
 * 
 * @param[in] root The key that is opened or created by this function is a subkey of the key that is identified by this parameter.
 * @param[in] key Pointer to a null-terminated string that specifies the name of a subkey that this function opens or creates.
 * This is a subkey of the key that is identified by <TT>root</TT>.
 * @param[in] name Pointer to a string containing the name of the value to set.
 * @param[in, out] value Pointer to a buffer that receives value data.
*/
gboolean
z_reg_key_read_dword(HKEY root, gchar *key, gchar *name, DWORD *value)
{
  HKEY hKey;
  DWORD t = sizeof(DWORD);
  DWORD type = REG_DWORD;

  if(RegOpenKeyEx(root,(LPCSTR)key, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return FALSE;

  if(RegQueryValueEx(hKey, (LPCSTR)name, 0, &type, (LPBYTE)value, &t) == ERROR_SUCCESS)
    {
      RegCloseKey(hKey);
      return TRUE;
    }
  RegCloseKey(hKey);
  return FALSE;
}

/**
 * This function retrieves the data for a specified value associated with a specified registry key.
 * 
 * @param[in] root The key that is opened or created by this function is a subkey of the key that is identified by this parameter.
 * @param[in] key Pointer to a null-terminated string that specifies the name of a subkey that this function opens or creates.
 * This is a subkey of the key that is identified by <TT>root</TT>.
 * @param[in] name Pointer to a string containing the name of the value to set.
 * @param[in, out] value Pointer to a buffer that receives value data.
*/
gboolean
z_reg_key_read_string(HKEY root, gchar *key, gchar *name, gchar **value)
{
  HKEY hKey;
  gchar temp[2000];
  DWORD t = 2000;
  DWORD type = REG_SZ;

  if(RegOpenKeyEx(root,(LPCSTR)key, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return FALSE;

  if(RegQueryValueEx(hKey, (LPCSTR)name, 0, &type, (LPBYTE)&temp, &t) == ERROR_SUCCESS)
    {
      *value = g_strdup(temp);
      RegCloseKey(hKey);
      return TRUE;
    }

  RegCloseKey(hKey);
  return FALSE;
}

gboolean
z_sid_to_text( PSID ps, char *buf, int bufSize )
{
  PSID_IDENTIFIER_AUTHORITY psia;
  DWORD dwSubAuthorities;
  DWORD dwSidRev = SID_REVISION;
  DWORD i;
  int n, size;
  char *p;

  if ( ! IsValidSid( ps ) )
    return FALSE;

  psia = GetSidIdentifierAuthority( ps );

  dwSubAuthorities = *GetSidSubAuthorityCount( ps );

  size = 15 + 12 + ( 12 * dwSubAuthorities ) + 1;

  if ( bufSize < size )
    {
      SetLastError( ERROR_INSUFFICIENT_BUFFER );
      return FALSE;
    }

  size = wsprintf( buf, "S-%lu-", dwSidRev );
  p = buf + size;

  if ( psia->Value[0] != 0 || psia->Value[1] != 0 )
    {
      n = wsprintf( p, "0x%02hx%02hx%02hx%02hx%02hx%02hx",
                    (USHORT) psia->Value[0], (USHORT) psia->Value[1],
                    (USHORT) psia->Value[2], (USHORT) psia->Value[3],
                    (USHORT) psia->Value[4], (USHORT) psia->Value[5] );
      size += n;
      p += n;
    }
  else
    {
      n = wsprintf( p, "%lu", ( (ULONG) psia->Value[5] ) +
                    ( (ULONG) psia->Value[4] << 8 ) + ( (ULONG) psia->Value[3] << 16 ) +
                    ( (ULONG) psia->Value[2] << 24 ) );
      size += n;
      p += n;
    }

  for ( i = 0; i < dwSubAuthorities; ++ i )
    {
      n = wsprintf( p, "-%lu", *GetSidSubAuthority( ps, i ) );
      size += n;
      p += n;
    }

  return TRUE;
}

#endif
