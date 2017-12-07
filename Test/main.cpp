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

//    aIntList.push_back(size_t(1));
//    aIntList.push_back(size_t(1));
    aIntList.push_back(size_t(2));
    aIntList.push_back(size_t(1));

    aIntList.print();
    aIntList.reverse();
    aIntList.print();

    cout<< "---------- Now filter the list -----" << endl;

    aIntList.modFilter(2, 0);
    aIntList.print();

    cout<< "---------- Now string the list -----" << endl;
    List<string> aStringList;
    aStringList.push_back("a");
    aStringList.print();
}