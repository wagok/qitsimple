#! /bin/sh
cp qitsimple /usr/sbin/qitsimple
chmod +x /usr/sbin/qitsimple
cp init /etc/init.d/qitsimple
chmod +x /etc/init.d/qitsimple
update-rc.d -f qitsimple defaults
update-rc.d qitsimple enable
