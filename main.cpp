#include <iostream>
#include "priorityqueue.hh"

int main()
{
    PriorityQueue<int,double> pqueue1;
    pqueue1.insert(1,2.0);
    PriorityQueue<int,double> pqueue2(pqueue1);
    pqueue1.insert(2,2.2);


    std::cout << pqueue1.size() << std::endl;
    std::cout << pqueue2.size() << std::endl;
    /*std::cout << "Created a shared Derived (as a pointer to Base)\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';*/
}

