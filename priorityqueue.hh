#ifndef PRIORITYQUEUE_HH_
#define PRIORITYQUEUE_HH_

#include <stddef.h>
#include <memory>
#include <set>
#include <exception>

class PriorityQueueEmptyException : public std::exception {
    public:
        virtual const char* what() const throw() {
            return "PriorityQueue empty exception";
        }
};

class PriorityQueueNotFoundException : public std::exception {
    public:
        virtual const char* what() const throw() {
            return "PriorityQueue not found exception";
        }
};

template<typename K, typename V>
class PriorityQueue {

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
        bool operator<(const PriorityQueue<K, V>& other) const;

        bool empty() const;
        size_type size() const;
        void insert(const K& key, const V& value);

    private:
        void deleteKey(const K& k);
        typedef struct pairKV {
            K key;
            V val;

            pairKV(const pairKV& pKV) {
                key(pKV.key);
                val(pKV.val);
            }

            pairKV(const K& k, const V& v) : key(k) , val(v) {
            }
        } pairKV;

        struct compareVK {
            bool operator() (const std::shared_ptr<pairKV>& lhs,
            const std::shared_ptr<pairKV>& rhs) const {
                if (lhs->val < rhs->val)
                    return true;
                else if (rhs->val < lhs->val)
                    return false;
                if (lhs->key < rhs->key)
                    return true;
                else if (rhs->key < lhs->key )
                    return false;
                return false;

            }
        };

