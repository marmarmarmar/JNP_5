#ifndef PRIORITYQUEUE_HH_
#define PRIORITYQUEUE_HH_

#include <stddef.h>
#include <memory>
#include <set>
#include <exception>

class PriorityQueueEmptyException : public exception{
    public:
        virtual const char* what() const throw()
        {
            return "PriorityQueue empty exception";
        }
};

class PriorityQueueNotFoundException : public exception{
    public:
        virtual const char* what() const throw()
        {
            return "PriorityQueue not found exception";
        }
};

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
        void changeValue(const K& key, const V& value);
        void merge(PriorityQueue<K, V>& queue);
        bool operator<(const PriorityQueue<K,V>& other) const;

        bool empty() const;
        size_type size() const;
        void insert(const K& key, const V& value);

    private:
        void deleteKey(const K& k);
        typedef struct pairKV{
            K key;
            V val;

            pairKV(const pairKV& pKV){
                key(pKV.key);
                val(pKV.val);
            }

            pairKV(const K& k, const V& v) : key(k) , val(v){
            }
        } pairKV;

        struct compareVK{
            bool operator() (const std::shared_ptr<pairKV>& lhs,
            const std::shared_ptr<pairKV>& rhs) const{
                if(lhs->val < rhs->val)
                    return true;
                else if(lhs->val > rhs->val)
                    return false;
                if(lhs->key < rhs->key)
                    return true;
                else if(lhs->key > rhs->key)
                    return false;
                return false;

            }
        };

        struct compareKV{
            bool operator() (const std::shared_ptr<pairKV>& lhs,
            const std::shared_ptr<pairKV>& rhs) const{
                if(lhs->key < rhs->key)
                    return true;
                else if(lhs->key > rhs->key)
                    return false;
                if(lhs->val < rhs->val)
                    return true;
                else if(lhs->val > rhs->val)
                    return false;
                return false;
            }
        };
        std::multiset<std::shared_ptr<pairKV>, compareVK> sortedSetVK;
        std::multiset<std::shared_ptr<pairKV>, compareKV> sortedSetKV;


};

/******************** Constructors ********************/

/* default constructor */
template<typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(){
}

/* copy constructor of map and set has strong exception guarantee */
template<typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(const PriorityQueue<K, V>& queue) :
sortedSetVK(queue.sortedSetVK), sortedSetKV(queue.sortedSetKV){
}

/* move constructor - just swap our empty multisets for the passed queue's
 * multiset ... */
template<typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(PriorityQueue<K, V>&& queue){
    queue.sortedSetVK.swap(sortedSetVK);
    queue.sortedSetKV.swap(sortedSetKV);
}

/* move assignment operator=(mainly for temporary objects being passed as a
 * parameter) */
/* 1!) What happens to queue.sortedSetVK after calling std::move on it? I hope it
    willl not call the destructor ... */
/* 2!) operator= provides basic exception guarantee */
template<typename K, typename V>
PriorityQueue<K, V>& PriorityQueue<K,V>::operator=(PriorityQueue<K, V> &&queue){
    if(this != &queue) {
        PriorityQueue<K,V> new_one(queue);
        this->swap(new_one);
    }
    return *this;
}

/* assignment operator= for lvalues ... we just copy the multisets ... */
/*  1!) What happens to the sortedSetVK if it had some elements(possibly allocated with new)?
 *  2!) operator= provides basic exception guarantee */
