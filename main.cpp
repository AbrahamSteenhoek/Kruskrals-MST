#include <vector>
#include <iostream>
#include <ostream>
#include <queue>
#include "adjList.h"


void unitTests() {
    cout << "UNIT TESTS" << endl;
    AdjList<int> graph(cin);
    cout << "adjlist pq:" << endl;
    graph.printPQ();
    graph.minpq.pop();
    graph.printPQ();

    graph.print(cout);
    graph.printMST();

    cout << "END UNIT TESTS" << endl;
}

int main() {
    //unitTests();
    AdjList<int> graph(cin);
    graph.printMST();
    return 0;
}