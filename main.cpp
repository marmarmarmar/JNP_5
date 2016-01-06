
#include <iostream>
#include <exception>
#include <cassert>

#include "priorityqueue.hh"

template<>
long long PriorityQueue<int,double>::inserted = 0;

template<>
long long PriorityQueue<int,int>::inserted = 0;

PriorityQueue<int, int> f(PriorityQueue<int, int> q)
{
    return q;
}

int main() {
    PriorityQueue<int, int> P = f(PriorityQueue<int, int>());
    assert(P.empty());

    P.insert(1, 42);
    P.insert(2, 13);

    assert(P.size() == 2);
    assert(P.maxKey() == 1);
    assert(P.maxValue() == 42);
    assert(P.minKey() == 2);
    assert(P.minValue() == 13);

    PriorityQueue<int, int> Q(f(P));
    Q.deleteMax();
    Q.deleteMin();
    Q.deleteMin();
    assert(Q.empty());

    PriorityQueue<int, int> R(Q);
std::cout << P.size() << std::endl;
    R.insert(1, 100);
    R.insert(2, 100);
    R.insert(3, 300);

    PriorityQueue<int, int> S;
    S = R;
std::cout << P.size() << std::endl;

    S.changeValue(4, 400);


    S.changeValue(2, 200);



    return 0;
}
