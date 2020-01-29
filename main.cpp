#include <vector>
#include <iostream>
#include <fstream>
#include <random>

void save_grid(std::vector<std::vector<bool>> grid, int m, int n, int iter){
  std::ofstream myfile;
  std::string filename ("../data/life__");
  filename += std::to_string(iter)+".pbm";
  myfile.open(filename);

  myfile << "P1\n" << m << " " << n << '\n';
  for (int i = 0; i < m; i++){
    for (int j = 0; j < n; j++)
      myfile << grid[i][j] << ' ';
    myfile << '\n';
  }
  myfile.close();
}

int main(int argc, char const *argv[]) {
  int N, n_iter;
  std::vector<std::vector<bool>> new_grid, old_grid;
  int left, topleft, top, topright, right, bottomright, bottom, bottomleft;
  int neighbours_sum;
  int idx;

  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<> dis(0, 1);

  if (argc > 1){
    N = atoi(argv[1]);
    n_iter = atoi(argv[2]);
  } else {
    N = 4;
    n_iter = 1;
  }

  for (int i = 0; i < N*N; i++){
    std::vector<bool> row(N, 0);
    new_grid.push_back(row);
    old_grid.push_back(row);
    for (int j = 0; j < N; j++)
      if (dis(gen) > 0.4)
        old_grid[i][j] = 1;
      else
        old_grid[i][j] = 0;
  }

  save_grid(old_grid, N, N, 0);

  // game of life
  for (int k = 0; k < n_iter; k++){
    for (int i = 0; i < N; i++){
      for (int j = 0; j < N; j++){
                         // left + right + top + bottom
       neighbours_sum = old_grid[i][(j-1+N)%N] + old_grid[i][(j+1)%N] + old_grid[(i-1+N)%N][j] + old_grid[(i+1)%N][j] +
                         // top left + top right + bottom left + bottom right
                        old_grid[(i-1+N)%N][(j-1+N)%N] + old_grid[(i-1+N)%N][(j+1)%N] + old_grid[(i+1)%N][(j-1+N)%N] + old_grid[(i+1)%N][(j+1)%N];

       if (old_grid[i][j] == 1)
         new_grid[i][j] = neighbours_sum >= 2 && neighbours_sum <= 3;
       else
         new_grid[i][j] = neighbours_sum ==3;

      }
    }
    new_grid.swap(old_grid);

    save_grid(old_grid, N, N, k+1);
  }

  return 0;
}
