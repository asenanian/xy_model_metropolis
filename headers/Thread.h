#ifndef __THREAD_H
#define __THREAD_H

#include "utilities.h"
#include "Parameters.h"

class Thread {

public:
    Thread(Parameters * p_pm, const int & p_myid);
        // constructs the lattice block and copies ext parameters.
    ~Thread();
        // destructor
    void sweep_spins();
        // updates spins across the block with metropolis mcmc
    void dump(std::ofstream & fout) const;
        // displays data to output file

private:
    void init_spins();
        // called in c'tor. randomizes spins.
    void update_boundaries();
        // receives and sends boundary conditions to other threads
    double energy(const double & theta_ij, const double & theta_ip1, const double & theta_im1, const double & theta_jp1, const double & theta_jm1);
        // hamiltonian

    Parameters * pm; // contains external parameters, e.g. temp
    int iter, myid; // iter: count of sweep. myid is thread number
    double * nyp1, * nym1; // BC's for neighboring blocks
    double ** theta; // block of lattice spins

};


#endif /* __THREAD_H */
