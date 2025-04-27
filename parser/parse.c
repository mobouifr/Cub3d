#include "parser.h"

void    skip_empty_line(int fd)
{
    char *line;

    line = get_next_line(fd);
    while (line != NULL);
    {
        
    }
}

int parser(int argc, char **argv)
{
    int fd;

    if (argc != 2)
    {
        write(2, "Error\n", 6);
        return (1);
    }
    
    /* 
        texture_files_check()
        {}
    */

    fd = open("map.cub", O_RDONLY);
    if (fd == -1 /* || !valid_extention_check()*/)
    {
        write(2, "Error\n", 6);
        return (1);
    }
    skip_empty_lines(fd);

}