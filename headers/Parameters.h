#ifndef __PARAMETERS_H
#define __PARAMETERS_H

#include "utilities.h"

typedef struct {
    int nx;
    int ny;
    int tot_reals;
    int numtasks;
    MPI_Status status1;
    MPI_Status status2;
    double k;
    double amp;
    long seed;
} Parameters;


#endif /* __PARAMETERS_H */


