#include <mpi.h>

#include <boost/mpi.hpp>

#include <vector>

namespace mpi = boost::mpi;

int main() {
  mpi::environment env;
  mpi::communicator world;

  std::cout << "I am process " << world.rank() << " out of " << world.size()
            << " processes" << '\n';

  if (world.rank() == 0 && world.size() > 1) {
    world.send(1, 0, std::string("Hello"));
    std::string msg;
    world.recv(1, 0, msg);
    std::cout << msg << "!" << std::endl;
    world.send(1, 1, std::vector<double>{1, 2, 3});
  } else if (1 == world.rank() && world.size() > 1) {
    std::string msg;
    world.recv(0, 0, msg);
    std::cout << msg << ", ";
    std::cout.flush();
    world.send(0, 0, std::string("world"));
    std::vector<double> v;
    world.recv(0, 1, v);
    for (auto e : v)
      std::cout << "received: " << e << '\n';
  }

  std::string s = "ok boomer";
  broadcast(world, s, 0);
  std::cout << "rank " << world.rank() << " says: " << s << '\n';

  if (0 == world.rank()) {
    std::vector<int> numbers;
    int mynumber = 42;
    mpi::gather(world, mynumber, numbers, 0);
    for (auto e : numbers)
      std::cout << "rank 0 owns: " << e << '\n';
  } else {
    int mynumber = world.rank() * world.rank();
    mpi::gather(world, mynumber, 0);
  }

  auto magic = [](int a, int b) { return a + b; };
  if (0 == world.rank()) {
    int mynumber = 3432;
    int minimum;
    reduce(world, mynumber, minimum, magic, 0);
    std::cout << "minimum is " << minimum << '\n';
  } else {
    int mynumber = world.rank() * world.rank();
    reduce(world, mynumber, magic, 0);
  }

  return 0;
}
