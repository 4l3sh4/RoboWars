/**********|**********|**********|
PROGRAM: YOUR_FILENAME.CPP / YOUR_FILENAME.H
COURSE: DATA STRUCTURES AND ALGORITHMS
TRIMESTER: 2410
NAME: ALESHA NOOR BINTI MOHAMMAD SALIM
ID: 242UC244SU
LECTURE SECTION: TC4L
TUTORIAL SECTION: T14L
EMAIL: ALESHA.NOOR.MOHAMMAD@STUDENT.MMU.EDU.MY
PHONE: 018-789-3381
**********|**********|**********/

#include <iostream>
#include <string>
#include <fstream> // Reading from file
#include <vector>
#include <algorithm> // Vector-searching purposes
#include <cstdlib> // Mainly for string-to-integer (stoi)
#include <ctime> // For randomization
using namespace std;

/*
These are all for printing purposes

for (size_t i = 0; i < myVector.size(); ++i) {
        std::cout << myVector[i] << " ";
}

for (size_t i = 0; i < (*battlefield_ptr).size(); ++i) {
        for (size_t j = 0; j < (*battlefield_ptr).size(); ++j) {
            cout << (*battlefield_ptr)[i][j];
        }
        cout << endl;
}

std::string text = "This is a test string with apple, banana, and orange.";
    std::vector<std::string> words_to_remove = {"apple", "banana", "orange"};

    for (const std::string& word : words_to_remove) {
        size_t pos = text.find(word);
        while (pos != std::string::npos) {
            text.erase(pos, word.length());
            pos = text.find(word, pos);
        }
}

*/

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

// For randomized numbers (used for the look() function)
int rand_x;
int rand_y;

int after_rand_x;
int after_rand_y;

int *after_rand_x_ptr = &after_rand_x;
int *after_rand_y_ptr = &after_rand_y;

// File reading purposes
string line;
int start_of_line;

string robot_name;

// List of all robot types.
vector<string> AllRobotType = {"GenericRobot", "HideBot", "JumpBot", "SprintBot", "LongShotBot", "SemiAutoBot", "ThirtyShotBot", "PreciseShotBot", "ScoutBot", "TrackBot", "2020Bot"};
vector<string> MovingRobotType = {"HideBot", "JumpBot", "SprintBot"};
vector<string> ShootingRobotType = {"LongShotBot", "SemiAutoBot", "ThirtyShotBot", "PreciseShotBot"};
vector<string> SeeingRobotType = {"ScoutBot", "TrackBot", "2020Bot"};

// Simulation steps
int steps;
int counter = 1;

// Number of robots
int robot_num;

// Type, name, and initial position of each robot
vector<string> RobotType, RobotNames;
vector<int> RobotPositionX, RobotPositionY, RobotLives, UpgradeCount, RobotShells, RobotRespawn;
vector<bool> RobotAlive, RobotUpgrade; //RobotAlive -> to flag the destructed ones later

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

// Classes
class MovingRobot{
    public:
        virtual void move_to(int x, int y, int index) = 0;
};

class ShootingRobot{
    public:
        virtual void fire(int x, int y, int index) = 0;
};

class SeeingRobot{
    public:
        virtual void look(int x, int y, int index) = 0;
};

class ThinkingRobot{
    public:
        virtual void think(int x, int y, int index) = 0;
};

class Robot: public MovingRobot, public ShootingRobot, public SeeingRobot, public ThinkingRobot
{
    public:
        string name;
        int robo_position_x;
        int robo_position_y;
        int lives;
};

class GenericRobot: public Robot
{
    public:
        GenericRobot(string n, int x, int y, int h){
            name = n;
            robo_position_x = x;
            robo_position_y = y;
            lives = h;

            cout << "GenericRobot " << name << " at position " << robo_position_x << ", " << robo_position_y << " is ready for battle!" << "\n";
        };

        void LoseLives(){
            lives -= 1;
        }

        // Resetting to robot to 3 lives, and letting it be out of the battlefield until the next turn.
        void RobotReset(int index){
            lives = 3;
            robo_position_x = -5;
            robo_position_y = -5;
            RobotRespawn[index] += 1; // Increment respawn count
        }

