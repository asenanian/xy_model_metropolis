#include "Thread.h"
#include "utilities.h"

Thread::Thread(Parameters * p_pm, const int & p_myid)
    : myid(p_myid), pm(p_pm), iter(0)
{
    // calloc_2d calloc_ macros are defined in utilities.h
    calloc_2d(theta,pm->ny,pm->nx);
    calloc_(nyp1,pm->nx);
    calloc_(nym1,pm->nx);

    // randomize spins
    init_spins();
}

Thread::~Thread()
{
   free_2d(theta,pm->ny);
   free(nyp1);
   free(nym1);
}

void Thread::init_spins()
{
    for ( int row = 0; row < pm->ny; row++ )
        for ( int col = 0; col < pm->nx; col++ )
            theta[row][col] = 2*PI*ran1(&(pm->seed));

    MPI_Barrier(MPI_COMM_WORLD);
    this->update_boundaries();
}

void Thread::sweep_spins()
{
    int nx = pm->nx;
    int ny = pm->ny;
    this->iter++;
    int start_pos = this->iter % 2;

    for ( int col = start_pos; col < pm->nx; col += 2 )
    {
        double dE = -energy(theta[0][col],theta[0][(col+1)%(pm->nx)],theta[1][col],theta[0][(col-1+pm->nx)%(pm->nx)],nym1[col]);
        double theta_new = theta[0][col] + pm->amp*(ran1(&(pm->seed))-.5);
        dE += energy(theta_new,theta[0][(col+1)%(pm->nx)],theta[1][col],theta[0][(col-1+pm->nx)%(pm->nx)],nym1[col]);
        if ( ran1(&(pm->seed)) < exp(-dE) )
            theta[0][col] = theta_new;
    }

    for ( int row = 1; row < pm->ny-1; row++ )
    {
        int init = (row - start_pos)%2;
        for ( int col = init; col < pm->nx; col+= 2)
        {
            //make_move(row,col+init);  
            double dE = -energy(theta[row][col],theta[row][(col+1)%(pm->nx)],theta[row+1][col],theta[row][(col-1+pm->nx)%(pm->nx)],theta[row-1][col]);
            double theta_new = theta[row][col] + pm->amp*(ran1(&(pm->seed))-.5);
            dE += energy(theta_new,theta[row][(col+1)%(pm->nx)],theta[row+1][col],theta[row][(col-1+pm->nx)%(pm->nx)],theta[row-1][col]);
            if ( ran1(&(pm->seed)) < exp(-dE) )
                theta[row][col] = theta_new;
        }
    }

    for ( int col = (start_pos+1)%2; col < pm->nx; col += 2 )
    {
        double dE = -energy(theta[pm->ny-1][col],theta[pm->ny-1][(col+1)%(pm->nx)],nyp1[col],theta[pm->ny-1][(col-1+pm->nx)%(pm->nx)],theta[pm->ny-2][col]);
        double theta_new = theta[pm->ny-1][col] + pm->amp*(ran1(&(pm->seed))-.5);
        dE += energy(theta_new,theta[pm->ny-1][(col+1)%(pm->nx)],nyp1[col],theta[pm->ny-1][(col-1+pm->nx)%(pm->nx)],theta[pm->ny-2][col]);
        if ( ran1(&(pm->seed)) < exp(-dE) )
            theta[pm->ny-1][col] = theta_new;
    }

    this->update_boundaries();

    if ( start_pos )
    { sweep_spins(); }
}

void Thread::dump(std::ofstream & fout) const
{
    for ( int row = 0; row < pm->ny; row++ ) 
    {
        for ( int col = 0; col < pm->nx - 1; col++ )
            fout << theta[row][col] << ",";
        fout << theta[row][pm->nx - 1] << std::endl;
    }
}

/* PRIVATE FUNCTIONS */

inline
double Thread::energy(const double & theta_ij, const double & theta_ip1, const double & theta_jp1, const double & theta_im1, const double & theta_jm1)
{
    return -pm->k*(cos(theta_ij - theta_ip1) + cos(theta_ij - theta_im1) + cos(theta_ij - theta_jp1) + cos(theta_ij - theta_jm1));
}

void Thread::update_boundaries() 
{
    int n_above = (this->myid - 1 + pm->numtasks)%(pm->numtasks);
    int n_below = (this->myid + 1)%(pm->numtasks);

    MPI_Sendrecv(&theta[0][0],pm->nx,MPI_DOUBLE,n_above,myid+100*iter,
            &nyp1[0],pm->nx,MPI_DOUBLE,n_below,n_below+100*iter,
            MPI_COMM_WORLD,&(pm->status1));

    MPI_Sendrecv(&theta[pm->ny-1][0],pm->nx,MPI_DOUBLE,n_below,myid+100*iter,
            &nym1[0],pm->nx,MPI_DOUBLE,n_above,n_above+100*iter,
            MPI_COMM_WORLD,&(pm->status2));
}

