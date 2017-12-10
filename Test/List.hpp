//
// Created by Vivek on 10-12-2017.
//

#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

/*
 * Exceptions related to List operations
 */
namespace exceptions {
    class LLUnsupportedOperationException : public std::exception {
        virtual const char *what() const throw() {
            return "Unsupported exception :  The performed operation is not supported by the List of given data type";
        }
    };

    class LLOutOfRange : public std::exception {
        virtual const char *what() const throw() {
            return "Range exception : Out of range Error";
        }
    };
}

template<typename T>
class List {
private:

    /*
     * Node data structure, that captures the data and the pointer to next node
     */
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
     * @param iDivisor: Divisor integer as input
     * @param iRemainder: conditional value the remainder will be compared with
     * @return bool of result matches the conditional value
     */
    struct ModPredicate : public BaseFunctor {
        ModPredicate(const size_t iDivisor, const size_t iRemainder) : _divisor(iDivisor), _remainder(iRemainder) {
        }

        bool operator()(const Node *iNode) const {
            if (iNode != nullptr)
                return (iNode->_value % _divisor) == _remainder;

            return BaseFunctor::operator()(iNode);
        }

    private:
        size_t _divisor, _remainder;
    };

public:

    // constructors
    List() : _head(nullptr), _tail(nullptr), _size(0) {}

    // destructor
    ~List();

    // accessors:

    /*
     * Returns the data at the given place
     * @param iPos : Position in the list
     * @return Data
     * @throws LLOutOfRange if its out of range
     */
    const T &at(const size_t iPos) const;

    /*
     * Returns the data at the given place
     * @param iPos : Position in the list
     * @return Data
     * @throws LLOutOfRange if its out of range
     */
    const T &operator[](const size_t iPos) const;

    // modifiers:

    /*
     * Add the given data into the list at the end
     * @param iValue : Input data to be added into the list
     * @return void
     */
    void push_back(const T &iValue);

    /*
     * Removes the last element from the list, if no element does nothing
     * @return void
     */
    void pop_back();

    /*
     * Reverses the entire list in-place
     * @return void
     */
    void reverse();

    /*
     * Filter the list using a Modulo operator
     * @param iDivisor: Input divisor integer
     * @param iRemainder: conditional value the result will be compared with
     * @throws: LLUnsupportedOperationException for execution on non-supported data types
     * @return void
     * Generic implementation, does not support String, Characters, non-integer data types
     */
    void modFilter(const size_t iDivisor, const size_t iRemainder);

    // utility methods

    /*
     * Template param T is of List data type, ReturnListType is the type of data returned by the operation function.
     * Applies given function on to each of list elements and stores the returned result into another list
     * @param iOperation : A function pointer that takes two arguments Data and return Value
     * @param oList : Output list containing the result of the operations performed on each element
     * @return void
     */
    template<typename ReturnListType>
    void map(void (*iOperation)(const T &, ReturnListType &), List<ReturnListType> &oList) const;

    /*
     * Template param T is of List data type, ReturnType is the type of data returned by the operation function.
     * Applies given function on to each of list elements and stores the returned result into another list
     * @param ioReturn : Its an input/ouput data which is updated as a result of operation performed on each element
     * @param iOperation : A function pointer that takes two arguments Data and return Value
     * @return void
     */
    template<typename ReturnType>
    void foldLeft(ReturnType &ioReturn, void (*iOperation)(const T &, ReturnType &)) const;

    // capacity:

    /*
     * Inline functions that returns the size of the list
     * @return Size(size_t) of the list
     */
    size_t size() const { return _size; }

    // for logging

    /*
     * Functions to build a string, which can be used for displaying the contents of the list in comma separated format
     * @oString : string that contains the output of the list contents, if empty it contains "Empty List" string.
     * @return void
     */
    void print(std::string &oString);

private:

    // Methods

    // Generic filter
    void filter(const BaseFunctor *iPredicate);

    // Private Utility method for reverse
    void reverseUtil(Node *iCurr, Node *iPrev, Node **iHead);