template<typename K, typename V>
PriorityQueue<K, V>& PriorityQueue<K,V>::operator=(PriorityQueue<K, V> &queue){
    if(this != &queue){
        PriorityQueue<K, V> new_one(queue);
        this->swap(new_one);
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
    auto ptr = std::make_shared<pairKV>(key, value);
    sortedSetVK.insert(ptr);
    sortedSetKV.insert(ptr);
}

template<typename K, typename V>
const V& PriorityQueue<K,V>::minValue() const{
    if(sortedSetVK.empty()){
        throw PriorityQueueEmptyException();
    }
    return (*sortedSetVK.begin())->val;
}

template<typename K, typename V>
const V& PriorityQueue<K,V>::maxValue() const{
    if(sortedSetVK.empty()){
        throw PriorityQueueEmptyException();
    }
    return (*sortedSetVK.rbegin())->val;
}


template<typename K, typename V>
const K& PriorityQueue<K,V>::minKey() const{
    if(sortedSetVK.empty()){
        throw PriorityQueueEmptyException();
    }
    return (*sortedSetVK.begin())->key;
}

template<typename K, typename V>
const K& PriorityQueue<K,V>::maxKey() const{
    if(sortedSetVK.empty()){
        throw PriorityQueueEmptyException();
    }
    return (*sortedSetVK.rbegin())->key;
}

template<typename K, typename V>
void PriorityQueue<K,V>::deleteMin(){
    std::cout << "before deleteMin: "<<sortedSetVK.size() << std::endl;
    if(sortedSetVK.empty())
        return;
    auto itVK = sortedSetVK.begin();
    auto itKV = sortedSetKV.find(*itVK);
    sortedSetVK.erase(itVK);
    sortedSetKV.erase(itKV);
    std::cout << "after deleteMin: "<<sortedSetVK.size() << std::endl;
}

template<typename K, typename V>
void PriorityQueue<K,V>::deleteMax(){
    std::cout << "before deleteMax: "<<sortedSetVK.size() << std::endl;
    if(sortedSetVK.empty())
        return;
    auto itVK = sortedSetVK.end();
    --itVK;
    auto itKV = sortedSetKV.find(*itVK);
    std::cout << &(*itVK) << " "<<&(*itKV) << std::endl;
    sortedSetVK.erase(itVK);
    std::cout << "before deleteMax: "<<sortedSetVK.size() << std::endl;
    sortedSetKV.erase(itKV);
    std::cout << "after deleteMax: "<<sortedSetVK.size() << std::endl;
}

template<typename K, typename V>
void PriorityQueue<K,V>::deleteKey(const K& k){
    static V dummyV;
    static K dummyK;
    static std::shared_ptr<pairKV> ptr =
        std::make_shared<pairKV>(dummyK, dummyV);

    ptr->key = k;
    auto it = sortedSetKV.lower_bound(ptr);
    if(it == sortedSetKV.end() || (*it)->key != k){
        throw PriorityQueueNotFoundException();
    }
    // we've found our element with key = k
    // let's make sure we delete only 1 element ...
    auto itVK = sortedSetVK.find(*it);
    sortedSetVK.erase(itVK);
    sortedSetKV.erase(it);
}

template<typename K, typename V>
void PriorityQueue<K,V>::changeValue(const K& key, const V& value){
    deleteKey(key);
    insert(key, value);
}


template<typename K, typename V>
void PriorityQueue<K,V>::merge(PriorityQueue<K, V>& queue){
    if(queue.empty())
        return;
    auto it_KV_curr = queue.sortedSetKV.begin();
    auto it_KV_next = ++it_KV_curr;
    while(queue.size() > 0){
        auto it_VK = queue.sortedSetVK.find(*it_KV_curr);

        auto copy_curr_ptr(*it_KV_curr);
        queue.sortedSetKV.erase(it_KV_curr);
        queue.sortedSetVK.erase(it_VK);
        sortedSetVK.insert(copy_curr_ptr);
        sortedSetKV.insert(copy_curr_ptr);
        it_KV_curr = it_KV_next;
        ++it_KV_next;
    }
}

template<typename K, typename V>
void PriorityQueue<K,V>::swap(PriorityQueue<K, V>& queue){
    if(this != &queue){
        queue.sortedSetVK.swap(sortedSetVK);
        queue.sortedSetKV.swap(sortedSetKV);
    }
}

template<typename K, typename V>
void swap(PriorityQueue<K,V>& lp, PriorityQueue<K,V>& rp){
    lp.swap(rp);
}

template<typename K, typename V>
bool PriorityQueue<K,V>::operator<(const PriorityQueue<K,V>& rhs) const{
    auto it_lhs = sortedSetKV.begin();
    auto it_rhs = rhs.sortedSetKV.begin();
    while(it_lhs != sortedSetKV.end() && it_rhs != rhs.sortedSetKV.end()){
        if((*it_lhs)->key < (*it_rhs)->key)
            return true;
        else if((*it_lhs)->key > (*it_rhs)->key)
            return false;
        // keys are equal by now ...
        if((*it_lhs)->val < (*it_rhs)->val)
            return true;
        else if((*it_lhs)->val > (*it_rhs)->val)
            return false;
        //values are equal if we got here ...
        ++it_lhs;
        ++it_rhs;
    }
    if(it_lhs == sortedSetKV.end() && it_rhs != rhs.sortedSetKV.end())
        return true;
    return false;
}


template<typename K, typename V>
bool operator<(const PriorityQueue<K,V>& lhs, const PriorityQueue<K,V>& rhs){
    return lhs.operator<(rhs);
}

template<typename K, typename V>
bool operator>(const PriorityQueue<K,V>& lhs, const PriorityQueue<K,V>& rhs){
    return rhs < lhs;
}

template<typename K, typename V>
bool operator==(const PriorityQueue<K,V>& lhs, const PriorityQueue<K,V>& rhs){
    return !(lhs > rhs) && !(lhs < rhs);
}

template<typename K, typename V>
bool operator!=(const PriorityQueue<K,V>& lhs, const PriorityQueue<K,V>& rhs){
    return !(lhs == rhs);
}

template<typename K, typename V>
bool operator<=(const PriorityQueue<K,V>& lhs, const PriorityQueue<K,V>& rhs){
    return !(lhs > rhs);
}

template<typename K, typename V>
bool operator>=(const PriorityQueue<K,V>& lhs, const PriorityQueue<K,V>& rhs){
    return !(lhs < rhs);
}

#endif /* PRIORITYQUEUE_HH_ */

