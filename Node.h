#ifndef DECISION_TREE_2_NODE_H
#define DECISION_TREE_2_NODE_H

#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>

class Node {
public:
    // ctors
    explicit Node(std::string value);
    // operations
    void m_extractVal();
    void attach(Node &&node);
    void attach(const Node &node);
    void print_my_children() const;
    // getters
    std::string get_val() const { return val; }
    const std::vector<Node>& get_vec() const { return vec; }
    unsigned int get_num_children() const { return num_children; }
private:
    std::vector<Node> vec;
    std::string val;
    unsigned int num_children;
};

Node pick_random_leaf(const Node &node);
Node build_tree_xml(std::fstream &file);
std::vector<Node> action_list(const Node &root);
void d_f_s(const Node &node, const std::string &target);
void b_f_s(const Node &node, const std::string &target);
bool equals_ignore_case(const std::string &s1, const std::string &s2);

#endif //DECISION_TREE_2_NODE_H
