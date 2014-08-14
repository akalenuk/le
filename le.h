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

#ifndef LE_H_INCLUDED
#define LE_H_INCLUDED

#include <vector>
#include <string>
#include <sstream>

namespace LE{

    typedef std::string String;
    typedef char Char;

    /*
    split makes a vector<String> out of String via Char or String:

        split("something something", ' ') --> ["something", "something"]
        split("something something", "some") --> ["thing", " thing"]
    */
    std::vector<String> split(const String&, const Char);
    std::vector<String> split(const String&, const String&);

    /*
    join joins it back:

        join(["something", "something"], ' ') --> "something something"
        join(["something", "something"], ", ") --> "something, something"
    */
    String join(const std::vector<String>& Lines, const String& Coma);
    String join(const std::vector<String>& Lines, const Char Coma);

    /*
    replace replaces stuff in a string:

        replace("something something", "thing", '?') --> "some? some?"
        replace("something something", "thing", "") --> "some some"
        replace("something something", ' ', ", ") --> "something, something"
        replace("something something", 's', 'S') --> "Something Something"
    */
    String replace(const String&, const Char, const Char);
    String replace(const String&, const Char, const String&);
    String replace(const String&, const String&, const Char);
    String replace(const String&, const String&, const String&);

    /*
    text and untext both work like boost lexical_cast, I just want them right here:

        text(123/4) --> "30"
        untext<double>("60") --> 60.0
    */
    String text(const short int);
    String text(const int);
    String text(const long int);
    String text(const unsigned short int);
    String text(const unsigned int);
    String text(const unsigned long int);
    String text(const float);
    String text(const double);

    template <class T>
    T untext(const String& S){
        std::stringstream ss(S);
        T to;
        ss >> to;
        return to;
    }

    /*
    calc calculates an expression in a string returning a string:

        calc("2 * ( 4 + 3.00/ (4+3) ) + 1") --> "9.84"

    NB: calc uses char based decimal arithmetics and this means two things.
        You don't have to worry about owerflows and type casting, which is good.
        But it works rather slow, which is not so good. It is best to use it in
        some sort of config reading or user input processing.
    */
    String calc(const String&);
}

#endif
