#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

#define MAX_CMD_LEN 128

void insert_word(node_t *root, node_t *node);
int find_word(node_t *node, const char *query);
void inorder_print(node_t *node);
void node_free(node_t *node);

// A helper function to spell check a specific file
// 'file_name': Name of the file to spell check
// 'dict': A dictionary containing correct words
int spell_check_file(const char *file_name, const dictionary_t *dict) {
    FILE *file = fopen(file_name, "r");
    char word[MAX_CMD_LEN];
    char c;
    
    if (file == NULL) {
        return 0;
    }

    word[0] = '\0';
    while ((c = fgetc(file)) != EOF) {
        if (isspace(c)) {
            if (dict_find(dict, word)) {
                printf("%s", word);
            } else {
                printf("%s[X]", word);
            }

            word[0] = 0;
            printf("%c", c);
        } else {
            int i = 0;
            while (i < MAX_CMD_LEN) {
                if (word[i] == '\0') {
                    word[i] = c;
                    word[i + 1] = '\0';
                    break;
                }

                i++;
            }
        }
    }

    fclose(file);
    return 1;
}

/*
 * This is in general *very* similar to the list_main file seen in lab
 */
int main(int argc, char **argv) {
    dictionary_t *dict = NULL;
    char cmd[MAX_CMD_LEN];
    char word[MAX_CMD_LEN];

    if (argc == 2) {
        dict = read_dict_from_text_file(argv[1]);
        
        if (dict == NULL) {
            return 0;
        }
    }

    if (argc == 3) {
        dict = read_dict_from_text_file(argv[1]);

        if (dict == NULL) {
            return 0;
        }

        if(spell_check_file(argv[2], dict) == 0) {
            printf("Spell check failed\n");
        }

        dict_free(dict);
        return 0;
    }

    printf("CSCI 2021 Spell Check System\n");
    printf("Commands:\n");
    printf("  add <word>:              adds a new word to dictionary\n");
    printf("  lookup <word>:           searches for a word\n");
    printf("  print:                   shows all words currently in the dictionary\n");
    printf("  load <file_name>:        reads in dictionary from a file\n");
    printf("  save <file_name>:        writes dictionary to a file\n");
    printf("  check <file_name>: spell checks the specified file\n");
    printf("  exit:                    exits the program\n");

    while (1) {
        printf("spell_check> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        } else if (strcmp("exit", cmd) == 0) {
            break;
        } else if (strcmp("add", cmd) == 0) {
            scanf("%s", word);

            if (dict == NULL) {
                dict = create_dictionary();
            }

            dict_insert(dict, word);

        } else if (strcmp("lookup", cmd) == 0) {
            scanf("%s", word);

            if (dict_find(dict, word) != 0) {
                printf("\'%s\' present in dictionary\n", word);
            } else {
                printf("\'%s\' not found\n", word);
            }

        } else if (strcmp("print", cmd) == 0) {
            dict_print(dict);
            
        } else if (strcmp("load", cmd) == 0) {
            scanf("%s", word);
            
            dict_free(dict);
            dict = read_dict_from_text_file(word);
            
        } else if (strcmp("save", cmd) == 0) {
            scanf("%s", word);

            if (dict == NULL) {
                printf("Failed to write dictionary to text file\n");
            } else {
                write_dict_to_text_file(dict, word);
                printf("Dictionary successfully written to text file\n");
            }
            
        } else if (strcmp("check", cmd) == 0) {
            scanf("%s", word);
            
            if (spell_check_file(word, dict) == 0) {
                printf("Spell check failed\n");
            }
            
        } else {
            printf("Unknown command %s\n", cmd);
        }
    }

    dict_free(dict);
    return 0;
}
