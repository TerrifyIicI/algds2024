#include <iostream>
#include <cassert>
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

Node* insert(Node* node, string key) {
    /* 1. выполнить стандартное вставку BST */
    if (node == NULL)
        return (newNode(key));

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else  // равные ключи не допускаются в BST
        return node;

    /* 2. обновить высоту этого предка узла */
    node->height = 1 + max(height(node->left), height(node->right));

    /* 3. получить баланс-фактор этого предка узла, чтобы проверить, стал ли этот узел несбалансированным */
    int balance = getBalance(node);

    // если этот узел становится несбалансированным, то есть 4 случая

    // случай влево влево
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // случай вправо вправо
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // случай влево вправо
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // случай вправо влево
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* возвращаем неизмененный указатель на узел */
    return node;
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
    // шаг 1: выполнить стандартное удаление BST
    if (root == NULL)
        return root;

    // если удаляемый ключ меньше ключа корня, то он находится в левом поддереве
    if (key < root->key)
        root->left = deleteNode(root->left, key);

    // если удаляемый ключ больше ключа корня, то он находится в правом поддереве
    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    // если удаляемый ключ равен ключу корня, то это узел, который нужно удалить
    else {
        // узел с только одним или без дочернего узла
        if ((root->left == NULL) || (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;

            // нет дочерних узлов
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else  // один дочерний узел
                *root = *temp;  // копировать содержимое не пустого дочернего узла
            free(temp);
        }
        else {
            // узел с двумя дочерними узлами: получить узел со следующим (самым маленьким)
            // ключом в правом поддереве
            Node* temp = minValueNode(root->right);

            // скопировать содержимое узла-наследника в этот узел
            root->key = temp->key;

            // удалить узел-наследника
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // если дерево имело только один узел, то вернуть его
    if (root == NULL)
        return root;

    // шаг 2: обновить высоту текущего узла
    root->height = 1 + max(height(root->left), height(root->right));

    // шаг 3: получить баланс-фактор узла (проверить, стал ли этот узел несбалансированным)
    int balance = getBalance(root);

    // если этот узел становится несбалансированным, то есть 4 случая

    // случай влево влево
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // случай влево вправо
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // случай вправо вправо
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // случай вправо влево
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
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
