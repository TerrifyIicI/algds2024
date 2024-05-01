#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <cassert>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Определение структуры Node для представления узла в бинарном дереве поиска.
struct Node {
    char* key; // Ключ узла, который определяет его позицию в дереве.
    struct Node* left; // Указатель на левого потомка узла.
    struct Node* right; // Указатель на правого потомка узла.
    struct Node* parent; // Указатель на родительский узел.
    int height; // Высота узла в дереве.
};

// Функция для создания нового узла с заданным ключом.
struct Node* newNode(const char* key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node)); // Выделение памяти под новый узел.
    node->key = _strdup(key); // Копирование ключа в узел.
    node->left = NULL; // Установка левого потомка узла в NULL.
    node->right = NULL; // Установка правого потомка узла в NULL.
    node->height = 1; // Установка высоты узла равной 1, так как узел является листом без потомков.
    return node; // Возвращение указателя на новый узел.
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

// Функция для вставки нового узла с заданным ключом в бинарное дерево поиска.
struct Node* insert(struct Node* root, const char* key) {
    if (root == NULL) { // Если дерево пусто,
        return newNode(key); // создаем новый узел с заданным ключом.
    }
    if (strcmp(key, root->key) < 0) { // Если заданный ключ меньше ключа корня,
        root->left = insert(root->left, key); // вставляем новый узел в левое поддерево.
    }
    else if (strcmp(key, root->key) > 0) { // Если заданный ключ больше ключа корня,
        root->right = insert(root->right, key); // вставляем новый узел в правое поддерево.
    }
    else { // Если ключи равны,
        return root; // возвращаем корень без изменений.
    }
    root->height = 1 + Max(height(root->left), height(root->right)); // Обновляем высоту корня.
    int balance = getBalance(root); // Получаем баланс-фактор корня.
    if (balance > 1 && strcmp(key, root->left->key) < 0) { // Если корень несбалансирован и ключ меньше ключа левого потомка корня,
        return rightRotate(root); // выполняем правый поворот.
    }
    if (balance < -1 && strcmp(key, root->right->key) > 0) { // Если корень несбалансирован и ключ больше ключа правого потомка корня,
        return leftRotate(root); // выполняем левый поворот.
    }
    if (balance > 1 && strcmp(key, root->left->key) > 0) { // Если корень несбалансирован, ключ больше ключа левого потомка корня,
        root->left = leftRotate(root->left); // выполняем левый поворот для левого потомка,
        return rightRotate(root); // затем выполняем правый поворот для корня.
    }
    if (balance < -1 && strcmp(key, root->right->key) < 0) { // Если корень несбалансирован, ключ меньше ключа правого потомка корня,
        root->right = rightRotate(root->right); // выполняем правый поворот для правого потомка,
        return leftRotate(root); // затем выполняем левый поворот для корня.
    }
    return root; // Возвращаем новый корень.
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
struct Node* deleteNode(struct Node* root, const char* key) {
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
#include <assert.h>
#include <string.h>

void test_tree() {
    struct Node* root = NULL;

    // Добавляем узлы в дерево и проверяем, что они добавляются правильно
    root = insert(root, "A");
    assert(strcmp(root->key, "A") == 0);
    assert(root->height == 1);

    root = insert(root, "B");
    assert(strcmp(root->key, "A") == 0);
    assert(root->height == 2);
    assert(strcmp(root->right->key, "B") == 0);

    root = insert(root, "C");
    assert(strcmp(root->key, "B") == 0);
    assert(root->height == 2);
    assert(strcmp(root->left->key, "A") == 0);
    assert(strcmp(root->right->key, "C") == 0);

    root = insert(root, "D");
    assert(strcmp(root->key, "B") == 0);
    assert(root->height == 3);
    assert(strcmp(root->left->key, "A") == 0);
    assert(strcmp(root->right->key, "C") == 0);
    assert(strcmp(root->right->right->key, "D") == 0);

    root = insert(root, "E");
    assert(strcmp(root->key, "B") == 0);
    assert(root->height == 3);
    assert(strcmp(root->left->key, "A") == 0);
    assert(strcmp(root->right->key, "D") == 0);
    assert(strcmp(root->right->left->key, "C") == 0);
    assert(strcmp(root->right->right->key, "E") == 0);

    root = insert(root, "F");
    assert(strcmp(root->key, "D") == 0);
    assert(root->height == 3);
    assert(strcmp(root->left->key, "B") == 0);
    assert(strcmp(root->left->left->key, "A") == 0);
    assert(strcmp(root->left->right->key, "C") == 0);
    assert(strcmp(root->right->key, "E") == 0);
    assert(strcmp(root->right->right->key, "F") == 0);

    root = insert(root, "G");
    assert(strcmp(root->key, "D") == 0);
    assert(root->height == 3);
    assert(strcmp(root->left->key, "B") == 0);
    assert(strcmp(root->left->left->key, "A") == 0);
    assert(strcmp(root->left->right->key, "C") == 0);
    assert(strcmp(root->right->key, "F") == 0);
    assert(strcmp(root->right->left->key, "E") == 0);
    assert(strcmp(root->right->right->key, "G") == 0);

    root = insert(root, "H");
    assert(strcmp(root->key, "D") == 0);
    assert(root->height == 4);
    assert(strcmp(root->left->key, "B") == 0);
    assert(strcmp(root->left->left->key, "A") == 0);
    assert(strcmp(root->left->right->key, "C") == 0);
    assert(strcmp(root->right->key, "F") == 0);
    assert(strcmp(root->right->left->key, "E") == 0);
    assert(strcmp(root->right->right->key, "G") == 0);
    assert(strcmp(root->right->right->right->key, "H") == 0);

    // Продолжаем добавлять узлы и проверяем, что дерево корректно балансируется
    root = insert(root, "I");
    assert(strcmp(root->key, "D") == 0);
    assert(root->height == 4);
    assert(strcmp(root->left->key, "B") == 0);
    assert(strcmp(root->left->left->key, "A") == 0);
    assert(strcmp(root->left->right->key, "C") == 0);
    assert(strcmp(root->right->key, "F") == 0);
    assert(strcmp(root->right->left->key, "E") == 0);
    assert(strcmp(root->right->right->key, "H") == 0);
    assert(strcmp(root->right->right->left->key, "G") == 0);
    assert(strcmp(root->right->right->right->key, "I") == 0);

    root = insert(root, "J");
    assert(strcmp(root->key, "D") == 0);
    assert(root->height == 4);
    assert(strcmp(root->left->key, "B") == 0);
    assert(strcmp(root->left->left->key, "A") == 0);
    assert(strcmp(root->left->right->key, "C") == 0);
    assert(strcmp(root->right->key, "H") == 0);
    assert(strcmp(root->right->left->key, "F") == 0);
    assert(strcmp(root->right->left->left->key, "E") == 0);
    assert(strcmp(root->right->left->right->key, "G") == 0);
    assert(strcmp(root->right->right->key, "I") == 0);
    assert(strcmp(root->right->right->right->key, "J") == 0);

    root = insert(root, "K");
    assert(strcmp(root->key, "D") == 0);
    assert(root->height == 4);
    assert(strcmp(root->left->key, "B") == 0);
    assert(strcmp(root->left->left->key, "A") == 0);
    assert(strcmp(root->left->right->key, "C") == 0);
    assert(strcmp(root->right->key, "H") == 0);
    assert(strcmp(root->right->left->key, "F") == 0);
    assert(strcmp(root->right->left->left->key, "E") == 0);
    assert(strcmp(root->right->left->right->key, "G") == 0);
    assert(strcmp(root->right->right->key, "J") == 0);
    assert(strcmp(root->right->right->left->key, "I") == 0);
    assert(strcmp(root->right->right->right->key, "K") == 0);

    root = insert(root, "L");
    assert(strcmp(root->key, "H") == 0);
    assert(root->height == 4);
    assert(strcmp(root->left->key, "D") == 0);
    assert(strcmp(root->left->left->key, "B") == 0);
    assert(strcmp(root->left->left->left->key, "A") == 0);
    assert(strcmp(root->left->left->right->key, "C") == 0);
    assert(strcmp(root->left->right->key, "F") == 0);
    assert(strcmp(root->left->right->left->key, "E") == 0);
    assert(strcmp(root->left->right->right->key, "G") == 0);
    assert(strcmp(root->right->key, "J") == 0);
    assert(strcmp(root->right->left->key, "I") == 0);
    assert(strcmp(root->right->right->key, "K") == 0);
    assert(strcmp(root->right->right->right->key, "L") == 0);

    root = insert(root, "M");
    assert(strcmp(root->key, "H") == 0);
    assert(root->height == 4);
    assert(strcmp(root->left->key, "D") == 0);
    assert(strcmp(root->left->left->key, "B") == 0);
    assert(strcmp(root->left->left->left->key, "A") == 0);
    assert(strcmp(root->left->left->right->key, "C") == 0);
    assert(strcmp(root->left->right->key, "F") == 0);
    assert(strcmp(root->left->right->left->key, "E") == 0);
    assert(strcmp(root->left->right->right->key, "G") == 0);
    assert(strcmp(root->right->key, "J") == 0);
    assert(strcmp(root->right->left->key, "I") == 0);
    assert(strcmp(root->right->right->key, "L") == 0);
    assert(strcmp(root->right->right->left->key, "K") == 0);
    assert(strcmp(root->right->right->right->key, "M") == 0);

    root = insert(root, "N");
    assert(strcmp(root->key, "H") == 0);
    assert(root->height == 4);
    assert(strcmp(root->left->key, "D") == 0);
    assert(strcmp(root->left->left->key, "B") == 0);
    assert(strcmp(root->left->left->left->key, "A") == 0);
    assert(strcmp(root->left->left->right->key, "C") == 0);
    assert(strcmp(root->left->right->key, "F") == 0);
    assert(strcmp(root->left->right->left->key, "E") == 0);
    assert(strcmp(root->left->right->right->key, "G") == 0);
    assert(strcmp(root->right->key, "L") == 0);
    assert(strcmp(root->right->left->key, "J") == 0);
    assert(strcmp(root->right->left->right->key, "K") == 0);
    assert(strcmp(root->right->right->key, "M") == 0);
    assert(strcmp(root->right->right->right->key, "N") == 0);

    // Удаляем узлы из дерева и проверяем, что они удаляются правильно
    root = deleteNode(root, "N");
    assert(strcmp(root->key, "H") == 0);
    assert(root->height == 4);
    assert(strcmp(root->left->key, "D") == 0);
    assert(strcmp(root->left->left->key, "B") == 0);
    assert(strcmp(root->left->left->left->key, "A") == 0);
    assert(strcmp(root->left->left->right->key, "C") == 0);
    assert(strcmp(root->left->right->key, "F") == 0);
    assert(strcmp(root->left->right->left->key, "E") == 0);
    assert(strcmp(root->left->right->right->key, "G") == 0);
    assert(strcmp(root->right->key, "L") == 0);
    assert(strcmp(root->right->left->key, "J") == 0);
    assert(strcmp(root->right->left->right->key, "K") == 0);
    assert(strcmp(root->right->right->key, "M") == 0);

    root = deleteNode(root, "H");
    assert(strcmp(root->key, "I") == 0);
    assert(root->height == 4);
    assert(strcmp(root->left->key, "D") == 0);
    assert(strcmp(root->left->left->key, "B") == 0);
    assert(strcmp(root->left->left->left->key, "A") == 0);
    assert(strcmp(root->left->left->right->key, "C") == 0);
    assert(strcmp(root->left->right->key, "F") == 0);
    assert(strcmp(root->left->right->left->key, "E") == 0);
    assert(strcmp(root->left->right->right->key, "G") == 0);
    assert(strcmp(root->right->key, "L") == 0);
    assert(strcmp(root->right->left->key, "J") == 0);
    assert(strcmp(root->right->left->right->key, "K") == 0);
    assert(strcmp(root->right->right->key, "M") == 0);

    root = deleteNode(root, "F");
    assert(strcmp(root->key, "I") == 0);
    assert(root->height == 4);
    assert(strcmp(root->left->key, "D") == 0);
    assert(strcmp(root->left->left->key, "B") == 0);
    assert(strcmp(root->left->left->left->key, "A") == 0);
    assert(strcmp(root->left->left->right->key, "C") == 0);
    assert(strcmp(root->left->right->key, "G") == 0);
    assert(strcmp(root->left->right->left->key, "E") == 0);
    assert(strcmp(root->right->key, "L") == 0);
    assert(strcmp(root->right->left->key, "J") == 0);
    assert(strcmp(root->right->left->right->key, "K") == 0);
    assert(strcmp(root->right->right->key, "M") == 0);


    printf("All tests passed successfully!\n");
}

int main() {
    test_tree();
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
