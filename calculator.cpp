#include <stdint.h> //defined fixed-width integer types -- this is C

#include <bitset> //fixed-sized sequences of bits -- java.util.BitSet in Java
#include <cmath> //mathematical functions -- java.lang.Math in Java
#include <cstdint> //C++ conventions -- this is C++
#include <fstream> //file I/O -- java.io.FileReader and java.io.FileWriter in Java
#include <iomanip> //provides functions to format input and output -- java.util.Fomatter or String.format() in Java
#include <iostream> //System.out.println in Java
#include <memory> //smart pointers -- manage memory automatically
#include <string>
#include <unordered_map> //HashMap
#include <vector> //ArrayList

#include <stack>
#include <memory>

using namespace std;

#define INPUT_CSV_FILE "/Users/innominate-majesty/Documents/GitHub/RPN_Calculator_Bitwise/rpn-input.csv"

// test controls
#define MIN_VALUE 0
#define MAX_VALUE 100
uint8_t const table_width[] = {14, 18, 14, 18, 14, 18}; //unsigned 8-bit integer (no negative values) -- similar to byte

// test harness structs and params
#define VALUE_NULLPTR -999 //similar to null in Java

enum command : uint16_t { //creates a list of commands with specific values -- similar to Java's enum
    cmd_enter = 0,
    cmd_clear,
    cmd_pop,
    cmd_top,
    cmd_left_shift,




    cmd_right_shift,
    cmd_or,
    cmd_and,
    cmd_add,
};

//holds the names of the commands in a string format -- List<string> in Java
// vector<string> command_name = {"cmd_enter",       "cmd_clear", "cmd_pop", "cmd_top", "cmd_left_shift",
//                                "cmd_right_shift", "cmd_or",    "cmd_and", "cmd_add"};

vector<string> command_name = {
    "cmd_enter", "cmd_clear", "cmd_pop", "cmd_top", 
    "cmd_left_shift", "cmd_right_shift", "cmd_or", 
    "cmd_and", "cmd_add"
};



uint8_t const width = 16U;

stack<uint16_t> rpn_stack; //stack to hold 16-bit values

/*
 * *** STUDENTS SHOULD WRITE CODE FOR THIS FUNCTION ***
 * Students should create or add any data structures needed.
 * Students should create or add any functions or classes they may need.
 */
shared_ptr<uint16_t> rpn_calc(command const cmd, uint16_t const value = 0) {
    // this is example code which returns a (smart shared) pointer to 16-bit value
    // uint16_t val = 0b1001100100000011;
    // shared_ptr<uint16_t> result = make_shared<uint16_t>(val);
    // return result;

    //Handle commands
    switch (cmd) {
        case cmd_enter: //Push a value onto the stack
            rpn_stack.push(value);
            return make_shared<uint16_t>(rpn_stack.top()); //popping all the elements
        
        case cmd_clear: //clear the stack
            while (!rpn_stack.empty()) rpn_stack.pop();
            return nullptr; //return null since the stack is empty
        
        case cmd_pop: //pop the top value off the stack
            if (rpn_stack.empty()) return nullptr; //return null if stack is empty
            rpn_stack.pop(); //pop the top element off
            if (!rpn_stack.empty()) //if it's not empty
                return make_shared<uint16_t>(rpn_stack.top()); // Return the new top of the stack
            return nullptr; //the stack will be empty after popping

        case cmd_top: //return the top value without modifying the rest of the stack
            if (rpn_stack.empty()) return nullptr;
            return make_shared<uint16_t>(rpn_stack.top()); // Return the top of the stack

        default: //if the command isn't recognized
            return nullptr;

    }

}


/*
 * *** STUDENTS SHOULD NOT NEED TO CHANGE THE CODE BELOW. IT IS A CUSTOM TEST HARNESS. ***
 */

void header() {
    cout << left << setw(table_width[0]) << setfill(' ') << "pass/fail";
    cout << left << setw(table_width[1]) << setfill(' ') << "command";
    cout << left << setw(table_width[2]) << setfill(' ') << "value";
    cout << left << setw(table_width[3]) << setfill(' ') << "value bits";
    cout << left << setw(table_width[4]) << setfill(' ') << "result";
    cout << left << setw(table_width[5]) << setfill(' ') << "result bits" << endl;

    cout << left << setw(table_width[0]) << setfill(' ') << "--------";
    cout << left << setw(table_width[1]) << setfill(' ') << "--------";
    cout << left << setw(table_width[2]) << setfill(' ') << "--------";
    cout << left << setw(table_width[3]) << setfill(' ') << "--------";
    cout << left << setw(table_width[4]) << setfill(' ') << "--------";
    cout << left << setw(table_width[5]) << setfill(' ') << "--------" << endl;
}

