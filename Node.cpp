

#include <cassert>
#include <ctime>
#include "Node.h"


/**
 * Constructs DEFAULT node object. Note: val is "100" for debug.
 *
 * @param none
 * @return none
 */
Node::Node() : num_children(0), val("100"), v() {}


/**
 * Constructs node object initialized with string value
 * and is attached to parent node parent.
 *
 * @param value
 * @param parent
 */
Node::Node(std::string value, Node &parent) : num_children(0), val(value), v() {
    parent.attach(*this);       // 'this' is pointer to object's self. Dereferenced to satisfy attach() ref parameter
}

/**
 * Constructs node object initialized with string value
 *
 * @param value
 */
Node::Node(const std::string &value) : num_children(0), val(value), v() {}

/**
 * Overloaded assignment operator= use primarily for debug
 *
 * @param other (returns the modified calling node)
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
 *
 * @param node
 * @return none
 */
void Node::attach(Node &node) {
    Node *nptr = &node;
    v.push_back(nptr);
    num_children++;
}

/**
 * Prints first layer children of <code>this</code> node
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
 *
 * @param none
 * @return none
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
int d_f_s(Node &node, std::string &target) {
    std::cout << "Target -> " << target << '\n';
    int count = 0;
    bool flag = false;
    std::stack<Node> stack;
    stack.push(node);

    while (!stack.empty()) {
        count += 1;
        Node tmp = stack.top();
        /**std::cout << tmp.get_val() << std::endl;*/           // for debug

        if (tmp.get_val() == target) {
            std::cout << "Status -> FOUND" << '\n';
            pick_random(tmp);
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
    if (!flag) {
        std::cout << "Status -> N/A\n";
        std::cout << "The node \"" << target << "\" does not exist." << '\n';
        return count;
    }

    return count;
}


/**
 * Breadth First Search - BFS
 *
 * @param node
 * @param target
 * @return number of nodes visited
 */
int b_f_s(Node &node, std::string &target) {
    std::cout << "Target -> " << target << '\n';
    bool flag = false;
    int count = 0;
    std::queue<Node> queue;
    queue.push(node);

    while (!queue.empty()) {
        count += 1;
        Node tmp = queue.front();
        /**std::cout << tmp.get_val() << std::endl;*/               // for debug

        if (tmp.get_val() == target) {
            std::cout << "Status -> FOUND" << '\n';
            pick_random(tmp);
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

    if (!flag) {
        std::cout << "Status -> N/A\n";
        std::cout << "The node \"" << target << "\" does not exist." << '\n';
        return count;

    }

    return count;
}


/**
 * Note: commented code at bottom for debug but also very helpful
 * to see list of possible leafs that could be chosen.
 *
 * Selects random leaf node from given parent {&node}. Seed is
 * initialized with time(NULL) inside pick_random. This function
 * is called twice, once in bfs, another in dfs. Race condition
 * started for identical output. Should always be same output though. In case of
 * different values (which has never occurred with my machine),
 * simply take out call from either dfs or bfs but not both. This
 * fulfills the same requirements with extra safety.
 *
 * @param node
 * @return the leaf node.
 */
Node pick_random(Node &node) {
    srand((unsigned int) time(NULL));       // must be here so bfs & dfs use same seed.
    std::vector<Node> vector;               /**< Holds node positions for printing & stochastic selection */
    std::stack<Node> stack;                 /**< For diving through tree. Not depth first! */
    stack.push(node);

    while (!stack.empty()) {
        Node tmp = stack.top();
        if (tmp.get_num_children() == 0) {
            vector.push_back(tmp);
            stack.pop();

        }
        else {
            stack.pop();                            // pop tmp since not a leaf. Warning! Do not place after for loop
                                                    // bad time friend ahead. pop a leaf and enter infinite loop!
            for (auto item : tmp.get_Vec()) {
                stack.push(*item);                  // unlike dfs, doesn't matter if leafs are reversed on stack
            }                                       //  since vector will choose them randomly
        }
    }

    // because nodes placed backwards on stack (not depth first), I use reverse_iterator to print in order.
    // If changed to depth first, use normal iterator.
    std::vector<Node>::reverse_iterator rit = vector.rbegin();
    for (; rit != vector.rend(); ++rit)
        std::cout << "Option -> " << rit->get_val() << '\n';

    int num = (int) (rand() % vector.size());

    Node the_chosen_one = vector[num];

    std::cout << "Stochastic Response -> " << the_chosen_one.get_val() << '\n';

    return the_chosen_one;
}
