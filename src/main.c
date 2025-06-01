#include "../include/tree.h"
#include <ncurses.h>
#include <time.h>

void print_menu(WINDOW *menu_win) {
    mvwprintw(menu_win, 1, 1, "Menu:");
    mvwprintw(menu_win, 2, 1, "1. Insert a risk");
    mvwprintw(menu_win, 3, 1, "2. Display the minimum risk");
    mvwprintw(menu_win, 4, 1, "3. Display the maximum risk");
    mvwprintw(menu_win, 5, 1, "4. Delete the minimum risk");
    mvwprintw(menu_win, 6, 1, "5. Delete the maximum risk");
    mvwprintw(menu_win, 7, 1, "6. Display tree size");
    mvwprintw(menu_win, 8, 1, "7. Quit");
    mvwprintw(menu_win, 9, 1, "Choose an option: ");
    wrefresh(menu_win);
}

void clear_message(WINDOW *menu_win) {
    for (int i = 10; i < 13; i++) {
        mvwprintw(menu_win, i, 1, "                                        ");
    }
    wrefresh(menu_win);
}

void redraw_window(WINDOW *menu_win) {
    box(menu_win, 0, 0);
    wrefresh(menu_win);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <tree_size>\n", argv[0]);
        return 1;
    }

    int tree_size = atoi(argv[1]);
    struct Tree *tree = create_empty_tree(tree_size);
    int choice, risk_level;
    struct Node *node;

    // Initialize random gen
    srand(time(NULL));

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Create a window for the menu
    WINDOW *menu_win = newwin(15, 40, 4, 4);
    box(menu_win, 0, 0);
    refresh();

    mvprintw(0, 0, "Tree size: %d", tree_size);
    mvprintw(1, 0, "Welcome to the risk management system!");
    refresh();

    while (1) {
        print_menu(menu_win);
        choice = wgetch(menu_win) - '0'; // Read a single character and convert it to an integer

        clear_message(menu_win);

        switch (choice) {
            case 1:
                mvwprintw(menu_win, 10, 1, "Enter risk level: ");
                redraw_window(menu_win);
                echo(); // Enable echo to show user input
                wrefresh(menu_win);
                wscanw(menu_win, "%d", &risk_level);
                noecho(); // Disable echo after input
                int id = rand(); // Generate a random ID
                insert(tree, risk_level, id);
                mvwprintw(menu_win, 11, 1, "Risk inserted with ID: %d", id);
                wrefresh(menu_win);
                break;
            case 2:
                node = find_min(tree);
                if (node) {
                    mvwprintw(menu_win, 10, 1, "Minimum risk: %d", node->risk_level);
                } else {
                    mvwprintw(menu_win, 10, 1, "No risk found.");
                }
                wrefresh(menu_win);
                break;
            case 3:
                node = find_max(tree);
                if (node) {
                    mvwprintw(menu_win, 10, 1, "Maximum risk: %d", node->risk_level);
                } else {
                    mvwprintw(menu_win, 10, 1, "No risk found.");
                }
                wrefresh(menu_win);
                break;
            case 4:
                delete_min(tree);
                mvwprintw(menu_win, 10, 1, "Minimum risk deleted.");
                wrefresh(menu_win);
                break;
            case 5:
                delete_max(tree);
                mvwprintw(menu_win, 10, 1, "Maximum risk deleted.");
                wrefresh(menu_win);
                break;
            case 6:
                mvwprintw(menu_win, 10, 1, "Tree size: %d", tree->current_nbr_data);
                wrefresh(menu_win);
                break;
            case 7:
                free_tree(tree);
                mvwprintw(menu_win, 10, 1, "Goodbye!");
                wrefresh(menu_win);
                endwin();
                return 0;
            default:
                mvwprintw(menu_win, 10, 1, "Invalid option. Please try again.");
                wrefresh(menu_win);
        }

        // Redraw the window border after each action
        redraw_window(menu_win);
    }
}