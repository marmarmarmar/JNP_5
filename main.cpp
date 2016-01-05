#include <iostream>
#include "priorityqueue.hh"

using namespace std;
int main()
{
    PriorityQueue<int,double> pqueue1;
    pqueue1.insert(1,2.0);
    PriorityQueue<int,double> pqueue2(pqueue1);
    pqueue1.insert(2,2.2);
    auto ptr = std::make_shared<std::pair<int,double>>(1,2.0);
    std::shared_ptr<std::pair<int,double>> * pt = &ptr;
    //pt = ptr;

    cout << ptr << " " << *pt << endl;
    std::cout << pqueue1.size() << std::endl;
    std::cout << pqueue2.size() << std::endl;
    /*std::cout << "Created a shared Derived (as a pointer to Base)\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';*/
}

