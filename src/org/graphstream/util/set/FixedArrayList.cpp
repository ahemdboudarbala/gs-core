#include "FixedArrayList.hpp"

// Constructors
template <typename E>
FixedArrayList<E>::FixedArrayList() : lastIndex(-1) {}

template <typename E>
FixedArrayList<E>::FixedArrayList(int capacity) : elements(capacity), lastIndex(-1) {}

// Accessors
template <typename E>
int FixedArrayList<E>::size() const {
    return elements.size() - freeIndices.size();
}

template <typename E>
int FixedArrayList<E>::realSize() const {
    return elements.size();
}

template <typename E>
bool FixedArrayList<E>::isEmpty() const {
    return size() == 0;
}

template <typename E>
E& FixedArrayList<E>::get(int i) {
    if (elements[i] == nullptr) {
        throw std::out_of_range("No element at index " + std::to_string(i));
    }
    return elements[i];
}

template <typename E>
const E& FixedArrayList<E>::get(int i) const {
    if (elements[i] == nullptr) {
        throw std::out_of_range("No element at index " + std::to_string(i));
    }
    return elements[i];
}

template <typename E>
E& FixedArrayList<E>::unsafeGet(int i) {
    return elements[i];
}

template <typename E>
const E& FixedArrayList<E>::unsafeGet(int i) const {
    return elements[i];
}

template <typename E>
bool FixedArrayList<E>::contains(const E& e) const {
    return std::find(elements.begin(), elements.end(), e) != elements.end();
}

template <typename E>
int FixedArrayList<E>::getLastIndex() const {
    return lastIndex;
}

template <typename E>
int FixedArrayList<E>::getNextAddIndex() const {
    return freeIndices.empty() ? elements.size() : freeIndices.back();
}

// Commands
template <typename E>
bool FixedArrayList<E>::add(const E& element) {
    if (element == nullptr) {
        throw std::invalid_argument("This array cannot contain null value");
    }

    if (!freeIndices.empty()) {
        int index = freeIndices.back();
        freeIndices.pop_back();
        elements[index] = element;
        lastIndex = index;
    } else {
        elements.push_back(element);
        lastIndex = elements.size() - 1;
    }

    return true;
}

template <typename E>
template <typename Collection>
bool FixedArrayList<E>::addAll(const Collection& c) {
    for (const auto& element : c) {
        add(element);
    }
    return true;
}

template <typename E>
E FixedArrayList<E>::remove(int i) {
    if (i < 0 || i >= elements.size()) {
        throw std::out_of_range("Index " + std::to_string(i) + " does not exist");
    }

    if (elements[i] == nullptr) {
        throw std::out_of_range("No element stored at index " + std::to_string(i));
    }

    E removedElement = elements[i];
    elements[i] = nullptr;
    freeIndices.push_back(i);

    return removedElement;
}

template <typename E>
bool FixedArrayList<E>::remove(const E& e) {
    auto it = std::find(elements.begin(), elements.end(), e);
    if (it != elements.end()) {
        int index = std::distance(elements.begin(), it);
        elements[index] = nullptr;
        freeIndices.push_back(index);
        return true;
    }
    return false;
}

template <typename E>
void FixedArrayList<E>::clear() {
    elements.clear();
    freeIndices.clear();
    lastIndex = -1;
}

// Nested Iterator class
template <typename E>
FixedArrayList<E>::FixedArrayIterator::FixedArrayIterator(FixedArrayList<E>& list)
    : list(list), index(-1) {}

template <typename E>
bool FixedArrayList<E>::FixedArrayIterator::hasNext() {
    for (int i = index + 1; i < list.elements.size(); ++i) {
        if (list.elements[i] != nullptr) {
            return true;
        }
    }
    return false;
}

template <typename E>
E& FixedArrayList<E>::FixedArrayIterator::next() {
    for (int i = index + 1; i < list.elements.size(); ++i) {
        if (list.elements[i] != nullptr) {
            index = i;
            return list.elements[i];
        }
    }
    throw std::out_of_range("No more elements in iterator");
}

template <typename E>
void FixedArrayList<E>::FixedArrayIterator::remove() {
    if (index >= 0 && index < list.elements.size() && list.elements[index] != nullptr) {
        list.remove(index);
    } else {
        throw std::out_of_range("No such element");
    }
}

template <typename E>
typename FixedArrayList<E>::FixedArrayIterator FixedArrayList<E>::iterator() {
    return FixedArrayIterator(*this);
}

#endif // FIXEDARRAYLIST_HPP
