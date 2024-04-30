#include <iostream>
#include <cassert>
#include <stack>
using namespace std;

struct Node {
    string key;
    Node* left;
    Node* right;
    Node* parent;
    int height;
};

Node* newNode(string key) {
    Node* node = new Node;
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->height = 1;  // новый узел изначально добавляется в лист
    return node;
}

int height(Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // выполнить поворот
    x->right = y;
    y->left = T2;

    // обновить высоту
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // вернуть новый корень
    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // выполнить поворот
    y->left = x;
    x->right = T2;

    // обновить высоту
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // вернуть новый корень
    return y;
}

// получить баланс-фактор узла N
int getBalance(Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}


Node* insert(Node* root, string key) {
    stack<Node*> path;
    Node* current = root;
    Node* lastNode = NULL;

    // Проходим вниз по дереву и сохраняем путь
    while (current != NULL) {
        path.push(current);
        lastNode = current;
        if (key < current->key) {
            current = current->left;
        }
        else if (key > current->key) {
            current = current->right;
        }
        else {
            // равные ключи не допускаются в BST
            return root;
        }
    }

    // Создаем и вставляем новый узел
    Node* newNode = new Node();
    newNode->key = key;  // Устанавливаем ключ для нового узла
    if (lastNode == NULL) {
        root = newNode;
    }
    else if (key < lastNode->key) {
        lastNode->left = newNode;
    }
    else {
        lastNode->right = newNode;
    }
    path.push(newNode);

    // Балансируем дерево, проходясь вверх по сохраненному пути
    while (!path.empty()) {
        Node* node = path.top();
        path.pop();

        // Обновляем высоту узла
        node->height = 1 + max(height(node->left), height(node->right));

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
        if (path.empty()) {
            root = node;
        }
        else if (node->key < path.top()->key) {
            path.top()->left = node;
        }
        else {
            path.top()->right = node;
        }
    }

    return root;
}


// Функция для поиска узла с минимальным ключом в поддереве с заданным корнем
Node* minValueNode(Node* node) {
    Node* current = node;

    // Пройти вниз по дереву до самого левого листа
    while (current->left != NULL)
        current = current->left;

    return current;
}

// функция для удаления узла с заданным ключом из поддерева с заданным корнем.
// возвращает корень нового поддерева.
Node* deleteNode(Node* root, string key) {
    stack<Node*> path;
    Node* current = root;
    Node* lastNode = NULL;

    // Проходим вниз по дереву и сохраняем путь
    while (current != NULL && current->key != key) {
        path.push(current);
        lastNode = current;
        if (key < current->key) {
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
        Node* temp = current->left ? current->left : current->right;
        if (lastNode == NULL) {
            root = temp;
        }
        else if (lastNode->left == current) {
            lastNode->left = temp;
        }
        else {
            lastNode->right = temp;
        }
    }
    else {
        Node* successor = minValueNode(current->right);
        current->key = successor->key;
        current->right = deleteNode(current->right, successor->key);
    }

    // Балансируем дерево, проходясь вверх по сохраненному пути
    while (!path.empty()) {
        Node* node = path.top();
        path.pop();

        // Обновляем высоту узла
        node->height = 1 + max(height(node->left), height(node->right));

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
        if (path.empty()) {
            root = node;
        }
        else if (node->key < path.top()->key) {
            path.top()->left = node;
        }
        else {
            path.top()->right = node;
        }
    }

    return root;
}


// Функция для печати бинарного дерева (прямой обход)
void printTree(Node* root, string indent = "", bool isLeft = true) {
    if (root != NULL) {
        cout << indent;
        if (isLeft)
            cout << "L----";
        else
            cout << "R----";
        cout << root->key << "(" << root->height << ")" << endl;
        indent += "     ";
        printTree(root->left, indent);
        printTree(root->right, indent, false);
    }
}

//void test_tree() {
//    Node* root = NULL;
//
//    // Добавляем узлы в дерево и проверяем, что они добавляются правильно
//    root = insert(root, "10");
//    assert(root->key == "10");
//    assert(root->height == 1);
//
//    root = insert(root, "20");
//    assert(root->key == "20");
//    assert(root->height == 2);
//    assert(root->left->key == "10");
//
//    root = insert(root, "30");
//    assert(root->key == "20");
//    assert(root->height == 2);
//    assert(root->left->key == "10");
//    assert(root->right->key == "30");
//
//    // Удаляем узлы из дерева и проверяем, что они удаляются правильно
//    root = deleteNode(root, "10");
//    assert(root->key == "20");
//    assert(root->height == 2);
//    assert(root->left == NULL);
//    assert(root->right->key == "30");
//
//    root = deleteNode(root, "30");
//    assert(root->key == "20");
//    assert(root->height == 1);
//    assert(root->left == NULL);
//    assert(root->right == NULL);
//
//    cout << "All tests passed!" << endl;
//}

int main() {
    //test_tree();

    Node* root = NULL;
    string command;
    string key;

    while (true) {
        cout << "Enter a command (add/delete) and a key, or 'exit': ";
        cin >> command;
        if (command == "exit") {
            break;
        }
        cin >> key;
        if (command == "add") {
            root = insert(root, key);
        }
        else if (command == "delete") {
            root = deleteNode(root, key);
        }
        else {
            cout << "Unknown command. Please enter 'add', 'delete', or 'exit'." << endl;
            continue;
        }
        printTree(root);  // Ваша функция для вывода дерева
        cout << endl;
    }

    return 0;
}
