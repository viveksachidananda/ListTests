//
// Created by Vivek on 06-12-2017.
//

#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

namespace exceptions {
    class LLUnsupportedOperationException : public std::exception {
        virtual const char *what() const throw() {
            return "Unsupported operation has been performed";
        }
    };

    class LLOutOfRange : public std::exception {
        virtual const char *what() const throw() {
            return "Out of range Error";
        }
    };
}

template<typename T>
class List {
private:
    struct Node {
        Node() : _link(nullptr), _value(T()) {}

        explicit Node(const T &val) : _link(nullptr), _value(val) {
        }

        explicit Node(const Node *node) : _link(node->_link), _value(node->_value) {
        }

        Node *_link;
        T _value;
    };

    /*
    * Acts like a Base functor class that needs to be passed for filter
    */
    struct BaseFunctor {
        virtual bool operator()(const Node *iNode) const {
            return true;
        }
    };

    /*
     * Its a functor which returns true if the mod condition matches
     * @param iInt: Input integer
     * @param iModEqual: conditional value the result will be compared with
     * @return bool of result matches the conditional value
     */
    struct ModPredicate : public BaseFunctor {
        ModPredicate(const size_t iInt, const size_t iModEqual) : _int(iInt), _modEqual(iModEqual) {
        }

        bool operator()(const Node *iNode) const {
            if (iNode != nullptr)
                return (iNode->_value % _int) == _modEqual;

            return BaseFunctor::operator()(iNode);
        }

    private:
        size_t _int, _modEqual;
    };

public:

    // constructors
    List() : _head(nullptr), _tail(nullptr), _size(0) {}

    explicit List(const size_t iSize);

    explicit List(const List &iList);

    // destructor
    ~List();

    // assignment operator overloading
    List<T> &operator=(const List &iList);

    // accessors:
    const T &at(const size_t iPos) const;

    const T &operator[](const size_t iPos) const;

    // modifiers:
    void push_back(const T &iValue);

    void pop_back();

    void insert(const size_t iPos, const T &iValue);

    void erase(const size_t iPos);

    void reverse();

    // utility functions
    void modFilter(const size_t iInt, const size_t iModEqualTo);

    template<typename ReturnListType>
    void map(void (*func)(const T &, ReturnListType &), List<ReturnListType> &ioList) const;

    template<typename ReturnListType>
    void foldLeft(ReturnListType &ioReturn, void (*func)(const T &, ReturnListType &)) const;

    // capacity:
    size_t size() const { return _size; }

    // traversal
    void print(std::string& oString);

private:

    void filter(const BaseFunctor *iPredicate);

    void reverseUtil(Node *iCurr, Node *iPrev, Node **iHead);

    template<typename ReturnListType>
    void mapUtil(Node *iCurr, void (*func)(const T &, ReturnListType &), List<ReturnListType> &ioList) const;

    template<typename ReturnListType>
    void foldLeftUtil(Node *iCurr, void (*func)(const T &, ReturnListType &), ReturnListType &iReturnData) const;

    void printUtil(Node* iCurr, std::string& oString);

    Node *_head, *_tail;    // Stores the pointers to head and tail of the linked list
    size_t _size;   // size of the linked list
};

// Class implementation

template<typename T>
void List<T>::print(std::string& oString)
{
    oString = "";

    if(_head != nullptr)
    {
        Node *aCurr = _head;

        printUtil(aCurr, oString);
        oString.pop_back();
    } else
        oString = "Empty List";
}

template<typename T>
void List<T>::printUtil(Node* iCurr, std::string& oString)
{
    // Base case
    if (iCurr == NULL)
        return;

    std::stringstream aTemp;
    aTemp << iCurr->_value;

    oString.append(aTemp.str());
    oString.append(",");

    printUtil(iCurr->_link, oString);
}

template<typename T>
template<typename ReturnListType>
void List<T>::map(void (*func)(const T &, ReturnListType &), List<ReturnListType> &ioList) const{
    if (_head == nullptr)
        return;

    Node *aCurr = _head;
    mapUtil(aCurr, func, ioList);
}

template<typename T>
template<typename ReturnListType>
void List<T>::mapUtil(Node *iCurr, void (*func)(const T &, ReturnListType &), List<ReturnListType> &ioList) const{
    if (iCurr == nullptr)
        return;

    ReturnListType aData = ReturnListType();
    func(iCurr->_value, aData);
    ioList.push_back(aData);

    mapUtil(iCurr->_link, func, ioList);
}

template<typename T>
template<typename ReturnListType>
void List<T>::foldLeft(ReturnListType &ioReturn, void (*func)(const T &, ReturnListType &)) const{
    if (_head == nullptr)
        return;

    return foldLeftUtil(_head, func, ioReturn);
}

template<typename T>
template<typename ReturnListType>
void List<T>::foldLeftUtil(Node *iCurr, void (*func)(const T &, ReturnListType &), ReturnListType &iReturnData) const{
    if (iCurr == nullptr)
        return;

    func(iCurr->_value, iReturnData);

    return foldLeftUtil(iCurr->_link, func, iReturnData);
}

