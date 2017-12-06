# 2D XY model with Metropolis

<p align="center">
  <img src="https://lh6.googleusercontent.com/OWXLMxqPXmXuPaTgBwvOaqSe64oftmNXk1cpLW5mHfOtpX-Y0ZkgGE6jIyLvh5jFzTowJv8m3DBuhEE6E_V9=w1440-h742" width="720"/>
</p>

## Usage

Note this is written with the MPI API for use on a cluster. The external parameters are with init_params function in main.cpp. Each computing node will work on a block of size nx x ny, where nx/(number of cores) = ny. ny must be even. 

## References
1. https://quantumtheory.physik.unibas.ch/people/bruder/Semesterprojekte2007/p6/index.html

3. http://young.physics.ucsc.edu/242/montecarlo.pdf

2. Kosterlitz, J. M.; Thouless, D. J. (1973), "Ordering, metastability and phase transitions in two-dimensional systems", Journal of Physics C: Solid State Physics, 6 (7): 1181–1203