        // New position-x and position-y for the respawned robot!
        void SetNewXY(int x, int y){
            robo_position_x = x;
            robo_position_y = y;
        }

        // Just printing the x-position, lawl.
        int ShowX(){
            return robo_position_x;
        }

        void RespawnQuote(){
            cout << "\nGenericRobot " << name << " has respawned at " << robo_position_x << ", " << robo_position_y << "!";
        }

        void selfDestruct(int index) { //picheolin
            robo_position_x = -5;
            robo_position_y = -5;
            RobotPositionX[index] = -5;
            RobotPositionY[index] = -5;
            RobotLives[index] = 0;
            RobotAlive[index] = false;
        }

        virtual void think(int x, int y, int index) override{
            if (!RobotAlive[index]) return; // skip if robot is destructed

            if (RobotShells[index] == 0) {
                cout << "\nGenericRobot " << name << " is out of shells! Shutting down permanently.";
                selfDestruct(index);
                return; // Stop execution if robot has no shells //picheolin
            }

            if (lives <= 0) {
                if (RobotRespawn[index] < 3) {
                    RobotReset(index); // respawns with 3 lives and moves off grid
                    return;
                } else {
                    cout << "\nGenericRobot " << name << " has no lives left and is now lost in the battlefield.";
                    selfDestruct(index);
                    return;
                }
            }

            cout << "\nGenericRobot " << name << " is strategising...";
            how_many_lives = 3;
            look(x,y,index);
        };

