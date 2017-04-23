

#include "Node.h"
#include <sstream>
#include <chrono>

int main() {

    std::cout << "\nThis is a decision tree.\nSearch tree for stochastic response. Enter \"000\" to terminate program.\n";

    /** BUILDING TREE */
    std::fstream file("../tree.xml");
    Node root = build_tree_xml(file);

    std::cout << "\n----- Available actions -----\n";
    std::vector<Node> avec = action_list(root);
    for (Node i : avec) {
        std::cout << i.get_val() << '\n';
    }
    std::cout << '\n';


    const char *c = "000";
    std::string str;
    std::getline(std::cin, str);

    while (!equals_ignore_case(str, c)) {

        std::cout << "\n=========== SEARCHING DECISION TREE ===========\n";


        std::cout << "\n----- DFS - Depth First Search -----\n";
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
        std::cout << "milliseconds: " << final_BrE.time_since_epoch().count() / 1000000.0 << std::endl;
        std::cout << "seconds: " << final_BrE.time_since_epoch().count() / 1000000000.0 << std::endl;


        std::cout << '\n';
        std::getline(std::cin, str);
    }


    /**-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
    *                                *
    *            Belugas!            *
    *                                *
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/


    return 0;
}
