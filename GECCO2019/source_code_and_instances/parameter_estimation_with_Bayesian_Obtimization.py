import os
from multiprocessing import Pool
from bayes_opt import BayesianOptimization
from bayes_opt import UtilityFunction
from statistics import mean, median
import matplotlib.pyplot as plt
import numpy as np

from skopt.space import Real, Integer
from skopt.utils import use_named_args

from skopt import gp_minimize

from numpy import argmax


from skopt.plots import plot_objective, plot_evaluations


INSTANCE_PATH = 'PATH_TO_QAPINSTANCES_FOLDER/QAPinstances/tai175e01.dat'
BINARY_PATH = ' PATH_TO_BINARY/binary '


# $POPSIZE \
# $MAX_ITERATIONS \
# $SEED \
# $START_MINIMUN_THETA \
# $TARGET_MID_EXPECTATION_PERCENTAGE \
# $FINAL_EXPECTATION \
# $T_ADD \
# $FAST_INCREASE_EACH_ITERATION_COUNTS_AS \
# $f\
# < $f >> "out_part"$thread".txt"


def obtain_result(file_name):

    with open(file_name, 'r') as f:
        doc = list()
        for line in f:
            doc.append(line.strip())

    return eval(doc[-2].split(",")[-1].strip("[]"))


# cmd = BINARY_PATH + '  250 50 2 0.25 0.25 0.1 20 3 tai75 <  ' + \
#     INSTANCE_PATH + ' > result.txt'

# os.system(cmd)  # returns the exit status


def _prep_cmd(POPSIZE, MAX_ITERATIONS, TARGET_MID_EXPECTATION_PERCENTAGE, FINAL_EXPECTATION, T_ADD, FAST_INCREASE_EACH_ITERATION_COUNTS_AS, EXPONENT, SEED):

    cmd = ''
    cmd += BINARY_PATH
    cmd += str(POPSIZE) + ' '
    cmd += str(MAX_ITERATIONS) + ' '
    cmd += str(SEED) + ' '
    cmd += '0.25 '
    cmd += str(TARGET_MID_EXPECTATION_PERCENTAGE) + ' '
    cmd += str(FINAL_EXPECTATION) + ' '
    cmd += str(T_ADD) + ' '
    cmd += str(FAST_INCREASE_EACH_ITERATION_COUNTS_AS) + ' '
    cmd += str(EXPONENT) + ' '
    cmd += 'tai75 < '
    cmd += INSTANCE_PATH
    cmd += ' > workbench/result'+str(SEED)+'.txt'

    return cmd



def black_box_f_individual(arg_list):


    POPSIZE, MAX_ITERATIONS, TARGET_MID_EXPECTATION_PERCENTAGE, FINAL_EXPECTATION, T_ADD, FAST_INCREASE_EACH_ITERATION_COUNTS_AS, EXPONENT, SEED = arg_list



    POPSIZE_INT = int(POPSIZE)
    MAX_ITERATIONS_INT = int(MAX_ITERATIONS)
    T_ADD_INT = int(T_ADD)
    FAST_INCREASE_EACH_ITERATION_COUNTS_AS_INT = int(FAST_INCREASE_EACH_ITERATION_COUNTS_AS)

    cmd = _prep_cmd(POPSIZE_INT, MAX_ITERATIONS_INT, TARGET_MID_EXPECTATION_PERCENTAGE,
                    FINAL_EXPECTATION, T_ADD_INT, FAST_INCREASE_EACH_ITERATION_COUNTS_AS_INT, EXPONENT, SEED)

    #print(cmd)
    
    os.system(cmd)  # returns the exit status
    
    result = obtain_result("workbench/result" + str(SEED) +".txt")
    #print(result)

    try:	
        os.system("rm workbench/result" + str(SEED) +".txt")
    except:
       pass

    return result


def black_box_function( TARGET_MID_EXPECTATION_PERCENTAGE, FINAL_EXPECTATION, EXPONENT, FAST_INCREASE_EACH_ITERATION_COUNTS_AS = 10, T_ADD = 20, POPSIZE = 20000, MAX_ITERATIONS = 1250): # FINAL_EXPECTATION, T_ADD, FAST_INCREASE_EACH_ITERATION_COUNTS_AS):

    results = list()
    N_OF_EVALS = 28

    r = np.random.randint(1000)

    args = [[POPSIZE, MAX_ITERATIONS, TARGET_MID_EXPECTATION_PERCENTAGE, FINAL_EXPECTATION, T_ADD, FAST_INCREASE_EACH_ITERATION_COUNTS_AS, EXPONENT ,i] for i in range(2 + r, 2 + N_OF_EVALS + r)]


    pool = Pool(processes=7)            
    results = pool.map(black_box_f_individual, args)
    pool.terminate()


    print("best: ",max(results), " | average: ", mean(results), args[0])
    if max(results) > 0:
        raise ValueError('The maximun of all values found to be higher than 0. The argmax is: ', argmax(results))


    # return  (-1.0 * mean(sorted(results[2:-2])))
    return  (-1.0 * median(results))




if __name__ == '__main__':


    # The list of hyper-parameters we want to optimize.

    space  =  [
              # Integer(2500, 2501, name='POPSIZE'),
              # Integer(500, 501, name='MAX_ITERATIONS'),
              Real(0.1, 0.85, name='TARGET_MID_EXPECTATION_PERCENTAGE'),
              Real(0.1, 10.0, "log-uniform", name='FINAL_EXPECTATION'), 
              # Integer(2, 30, name='T_ADD'),
              # Integer(2, 20, name='FAST_INCREASE_EACH_ITERATION_COUNTS_AS'),
              Real(0.1, 8.0, "log-uniform", name='EXPONENT')
              ]

    # tai75 [mid=0.35, FIN=8.0, EXPO=0.45]


    # estimate variance
    # space  = [Integer(15000, 15001, name='POPSIZE'),
    #           Integer(700, 701, name='MAX_ITERATIONS'),
    #           Real(0.7, 0.71, name='TARGET_MID_EXPECTATION_PERCENTAGE'),
    #           Real(5.0, 5.1, name='FINAL_EXPECTATION'),
    #           Integer(10, 11, name='T_ADD'),
    #           Integer(25, 26, name='FAST_INCREASE_EACH_ITERATION_COUNTS_AS'),
    #           ]


    @use_named_args(space)
    def objective(**params):

        return black_box_function(**params)


   

    # this decorator allows your objective function to receive a the parameters as
    # keyword arguments. This is particularly convenient when you want to set scikit-learn
    # estimator parameters

    res_gp = gp_minimize(objective,
                         space,
                         n_random_starts=60,
                         n_calls=80,
                         random_state=0,
                         verbose=True,
                         noise=25000,) # nevals = 17 & discard first 2 and last 2
                         #noise=22843054.0, )

    _ = plot_objective(res_gp)
    plt.savefig("bo_heatmap.png", figsize=(20,10))
    _ = plot_evaluations(res_gp)
    plt.savefig("bo_sampled_points.png", figsize=(20,10))
    
    print(res_gp.x)





    
