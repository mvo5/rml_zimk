/*
 * rlm_zimk.c
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 * Copyright 2000,2006  The FreeRADIUS server project
 * Copyright 2013 Michael Vogt <michael.vogt@uni-trier.de>
 */

#include<glib.h>

#include <freeradius/ident.h>
#include <freeradius/radiusd.h>
#include <freeradius/modules.h>
#include <freeradius/conffile.h>

#include "zimk_userblacklist.h"

/*
 *	Define a structure for our module configuration.
 *
 *	These variables do not need to be in a structure, but it's
 *	a lot cleaner to do so, and a pointer to the structure can
 *	be used as the instance handle.
 */
typedef struct rlm_zimk_t {
	const char	*blacklist_file;
} rlm_zimk_t;

static const CONF_PARSER module_config[] = {
        { "blacklist_file", PW_TYPE_STRING_PTR,
          offsetof(rlm_zimk_t, blacklist_file), NULL, NULL },

        { NULL, -1, 0, NULL, NULL }
};


/*
 *	Do any per-module initialization that is separate to each
 *	configured instance of the module.  e.g. set up connections
 *	to external databases, read configuration files, set up
 *	dictionary entries, etc.
 *
 *	If configuration information is given in the config section
 *	that must be referenced in later calls, store a handle to it
 *	in *instance otherwise put a null pointer there.
 */
static int zimk_instantiate(CONF_SECTION *conf, void **instance)
{
	rlm_zimk_t *data;

	data = rad_malloc(sizeof(*data));
	if (!data) {
		return -1;
	}
	memset(data, 0, sizeof(*data));

	/*
	 *	If the configuration parameters can't be parsed, then
	 *	fail.
	 */
	if (cf_section_parse(conf, data, module_config) < 0) {
		free(data);
		return -1;
	}

	*instance = data;

	return 0;
}

/*
 *	Authenticate the user with the given password.
 */
static int zimk_authenticate(void *instance, REQUEST *request)
{
	/* quiet the compiler */
	instance = instance;

        /* do the actual check */
        const char *blacklist_file = "/etc/zimk_user_blacklist.conf";
        const char* const username = request->username->vp_strvalue;
        if (zimk_username_in_blacklist(blacklist_file, username)) {
           radlog(L_AUTH, "rlm_zimk: user in blacklist: %s", username);
           return RLM_MODULE_REJECT;
        }

	return RLM_MODULE_OK;
}

/*
 *	Only free memory we allocated.  The strings allocated via
 *	cf_section_parse() do not need to be freed.
 */
static int zimk_detach(void *instance)
{
	free(instance);
	return 0;
}

/*
 *	The module name should be the only globally exported symbol.
 *	That is, everything else should be 'static'.
 *
 *	If the module needs to temporarily modify it's instantiation
 *	data, the type should be changed to RLM_TYPE_THREAD_UNSAFE.
 *	The server will then take care of ensuring that the module
 *	is single-threaded.
 */
module_t rlm_zimk = {
	RLM_MODULE_INIT,
	"zimk",
	RLM_TYPE_THREAD_SAFE,		/* type */
	zimk_instantiate,		/* instantiation */
	zimk_detach,			/* detach */
	{
	  zimk_authenticate,	/* authentication */
	  NULL,         	/* authorization */
	  NULL,  	        /* preaccounting */
	  NULL,         	/* accounting */
	  NULL,          	/* checksimul */
	  NULL,			/* pre-proxy */
	  NULL,			/* post-proxy */
	  NULL			/* post-auth */
	},
};
