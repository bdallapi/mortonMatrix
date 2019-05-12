#include <boost/program_options.hpp>
#include <cstdlib>
#include <iostream>
#include <mortonMatrix/mortonMatrix.hpp>

using namespace morton;
namespace po = boost::program_options;

struct Args {
  bool useMortonOrder = true;
  uint16_t matrixM = 1024;
  uint16_t matrixN = 1024;
  uint16_t blockM = 16;
  uint16_t blockN = 16;
  size_t iterations = 1000;
};

Args readArgs(int argc, char *argv[]) {
  Args args;
  po::options_description desc("profile Morton-ordered matrices");
  desc.add_options()("help", "produce help message")(
      "rows,r", po::value<uint16_t>(&args.matrixM)->default_value(1024),
      "Number of rows")("columns,c",
                        po::value<uint16_t>(&args.matrixN)->default_value(1024),
                        "Number of columns")(
      "blockRows,m", po::value<uint16_t>(&args.blockM)->default_value(16),
      "Block number of rows")(
      "blockCols,n", po::value<uint16_t>(&args.blockN)->default_value(16),
      "Block number of columns")(
      "iterations,i", po::value<size_t>(&args.iterations)->default_value(1000),
      "number of iterations")("ordering,o",
                              po::value<std::string>()->default_value("morton"),
                              "ordering (\"morton\", \"rowMajor\")");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    throw std::runtime_error("");
  }

  if (vm.at("ordering").as<std::string>() == "morton") {
    args.useMortonOrder = true;
  } else if (vm.at("ordering").as<std::string>() == "rowMajor") {
    args.useMortonOrder = false;
  } else {
    std::cout << "ordering must be either \"morton\" or \"rowMajor\""
              << std::endl;
    throw std::runtime_error("");
  }
  return args;
}

int main(int argc, char *argv[]) {
  try {
    auto args = readArgs(argc, argv);
    return EXIT_SUCCESS;
  } catch (...) {
    return EXIT_FAILURE;
  }
}
