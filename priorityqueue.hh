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
        void swap(PriorityQueue<K, V>& queue);
        const V& minValue() const;
        const V& maxValue() const;
        const K& minKey() const;
        const K& maxKey() const;
        void deleteMin();
        void deleteMax();

        bool empty() const;
        size_type size() const;
        void insert(const K& key, const V& value);

    private:
        static long long inserted;
        typedef struct triple{
            K key;
            V val;
            long long counter;

            triple(const triple& trip){
                key(trip.key);
                val(trip.val);
                counter = trip.counter; //inserted??
            }

            triple(const K& k, const V& v, long long count) : key(k) , val(v), counter(count){
            }

        } triple;

        struct compareVKC{
            bool operator() (const std::shared_ptr<triple>& lhs,
            const std::shared_ptr<triple>& rhs) const{
                return lhs->val < rhs->val ?
                    (lhs->key < rhs->key ?
                        lhs->counter < rhs->counter: false)
                    : false;
            }
        };

        struct compareKC{
            bool operator() (const std::shared_ptr<triple>& lhs,
            const std::shared_ptr<triple>& rhs) const{
                return lhs->key < rhs->key ?
                    lhs->counter < rhs->counter : false;
            }
        };
        std::set<std::shared_ptr<triple>, compareVKC> sortedSetVKC;
        std::set<std::shared_ptr<triple>, compareKC> sortedSetKC;


};

//PriorityQueue<K,V>::inserted = 0; - initialize!!!!!!!!!!!!!!


/******************** Constructors ********************/

template<typename K, typename V>
bool PriorityQueue<K,V>::empty() const{
    return sortedSetVKC.empty();
}

/* 1!) typename keyword added */
template<typename K, typename V>
typename PriorityQueue<K,V>::size_type PriorityQueue<K,V>::size() const{
    return sortedSetVKC.size();
}

/* default constructor */
template<typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(){
}

/* move constructor - just swap our empty multisets for the passed queue's
 * multiset ... */
template<typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(PriorityQueue<K, V>&& queue){
    queue.sortedSetVKC.swap(sortedSetVKC);
    queue.sortedSetKC.swap(sortedSetKC);
}

template<typename K, typename V>
void PriorityQueue<K,V>::insert(const K& key, const V& value){
    auto ptr = std::make_shared<triple>(key, value, inserted);
    inserted++;
    sortedSetVKC.insert(ptr);
    sortedSetKC.insert(ptr);
}

template<typename K, typename V>
const V& PriorityQueue<K,V>::minValue() const{
    if(sortedSetVKC.empty()){
        ;//throw wyjatek
    }
    return (*sortedSetVKC.begin())->val;
}

template<typename K, typename V>
const V& PriorityQueue<K,V>::maxValue() const{
    if(sortedSetVKC.empty()){
        ;//throw wyjatek
    }
    return (*sortedSetVKC.rbegin())->val;
}

template<typename K, typename V>
const K& PriorityQueue<K,V>::minKey() const{
    if(sortedSetVKC.empty()){
        ;//throw wyjatek
    }

    return (*sortedSetVKC.begin())->key;
}

template<typename K, typename V>
const K& PriorityQueue<K,V>::maxKey() const{
    if(sortedSetVKC.empty()){
        ;//throw wyjatek
    }
    return (*sortedSetVKC.rbegin())->key;
}

template<typename K, typename V>
void PriorityQueue<K,V>::deleteMin(){
    if(sortedSetVKC.empty())
        return;
    auto itVKC = sortedSetVKC.begin();
    auto itKC = sortedSetKC.find(*itVKC);
    sortedSetVKC.erase(itVKC);
    sortedSetKC.erase(itKC);
}

template<typename K, typename V>
void PriorityQueue<K,V>::deleteMax(){
    if(sortedSetVKC.empty())
        return;
    auto itVKC = sortedSetVKC.rbegin();
    auto itKC = sortedSetKC.find(*itVKC);
    sortedSetVKC.erase(itVKC);
    sortedSetKC.erase(itKC);
}


/* copy constructor of map and set has strong exception guarantee */
template<typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(const PriorityQueue<K, V>& queue){
    for(auto it = queue.sortedSetVKC.begin(); it != queue.sortedSetVKC.end(); ++it){
        auto ptr = std::make_shared<triple>(*it);

    }
}








/* move assignment operator=(mainly for temporary objects being passed as a
 * parameter) */
/* 1!) What happens to queue.sortedSetVK after calling std::move on it? I hope it
    willl not call the destructor ... */
/* 2!) operator= provides basic exception guarantee */
template<typename K, typename V>
PriorityQueue<K, V>& PriorityQueue<K,V>::operator=(PriorityQueue<K, V> &&queue) {
    if(this != &queue) {
        PriorityQueue <K, V> new_one(queue);
        this->swap(new_one);
    }
    return *this;
}




/* assignment operator= for lvalues ... we just copy the multisets ... */
/*  1!) What happens to the sortedSetVK if it had some elements(possibly allocated with new)?
 *  2!) operator= provides basic exception guarantee */
/*template<typename K, typename V>
PriorityQueue<K, V>& PriorityQueue<K,V>::operator=(PriorityQueue<K, V> &queue){
    if(this != &queue){
        sortedSetVK = queue.sortedSetVK;
        sortedSetK = queue.sortedSetK;
    }
    return *this;
}*/






template<typename K, typename V>
void PriorityQueue<K,V>::swap(PriorityQueue<K, V>& queue){
    if(this != &queue){
        queue.sortedSetVKC.swap(sortedSetVKC);
        queue.sortedSetKC.swap(sortedSetKC);
    }
}


#endif /* PRIORITYQUEUE_HH_ */

