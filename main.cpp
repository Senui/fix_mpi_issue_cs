#include <iostream>
#include "util.h"
#include "mpi.h"

using std::vector;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int worldSize = 0;
    int myRank = 0;
    static int file_count = 0;
    
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    
    if (myRank == 0)
    {
        std::cout << "I'm the manager!" << std::endl;
        XMLParams params;
        
        MPI_Send_Obj_ROOT<XMLParams>(&params, 1, 1);
    }
    else
    {
        std::cout << "I'm a worker!" << std::endl;

        int size = 0;
        MPI_Recv(&size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << size << std::endl;
        XMLParams* params = nullptr;
        params = MPI_Recv_Obj_ROOT<XMLParams>(size, 0, 1);
        
        std::cout << *params << std::endl;
    }
    
    MPI_Finalize();
}