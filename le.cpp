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

#include <deque>
#include <string>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <iostream> // for testing only
#include "le.h"

using namespace std;

namespace LE{

    // exceptions

    struct LEDivByZeroException : public runtime_error{
        LEDivByZeroException(const string& message) : runtime_error(message){
        }
    };

    struct LEExpessionSyntaxException : public runtime_error{
        LEExpessionSyntaxException(const string& message) : runtime_error(message){
        }
    };


    // split

    template <class ComaType> void do_split(vector<String>& To, String Line, ComaType Coma, size_t Coma_len){
        size_t coma_pos = 0;
        while(coma_pos != String::npos){
            size_t new_coma_pos = Line.find(Coma, coma_pos);
            if(new_coma_pos != String::npos){
                To.push_back(Line.substr(coma_pos, new_coma_pos-coma_pos));
                coma_pos = new_coma_pos+Coma_len;
            }else{
                To.push_back(Line.substr(coma_pos, String::npos));
                coma_pos = String::npos;
            }
        }
    }

    vector<String> split(const String& Line, const Char Coma){
        vector<String> ret;
        do_split<Char>(ret, Line, Coma, 1);
        return ret;
    }

    vector<String> split(const String& Line, const String& Coma){
        vector<String> ret;
        do_split<String>(ret, Line, Coma, Coma.length());
        return ret;
    }


    // join

    template <class ComaType> struct join_with{
        String operator()(String S1, String S2){
            return (S1 + the_) + S2;
        }
        join_with(ComaType& the) : the_(the){
        }
        private: ComaType the_;
    };

    template <class ComaType> void do_join(String& To, const vector<String>& Lines, ComaType Coma){
        if(Lines.empty()){
            return;
        }else{
            join_with<ComaType> joiner = join_with<ComaType>(Coma);
            To = Lines.front() + accumulate(Lines.begin()+1, Lines.end(), String(), joiner);
            return;
        }
    }

    String join(const vector<String>& Lines, const Char Coma){
        String ret;
        do_join<Char>(ret, Lines, Coma);
        return ret;
    }

    String join(const vector<String>& Lines, const String& Coma){
        String ret;
        do_join<String>(ret, Lines, Coma);
        return ret;
    }


    // replace

    String replace(const String& Line, const Char Coma1, const Char Coma2){
        return join(split(Line, Coma1), Coma2);
    }

    String replace(const String& Line, const Char Coma1, const String& Coma2){
        return join(split(Line, Coma1), Coma2);
    }

    String replace(const String& Line, const String& Coma1, const Char Coma2){
        return join(split(Line, Coma1), Coma2);
    }

    String replace(const String& Line, const String& Coma1, const String& Coma2){
        return join(split(Line, Coma1), Coma2);
    }


    // text

    template <class T>
    String do_text(T N){
        ostringstream ss;
        ss << N;
        return ss.str();
    }

    String text(const unsigned int i){
        return do_text<unsigned int>(i);
    }

    String text(const int i){
        return do_text<int>(i);
    }

    String text(const unsigned short int i){
        return do_text<unsigned short int>(i);
    }

    String text(const short int i){
        return do_text<short int>(i);
    }

    String text(const unsigned long int i){
        return do_text<unsigned long int>(i);
    }

    String text(const long int i){
        return do_text<long int>(i);
    }

    String text(const double f){
        return do_text<double>(f);
    }

    String text(const float f){
        return do_text<float>(f);
    }


    // us calc routines

    String ensure_point(const String& S){
        if(S.find_first_of('.') == String::npos){
            return S+'.';
        }else{
            return S;
        }
    }

    String ensure_minus(const String& S){
        if(S[0] != '-'){
            return "-"+S;
        }else{
            return S;
        }
    }

    String ensure_zero(const String& S){
        if(S == "-0"){
            return "0";
        }else{
            return S;
        }
    }

    String invert(const String& S){
        if(S[0] != '-'){
            return "-"+S;
        }else{
            return S.substr(1, String::npos);
        }
    }

    String align_first_to_second(const String& S1, const String& S2){
        size_t PL1 = S1.find_first_of('.');
        size_t PR1 = S1.length() - PL1;
        size_t PL2 = S2.find_first_of('.');
        size_t PR2 = S2.length() - PL2;
        size_t PLM = max(PL1, PL2);
        size_t PRM = max(PR1, PR2);
        String S;
        for(size_t i=0; i<(PLM-PL1); i++) S += '0';
        S += S1;
        for(size_t j=0; j<(PRM-PR1); j++) S += '0';
        return S;
    }

