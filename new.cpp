#include<stdio.h>
#include<stdlib.h>
typedef struct bnode* btree;

struct bnode {
    int element[3];
    btree parent;
    btree left, middle, right;
    int num_of_element;
    int isleaf;
};
int lele[4]; int rele[3];// 
int find(btree node);
btree insert(int x, btree t);
void print(btree t);
btree queue[20000];
int head, rear;
void enqueue(btree t) {
    queue[++head] = t;
}
btree dequeue() {
    return queue[++rear];
}
int main() {
    int n, a[20000], dup[200000], num;
    btree t;
    t = (btree)malloc(sizeof(struct bnode));
    for (int i = 0; i < 200000; ++i)
        dup[i] = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
        dup[a[i]]++;
        if (dup[a[i]] >= 2) {
            printf("key %d is duplicated\n", a[i]);
            i--;
            n--;
        }
        num = i;
    }
    //insert 1
    int m = 0;
    while (a[m++] < 0);
    /**/
    {
        t = (btree)malloc(sizeof(struct bnode));
        t->left = t->middle = t->right = NULL;
        t->element[0] = a[m - 1];
        t->element[1] = t->element[2] = -1;
        t->isleaf = 1;
        t->num_of_element = 1;
        t->parent = NULL;
    };
    /**/
    for (int i = m; i <= num; ++i) {
        t = insert(a[i], t);
    }
    print(t);
    return 0;
}
int find(btree node) {
    while (!node->isleaf) {
        node = node->left;
    }
    return node->element[0];
}
btree insert(int x, btree t) {
    //find 叶结点
    btree node0, node1, node2, node3, node4, root;
    node0 = t;
    //find the place 
    while (!node0->isleaf) {
        if (node0->num_of_element == 2) {
            if (x < node0->element[0]) node0 = node0->left;
            else node0 = node0->right;
        }
        else {//==3
            if (x < node0->element[0]) node0 = node0->left;
            else if (x > node0->element[0] && x < node0->element[1]) node0 = node0->middle;
            else if (x > node0->element[1]) node0 = node0->right;
        }
    }
    if (node0->num_of_element < 3) {//叶结点不满
        int o;
        for (o = node0->num_of_element; o >= 1 && x <= node0->element[o - 1]; o--)
            node0->element[o] = node0->element[o - 1];
        node0->element[o] = x;
        node0->num_of_element++;
        return t;
    }
    else if (node0->num_of_element == 3) {//满了
        for (int i = 0; i < 3; ++i)
            lele[i] = node0->element[i];
        int o;
        for (o = 3; o >= 1 && x <= lele[o - 1]; o--)
            lele[o] = lele[o - 1];
        lele[o] = x;

        node1 = (btree)malloc(sizeof(struct bnode));
        node2 = (btree)malloc(sizeof(struct bnode));
        node1->element[0] = lele[0]; node1->element[1] = lele[1]; node1->element[2] = -1;
        node2->element[0] = lele[2]; node2->element[1] = lele[3]; node2->element[2] = -1;
        node1->left = node1->right = node1->middle = NULL;
        node2->left = node2->right = node2->middle = NULL;
        node1->parent = node2->parent = node0->parent;
        node1->isleaf = node2->isleaf = 1;
        node1->num_of_element = node2->num_of_element = 2;
        node0 = node1;
        if (node0->parent) node0->parent->isleaf = 0;

        while (node0 != NULL) {
            if (node0->parent == NULL) {
                root = (btree)malloc(sizeof(struct bnode));
                root->left = node1;
                root->right = node2;
                root->middle = NULL;
                root->isleaf = 0;
                root->num_of_element = 2;
                root->element[0] = find(root->right);
                root->element[1] = root->element[2] = -1;
                root->parent = NULL;
                node1->parent = node2->parent = root;
                t = root;
                break;
            }
            else if (node0->parent->num_of_element == 2) {
                if (x < node0->parent->element[0]) {
                    node0->parent->left = node1;
                    node0->parent->middle = node2;
                }
                else {
                    node0->parent->middle = node1;
                    node0->parent->right = node2;
                }
                node0->parent->num_of_element = 3;
                node0->parent->element[0] = find(node0->parent->middle);
                node0->parent->element[1] = find(node0->parent->right);
                node0->parent->element[2] = -1;
                node0->parent->isleaf = 0;
                node1->parent = node2->parent = node0->parent;
                break;
            }
            else if (node0->parent->num_of_element == 3) {
                node3 = node0->parent;
                node4 = (btree)malloc(sizeof(struct bnode));
                node3->num_of_element = node4->num_of_element = 2;
                node3->isleaf = node4->isleaf = 0;
                node3->parent = node4->parent = node0->parent->parent;
                if (x < node0->parent->element[0]) {
                    node4->left = node0->parent->middle;
                    node4->right = node0->parent->right;
                    node4->middle = NULL;
                    node3->left = node1;
                    node4->right = node2;
                    node4->middle = NULL;
                }
                else if (x > node0->parent->element[0] && x < node0->parent->element[1]) {
                    node3->left = node0->parent->left;
                    node3->right = node1;
                    node4->left = node2;
                    node4->right = node0->parent->right;
                    node3->middle = node4->middle = NULL;
                }
                else if (x > node0->parent->element[1]) {
                    node3->left = node0->parent->left;
                    node3->right = node0->parent->middle;
                    node4->left = node1;
                    node4->right = node2;
                    node3->middle = node4->middle = NULL;
                }
                node3->element[0] = find(node3->right);
                node4->element[0] = find(node4->right);
                node3->element[1] = node3->element[2] = -1;
                node4->element[1] = node4->element[2] = -1;
                node1 = node3, node2 = node4;
                node0 = node0->parent;
            }
        }
    }
    return t;
}
void print(btree t) {
    if (t == NULL) return;
    btree node;
    int number_in_nextlevel = 0;
    int count = 1;
    enqueue(t);
    while (head != rear) {
        node = dequeue();
        //        printf("%d ",node->isleaf);
        if (node->isleaf == 0) {
            //        	printf("*****\n");
            count--;
            printf("[%d", node->element[0]);
            if (node->left) {
                enqueue(node->left);
                number_in_nextlevel++;
            }
            if (node->element[1] != -1) {
                printf(",%d]", node->element[1]);
                if (node->middle) {
                    enqueue(node->middle);
                    number_in_nextlevel++;
                }
            }
            else printf("]");
            if (node->right) {
                enqueue(node->right);
                number_in_nextlevel++;
            }
            if (!count) {
                printf("\n");
                count = number_in_nextlevel;
                number_in_nextlevel = 0;
            }

        }
        else {
            printf("[%d", node->element[0]);
            if (node->element[1] != -1) printf(",%d", node->element[1]);
            if (node->element[2] != -1) printf(",%d]", node->element[2]);
            else printf("]");
        }
    }
    printf("\n");
}