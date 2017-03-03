/**
 * Algorithm Engineering: HW 3, by Steven Kneiser
 *
 * Last Modified on Feb 28, 2017
 */
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <set>
#include <unordered_set>
#include <vector>

static float temp;
#define SWAP(a, b) temp=(a);(a)=(b);(b)=temp;
#define HYBRID_CONSTANT 35

void part1(int, int);
void part2(int, int);

//clock_t _clockrun(void*, float*, int, ...);
void _rfill(float*, int);
void _hsort(float*, int);
void _isort(float*, int);
void _qsort(float*, int, int);
int _partition(float*, int, int);


/**
 * Core Business Logic
 */
int main(int argc, char **argv) {
	srand((unsigned int)time(NULL));

	if (argc != 3) {
		printf("ERROR: Expected N & K  ...got %d arguments.\n", argc - 1);
		exit(EXIT_FAILURE);
	}

	int n = atoi(argv[1]); // Size of arrays
	int k = atoi(argv[2]); // Number of test cases

	part1(n, k);
	printf("\n");
	part2(n, k);

	return EXIT_SUCCESS;
}


/**
 * Part 1
 *
 * Solve problems 1 & 2
 */
void part1(int n, int k) {

	float *arr = (float *)calloc((size_t)n, sizeof(float));
	float *brr = (float *)calloc((size_t)n, sizeof(float));
	float *crr = (float *)calloc((size_t)n, sizeof(float));

	for (int test = 0; test < k; test++) {

		// Populate arrays
		_rfill(arr, n);
		for (int i = 0; i < n; i++) {
			crr[i] = brr[i] = arr[i];
		}

		// Time: insertion sort
		clock_t isort_start = clock();
		_isort(arr, n);
		clock_t isort_time = (clock() - isort_start) * 1000000 / CLOCKS_PER_SEC;

		// Time: quicksort
		clock_t qsort_start = clock();
		_qsort(brr, 0, n);
		clock_t qsort_time = (clock() - qsort_start) * 1000000 / CLOCKS_PER_SEC;

		// Time: hybird sort
		clock_t hsort_start = clock();
		_hsort(crr, n);
		clock_t hsort_time = (clock() - hsort_start) * 1000000 / CLOCKS_PER_SEC;

		/*
		for (int i = 0; i < n; i++) {
			if (i < 20) {
				printf("i%f ", arr[i]);
				printf("q%f ", brr[i]);
			}
		}
		printf("\n");
		*/

		// Record results
		printf("Insertion sort: %lu microseconds\n", isort_time);
		printf("     Quicksort: %lu microseconds\n", qsort_time);
		printf("   Hybird sort: %lu microseconds\n", hsort_time);
		printf("===============\n");
	}

	free(arr);
	free(brr);
	free(crr);
}


/**
 * Part 2
 *
 * Solve problems 3 & 4
 */
void part2(int n, int k) {

	std::vector<float> vec;
	std::multiset<float> tree;
	std::unordered_multiset<float> table;
	clock_t tree_time, tree_start, table_time, table_start, vec_time, vec_start;
	float temp;

	for (int test = 0; test < k; test++) {
		tree_time = table_time = vec_time = (clock_t)0;

		for (int i = 0; i < n; i++) {
			temp = rand() / RAND_MAX;

			tree_start = clock();
			tree.insert(temp);
			tree_time += clock() - tree_start;

			table_start = clock();
			table.insert(temp);
			table_time += clock() - table_start;

			vec_start = clock();
			vec.insert(std::upper_bound(vec.begin(), vec.end(), temp), temp);
			vec_time += clock() - vec_start;
		}

		// Record results
		printf("==================================\n");
		printf("unordered_multiset insertion time: %lu microseconds\n", tree_time);
		printf("          multiset insertion time: %lu microseconds\n", table_time);
		printf("     sorted-vector insertion time: %lu microseconds\n", vec_time);
	}
}


/**
 * Random fill
 *
 * Populate a given array of floats with random positive values
 */
void _rfill(float *arr, int n) {
	for (int i = 0; i < n; i++) {
		arr[i] = (float)rand() / RAND_MAX;
	}
}


/**
 * Clock function run-time
 *
 * Return the runtime of a given sort function
 *
clock_t _clockrun(void *func, float *arr, int n, ...) {

	va_list valist;
	va_start(valist, n);

	clock_t func_start = clock();
	(*func)(arr, a, ...);
	clock_t func_time = clocl() - func_start;

	return clock() - func_start;

}
*/


/**
 * Hybird sort
 *
 * Artisanly crafted with the sophistication of yours truly
 */
void _hsort(float *arr, int n) {
	(n < HYBRID_CONSTANT) ? _isort(arr, n) : _qsort(arr, 0, n);
}


/**
 * Insertion sort
 *
 * Inspired by:
 *   Numerical Recipes in C (pg. 330)
 *
 * Updated for correctness with pseudocode from:
 *   https://en.wikipedia.org/wiki/Insertion_sort#List_insertion_sort_code_in_C
 */
void _isort(float *arr, int n) {
	int i, j;
	float key;

	for (i = 1; i < n; i++) {
		key = arr[i];
		j = i - 1;

		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j -= 1;
		}
		arr[j + 1] = key;
	}
}


/**
 * Quick sort
 *
 * Inspired by:
 *   http://www.comp.dit.ie/rlawlor/Alg_DS/sorting/quickSort.c
 */
void _qsort(float *arr, int left, int right) {
	if (left < right) {
		int j = _partition(arr, left, right);

		_qsort(arr, left, j - 1);
		_qsort(arr, j + 1, right);
	}
}


/**
 * Quick sort's partition
 *
 * Inspired by:
 *   http://www.comp.dit.ie/rlawlor/Alg_DS/sorting/quickSort.c
 */
int _partition(float *arr, int left, int right) {
   float pivot = arr[left];
   int i = left;
   int j = right + 1;

   while (1) {
   		do ++i; while(arr[i] <= pivot && i <= right);
   		do --j; while(arr[j] > pivot);

   		if(i >= j) break;
		SWAP(arr[i], arr[j]);
   }

   SWAP(arr[left], arr[j]);
   return j;
}
