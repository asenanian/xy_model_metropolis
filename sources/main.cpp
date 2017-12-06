#include "Thread.h"
#include "Parameters.h"
#include "utilities.h"


int main(int argc, char ** argv) {
    int numtasks, myid, ierr; // MPI variables
    long init_seed;

    //MPI init.
    MPI_Status status;
    ierr = MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    Parameters *pm;
    calloc_(pm,1);
    init_params(pm,numtasks);

    // distribute seeds across nodes
    if ( myid == 0 ) { 
        pm->seed = -(long)time(NULL);
        for ( int i = 1; i < numtasks; i++)
        {
            init_seed = (long)(-10000*ran1(&(pm->seed)));
            ierr = MPI_Send(&init_seed,1,MPI_LONG,i,1,MPI_COMM_WORLD);
        }
    }
    else 
    { ierr = MPI_Recv(&(pm->seed),1,MPI_LONG,0,1,MPI_COMM_WORLD,&status); }

    // initialize threads to handle blocks of lattice
    Thread * thread = new Thread(pm,myid);

    for ( int i = 0; i < pm->tot_reals; i++)
        thread->sweep_spins();

    std::string projectname = argv[1];

    std::stringstream filename;
    std::ofstream fout;
    filename << "output/" << projectname
             << "/run" << myid << ".csv" << std::ends;
    fout.open(filename.str().c_str());
    fout.precision(13);
    fout.setf(std::ios::fixed, std::ios::floatfield);

    thread->dump(fout);
    

    delete thread;
    free(pm);
    ierr = MPI_Finalize();
    return 0;
}

void init_params(Parameters * pm, int numtasks) 
{
    pm->nx = 3600;
    pm->numtasks = numtasks;
    pm->ny = pm->nx/(pm->numtasks);
    pm->tot_reals = 400000;
    pm->amp = PI/3.;
    pm->k = 1.12;
}