    String _align_first_to_second(const String& S1, const String& S2){
        size_t PL1 = S1.find_first_of('.');
        size_t PR1 = S1.length() - PL1;
        size_t PL2 = S2.find_first_of('.');
        size_t PR2 = S2.length() - PL2;
        size_t PLM = max(PL1, PL2);
        size_t PRM = max(PR1, PR2);
        String S(PLM+PRM, '0');
        S.replace(PLM-PL1, S1.length(), S1);
        return S;
    }

    void align_to_each_other_with_0(const String& S1, const String& S2, String& NS1, String& NS2){
        size_t PL1 = S1.find_first_of('.');
        size_t PR1 = S1.length() - PL1;
        size_t PL2 = S2.find_first_of('.');
        size_t PR2 = S2.length() - PL2;
        size_t PLM = max(PL1, PL2);
        size_t PRM = max(PR1, PR2);

        NS1.assign(PLM+PRM+1, '0');
        NS1.replace(PLM-PL1+1, S1.length(), S1);
        NS2.assign(PLM+PRM+1, '0');
        NS2.replace(PLM-PL2+1, S2.length(), S2);
    }


    String trim_left(const String& S){
        String t = S.substr(S.find_first_not_of('0'), String::npos);
        if(t == ""){
            return "0";
        }
        if(t[0] == '.'){
            return "0"+t;
        }
        return t;
    }

    String trim_right(const String& S){
        String t = S.substr(0, S.find_last_not_of('0') + 1);
        if(t[t.length()-1] == '.'){
            return t.substr(0, t.length()-1);
        }
        return t;
    }

    String trim(const String& S){
        String s_or_null = trim_right(trim_left(S));
        if(s_or_null == ""){
            return "0";
        }else{
            return s_or_null;
        }
    }

    String us_add(const String& S1, const String& S2){
        String EPS1 = ensure_point(S1);
        String EPS2 = ensure_point(S2);

        String AS1, AS2;
        align_to_each_other_with_0(EPS1, EPS2, AS1, AS2);

        deque<Char> Res;
        Char Add = 0;

        String::reverse_iterator It1 = AS1.rbegin();
        String::reverse_iterator It2 = AS2.rbegin();
        String::reverse_iterator ItE1 = AS1.rend();
        for(; It1 != ItE1; ++It1, ++It2){
            if(*It1 == '.'){
                Res.push_front('.');
            }else{
                Char n = (*It1 -'0') + (*It2 -'0') + Add;
                if(n > 9){
                    n -= 10;
                    Add = 1;
                }else{
                    Add = 0;
                }
                Res.push_front(n + '0');
            }
        }
        return trim( String(Res.begin(), Res.end()) );
    }

    String us_sub(const String& S1, const String& S2){
        String EPS1 = ensure_point(S1);
        String EPS2 = ensure_point(S2);
        String AS1, AS2, Sign;
        align_to_each_other_with_0(EPS1, EPS2, AS1, AS2);
        if(AS1.compare(AS2) < 0){
            AS1.swap(AS2);
            Sign = "-";
        }else{
            Sign = "";
        }

        deque<Char> Res;
        Char Sub = 0;

        String::reverse_iterator It1 = AS1.rbegin();
        String::reverse_iterator It2 = AS2.rbegin();
        String::reverse_iterator ItE1 = AS1.rend();
        for(; It1 != ItE1; ++It1, ++It2){
            if(*It1 == '.'){
                Res.push_front('.');
            }else{
                Char n = 10 + (*It1 -'0') - (*It2 -'0') - Sub;
                if(n > 9){
                    n -= 10;
                    Sub = 0;
                }else{
                    Sub = 1;
                }
                Res.push_front(n + '0');
            }
        }
        return Sign + trim( String(Res.begin(), Res.end()) );
    }

    String us_mul1(const String& S, Char C){
        Char c = C - '0';
        String EPS = ensure_point(S);
        String AS = "0"+EPS;

        deque<Char> Res;
        Char Add = 0;

        String::reverse_iterator It = AS.rbegin();
        String::reverse_iterator ItE = AS.rend();
        for(; It != ItE; ++It){
            if(*It == '.'){
                Res.push_front('.');
            }else{
                Char n = (*It -'0') * c + Add;
                Add = n / 10;
                n = n % 10;
                Res.push_front(n + '0');
            }
        }
        return trim_right(String(Res.begin(), Res.end()));
    }

