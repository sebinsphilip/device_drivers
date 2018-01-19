#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "query_ioctl.h"

void get_vars(int fd)
{
	query_arg_t q;

	if (ioctl(fd, QUERY_GET_VARIABLES, &q) == -1)
	{
		perror("query_apps ioctl get");
	}
	else
	{
		printf("Status : %d\n", q.status);
		printf("Dignity: %d\n", q.dignity);
		printf("Ego	: %d\n", q.ego);
	}
}

void clr_vars(int fd)
{
	if (ioctl(fd, QUERY_CLR_VARIABLES) == -1)
	{
		perror("query_apps ioctl clr");
	}
}

void set_vars(int fd)
{
    query_arg_t q;
    printf ("Enter status dignity ego :");
    scanf ("%d %d %d", &q.status, &q.dignity, &q.ego);
    if (ioctl(fd, QUERY_SET_VARIABLES, &q) == -1)
    {
		perror("query_apps ioctl set");
    }
}

int main (int argc, char *argv[])
{
    char *file_name = "/dev/query";
    int fd;
    enum {
        e_get,
        e_clr,
        e_set
    }option;
    if (1 == argc)
    {
        option = e_get;
    }
    else if (2 == argc)
    {
        if ("-g" == argv[1])
        {
            option = e_get;
        }
        else if ("-c" == argv[1])
        {
            option = e_clr;
        }
        else if ("-s" == argv[1])
        {
            option = e_set;
        }
        else
        {
            fprintf(stderr, "Usage: %s [ -g | -s | -c ] \n", argv[0]);
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Usage: %s [ -g | -s | -c ] \n", argv[0]);
        return 1;
    }
    fd = open(file_name, O_RDWR);
    if (-1 == fd)
    {
        perror ("query_apps open\n");
        return 2;
    }
    switch (option)
    {
        case e_get:
            get_vars(fd);
            break;
        case e_clr:
            clr_vars(fd);
            break;
        case e_set:
            set_vars(fd);
            break;
        default:
            break;
    }
    close(fd);
}
