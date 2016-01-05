#ifndef PRIORITYQUEUE_HH_
#define PRIORITYQUEUE_HH_

#include <stddef.h>
#include <memory>
#include <set>

template<typename K, typename V>
class PriorityQueue{

    public:
        typedef size_t size_type;
        typedef K key_type;
        typedef V value_type;

        PriorityQueue();
        PriorityQueue(const PriorityQueue<K, V>& queue);
        PriorityQueue(PriorityQueue<K, V>&& queue);
        PriorityQueue<K, V>& operator=(PriorityQueue<K, V> &queue);
        PriorityQueue<K, V>& operator=(PriorityQueue<K, V> &&queue);

        bool empty() const;
        size_type size() const;
        void insert(const K& key, const V& value);

    private:

        struct compareVK{
            bool operator() (const std::shared_ptr<std::pair<K,V>>& lhs,
            const std::shared_ptr<std::pair<K,V>>& rhs) const{
                return (lhs->second == rhs->second) ?
                    lhs->first < rhs->first : lhs->second < rhs->second;
            }
        };

        std::set<std::shared_ptr<std::pair<key_type,value_type>>, compareVK > sortedSetVK;
        std::set<std::shared_ptr<std::pair<key_type,value_type>>> sortedSetK;


};


/******************** Constructors ********************/

/* default constructor */
template<typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(){
}

/* copy constructor of multiset has a strong exception guarantee */
template<typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(const PriorityQueue<K, V>& queue) :
    sortedSetVK(queue.sortedSetVK), sortedSetK(queue.sortedSetK){
}

/* move constructor - just swap our empty multisets for the passed queue's
 * multiset ... */
template<typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(PriorityQueue<K, V>&& queue){
    queue.sortedSetVK.swap(sortedSetVK);
    queue.sortedSetK.swap(sortedSetK);
}

/* assignment operator= for lvalues ... we just copy the multisets ... */
/*  1!) What happens to the sortedSetVK if it had some elements(possibly allocated with new)?
 *  2!) operator= provides basic exception guarantee */
template<typename K, typename V>
PriorityQueue<K, V>& PriorityQueue<K,V>::operator=(PriorityQueue<K, V> &queue){
    if(this != &queue){
        sortedSetVK = queue.sortedSetVK;
        sortedSetK = queue.sortedSetK;
    }
    return *this;
}

/* move assignment operator=(mainly for temporary objects being passed as a
 * parameter) */
/* 1!) What happens to queue.sortedSetVK after calling std::move on it? I hope it
    willl not call the destructor ... */
/* 2!) operator= provides basic exception guarantee */
template<typename K, typename V>
PriorityQueue<K, V>& PriorityQueue<K,V>::operator=(PriorityQueue<K, V> &&queue){
    if(this != &queue){
        sortedSetVK = std::move(queue.sortedSetVK);
        sortedSetK = std::move(queue.sortedSetK);
    }
    return *this;
}




template<typename K, typename V>
bool PriorityQueue<K,V>::empty() const{
    return sortedSetVK.empty();
}

/* 1!) typename keyword added */
template<typename K, typename V>
typename PriorityQueue<K,V>::size_type PriorityQueue<K,V>::size() const{
    return sortedSetVK.size();
}

template<typename K, typename V>
void PriorityQueue<K,V>::insert(const K& key, const V& value){
    auto ptr = std::make_shared<std::pair<K,V>>(key,value);
    sortedSetVK.insert(ptr);
    sortedSetK.insert(ptr);
}

#endif /* PRIORITYQUEUE_HH_ */

