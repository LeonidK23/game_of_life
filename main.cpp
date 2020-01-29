#include <vector>
#include <iostream>

int main(int argc, char const *argv[]) {
  int N, n_iter;
  std::vector<bool> new_grid, old_grid;
  int left, topleft, top, topright, right, bottomright, bottom, bottomleft;
  int neighbours_sum;
  int idx;

  if (argc > 1){
    N = atoi(argv[1]);
    n_iter = atoi(argv[2]);
  } else {
    N = 4;
    n_iter = 1;
  }

  new_grid.reserve(N*N);
  old_grid.reserve(N*N);
  for (int i = 0; i < N*N; i++){
    new_grid.push_back(0);
    old_grid.push_back(0);
  }

  old_grid[6] = 1;
  old_grid[10] = 1;
  old_grid[14] = 1;

  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++)
      std::cout << old_grid[i*N + j] << ' ';
    std::cout << '\n';
  }
  std::cout << "------------" << '\n';

  // game of life
  for (int k = 0; k < n_iter; k++){
    for (int i = 0; i < N; i++){
      for (int j = 0; j < N; j++){
        idx = i*N + j;
        // neighbours:
        // left
        if (idx % N == 0){
          left = idx - 1 + N;
          // top left grid corner
          if (idx < N)
            topleft = N*N - 1;
          else
            topleft = idx - 1;
          // left bottom corner of the grid
          if (idx >= N*(N - 1))
            bottomleft = N - 1;
          // left corner of the grid
          else
            bottomleft = idx - 1 + N + N;
        } else {
          // if not left border
          left = idx - 1;
          if (idx < N)
            topleft = N*(N - 1) + j - 1;
          else
            topleft = idx - 1 - N;
          // if not left and bottom
          if (idx >= N*(N - 1))
            bottomleft = j - 1;
          else
            bottomleft = idx + N - 1;
        }

        if ((idx + 1)%N == 0){
          right = i * N;
          if (idx < N)
            topright = N*(N - 1);
          else
            topright = idx - N + 1;
          if (idx >= N*(N - 1))
            bottomright = 0;
          else
            bottomright = idx + 1;
        }
        else{
          right = idx + 1;
          if (idx < N)
            topright = N*(N - 1) + j + 1;
          else
            topright = idx - N + 1;
          if (idx >= N*(N - 1))
            bottomright = j + 1;
          else
            bottomright = idx + N + 1;
        }

        if (idx < N)
          top = N*(N - 1) + j;
        else
          top = idx - N;

        if (idx >= N*(N - 1))
          bottom = j;
        else
          bottom = idx + N;

        neighbours_sum = old_grid[left] + old_grid[right] + old_grid[top] + old_grid[bottom] +
                     old_grid[topleft] + old_grid[topright] + old_grid[bottomleft] + old_grid[bottomright];

        // std::cout << idx << ' ' << top << ' ' << left << ' ' << bottom << ' ' << right << '\n';
        // std::cout << idx << ' ' << topleft << ' ' << topright << ' ' << bottomleft << ' ' << bottomright << '\n';

        if (old_grid[idx] == 1)
          new_grid[idx] = neighbours_sum >= 2 && neighbours_sum <= 3;
        else
          new_grid[idx] = neighbours_sum ==3;
      }
    }
    new_grid.swap(old_grid);
    for (int i = 0; i < N; i++){
      for (int j = 0; j < N; j++)
        std::cout << old_grid[i*N + j] << ' ';
      std::cout << '\n';
    }
    std::cout << "--------" << '\n';
  }

  return 0;
}
