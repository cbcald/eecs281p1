

#include <vector>
#include <iostream>
#include <algorithm> // std::sort
#include <getopt.h>
#include <string>    // needed for VS
#include <stack>
#include <queue>
#include "coordinate.h"
#include <cstdlib> // For atoi

//Project Identifier: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD



// ----------------------------------------------------------------------------
//                    TreasureHunt Declarations
// ----------------------------------------------------------------------------


class TreasureHunt {
    
public:
    
    // Read in the file 
    void read_data();

    // Read and process command line arguments.
    void get_options(int argc, char** argv);
    
    // to be used in the driver
    void hunt();

    void land_hunt(pair<size_t, size_t> land_coordinates); 

    void sail_north(); 
    void sail_south(); 
    void sail_east(); 
    void sail_west();     

    void print_stats(); 

    int find_path(); 
    
private:

    Coordinate search_location; // hold the currect search location
    Coordinate sail_location;  // hold the current sail location

    deque<Coordinate> captian; // maybe change this to be pair type instead of Coordinate
                        // but this is the stack that will hold either sail or shore locations 

    deque <Coordinate> first_mate; //the queue container that will hold the locations in a queue 
                                // which values this will hold is determined by the command line options 

    char filetype; // this is either Map or List
    size_t dimention; // will determine how large the vector is to hold the whole map 
    int num_water_locations = 0; // to print out
    int num_land_locations = 0; // to print out 
    int times_ashore = 0; // to print out 
    Coordinate starting_location; 
    Coordinate treasure_location; 
    bool treasure_found = false; 

    // all of these are to keep track of what is specificed on the options on command line 
    string hunt_order = "NESW"; 
    string c_option = "STACK"; 
    string fm_option = "QUEUE"; 
    bool verbose_option = false; 
    bool stats_option = false; 
    bool p_option = false; 

    int path_length =0; 


    vector<vector<Coordinate>> map; // the vector that will act as a map and will be filled when read_data is called 
    
};




// ----------------------------------------------------------------------------
//                               Driver
// ----------------------------------------------------------------------------


int main(int argc, char** argv) {
    TreasureHunt treasure; 

    treasure.get_options(argc,argv); 

    treasure.read_data(); 

    treasure.hunt(); 

    //call functions read data

    // read data
}

// ----------------------------------------------------------------------------
//                    TreasureHunt Definitions
// ----------------------------------------------------------------------------



// Read and process command line options.
void TreasureHunt::get_options(int argc, char** argv) {
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;

    /*

        TODO: Add the remaining elements into the longOpts array.

    */
    // use getopt to find command line options
    struct option longOpts[] = {{ "help", no_argument, nullptr, 'h' },
                                { "captian", required_argument, nullptr, 'c'}, 
                                { "first-mate", required_argument, nullptr, 'f'}, 
                                { "hunt-order", required_argument, nullptr, 'o'},
                                { "verbose", no_argument, nullptr, 'v'},
                                { "stats", no_argument, nullptr, 's'},
                                {"show-path", required_argument, nullptr, 'p'},
                                { nullptr, 0, nullptr, '\0' }};
    
    while ((option = getopt_long(argc, argv, "hc:f:h:vsp:", longOpts, &option_index)) != -1) {
        switch (option) {
            case 'c' :{
                string arg {optarg}; 
                if (arg != "QUEUE" && arg != "STACK"){
                    cerr << "Error: invalid mode" << endl; 
                    cerr << "I dont recognize: " << arg << endl; 
                    exit(1); 
                }

                if (arg == "QUEUE"){
                    c_option = "QUEUE"; 
                }

                break; 
            }

            case 'f': {
                string arg {optarg}; 
                if (arg != "QUEUE" && arg != "STACK"){
                    cerr << "Error: invalid mode" << endl; 
                    cerr << "I dont recognize: " << arg << endl; 
                    exit(1); 
                }

                if (arg == "STACK"){
                    fm_option = "STACK"; 
                }

                break; 
            }

            case 'o': {
                string arg {optarg}; 
                if (arg != "NESW" && arg != "NEWS" && arg != "NSEW" && arg != "NSWE" && 
                    arg != "NWES" && arg != "NWSE" && arg != "ENSW" && arg != "ENWS" &&
                    arg != "ESNW" && arg != "ESWN" && arg != "EWNS" && arg != "EWSN" && 
                    arg != "SNEW" && arg != "SNWE" && arg != "SENW" && arg != "SWEN" && 
                    arg != "SWNE" && arg != "SEWN" && arg != "WNES" && arg != "WNSE" && 
                    arg != "WENS" && arg != "WESN" && arg != "WSEN" && arg != "WSNE"){
                    cerr << "Error: invalid mode" << endl; 
                    cerr << "I dont recognize: " << arg << endl; 
                    exit(1); 
                }

                hunt_order = arg; 

                break; 
            }


            case 'v': 
                verbose_option = true; 
                break; 


            case 's': 
                stats_option = true; 
                break; 

            case 'p': 
                p_option = 'n'; 
                break;           

            case 'h':
                std::cout << "some sort of help message";
                exit(0);
        }
    }
}




