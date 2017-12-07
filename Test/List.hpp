//
// Created by Meghana on 06-12-2017.
//

#ifndef TEST_LIST_HPP
#define TEST_LIST_HPP
#include <cstddef>
#include <iostream>

template <typename T>
class List
{
private:
    struct Node
    {
        Node(): _link(nullptr), _value(T()) {}
        Node(const T& val) : _link(nullptr), _value(val) {}
        Node(const Node* node) : _link(node->_link), _value(node->_value) {}

        Node *_link;
        T _value;
    };
public:
    // constructors
    List() : _head(nullptr), _size(0) {}
    List(const size_t iSize);
    List(const List& iList);
    ~List();

    List<T>& operator=(const List& iList);

    // element access:
    const T& at(const size_t iPos) const;
    const T& operator[](const size_t iPos) const;

    // modifiers:
    void push_back(const T& iValue);
    void pop_back();
    void insert(const size_t iPos, const T& iValue);
    void erase(const size_t iPos);
    void reverse();

    // capacity:
    size_t size() const {return _size;}


private:

    void reverseUtil(Node *iCurr, Node *iPrev, Node **iHead);
    void subError() const; // Handles memory subscripts out of range

    Node *_head, *_tail;
    size_t _size;
};

// Class implementation

template <typename T>
List<T>::List(size_t const iSize) : _head(nullptr), _tail(nullptr), _size(iSize)
{
    Node aHead;
    Node* aNodePtr = &aHead;
    for (size_t i = 0; i < _size; i++) {
        aNodePtr->_link = new Node;
        this->_tail = aNodePtr = aNodePtr->_link;
    }

    this->_head = aHead._link;
}

template <typename T>
List<T>::List(const List& iList)
{
    List::operator =(iList);
}

template <typename T>
List<T>& List<T>::operator=(const List& iList)
{
    _size = iList._size;
    Node aHead;
    for (Node* aIter = iList._head, *n = &aHead; aIter; aIter = aIter->_link) {
        n->_link = new Node(aIter);
        this->_tail = n = n->_link;
    }
    this->_head = aHead._link;
    return *this;
}

template <typename T>
List<T>::~List()
{
    Node* aNodePtr = _head;
    while (aNodePtr != nullptr) {
        Node* nextNode = aNodePtr->_link;
        delete aNodePtr;
        aNodePtr = nextNode;
    }
}

template <typename T>
const T& List<T>::at(const size_t iPos) const
{
    Node* aNodePtr = _head;
    if (iPos < 0 || iPos >= this->size()) {
        subError();
    } else {

        for (size_t i = 0; i < iPos; i++) {
            aNodePtr = aNodePtr->_link;
        }
    }
    return aNodePtr->_value;
}

template <typename T>
const T& List<T>::operator[](const size_t iPos) const {
    return at(iPos);
}

template <typename T>
void List<T>::push_back(const T& iValue)
{
    Node* aNewNode = new Node(iValue);
    // If there are no nodes in the list make newNode the first node.
    if (_head == nullptr) {
        _tail = _head = aNewNode;
        // Otherwise, insert newNode at end.
    } else {
        _tail->_link = aNewNode;
        _tail = aNewNode;
    }
//    else {
//        Node* nodePtr = _head;
//        // Go to the last node in the list.
//        while (nodePtr->_link) {
//            nodePtr = nodePtr->_link;
//        }
//        // Insert newNode as the last node.
//        nodePtr->_link = newNode;
//    }
    ++_size;
}

template <typename T>
void List<T>::pop_back()
{
    if (!_head) {
        return;
    } else if (!_head->_link) {
        delete _head;
        _head = nullptr;
        _tail = nullptr;
        _size = 0;
    } else {

        Node* aNodePtr = _head;
        Node* aPrevNode = nullptr;

        while (aNodePtr->_link) {
            aPrevNode = aNodePtr;
            aNodePtr = aNodePtr->_link;
        }
        aPrevNode->_link = nullptr;

        _tail = aPrevNode;
        delete aNodePtr;
        --_size;
    }
}

template <typename T>
void List<T>::insert(const size_t iPos, const T& iValue)
{
    if (iPos > _size) {
        subError();
    }

    Node* aNewNode = new Node(iValue);
    Node* aNodePtr = _head;
    Node* aPrevNode = nullptr;

    if (_head == nullptr) {
        _head = aNewNode;
        // Insert at beginning of list
    } else if (iPos == 0) {
        _tail = _head = aNewNode;
        aNewNode->_link = aNodePtr;
        // Otherwise insert new node at given position
    } else {
        for (size_t i = 0; i < iPos; i++) {
            aPrevNode = aNodePtr;
            aNodePtr = aNodePtr->_link;
        }

        aPrevNode->_link = aNewNode;
        aNewNode->_link = aNodePtr;

        // if the added node is the last node then update the tail position
        if(aNewNode->_link == nullptr)
            _tail = aNewNode;
    }
    ++_size;
}

template <typename T>
void List<T>::erase(const size_t iPos)
{
    if (_size <= iPos || _head == nullptr) {
        subError();
    }
    Node* aNodePtr = _head;
    Node* aPrevNode = nullptr;
    // Erase first element
    if (iPos == 0) {
        _head = aNodePtr->_link;
        delete aNodePtr;
        // Otherwise erase element at position
    } else {
        for (size_t i = 0; i < iPos; i++) {
            aPrevNode = aNodePtr;
            aNodePtr = aNodePtr->_link;
        }

        aPrevNode->_link = aNodePtr->_link;

        // if the deleted node is the last node then update the tail position
        if (aNodePtr->_link == nullptr)
            _tail = aPrevNode;

        delete aNodePtr;
    }
    --_size;
}

template <typename T>
void List<T>::subError() const{
    std::cout << "ERROR: Subscript out of range.\n";
    exit(EXIT_FAILURE);
}

// This function mainly calls reverseUtil()
// with prev as NULL
template <typename T>
void List<T>::reverse()
{
    if (_head == nullptr)
        return;

    reverseUtil(_head, NULL, &_head);
}

// A simple and tail recursive function to reverse
// a linked list.  prev is passed as NULL initially.
template <typename T>
void List<T>::reverseUtil(Node *iCurr, Node *iPrev, Node **iHead)
{
    /* If last node mark it head*/
    if (iCurr->_link == nullptr)
    {
        *iHead = iCurr;

        /* Update next to prev node */
        iCurr->_link = iPrev;
        return;
    }

    /* Save curr->next node for recursive call */
    Node *next = iCurr->_link;

    /* and update next ..*/
    iCurr->_link = iPrev;

    reverseUtil(next, iCurr, iHead);
}

#endif //TEST_LIST_HPP