    String mul_10(const String& N){
        size_t point = N.find_first_of('.');
        if(point == String::npos){
            return N+"0";
        }else{
            String M=N;
            M[point] = M[point+1];
            M[point+1] = '.';
            return trim_right(M);
        }
    }

    String mul_10(const String& N, unsigned int P){
        if(P == 0){
            return N;
        }else{
            return mul_10(mul_10(N), P-1);
        }
    }

    String div_10(const String& N){
        size_t point = N.find_first_of('.');
        String M;
        if(point == String::npos){
            M = N + N[N.length()-1];
            M[N.length()-1] = '.';
        }else{
            M = N;
            M[point] = M[point-1];
            M[point-1] = '.';
        }

        if(M[0]=='.'){
            return "0"+M;
        }else{
            return M;
        }
    }

    String div_10(const String& N, unsigned int P){
        if(P == 0){
            return N;
        }else{
            return div_10(div_10(N), P-1);
        }
    }

    String us_mul(const String& S1, const String& S2){
        if(S1 == "0" or S2 == "0"){
            return "0";
        }
        if(S1.length() < S2.length()){
            return us_mul(S2, S1);
        }else{
            size_t point = S2.find_first_of('.');
            unsigned int shift;
            if(point == String::npos){
                shift = 0;
            }else{
                shift = S2.length() - point -1;
            }
            String CS2 = replace(S2, '.', "");
            String::reverse_iterator It = CS2.rbegin();
            String::reverse_iterator ItE = CS2.rend();
            vector<String> to_sum;
            to_sum.reserve(CS2.length());
            for(unsigned int i = 0; It != ItE; ++It, ++i){
                to_sum.push_back( mul_10( us_mul1(S1, *It), i));
            }
            return div_10( accumulate(to_sum.begin(), to_sum.end(), String("0"), us_add), shift);
        }
    }

    bool int_less(const String& S1, const String& S2){
        if(S1.length() < S2.length())return true;
        if(S1.length() > S2.length())return false;
        if(S1.compare(S2) < 0)return true;
        return false;
    }

    String int_div(const String& S1, const String& S2){
        if(int_less(S1, S2)){
            return "0";
        }

        String dmuls[10];
        dmuls[0] = "0";
        for(unsigned int i = 1; i <= 9; i++){
            dmuls[i] = us_mul1(S2, '0' + i);
        }

        String n = S1;
        String res;
        size_t l1 = S1.length();
        size_t l2 = S2.length();
        for(int i = l1-l2; i >= 0; i--){
            int j = 9;
            String sub = "0";
            for(; j > 0 ; j--){
                sub = trim_left(mul_10(dmuls[j], i));
                if(not int_less(n, sub)){
                    break;
                }
            }
            if(j>0){
                n = us_sub(n, sub);
            }
            res += ('0'+j);
        }
        return trim_left(res);
    }

    String us_div(const String& S1, const String& S2){
        size_t point1 = S1.find_first_of('.');
        unsigned int after_point1 = (point1 == String::npos)? 0 : S1.length()-point1-1;
        size_t point2 = S2.find_first_of('.');
        unsigned int after_point2 = (point2 == String::npos)? 0 : S2.length()-point2-1;
        unsigned int max_after_point = max(after_point1, after_point2);
        String IS1 = mul_10(S1, max_after_point*2);
        String IS2 = mul_10(S2, max_after_point);
        return div_10( int_div(IS1, IS2), max_after_point);
    }


    // add sub mul div

    String add(const String& S1, const String& S2){
        if(S1 == "" or S2 == ""){
            throw LEExpessionSyntaxException("Add arguments problem: \"" + S1 + "\" + \"" + S2 + "\"");
        }
        bool SN1 = S1[0] == '-';
        String US1 = SN1 ? S1.substr(1, String::npos) : S1;
        bool SN2 = S2[0] == '-';
        String US2 = SN2 ? S2.substr(1, String::npos) : S2;
        if( !SN1 and !SN2 ){
            return us_add(US1, US2);
        }else if( SN1 and !SN2 ){
            return us_sub(US2, US1);
        }else if( !SN1 and SN2 ){
            return us_sub(US1, US2);
        }else{
            return ensure_minus( us_add(US1, US2) );
        }
    }

