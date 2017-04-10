

#ifndef DECISION_TREE_2_NODE_H
#define DECISION_TREE_2_NODE_H

#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <sstream>
#include <ctime>

class Node {
public:
    Node();
    Node(std::string value, Node &parent);
    Node(std::string value);
    std::string get_val() const { return val; }
    int get_num_children() const { return num_children; }
    std::vector<Node *> get_Vec() { return v; }
    void attach(Node &node);
    void print_node_children() const;
    void m_extractVal();
    Node&operator=(const Node& other);

private:
    std::vector<Node *> v;
    std::string val;
    int num_children;

};

int d_f_s(Node &node, std::string &target);
int b_f_s(Node &node, std::string &target);
Node pick_random(Node &n);



#endif //DECISION_TREE_2_NODE_H
