# netshim
Network tracing shim for POSIX.


Intercepts following libc functions during runtime and logs them to `stderr`:

* `connect()`
* `getnameinfo()`
* `gethostbyname()`
* `gethostbyname_r()`
* `gethostbyname2()`
* `gethostbyname2_r()`
* `getaddrinfo()`



## Usage

```
git clone https://github.com/gcarq/netshim.git
cd netshim
gcc -s -O2 -shared -fPIC -o netshim.{so,c} -ldl
LD_PRELOAD=/full/path/to/netshim.so /usr/bin/foo
```


## Example

```
$ LD_PRELOAD=/home/foo/netshim.so chromium
23:53:53 [CONNECT] AF_UNIX: 	socket_type=0x00000001
23:53:53 [CONNECT] AF_UNIX: /run/dbus/system_bus_socket	socket_type=0x00000001
23:53:53 [CONNECT] AF_UNIX: /run/user/1000/pulse/native	socket_type=0x00000001
23:53:53 [CONNECT] AF_UNIX: /var/run/pulse/native	socket_type=0x00000001
23:53:54 [CONNECT] AF_UNIX: /run/user/1000/bus	socket_type=0x00000001
23:53:54 [CONNECT] 2001:4860:4860::8888:53	socket_type=0x00000002
23:53:54 [GETADDRINFO] www.google.at
23:53:54 [CONNECT] AF_UNIX: /var/run/dbus/system_bus_socket	socket_type=0x00000001
23:53:54 [GETADDRINFO] apis.google.com
23:53:54 [CONNECT] AF_UNIX: /var/run/dbus/system_bus_socket	socket_type=0x00000001
23:53:54 [CONNECT] 8.8.4.4:53	socket_type=0x00000002
23:53:54 [GETADDRINFO] ssl.gstatic.com
23:53:54 [CONNECT] AF_UNIX: /var/run/dbus/system_bus_socket	socket_type=0x00000001
23:53:54 [GETADDRINFO] www.google.com
23:53:54 [CONNECT] AF_UNIX: /var/run/dbus/system_bus_socket	socket_type=0x00000001
23:53:54 [GETADDRINFO] translate.googleapis.com
23:53:54 [CONNECT] AF_UNIX: /var/run/dbus/system_bus_socket	socket_type=0x00000001
23:53:54 [GETADDRINFO] www.gstatic.com
23:53:54 [CONNECT] 216.58.214.99:443	socket_type=0x00000001
23:53:54 [CONNECT] AF_UNIX: /var/run/dbus/system_bus_socket	socket_type=0x00000001
23:53:54 [CONNECT] 8.8.4.4:53	socket_type=0x00000002
23:53:54 [CONNECT] 216.58.214.106:443	socket_type=0x00000001
23:53:59 [CONNECT] 2001:4860:4860::8888:53	socket_type=0x00000002
23:53:59 [GETADDRINFO] www.google.com
23:53:59 [CONNECT] AF_UNIX: /var/run/dbus/system_bus_socket	socket_type=0x00000001
23:53:59 [CONNECT] 8.8.4.4:53	socket_type=0x00000002
23:53:59 [CONNECT] 216.58.214.100:443	socket_type=0x00000001
23:54:01 [CONNECT] 2001:4860:4860::8888:53	socket_type=0x00000002
23:54:01 [GETADDRINFO] csnoqgaal
23:54:01 [CONNECT] AF_UNIX: /var/run/dbus/system_bus_socket	socket_type=0x00000001
23:54:01 [CONNECT] 8.8.4.4:53	socket_type=0x00000002
23:54:01 [GETADDRINFO] kkbrnillvnhssns
23:54:01 [GETADDRINFO] rtorxljrlqijao
23:54:01 [CONNECT] AF_UNIX: /var/run/dbus/system_bus_socket	socket_type=0x00000001
23:54:01 [CONNECT] AF_UNIX: /var/run/dbus/system_bus_socket	socket_type=0x00000001
23:54:01 [CONNECT] 8.8.4.4:53	socket_type=0x00000002
23:54:04 [CONNECT] 2001:4860:4860::8888:53	socket_type=0x00000002
23:54:04 [CONNECT] 216.58.214.99:443	socket_type=0x00000001
```
