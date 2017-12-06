#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "mpi.h"
#include "Parameters.h"

const double PI = acos(-1);

#define calloc_(pointer,l) (pointer)=(typeof(pointer))calloc((l),sizeof((pointer)[0]))

#define calloc_2d(pointer,i,j) do{                  \
    int _i;                                         \
    calloc_((pointer),(i));                         \
    for(_i=0;_i<i;_i++) calloc_((pointer)[_i],(j)); \
} while(0)

#define free_2d(pointer,i) do{                     \
    int _i;                                        \
    for(_i=0;_i<i;_i++) free((pointer)[_i]);       \
    free(pointer);                                 \
} while(0);


float ran1(long *idum);
void init_params(Parameters * pm, int numtasks);

#endif /* __UTILITIES_H */
