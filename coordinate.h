// Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD


#include <algorithm> // std::sort
#include <string> 
#include <utility>
#include <cassert>

using namespace std; 

class Coordinate {
        char type; 
        char direction; 
        bool discovered; 

    public: 

        Coordinate() : 
            type('.'), direction ('1'), discovered(false) {}


        Coordinate (char t, char d, bool dis): 
                type(t), direction(d), discovered(dis) {}

        char get_type()const {
            return type; 
        }

        char get_direction() const{
            return direction; 
        }

        bool get_if_discovered() const{
            return discovered; 
        }

        void is_discovered(){
            discovered = true; 
        }

        void set_direction (char d){
            assert(d == 'N' || d == 'S' || d == 'E' || d == 'W'); 
            direction = d; 
        }

        void change_type(char t){
            assert(t =='+' || t == '|' || t =='X' || t=='-' ); 
            type = t; 
        }


};






