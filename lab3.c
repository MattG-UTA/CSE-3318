// Matthew Gutierrez
// 1002333697
// Lab 3

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int reachable;
    int prevA;
    int prevB;
    char choice;
} State;

static int state_index(int step, int sumA, int sumB, int target) {
    int width = target + 1;
    return step * width * width + sumA * width + sumB;
}

static void print_sequence(const int *values, int count) {
    int i;

    printf("Sequence:");
    for (i = 0; i < count; i++) {
        printf(" %d", values[i]);
    }
    printf("\n");
}

static void print_dp_table(const State *states, int count, int target) {
    int step;
    int sumA;
    int sumB;

    printf("Dynamic programming table:\n");
    for (step = 0; step <= count; step++) {
        printf("i = %d\n", step);
        printf("    ");
        for (sumB = 0; sumB <= target; sumB++) {
            printf("%3d", sumB);
        }
        printf("\n");

        for (sumA = 0; sumA <= target; sumA++) {
            printf("%3d", sumA);
            for (sumB = 0; sumB <= target; sumB++) {
                int idx = state_index(step, sumA, sumB, target);
                printf("%3c", states[idx].reachable ? '1' : '.');
            }
            printf("\n");
        }
    }
}

static void print_solution(const int *values, const int *groups, int count) {
    int i;

    printf("  i    0    1    2\n");
    for (i = 0; i < count; i++) {
        printf("%3d", i + 1);

        if (groups[i] == 0) {
            printf("%5d", values[i]);
        } else {
            printf("%5s", "");
        }

        if (groups[i] == 1) {
            printf("%5d", values[i]);
        } else {
            printf("%5s", "");
        }

        if (groups[i] == 2) {
            printf("%5d", values[i]);
        } else {
            printf("%5s", "");
        }

        printf("\n");
    }
}

int main(void) {
    int capacity = 16;
    int count = 0;
    int *raw = (int *)malloc(capacity * sizeof(int));
    int *values = NULL;
    int *groups = NULL;
    State *states = NULL;
    int i;
    int total = 0;
    int target;
    int effectiveCount;

    if (raw == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    while (1) {
        int nextValue;

        if (scanf("%d", &nextValue) != 1) {
            break;
        }

        if (count == capacity) {
            int *grown;
            capacity *= 2;
            grown = (int *)realloc(raw, capacity * sizeof(int));
            if (grown == NULL) {
                printf("Memory allocation failed.\n");
                free(raw);
                return 1;
            }
            raw = grown;
        }

        raw[count++] = nextValue;
    }

    if (count == 0) {
        printf("No input values were provided.\n");
        free(raw);
        return 1;
    }

    if (count == raw[0] + 1) {
        effectiveCount = raw[0];
        values = (int *)malloc(effectiveCount * sizeof(int));
        if (values == NULL) {
            printf("Memory allocation failed.\n");
            free(raw);
            return 1;
        }
        for (i = 0; i < effectiveCount; i++) {
            values[i] = raw[i + 1];
        }
    } else {
        effectiveCount = count;
        values = (int *)malloc(effectiveCount * sizeof(int));
        if (values == NULL) {
            printf("Memory allocation failed.\n");
            free(raw);
            return 1;
        }
        for (i = 0; i < effectiveCount; i++) {
            values[i] = raw[i];
        }
    }

    free(raw);

    for (i = 0; i < effectiveCount; i++) {
        if (values[i] <= 0) {
            print_sequence(values, effectiveCount);
            printf("Error: all sequence values must be positive integers.\n");
            free(values);
            return 1;
        }
        total += values[i];
    }

    print_sequence(values, effectiveCount);

    if (total % 3 != 0) {
        printf("Error: total sum %d is not divisible by 3.\n", total);
        free(values);
        return 1;
    }

    target = total / 3;
    groups = (int *)malloc(effectiveCount * sizeof(int));
    if (groups == NULL) {
        printf("Memory allocation failed.\n");
        free(values);
        return 1;
    }

    {
        int width = target + 1;
        int tableSize = (effectiveCount + 1) * width * width;
        int prefix = 0;
        states = (State *)calloc((size_t)tableSize, sizeof(State));
        if (states == NULL) {
            printf("Memory allocation failed.\n");
            free(values);
            free(groups);
            return 1;
        }

        states[state_index(0, 0, 0, target)].reachable = 1;

        for (i = 0; i < effectiveCount; i++) {
            int sumA;
            int sumB;
            prefix += values[i];

            for (sumA = 0; sumA <= target; sumA++) {
                for (sumB = 0; sumB <= target; sumB++) {
                    int currentIdx = state_index(i, sumA, sumB, target);

                    if (!states[currentIdx].reachable) {
                        continue;
                    }

                    if (!states[state_index(i + 1, sumA, sumB, target)].reachable &&
                        prefix - sumA - sumB <= target) {
                        State *next = &states[state_index(i + 1, sumA, sumB, target)];
                        next->reachable = 1;
                        next->prevA = sumA;
                        next->prevB = sumB;
                        next->choice = 2;
                    }

                    if (sumA + values[i] <= target &&
                        !states[state_index(i + 1, sumA + values[i], sumB, target)].reachable) {
                        State *next = &states[state_index(i + 1, sumA + values[i], sumB, target)];
                        next->reachable = 1;
                        next->prevA = sumA;
                        next->prevB = sumB;
                        next->choice = 0;
                    }

                    if (sumB + values[i] <= target &&
                        !states[state_index(i + 1, sumA, sumB + values[i], target)].reachable) {
                        State *next = &states[state_index(i + 1, sumA, sumB + values[i], target)];
                        next->reachable = 1;
                        next->prevA = sumA;
                        next->prevB = sumB;
                        next->choice = 1;
                    }
                }
            }
        }
    }

    if (target < 10) {
        print_dp_table(states, effectiveCount, target);
    }

    if (!states[state_index(effectiveCount, target, target, target)].reachable) {
        printf("Error: no equal-sum 3-way partition exists.\n");
        free(values);
        free(groups);
        free(states);
        return 1;
    }

    {
        int sumA = target;
        int sumB = target;

        for (i = effectiveCount; i > 0; i--) {
            State current = states[state_index(i, sumA, sumB, target)];
            groups[i - 1] = current.choice;
            sumA = current.prevA;
            sumB = current.prevB;
        }
    }

    print_solution(values, groups, effectiveCount);

    free(values);
    free(groups);
    free(states);
    return 0;
}
