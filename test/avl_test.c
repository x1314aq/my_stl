//
//  Created by x1314aq on 2019/1/23.
//

#include "avl.h"

struct test_node {
    uint32_t key;
    struct avl_node avl;
};

static void insert(struct test_node *node, struct avl_root *root)
{
    struct avl_node **new = &root->node;
    struct avl_node *parent = NULL;
    uint32_t key = node->key;
    struct test_node *current;

    while(*new) {
        parent = *new;
        current = container_of(parent, struct test_node, avl);
        if(key < current->key)
            new = &parent->left;
        else
            new = &parent->right;
    }

    avl_link_node(&node->avl, parent, new);
    avl_insert_rotate(&node->avl, root);
}

static inline void erase(struct test_node *node, struct avl_root *root)
{
    avl_erase(&node->avl, root);
}

static inline void print_usage()
{
    puts("Usage: avl_test -n <NUMBER>   default: 10000");
}

int main(int argc, char *argv[])
{
    int i, c, number = 10000;
    struct test_node *test, *nodes;
    struct avl_node *node;
    struct avl_root root = { NULL };

    while((c = getopt(argc, argv, "hn:")) != -1) {
        switch(c) {
            case 'n':
                number = atoi(optarg);
                break;
            case 'h':
            case '?':
            default:
                print_usage();
                return 0;
        }
    }

    if(number < 0) {
        printf("negative node NUMBER: %d\n", number);
        return 1;
    }

    nodes = (struct test_node *) calloc(number, sizeof(struct test_node));

    for(i = 0; i < number; i++) {
        nodes[i].key = rand();
        insert(&nodes[i], &root);
    }

    for(node = avl_first(&root); node; node = avl_next(node)) {
        test = container_of(node, struct test_node, avl);
        printf("%u\n", test->key);
    }

    puts("---------------");

    for(node = avl_last(&root); node; node = avl_prev(node)) {
        test = container_of(node, struct test_node, avl);
        printf("%u\n", test->key);
    }

    for(i = 0; i < number - 1; i++)
        erase(&nodes[i], &root);

    if(root.node != &nodes[number - 1].avl) {
        puts("Error!\n");
        free(nodes);
        return 1;
    }

    free(nodes);
    return 0;
}