    // Private Utility method for map
    template<typename ReturnListType>
    void mapUtil(Node *iCurr, void (*func)(const T &, ReturnListType &), List<ReturnListType> &ioList) const;

    // Private Utility method for foldLeft
    template<typename ReturnType>
    void foldLeftUtil(Node *iCurr, void (*func)(const T &, ReturnType &), ReturnType &iReturnData) const;

    void printUtil(Node *iCurr, std::string &oString);

    // Properties
    Node *_head, *_tail;    // Stores the pointers to head and tail of the linked list
    size_t _size;   // size of the linked list
};

// Class implementation

template<typename T>
void List<T>::print(std::string &oString) {
    oString = "";

    if (_head != nullptr) {
        Node *aCurr = _head;

        printUtil(aCurr, oString);
        oString.pop_back();
    } else
        oString = "Empty List";
}

template<typename T>
void List<T>::printUtil(Node *iCurr, std::string &oString) {
    // Base case
    if (iCurr == NULL)
        return;

    std::stringstream aTemp;
    aTemp << iCurr->_value;

    oString.append(aTemp.str());
    oString.append(",");

    printUtil(iCurr->_link, oString);
}

/*
 * Template param T is of List data type, ReturnListType is the type of data returned by the operation function.
 * Applies given function on to each of list elements and stores the returned result into another list
 * @param iOperation : A function pointer that takes two arguments Data and return Value
 * @param ioList : Output list containing the result of the operations performed on each element
 */
template<typename T>
template<typename ReturnListType>
void List<T>::map(void (*iOperation)(const T &, ReturnListType &), List<ReturnListType> &oList) const {
    if (_head == nullptr)
        return;

    mapUtil(_head, iOperation, oList);
}

// Private Utility method for map
template<typename T>
template<typename ReturnListType>
void List<T>::mapUtil(Node *iCurr, void (*func)(const T &, ReturnListType &), List<ReturnListType> &ioList) const {
    if (iCurr == nullptr)
        return;

    // Initialize the variable with default value
    ReturnListType aData = ReturnListType();

    // Apply the function
    func(iCurr->_value, aData);

    // Push the data into the list
    ioList.push_back(aData);

    // do this, recursively
    mapUtil(iCurr->_link, func, ioList);
}

/*
 * Template param T is of List data type, ReturnType is the type of data returned by the operation function.
 * Applies given function on to each of list elements and stores the returned result into another list
 * @param ioReturn : Its an input/ouput data which is updated as a result of operation performed on each element
 * @param iOperation : A function pointer that takes two arguments Data and return Value
 */
template<typename T>
template<typename ReturnType>
void List<T>::foldLeft(ReturnType &ioReturn, void (*iOperation)(const T &, ReturnType &)) const {
    if (_head == nullptr)
        return;

    return foldLeftUtil(_head, iOperation, ioReturn);
}

// Private Utility method for foldLeft
template<typename T>
template<typename ReturnType>
void List<T>::foldLeftUtil(Node *iCurr, void (*func)(const T &, ReturnType &), ReturnType &iReturnData) const {
    if (iCurr == nullptr)
        return;

    // Apply the function
    func(iCurr->_value, iReturnData);

    return foldLeftUtil(iCurr->_link, func, iReturnData);
}

