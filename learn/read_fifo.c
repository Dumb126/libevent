#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <event2/event.h>

void read_cb(evutil_socket_t fd, short what, void *arg)
{
    char buffer[1024] = {0};
    int len = read(fd, buffer, sizeof(buffer));

    printf("read event : %s \n", what & EV_READ ? "YES" : "NO");
    printf("data len = %d, buff = %s\n", len, buffer);
    sleep(1);
}

int main()
{
    unlink("myfifo");
    int res = mkfifo("myfifo", 0664);
    if (-1 == res)
    {
        /* code */
        perror("mkfifo error\n");
        return -1;
    }

    int fd = open("myfifo", O_RDONLY | O_NONBLOCK);
    if(-1 == fd)
    {
        perror("open error\n");
        return -1;
    }

    /* 创建event_base */
    struct event_base * ev_base = event_base_new();

    /* 创建事件 */
    struct event *ev = event_new(ev_base, fd, EV_READ | EV_PERSIST, read_cb, NULL);

    /* 添加事件 */
    event_add(ev, NULL);

    event_base_dispatch(ev_base);

    /* 释放资源 */
    event_free(ev);
    event_base_free(ev_base);
    close(fd);
    return 0;
    
}