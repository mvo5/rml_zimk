#include <stdio.h>
#include <unistd.h>
#include <glib.h>

#include "zimk_userblacklist.h"


static const char *blacklist_filename = "user_blacklist.cfg";

void test_userblacklist_setup(void)
{
   FILE *f = fopen(blacklist_filename, "w");
   fputs("foo\n", f);
   fclose(f);
}

void test_userblacklist_teardown(void)
{
   unlink(blacklist_filename);
}

void test_userblacklist(void)
{
   test_userblacklist_setup();

   /* this user is in the blacklist */
   g_assert_cmpint(TRUE,
                    ==, 
                    zimk_username_in_blacklist(blacklist_filename, "foo"));
   /* and this one is not */
   g_assert_cmpint(FALSE,
                    ==, 
                    zimk_username_in_blacklist(blacklist_filename, "bar"));

   test_userblacklist_teardown();
}



int main(int argc, char** argv)
{
    g_test_init(&argc, &argv, NULL);
    // FIXME: use something like g_test_add() here
    g_test_add_func("/rlm_zimk/userblacklist", test_userblacklist);
    return g_test_run();
}
