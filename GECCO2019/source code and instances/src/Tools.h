/*
 * Tools.h
 *
 *  Created on: Jun 28, 2018
 *      Author: earza
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>

using std::ifstream;
using std::istream;
using std::ofstream;
using std::ostream;
using namespace std;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

template <class T>
void print_variable(string variable_name, T variable, bool end_line = true)
{
	cout << variable_name << ": " << variable;

	if (end_line)
	{
		cout << endl;
	}
}

template <class T>
void print_vector(string vector_name, T *vector, int len)
{
	cout << vector_name << ": {";
	for (int i = 0; i < len; i++)
	{
		cout << vector[i];
		if (i != len - 1)
		{
			cout << ", ";
		}
	}
	cout << "}" << endl;
}

template <class T>
string toString(const T &t, bool *ok = NULL)
{
	ostringstream stream;
	stream << t;
	if (ok != NULL)
		*ok = (stream.fail() == false);
	return stream.str();
}

//https://stackoverflow.com/questions/3909272/sorting-two-corresponding-arrays
//sort 2 arrays simultaneously
template <class A, class B>
void QuickSort2Desc(A a[], B b[], int l, int r)
{
	int i = l;
	int j = r;
	A v = a[(l + r) / 2];
	do
	{
		while (a[i] > v)
			i++;
		while (v > a[j])
			j--;
		if (i <= j)
		{
			std::swap(a[i], a[j]);
			std::swap(b[i], b[j]);
			i++;
			j--;
		};
	} while (i <= j);
	if (l < j)
		QuickSort2Desc(a, b, l, j);
	if (i < r)
		QuickSort2Desc(a, b, i, r);
}


//sort 3 arrays simultaneously
template <class A, class B, class C>
void QuickSort3Desc(A a[], B b[], C c[], int l, int r)
{
	int i = l;
	int j = r;
	A v = a[(l + r) / 2];
	do
	{
		while (a[i] > v)
			i++;
		while (v > a[j])
			j--;
		if (i <= j)
		{
			std::swap(a[i], a[j]);
			std::swap(b[i], b[j]);
			std::swap(c[i], c[j]);

			i++;
			j--;
		};
	} while (i <= j);
	if (l < j)
		QuickSort3Desc(a, b, c, l, j);
	if (i < r)
		QuickSort3Desc(a, b, c, i, r);
}


template <class T>
void PrintMatrix(T **M, int m, int n)
{

	cout << "\n";
	for (int i = 0; i < m; i++)
	{
		cout << "( ";
		for (int j = 0; j < n; j++)
		{
			cout << M[i][j] << " ";
		}
		cout << ")\n";
	}
}

template <class T>
void PrintPythonArray(T *array, int len, bool end_line = true)
{

	if (len == 1)
	{
		cout << "[" << array[0] << "]";

		if (end_line)
		{
			cout << endl;
		}

		return;
	}

	cout << "[";
	for (int i = 0; i < len - 1; i++)
	{
		cout << array[i] << ", ";
	}
	cout << array[len - 1];
	cout << "]";
	if (end_line)
	{
		cout << endl;
	}
}

template <class T>
double Average(T *array, int len)
{

	double sum = 0;

	for (int i = 0; i < len; i++)
	{
		sum += array[i];
	}

	return (double)sum / len;
}

template <class T>
double Variance(T *array, int len)
{

	double mean = Average(array, len);

	double var = 0;
	for (int i = 0; i < len; i++)
	{
		var += (array[i] - mean) * (array[i] - mean);
	}

	return (double)var / len;
}

template <class T>
void copy_vector(T *v_from, T *v_to, int len)
{
	for (int i = 0; i < len; ++i)
	{
		v_to[i] = v_from[i];
	}
}

template <class T>
void copy_matrix(T **M_from, T **M_to, int m, int n)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			M_to[i][j] = M_from[i][j];
		}
	}
}

template <class T>
void delete_matrix(T **M, int m, int n)
{
	for (int i = 0; i < m; i++)
	{
		delete[] M[i];
	}
	delete[] M;
}

// static int calculate_hamming_distance(int *v1, int *v2, int len)
// {
// 	int dist = 0;
// 	for (int i = 0; i < len; ++i)
// 	{
// 		if (v1[i] != v2[i])
// 		{
// 			dist++;
// 		}
// 	}
// 	return dist;
// }

// returns true if they are equal, and returns false if they are not.
template <class T>
bool compare_vectors(T *vec1, T *vec2, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (vec1[i] != vec2[i])
		{
			return false;
		}
	}
	return true;
}

// writes in  bool *is_ith_position_repeated (true --> vector is a repetition, false--> vector is not a repetition)
template <class T>
void which_indexes_correspond_to_repeated_vectors(T **vec_array, int vec_len, int n_of_vecs, bool *is_ith_position_repeated, bool is_known_last_repeated_indexes)
{
	if (n_of_vecs == 1)
	{
		is_ith_position_repeated[0] = false;
		return;
	}
	else if (n_of_vecs == 2)
	{
		is_ith_position_repeated[0] = false;
		is_ith_position_repeated[1] = compare_vectors(vec_array[0], vec_array[1], vec_len);
		return;
	}

	is_ith_position_repeated[0] = false;
	is_ith_position_repeated[1] = compare_vectors(vec_array[0], vec_array[1], vec_len);

	for (int i = 2; i < n_of_vecs; i++)
	{
		if (is_known_last_repeated_indexes && not is_ith_position_repeated[i])
		{
			continue;
		}
		for (int j = i - 1; j >= 0; j--)
		{
			is_ith_position_repeated[i] = false;
			if (compare_vectors(vec_array[i], vec_array[j], vec_len))
			{
				is_ith_position_repeated[i] = true;
				break;
			}
		}
	}
}

template <class T>
void swap_two_positions(T *vec, int pos1, int pos2)
{
	T temp;
	temp = vec[pos1];
	vec[pos1] = vec[pos2];
	vec[pos2] = temp;
}


void shuffle_vector(int *vec, int len);


int random_integer_fast(int min, int max);

// https://ericlippert.com/2013/12/16/how-much-bias-is-introduced-by-the-remainder-technique/
int random_integer_uniform(int min, int max = 0);

// chooses a random integer from {0,1,2, range_max - 1}
int random_range_integer_uniform(int range_max);

double random_0_1_float();

double sigmoid(double x);

int chose_index_given_probabilities(double *probabilities_array, int max_index);

bool coin_toss(double p_of_true);

int tools_round(double x);

#endif
