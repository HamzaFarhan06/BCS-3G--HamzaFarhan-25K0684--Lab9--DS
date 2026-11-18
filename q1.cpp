#include <iostream>
using namespace std;

struct Node {
    int val;
    Node *left, *right;
    Node(int x) {
        val = x;
        left = right = nullptr;
    }
};

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) {
        val = x;
        next = nullptr;
    }
};

Node *insert(Node *root, int x) {
    if (root == nullptr) return new Node(x);
    if (x < root->val) root->left = insert(root->left, x);
    else if (x > root->val) root->right = insert(root->right, x);
    return root;
}

int height(Node *root) {
    if (root == nullptr) return 0;
    int l = height(root->left);
    int r = height(root->right);
    return 1 + (l > r ? l : r);
}

void storeInorder(Node *root, ListNode *&head, ListNode *&tail) {
    if (root == nullptr) return;
    storeInorder(root->left, head, tail);
    ListNode *temp = new ListNode(root->val);
    if (head == nullptr) head = tail = temp;
    else {
        tail->next = temp;
        tail = temp;
    }
    storeInorder(root->right, head, tail);
}

Node *buildBalanced(ListNode *head) {
    if (head == nullptr) return nullptr;
    if (head->next == nullptr) return new Node(head->val);

    ListNode *prev = nullptr, *slow = head, *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }

    if (prev != nullptr) prev->next = nullptr;

    Node *root = new Node(slow->val);
    root->left = buildBalanced(head);
    root->right = buildBalanced(slow->next);
    return root;
}

Node *balanceBST(Node *root) {
    if (root == nullptr) return nullptr;
    ListNode *head = nullptr, *tail = nullptr;
    storeInorder(root, head, tail);
    return buildBalanced(head);
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
    int arr[] = {101, 102, 103, 104, 105, 106, 107};
    int n = 7;
    Node *root = nullptr;

    for (int i = 0; i < n; i++) root = insert(root, arr[i]);

    cout << "Original BST:\n";
    cout << "Inorder: ";
    inorder(root);
    cout << "\nHeight Before: " << height(root) << "\n";
    printTree(root);

    int before = height(root);
    Node *newRoot = balanceBST(root);

    cout << "\nBalanced BST:\n";
    cout << "Inorder: ";
    inorder(newRoot);
    cout << "\nHeight After: " << height(newRoot) << "\n";
    printTree(newRoot);

    cout << "\nInitial Height: " << before;
    cout << "\nBalanced Height: " << height(newRoot);
    cout << "\nHeight Reduction: " << before - height(newRoot) << "\n";

    return 0;
}