// Read data into the program through stdin.
void TreasureHunt::read_data() {
    string junk;
    char word; 

    while (cin >> word){
        if (word == '#'){
            getline(cin , junk); 
        }

        if (word == 'L' || word == 'M'){
            filetype = word; 
            cin >> dimention; 
            break; 
        }
    }

    if (filetype == 'M'){
            Coordinate c; 
            map.resize(dimention, vector<Coordinate>(dimention, c)); 
                char type; 
                for (size_t r = 0; r < dimention; r++){
                    for (size_t c = 0; c < dimention; c++){
                        cin >> type; 
                        pair<int, int> coordinates = {r,c}; 
                        Coordinate new_coordinate (type, coordinates ,'1', false); 
                        map[r][c] =  new_coordinate; 
                        if (type == '@'){
                            starting_location = new_coordinate; 
                        }
                    }
                }
    }
    
    else if (filetype == 'L'){
        
        Coordinate c; 
        map.resize(dimention, vector<Coordinate>(dimention, c)); 

        size_t row, col;
        char type; 
        char word; 

        while (cin >> word){
            row = static_cast<size_t>(word);
            cin >> col; 
            cin >> type;
            pair<int,int> coordinates = {row, col}; 
            Coordinate c (type, coordinates, '1', false); 
            map[row][col] = c; 
            if (type == '@'){
                starting_location = c; 
            }
        }
    }
}

// Sort and print the data.
void TreasureHunt::hunt() {

    captian.push_back(starting_location); 

    map[starting_location.get_cords().first]
        [starting_location.get_cords().second].is_discovered(); 
    
    sail_location =  map[starting_location.get_cords().first]
        [starting_location.get_cords().second]; 

    if (verbose_option){
        cout << "Treasure hunt started at: " << 
        starting_location.get_cords().first << 
        "," << starting_location.get_cords().second << endl; 
    }
    
    while (!captian.empty() && !treasure_found){
        // if stack pop back if queue pop front? 
        if (c_option == "STACK"){
            sail_location = captian.back(); 
            captian.pop_back(); 
        }

        else if (c_option == "QUEUE"){
            sail_location = captian.front(); 
            captian.pop_front(); 
        }

        for (char d: hunt_order){

            pair<size_t,size_t> current_cords = sail_location.get_cords(); //so we can easily access the locations on the other side of this location 
            size_t current_row = current_cords.first;
            size_t current_col = current_cords.second;

            if (d == 'N' && current_row != 0 && map[current_row-1][current_col].get_type() != '#' && 
                !map[current_row-1][current_col].get_if_discovered()){
                sail_north(); 
            } //end if for north direction

            else if (d == 'S' && current_row != dimention -1 && map[current_row+1][current_col].get_type() != '#' && 
                !map[current_row+1][current_col].get_if_discovered()){
                sail_south(); 
            }

            else if (d == 'E' && current_col != dimention -1 && map[current_row][current_col+1].get_type() != '#' && 
                !map[current_row][current_col+1].get_if_discovered()){
                    sail_east(); 
            }    

            else if (d == 'W' && current_col != 0 && map[current_row][current_col-1].get_type() != '#' && 
                !map[current_row][current_col-1].get_if_discovered()){
                    sail_west(); 
            } 
        }
    }

    if (!treasure_found){
        cout << "Treasure hunt failed" << endl; 
    }

    if (stats_option){
        print_stats();
    }


} 

void TreasureHunt::sail_north(){
    pair<size_t,size_t> current_cords = sail_location.get_cords(); //so we can easily access the locations on the other side of this location 
    size_t current_row = current_cords.first;
    size_t current_col = current_cords.second;

    if (map[current_row-1][current_col].get_type() == '.'){
        map[current_row-1][current_col].set_direction('N'); 
        map[current_row-1][current_col].is_discovered(); 
        num_water_locations ++; 
        captian.push_back(map[current_row-1][current_col]);

    }

    else if (map[current_row-1][current_col].get_type() == 'o' ||
        map[current_row-1][current_col].get_type() == '$'){
            map[current_row-1][current_col].set_direction('N');
            pair<size_t,size_t> c = {current_row-1, current_col}; 
            land_hunt(c);
            times_ashore ++; 
            if (verbose_option){
                cout << "Went ashore at: " << map[current_row-1][current_col].get_cords().first << 
                "," << 
                map[current_row-1][current_col].get_cords().second << endl;  
            }

            if (treasure_found){
                cout
                << "Searching island... party found treasure at: "
                << treasure_location.get_cords().first
                << "," << treasure_location.get_cords().second 
                << "." << endl; 

                // do things here for what happens when you find treasure 
            }

            else if (verbose_option){
                cout 
                << "Searching island... party returned with no treasure. " << endl; 
                //the treasure was not found 
            }

    }

}

