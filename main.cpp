#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <mpi.h>

void save_grid(int* grid, int m, int n, int iter){
  std::ofstream myfile;
  std::string filename ("../data/life__");
  filename += std::to_string(iter)+".pbm";
  myfile.open(filename);

  myfile << "P1\n" << m << " " << n << '\n';
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++)
      myfile << grid[i*n+j] << ' ';
    myfile << '\n';
  }
  myfile.close();
}

int main(int argc, char *argv[]) {
  int N, n_iter;
  std::vector<int> old_grid;
  int left, topleft, top, topright, right, bottomright, bottom, bottomleft;
  int neighbours_sum;
  int idx;
  int height;

  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<> dis(0, 1);

  int world_size, rank;
  int ret = MPI_Init(&argc, &argv);
  MPI_Request r;
  MPI_Status stat;

  if(MPI_SUCCESS != ret)
    MPI_Abort(MPI_COMM_WORLD, ret);

  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (argc > 1){
    N = atoi(argv[1]);
    n_iter = atoi(argv[2]);
  } else {
    N = 4;
    n_iter = 1;
  }

  height = N/world_size + 2;
  int elements_per_proc = N*N/world_size;
  int* part_grid = new int[height*N];
  int* new_part_grid = new int[height*N];
  int* new_grid = new int[N*N];

  if (rank == 0){
    for (int i = 0; i < N*N; i++){
      if (dis(gen) > 0.5)
        old_grid.push_back(1);
      else
        old_grid.push_back(0);
    }

    save_grid(old_grid.data(), N, N, 0);
  }

  MPI_Scatter(old_grid.data(), elements_per_proc, MPI_INT, part_grid + N,
              elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

  for (int k = 0; k < n_iter; k++){
    // std::cout << rank << ' ' << (rank+1)%world_size << ' ' << (rank-1+world_size)%world_size << '\n';

    MPI_Sendrecv(part_grid + (N/world_size)*N, N, MPI_INT, (rank+1)%world_size, 0, part_grid, N, MPI_INT,
                 (rank-1+world_size)%world_size, 0, MPI_COMM_WORLD, &stat);
    MPI_Sendrecv(part_grid + N, N, MPI_INT, (rank-1+world_size)%world_size, 0, part_grid + (N/world_size+1)*N, N, MPI_INT,
                 (rank+1)%world_size, 0, MPI_COMM_WORLD, &stat);

    // MPI_Sendrecv(part_grid + (N/world_size)*N, N, MPI_INT, (rank+1)%world_size, 0, part_grid + (N/world_size+1)*N, N, MPI_INT,
    //              (rank+1)%world_size, 0, MPI_COMM_WORLD, &stat);
    // MPI_Sendrecv(part_grid + N, N, MPI_INT, (rank-1+world_size)%world_size, 0, part_grid, N, MPI_INT,
    //              (rank-1+world_size)%world_size, 0, MPI_COMM_WORLD, &stat);

    // game of life
    for (int i = 1; i < height - 1; i++){
      for (int j = 0; j < N; j++){
        // left + right + top + bottom
        neighbours_sum = part_grid[i*N + (j-1+N)%N] + part_grid[i*N+(j+1)%N] + part_grid[((i-1+N)%N)*N+j] + part_grid[((i+1)%N)*N+j] +
        // top left + top right + bottom left + bottom right
        part_grid[((i-1+N)%N)*N+(j-1+N)%N] + part_grid[((i-1+N)%N)*N+(j+1)%N] + part_grid[((i+1)%N)*N+(j-1+N)%N] + part_grid[((i+1)%N)*N+(j+1)%N];

        if (part_grid[i*N+j] == 1)
        new_part_grid[i*N+j] = neighbours_sum >= 2 && neighbours_sum <= 3;
        else
        new_part_grid[i*N+j] = neighbours_sum ==3;

      }
    }
    MPI_Gather(new_part_grid + N, N*N/world_size, MPI_INT, new_grid, N*N/world_size, MPI_INT, 0,
      MPI_COMM_WORLD);

    std::swap(part_grid, new_part_grid);

    if (rank == 0)
      save_grid(new_grid, N, N, k+1);
  }

  MPI_Finalize();

  return 0;
}