        struct compareKV {
            bool operator() (const std::shared_ptr<pairKV>& lhs,
            const std::shared_ptr<pairKV>& rhs) const {
                if (lhs->key < rhs->key)
                    return true;
                else if (rhs->key < lhs->key)
                    return false;
                if (lhs->val < rhs->val)
                    return true;
                else if (rhs->val < lhs->val)
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
PriorityQueue<K, V>::PriorityQueue() {
}

/* copy constructor of map and set has strong exception guarantee */
template<typename K, typename V>
PriorityQueue<K, V>::PriorityQueue(const PriorityQueue<K, V>& queue) {
  for (auto iterator = queue.sortedSetVK.begin();
      iterator != queue.sortedSetVK.end();
      ++iterator) {
    auto temporary_pointer = std::make_shared<pairKV>(
        (*iterator)->key, (*iterator)->val);
    sortedSetVK.insert(sortedSetVK.end(), temporary_pointer);
    sortedSetKV.insert(temporary_pointer);
  }
}

/* move constructor - just swap our empty multisets for the passed queue's
 * multiset ... */
template<typename K, typename V>
PriorityQueue<K, V>::PriorityQueue(PriorityQueue<K, V>&& queue) {
    queue.sortedSetVK.swap(sortedSetVK);
    queue.sortedSetKV.swap(sortedSetKV);

}

/* move assignment operator=(mainly for temporary objects being passed as a
 * parameter) */
/* COMPLEXITY : O(1) : obvious - swap. */
template<typename K, typename V>
PriorityQueue<K, V>& PriorityQueue<K, V>::operator=(PriorityQueue<K, V> &&queue) {
    if (this != &queue) {
        queue.sortedSetVK.swap(sortedSetVK);
        queue.sortedSetKV.swap(sortedSetKV);
    }
    return *this;
}

/* assignment operator= for lvalues ... we just copy the multisets ... */
/* COMPLEXITY : O(size(queue)) : from stl::set) */
template<typename K, typename V>
PriorityQueue<K, V>& PriorityQueue<K, V>::operator=(PriorityQueue<K, V> &queue) {

    if (this != &queue) {
        PriorityQueue<K, V> new_one(queue);
        this->swap(new_one);
    }
    return *this;
}

/* COMPLEXITY : O(1) : from stl::set */
template<typename K, typename V>
bool PriorityQueue<K, V>::empty() const {
    return sortedSetVK.empty();
}

/* 1!) typename keyword added */
/* COMPLEXITY : O(1) : from stl::set */
template<typename K, typename V>
typename PriorityQueue<K, V>::size_type PriorityQueue<K, V>::size() const {
    return sortedSetVK.size();
}

/* COMPLEXITY : O(log(size(this))) : */
template<typename K, typename V>
void PriorityQueue<K, V>::insert(const K& key, const V& value) {
    auto ptr = std::make_shared<pairKV>(key, value);
    auto helper_iterator = sortedSetVK.lower_bound(ptr);
    sortedSetVK.insert(helper_iterator, ptr);
    try {
      auto helper_iterator_2 = sortedSetKV.lower_bound(ptr);
      sortedSetKV.insert(helper_iterator_2, ptr);
    } catch (...) {
      sortedSetVK.erase(helper_iterator);
      throw;
    }
}

/* COMPLEXITY - O(1) */
template<typename K, typename V>
const V& PriorityQueue<K, V>::minValue() const {
    if (sortedSetVK.empty()) {
        throw PriorityQueueEmptyException();
    }
    return (*sortedSetVK.begin())->val;
}

/* COMPLEXITY - O(1) */
template<typename K, typename V>
const V& PriorityQueue<K, V>::maxValue() const {
    if (sortedSetVK.empty()) {
        throw PriorityQueueEmptyException();
    }
    return (*sortedSetVK.rbegin())->val;
}

/* COMPLEXITY - O(1) */
template<typename K, typename V>
const K& PriorityQueue<K, V>::minKey() const {
    if (sortedSetVK.empty()) {
        throw PriorityQueueEmptyException();
    }
    return (*sortedSetVK.begin())->key;
}

/* COMPLEXITY - O(1) */
template<typename K, typename V>
const K& PriorityQueue<K, V>::maxKey() const {
    if (sortedSetVK.empty()) {
        throw PriorityQueueEmptyException();
    }
    return (*sortedSetVK.rbegin())->key;
}

/* COMPLEXITY - O(log(size(this))) */
template<typename K, typename V>
void PriorityQueue<K, V>::deleteMin() {
    if (sortedSetVK.empty())
        return;
    auto itVK = sortedSetVK.begin();
    auto itKV = sortedSetKV.find(*itVK);
    sortedSetVK.erase(itVK);
    sortedSetKV.erase(itKV);
}

/* COMPLEXITY - O(log(size(this))) */
template<typename K, typename V>
void PriorityQueue<K, V>::deleteMax() {
    if (sortedSetVK.empty())
        return;
    auto itVK = sortedSetVK.end();
    --itVK;
    auto itKV = sortedSetKV.find(*itVK);
    sortedSetVK.erase(itVK);
    sortedSetKV.erase(itKV);
}

/* COMPLEXITY - O(log(size(this))) */
template<typename K, typename V>
void PriorityQueue<K, V>::changeValue(const K& key, const V& value) {
    std::shared_ptr<pairKV> ptr =
        std::make_shared<pairKV>(key, value);

    auto it = sortedSetKV.lower_bound(ptr);

    if (this->empty() ||
        !((it != sortedSetKV.end() && (*it)->key == key) ||
          (it != sortedSetKV.begin() && (*(--it))->key == key))) {
      throw PriorityQueueNotFoundException();
    }

    auto temp_ptr = *it;

    auto helper_insert_it_1 = sortedSetKV.lower_bound(ptr);
    auto helper_insert_it_2 = sortedSetVK.lower_bound(ptr);

    helper_insert_it_1 = sortedSetKV.insert(helper_insert_it_1, ptr);
    helper_insert_it_2 = sortedSetVK.insert(helper_insert_it_2, ptr);

    try {

      auto helper_erase_it_1 = sortedSetKV.lower_bound(temp_ptr);
      auto helper_erase_it_2 = sortedSetVK.lower_bound(temp_ptr);

      sortedSetKV.erase(helper_erase_it_1);
      sortedSetVK.erase(helper_erase_it_2);

    } catch (...) {

      sortedSetKV.erase(helper_insert_it_1);
      sortedSetVK.erase(helper_insert_it_2);

      throw;
    }
}

// COMPLEXITY = O(size() + queue.size() * log(size() + queue.size())) 
template<typename K, typename V>
void PriorityQueue<K, V>::merge(PriorityQueue<K, V>& queue) {
    if (queue.empty())
        return;

    if (this != &queue) {
      PriorityQueue<K, V> new_one(*this);

      for (auto iterator = queue.sortedSetVK.begin();
          iterator != queue.sortedSetVK.end();
          ++iterator) {

        new_one.sortedSetVK.insert(*iterator);
        new_one.sortedSetKV.insert(*iterator);
      }

      queue = PriorityQueue<K, V>();
      this->swap(new_one); 
    }
}

// COMPLEXITY = O(1) 
template<typename K, typename V>
void PriorityQueue<K, V>::swap(PriorityQueue<K, V>& queue) {
    if (this != &queue) {
      std::swap(queue.sortedSetVK, sortedSetVK);
      std::swap(queue.sortedSetKV, sortedSetKV);
    }
}

// COMPLEXITY = O(1) 
template<typename K, typename V>
void swap(PriorityQueue<K, V>& lp, PriorityQueue<K, V>& rp) {
    lp.swap(rp);
}

// COMPLEXITY = O(size()) 
template<typename K, typename V>
bool PriorityQueue<K, V>::operator<(const PriorityQueue<K, V>& rhs) const {

    auto it = sortedSetKV.begin();
    auto it_rhs = rhs.sortedSetKV.begin();

    while (it != sortedSetKV.end() && it_rhs != rhs.sortedSetKV.end()) {

        if ((*it)->key < (*it_rhs)->key)
            return true;
        else if ((*it_rhs)->key < (*it)->key)
            return false;
        // keys are equal by now ...
        if ((*it)->val < (*it_rhs)->val)
            return true;
        else if ((*it_rhs)->val < (*it)->val )
            return false;
        //values are equal if we got here ...
        ++it;
        ++it_rhs;
    }
    if (it == sortedSetKV.end() && it_rhs != rhs.sortedSetKV.end())
        return true;
    return false;
}


template<typename K, typename V>
bool operator<(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    return lhs.operator<(rhs);
}

template<typename K, typename V>
bool operator>(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    return rhs < lhs;
}

template<typename K, typename V>
bool operator==(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    return !(lhs > rhs) && !(lhs < rhs);
}

template<typename K, typename V>
bool operator!=(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    return !(lhs == rhs);
}

template<typename K, typename V>
bool operator<=(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    return !(lhs > rhs);
}

template<typename K, typename V>
bool operator>=(const PriorityQueue<K, V>& lhs, const PriorityQueue<K, V>& rhs) {
    return !(lhs < rhs);
}

#endif /* PRIORITYQUEUE_HH_ */
