#ifndef DONGLE_H
# define DONGLE_H
# include <stdlib.h>

typedef struct s_dongle
{
    int is_free;
    int id;
} t_dongle;

t_dongle	*dongle_list_init(int coders_nb);

#endif
