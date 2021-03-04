#include <iostream>
#include "Debug.hpp"
#include "LinearAllocator.h"
#include <benchmark/benchmark.h>
#include <vector>
#include <numeric>

using namespace alloc;
using namespace std;

vector<string> IntToStringConversionTest(int count)
{
    vector<int> inputNumbers(count);
    vector<string> outNumbers;

    iota(begin(inputNumbers), end(inputNumbers), 0);
    for (auto &num : inputNumbers)
        outNumbers.push_back(to_string(num));

    return outNumbers;
}

void IntToString(benchmark::State& state) {
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(
           IntToStringConversionTest(state.range_x())
        );
    }
}
BENCHMARK(IntToString)->Arg(100);
BENCHMARK_MAIN();

