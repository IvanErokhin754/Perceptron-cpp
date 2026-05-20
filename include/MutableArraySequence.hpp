#ifndef MUTABLE_ARRAY_SEQUENCE_HPP
#define MUTABLE_ARRAY_SEQUENCE_HPP

#include "ArraySequence.hpp"
#include <initializer_list>

template<typename T>
class MutableArraySequence : public ArraySequence<T> {
protected:
    ArraySequence<T>* CreateFromArray(const DynamicArray<T>& other) const override {
        return new MutableArraySequence<T>(other);
    }

public:
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) {}
    MutableArraySequence(const DynamicArray<T>& other) : ArraySequence<T>(other) {}
    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>(other) {}

    MutableArraySequence<T>& operator=(const MutableArraySequence<T>& other) {
        if (this != &other) {
            ArraySequence<T>::operator=(other);
        }
        return *this;
    }

    MutableArraySequence(size_t count) : ArraySequence<T>() {
        this->array.Resize(count);
        for (size_t i = 0; i < count; ++i) {
            this->array[i] = T();
        }
    }

    MutableArraySequence(size_t count, const T& default_value) : ArraySequence<T>() {
        this->array.Resize(count);
        for (size_t i = 0; i < count; ++i) {
            this->array[i] = default_value;
        }
    }

    MutableArraySequence(std::initializer_list<T> list) : ArraySequence<T>() {
        this->array.Resize(list.size());
        size_t i = 0;
        for (const T& val : list) {
            this->array[i++] = val;
        }
    }

    size_t size() const { return this->GetLength(); }
    bool empty() const { return this->GetLength() == 0; }
    void push_back(const T& value) { this->AppendInternal(value); }

    void assign(size_t count, const T& value) {
        this->array.Resize(count);
        for(size_t i = 0; i < count; ++i) {
            this->array[i] = value;
        }
    }

    void insert(T* pos, const T* first, const T* last) {
        size_t index = pos - this->begin();
        size_t count = last - first;
        size_t old_size = this->size();
        this->array.Resize(old_size + count);
        for (size_t i = old_size; i > index; --i) {
            this->array[i + count - 1] = this->array[i - 1];
        }
        for (size_t i = 0; i < count; ++i) {
            this->array[index + i] = *(first + i);
        }
    }

    T& operator[](size_t index) {
        return this->array[index];
    }

    const T& operator[](size_t index) const {
        return this->Get(index);
    }
    
    MutableArraySequence<T>* Append(const T& value) {
        this->AppendInternal(value);
        return this;
    }

    MutableArraySequence<T>* Prepend(const T& value) {
        this->PrependInternal(value);
        return this;
    }

    MutableArraySequence<T>* InsertAt(size_t index, const T& value) {
        this->InsertAtInternal(index, value);
        return this;
    }

    MutableArraySequence<T>* Remove(size_t index) {
        this->RemoveAtInternal(index);
        return this;
    }

    Sequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
};


#endif /* MUTABLE_ARRAY_SEQUENCE_HPP */