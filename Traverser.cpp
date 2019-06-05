
#include <iostream>
#include <vector>

struct BinaryNode {
    int data;
    BinaryNode *parent = nullptr;
    BinaryNode *left = nullptr;
    BinaryNode *right = nullptr;

    BinaryNode(int data) : data(data) {}

    void addLeft(BinaryNode *left) {
        this->left = left;
        left->parent = this;
    }

    void addRight(BinaryNode *right) {
        this->right = right;
        right->parent = this;
    }
};

void printBinaryPreOrder(BinaryNode *node) {
    if (node == nullptr) {
        return;
    }
    std::cout << node->data << " ";
    printBinaryPreOrder(node->left);
    printBinaryPreOrder(node->right);
}

void printBinaryPostOrder(BinaryNode *node) {
    if (node == nullptr) {
        return;
    }
    printBinaryPostOrder(node->left);
    printBinaryPostOrder(node->right);
    std::cout << node->data << " ";
}

void printBinaryTree() {
    std::vector<BinaryNode*> nodes;
    for (int i = 0; i < 10; ++i) {
        nodes.push_back(new BinaryNode(i));
    }
    auto *root = nodes[0];
    root->addLeft(nodes[1]);
    root->left->addLeft(nodes[2]);
    root->left->left->addRight(nodes[3]);
    root->left->addRight(nodes[4]);
    root->left->right->addLeft(nodes[5]);
    root->addRight(nodes[6]);
    root->right->addRight(nodes[7]);
    root->right->right->addLeft(nodes[8]);
    root->right->right->addRight(nodes[9]);
    std::cout << "Binary tree pre order\n";
    printBinaryPreOrder(root);
    std::cout << std::endl;
    std::cout << "Binary tree post order\n";
    printBinaryPostOrder(root);
    std::cout << std::endl;
    for (int i = 0; i < 10; ++i) {
        delete nodes[i];
    }
}

struct Node {
    int data;
    Node *parent = nullptr;
    std::vector<Node*> children;

    Node(int data) : data(data) {}

    void addNode(Node *node) {
        children.push_back(node);
        node->parent = this;
    }
};

void printPreOrder(Node *node) {
    if (node == nullptr) {
        return;
    }
    std::cout << node->data << " ";
    for (auto &child : node->children) {
        printPreOrder(child);
    }
}

void printPostOrder(Node *node) {
    if (node == nullptr) {
        return;
    }
    for (auto &child : node->children) {
        printPostOrder(child);
    }
    std::cout << node->data << " ";
}

void printTree() {
    std::vector<Node*> nodes;
    for (int i = 0; i < 10; ++i) {
        nodes.push_back(new Node(i));
    }
    nodes[0]->addNode(nodes[1]);
    nodes[0]->addNode(nodes[2]);
    nodes[0]->addNode(nodes[3]);
    nodes[1]->addNode(nodes[4]);
    nodes[1]->addNode(nodes[5]);
    nodes[2]->addNode(nodes[6]);
    nodes[3]->addNode(nodes[7]);
    nodes[3]->addNode(nodes[8]);
    nodes[7]->addNode(nodes[9]);
    std::cout << "Tree pre order\n";
    printPreOrder(nodes[0]);
    std::cout << std::endl;
    std::cout << "Tree post order\n";
    printPostOrder(nodes[0]);
    std::cout << std::endl;
    for (int i = 0; i < 10; ++i) {
        delete nodes[i];
    }
}

//int main() {
//    printBinaryTree();
//    printTree();
//    return 0;
//}