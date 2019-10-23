// HashSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;

    int _size;
    int capacity;

    class Node {
    public:
        ElementType element;
        Node* next;

        Node(const ElementType& e) : element(e), next(NULL) { }

        Node(Node* s) {
            this->element = s->element;
            if(s->next != NULL) {
                this->next = new Node(s->next);
            }
            else {
                this->next = NULL;
            }
        }

        ~Node() noexcept {
            if(next != NULL) {
                delete next;
            }
        }

    };

    Node** array;

    // resize array
    void resize();
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    _size = 0;
    capacity = DEFAULT_CAPACITY;
    array = new Node*[capacity];
    
    for(auto i = 0; i < capacity; i++) {
        array[i] = NULL;
    }
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    for(auto i = 0; i < capacity; i++) {
        delete array[i];
    }   
    delete[] array;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    this->_size = s._size;
    this->capacity = s.capacity;
    this->array = new Node*[this->capacity];
    for(auto i = 0; i < capacity; i++) {
        if(s.array[i] != NULL) {
            this->array[i] = new Node(s.array[i]);
        }   
        else {
            this->array[i] = NULL;
        }
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    this->_size = s._size;
    this->capacity = s.capacity;
    this->array = new Node*[this->capacity];
    for(auto i = 0; i < capacity; i++) {
        if(s.array[i] != NULL) {
            this->array[i] = new Node(s.array[i]);
        }   
        else {
            this->array[i] = NULL;
        }
    }
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    this->_size = s._size;
    this->capacity = s.capacity;
    // check if we have to delete somthing first
    if(this->array != NULL) {
        for(auto i = 0; i < capacity; i++) {
            delete array[i];
        }
        delete[] this->array;
    }
    this->array = new Node*[this->capacity];
    for(auto i = 0; i < capacity; i++) {
        if(s.array[i] != NULL) {
            this->array[i] = new Node(s.array[i]);
        }   
        else {
            this->array[i] = NULL;
        }
        
    }
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    this->_size = s._size;
    this->capacity = s.capacity;
    // check if we have to delete somthing first
    if(this->array != NULL) {
        for(auto i = 0; i < capacity; i++) {
            delete array[i];
        }
        delete[] this->array;
    }
    this->array = new Node*[this->capacity];
    for(auto i = 0; i < capacity; i++) {
        if(s.array[i] != NULL) {
            this->array[i] = new Node(s.array[i]);
        }   
        else {
            this->array[i] = NULL;
        }
    }
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if( (double)_size / capacity > 0.8) {
        resize();
    }

    int index = hashFunction(element) % capacity;

    Node* newNode = new Node(element);
    newNode->next = array[index];
    array[index] = newNode;
    _size++;
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    int index = hashFunction(element) % capacity;
    Node* workingNode = array[index];
    while(workingNode != NULL) {
        if(workingNode->element == element)
            return true;
        workingNode = workingNode->next;
    }
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return _size;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    int count = 0;
    Node* workingNode = array[index];
    while(workingNode != NULL) {
        count++;
        workingNode = workingNode->next;
    }
    return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    Node* workingNode = array[index];
    while(workingNode != NULL) {
        if(workingNode->element == element)
            return true;
        workingNode = workingNode->next;
    }
    return false;
}


template <typename ElementType>
void HashSet<ElementType>::resize() 
{
    Node** newArray = new Node*[2 * capacity];
    
    for(auto i = 0; i < capacity; i++) {
        if(array[i] != NULL)
            newArray[i] = new Node(array[i]);
        else
            newArray[i] = NULL;
    }

    for(auto i = 0; i < capacity; i++) {
        delete array[i];
    }   
    delete[] array;
    
    array = newArray;
    
    capacity *= 2;
}


#endif // HASHSET_HPP

    