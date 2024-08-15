#ifndef FIXEDARRAYLIST_HPP
#define FIXEDARRAYLIST_HPP

#include <vector>
#include <stdexcept>
#include <iterator>
#include <algorithm>

template <typename E>
class FixedArrayList {
public:
    // Constructors
    FixedArrayList();
    FixedArrayList(int capacity);

    // Accessors
    int size() const;
    int realSize() const;
    bool isEmpty() const;
    E& get(int i);
    const E& get(int i) const;
    E& unsafeGet(int i);
    const E& unsafeGet(int i) const;
    bool contains(const E& e) const;
    int getLastIndex() const;
    int getNextAddIndex() const;

    // Commands
    bool add(const E& element);
    template <typename Collection>
    bool addAll(const Collection& c);
    E remove(int i);
    bool remove(const E& e);
    void clear();

    // Nested Iterator class
    class FixedArrayIterator {
    public:
        FixedArrayIterator(FixedArrayList<E>& list);
        bool hasNext();
        E& next();
        void remove();

    private:
        FixedArrayList<E>& list;
        int index;
    };

    FixedArrayIterator iterator();

private:
    std::vector<E> elements;
    std::vector<int> freeIndices;
    int lastIndex;
};

#include "FixedArrayList.tpp" // Include the template implementation file

#endif // FIXEDARRAYLIST_HPP
