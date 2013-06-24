#include <stdio.h>
#include <unistd.h>
#include <glib.h>

#include "zimk_userblacklist.h"


long get_memory_usage(void)
{
   // getrusage() seems to not work properly, so we use statm (see man(8) proc)
   FILE *f = fopen("/proc/self/statm", "r");
   unsigned long size, tmp;
   // get the VmSize
   if(!fscanf(f, "%ld %ld %ld %ld %ld %ld %ld %ld",
              &size, &tmp, &tmp, &tmp, &tmp, &tmp, &tmp, &tmp))
      return -1;
   fclose(f);
   return size;
}

void test_userblacklist_setup(const char *blacklist_filename)
{
   int i;
   char s[255];

   FILE *f = fopen(blacklist_filename, "w");
   for (i=0;i<1000;i++) {
      snprintf(s, sizeof(s)-1, "foo-%i\n", i);
      fputs(s, f);
   }
   fclose(f);
}

void test_userblacklist_teardown(const char *blacklist_filename)
{
   unlink(blacklist_filename);
}

void test_userblacklist(void)
{
   int i;
   long mem_size = 0;
   char blacklist_filename[255];

   // run in a loop for memory size debugging
   for(i=0;i<100;i++)
   {
      //printf("%ld\n", get_memory_usage());

      // we need a new filename to force re-loading the blacklist
      snprintf(blacklist_filename, sizeof(blacklist_filename),
               "user_blacklist-%i.cfg", i);
      
      test_userblacklist_setup(blacklist_filename);

      /* this user is in the blacklist */
      g_assert_cmpint(TRUE,
                      ==, 
                      zimk_username_in_blacklist(blacklist_filename, "foo-0"));
      /* and this one is not */
      g_assert_cmpint(FALSE,
                      ==, 
                      zimk_username_in_blacklist(blacklist_filename, "bar"));
      
      test_userblacklist_teardown(blacklist_filename);

      // store the mem size after the first successfull run
      if (mem_size == 0)
         mem_size = get_memory_usage();
   }
   // ensure the mem size is not growing
   g_assert_cmpint(mem_size, ==, get_memory_usage());

}



int main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    // FIXME: use something like g_test_add() here
    g_test_add_func("/rlm_zimk/userblacklist", test_userblacklist);
    return g_test_run();
}