        virtual void look(int x, int y, int index) override{
            move_check = true;

            // Add 1 or 0 (Possible numbers if x is 2: 3 or 2)
            rand_x = x + (rand() % 2);
            rand_y = y + (rand() % 2);

            // Subtract 1 or 0 (Possible numbers if x is 2: 1 or 2)
            // Therefore, x could be possibly 1, 2 or 3!
            rand_x = x - (rand() % 2);
            rand_y = y - (rand() % 2);

            // Making sure the robot doesn't look at a position that is out-of-bounds.
            if(rand_x <= 0){
                rand_x += 1;
            }
            if(rand_x > column){
                rand_x -= 1;
            }
            if(rand_y <= 0){
                rand_y += 1;
            }
            if(rand_y > row){
                rand_y -= 1;
            }

            robot_check = false;

            cout << "\nGenericRobot " << name << " located at " << robo_position_x << ", " << robo_position_y << " will be looking at " << rand_x << ", " << rand_y << ".\n";

            for(int e = -1; e <= 1; ++e){
                for(int f = -1; f <= 1; ++f){
                    // Checking a 3x3 area!
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,rand_x+f, rand_y+e);

                    // Making sure that the robot doesn't find itself, LMAO!
                    if(robot_check == true && robo_position_x == rand_x+f && robo_position_y == rand_y+e){
                        robot_check = false;
                    }

                    if(robot_check == true){
                        after_rand_x = rand_x+f;
                        after_rand_y = rand_y+e;
                        break;
                    }
                }
                if(robot_check == true){
                    // Checking which robot it has detected (Note: It cannot detect itself)
                    which_robot = findInListIndex(RobotPositionX, RobotPositionY, after_rand_x, after_rand_y);
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " has been revealed at " << after_rand_x << ", " << after_rand_y << "!";
                    *after_rand_x_ptr = after_rand_x;
                    *after_rand_y_ptr = after_rand_y;
                    // Do this if the robot's target is within range of its shooting
                    if(robo_position_x == after_rand_x - 1 || robo_position_x == after_rand_x || robo_position_x == after_rand_x + 1){
                        if(robo_position_y == after_rand_y - 1 || robo_position_y == after_rand_y || robo_position_y == after_rand_y + 1){
                            move_check = false;
                            fire(after_rand_x, after_rand_y, index);
                            move_to(after_rand_x, after_rand_y, index);
                        }
                    }
                    // Do this if the robot needs to move to get to its target, and then shoot
                    if(move_check == true){
                        if(robo_position_x == after_rand_x - 2 || robo_position_x == after_rand_x - 1 || robo_position_x == after_rand_x || robo_position_x == after_rand_x + 1 || robo_position_x == after_rand_x + 2){
                            if(robo_position_y == after_rand_y - 2 || robo_position_y == after_rand_y - 1 || robo_position_y == after_rand_y || robo_position_y == after_rand_y + 1 || robo_position_y == after_rand_y + 2){
                                move_to(after_rand_x, after_rand_y, index);
                                if(robot_blocked == false)
                                    fire(*after_rand_x_ptr,*after_rand_y_ptr, index);
                            }
                        }
                    }
                    break;
                }
            }
            // No robots detected
            if(robot_check == false){
                cout << "GenericRobot " << name << " could not find any robots...";
            }

        };

        // Reminder to Alesha: Fix the AI for the move_to() action to avoid possible softlocking.
        virtual void move_to(int x, int y, int index) override{
            robot_blocked = false;

            if(move_check == true){
                if((robo_position_x == x - 2) && (robo_position_y == y - 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y-1);
                    if(robot_check == true){
                        robo_position_x -= 1;
                        robo_position_y -= 1;
                        RobotPositionX[index] -= 1;
                        RobotPositionY[index] -= 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                if((robo_position_x == x + 2) && (robo_position_y == y - 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y-1);
                    if(robot_check == true){
                        robo_position_x += 1;
                        robo_position_y -= 1;
                        RobotPositionX[index] += 1;
                        RobotPositionY[index] -= 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                if((robo_position_x == x - 2) && (robo_position_y == y + 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+1);
                    if(robot_check == true){
                        robo_position_x -= 1;
                        robo_position_y += 1;
                        RobotPositionX[index] -= 1;
                        RobotPositionY[index] += 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                if((robo_position_x == x + 2) && (robo_position_y == y + 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+1);
                    if(robot_check == true){
                        robo_position_x += 1;
                        robo_position_y += 1;
                        RobotPositionX[index] += 1;
                        RobotPositionY[index] += 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                if(((robo_position_x == x - 1) || (robo_position_x == x) || (robo_position_x == x + 1)) && (robo_position_y == y - 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x,robo_position_y-1);
                    if(robot_check == true){
                        robo_position_y -= 1;
                        RobotPositionY[index] -= 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                if(((robo_position_y == y - 1) || (robo_position_y == y) || (robo_position_y == y + 1)) && (robo_position_x == x - 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y);
                    if(robot_check == true){
                        robo_position_x -= 1;
                        RobotPositionX[index] -= 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                if(((robo_position_y == y - 1) || (robo_position_y == y) || (robo_position_y == y + 1)) && (robo_position_x == x + 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y);
                    if(robot_check == true){
                        robo_position_x += 1;
                        RobotPositionX[index] += 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                if(((robo_position_x == x - 1) || (robo_position_x == x) || (robo_position_x == x + 1)) && (robo_position_y == y + 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x,robo_position_y+1);
                    if(robot_check == true){
                        robo_position_y += 1;
                        RobotPositionY[index] += 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
            }
            else{
                for(int e = -1; e <= 1; ++e){
                    for(int f = -1; f <= 1; ++f){
                        // Checking a 3x3 area!
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+f, robo_position_y+e);

                        if(robot_check == false){
                            robo_position_x = robo_position_x + f;
                            robo_position_y = robo_position_y + e;
                            RobotPositionX[index] = robo_position_x;
                            RobotPositionY[index] = robo_position_y;
                            break;
                        }
                    }
                    if(robot_check == false){
                        break;
                    }
                }
            }
            if(robo_position_x <= 0){
                robo_position_x += 1;
                RobotPositionX[index] = robo_position_x;
            }
            if(robo_position_x > column){
                robo_position_x -= 1;
                RobotPositionX[index] = robo_position_x;
            }
            if(robo_position_y <= 0){
                robo_position_y += 1;
                RobotPositionY[index] = robo_position_y;
            }
            if(robo_position_y > column){
                robo_position_y -= 1;
                RobotPositionY[index] = robo_position_y;
            }
            if(robot_blocked == false){
                cout << "\nGenericRobot " << name << " moves to position " << robo_position_x << ", " << robo_position_y << "!";
            }
            else{
                cout << "\nGenericRobot " << name << " is blocked by another robot, so it stays at position " << robo_position_x << ", " << robo_position_y << ".";
            }
            robot_check = true;
        };

        virtual void fire(int x, int y, int index) override{
            cout << "\nGenericRobot " << name << " fires at position " << x << ", " << y << "!";
            RobotShells[index] -= 1; //decrease shells by 1 //picheolin

            // Check which robot got hit
            which_robot = findInListIndex(RobotPositionX,RobotPositionY,x,y);
            if(which_robot > -1){
                RobotLives[which_robot] = RobotLives[which_robot] - 1;
                if(RobotLives[which_robot] == 1){
                    cout << "\n" << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at " << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << " now only has " << RobotLives[which_robot] << " life!";
                }
                else if(RobotLives[which_robot] == 0){
                    cout << "\n" << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at " << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << " is now DESTROYED!";
                    RobotUpgrade[index] = true;
                }
                else if(RobotLives[which_robot] > 1){
                    cout << "\n" << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at " << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << " now only has " << RobotLives[which_robot] << " lives!";
                }
                how_many_lives = RobotLives[which_robot];
            }
        };

};

class ThirtyShotBot: public GenericRobot{
    public:
    ThirtyShotBot(string n, int x, int y, int h) : GenericRobot(n, x, y, h) {
        cout << "ThirtyShotBot " << name << " with 30 shells is ready!\n";
    }
};

class ScoutBot: public GenericRobot{
    public:

    int look_uses_left = 3;

    ScoutBot(string n, int x, int y, int h) : GenericRobot(n, x, y, h) {
        cout << "ScoutBot " << name << " deployed with battlefield scan ability (3 uses)." << endl;
    }

    void look(int x, int y, int index) override {
        if (look_uses_left > 0) {
            cout << "\nScoutBot " << name << " is scanning the entire battlefield!\n";
            look_uses_left--;

            bool found = false;
            for (int i = 0; i < RobotNames.size(); ++i) {
                if (i != index && RobotAlive[i]) {
                    cout << RobotType[i] << " " << RobotNames[i]
                         << " found at " << RobotPositionX[i] << ", "
                         << RobotPositionY[i] << " with "
                         << RobotLives[i] << " lives.\n";
                    found = true;
                }
            }

            if (!found) {
                cout << "No other robots detected on the battlefield." << endl;
            }

        } else {
            cout << "\nScoutBot " << name << " has no battlefield scans left. Performing normal look.\n";
            GenericRobot::look(x, y, index);  // fallback to base behavior
        }
    }
};

// Program purposes
bool loop = true;

// Actual program
int main() {
    // Always randomized
    srand(time(0));
    ifstream file("fileInputRobot.txt");

    // If file is able to be opened
    if (file.is_open()) {
        while (getline(file, line)) {

            // Read line-by-line
            if(line.find("M by N : ") == 0){
                    line.erase(0,9);
                    size_t pos = line.find(' ');
                    column = stoi(line.substr(0,pos));
                    row = stoi(line.substr(pos));
            }

            // Steps of the simulation
            if(line.find("steps: ") == 0){
                line.erase(0,6);
                steps = stoi(line);
            }

            // Amount of robots
            if(line.find("robots: ") == 0){
                line.erase(0,7);
                robot_num = stoi(line);
            }

            // Check robot type
            if (findInList(AllRobotType, line, RobotType)){
                line = line.erase(0,line.find(" ") + 1);
                if (line.find(" ")){
                    // Check their name
                    robot_name = line.substr(0,line.find(" "));
                    // Add detected name into a vector named 'RobotNames'
                    RobotNames.push_back(robot_name);
                    line = line.erase(0,line.find(" ") + 1);
                    if (line.find(" ")){
                        // Check their position-x
                        position_x_str = line.substr(0,line.find(" "));
                        if(position_x_str == "random"){
                            position_x = (rand() % column) + 1;
                        }
                        else{
                            position_x = stoi(position_x_str);
                        }
                        // Add detected position-x into a vector named 'RobotPositionX'
                        RobotPositionX.push_back(position_x);
                        // Check their position-y
                        position_y_str = line.erase(0,line.find(" ") + 1);
                        if(position_y_str == "random"){
                            position_y = (rand() % row) + 1;
                        }
                        else{
                            position_y = stoi(position_y_str);
                        }
                        // Add detected position-y into a vector named 'RobotPositionY'
                        RobotPositionY.push_back(position_y);
                    }
                }
            }
        }
        file.close(); // Close the file
    } else {
        cout << "Unable to open file";
    }

    // Presentation starts!

    // Title
    cout << " (       )         )                 (    (\n";
    cout << " )\\ ) ( /(   (  ( /( (  (      (     )\\ ) )\\ )\n";
    cout << "(()/( )\\())( )\\ )\\()))\\))(   ' )\\   (()/((()/(\n";
    cout << " /(_)|(_)\\ )((_|(_)\\((_)()\\ |(((_)(  /(_))/(_))\n";
    cout << "(_))   ((_|(_)_  ((_)(())\\_)()\\ _ )\\(_)) (_))\n";
    cout << "| _ \\ / _ \\| _ )/ _ \\ \\((_)/ (_)_\\(_) _ \\/ __|\n";
    cout << "|   /| (_) | _ \\ (_) \\ \\/\\/ / / _ \\ |   /\\__ \\\n";
    cout << "|_|_\\ \\___/|___/\\___/ \\_/\\_/ /_/ \\_\\|_|_\\|___/\n";

    cout << "\n     \"WHO WILL BE THE LAST ONE STANDING?\" \n\n";

    cout << "______________________________________________\n\n";

    // Print out the battlefield
    for(int y = 1; y <= row; ++y){
        for (int i = 1; i <= column; ++i){
            cout << col_box;
        }

        cout << "*\n";
            for (int i = 1; i <= column; ++i){
                // Robots' initial position
                number_check = findInListInt(RobotPositionX, RobotPositionY, i, y);
                if(number_check == true){
                    cout << row_box + " X ";
                }
                else
                    cout << row_box + "   ";
            }
        cout << row_box + "\n";
    }

    for (int i = 1; i <= column; ++i){
        cout << col_box;
    }

    cout << "*\n\n";
    // Program presentation end

    // Object vector
    vector<GenericRobot*> GenericRobots;

    // Creating the objects
    for (int i = 0; i < robot_num; ++i) {
        if (RobotNames[i] == "ThirtyShotBot") {
            GenericRobots.push_back(new ThirtyShotBot(RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3));
            RobotShells.push_back(30);  // Set to 30 for ThirtyShotBot
        } else {
            GenericRobots.push_back(new GenericRobot(RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3));
            RobotShells.push_back(10);  // Default shell count
        }

        RobotLives.push_back(3);
        RobotAlive.push_back(true);
        RobotUpgrade.push_back(false);
        UpgradeCount.push_back(3);
        RobotRespawn.push_back(0);
    }
    // Do this program until the amount of steps is sufficient
    while(counter <= steps){
        cout << "\n[TURN " << counter << "]";

        // 1 robot is able to respawn
        bool respawn = true;
        for (int i = 0; i < robot_num; ++i) {
            // Respawn the robot
            if(GenericRobots[i]->ShowX() <= -5 && respawn == true){
                int new_x, new_y;
                new_x = (rand() % row) + 1;
                new_y = (rand() % column) + 1;
                // Making sure that the box the robot will occupy in after respawning will not have an already-existing robot in it
                while(findInListInt(RobotPositionX, RobotPositionY, new_x, new_y) == true){
                    new_x = (rand() % row) + 1;
                    new_y = (rand() % column) + 1;
                }
                GenericRobots[i]->SetNewXY(new_x, new_y);
                RobotPositionX[i] = new_x;
                RobotPositionY[i] = new_y;
                GenericRobots[i]->RespawnQuote();
                // No robot can be respawned anymore until the next round
                respawn = false;
            }
            if(GenericRobots[i]->ShowX() != -5){
                // If robot is not dead, then it will do its action
                GenericRobots[i]->think(RobotPositionX[i],RobotPositionY[i],i);
                //picheolin - try add upgrade logic here
            }
            if(how_many_lives < 3){
                GenericRobots[which_robot]->LoseLives();
            }
            if(how_many_lives == 0){
                // If lives reaches 0, it dies and doesn't appear into the battlefield again until the next turn
                GenericRobots[which_robot]->RobotReset(which_robot);
                RobotLives[which_robot] = 3;
                RobotPositionX[which_robot] = -5;
                RobotPositionY[which_robot] = -5;
            }
            cout << "\n\n";

            if (RobotUpgrade[i]==true && UpgradeCount[i] < 3) {

                // --- Upgrade the attacker robot --- picheolin
                int abilityType = rand() % 3;  // 0 = moving, 1 = shooting, 2 = seeing
                string chosenType;

                if (abilityType == 0) {
                    chosenType = MovingRobotType[rand() % MovingRobotType.size()];
                }
                else if (abilityType == 1) {
                    chosenType = ShootingRobotType[rand() % ShootingRobotType.size()];
                }
                else if (abilityType == 2) {
                    chosenType = SeeingRobotType[rand() % SeeingRobotType.size()];
                }

                if (!chosenType.empty()) {
                    cout << "\n" << RobotNames[which_robot] << " upgrades and transforms into a " << chosenType << "!";

                    // Save current robot info
                    string oldName = RobotNames[which_robot];
                    int oldX = RobotPositionX[which_robot];
                    int oldY = RobotPositionY[which_robot];
                    int oldLives = RobotLives[which_robot];

                    // Delete old robot object
                    delete GenericRobots[which_robot];
                    GenericRobots[which_robot] = nullptr;

                    // Create new robot based on chosenType
                    /*if (chosenType == "HideBot") {
                        GenericRobots[which_robot] = new HideBot(oldName, oldX, oldY, oldLives);
                    }
                    else if (chosenType == "JumpBot") {
                        GenericRobots[which_robot] = new JumpBot(oldName, oldX, oldY, oldLives);
                    }
                    else if (chosenType == "SprintBot") {
                        GenericRobots[which_robot] = new SprintBot(oldName, oldX, oldY, oldLives);
                    }
                    else if (chosenType == "LongShotBot") {
                        GenericRobots[which_robot] = new LongShotBot(oldName, oldX, oldY, oldLives);
                    }
                    else if (chosenType == "SemiAutoBot") {
                        GenericRobots[which_robot] = new SemiAutoBot(oldName, oldX, oldY, oldLives);
                    }
                    else */if (chosenType == "ThirtyShotBot") {
                        GenericRobots[which_robot] = new ThirtyShotBot(oldName, oldX, oldY, oldLives);
                    }
                    /*else if (chosenType == "PreciseShotBot") {
                        GenericRobots[which_robot] = new PreciseShotBot(oldName, oldX, oldY, oldLives);
                    }*/
                    else if (chosenType == "ScoutBot") {
                        GenericRobots[which_robot] = new ScoutBot(oldName, oldX, oldY, oldLives);
                    }
                    /*else if (chosenType == "TrackBot") {
                        GenericRobots[which_robot] = new TrackBot(oldName, oldX, oldY, oldLives);
                    }
                    else if (chosenType == "Bot2020") {
                        GenericRobots[which_robot] = new Bot2020(oldName, oldX, oldY, oldLives);
                    }*/
                    else {
                        // fallback to generic robot
                        GenericRobots[which_robot] = new GenericRobot(oldName, oldX, oldY, oldLives);
                    }

                    // Update robot type for the attacker
                    RobotType[which_robot] = chosenType;
                }

                RobotUpgrade[i]= false;
                UpgradeCount[i] = +1;
            }

            // Print the aftermath of the current robot's actions
            for(int y = 1; y <= row; ++y){
                for (int i = 1; i <= column; ++i){
                    cout << col_box;
                }

                cout << "*\n";
                    for (int i = 1; i <= column; ++i){
                        number_check = findInListInt(RobotPositionX, RobotPositionY, i, y);
                        if(number_check == true){
                            cout << row_box + " X ";
                        }
                        else
                            cout << row_box + "   ";
                    }
                cout << row_box + "\n";
            }

            for (int i = 1; i <= column; ++i){
                cout << col_box;
            }

            cout << "*\n\n";
        }

        counter += 1;
    }
}