void print_row(bool const test_success, command const cmd, int16_t const value, shared_ptr<uint16_t> top_of_stack) {
    // print results
    string const pass_fail = test_success ? "PASS" : "FAIL";
    cout << left << setw(table_width[0]) << setfill(' ') << pass_fail;
    cout << left << setw(table_width[1]) << setfill(' ') << command_name[cmd];
    if (value == VALUE_NULLPTR) {
        cout << left << setw(table_width[2]) << setfill(' ') << " ";
        cout << left << setw(table_width[3]) << setfill(' ') << " ";
    } else {
        cout << left << setw(table_width[2]) << setfill(' ') << value;
        cout << left << setw(table_width[3]) << setfill(' ') << bitset<width>(value);
    }

    if (top_of_stack) {
        cout << left << setw(table_width[4]) << setfill(' ') << *top_of_stack;
        cout << left << setw(table_width[5]) << setfill(' ') << bitset<width>(*top_of_stack) << endl;
    } else {
        cout << left << setw(table_width[4]) << setfill(' ') << " ";
        cout << left << setw(table_width[5]) << setfill(' ') << " " << endl;
    }
}

vector<string> split(string const &s, string const &delimiter) {
    vector<string> tokens;
    size_t pos = 0;
    size_t start = 0;
    string token;
    while (pos != string::npos) {
        pos = s.find(",", start);
        token = s.substr(start, pos - start);
        tokens.push_back(token);
        start = pos + delimiter.length();
    }

    return tokens;
}

void init_command_map(unordered_map<string, command> &command_map) {
    for (size_t i = 0; i < command_name.size(); i++) {
        string const cmd = command_name[i];
        command_map[cmd] = static_cast<command>(i);
    }
}

bool parse_csv_line(string const line, unordered_map<string, command> command_map, command &input_cmd,
                    uint16_t &input_value, int32_t &answer_value) {
    try {
        vector<string> tokens = split(line, ",");

        // get command
        string cmd = tokens[0];
        // if command is  valid
        if (command_map.count(cmd) == 1) {
            input_cmd = command_map[cmd];
        } else {
            cout << "ERROR: Invalid command cmd=" << cmd << endl;
            return false;
        }

        // get input and value
        input_value = stoi(tokens[1].c_str());
        answer_value = stoi(tokens[2].c_str());

    } catch (exception const &e) {
        cout << "ERROR: Unable to parse input csv file, line=" << line << endl;
        cout << "ERROR: exception e=" << e.what() << endl;
        return false;
    }

    return true;
}

bool test() {
    // open input file
    ifstream input_file(INPUT_CSV_FILE);
    if (!input_file.is_open()) {
        cout << "ERROR: Unable to find and open the file " << INPUT_CSV_FILE << endl;
        cout << "       Make sure the path to the file is correct in your code" << endl;
        return false;
    }

    // init command map
    unordered_map<string, command> command_map;
    init_command_map(command_map);

    // print header
    header();

    bool success = true;
    bool all_test_pass = true;
    uint16_t pass = 0;
    string line;
    size_t row = 0;
    while (getline(input_file, line)) {
        // cout << "line " << row << ":" << line << endl;
        if (row > 0) {
            // parse csv line
            command input_cmd;
            uint16_t input_value;
            int32_t input_answer;
            bool parse_success = parse_csv_line(line, command_map, input_cmd, input_value, input_answer);
            if (!parse_success) {
                return false;
            }

            // set answer value
            shared_ptr<uint16_t> answer;
            if (input_answer == VALUE_NULLPTR) {
                answer = nullptr;
            } else {
                answer = make_shared<uint16_t>(input_answer);
            }

            // do the command
            shared_ptr<uint16_t> result = rpn_calc(input_cmd, input_value);

            // test the results
            bool test_success = false;
            bool both_null = answer == nullptr && result == nullptr;
            bool both_same_value = answer && result && (*answer == *result);
            if (both_null || both_same_value) {
                pass += 1;
                test_success = true;
            }

            // update all tests
            all_test_pass = all_test_pass && test_success;

            // print row
            print_row(test_success, input_cmd, input_value, result);
        }
        row += 1;
    }

    // close input file
    input_file.close();

    // summarize results
    cout << "-------------------------------------------" << endl;
    if (all_test_pass) {
        cout << "SUCCESS ";
    } else {
        cout << "FAILURE ";
    }
    const size_t num_tests = row - 1;
    cout << pass << "/" << num_tests << " passed" << endl;
    cout << "-------------------------------------------" << endl;

    return success;
}

int main() {
    if (!test()) {
        return -1;
    }
    return 0;
}
//
//
