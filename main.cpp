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
#include "Variables.h" // The variables
#include "Robots.h" // The basic robots
using namespace std;

// Program purposes
bool loop = true;

// Actual program
int main() {
    // Always randomized
    srand(time(0));
    ifstream file("fileInputRobot2.txt");

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
    vector<GenericRobot> GenericRobots;

    // Creating the objects
    for (int i = 0; i < robot_num; ++i) {
        GenericRobots.emplace_back(RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3);
        RobotLives.push_back(3);
    }

    // Do this program until the amount of steps is sufficient
    while(counter <= steps){
        cout << "\n[TURN " << counter << "]";

        // 1 robot is able to respawn
        bool respawn = true;
        for (int i = 0; i < robot_num; ++i) {
            // Respawn the robot
            if(GenericRobots[i].ShowX() <= -5 && respawn == true){
                int new_x, new_y;
                new_x = (rand() % row) + 1;
                new_y = (rand() % column) + 1;
                // Making sure that the box the robot will occupy in after respawning will not have an already-existing robot in it
                while(findInListInt(RobotPositionX, RobotPositionY, new_x, new_y) == true){
                    new_x = (rand() % row) + 1;
                    new_y = (rand() % column) + 1;
                }
                GenericRobots[i].SetNewXY(new_x, new_y);
                RobotPositionX[i] = new_x;
                RobotPositionY[i] = new_y;
                GenericRobots[i].RespawnQuote();
                // No robot can be respawned anymore until the next round
                respawn = false;
            }
            if(GenericRobots[i].ShowX() != -5){
                // If robot is not dead, then it will do its action
                GenericRobots[i].think(RobotPositionX[i],RobotPositionY[i],i);
            }
            if(how_many_lives < 3){
                GenericRobots[which_robot].LoseLives();
            }
            if(how_many_lives == 0){
                // If lives reaches 0, it dies and doesn't appear into the battlefield again until the next turn
                GenericRobots[which_robot].RobotReset();
                RobotLives[which_robot] = 3;
                RobotPositionX[which_robot] = -5;
                RobotPositionY[which_robot] = -5;
            }
            cout << "\n\n";

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
