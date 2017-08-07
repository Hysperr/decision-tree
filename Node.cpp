

#include <cassert>
#include <ctime>
#include "Node.h"

/**
 * Constructs DEFAULT node object. "-".
 */
Node::Node() : num_children(0), val("-"){}

/**
 * Constructs node object initialized with string parameter <code>value</code>.
 * @param value
 */
Node::Node(const std::string &value) : num_children(0), val(value) {}

/**
 * Constructs node object initialized with string value
 * and is attached to parent node parent <code>this</code>.
 * @param value
 * @param parent
 */
Node::Node(std::string value, Node &parent) : num_children(0), val(std::move(value)) {
    parent.attach(*this);
}



/**
 * Overloaded assignment operator.
 * @param other
 * @return
 */
Node &Node::operator=(const Node &other) {
    v = other.v;
    val = other.val;
    num_children = other.num_children;
    return *this;
}

/**
 * Attaches <code>node</code> to <code>this</code> node.
 * Improved.
 * @param node
 */
void Node::attach(Node &node) {
    Node *nptr = &node;
    v.push_back(nptr);
    num_children++;
}

/**
 * Prints first layer children of <code>this</code> node.
 */
void Node::print_my_children() const {
    for (Node *i : v) {
        std::cout << i->get_val() << std::ends;
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
            val[found] = '\0';                                  // do nothing. Or use (char) 0, this is null char
            found = val.find_first_of("<>/", found + 1);

        }
        return;
    }

    if (val.find("behavior=\"\"") == std::string::npos) {      // we have behaviour
        std::string str = val; val.clear();
        std::size_t pos_b = str.find_first_of('"');
        pos_b++;

        while (str.at(pos_b) != '"') {
            val.push_back(str[pos_b]);
            pos_b++;

        }
        pos_b = val.find(" response=");
        if (pos_b != std::string::npos)                         // found " response=", erase to end of string
            val.erase(pos_b);

    }
    else {                                                      // we have response
        std::string str = val; val.clear();
        std::size_t pos_r = str.find_first_of('"', str.find("response="));
        pos_r++;                                                // places us 1 index after quote of response="

        while (str.at(pos_r) != '"') {
            val.push_back(str[pos_r]);
            pos_r++;

        }
    }
}

/**
 * Parses XML file and builds the decision tree.
 * File format must follow tree.xml's format.
 * @param file
 * @return
 */
Node build_tree_xml(std::fstream &file) {
    std::stack<Node> stack;
    std::string line; std::getline(file, line);
    Node *r0 = new Node(line);
    stack.push(*r0);
    while (stack.top().get_val().find("</root>") == std::string::npos) {

        if (stack.top().get_val().find("/>") != std::string::npos) {
            Node *tmp1 = new Node(stack.top());
            stack.pop();
            tmp1->m_extractVal();
            stack.top().attach(*tmp1);

        }
        else if (stack.top().get_val().find("/node") != std::string::npos) {
            stack.pop();
            Node *tmp2 = new Node(stack.top());
            stack.pop();
            tmp2->m_extractVal();
            stack.top().attach(*tmp2);

        }

        std::getline(file, line);
        Node *tmp3 = new Node(line);
        stack.push(*tmp3);

    }

    stack.pop();                                // pop </root>
    Node tree_root = stack.top();               // retrieve root node
    stack.pop();                                // clear stack
    assert(stack.empty());                      // check stack empty
    return tree_root;
}

/**
 * Depth First Search - DFS
 *
 * @param node
 * @param target
 * @return number of nodes visited
 */
void d_f_s(Node &node, const std::string &target) {
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
                for (int i = (int) (tmp.get_Vec().size() - 1); i >= 0; i--)
                    stack.push(*tmp.get_Vec()[i]);
            }
        }
    }

    if (!flag)
        std::cout << "Status -> N/A\nThe node \"" << target << "\" does not exist." << '\n';
}

/**
 * Breadth First Search - BFS
 *
 * @param node
 * @param target
 * @return number of nodes visited
 */
void b_f_s(Node &node, const std::string &target) {
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
                for (Node *i : tmp.get_Vec())
                    queue.push(*i);
            }
        }
    }

    if (!flag)
        std::cout << "Status -> N/A\nThe node \"" << target << "\" does not exist." << '\n';
}

/**
 * Selects random leaf node from given parent <code>node</code>.
 * This function is called twice, once in bfs, another in dfs.
 * Seed is initialized with time(NULL) inside function. Thus,
 * a call to rand() uses same seed which is desired for this
 * program; we want dfs and bfs to select same node.
 * @param node
 * @return
 */
Node pick_random_leaf(const Node &node) {
    srand((unsigned int) time(NULL));       // must be here so bfs & dfs use same seed.
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

            for (auto &item : tmp.get_Vec()) {
                stack.push(*item);                  // unlike dfs, doesn't matter if leafs are reversed on stack
            }                                       // since response_vec will choose them randomly.
        }
    }

    // nodes placed reversed on stack so reverse iterator to print in order
    std::vector<Node>::reverse_iterator rit = response_vec.rbegin();
    for (; rit != response_vec.rend(); ++rit)
        std::cout << "Option -> " << rit->get_val() << '\n';

    int num = (int) (rand() % response_vec.size());

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
bool equals_ignore_case(const std::string &s1, const std::string &s2) {
    unsigned long long tmp = s1.size();     // or use size_t since 8 bytes like ULL
    if (s2.size() != tmp)
        return false;
    for (unsigned long long i = 0; i < tmp; i++)
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
            for (auto &node : tmp.get_Vec()) {
                queue.push(*node);
            }
        }
    }
    return vector;
}
