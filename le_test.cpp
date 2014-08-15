/*
    Copyright 2013 Alexandr Kalenuk (akalenuk@gmail.com)

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <iostream>
#include "le.h"

using namespace std;

namespace LE{

    void test(){
        const string test_array[6] = {""," and ","y things. <", " href='","y","'>!"};
        const vector<string> test_vector(test_array, test_array+6);

        cout << " +++ split, join, replace: \n";
        vector<string> splitted = LE::split("dirt and dirty things. <dirt href='dirtydirt'>!", "dirt");
        cout << "\tsplit by string - " << (splitted == test_vector) << "\n";

        string joined = LE::join(test_vector, '*');
        cout << "\tjoin by char - " << (joined == "* and *y things. <* href='*y*'>!") << "\n";

        splitted = LE::split(joined, '*');
        cout << "\tsplit by char - " << (splitted == test_vector) << "\n";

        joined = LE::join(splitted, "icecream");
        cout << "\tjoin by string - " << (joined == "icecream and icecreamy things. <icecream href='icecreamyicecream'>!") << "\n";
        string replaced = LE::replace("dirt and dirty things. <dirt href='dirtydirt'>!", "dirt", "icecream");
        cout << "\treplace - " << (replaced == "icecream and icecreamy things. <icecream href='icecreamyicecream'>!") << "\n";

        cout << "\n +++ text/untext: \n";
        cout << "\ttext - " << (LE::text(123) == "123") << "\n";
        cout << "\tuntext <int> - " << (LE::untext<int>("123") == 123) << "\n";

        cout << "\n +++ calc: \n";
        cout << "\tcalc - " << (LE::calc("2 * ( 4 + 3.00/ (4+3) ) + (2 - 1) + 1") == "10.84") << "\n";
    }
}
