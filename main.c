#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <tui.h>

typedef struct
{
    int fd;
    char *buff;
    size_t bytes;
} read_attr_t;

void *thread_read(void *arg)
{
    read_attr_t *att = (read_attr_t *)arg;
    while (1)
    {

        read(att->fd, att->buff, att->bytes);
        printf("%c", *att->buff);
        fflush(stdout);
    }
    return NULL;
}

int main()
{

    tui_enable_raw_mode();
    pthread_t threadId;

    char buff;
    read_attr_t attr = {.fd = STDIN_FILENO, .buff = &buff, .bytes = 8};
    pthread_create(&threadId, NULL, thread_read, &attr);
    while (1)
    {
        printf("..............................\n");
        sleep(1);
    }
    pthread_join(threadId, NULL);
    return 0;
}