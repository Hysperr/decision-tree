#define NDEBUG


#include "Node.h"
#include <stack>
#include <sstream>
#include <fstream>
#include <cassert>

int main() {

    Node s0("hello");
    Node s1;
    s0.attach(s1);
    assert(s1.get_Vec().empty());
    assert(!s0.get_Vec().empty());
    std::cout << "s0 size is " << s0.get_Vec().size() << std::endl;     // 1
    std::cout << "s1 size is " << s1.get_Vec().size() << std::endl;     // 0

    Node n0;    // value is 100 by default
    Node n1;
    n0.attach(n1);
    std::cout << n0.get_val() << std::endl;                     // passed       100

    // attaching 2 more nodes to root
    Node n2, n3;
    n0.attach(n2);
    n0.attach(n3);


    // retrieve root's num_children
    std::cout << n0.get_num_children() << std::endl;            // passed       3


    // Cause reallocation in attach function. (because greater than 3 pointers required)
    Node n4;
    n0.attach(n4);
    std::cout << n0.get_num_children() << std::endl;            // passed       4


    // Testing get_Vec()
    std::vector<Node *> v = n0.get_Vec();
    // now list values of each child of this vector
    for (Node *i : v)
        std::cout << i->get_val() << std::ends;
    std::cout << "\n";
                                                                // passed       100, 100, 100, 100

    // Test print_node_children() function
    n0.print_node_children();                                   // passed       100, 100, 100, 100


    // Attach layer 3 node (n5) to root's child (layer 2) then print layer 2's num children
    Node n5;
    n1.attach(n5);
    std::cout << n1.get_num_children() << std::endl;            // passed       1


    // New node with custom value and parent
    Node n6("Miko", n0);
    std::cout << n0.get_num_children() << std::endl;            // passed       5
    std::cout << n6.get_val() << std::endl;                     // passed       Miko


    std::cout << "======== CORE FUNCTIONALITY COMPLETE ========\nSuccess!\n";


    /** BUILDING TREE */

    std::stack<Node> stack;
    std::fstream file("../tree.xml");
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

    stack.pop();                                                // pop </root>
    Node root = stack.top();                                    // retrieve root node
    stack.pop();                                                // clear stack
    assert(stack.empty());                                      // check stack empty


//    std::cout << root.get_val() << std::endl;                 // for debug
//    std::cout << root.get_num_children() << std::endl;
//    root.print_node_children();
//    std::cout << '\n';


    std::cout << "=========== SEARCHING ===========\n";

    /**
     *  V1 - Option for User-entered string.
     *
    std::cout << "Enter name of node ->";
    std::string str;
    std::getline(std::cin, str);
    */



    /**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
     *                                *
     *   Manage String input Below!   *
     *                                *
     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


    std::string str = "Combat";
//    std::string str = "Beluga!";



    std::cout << "DFS - Depth First Search\n";
    std::cout << d_f_s(root, str) << " nodes visited." << std::endl;
    std::cout << '\n';
    std::cout << "BFS - Breadth First Search\n";
    std::cout << b_f_s(root, str) << " nodes visited." << std::endl;



    return 0;
}



/** BELOW IS THE CODE EQUIVALENT OF SCRATCH WORK USED THROUGH THE PROJECT **/


/*  Testing stack references. Conclusion follows.
 *
 *  using namespace std;
    cout << '\n';
    std::stack<int> mm;
    mm.push(1);
    mm.push(2);
    mm.push(3);
    cout << "size: " << mm.size() << endl;
    cout << "top: " << mm.top() << endl;
    while (mm.size() > 1) {
        cout << mm.top() << endl;
        mm.pop();
    }
    cout << '\n';
    int x = mm.top();
    cout << "top: " << mm.top() << endl;
    mm.pop();
    cout << x << endl;
    cout << mm.size() << endl;
    cout << x << endl;
*/
/*  Conclusion:

    Let stack.size() == 1;
    stack.top() returns reference top of stack.
    if it stack.pop() is called, the stack is empty such that stack.size() == 0;
    however, if variable x was assigned to stack.top() before the pop, then x retains
    its value even after the pop. The value still exists in memory. It's just not in the
    stack anymore.

    A reference var is an alias for another var - they share same memory cell.
*/

/*
 *  empty containers are value initialized, which defined by standard to be 0
    thus if condition is true.

    std::vector<Node *> vvv(5);
    if (vvv[0] == nullptr) {    // or 0 or NULL or (void *) 0
        std::cout << "I'm null\n";
    }
    std::cout << "vvv size is: " << vvv.size() << std::endl;            // 5
    std::cout << "vvv capacity is: " << vvv.capacity() << std::endl;    // 5
    // Found within stl_vector.h line 722 directive #if __cplusplus >= 201103L  It shrinks capacity down to size.
    // http://stackoverflow.com/questions/11053960 it is C++ 2011 standard approved by committee on 2011 March 3.
    vvv.reserve(10);                                                    // reserve total is 10
    std::cout << "vvv size is: " << vvv.size() << std::endl;            // 5
    std::cout << "vvv capacity is: " << vvv.capacity() << std::endl;    // 10
 */


/*  Testing unnecessarily weird things.
 *
    std::cout << v_def[0].get_val() << std::endl;
    std::cout << v_def[0].get_num_children() << std::endl;
    std::vector<Node *> n = v_def[0].get_Vec();
    for (Node *nn : n) {
        std::cout << nn->get_val() << std::ends;
    }
    std::cout << "\n";
*/


/* Testing whether deleting var also deletes its reference. It does not.
 *
 *
 *  std::stack<Node> s;
    Node nn;

    s.push(nn);
    Node &nnn = nn;

    s.pop();

    std::cout << nnn.get_val() << '\n';
    std::cout << s.size() << '\n';
 *
 *
 */


/* Testing file I/O
 *
 *
    std::ofstream ofstream("example.txt");
    if (ofstream.is_open()) {
        ofstream << "This is a line. \n";
        ofstream << "This is another line. \n";
        ofstream.close();
    }
    else
        std::cout << "Unable to open file \n";


    std::ifstream file("example.txt");
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::cout << line << '\n';
        }
    }
 *
 */


/* Stringstream best used when formatting other kinds of data besides strings.
 *
 * std::stringstream iss(line);
 * std::cout << iss.str() << "\n";
 *
 */


/* Testing stack operations. Top() checks top of stack. Pop() removes top of stack.
 *
 *  std::stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    size_t size = s.size();

    for (int i = 0; i < size; i++) {
        std::cout << s.top() << std::ends;
        s.pop();
    }

    std::cout << "\n";
 */


// I could have another constructor with val and where it points. But could quickly become circular or unwieldy.

/* Testing get_Val() function. (old)
 * int get_val() const { if (this) return val; else return -1; }     // or use, return this ? val : -1;
*
    std::vector<Node *> v = n1.get_Vec();
    std::cout << v[1]->get_val() << std::endl;

    for (int i = 0; i < v.size(); i++)
        std::cout << v[i]->get_val() << std::endl;

    for (Node *i : v)
        std::cout << i->get_val() << std::endl;
*/
