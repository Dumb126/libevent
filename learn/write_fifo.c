/*************************************************************************
  > File Name: mywrite.c
  > Author: Winter
  > Created Time: 2022年02月18日 星期五 21时24分04秒
 ************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <event2/event.h>
#include <fcntl.h>
 
// 回调函数
void write_cb(evutil_socket_t fd, short what, void* arg) {
        // 写管道
        char buff[1024] = {0};
        static int num = 0;
        sprintf(buff, "hello world - %d", num++);
        write(fd, buff, strlen(buff) + 1);
        sleep(1);
}
 
int main(int argc, char* argv[])
{
        // 与myread.c配套，但是要先打开myread程序
        // 打开管道
        int fd = open("myfifo", O_WRONLY | O_NONBLOCK);
        if (fd == -1) {
                perror("open error\n");
                exit(1);
        }
 
        // 创建event_base
        struct event_base* base = event_base_new();
 
        // 创建写事件   EV_PERSIST是持续写
        struct event* ev = event_new(base, fd, EV_WRITE | EV_PERSIST, write_cb, NULL);
				// struct event* ev = event_new(base, fd, EV_WRITE, write_cb, NULL);
        // 添加事件
        event_add(ev, NULL);
 
        // 事件循环
        event_base_dispatch(base);
 
        // 释放资源
        event_free(ev);
        event_base_free(base);
        close(fd);
 
        return 0;
}