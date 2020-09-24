#!/usr/bin/python3
import subprocess
import argparse
import os

benchmarks = "trace_gen_inv_quad_form_ldlt trace_inv_quad_form_ldlt mdivide_left_ldlt log_determinant_ldlt inv_chi_square gumbel gamma cholesky_decompose eigenvalues_sym eigenvectors_sym sd squared_distance variance sum unit_vector_constrain quad_form trace_quad_form trace_gen_quad_form multiply mdivide_left mdivide_left_spd mdivide_left_tri matrix_power softmax log_softmax log_determinant_spd log_determinant log_sum_exp dot_product divide gp_exp_quad_cov gp_periodic_cov dot_self columns_dot_self tcrossprod determinant inverse multiply_lower_tri_self_transpose columns_dot_product rows_dot_product cauchy chi_square double_exponential exp_mod_normal exponential frechet ordered_constrain positive_ordered_constrain simplex_constrain".split()

parser = argparse.ArgumentParser()
parser.add_argument("branch", help = "branch to benchmark")
parser.add_argument("run_script", help = "location to write run script")
parser.add_argument("--overwrite", help = "overwrite output", action = "store_true")
parser.add_argument("--varmat", help = "use var_value<Matrix<T, R, C>> types", action = "store_true")
args = parser.parse_args()

build_cmd = "./build.sh " + args.branch + (" true" if args.varmat else "")
subprocess.run(build_cmd.split(" "))

commit = subprocess.check_output("git --git-dir=math/.git rev-parse --short HEAD".split(" ")).strip().decode("utf-8")
folder = "benchmarks/{0}_{1}_{2}".format(args.branch, commit, "varmat" if args.varmat else "matvar")

if not os.path.exists("benchmarks"):
    os.mkdir("benchmarks")

if not os.path.exists(folder):
    os.mkdir(folder)

with open(args.run_script, "w") as f:
    for benchmark in benchmarks:
        binary = "src/" + benchmark
        output = "{0}/{1}.csv".format(folder, benchmark)

        if os.path.exists(binary) and (args.overwrite or not os.path.exists(output)):
            f.write("{0} --benchmark_color=fase --benchmark_format=csv --benchmark_repetitions=30 --benchmark_min_time=0.1 > {1}{2}".format(binary, output, os.linesep))
