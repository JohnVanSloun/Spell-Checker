#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"



dictionary_t *create_dictionary() {
    dictionary_t *dict = malloc(sizeof(dictionary_t));
    
    dict->root = NULL;
    dict->size = 0;
    return dict;
}

void insert_word(node_t *root, node_t *node) {
    int cmp_val = strcmp(root->word, node->word);

    if (cmp_val == 0) {
        return;
    } else if (cmp_val > 0) {
        if (root->left == NULL) {
            root->left = node;
            return;
        } else {
            insert_word(root->left, node);
        }
    } else {
        if (root->right == NULL) {
            root->right = node;
            return;
        } else {
            insert_word(root->right, node);
        }
    }
}

int dict_insert(dictionary_t *dict, const char *word) {
    node_t *node = malloc(sizeof(node_t));

    strcpy(node->word, word);
    node->left = NULL;
    node->right = NULL;
    

    if (dict->root == NULL) {
        dict->root = node;
    } else {
        insert_word(dict->root, node);
    }

    dict->size++;
    return 0;
}

int find_word(node_t *node, const char *query) {
    if (node == NULL) {
        return 0;
    } 

    int cmp_val = strcmp(node->word, query);

    if (cmp_val > 0) {
        return find_word(node->left, query);
    } else if (cmp_val < 0){
        return find_word(node->right, query);
    } else {
        return 1;
    }
}

int dict_find(const dictionary_t *dict, const char *query) {
    if (dict == NULL) {
        return 0;
    }

    return find_word(dict->root, query);
}

void inorder_print(node_t *node) {
    if (node == NULL) {
        return;
    }

    inorder_print(node->left);

    printf("%s\n", node->word);

    inorder_print(node->right);
}

void dict_print(const dictionary_t *dict) {
    if (dict == NULL) {
        return;
    }

    inorder_print(dict->root);
}

void node_free(node_t *node) {
    if (node == NULL) {
        return;
    }

    node_free(node->left);
    node_free(node->right);

    free(node);
}

void dict_free(dictionary_t *dict) {
    if (dict == NULL) {
        return;
    }

    node_free(dict->root);
    free(dict);
}

dictionary_t *read_dict_from_text_file(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    char word[MAX_WORD_LEN];

    if (file == NULL) {
        printf("Failed to read dictionary from text file\n");
        return NULL;
    }
    
    dictionary_t *dict = create_dictionary();

    while (fscanf(file, " %127s", word) == 1) {
        dict_insert(dict, word);
    }

    fclose(file);

    printf("Dictionary successfully read from text file\n");

    return dict;
}

void write_preorder(node_t *node, FILE *file) {
    if (node == NULL) {
        return;
    }

    fprintf(file, "%s\n", node->word);

    write_preorder(node->left, file);

    write_preorder(node->right, file);   
}

int write_dict_to_text_file(const dictionary_t *dict, const char *file_name) {
    if (dict == NULL) {
        return 0;
    }

    FILE *file = fopen(file_name, "w");
    
    write_preorder(dict->root, file);

    fclose(file);
    return 1;
}