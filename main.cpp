
#define NDEBUG


#include "Node.h"
#include <sstream>
#include <chrono>

int main() {

    /** BUILDING TREE */
    std::fstream file("../tree.xml");
    Node root = build_tree_xml(file);

    std::cout << "\n=========== SEARCHING DECISION TREE ===========\n\n";

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


    std::string str = "<root>";
//    std::string str = "Beluga!";



    std::cout << "----- DFS - Depth First Search -----\n";
    std::chrono::steady_clock::time_point begin_dfs = std::chrono::steady_clock::now();

    std::cout << d_f_s(root, str) << " node(s) visited." << std::endl;

    // clockwork
    std::cout << "----- DFS Elapsed Time: -----\n";
    std::chrono::steady_clock::time_point end_dfs = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point final_dfs(end_dfs - begin_dfs);
    std::cout << "nanoseconds: " << std::chrono::duration_cast<std::chrono::nanoseconds>(final_dfs.time_since_epoch()).count() << std::endl;
    std::cout << "milliseconds: " << final_dfs.time_since_epoch().count() / 1000000.0 << std::endl;     // 1 million
    std::cout << "seconds: " << final_dfs.time_since_epoch().count() / 1000000000.0 << std::endl;       // 1 billion
    ///
    ///
    std::cout << "\n----- BFS - Breadth First Search -----\n";
    std::chrono::steady_clock::time_point begin_BrE = std::chrono::steady_clock::now();

    std::cout << b_f_s(root, str) << " node(s) visited." << std::endl;

    // clockwork
    std::cout << "----- BFS Elapsed Time: -----\n";
    std::chrono::steady_clock::time_point end_BrE = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point final_BrE(end_BrE - begin_BrE);
    std::cout << "nanoseconds: " << std::chrono::duration_cast<std::chrono::nanoseconds>(final_BrE.time_since_epoch()).count() << std::endl;
    std::cout << "milliseconds: " << final_BrE.time_since_epoch().count() / 1000000.0 << std::endl;     // 1 million
    std::cout << "seconds: " << final_BrE.time_since_epoch().count() / 1000000000.0 << std::endl;       // 1 billion




    return 0;
}



/** SCRATCH WORK BELOW **/

/*
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<std::endl;
 */

/*  Testing stack references. Conclusion follows. Place into gist eventually.
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
