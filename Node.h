

#ifndef DECISION_TREE_2_NODE_H
#define DECISION_TREE_2_NODE_H

#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>

class Node {
public:
    /// ctors
    Node();
    explicit Node(const std::string &value);
    Node(std::string value, Node &parent);
    /// operations
    void m_extractVal();
    void attach(Node &node);
    void print_my_children() const;
    Node&operator=(const Node &other);
    /// getters
    std::string get_val() const { return val; }
    std::vector<Node *> get_Vec() const { return v; }
    int get_num_children() const { return num_children; }
private:
    std::vector<Node *> v;
    std::string val;
    int num_children;
};

Node pick_random_leaf(const Node &node);
Node build_tree_xml(std::fstream &file);
std::vector<Node> action_list(const Node &root);
void d_f_s(Node &node, const std::string &target);
void b_f_s(Node &node, const std::string &target);
bool equals_ignore_case(const std::string &s1, const std::string &s2);



#endif //DECISION_TREE_2_NODE_H
