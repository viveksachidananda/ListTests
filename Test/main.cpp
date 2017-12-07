#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <vector>
#include "List.hpp"
using namespace std;


/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main() {
    List<size_t> aIntList;

    aIntList.push_back(size_t(1));
    aIntList.push_back(size_t(2));
    aIntList.push_back(size_t(4));
    aIntList.push_back(size_t(3));

    cout << "At before : " << aIntList.at(2) <<  endl;

    aIntList.reverse();

    cout << "after reverse At : " << aIntList.at(2) <<  endl;


}