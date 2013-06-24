#include<stdlib.h>
#include<stdio.h>
#include<glib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <freeradius/ident.h>
#include <freeradius/radiusd.h>
#include <freeradius/modules.h>
#include <freeradius/conffile.h>

#include "zimk_userblacklist.h"

GTree *blacklisted_users = NULL;
time_t blacklisted_users_mtime = 0;
char *blacklisted_users_cached_filename = NULL;

static gboolean
read_blacklisted_users(const char *blacklist_file, GTree **blacklisted_users)
{
   if(*blacklisted_users)
      g_tree_destroy(*blacklisted_users);
   
   *blacklisted_users = g_tree_new_full ((GCompareDataFunc)g_strcmp0,
                                         NULL,
                                         g_free,
                                         NULL);

   char line[255];
   FILE *f = fopen(blacklist_file, "r");
   if (f == NULL)
   {
      //radlog(L_AUTH, "rlm_zimk: blacklist_file '%s' could not be opened (%s)",
      //      blacklist_file, strerror(errno));
      return FALSE;
   }

   while(fgets(line, sizeof(line), f) != NULL) {
      g_strstrip(line);
      g_tree_insert(*blacklisted_users, strdup(line), "1");
   }
   fclose(f);

   return TRUE;
}

gboolean
zimk_username_in_blacklist(const char *blacklist_file,
                           const char *username)
{
   struct stat buf;

   if (stat(blacklist_file, &buf) < 0)
   {
      // FIXME: add log that file can not be read
      fprintf(stderr, "can not open '%s'", blacklist_file);
      return FALSE;
   }

   // update tree if needed
   if (buf.st_mtime != blacklisted_users_mtime ||
       g_strcmp0(blacklist_file, blacklisted_users_cached_filename) != 0)
   {
      // (re)read blacklist
      read_blacklisted_users(blacklist_file, &blacklisted_users);

      // update the cache info
      blacklisted_users_mtime = buf.st_mtime;
      if(blacklisted_users_cached_filename)
         g_free(blacklisted_users_cached_filename);
      blacklisted_users_cached_filename = g_strdup(blacklist_file);
   }

   // lookup user from blacklist
   if(g_tree_lookup(blacklisted_users, username))
      return TRUE;

   return FALSE;
}
