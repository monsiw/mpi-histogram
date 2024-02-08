#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) 
{
    int myid, numprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid); 
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	
    int N = numprocs-2;
	
    int* x = new int[(numprocs-2)*5];
    int* A = new int[numprocs-2];
    int* H = new int[numprocs-2];

    std::fill_n(x, (numprocs-2)*5, 0);
	std::fill_n(A, numprocs-2, 0);
	std::fill_n(H, numprocs-2, 0);
	
    int temp0 = 0; int temp = 0; int temp2 = 0;
    int tabHi = 1; 
    int end_for_x = 0;
    int end_sig_seg_p = 0;
    int thresholds = -1;

    if(myid == 0) 
    {
        int number = 10; 
        std::cout << std::endl << "Thresholds: " << std::endl; 
        for(int i = 0; i < N; i++)
        {
            A[i] = number;
            number = number + 10;
            std::cout << A[i] << " ";
            std::cout.flush();
        }
        srand(time(0));
        std::cout << std::endl << "List of generated numbers:" << std::endl;
        for(int i = 0; i < 5*N; i++) 
        {
            int random_number = rand() % 100;
            x[i] = random_number;
            std::cout << x[i] << " ";
            std::cout.flush();
        }
        for(int i = 0; i < N; i++)
        {
            MPI_Send(&(A[i]), 1, MPI_INT, i+1, i+1, MPI_COMM_WORLD);            
        }
        MPI_Recv(&thresholds, 1, MPI_INT, N, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i = 0; i < 5*N; i++)
        {
            MPI_Send(&x[i], 1, MPI_INT, 1, i+1+300, MPI_COMM_WORLD);
        }
        end_for_x = -2;
        MPI_Send(&end_for_x, 1, MPI_INT, 1, 351, MPI_COMM_WORLD); 
        for(int i = 1; i <= N; i++) 
        {
            MPI_Recv(&end_sig_seg_p, 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        std::cout << std::endl << "Histogram:" << std::endl; std::cout.flush();
        MPI_Recv(H, 10, MPI_INT, N+1, N+1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        for(int i = 0; i < N; i++) 
        {
           std::cout << H[i] << ", ";
           std::cout.flush();
        }
    }

    int i = 0, x1 = 0, y = 0;

    if(myid > 0 && myid <= N) 
    {
        MPI_Recv(&temp, 1, MPI_INT, 0, myid, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        if(myid == 1)
        {
            while(temp0 != -2)
            {
                i++;
                MPI_Recv(&temp0, 1, MPI_INT, 0, i+300, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if(temp0 < temp)
                {
                    if(temp0 != -2) MPI_Send(&tabHi, 1, MPI_INT, N+1, myid, MPI_COMM_WORLD);
                }
                else
                {
                    if(temp0 != -2) MPI_Send(&temp0, 1, MPI_INT, myid+1, myid, MPI_COMM_WORLD);
                }
            } 
            end_sig_seg_p = -1;
            MPI_Send(&end_sig_seg_p, 1, MPI_INT, 0, myid, MPI_COMM_WORLD);
            x1 = -1;
            MPI_Send(&x1, 1, MPI_INT, myid+1, myid, MPI_COMM_WORLD);
            tabHi = -1;
            MPI_Send(&tabHi, 1, MPI_INT, N+1, myid, MPI_COMM_WORLD);
            tabHi = 1;
        }
        if(myid > 1 && myid < N)
        {          
            tabHi = 1;
            while(temp2 != (-1))
            {
                MPI_Recv(&temp2, 1, MPI_INT, myid-1, myid-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);                  
                if(temp2 < temp)
                {
                    if(temp2 != (-1)) MPI_Send(&tabHi, 1, MPI_INT, N+1, myid, MPI_COMM_WORLD);
                }
                else
                {
                    if(temp2 != (-1)) MPI_Send(&temp2, 1, MPI_INT, myid+1, myid, MPI_COMM_WORLD);
                }
            }
            end_sig_seg_p = -1;
            y = -1;
            MPI_Send(&end_sig_seg_p, 1, MPI_INT, 0, myid, MPI_COMM_WORLD);
            MPI_Send(&y, 1, MPI_INT, myid+1, myid, MPI_COMM_WORLD);   
            tabHi = -1;
            MPI_Send(&tabHi, 1, MPI_INT, N+1, myid, MPI_COMM_WORLD);
            y = 0;
            tabHi = 1;
        }
        if(myid == N)
        {
            tabHi = 1;
            MPI_Send(&thresholds, 1, MPI_INT, 0, 101, MPI_COMM_WORLD);
            while(temp2 != (-1))
            {
                MPI_Recv(&temp2, 1, MPI_INT, myid-1, myid-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if(temp2 != (-1)) MPI_Send(&tabHi, 1, MPI_INT, N+1, myid, MPI_COMM_WORLD);                       
            }   
            tabHi = -1;
            MPI_Send(&tabHi, 1, MPI_INT, N+1, myid, MPI_COMM_WORLD);
            tabHi = 1;
            MPI_Send(&end_sig_seg_p, 1, MPI_INT, 0, myid, MPI_COMM_WORLD);
        }
    }

    if(myid == N+1) 
    {
        tabHi = 1;
        for(int i = 1; i <= N; i++) 
        {
            while(tabHi != -1)
            {
                MPI_Recv(&tabHi, 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if(tabHi != (-1)) H[i-1]++;
            }
            tabHi = 1;
        }
        MPI_Send(H, 10, MPI_INT, 0, N+1, MPI_COMM_WORLD);         
    }
	
    delete[] x;
    delete[] A;
    delete[] H;
    MPI_Finalize();
	return 0;	
}