// Generic filter
template<typename T>
void List<T>::filter(const BaseFunctor *iPredicate) {
    // If no item return
    if (_head == nullptr)
        return;

    Node *aCurrent = _head;
    Node *aPrevious = nullptr;

    // Loop until we reach end
    while (aCurrent != nullptr) {
        // If predicate fails then we need to delete the node, else we proceed to next node
        if (!iPredicate->operator()(aCurrent)) {
            // If the node to be deleted is the head, then update the head
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
 * Filter the list using a Modulo operator
 * @param iDivisor: Input divisor integer
 * @param iRemainder: conditional value the result will be compared with
 *
 * Generic implementation, does not support String, Characters, non-integer data types
 */
template<typename T>
void List<T>::modFilter(const size_t iDivisor, const size_t iRemainder) {
    throw exceptions::LLUnsupportedOperationException();
}

/*
 * Filter the list using a Modulo operator
 * @param iDivisor: Input divisor integer
 * @param iRemainder: conditional value the result will be compared with
 *
 * Template fully Specialized for Integer(size_t)
 */
template<>
void List<size_t>::modFilter(const size_t iInt, const size_t iModEqualTo) {
    ModPredicate aModPredicate(iInt, iModEqualTo);

    filter(&aModPredicate);
}

/*
 * Filter the list using a Modulo operator
 * @param iDivisor: Input divisor integer
 * @param iRemainder: conditional value the result will be compared with
 *
 * Template fully Specialized for Integer(int)
 */
template<>
void List<int>::modFilter(const size_t iInt, const size_t iModEqualTo) {
    ModPredicate aModPredicate(iInt, iModEqualTo);

    filter(&aModPredicate);
}

// Destructor
template<typename T>
List<T>::~List() {
    Node *aNodePtr = _head;
    // Loop until the end and delete the nodes
    while (aNodePtr != nullptr) {
        Node *aNextNode = aNodePtr->_link;
        delete aNodePtr;
        aNodePtr = aNextNode;
    }
}

/*
 * Returns the data at the given place
 * @param iPos : Position in the list
 * @return Data
 * @throws LLOutOfRange if its out of range
 */
template<typename T>
const T &List<T>::at(const size_t iPos) const {
    Node *aNodePtr = _head;

    // if pos is out of bound, else fetch the data
    if (iPos < 0 || iPos >= this->size()) {
        throw exceptions::LLOutOfRange();
    } else {
        for (size_t i = 0; i < iPos; ++i) {
            aNodePtr = aNodePtr->_link;
        }
    }
    return aNodePtr->_value;
}

/*
 * Returns the data at the given place
 * @param iPos : Position in the list
 * @return Data
 * @throws LLOutOfRange if its out of range
 */
template<typename T>
const T &List<T>::operator[](const size_t iPos) const {
    return at(iPos);
}

/*
 * Add the given data into the list at the end
 * @param iValue : Input data to be added into the list
 * @return void
 */
template<typename T>
void List<T>::push_back(const T &iValue) {
    Node *aNewNode = new Node(iValue);
    // If there are no nodes in the list make newNode the first node, otherwise add the newNode at the end
    if (_head == nullptr) {
        _tail = _head = aNewNode;
    } else {
        _tail->_link = aNewNode;
        _tail = aNewNode;
    }

    // increment the size
    ++_size;
}

/*
 * Removes the last element from the list, if no element does nothing
 * @return void
 */
template<typename T>
void List<T>::pop_back() {
    // if head is nullptr then return

    if (_head == nullptr)
        return;

    // If there is only one node then delete the head, else delete the tail
    if (_head->_link == nullptr) {
        delete _head;
        _head = nullptr;
        _tail = nullptr;
        _size = 0;
    } else {

        Node *aNodePtr = _head;
        Node *aPrevNode = nullptr;

        // Loop until we find the tail
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

/*
 * Reverses the entire list in-place
 * @return void
 */
template<typename T>
void List<T>::reverse() {
    if (_head == nullptr)
        return;

    // Initialize the tail to head
    _tail = _head;

    // call the recursive function
    reverseUtil(_head, NULL, &_head);
}

// Private Utility method for reverse
template<typename T>
void List<T>::reverseUtil(Node *iCurr, Node *iPrev, Node **iHead) {
    //If last node mark it head
    if (iCurr->_link == nullptr) {
        *iHead = iCurr;

        /* Update next to prev node */
        iCurr->_link = iPrev;
        return;
    }

    //Save next node for recursive call
    Node *aNext = iCurr->_link;

    // update next with previous (swapping)
    iCurr->_link = iPrev;

    // do this, recursively
    reverseUtil(aNext, iCurr, iHead);
}

#endif //LIST_HPP
