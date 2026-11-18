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

struct QueueNode {
    Node *tree;
    QueueNode *next;
    QueueNode(Node *x) {
        tree = x;
        next = nullptr;
    }
};

class Queue {
    QueueNode *frontNode, *rearNode;
    int cnt;

public:
    Queue() {
        frontNode = rearNode = nullptr;
        cnt = 0;
    }

    ~Queue() {
        while (!isEmpty()) dequeue();
    }

    bool isEmpty() {
        return frontNode == nullptr;
    }

    int size() {
        return cnt;
    }

    void enqueue(Node *x) {
        QueueNode *temp = new QueueNode(x);
        if (rearNode == nullptr) frontNode = rearNode = temp;
        else {
            rearNode->next = temp;
            rearNode = temp;
        }
        cnt++;
    }

    Node *dequeue() {
        if (isEmpty()) return nullptr;
        QueueNode *temp = frontNode;
        Node *x = temp->tree;
        frontNode = frontNode->next;
        if (frontNode == nullptr) rearNode = nullptr;
        delete temp;
        cnt--;
        return x;
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

Node *findMin(Node *root) {
    while (root && root->left != nullptr) root = root->left;
    return root;
}

Node *deleteNode(Node *root, int x) {
    if (root == nullptr) return root;

    if (x < root->val) root->left = deleteNode(root->left, x);
    else if (x > root->val) root->right = deleteNode(root->right, x);
    else {
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

    if (bf > 1 && getBalance(root->left) >= 0) return rightRotate(root);

    if (bf > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (bf < -1 && getBalance(root->right) <= 0) return leftRotate(root);

    if (bf < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void levelOrder(Node *root) {
    if (root == nullptr) {
        cout << "Tree is empty.\n";
        return;
    }

    Queue q;
    q.enqueue(root);
    int level = 1;

    while (!q.isEmpty()) {
        int n = q.size();
        cout << "Level " << level << ": ";

        for (int i = 0; i < n; i++) {
            Node *cur = q.dequeue();
            cout << "[" << cur->val << " | Height: " << cur->height << " | BF: " << getBalance(cur) << "] ";

            if (cur->left) q.enqueue(cur->left);
            if (cur->right) q.enqueue(cur->right);
        }

        cout << "\n";
        level++;
    }
}

bool verifyBalance(Node *root) {
    if (root == nullptr) return true;
    int bf = getBalance(root);
    if (bf < -1 || bf > 1) return false;
    return verifyBalance(root->left) && verifyBalance(root->right);
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
    Node *root = nullptr;
    int a[] = {30, 20, 40, 10, 25};

    for (int i = 0; i < 5; i++) root = insert(root, a[i]);

    cout << "After Insertions:\n";
    levelOrder(root);
    printTree(root);

    int b[] = {10, 40};
    for (int i = 0; i < 2; i++) root = deleteNode(root, b[i]);

    cout << "\nAfter Deletions:\n";
    levelOrder(root);
    printTree(root);

    if (verifyBalance(root)) cout << "\nAVL Tree is balanced.\n";
    else cout << "\nAVL Tree is not balanced.\n";

    return 0;
}