ZIMK dynamic radius blacklist
=============================

Add usernames to /etc/zimk_user_blacklist.conf and they will
be dynamically blacklisted from login.

E.g.
```
$ cat /etc/zimk_user_blacklist.conf
foo
bar
```

To install
----------

Copy the module in place:
```
# install -m 644 -u root -g root rlm_zimk.so /usr/lib/freeradius/
```

Add options for the zimk module:
```
# cat << EOF > /etc/freeradius/modules/zimk
zimk {
  blacklist_file = "/etc/zimk_user_blacklist.conf"
}
EOF
```

Then add "zimk" as the first item to the "authenticate" rules of your
site, e.g. /etc/freeradius/sites-enabled/default:
```
# grep -A2 ^authenticate /etc/freeradius/sites-enabled/default 
authenticate {
        zimk
```