    String sub(const String& S1, const String& S2){
        if(S1 == ""){
            throw LEExpessionSyntaxException("Subtract arguments problem: \"" + S1 + "\" - \"" + S2 + "\"");
        }
        bool SN1 = S1[0] == '-';
        String US1 = SN1 ? S1.substr(1, String::npos) : S1;
        String RS2 = (S2=="") ? "0" : S2; // unary fix
        bool SN2 = RS2[0] == '-';
        String US2 = SN2 ? RS2.substr(1, String::npos) : RS2;
        if( !SN1 and !SN2 ){
            return us_sub(US1, US2);
        }else if( SN1 and !SN2 ){
            return ensure_minus( us_add(US2, US1) );
        }else if( !SN1 and SN2 ){
            return us_add(US1, US2);
        }else{
            return ensure_zero( invert( us_sub(US1, US2) ) );
        }
    }

    String mul(const String& S1, const String& S2){
        if(S1 == "" or S2 == ""){
            throw LEExpessionSyntaxException("Multiply arguments problem: \"" + S1 + "\" * \"" + S2 + "\"");
        }
        if(S1 == "0" or S2 == "0"){
            return "0";
        }
        bool SN1 = S1[0] == '-';
        String US1 = SN1 ? S1.substr(1, String::npos) : S1;
        bool SN2 = S2[0] == '-';
        String US2 = SN2 ? S2.substr(1, String::npos) : S2;
        if( (!SN1 and !SN2) or (SN1 and SN2) ){
            return us_mul(US1, US2);
        }else{
            return ensure_minus( us_mul(US1, US2) );
        }
    }

    String div(const String& S1, const String& S2){
        if(S1 == "" or S2 == ""){
            throw LEExpessionSyntaxException("Divide arguments problem: \"" + S1 + "\" / \"" + S2 + "\"");
        }
        if(S1 == "0"){
            return "0";
        }
        if(S2 == "0"){
            throw LEDivByZeroException("Problem calculating: \"" + S1 + "\" / \"" + S2 + "\"");
        }
        bool SN1 = S1[0] == '-';
        String US1 = SN1 ? S1.substr(1, String::npos) : S1;
        bool SN2 = S2[0] == '-';
        String US2 = SN2 ? S2.substr(1, String::npos) : S2;
        if( (!SN1 and !SN2) or (SN1 and SN2) ){
            return us_div(US1, US2);
        }else{
            return ensure_zero( ensure_minus( us_div(US1, US2) ) );
        }
    }


    // calc

    String calc(const String& Exp){
        String S = replace(Exp, ' ', "");
        string::iterator SIt = S.begin();
        string::iterator SItE = S.end();
        for(; SIt != SItE; ++SIt){
            if( not (*SIt >= '(' and *SIt <= '9' and *SIt != ',') ){
                throw LEExpessionSyntaxException("Uncalculatable symbols in: \"" + S + "\"");
            }
        }

        size_t ob = S.find_first_of('(');
        size_t cb = S.find_last_of(')');
        if(ob!=String::npos and cb!=String::npos){
            return calc(S.substr(0, ob) + calc(S.substr(ob+1, cb-ob-1)) + S.substr(cb+1, String::npos));
        }else if(ob==String::npos and cb==String::npos){
            vector<String> pluses = split(S, '+');
            vector<String>::iterator plIt = pluses.begin();
            vector<String>::iterator plItE = pluses.end();
            vector<String> calc_pluses;
            for( ; plIt!=plItE; ++plIt){
                vector<String> minuses = split(*plIt, '-');
                vector<String>::iterator miIt = minuses.begin();
                vector<String>::iterator miItE = minuses.end();
                vector<String> calc_minuses;
                for( ; miIt!=miItE; ++miIt){
                    vector<String> divs = split(*miIt, '/');
                    vector<String>::iterator diIt = divs.begin();
                    vector<String>::iterator diItE = divs.end();
                    vector<String> calc_divs;
                    for( ; diIt!=diItE; ++diIt){
                        if((*diIt).find_first_of('*') == String::npos ){
                            calc_divs.push_back(*diIt);
                        }else{
                            vector<String> muls = split(*diIt, '*');
                            calc_divs.push_back( accumulate(muls.begin()+1, muls.end(), muls[0], mul) );
                        }
                    }
                    calc_minuses.push_back( accumulate(calc_divs.begin()+1, calc_divs.end(), calc_divs[0], div) );
                }
                calc_pluses.push_back( accumulate(calc_minuses.begin()+1, calc_minuses.end(), calc_minuses[0], sub) );
            }
            return accumulate(calc_pluses.begin()+1, calc_pluses.end(), calc_pluses[0], add);
        }else{
            throw LEExpessionSyntaxException("Brackets problem in: " + Exp);
        }
    }


