add_executable(benchmarks Movegen_benchmark.cc Search_benchmark.cc)

target_link_libraries(benchmarks engine)
target_link_libraries(benchmarks Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(benchmarks)