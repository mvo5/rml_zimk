#include<stdio.h>

#include <freeradius/ident.h>
#include <freeradius/radiusd.h>
#include <freeradius/modules.h>
#include <freeradius/conffile.h>


#include "libmeep/meep.h"
#include "zimk_userblacklist.h"


int zimk_username_in_blacklist(const char *blacklist_file,
                               const char *username)
{
   char line[255];
   FILE *f = fopen(blacklist_file, "r");
   if (f == NULL)
   {
      //radlog(L_AUTH, "rlm_zimk: blacklist_file '%s' could not be opened (%s)",
      //      blacklist_file, strerror(errno));
      return FALSE;
   }

   while(fgets(line, sizeof(line), f) != NULL) {
      str_rstrip(line);
      if(strcmp(line, username) == 0)
         return TRUE;
   }

   return FALSE;
}
