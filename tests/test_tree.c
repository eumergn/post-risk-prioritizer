#include "../include/tree.h"
#include <assert.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

void test_create_empty_tree() {
    struct Tree *tree = create_empty_tree(10);
    assert(tree != NULL);
    assert(tree->current_nbr_data == 0);
    assert(tree->root == NULL);
    printf("Test create_empty_tree: OK\n");
    free_tree(tree);
}

void basic_insert_and_find() {
    struct Tree *tree = create_empty_tree(10);
    insert(tree, 5, 100); // value, id 
    insert(tree, 10, 101);
    insert(tree, 3, 102);

    struct Node *min = find_min(tree);
    struct Node *max = find_max(tree);

    assert(min->risk_level == 3);
    assert(max->risk_level == 10);
    printf("Test basic_and_find: OK\n");

    for (int i = 0; i < 3; i++) {
        delete_min(tree);
    }

    free_tree(tree);
}

void delete_min_test() {
    struct Tree *tree = create_empty_tree(10);

    int min = 0;
    int risk_level = 0;

    // Insert random values
    for (int i = 0; i < 10; i++) {
        risk_level = rand() % 10;
        insert(tree, risk_level, i);

        if (i == 0) {
            min = risk_level;
        } else {
            if (risk_level < min) {
                min = risk_level;
            }
        }
    }

    // Delete and check minimum values
    for (int i = 0; i < 10; i++) {
        struct Node *current_min = find_min(tree);
        assert(current_min->risk_level == min);
        delete_min(tree);

        // Find the new minimum
        struct Node *new_min = find_min(tree);
        if (new_min != NULL) {
            min = new_min->risk_level;
        }
    }

    printf("Test delete_min_test: OK\n");
    free_tree(tree);
}

void delete_max_test() {
    struct Tree *tree = create_empty_tree(10);

    int max = 0;
    int risk_level = 0;

    // Insert random values
    for (int i = 0; i < 10; i++) {
        risk_level = rand() % 10;
        insert(tree, risk_level, i);

        if (i == 0) {
            max = risk_level;
        } else {
            if (risk_level > max) {
                max = risk_level;
            }
        }
    }

    // Delete and check maximum values
    for (int i = 0; i < 10; i++) {
        struct Node *current_max = find_max(tree);
        assert(current_max->risk_level == max);
        delete_max(tree);

        // Find the new maximum
        struct Node *new_max = find_max(tree);
        if (new_max != NULL) {
            max = new_max->risk_level;
        }
    }

    printf("Test delete_max_test: OK\n");
    free_tree(tree);
}

void right_min_test() {
    struct Tree *tree = create_empty_tree(10);

    int min = 0;
    int risk_level = 0;

    // Insert random values
    for (int i = 0; i < 10; i++) {
        risk_level = rand() % 10;
        insert(tree, risk_level, i);

        if (i == 0) {
            min = risk_level;
        } else {
            if (risk_level < min) {
                min = risk_level;
            }
        }
    }

    struct Node* temp = tree->root;
    while (temp->left_child)
    {
        temp = temp->left_child;
    }
    assert(temp->risk_level == min);

    printf("Test right_min_test: OK\n");
    free_tree(tree);
}

void balance_test() {
    struct Tree *tree = create_empty_tree(100);

    // Insert values
    for (int i = 0; i < 100; i++) {
        insert(tree, i, i);
    }

    // Check if the tree is balanced
    assert(height_diff(tree->root) >= -1 && height_diff(tree->root) <= 1);

    // Delete some values
    for (int i = 0; i < 50; i++) {
        delete_min(tree);
    }

    // Check if the tree is still balanced
    assert(height_diff(tree->root) >= -1 && height_diff(tree->root) <= 1);

    printf("Test balance_test: OK\n");
    free_tree(tree);
}

void stress_test() {
    struct Tree *tree = create_empty_tree(1000000);
    
    // Insert 1 million elements
    for (int i = 0; i < 1000000; i++) {
        insert(tree, rand() % 1000000, i);
    }

    // Vérifier que l'arbre est équilibré après les insertions
    assert(height_diff(tree->root) >= -1 && height_diff(tree->root) <= 1);

    // Vérifier que le nombre d'éléments est correct
    assert(tree->current_nbr_data == 1000000);

    // Delete 1 million elements
    for (int i = 0; i < 1000000; i++) {
        delete_min(tree);
    }

    // Check if the tree is empty
    assert(tree->current_nbr_data == 0);
    assert(tree->root == NULL);

    printf("Test stress_test: OK\n");

    free_tree(tree);
}

// Custom stress test
void custom_stress_test(int num_elements, int max_data) {
    struct Tree *tree = create_empty_tree(max_data);
    
    // Insert num_elements elements
    for (int i = 0; i < num_elements; i++) {
        insert(tree, rand() % max_data, i);
    }

    // check if the tree is balanced after insertions
    assert(height_diff(tree->root) >= -1 && height_diff(tree->root) <= 1);

    // Check if the number of elements is correct
    assert(tree->current_nbr_data == max_data);

    // Delete num_elements elements
    for (int i = 0; i < num_elements; i++) {
        delete_min(tree);
    }

    // Check if the tree is empty
    assert(tree->current_nbr_data == 0);
    assert(tree->root == NULL);

    printf("Test custom_stress_test: OK\n");

    free_tree(tree);
}


