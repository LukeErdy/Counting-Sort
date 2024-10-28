#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINTING 1
char skills[5][32] = {"SKILL_BREAKDANCING", "SKILL_APICULTURE", "SKILL_BASKET", "SKILL_XBASKET", "SKILL_SWORD"};

int* readScores(int* count) {
	int capacity = 128;
	int* array = malloc(capacity * sizeof(int));
	*count = 0;
	while (fscanf(stdin, "%d", &array[*count]) == 1) {
		(*count)++;
		if (*count >= capacity) {
			capacity *= 2;
			array = realloc(array, capacity * sizeof(int));
		}
	}
	return array;
}

// compare function for qsort()
int compare(const void* a, const void* b) {
	int c = *((int*)a);
	int d = *((int*)b);
	if (c > d) return -1;
	if (c < d) return 1;
	return 0;
}

void custom(int** array, int count, int length) {
	int* under_10k = calloc(10000, sizeof(int));
	int* not_under_10k = malloc((count / 5) * sizeof(int));
	int index = 0; // index to keep track of length of not_under_10k
	for (int i = 0; i < length; i++) {
		if ((*array)[i] < 10000) under_10k[(*array)[i]] += 1;
		else {
			not_under_10k[index] = (*array)[i];
			index++;
		}
	}
	// Quicksort for array of larger values
	if (index > 1) {
		int stack[index];
		int top = -1, start = 0, end = index - 1;
		stack[++top] = start;
		stack[++top] = end;
		while (top >= 0) {
			end = stack[top--];
			start = stack[top--];
			int pivot = not_under_10k[end];
			int i = start - 1;
			for (int j = start; j <= end - 1; j++) {
				if (not_under_10k[j] > pivot) {
					i++;
					int temp = not_under_10k[i];
					not_under_10k[i] = not_under_10k[j];
					not_under_10k[j] = temp;
				}
			}
			int temp = not_under_10k[i + 1];
			not_under_10k[i + 1] = not_under_10k[end];
			not_under_10k[end] = temp;
			int pivot_index = i + 1;
			if (pivot_index - 1 > start) {
				stack[++top] = start;
				stack[++top] = pivot_index - 1;
			}
			if (pivot_index + 1 < end) {
				stack[++top] = pivot_index + 1;
				stack[++top] = end;
			}
		}
	}
	for (int i = 9999; i > -1; i--) {
		for (int j = 0; j < under_10k[i]; j++) {
			not_under_10k[index] = i;
			index++;
		}
	}
	*array = not_under_10k;
}

int main(int argc, char** argv) {
	int count;
	int* array = readScores(&count);
	int* totals = calloc(count / 5, sizeof(int));
	for (int i = 0; i < 5; i++) {
		if (PRINTING) printf("%s\n", skills[i]);
		int* values = malloc((count / 5) * sizeof(int)); // array for values pertaining to the current skill
		int index = 0; // keep track of where we are in the above array
		for (int j = i; j < count; j += 5) {
			values[index] = array[j];
			totals[index] += array[j];
			index++;
		}
		if (!strcmp(argv[1], "custom")) custom(&values, count, index);
		else if (!strcmp(argv[1], "standard")) qsort(values, index, sizeof(int), compare);
		if (PRINTING) {
			for (int k = 0; k < index; k++) {
				printf("%d\n", values[k]);
			}
			printf("\n");
		}
		free(values);
	}
	if (!strcmp(argv[1], "custom")) custom(&totals, count, count / 5);
	else if (!strcmp(argv[1], "standard")) qsort(totals, count / 5, sizeof(int), compare);
	if (PRINTING) {
		printf("TOTAL_XP\n");
		for (int l = 0; l < count / 5; l++) printf("%d\n", totals[l]); // EXTRA \n may cause issues (CHECK THIS)
		
	}
	return 0;
}