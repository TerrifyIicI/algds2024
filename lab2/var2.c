#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <cassert>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Node {
    char* key;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int height;
};

struct Node* newNode(char* key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = _strdup(key);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int height(struct Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int Max(int a, int b) {
    return (a > b) ? a : b;
}

struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    // выполнить поворот
    x->right = y;
    y->left = T2;

    // обновить высоту
    y->height = Max(height(y->left), height(y->right)) + 1;
    x->height = Max(height(x->left), height(x->right)) + 1;

    // вернуть новый корень
    return x;
}

struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    // выполнить поворот
    y->left = x;
    x->right = T2;

    // обновить высоту
    x->height = Max(height(x->left), height(x->right)) + 1;
    y->height = Max(height(y->left), height(y->right)) + 1;

    // вернуть новый корень
    return y;
}

// получить баланс-фактор узла N
int getBalance(struct Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

struct Node* insert(struct Node* root, char* key) {
    if (root == NULL) {
        return newNode(key);
    }
    if (strcmp(key, root->key) < 0) {
        root->left = insert(root->left, key);
    }
    else if (strcmp(key, root->key) > 0) {
        root->right = insert(root->right, key);
    }
    else {
        return root;
    }
    root->height = 1 + Max(height(root->left), height(root->right));
    int balance = getBalance(root);
    if (balance > 1 && strcmp(key, root->left->key) < 0) {
        return rightRotate(root);
    }
    if (balance < -1 && strcmp(key, root->right->key) > 0) {
        return leftRotate(root);
    }
    if (balance > 1 && strcmp(key, root->left->key) > 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && strcmp(key, root->right->key) < 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// Функция для поиска узла с минимальным ключом в поддереве с заданным корнем
struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;

    // Пройти вниз по дереву до самого левого листа
    while (current->left != NULL)
        current = current->left;

    return current;
}

// функция для удаления узла с заданным ключом из поддерева с заданным корнем.
// возвращает корень нового поддерева.
struct Node* deleteNode(struct Node* root, char* key) {
    struct Node* path[1000];
    int path_length = 0;
    struct Node* current = root;
    struct Node* lastNode = NULL;

    // Проходим вниз по дереву и сохраняем путь
    while (current != NULL && strcmp(current->key, key) != 0) {
        path[path_length++] = current;
        lastNode = current;
        if (strcmp(key, current->key) < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    // Если узел не найден, возвращаем корень без изменений
    if (current == NULL) {
        return root;
    }

    // Удаляем найденный узел
    if (current->left == NULL || current->right == NULL) {
        struct Node* temp = current->left ? current->left : current->right;
        if (lastNode == NULL) {
            root = temp;
        }
        else if (lastNode->left == current) {
            lastNode->left = temp;
        }
        else {
            lastNode->right = temp;
        }
        free(current);
    }
    else {
        struct Node* successor = minValueNode(current->right);
        free(current->key);
        current->key = _strdup(successor->key);
        current->right = deleteNode(current->right, successor->key);
    }

    // Балансируем дерево, проходясь вверх по сохраненному пути
    for (int i = path_length - 1; i >= 0; i--) {
        struct Node* node = path[i];

        // Обновляем высоту узла
        node->height = 1 + Max(height(node->left), height(node->right));

        // Получаем баланс-фактор узла
        int balance = getBalance(node);

        // Если узел несбалансирован, выполняем соответствующий поворот
        if (balance > 1) {
            if (getBalance(node->left) >= 0) {
                node = rightRotate(node);
            }
            else {
                node->left = leftRotate(node->left);
                node = rightRotate(node);
            }
        }
        else if (balance < -1) {
            if (getBalance(node->right) <= 0) {
                node = leftRotate(node);
            }
            else {
                node->right = rightRotate(node->right);
                node = leftRotate(node);
            }
        }

        // Если мы в корне, обновляем корень
        if (i == 0) {
            root = node;
        }
        else if (path[i - 1]->left == node) {
            path[i - 1]->left = node;
        }
        else {
            path[i - 1]->right = node;
        }
    }

    return root;
}

// Функция для печати бинарного дерева (прямой обход)
void printTree(struct Node* root, int indent, int isLeft) {
    if (root != NULL) {
        for (int i = 0; i < indent; i++) {
            printf("    ");
        }
        if (isLeft)
            printf("L----");
        else
            printf("R----");
        printf("%s (%d)\n", root->key, root->height);
        printTree(root->left, indent + 1, 1);
        printTree(root->right, indent + 1, 0);
    }
}
void test_complex_operations() {
    struct Node* root = NULL;

    // Последовательно добавляем узлы
    root = insert(root, "A");
    assert(strcmp(root->key, "A") == 0);

    root = insert(root, "B");
    assert(strcmp(root->key, "A") == 0);
    assert(strcmp(root->right->key, "B") == 0);

    root = insert(root, "C");
    assert(strcmp(root->key, "B") == 0);
    assert(strcmp(root->left->key, "A") == 0);
    assert(strcmp(root->right->key, "C") == 0);

    root = insert(root, "G");
    root = insert(root, "F");
    root = insert(root, "H");
    root = insert(root, "I");
    root = insert(root, "J");
    root = insert(root, "k");
    root = insert(root, "l");
    root = insert(root, "m");
    root = insert(root, "n");

    // Проверяем структуру дерева после добавления всех узлов
    assert(strcmp(root->key, "J") == 0);
    assert(strcmp(root->left->key, "F") == 0);
    assert(strcmp(root->right->key, "l") == 0);

    // Последовательно удаляем узлы
    root = deleteNode(root, "H");
    assert(strcmp(root->key, "J") == 0);
    assert(root->left->right->right == NULL); // Проверяем, что H удален

    root = deleteNode(root, "B");
    assert(strcmp(root->left->left->key, "C") == 0); // Проверяем, что B заменен на C

    root = deleteNode(root, "n");
    assert(root->right->right->right == NULL); // Проверяем, что n удален

    root = deleteNode(root, "J");
    assert(strcmp(root->key, "k") == 0); // Проверяем, что J заменен на k

    printf("Все тесты пройдены успешно!");
}

int main() {
    test_complex_operations();
    struct Node* root = NULL;
    char command[10];
    char key[100];

    while (1) {
        printf("Enter a command (add/delete) and a key, or 'exit': ");
        scanf("%s", command);
        if (strcmp(command, "exit") == 0) {
            break;
        }
        scanf("%s", key);
        if (strcmp(command, "add") == 0) {
            root = insert(root, _strdup(key));
        }
        else if (strcmp(command, "delete") == 0) {
            root = deleteNode(root, strdup(key));
        }
        else {
            printf("Unknown command. Please enter 'add', 'delete', or 'exit'.\n");
            continue;
        }
        printTree(root, 0, 0);  // Ваша функция для вывода дерева
        printf("\n");
    }

    return 0;
}