// For insert time measurements
void measure_individual_insert_time(struct Tree *tree, int start_elements, int num_elements, int num_trials, int *elements_list, double *time_list, int index) {
    struct timespec start, end;
    double total_time = 0.0;

    // Insérer les valeurs supplémentaires pour atteindre num_elements
    for (int i = start_elements; i < num_elements; i++) {
        insert(tree, rand() % num_elements, i);
    }

    // Mesurer les insertions individuelles
    for (int i = 0; i < num_trials; i++) {
        int value = rand() % num_elements;

        clock_gettime(CLOCK_MONOTONIC, &start);
        insert(tree, value, num_elements + i);
        clock_gettime(CLOCK_MONOTONIC, &end);

        total_time += ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) / 1e3; //En microsecondes

        // Supprimer immédiatement l'élément inséré pour maintenir un état stable
        delete_min(tree);
    }

    double average_time = total_time / num_trials;

    elements_list[index] = num_elements;
    time_list[index] = average_time;

    printf("Elements: %d, Average Insert Time: %f µs\n", num_elements, average_time);
}

// For delete time measurements
void measure_individual_delete_time(struct Tree *tree, int start_elements, int num_elements, int num_trials, int *elements_list, double *time_list, int index) {
    struct timespec start, end;
    double total_time = 0.0;

    // Insérer les valeurs supplémentaires pour atteindre num_elements
    for (int i = start_elements; i < num_elements; i++) {
        insert(tree, rand() % num_elements, i);
    }

    // Mesurer les suppressions individuelles
    for (int i = 0; i < num_trials; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        delete_min(tree);
        clock_gettime(CLOCK_MONOTONIC, &end);

        total_time += ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec)) / 1e3; // En microsecondes

        // Réinsérer un élément pour maintenir un état stable
        insert(tree, rand() % num_elements, num_elements + i);
    }

    double average_time = total_time / num_trials;

    elements_list[index] = num_elements;
    time_list[index] = average_time;

    printf("Elements: %d, Average Delete Time: %f µs\n", num_elements, average_time);
}

void measure_insert_times() {
    int sizes[] = {10, 20, 30, 40, 50, 75, 100, 200, 300, 400, 500, 750, 1000, 2000, 3000, 4000, 5000, 7500, 10000, 20000, 30000, 40000, 50000, 75000, 100000, 150000, 200000, 250000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000, 1500000, 2000000, 2500000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000, 10000000, 15000000, 20000000, 25000000, 30000000, 40000000, 50000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_trials = 100000;

    int elements_list[num_sizes];
    double time_list[num_sizes];

    struct Tree *tree = create_empty_tree(sizes[num_sizes - 1]);

    for (int i = 0; i < num_sizes; i++) {
        int start_elements = (i == 0) ? 0 : sizes[i - 1];
        measure_individual_insert_time(tree, start_elements, sizes[i], num_trials, elements_list, time_list, i);
    }

    free_tree(tree);
}

void measure_delete_times() {
    int sizes[] = {10, 20, 30, 40, 50, 75, 100, 200, 300, 400, 500, 750, 1000, 2000, 3000, 4000, 5000, 7500, 10000, 20000, 30000, 40000, 50000, 75000, 100000, 150000, 200000, 250000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000, 1500000, 2000000, 2500000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000, 10000000, 15000000, 20000000, 25000000, 30000000, 40000000, 50000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_trials = 100000;

    int elements_list[num_sizes];
    double time_list[num_sizes];

    struct Tree *tree = create_empty_tree(sizes[num_sizes - 1]);

    for (int i = 0; i < num_sizes; i++) {
        int start_elements = (i == 0) ? 0 : sizes[i - 1];
        measure_individual_delete_time(tree, start_elements, sizes[i], num_trials, elements_list, time_list, i);
    }

    free_tree(tree);
}


int main() {
    char response;

    printf("Running tests...\n");
    printf("------------------------------\n");
    test_create_empty_tree();
    printf("------------------------------\n");
    basic_insert_and_find();
    printf("------------------------------\n");
    delete_min_test();
    printf("------------------------------\n");
    delete_max_test();
    printf("------------------------------\n");
    right_min_test();
    printf("------------------------------\n");
    balance_test();
    printf("------------------------------\n");
    stress_test();
    printf("------------------------------\n");

    printf("Do you want to run the custom stress test? (y/n): ");
    scanf(" %c", &response);
    if (response == 'y' || response == 'Y') {
        printf("Enter the number of elements to insert: ");
        int num_elements;
        scanf("%d", &num_elements);

        printf("Enter the maximum size of the tree: ");
        int max_data;
        scanf("%d", &max_data);

        custom_stress_test(num_elements, max_data);
        printf("------------------------------\n");
    } else {
        printf("Custom stress test skipped.\n");
    }

    printf("Do you want to run the performance measurements for insertions? (y/n): ");
    scanf(" %c", &response);
    if (response == 'y' || response == 'Y') {
        measure_insert_times();
        printf("------------------------------\n");
    } else {
        printf("Performance measurements skipped.\n");
    }

    printf("------------------------------\n");

    printf("Do you want to run the performance measurements for deletions? (y/n): ");
    scanf(" %c", &response);
    if (response == 'y' || response == 'Y') {
        measure_delete_times();
        printf("------------------------------\n");
    } else {
        printf("Performance measurements for deletions skipped.\n");
    }

    return 0;
}
