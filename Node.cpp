#include "Node.h"
#include <random>
#include <chrono>

/**
 * Constructs node object initialized with moved string parameter <code>value</code>.
 * Either a copy then move or a move and move if dev optimizes for it.
 * @param value
 */
Node::Node(std::string value) : num_children(0), val(std::move(value)) {}

/**
 * Attaches <code>node</code> to <code>this</code> node.
 * Rvalue reference binds to rvalue. Rvalue ref is an lvalue so
 * std::move casts node to rvalue to bind to push_back's rvalue ref param.
 * Move semantic order:
 * temporary binds to Node&& -> std::move casts to rvalue -> push_back bind -> implementation (moved)
 * The push_back called is push_back(value_type &&__x)
 * @param node
 */
void Node::attach(Node &&node) {
    vec.push_back(std::move(node));
    num_children++;
}

/**
 * Attaches node to this node. const Node& will bind to r or l values.
 * If rvalue ref funct active, this will not get the rvalue call. The
 * push_back called is push_back(const value_type &__x)
 */
void Node::attach(const Node &node) {
    vec.push_back(node);
    num_children++;
}

/**
 * Prints first layer children of <code>this</code> node.
 */
void Node::print_my_children() const {
    for (const auto &i : vec) {
        std::cout << i.get_val() << ' ';
    }
    std::cout << "\n";
}

/**
 * Extracts behaviour or response string value from calling node.
 * Root node's value <root> remains unchanged in this implementation.
 */
void Node::m_extractVal() {
    if (val.find("<root>") != std::string::npos || val.find("</node>") != std::string::npos) {
        std::size_t found = val.find_first_of("<>/");
        while (found != std::string::npos) {
            val[found] = '\0';                              // do nothing. Or use (char) 0, this is null char
            found = val.find_first_of("<>/", found + 1);
        }
        return;
    }
    if (val.find("behavior=\"\"") == std::string::npos) {   // we have behaviour
        std::string str = val; val.clear();
        std::size_t pos_b = str.find_first_of('"');
        pos_b++;
        while (str.at(pos_b) != '"') {
            val.push_back(str[pos_b]);
            pos_b++;
        }
        pos_b = val.find(" response=");
        if (pos_b != std::string::npos) {                   // found " response=", erase to end of string
            val.erase(pos_b);
        }
    }
    else {                                                  // we have response
        std::string str = val; val.clear();
        std::size_t pos_r = str.find_first_of('"', str.find("response="));
        pos_r++;                                            // places us 1 index after quote of response="
        while (str.at(pos_r) != '"') {
            val.push_back(str[pos_r]);
            pos_r++;

        }
    }
}

/**
 * Parses XML file and builds the decision tree.
 * File format must follow tree.xml's format.
 * @param file the xml file to read from
 * @return a n-ary decision tree
 */
Node build_tree_xml(std::fstream &file) {
    std::stack<Node> stack;
    std::string line; std::getline(file, line);
    stack.push(Node(line));
    while (stack.top().get_val().find("</root>") == std::string::npos) {
        if (stack.top().get_val().find("/>") != std::string::npos) {
            Node tmp1(stack.top());
            stack.pop();
            tmp1.m_extractVal();
            stack.top().attach(tmp1);
        }
        else if (stack.top().get_val().find("/node") != std::string::npos) {
            stack.pop();
            Node tmp2(stack.top());
            stack.pop();
            tmp2.m_extractVal();
            stack.top().attach(tmp2);
            // stack.top().attach(Node(line)); // testing attach(Node &&node)
        }
        std::getline(file, line);
        stack.push(Node(line));
    }
    stack.pop();        // pop </root>
    return stack.top(); // retrieve tree root node (last node in stack)
}

/**
 * Depth First Search - DFS
 * @param node
 * @param target
 * @return number of nodes visited
 */
