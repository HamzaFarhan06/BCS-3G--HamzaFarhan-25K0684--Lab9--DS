#include <iostream>
using namespace std;

struct Node {
    int val, height;
    Node *left, *right;
    Node(int x) {
        val = x;
        left = right = nullptr;
        height = 1;
    }
};

int height(Node *root) {
    if (root == nullptr) return 0;
    return root->height;
}

int getMax(int a, int b) {
    return a > b ? a : b;
}

int getBalance(Node *root) {
    if (root == nullptr) return 0;
    return height(root->left) - height(root->right);
}

Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *t = x->right;

    x->right = y;
    y->left = t;

    y->height = getMax(height(y->left), height(y->right)) + 1;
    x->height = getMax(height(x->left), height(x->right)) + 1;
    return x;
}

Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *t = y->left;

    y->left = x;
    x->right = t;

    x->height = getMax(height(x->left), height(x->right)) + 1;
    y->height = getMax(height(y->left), height(y->right)) + 1;
    return y;
}

Node *findMin(Node *root) {
    while (root && root->left != nullptr) root = root->left;
    return root;
}

Node *insert(Node *root, int x) {
    if (root == nullptr) return new Node(x);
    if (x < root->val) root->left = insert(root->left, x);
    else if (x > root->val) root->right = insert(root->right, x);
    else return root;

    root->height = 1 + getMax(height(root->left), height(root->right));
    int bf = getBalance(root);

    if (bf > 1 && x < root->left->val) return rightRotate(root);
    if (bf < -1 && x > root->right->val) return leftRotate(root);

    if (bf > 1 && x > root->left->val) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (bf < -1 && x < root->right->val) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node *deleteNode(Node *root, int x) {
    if (root == nullptr) {
        cout << "Record " << x << " not found.\n";
        return root;
    }

    if (x < root->val) root->left = deleteNode(root->left, x);
    else if (x > root->val) root->right = deleteNode(root->right, x);
    else {
        cout << "Deleting record: " << x << "\n";

        if (root->left == nullptr || root->right == nullptr) {
            Node *temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            }
            else *root = *temp;
            delete temp;
        }
        else {
            Node *temp = findMin(root->right);
            root->val = temp->val;
            root->right = deleteNode(root->right, temp->val);
        }
    }

    if (root == nullptr) return root;

    root->height = 1 + getMax(height(root->left), height(root->right));
    int bf = getBalance(root);

    if (bf > 1 && getBalance(root->left) >= 0) {
        cout << "LL Imbalance at " << root->val << "\n";
        return rightRotate(root);
    }

    if (bf > 1 && getBalance(root->left) < 0) {
        cout << "LR Imbalance at " << root->val << "\n";
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (bf < -1 && getBalance(root->right) <= 0) {
        cout << "RR Imbalance at " << root->val << "\n";
        return leftRotate(root);
    }

    if (bf < -1 && getBalance(root->right) > 0) {
        cout << "RL Imbalance at " << root->val << "\n";
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void inorder(Node *root) {
    if (root == nullptr) return;
    inorder(root->left);
    cout << root->val << " ";
    inorder(root->right);
}

void printTree(Node *root, int space = 0) {
    if (root == nullptr) return;
    space += 6;
    printTree(root->right, space);
    cout << "\n";
    for (int i = 6; i < space; i++) cout << " ";
    cout << "[" << root->val << " | BF:" << getBalance(root) << "]\n";
    printTree(root->left, space);
}

int main() {
    int a[] = {9, 5, 10, 0, 6, 11, -1, 1, 2};
    int n = 9;
    Node *root = nullptr;

    for (int i = 0; i < n; i++) root = insert(root, a[i]);

    cout << "Initial Tree:\n";
    cout << "Inorder: ";
    inorder(root);
    printTree(root);

    int b[] = {10, 11, 1};

    for (int i = 0; i < 3; i++) {
        cout << "\nDeleting " << b[i] << ":\n";
        root = deleteNode(root, b[i]);
        cout << "Inorder: ";
        inorder(root);
        printTree(root);
    }

    return 0;
}