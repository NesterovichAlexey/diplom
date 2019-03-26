//
// Created by Aliaksei Nestsiarovich on 2019-03-04.
//

#ifndef DIPLOM_TREETRAVERSAL_H
#define DIPLOM_TREETRAVERSAL_H

#include <iostream>
#include <stack>

struct Node {
    void* data;
    Node* left;
    Node* right;
};

// node, left, right
void preOrder(Node* tree) {
    std::stack<Node*> stack;
    stack.push(tree);
    while (stack.empty() == false) {
        Node* cur = stack.top();
        stack.pop();
        std::cout << cur->data << std::endl;
        stack.push(cur->right);
        stack.push(cur->left);
    }
}

// left, node, right
void inOrder(Node* tree) {
    std::stack<Node*> stack;
    Node* cur = tree;
    while (stack.empty() == false || cur != nullptr) {
        if (cur != nullptr) {
            stack.push(cur);
            cur = cur->left;
        } else {
            cur = stack.top();
            stack.pop();
            std::cout << cur->data << std::endl;
            cur = cur->right;
        }
    }
}

// left, right, node
void postOrder(Node* tree) {
    std::stack<Node*> stack;
    Node* prev = nullptr;
    Node* cur = tree;
    while (stack.empty() == false || cur != nullptr) {
        if (cur != nullptr) {
            stack.push(cur);
            cur = cur->left;
        } else {
            Node* peekNode = stack.top();
            if (peekNode->right != nullptr && prev != peekNode->right) {
                cur = peekNode->right;
            } else {
                std::cout << peekNode->data << std::endl;
                prev = peekNode;
                stack.pop();
            }
        }
    }
}

#endif //DIPLOM_TREETRAVERSAL_H
