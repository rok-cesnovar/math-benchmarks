#include <benchmark/benchmark.h>
#include <stan/math.hpp>
#include <utility>
#include "toss_me.hpp"
#include "callback_bench_impl.hpp"

static void mdivide_left_spd_matrix_vector(benchmark::State& state) {
  using stan::math::var;
  using stan::math::promote_scalar;
  
  auto init = [](benchmark::State& state) {
    Eigen::MatrixXd x_val = Eigen::MatrixXd::Random(state.range(0), state.range(0));
    Eigen::VectorXd y_val = Eigen::VectorXd::Random(state.range(0));

    x_val = (x_val + x_val.transpose()).eval();

    for(size_t i = 0; i < x_val.rows(); ++i)
      x_val(i, i) += 2.0 * state.range(0);

    return std::make_tuple(CAST_VAR(x_val), CAST_VAR(y_val));
  };

  auto run = [](const auto&... args) {
    return sum(mdivide_left_spd(args...));
  };

  callback_bench_impl(init, run, state);
}

static void mdivide_left_spd_matrix_matrix(benchmark::State& state) {
  using stan::math::var;
  using stan::math::promote_scalar;
  
  auto init = [](benchmark::State& state) {
    Eigen::MatrixXd x_val = Eigen::MatrixXd::Random(state.range(0), state.range(0));
    Eigen::MatrixXd y_val = Eigen::MatrixXd::Random(state.range(0), state.range(0));

    x_val = (x_val + x_val.transpose()).eval();

    for(size_t i = 0; i < x_val.rows(); ++i)
      x_val(i, i) += 2.0 * state.range(0);

    return std::make_tuple(CAST_VAR(x_val), CAST_VAR(y_val));
  };

  auto run = [](const auto&... args) {
    return sum(mdivide_left_spd(args...));
  };

  callback_bench_impl(init, run, state);
}

// The start and ending sizes for the benchmark
int start_val = 2;
int end_val = 1024;
BENCHMARK(toss_me);
BENCHMARK(mdivide_left_spd_matrix_vector)->RangeMultiplier(2)->Range(start_val, end_val)->UseManualTime();
BENCHMARK(mdivide_left_spd_matrix_matrix)->RangeMultiplier(2)->Range(start_val, end_val)->UseManualTime();
BENCHMARK_MAIN();