void d_f_s(const Node &node, const std::string &target) {
    std::cout << "Target -> " << target << '\n';
    bool flag = false;
    int count = 0;
    std::stack<Node> stack;
    stack.push(node);
    while (!stack.empty()) {
        count += 1;
        Node tmp = stack.top();
        if (equals_ignore_case(tmp.get_val(), target)) {
            std::cout << "Status -> FOUND" << '\n';
            std::cout << "Travel -> " << count << " nodes" << '\n';
            pick_random_leaf(tmp);
            flag = true;
            break;
        }
        else {
            stack.pop();
            if (tmp.get_num_children() != 0) {
                for (auto i = static_cast<int>(tmp.get_vec().size() - 1); i >= 0; --i)
                    stack.push(tmp.get_vec()[i]);
            }
        }
    }
    if (!flag)
        std::cout << "Status -> N/A\nThe node \"" << target << "\" does not exist." << '\n';
}

/**
 * Breadth First Search - BFS
 * @param node
 * @param target
 * @return number of nodes visited
 */
void b_f_s(const Node &node, const std::string &target) {
    std::cout << "Target -> " << target << '\n';
    bool flag = false;
    int count = 0;
    std::queue<Node> queue;
    queue.push(node);
    while (!queue.empty()) {
        count += 1;
        Node tmp = queue.front();
        if (equals_ignore_case(tmp.get_val(), target)) {
            std::cout << "Status -> FOUND" << '\n';
            std::cout << "Travel -> " << count << " nodes" << '\n';
            pick_random_leaf(tmp);
            flag = true;
            break;
        }
        else {
            queue.pop();
            if (tmp.get_num_children() != 0) {
                for (const Node &i : tmp.get_vec())
                    queue.push(i);
            }
        }
    }
    if (!flag)
        std::cout << "Status -> N/A\nThe node \"" << target << "\" does not exist." << '\n';
}

/**
 * Selects random leaf node from given parent <code>node</code>.
 * This function is called twice, once in bfs, another in dfs.
 * @param node
 * @return
 */
Node pick_random_leaf(const Node &node) {
    std::vector<Node> response_vec;
    std::stack<Node> stack;
    stack.push(node);
    while (!stack.empty()) {
        Node tmp = stack.top();
        if (tmp.get_num_children() == 0) {
            response_vec.push_back(tmp);
            stack.pop();
        }
        else {
            stack.pop();
            for (const Node &i : tmp.get_vec()) {
                stack.push(i);      // unlike dfs, doesn't matter if leafs are reversed on stack
            }                       // since response_vec will choose them randomly.
        }
    }
    // nodes placed reversed on stack; reversed in vector so reverse iterator to print in order
    for (auto rit = response_vec.rbegin(); rit != response_vec.rend(); ++rit) {
        std::cout << "Option -> " << rit->get_val() << '\n';
    }
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 mt {seed};
    auto uid = std::uniform_int_distribution<int>(0, static_cast<int>(response_vec.size() - 1));
    int num = uid(mt);
    Node the_chosen_one = response_vec[num];
    std::cout << "Stochastic Response -> " << the_chosen_one.get_val() << '\n';
    return the_chosen_one;
}

/**
 * Case insensitive string comparison.
 * Custom string function that returns <code>true</code> if two strings are
 * equal (ignoring cases). False otherwise. Does not account for unicode, etc.
 * @param s1
 * @param s2
 * @return
 */
bool equals_ignore_case(const std::string &s1, const std::string &s2) { // consider move optimization
    size_t tmp = s1.size();
    if (s2.size() != tmp)
        return false;
    for (size_t i = 0; i < tmp; i++)
        if (std::tolower(s1[i]) != std::tolower(s2[i]))
            return false;
    return true;
}

/**
 * Returns list of possible actions to take within the decision tree.
 * @param root
 * @return
 */
std::vector<Node> action_list(const Node &root) {
    std::queue<Node> queue;
    std::vector<Node> vector;
    queue.push(root);
    while (!queue.empty()) {
        Node tmp = queue.front();
        vector.push_back(tmp);
        queue.pop();
        if (tmp.get_num_children() != 0) {
            for (const auto &node : tmp.get_vec()) {
                queue.push(node);
            }
        }
    }
    return vector;
}
