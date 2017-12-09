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
    void function_length(const string &iString, size_t &oData) {
        oData = 0;
        oData = iString.length();
    }

    // Converts the given string to uppercase
    void function_convert_to_caps(const string &iString, string &oData) {
        oData = "";

        for (int i = 0; i < iString.size(); i++) {
            oData.push_back(toupper(iString.at(i)));
        }
    };

    void function_concat(const string &iData, string &ioData) {
        ioData += iData;
    }

    void function_add_length(const string &iData, size_t &ioData) {
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

                // Check for tail and head member properties work fine
                {
                    if (aListInt._head != nullptr)
                        assertEqual<size_t>(2, aListInt._head->_value);
                    else
                        throw runtime_error("Head is null");

                    if (aListInt._tail == nullptr) {
                        throw runtime_error("Tail is null");
                    } else {
                        assertEqual<size_t>(2, aListInt._tail->_value);

                        if (aListInt._tail->_link != nullptr)
                            throw runtime_error("Tail link not null");
                    }
                }
            }

            // modFilter on String
            {
                List<string> aListString;
                aListString.push_back("1");

                // test X mod 2 == 0
                bool anException = false;

                try {
                    aListString.modFilter(2, 0);
                }
                catch (exceptions::LLUnsupportedOperationException &iExc) {
                    anException = true;
                }

                if (!anException)
                    throw runtime_error("Unsupported type exception not thrown");

                aListString.print(aResultStr);
                assertEqual<string>("1", aResultStr);
                assertEqual<size_t>(1, aListString.size());
            }
        }
    }
    catch (exception &iExc) {
        cout << "Filter exception, Reason : " << iExc.what() << endl;
    }

    // UT - Map operations
    try {
        // Nominal Map case
        {
            // Length
            {
                List<string> aStringList;
                List<size_t> aMappedLenghtList;

                string aResultStr;

                aStringList.push_back("ABC");
                aStringList.push_back("BCDE");
                aStringList.push_back("CDEFG");

                aStringList.map(function_length, aMappedLenghtList);

                aMappedLenghtList.print(aResultStr);
                assertEqual<string>("3,4,5", aResultStr);
            }

            //To uppercase
            {
                List<string> aStringList;
                List<string> aMappedResultList;

                string aResultStr;

                aStringList.push_back("123");
                aStringList.push_back("abcd");
                aStringList.push_back("hi29");

                aStringList.map(function_convert_to_caps, aMappedResultList);

                aMappedResultList.print(aResultStr);
                assertEqual<string>("123,ABCD,HI29", aResultStr);
            }
        }

        // Use of function_length on empty list
        {
            List<string> aStringList;
            List<size_t> aMappedLenghtList;

            string aResultStr;

            bool anExceptionThrown = false;
            try {
                aStringList.map(function_length, aMappedLenghtList);
            } catch (...) {
                anExceptionThrown = true;
            }

            if (anExceptionThrown)
                throw runtime_error("Exception thrown by Map");

            aMappedLenghtList.print(aResultStr);
            assertEqual<string>("Empty List", aResultStr);
        }
    }
    catch (exception &iExc) {
        cout << "Map exception, Reason : " << iExc.what() << endl;
    }

    // UT - Foldleft operations
    try {
        // Nominal Foldleft case
        {
            // Length
            {
                List<string> aStringList;
                size_t aTotalLength = 0;

                string aResultStr;

                aStringList.push_back("ABC");
                aStringList.push_back("BCDE");
                aStringList.push_back("CDEFG");

                aStringList.foldLeft(aTotalLength, function_add_length);

                assertEqual<size_t>(12, aTotalLength);

                // Initialize the total length
                aTotalLength = 3;
                aStringList.foldLeft(aTotalLength, function_add_length);

                assertEqual<size_t>(15, aTotalLength);
            }

            //To concatanate
            {
                List<string> aStringList;
                string aResultString;

                aStringList.push_back("123");
                aStringList.push_back("abcd");
                aStringList.push_back("hi29");

                aStringList.foldLeft(aResultString, function_concat);
                assertEqual<string>("123abcdhi29", aResultString);

                aResultString = "----";
                aStringList.foldLeft(aResultString, function_concat);
                assertEqual<string>("----123abcdhi29", aResultString);
            }
        }

        // Use of function_length on empty list
        {
            List<string> aStringList;
            string aResultString;

            bool anExceptionThrown = false;
            try {
                aStringList.foldLeft(aResultString, function_concat);
            } catch (...) {
                anExceptionThrown = true;
            }

            if (anExceptionThrown)
                throw runtime_error("Exception thrown by Map");

            assertEqual<string>("", aResultString);

            aResultString = "----";
            try {
                aStringList.foldLeft(aResultString, function_concat);
            } catch (...) {
                anExceptionThrown = true;
            }

            if (anExceptionThrown)
                throw runtime_error("Exception thrown by Map");

            assertEqual<string>("----", aResultString);
        }
    }
    catch (exception &iExc) {
        cout << "FoldLeft exception, Reason : " << iExc.what() << endl;
    }
}