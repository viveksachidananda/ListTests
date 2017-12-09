#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>

// Made public for UT purpose
#define private public

#include "List.hpp"

using namespace std;

// private namespace for filter functions
namespace {
    // Returns the length of the given string
    void function_length(string &iString, size_t &oData) {
        oData = 0;
        oData = iString.length();
    }

    // Converts the given string to uppercase
    void function_convert_to_caps(string &iString, string &oData) {
        oData = "";

        for (int i = 0; i < iString.size(); i++) {
            oData.push_back(toupper(iString.at(i)));
        }
    };

    void function_concat(string &iData, string &ioData) {
        ioData += iData;
    }

    void function_sum(string &iData, size_t &ioData) {
        ioData += iData.length();
    }

    template<typename T>
    class AssertFailed : public exception {
    public:
        AssertFailed(const string &iReason) : _reason(iReason) {
        }

        virtual const char *what() const throw() {
            return _reason.c_str();
        }

    private:
        std::string _reason;
    };

    template<typename T>
    void assertEqual(T iExpected, T iActual) {
        if (iExpected != iActual) {
            stringstream aReason;
            aReason << "Assert Equal failed : Given --> [" << iActual << "] Expected --> [" << iExpected << "]";

            throw AssertFailed<T>(aReason.str());
        }
    }
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main() {

    List<string> aStringList;
    string aPrintString;

    aStringList.push_back("Kimi");

    cout << "---------- Now Map operation on string with length  -----" << endl;
    List<size_t> aMappedLLSize;
    aStringList.push_back("Vivek");
    aStringList.print(aPrintString);
    cout << "List : " << aPrintString << endl;
    aStringList.map(function_length, aMappedLLSize);
    aMappedLLSize.print(aPrintString);
    cout << "List : " << aPrintString << endl;
    cout << "---------- Now Map operation on string with CAPS  -----" << endl;
    List<string> aMappedCaps;
    aStringList.map(function_convert_to_caps, aMappedCaps);
    aMappedCaps.print(aPrintString);
    cout << "List : " << aPrintString << endl;

    size_t aInitialSum = 0;
    string aConcatanatedString = "";

    cout << "fold left : " << aStringList.foldLeft(aInitialSum, function_sum) << endl;
    cout << "fold left same : " << aStringList.foldLeft(aConcatanatedString, function_concat) << endl;

//    try {
//        aStringList.modFilter(1, 3);
//    }
//    catch (exception &iExc) {
//        std::cout << "What : " << iExc.what() << endl;
//    }

    // UT - for size
    try {
        List<size_t> aIntList;
        assertEqual<size_t>(0, aIntList.size());
        aIntList.push_back(1);
        assertEqual<size_t>(1, aIntList.size());
        aIntList.push_back(1);
        assertEqual<size_t>(2, aIntList.size());
        aIntList.insert(0, 3);
        assertEqual<size_t>(3, aIntList.size());
        aIntList.pop_back();
        assertEqual<size_t>(2, aIntList.size());
        aIntList.pop_back();
        assertEqual<size_t>(1, aIntList.size());
        aIntList.pop_back();
        assertEqual<size_t>(0, aIntList.size());
        aIntList.pop_back();
        assertEqual<size_t>(0, aIntList.size());
    }
    catch (exception &iExc) {
        cout << iExc.what() << endl;
    }

    // UT - for pushBack
    try {
        List<size_t> aIntList;
        string aResult;

        aIntList.push_back(1);
        aIntList.print(aResult);
        assertEqual<string>("1", aResult);

        aIntList.push_back(2);
        aIntList.print(aResult);
        assertEqual<string>("1,2", aResult);

        List<string> aStringList;
        string aResultStr;

        aStringList.push_back("ABC");
        aStringList.print(aResultStr);
        assertEqual<string>("ABC", aResultStr);

        aStringList.push_back("BCD");
        aStringList.print(aResultStr);
        assertEqual<string>("ABC,BCD", aResultStr);
    }
    catch (exception &iExc) {
        cout << "Push_back exception, Reason : " << iExc.what() << endl;
    }

    // UT - for popBack
    try {
        List<size_t> aIntList;
        string aResult;

        aIntList.push_back(1);
        aIntList.push_back(2);
        aIntList.push_back(3);

        aIntList.pop_back();

        aIntList.print(aResult);
        assertEqual<string>("1,2", aResult);

        aIntList.pop_back();
        aIntList.print(aResult);
        assertEqual<string>("1", aResult);

        aIntList.pop_back();
        aIntList.print(aResult);
        assertEqual<string>("Empty List", aResult);

        List<string> aStringList;
        string aResultStr;

        aStringList.pop_back();
        aStringList.print(aResultStr);
        assertEqual<string>("Empty List", aResult);

        aStringList.push_back("ABC");
        aStringList.push_back("BCD");
        aStringList.push_back("CDE");

        aStringList.pop_back();
        aStringList.print(aResultStr);
        assertEqual<string>("ABC,BCD", aResultStr);
    }
    catch (exception &iExc) {
        cout << "Pop_back exception, Reason : " << iExc.what() << endl;
    }

    // UT - Reverse
    try {
        // Nominal reverse case
        {
            List<string> aStringList;
            string aResultStr;

            aStringList.push_back("ABC");
            aStringList.push_back("BCD");
            aStringList.push_back("CDE");

            aStringList.reverse();
            aStringList.print(aResultStr);
            assertEqual<string>("CDE,BCD,ABC", aResultStr);

            // Check for tail and head member properties work fine
            {
                if (aStringList._head != nullptr)
                    assertEqual<string>("CDE", aStringList._head->_value);
                else
                    throw exception();

                if (aStringList._tail == nullptr or (aStringList._tail and aStringList._tail->_link != nullptr))
                    throw exception();
            }
        }

        // reverse when list has only one item
        {
            List<int> aIntList;
            aIntList.push_back(1);

            aIntList.reverse();

            // Check for tail and head member properties work fine
            {
                if (aIntList._head != nullptr)
                    assertEqual<size_t>(1, aIntList._head->_value);
                else
                    throw exception();

                if (aIntList._tail == nullptr or (aIntList._tail and aIntList._tail->_link != nullptr))
                    throw exception();
            }
        }

        // reverse when list is empty
        {
            List<int> aIntList;
            aIntList.reverse();

            // Check for tail and head member properties work fine
            {
                if (aIntList._head != nullptr)
                    throw runtime_error("Head is not null");
                if (aIntList._tail != nullptr)
                    throw runtime_error("Tail is not null");
            }
        }
    }
    catch (exception &iExc) {
        cout << "Reverse exception, Reason : " << iExc.what() << endl;
    }

    // UT - FilterMod
    try {
        string aResultStr;

        // Nominal filter case
        {
            List<int> aListInt;
            aListInt.push_back(1);
            aListInt.push_back(-2);
            aListInt.push_back(-3);
            aListInt.push_back(0);
            aListInt.push_back(-6);
            aListInt.push_back(-7);

            // test X mod 2 == 0
            aListInt.modFilter(2, 0);
            aListInt.print(aResultStr);
            assertEqual<string>("-2,0,-6", aResultStr);
        }

        // Edge filter case
        {
            // only one item and not matching
            {
                List<int> aListInt;
                aListInt.push_back(1);

                // test X mod 2 == 0
                aListInt.modFilter(2, 0);
                aListInt.print(aResultStr);
                assertEqual<string>("Empty List", aResultStr);
                assertEqual<size_t>(0, aListInt.size());

                // Check for head and tail properties
                {
                    if (aListInt._head != nullptr)
                        throw runtime_error("Head is not null");
                    if (aListInt._tail != nullptr)
                        throw runtime_error("Tail is not null");
                }
            }

            // Last item filtered
            {
                List<int> aListInt;
                aListInt.push_back(2);
                aListInt.push_back(1);

                // test X mod 2 == 0
                aListInt.modFilter(2, 0);
                aListInt.print(aResultStr);
                assertEqual<string>("2", aResultStr);
                assertEqual<size_t>(1, aListInt.size());

                // Check for head and tail properties
                // Check for tail and head member properties work fine
                {
                    if (aListInt._head != nullptr)
                        assertEqual<size_t>(2, aListInt._head->_value);
                    else
                        throw runtime_error("Head is null");

                    if (aListInt._tail == nullptr or (aListInt._tail and aListInt._tail->_link != nullptr))
                        throw runtime_error("Tail is null");
                }
            }
        }
    }
    catch (exception &iExc) {
        cout << "Filter exception, Reason : " << iExc.what() << endl;
    }
}