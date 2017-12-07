#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <vector>

using namespace std;

vector<std::string> split(const string &input, const string &regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    sregex_token_iterator
            first{input.begin(), input.end(), re, -1},
            last;
    return {first, last};
}

class Node {
public:
    typedef shared_ptr<Node> NodePtr_t;

    Node(string &iId, NodePtr_t iLeft, NodePtr_t iRight, size_t iValue) : _left(iLeft), _right(iRight), _value(iValue),
                                                                          _id(stoi(iId)) {}

    Node(string &iId, NodePtr_t iLeft, NodePtr_t iRight, string &iValue) : _left(iLeft), _right(iRight),
                                                                           _value(stoi(iValue)), _id(stoi(iId)) {}

    const NodePtr_t &getLeft() const {
        return _left;
    }

    const NodePtr_t &getRight() const {
        return _right;
    }

    NodePtr_t &accessLeft() {
        return _left;
    }

    NodePtr_t &accessRight() {
        return _right;
    }

    const size_t getId() {
        return _id;
    }

    const size_t getValue() {
        return _value;
    }

    void addValue(size_t iValue) {
        _value += iValue;
    }

private:
    NodePtr_t _left, _right;
    size_t _value, _id;
};

Node::NodePtr_t find(Node::NodePtr_t &iRoot, string &iId) {
    if (iRoot != nullptr) {
        if (iRoot->getId() == stoi(iId))
            return iRoot;

        Node::NodePtr_t aLeft = find(iRoot->accessLeft(), iId);
        return (aLeft != nullptr) ? aLeft : find(iRoot->accessRight(), iId);
    }

    return nullptr;
}

size_t findMax(Node::NodePtr_t &iRoot) {
    int aMax = 0;

    if (iRoot != nullptr) {

        aMax = iRoot->getValue();

        size_t  aMaxLeft = findMax(iRoot->accessLeft());
        aMax = (aMaxLeft > aMax) ? aMaxLeft : aMax;
        size_t  aMaxRight = findMax(iRoot->accessRight());
        aMax = (aMaxRight > aMax) ?aMaxRight : aMax;
    }

    return aMax;
}

void print(const Node::NodePtr_t &iRoot) {
    if (iRoot != nullptr) {
        print(iRoot->getLeft());
        cout << "Node id " << iRoot->getId() << " value: " << iRoot->getValue() << endl;
        print(iRoot->getRight());
    }
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main() {
    Node::NodePtr_t aHeadNode = nullptr, aCurrNode = nullptr;

    int N;
    cin >> N;
    cin.ignore();
    for (int i = 0; i < N; i++) {
        string room;
        getline(cin, room);
        vector<string> aVector = split(room, " ");

        Node::NodePtr_t aLeft = nullptr, aRight = nullptr;

        if (aHeadNode == nullptr) {
            if (aVector[2] != "E")
                aLeft = make_shared<Node>(aVector[2], nullptr, nullptr, aVector[1]);
            if (aVector[3] != "E")
                aRight = make_shared<Node>(aVector[3], nullptr, nullptr, aVector[1]);

            aHeadNode = make_shared<Node>(aVector[0], aLeft, aRight, aVector[1]);
        } else {
            aCurrNode = find(aHeadNode, aVector[0]);
            if (aCurrNode != nullptr) {

                aCurrNode->addValue(stoi(aVector[1]));

                if (aVector[2] != "E")
                    aLeft = make_shared<Node>(aVector[2], nullptr, nullptr, aCurrNode->getValue());
                if (aVector[3] != "E")
                    aRight = make_shared<Node>(aVector[3], nullptr, nullptr, aCurrNode->getValue());

                aCurrNode->accessLeft() = aLeft;
                aCurrNode->accessRight() = aRight;
            }
        }
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

//    print(aHeadNode);
    cout << findMax(aHeadNode);
}