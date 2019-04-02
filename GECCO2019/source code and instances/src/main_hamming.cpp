#include <assert.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "Parameters.h"

#include "KEDAMM.h"
#include "Population.h"
#include "Tools.h"
#include "SimpleLog.h"
#include "Parameters.h"
using namespace std;

Population *eda;
Parameters *param;

#define HOW_MANY_MAX_PRINT_STATUS 100000

int main(int argc, char *argv[])
{
  int best_fitness_eda = 0;
  int n_evals_eda = 0;

  param = new Parameters(argc, argv);
  eda = new KEDAMM(param);

  param->print_parameters();



  // int print_every;
  // if (param->MAX_ITERATIONS <= HOW_MANY_MAX_PRINT_STATUS)
  // {
  //   print_every = 1;
  // }
  // else
  // {
  //   print_every = tools_round((double)param->MAX_ITERATIONS / (double)HOW_MANY_MAX_PRINT_STATUS);
  // }

  int purge_every = tools_round((double)param->MAX_ITERATIONS / (double)100);

  for (int i = 0; i < param->MAX_ITERATIONS * 1000; ++i)
  {
    if (param->DEBUG)
    {
      print_variable("i", i, false);
      cout << " | " << eda->get_best_fitness() << endl;

    }
    eda->learn();

    if (eda->ker_finished)
    {
      //cout << i << ", ";
      //cout << eda->ker_percentage_done << ": ";
      //cout << eda->get_best_fitness() << endl;

      best_fitness_eda = eda->get_best_fitness();
      n_evals_eda = eda->get_fitness_evals();
      break;
    }

    if (i % purge_every == 0) // purging hast to be done only after the learning phase, else, some individuals may have the wrong fitness value
    {
      // cout << i << ": " << eda->ker_percentage_done << " | ";
      eda->purge_repeated();
    }

    eda->sample();

    // if (i % print_every == 0)
    // {
    //   cout << i << ", ";
    //   cout << eda->ker_percentage_done << ": ";
    //   cout << eda->get_best_fitness() << endl;
    //   best_fitness_eda = eda->get_best_fitness();
    // }
  }

  eda->local_search();

  //print out the results [n_evals_eda, score_eda, n_evals_local, score_local]
  cout << "[";
  cout << n_evals_eda << ", ";
  cout << best_fitness_eda << ", ";
  cout << eda->get_fitness_evals() << ", ";
  cout << eda->get_best_fitness() << "]" << endl;

  PrintPythonArray(eda->best_permutation, eda->n);

  // // Write result summary on results.txt
  // SimpleLog result_writer("results.txt", false);
  // result_writer.write(best_fitness_eda, false);
  // result_writer.write(" , ", false);
  // result_writer.write(eda->best_fitness, false);
  // result_writer.write(" , ", false);
  // result_writer.write(param->return_parameter_string(), false);
  exit(0);
}