template<typename T>
void List<T>::filter(const BaseFunctor *iPredicate) {
    if (_head == nullptr)
        return;

    Node *aCurrent = _head;
    Node *aPrevious = nullptr;

    while (aCurrent != nullptr) {
        // If predicate fails then we need to delete the node
        if (!iPredicate->operator()(aCurrent)) {
            if (aCurrent == _head) {
                _head = aCurrent->_link;

                // If there is only one item then set the tail to head->next(NULLPTR)
                if (_tail == aCurrent)
                    _tail = aCurrent->_link;

                --_size;
                delete aCurrent;
                aCurrent = _head;
                aPrevious = aCurrent;
            } else {
                aPrevious->_link = aCurrent->_link;

                // If the tail is deleted then set the tail to previous
                if (_tail == aCurrent)
                    _tail = aPrevious;

                --_size;
                delete aCurrent;
                aCurrent = aPrevious->_link;
            }
        } else {
            aPrevious = aCurrent;
            aCurrent = aCurrent->_link;
        }
    }
}

/*
 * Its a functor which returns true if the mod condition matches
 * @param iInt: Input integer
 * @param iModEqual: conditional value the result will be compared with
 * @return bool of result matches the conditional value
 *
 * Generic implementation, does not support String, Characters, non-integer data types
 */
template<typename T>
void List<T>::modFilter(const size_t iInt, const size_t iModEqualTo) {
    throw exceptions::LLUnsupportedOperationException();
}

/*
 * Its a functor which returns true if the mod condition matches
 * @param iInt: Input integer
 * @param iModEqual: conditional value the result will be compared with
 * @return bool of result matches the conditional value
 *
 * Template fully Specialized for Integer(size_t)
 */
template<>
void List<size_t>::modFilter(const size_t iInt, const size_t iModEqualTo) {
    ModPredicate aModPredicate(iInt, iModEqualTo);

    filter(&aModPredicate);
}

template<>
void List<int>::modFilter(const size_t iInt, const size_t iModEqualTo) {
    ModPredicate aModPredicate(iInt, iModEqualTo);

    filter(&aModPredicate);
}

template<typename T>
List<T>::List(const size_t iSize) : _head(nullptr), _tail(nullptr), _size(iSize) {
    Node aHead;
    Node *aNodePtr = &aHead;
    for (size_t i = 0; i < _size; i++) {
        aNodePtr->_link = new Node;
        this->_tail = aNodePtr = aNodePtr->_link;
    }

    this->_head = aHead._link;
}

template<typename T>
List<T>::List(const List &iList) {
    List::operator=(iList);
}

template<typename T>
List<T> &List<T>::operator=(const List &iList) {
    _size = iList._size;
    Node aHead;
    for (Node *aIter = iList._head, *n = &aHead; aIter; aIter = aIter->_link) {
        n->_link = new Node(aIter);
        this->_tail = n = n->_link;
    }
    this->_head = aHead._link;
    return *this;
}

template<typename T>
List<T>::~List() {
    Node *aNodePtr = _head;
    while (aNodePtr != nullptr) {
        Node *nextNode = aNodePtr->_link;
        delete aNodePtr;
        aNodePtr = nextNode;
    }
}

template<typename T>
const T &List<T>::at(const size_t iPos) const {
    Node *aNodePtr = _head;
    if (iPos < 0 || iPos >= this->size()) {
        throw exceptions::LLOutOfRange();
    } else {

        for (size_t i = 0; i < iPos; i++) {
            aNodePtr = aNodePtr->_link;
        }
    }
    return aNodePtr->_value;
}

template<typename T>
const T &List<T>::operator[](const size_t iPos) const {
    return at(iPos);
}

template<typename T>
void List<T>::push_back(const T &iValue) {
    Node *aNewNode = new Node(iValue);
    // If there are no nodes in the list make newNode the first node.
    if (_head == nullptr) {
        _tail = _head = aNewNode;
        // Otherwise, insert newNode at end.
    } else {
        _tail->_link = aNewNode;
        _tail = aNewNode;
    }
    ++_size;
}

template<typename T>
void List<T>::pop_back() {
    if (!_head) {
        return;
    } else if (!_head->_link) {
        delete _head;
        _head = nullptr;
        _tail = nullptr;
        _size = 0;
    } else {

        Node *aNodePtr = _head;
        Node *aPrevNode = nullptr;

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

template<typename T>
void List<T>::insert(const size_t iPos, const T &iValue) {
    if (iPos > _size) {
        throw exceptions::LLOutOfRange();
    }

    Node *aNewNode = new Node(iValue);
    Node *aNodePtr = _head;
    Node *aPrevNode = nullptr;

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
        if (aNewNode->_link == nullptr)
            _tail = aNewNode;
    }
    ++_size;
}

template<typename T>
void List<T>::erase(const size_t iPos) {
    if (_size <= iPos || _head == nullptr) {
        throw exceptions::LLOutOfRange();
    }
    Node *aNodePtr = _head;
    Node *aPrevNode = nullptr;
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

// This function mainly calls reverseUtil()
// with prev as NULL
template<typename T>
void List<T>::reverse() {
    if (_head == nullptr)
        return;

    // Initialize the tail to head
    _tail = _head;

    reverseUtil(_head, NULL, &_head);
}

// A simple and tail recursive function to reverse
// a linked list.  prev is passed as NULL initially.
template<typename T>
void List<T>::reverseUtil(Node *iCurr, Node *iPrev, Node **iHead) {
    /* If last node mark it head*/
    if (iCurr->_link == nullptr) {
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

#endif //LIST_HPP
