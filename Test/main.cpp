#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <vector>
#include "List.hpp"

using namespace std;

size_t function_length(string& iString)
{
    return iString.length();
}

string function_convert_to_caps(string& iString)
{
    string aToReturn;

    for(int i = 0; i < iString.size(); i++) {
        aToReturn.push_back(toupper(iString.at(i)));
    }

    return aToReturn;
};

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

    cout << "---------- Now filter the list -----" << endl;

    aIntList.modFilter(2, 0);
    aIntList.print();

    cout << "---------- Now string the list -----" << endl;
    List<string> aStringList;
    aStringList.push_back("a");
    aStringList.push_back("b");
    aStringList.print();
    cout << "---------- Now string the list reverse -----" << endl;
    aStringList.reverse();
    aStringList.print();

    cout << "---------- Now Map operation on string with length  -----" << endl;
    List<size_t> aMappedLLSize;
    aStringList.push_back("Vivek");
    aStringList.print();
    aStringList.map(function_length, aMappedLLSize);
    aMappedLLSize.print();
    cout << "---------- Now Map operation on string with CAPS  -----" << endl;
    List<string> aMappedCaps;
    aStringList.map(function_convert_to_caps, aMappedCaps);
    aMappedCaps.print();
//    try {
//        aStringList.modFilter(1, 3);
//    }
//    catch (exception &iExc) {
//        std::cout << "What : " << iExc.what() << endl;
//    }

}