void TreasureHunt::sail_south(){

    pair<size_t,size_t> current_cords = sail_location.get_cords(); //so we can easily access the locations on the other side of this location 
    size_t current_row = current_cords.first;
    size_t current_col = current_cords.second;

    if (map[current_row+1][current_col].get_type() == '.'){

                    map[current_row+1][current_col].is_discovered(); 
                    map[current_row+1][current_col].set_direction('S'); 
                    num_water_locations ++; 
                    captian.push_back(map[current_row+1][current_col]); 

                }

                else if (map[current_row+1][current_col].get_type() == 'o' ||
                        map[current_row+1][current_col].get_type() == '$'){
                            map[current_row+1][current_col].set_direction('S');
                            pair<size_t,size_t> south = {current_row+1,current_col}; 
                            land_hunt(south); 
                            times_ashore++;
                            if (verbose_option){
                                cout << "Went ashore at: " << map[current_row+1][current_col].get_cords().first << 
                                "," << 
                                map[current_row+1][current_col].get_cords().second << endl; 
                            }

                            if (treasure_found){
                                cout
                                    << "Searching island... party found treasure at: "
                                    << treasure_location.get_cords().first
                                    << "," << treasure_location.get_cords().second 
                                    << "." << endl; 

                                // do things here for what happens when you find treasure 
                            }

                            else if (verbose_option){

                                cout 
                                    << "Searching island... party returned with no treasure. " << endl; 
                                //the treasure was not found 
                            }

                }

}

void TreasureHunt::sail_east(){
    pair<size_t,size_t> current_cords = sail_location.get_cords(); //so we can easily access the locations on the other side of this location 
    size_t current_row = current_cords.first;
    size_t current_col = current_cords.second;

    if (map[current_row][current_col+1].get_type() == '.'){

        map[current_row][current_col+1].is_discovered(); 
        map[current_row][current_col+1].set_direction('E'); 
        num_water_locations ++; 
        captian.push_back(map[current_row][current_col+1]); 

    }

    else if (map[current_row][current_col+1].get_type() == 'o' ||
            map[current_row][current_col+1].get_type() == '$'){
            map[current_row][current_col+1].set_direction('E'); 
            pair<size_t,size_t> east = {current_row,current_col+1}; 
            land_hunt(east); 
            times_ashore++;
        if (verbose_option){
            cout << "Went ashore at: " << map[current_row][current_col+1].get_cords().first << 
            "," << 
            map[current_row][current_col+1].get_cords().second << endl; 
            }

        if (treasure_found){
            cout
            << "Searching island... party found treasure at: "
            << treasure_location.get_cords().first
            << "," << treasure_location.get_cords().second 
            << "." << endl; 
            // do things here for what happens when you find treasure 
        }

        else if (verbose_option){

            cout 
            << "Searching island... party returned with no treasure. " << endl; 
            //the treasure was not found 
            }

    }
}

void TreasureHunt::sail_west(){
    pair<size_t,size_t> current_cords = sail_location.get_cords(); //so we can easily access the locations on the other side of this location 
    size_t current_row = current_cords.first;
    size_t current_col = current_cords.second;

    if (map[current_row][current_col-1].get_type() == '.'){

        map[current_row][current_col-1].is_discovered(); 
        map[current_row][current_col-1].set_direction('W'); 
        num_water_locations ++; 
        captian.push_back(map[current_row][current_col-1]); 

    }

    else if (map[current_row][current_col-1].get_type() == 'o' ||
            map[current_row][current_col-1].get_type() == '$'){
                map[current_row][current_col-1].set_direction('W'); 
                pair<size_t,size_t> west = {current_row,current_col-1}; 
                land_hunt(west); 
                times_ashore++; 
        if (verbose_option){
            cout << "Went ashore at: " << map[current_row][current_col-1].get_cords().first << 
            "," << 
            map[current_row][current_col-1].get_cords().second << endl; 
            }

        if (treasure_found){
            cout
            << "Searching island... party found treasure at: "
            << treasure_location.get_cords().first
            << "," << treasure_location.get_cords().second 
            << "." << endl; 
            // do things here for what happens when you find treasure 
        }

        else if (verbose_option){

            cout 
            << "Searching island... party returned with no treasure. " << endl; 
            //the treasure was not found 
            }

    }
}

