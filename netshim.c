/*
 * LD_PRELOAD Network tracing shim for POSIX
 *
 * Build with: gcc -s -O2 -shared -fPIC -o netshim.{so,c} -ldl
 */

/* for RTLD_NEXT */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

void write_log(const char *format, ...) {
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 4096, format, args);
    va_end(args);

    time_t timer;
    time(&timer);

    struct tm* tm_info;
    tm_info = localtime(&timer);

    char time_str[9];
    strftime(time_str, 9, "%H:%M:%S", tm_info);
    fprintf(stderr, "%s %s", time_str, buffer);
}

void addr_to_str(char *buffer, size_t bufsize, const struct sockaddr *addr) {
    switch(addr->sa_family) {
        case AF_INET: {
            struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
            char ip_addr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(addr_in->sin_addr), ip_addr, INET_ADDRSTRLEN);
            snprintf(buffer, bufsize, "%s:%d", ip_addr, ntohs(addr_in->sin_port));
            break;
        }
        case AF_INET6: {
            struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)addr;
            char ip_addr[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &(addr_in6->sin6_addr), ip_addr, INET6_ADDRSTRLEN);
            snprintf(buffer, bufsize, "%s:%d", ip_addr, ntohs(addr_in6->sin6_port));
            break;
        }
        case AF_UNIX: {
            struct sockaddr_un *addr_un = (struct sockaddr_un *)addr;
            snprintf(buffer, bufsize, "AF_UNIX: %s", addr_un->sun_path);
            break;
        }
        default:
            snprintf(buffer, bufsize, "socket_type: %d", addr->sa_family);
            break;
    }
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int (*libc_connect)(int, const struct sockaddr *, socklen_t) = dlsym(RTLD_NEXT, "connect");

    int sock_type;
    int length = sizeof(int);
    getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &sock_type, &length);

    char str_addr[109]; // len(sun_path) + 1
    addr_to_str(str_addr, INET6_ADDRSTRLEN * 2, addr);
    write_log("[CONNECT] %s\tsocket_type=0x%08x\n", str_addr, sock_type);
    return libc_connect(sockfd, addr, addrlen);
}

/*
ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
    ssize_t (*libc_send)(int, const void *, size_t, int) = dlsym(RTLD_NEXT, "send");

    ssize_t res = libc_send(sockfd, buf, len, flags);
    if(res > 0) {
        write_log("[SEND] Sent %zu bytes\n", res);
    }
    return res;
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
    ssize_t (*libc_recv)(int, void *, size_t, int) = dlsym(RTLD_NEXT, "recv");

    ssize_t res = libc_recv(sockfd, buf, len, flags);
    if(res > 0) {
        write_log("[RECV] Received %zu bytes\n", res);
    }
    return res;
}

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen) {
    ssize_t (*libc_recvfrom)(int, void *, size_t, int, struct sockaddr *, socklen_t *) = dlsym(RTLD_NEXT, "recvfrom");

    ssize_t res = libc_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    if(res > -1) {
        write_log("[RECVFROM] Received %zu bytes\n", res);
    } else {
        write_log("[RECVFROM] Error occured.\n");
    }
    return res;
}

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags) {
    ssize_t (*libc_recvmsg)(int, struct msghdr *, int) = dlsym(RTLD_NEXT, "recvmsg");

    ssize_t res = libc_recvmsg(sockfd, msg, flags);
    if(res > -1) {
        write_log("[RECVMSG] Received %zu bytes\n", res);
    } else {
        write_log("[RECVMSG] Error occured.\n");
    }
    return res;
}
*/

extern int getnameinfo (const struct sockaddr * sa,
			socklen_t salen, char * host,
			socklen_t hostlen, char * serv,
			socklen_t servlen, int flags) {
    int (*libc_getnameinfo)(const struct sockaddr *, socklen_t, char *, socklen_t, char *, socklen_t, int) = dlsym(RTLD_NEXT, "getnameinfo");

    write_log("[GETNAMEINFO] %s\n", host);

    return libc_getnameinfo(sa, salen, host, hostlen, serv, servlen, flags);
}

struct hostent *gethostbyname(const char *name) {
    struct hostent* (*libc_gethostbyname)(const char *) = dlsym(RTLD_NEXT, "gethostbyname");

    write_log("[GETHOSTBYNAME] %s\n", name);

    return libc_gethostbyname(name);
}

int gethostbyname_r(const char *name,
        struct hostent *ret, char *buf, size_t buflen,
        struct hostent **result, int *h_errnop) {
    struct hostent* (*libc_gethostbyname_r)(const char *, struct hostent *, char *, size_t, struct hostent **, int *) = dlsym(RTLD_NEXT, "gethostbyname_r");

    write_log("[GETHOSTBYNAME_R] %s\n", name);

    return libc_gethostbyname_r(name, ret, buf, buflen, result, h_errnop);
}

struct hostent *gethostbyname2(const char *name, int af) {
    struct hostent* (*libc_gethostbyname2)(const char *, int af) = dlsym(RTLD_NEXT, "gethostbyname2");

    write_log("[GETHOSTBYNAME2] %s\n", name);

    return libc_gethostbyname2(name, af);
}

int gethostbyname2_r(const char *name, int af,
        struct hostent *ret, char *buf, size_t buflen,
        struct hostent **result, int *h_errnop) {
    struct hostent* (*libc_gethostbyname2_r)(const char *, int, struct hostent *, char *, size_t, struct hostent **, int *) = dlsym(RTLD_NEXT, "gethostbyname2_r");

    write_log("[GETHOSTBYNAME2_R] %s\n", name);

    return libc_gethostbyname2_r(name, af, ret, buf, buflen, result, h_errnop);
}

int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints,
                struct addrinfo **res) {
    int (*libc_getaddrinfo)(const char *, const char *, const struct addrinfo *, struct addrinfo **) = dlsym(RTLD_NEXT, "getaddrinfo");

    write_log("[GETADDRINFO] %s\n", node);

    return libc_getaddrinfo(node, service, hints, res);
}
