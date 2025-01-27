// Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD


#include <algorithm> // std::sort
#include <string> 
#include <utility>
#include <cassert>

using namespace std; 

class Coordinate {
        char type; 
        pair<size_t, size_t> coordinates; 
        char direction; 
        bool discovered; 

    public: 

        Coordinate() : 
            type('.'), coordinates({0,0}), direction ('1'), discovered(false) {}


        Coordinate (char t, pair<int,int> cords, char d, bool dis): 
                type(t), coordinates(cords), direction(d), discovered(dis) {}

        char get_type()const {
            return type; 
        }

        pair<size_t,size_t> get_cords () const{
            return coordinates; 
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


};






