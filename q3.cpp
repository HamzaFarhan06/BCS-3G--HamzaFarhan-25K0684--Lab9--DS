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

Node *insert(Node *root, int x) {
    if (root == nullptr) {
        cout << "Inserted: " << x << "\n";
        return new Node(x);
    }

    if (x < root->val) root->left = insert(root->left, x);
    else if (x > root->val) root->right = insert(root->right, x);
    else {
        cout << "Key already exists: " << x << "\n";
        return root;
    }

    root->height = 1 + getMax(height(root->left), height(root->right));
    int bf = getBalance(root);

    if (bf > 1 && x < root->left->val) {
        cout << "LL Imbalance at " << root->val << "\n";
        return rightRotate(root);
    }

    if (bf < -1 && x > root->right->val) {
        cout << "RR Imbalance at " << root->val << "\n";
        return leftRotate(root);
    }

    if (bf > 1 && x > root->left->val) {
        cout << "LR Imbalance at " << root->val << "\n";
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (bf < -1 && x < root->right->val) {
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
    cout << "[" << root->val << "]\n";
    printTree(root->left, space);
}

int main() {
    int a[] = {13, 10, 11};
    Node *root1 = nullptr;

    for (int i = 0; i < 3; i++) root1 = insert(root1, a[i]);

    cout << "\nLR Test:\n";
    cout << "Root: " << root1->val << "\n";
    cout << "Expected: 11\n";
    cout << "Result: " << (root1->val == 11 ? "SUCCESS" : "FAILED") << "\n";
    cout << "Inorder: ";
    inorder(root1);
    printTree(root1);

    int b[] = {10, 15, 12};
    Node *root2 = nullptr;

    for (int i = 0; i < 3; i++) root2 = insert(root2, b[i]);

    cout << "\nRL Test:\n";
    cout << "Root: " << root2->val << "\n";
    cout << "Expected: 12\n";
    cout << "Result: " << (root2->val == 12 ? "SUCCESS" : "FAILED") << "\n";
    cout << "Inorder: ";
    inorder(root2);
    printTree(root2);

    return 0;
}