    // tests

    void test(){
        const string test_array[6] = {""," and ","y things. <", " href='","y","'>!"};
        const vector<string> test_vector(test_array, test_array+6);

        cout << " +++ split, join, replace: \n";
        vector<string> splitted = LE::split("dirt and dirty things. <dirt href='dirtydirt'>!", "dirt");
        cout << "split by string - " << (splitted == test_vector) << "\n";

        string joined = LE::join(test_vector, '*');
        cout << "join by char - " << (joined == "* and *y things. <* href='*y*'>!") << "\n",

        splitted = LE::split(joined, '*');
        cout << "split by char - " << (splitted == test_vector) << "\n";

        joined = LE::join(splitted, "icecream");
        cout << "join by string - " << (joined == "icecream and icecreamy things. <icecream href='icecreamyicecream'>!") << "\n";
        string replaced = LE::replace("dirt and dirty things. <dirt href='dirtydirt'>!", "dirt", "icecream");
        cout << "replace - " << (replaced == "icecream and icecreamy things. <icecream href='icecreamyicecream'>!") << "\n";

        cout <<  "text - " << (LE::text(123) == "123") << "\n";
        cout <<  "untext <int> - " << (LE::untext<int>("123") == 123) << "\n";

        cout << "calc - " << (LE::calc("2 * ( 4 + 3.00/ (4+3) ) + 1") == "9.84") << "\n";
    }

    void calc_test(){
        cout << "\n +++ calc basic test:\n";
        cout <<  "us_add - " << (LE::us_add("1128", "896.98") == "2024.98") << "\n";
        cout <<  "us_sub - " << (LE::us_sub("1128", "896.98") == "231.02") << "\n";
        cout <<  "us_sub (neg) - " << (LE::us_sub("896.98", "1128") == "-231.02") << "\n";

        cout <<  "us_mul1 - " << (LE::us_mul1("896.98", '9') == "8072.82") << "\n";
        cout <<  "mul10 - " << (LE::mul_10(LE::mul_10(LE::mul_10("123.45"))) == "123450") << "\n";
        cout <<  "div10 - " << (LE::div_10(LE::div_10(LE::div_10("123"))) == "0.123") << "\n";
        cout <<  "us_mul - " << (LE::us_mul("896.98", "1128") == "1011793.44") << "\n";
        cout <<  "us_mul (point) - " << (LE::us_mul("1234", "5.6") == "6910.4") << "\n";

        cout <<  "us_div - " << (LE::us_div("2.5", "8.9") == "0.2") << "\n";
        cout <<  "int_div - " << (LE::int_div("1234567", "89") == "13871") << "\n";
        cout <<  "us_div - " << (LE::us_div("1234.567", "8.9") == "138.715") << "\n";
    }

    void calc_mass_test(){
        cout << "\n +++ -99 .. 99 test...\n";
        unsigned int fails = 0;
        for(int i=-99; i<=99; i++){
            for(int j=-99; j<=99; j++){
                if(LE::add(LE::text(i), LE::text(j)) != LE::text(i+j)){
                    cout << (LE::text(i) + " + " + LE::text(j) + " != " + LE::text(i+j) + "     (" + LE::add(LE::text(i), LE::text(j)) + ")") << "\n";
                    fails++;
                }
                if(LE::sub(LE::text(i), LE::text(j)) != LE::text(i-j)){
                    cout << (LE::text(i) + " - " + LE::text(j) + " != " + LE::text(i-j) + "     (" + LE::sub(LE::text(i), LE::text(j)) + ")") << "\n";
                    fails++;
                }
                if(LE::mul(LE::text(i), LE::text(j)) != LE::text(i*j)){
                    cout << (LE::text(i) + " * " + LE::text(j) + " != " + LE::text(i*j) + "     (" + LE::mul(LE::text(i), LE::text(j)) + ")") << "\n";
                    fails++;
                }
                if(j != 0){
                    if(LE::div(LE::text(i), LE::text(j)) != LE::text(i/j)){
                        cout << (LE::text(i) + " / " + LE::text(j) + " != " + LE::text(i/j) + "     (" + LE::div(LE::text(i), LE::text(j)) + ")") << "\n";
                        fails++;
                    }
                }
            }
        }
        if(fails == 0){
            cout << "passed! \n";
        }else{
            cout << "failed " << fails << " times.";
        }
    }
}
