from ubuntu:20.04

run apt-get -qq update && apt-get install -qq --no-install-recommends xinetd rsyslog python3 python3-pip build-essential
run pip3 install click

# only for testing
run apt-get install -qq --no-install-recommends vim net-tools netcat.openbsd
run pip3 install ipython


run mkdir -p /rick && chmod 755 /rick
copy backend.py /rick/
run chmod 755 /rick/backend.py
run chown -R root:root /rick

copy flag /rick/flag
run chmod 644 /rick/flag

copy rick.conf /etc/xinetd.d/rick.conf
run chmod 440 /etc/xinetd.d/rick.conf
copy banner_fail /banner_fail

copy wrapper /opt/wrapper
run chmod 555 /opt/wrapper
run chmod 551 /opt

run chmod 773 /tmp/

expose 4343
# cmd chmod go-rwx /proc && /usr/sbin/xinetd -syslog local0 -dontfork -f /etc/xinetd.d/rick.conf
cmd /usr/sbin/xinetd -syslog local0 -dontfork -f /etc/xinetd.d/rick.conf
