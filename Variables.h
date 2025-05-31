// variables.h

#ifndef VARIABLES_H
#define VARIABLES_H
using namespace std;

// All of these must be read from a file!
// M  x N
int row;
int column;

// To draw the box
string col_box = "****";
string row_box = "*";

// Checking purposes
bool number_check;
bool robot_check;
bool move_check = true;
bool robot_blocked;
int which_robot;
int how_many_lives;
bool no_target;
string user_input;

// For randomized numbers (used for the look() function)
int rand_x;
int rand_y;
int rand_num_fire;

int after_rand_x;
int after_rand_y;

int *after_rand_x_ptr = &after_rand_x;
int *after_rand_y_ptr = &after_rand_y;

// File reading purposes
string line;
int start_of_line;

string robot_name;

// List of all robot types.
vector<string> AllRobotType = {"GenericRobot", "HideBot", "JumpBot", "SprintBot", "LongShotBot", "SemiAutoBot", "ThirtyShotBot", "PreciseShotBot", "ScoutBot", "TrackBot", "EagleEyeBot"};
vector<string> MovingRobotType = {"HideBot", "JumpBot", "SprintBot"};
vector<string> ShootingRobotType = {"LongShotBot", "SemiAutoBot", "ThirtyShotBot", "PreciseShotBot"};
vector<string> SeeingRobotType = {"ScoutBot", "TrackBot", "EagleEyeBot"};

// Simulation steps
int steps;
int counter = 1;

// Number of robots
int robot_num;

// Type, name, and initial position of each robot
vector<string> RobotType, RobotNames;
vector<int> RobotPositionX, RobotPositionY, RobotLives, UpgradeCount, RobotShells, RobotRespawn;
vector<bool> RobotAlive, RobotUpgrade, RobotIsHiding; // RobotAlive -> to flag the detstructed ones later

// Initial position
int position_x;
int position_y;

string position_x_str;
string position_y_str;

// Functions
// This one to find the robot's type, if an element of a vector matches that of a string
bool findInList(const vector<string>& vec, const string& str, vector<string>& database) {
    return any_of(vec.begin(), vec.end(), [&](const string& element) {
        if(str.find(element) == 0){
            database.push_back(element);
        }
        return str.find(element) == 0;
    });
}

// This one to ensure the robots are in the respective positions
// Reminder: Maybe amend this to take into consideration of the random x and y robots accidently being placed
// on the same box of another robot in the battlefield
bool findInListInt(const vector<int>& vec1, const vector<int>& vec2, int number1, int number2) {
    for (int e = 0; e < vec1.size(); ++e) {
        if (vec1[e] == number1) {
            if (vec2[e] == number2) {
                return true;
            }
        }
    }
    return false;
}

// To return the index of the robot we are looking for
int findInListIndex(const vector<int>& vec1, const vector<int>& vec2, int number1, int number2) {
    for (int e = 0; e < vec1.size(); ++e) {
        if (vec1[e] == number1) {
            if (vec2[e] == number2) {
                return e;
            }
        }
    }
    return -1;
}

#endif
