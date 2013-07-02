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
# make install
```

Add options for the zimk module:
```
# cat << EOF > /etc/freeradius/modules/zimk
zimk {
  blacklist_file = "/etc/zimk_user_blacklist.conf"
}
EOF
```

Then add "zimk" as the first item to the "authorize" rules of your
site, e.g. /etc/freeradius/sites-enabled/default:
```
# grep -A2 ^authorize /etc/freeradius/sites-enabled/default 
authorize {
        zimk
```


To test
-------

Check if simple auth is working:
```
# echo 'foouser Cleartext-Password := "foopasswd"' >> /etc/freeradius/users
# /etc/init.d/freeradius reload
# radtest foouser foopasswd localhost 0 testing123
rad_recv: Access-Accept packet from host 127.0.0.1 port 1812, id=194, length=20
```

Then add the user to the blacklist:
```
# echo foouser >> /etc/zimk_user_blacklist.conf
# radtest foouser foopasswd localhost 0 testing123
rad_recv: Access-Reject packet from host 127.0.0.1 port 1812, id=177, length=20
```


References
----------

This resource: http://wiki.freeradius.org/contributing/Modules1
is very helpful for radius module development.