void TreasureHunt::land_hunt(pair<size_t, size_t> cor){
    first_mate.push_back(map[cor.first][cor.second]);
    map[cor.first][cor.second].is_discovered(); 

    while (!first_mate.empty() && !treasure_found){
        if (fm_option == "STACK"){
            search_location =first_mate.back(); 
            first_mate.pop_back(); 
        }

        else if (fm_option == "QUEUE"){
            search_location = first_mate.front();
            first_mate.pop_front(); 
        }

        for (char d: hunt_order){
            pair<size_t, size_t> current = search_location.get_cords(); 
            size_t current_row = current.first; 
            size_t current_col = current.second; //dont need to do this everytime put it outside the for loop

            if (d == 'N' && current_row != 0 && map[current_row-1][current_col].get_type() != '#' &&
                map[current_row-1][current_col].get_type() != '.'  &&  !map[current_row-1][current_col].get_if_discovered()){
                    if (map[current_row-1][current_col].get_type() == '$'){
                        treasure_found = true; 
                        treasure_location = map[current_row-1][current_col]; 
                        map[current_row-1][current_col].set_direction('N'); 
                    }

                    else if (map[current_row-1][current_col].get_type() == 'o'){
                        map[current_row-1][current_col].is_discovered(); 
                        map[current_row-1][current_col].set_direction('N'); 
                        num_land_locations++; 
                        first_mate.push_back(map[current_row-1][current_col]); 
                    }
                }

            else if (d == 'S' && current_row != dimention-1 && map[current_row+1][current_col].get_type() != '#' &&
                map[current_row+1][current_col].get_type() != '.'  &&  !map[current_row+1][current_col].get_if_discovered()){
                    if (map[current_row+1][current_col].get_type() == '$'){
                        treasure_found = true; 
                        treasure_location = map[current_row+1][current_col]; 
                        map[current_row+1][current_col].set_direction('S'); 
                        

                    }
                    else if (map[current_row+1][current_col].get_type() == 'o'){
                        map[current_row+1][current_col].is_discovered(); 
                        map[current_row+1][current_col].set_direction('S'); 
                        num_land_locations++; 
                        first_mate.push_back(map[current_row+1][current_col]); 

                    }
                }

            else if (d == 'E' && current_col != dimention-1 && map[current_row][current_col+1].get_type() != '#' &&
                map[current_row][current_col+1].get_type() != '.'  &&  !map[current_row][current_col+1].get_if_discovered()){
                    if (map[current_row][current_col+1].get_type() == '$'){
                        treasure_found = true; 
                        treasure_location = map[current_row][current_col+1]; 
                        map[current_row][current_col+1].set_direction('E'); 

                    }
                    else if (map[current_row][current_col+1].get_type() == 'o'){
                        map[current_row][current_col+1].is_discovered(); 
                        map[current_row][current_col+1].set_direction('E'); 
                        num_land_locations++; 
                        first_mate.push_back(map[current_row][current_col+1]);
                    }
                }

            else if (d == 'W' && current_col != 0 && map[current_row][current_col-1].get_type() != '#' &&
                map[current_row][current_col-1].get_type() != '.'  &&  !map[current_row][current_col-1].get_if_discovered()){
                    if (map[current_row][current_col-1].get_type() == '$'){
                        treasure_found = true; 
                        treasure_location = map[current_row][current_col-1]; 
                        map[current_row][current_col-1].set_direction('W'); 

                    }
                    else if (map[current_row][current_col-1].get_type() == 'o'){
                        map[current_row][current_col-1].is_discovered(); 
                        map[current_row][current_col-1].set_direction('W'); 
                        num_land_locations++; 
                        first_mate.push_back(map[current_row][current_col-1]); 

                    }
                }


        }



    }

}


void TreasureHunt::print_stats(){
    cout
        << "--- STATS ---" << endl
        << "Starting location: " << starting_location.get_cords().first
        << "," << starting_location.get_cords().second << endl
        << "Water locations investigated: " << num_water_locations << endl
        << "Land locations investigated: " << num_land_locations << endl
        << "Went ashore: " << times_ashore << endl; 

    if (treasure_found){
        path_length = find_path(); 
        cout
            << "Path length: " << path_length << endl
            << "Treasure location: " << treasure_location.get_cords().first
            << "," << treasure_location.get_cords().second << endl; 
    }

    cout
        << "--- STATS ---" << endl;
}

int TreasureHunt::find_path(){
    return path_length; 
}

