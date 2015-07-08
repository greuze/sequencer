#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_SIZE 20
#define VALUES 4
#define DELAY 3

enum checkValues {
	WRONG, OK
};

typedef enum checkValues check;

// Returns a value between 0 and values-1
int constrainedRandom(int values) {
	return rand() % values;
}

void clearScreen(int delay) {
	sleep(delay);
	int i;
	for (i = 0; i < 50; i++) {
		printf("\n");
	}
}

void printSolution(int arr[], int level) {
	int i;
	for (i = 0; i < level; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void printNextLevel(int arr[], int level) {
	arr[level - 1] = constrainedRandom(VALUES);

	printf("Sequence for level %d is:\n", level);
	printSolution(arr, level);

	clearScreen(DELAY);
}

check checkLevel(int arr[], int level) {
	printf("Enter the %d digits sequence: ", level);

	int i, userInput;
	for (i = 0; i < level; i++) {
		// Read only one digit
		scanf("%1d", &userInput);
		if (arr[i] != userInput) {
			printf("Position %d was %d, not %d\nSolution was ", i + 1, arr[i],
					userInput);
			printSolution(arr, level);
			return WRONG;
		}
	}
	// Purge input
	scanf("%*[^\n]");

	clearScreen(0);

	return OK;
}

int main(void) {
	int arr[MAX_SIZE];
	int level = 1;

	// Ensure the seed is different in every execution
	srand(time(NULL));

	do {
		printNextLevel(arr, level);

		int check = checkLevel(arr, level);

		if (check == WRONG) {
			printf("You lost in level %d\n", level);
			return EXIT_FAILURE;
		}
		level++;
	} while (level <= MAX_SIZE);

	printf("You won after level level %d\n", level - 1);
	return EXIT_SUCCESS;
}
