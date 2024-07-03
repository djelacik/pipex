#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"
int    main(void)
{
    char    *line;
    char    *line2;
    char    *line3;
    int        one = 1;
    int        two = 1;
    int        three =1;
    int        fd1;
    int        fd2;
    int        fd3;
    fd1 = open("heartless.txt", O_RDONLY);
    fd2 = open("stronger", O_RDONLY);
    fd3 = open("test", O_RDONLY);
    while (1)
    {
        line = get_next_line(fd1);
        line2 = get_next_line(fd2);
        line3 = get_next_line(fd3);
        if (!line)
            break ;
        if (line)
        printf("fd 1 line [%02d]: %s", one, line);
        if (line2)
        printf("fd 2 line [%02d]: %s", two, line2);
        if (line3)
        printf("fd 3 line [%02d]: %s", three, line3);
        free(line);
        free(line2);
        free(line3);
        one++;
        two++;
        three++;
    }
    close(fd1);
    close(fd2);
    close(fd3);
    return (0);
}