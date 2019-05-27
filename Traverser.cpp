
#include <iostream>
#include <vector>

struct Node {
    int data;
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
    std::vector<Node> childrens;

    Node(int data) : data(data) {}

    void addLeft(Node *left) {
        this->left = left;
        left->parent = this;
    }

    void addRight(Node *right) {
        this->right = right;
        right->parent = this;
    }
};

void printPreOrder(Node *root) {
    if (root == nullptr) {
        return;
    }
    std::cout << root->data << " ";
    for (auto children : root->childrens) {
        printPreOrder(&children);
    }
    printPreOrder(root->left);
    printPreOrder(root->right);
}

void printPostOrder(Node *root) {
    if (root == nullptr) {
        return;
    }
    printPostOrder(root->left);
    printPostOrder(root->right);
    std::cout << root->data << " ";
}

//int main() {
//    Node *root = new Node(0);
//    root->addLeft(new Node(1));
//    root->left->addLeft(new Node(2));
//    root->left->left->addRight(new Node(3));
//    root->left->addRight(new Node(4));
//    root->left->right->addLeft(new Node(5));
//    root->addRight(new Node(6));
//    root->right->addRight(new Node(7));
//    root->right->right->addLeft(new Node(8));
//    root->right->right->addRight(new Node(9));
//    printPreOrder(root);
//    std::cout << std::endl;
//    printPostOrder(root);
//    std::cout << std::endl;
//    return 0;
//}