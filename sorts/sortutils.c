/* C Program To Sort page in ascending order using Bubble Sort. */
#include <stdlib.h>
#include <string.h>
#include "fileutils.h"
#include "sortutils.h"

void swap(str *a, str *b) { str tmp = *a; *a = *b; *b = tmp; }

void bubble_sort(str array[], int size)
{
	for (int i = 0; i < size - 1; ++i) {
		for (int j = 0; j < size - i - 1; ++j) {
			if (strcmp(array[j], array[j + 1]) > 0) {
				swap(&array[j], &array[j + 1]);
			}
		}
	}
}

void selection_sort(str array[], int size)
{
	int min;

	for (int i = 0; i < size; i++) {
		min = i;
		for (int j = i; j < size; j++) {
			if (strcmp(array[min], array[j]) > 0)
				min = j;
		}
		swap(&array[i], &array[min]);
	}
}

void insertion_sort(str array[], int size)
{
	int i, j;

	for (i = 1; i < size; i++) {
		str tmp = array[i];
		j = i;
		while (j && (strcmp(array[j - 1], tmp) > 0)) {
			array[j] = array[j - 1];
			j--;
		}
		array[j] = tmp;
	}
}

void shell_sort(str array[], int size)
{
	int i, j, k;

	for (i = size / 2; i > 0; i /= 2) {
		for (j = i; j < size; j++) {
			for (k = j - i; k >= 0; k = k - i) {
				if (strcmp(array[k+i], array[k]) > 0)
					break;
				 else
					swap(&array[k], &array[k+i]);
			}
		}
	}
}

void merge(str array[], str left[], str right[], int size_l, int size_r)
{
	int i = 0, j = 0, k = 0;

	while (i < size_l && j < size_r) {
		if (strcmp(left[i], right[j]) < 0)
			array[k++] = left[i++];
		else
			array[k++] = right[j++];
	}

	while (i < size_l)
		array[k++] = left[i++];

	while (j < size_r)
		array[k++] = right[j++];
}

void merge_sort(str array[], int size)
{
	if (size <= 1) return; // nothing to sort

	// divide
	int i = size / 2;
	int j = size - i;

	// dynamic
	str left[i];
	str right[j];

	memcpy(left, array, i * sizeof(str));
	memcpy(right, array + i, j * sizeof(str));

	// sort left and right
	merge_sort(left, i);
	merge_sort(right, j);

	merge(array, left, right, i, j);
}
