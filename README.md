LE - Letters Enhancer
=======

These are functions I find useful working with text. Useful enough to make my own implementation with blackjack and hookers.

"split" makes a vector<String> out of String via Char or String:

    split("something something", ' ') --> ["something", "something"]
    split("something something", "some") --> ["thing", " thing"]

"join" joins it back:

    join(["something", "something"], ' ') --> "something something"
    join(["something", "something"], ", ") --> "something, something"

"replace" replaces stuff in a string:

    replace("something something", "thing", '?') --> "some? some?"
    replace("something something", "thing", "") --> "some some"
    replace("something something", ' ', ", ") --> "something, something"
    replace("something something", 's', 'S') --> "Something Something"

"text" and "untext" both work like boost lexical_cast, I just want them right here with no extra dependencies:

    text(123/4) --> "30"
    untext<double>("60") --> 60.0

"calc" calculates an expression in a string returning a string:

    calc("2 * ( 4 + 3.00/ (4+3) ) + 1") --> "9.84"

*NB:* calc uses char based decimal arithmetics and this means two things. You don't have to worry about owerflows and type casting, which is good. But it works rather slow, which is not so good. It is best to use it in some sort of config reading or user input processing.

If you find a bug or two, feel free to write me: akalenuk@gmail.com
