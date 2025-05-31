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
    ifstream file("fileInputRobot4.txt");

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
    for (int i = 0; i < robot_num; ++i){
        if (RobotType[i] == "HideBot"){
            GenericRobots.push_back(new HideBot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 10));
            RobotShells.push_back(10);
        }
        else if (RobotType[i] == "JumpBot"){
            GenericRobots.push_back(new JumpBot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 10));
            RobotShells.push_back(10);
        }
        else if (RobotType[i] == "SprintBot"){
            GenericRobots.push_back(new SprintBot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 10));
            RobotShells.push_back(10);
        }
        else if (RobotType[i] == "LongShotBot"){
            GenericRobots.push_back(new LongShotBot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 10));
            RobotShells.push_back(10);
        }
        else if (RobotType[i] == "SemiAutoBot"){
            GenericRobots.push_back(new SemiAutoBot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 10));
            RobotShells.push_back(10);
        }
        else if (RobotType[i] == "ThirtyShotBot") {
            // Set to 30 for ThirtyShotBot
            GenericRobots.push_back(new ThirtyShotBot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 30));
            RobotShells.push_back(30);
        }
        else if (RobotType[i] == "PreciseShotBot"){
            GenericRobots.push_back(new PreciseShotBot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 10));
            RobotShells.push_back(10);
        }
        else if (RobotType[i] == "ScoutBot"){
            GenericRobots.push_back(new ScoutBot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 10));
            RobotShells.push_back(10);
        }
        else if (RobotType[i] == "TrackBot"){
            GenericRobots.push_back(new TrackBot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 10));
            RobotShells.push_back(10);
        }
        else if (RobotType[i] == "EagleEyeBot"){
            GenericRobots.push_back(new EagleEyeBot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 10));
            RobotShells.push_back(10);
        }
        else if (RobotType[i] == "GenericRobot"){
            GenericRobots.push_back(new GenericRobot(RobotType[i], RobotNames[i], RobotPositionX[i], RobotPositionY[i], 3, 10));
            GenericRobots[i]->BattleStart();
            RobotShells.push_back(10);
        }

        RobotLives.push_back(3);
        RobotIsHiding.push_back(false);
        RobotAlive.push_back(true);
        RobotUpgrade.push_back(false);
        UpgradeCount.push_back(0);
        RobotRespawn.push_back(0);
    }

    // Do this program until the amount of steps is sufficient
    while(counter <= steps){
        cout << "\n[TURN " << counter << "]\n";

        // 1 robot is able to respawn
        bool respawn = true;
        for (int i = 0; i < robot_num; ++i) {
            cout << "[ROUND " << i + 1 << "]\n";
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
            cout << "\n";

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
                    cout << RobotType[i] << " " << RobotNames[i] << " upgrades and transforms into a " << chosenType << "!\n";

                    // Save current robot info
                    string oldName = RobotNames[i];
                    int oldX = RobotPositionX[i];
                    int oldY = RobotPositionY[i];
                    int oldLives = RobotLives[i];
                    int oldShells = RobotShells[i];

                    // Delete old robot object
                    delete GenericRobots[i];
                    GenericRobots[i] = nullptr;

                    // Create new robot based on chosenType
                    if (chosenType == "HideBot") {
                        GenericRobots[i] = new HideBot("HideBot",oldName, oldX, oldY, oldLives, oldShells);
                    }

                    else if (chosenType == "JumpBot") {
                        GenericRobots[i] = new JumpBot("JumpBot",oldName, oldX, oldY, oldLives, oldShells);
                    }
                    else if (chosenType == "SprintBot") {
                        GenericRobots[i] = new SprintBot("SprintBot",oldName, oldX, oldY, oldLives, oldShells);
                    }
                    else if (chosenType == "LongShotBot") {
                        GenericRobots[i] = new LongShotBot("LongShotBot",oldName, oldX, oldY, oldLives, oldShells);
                    }
                    else if (chosenType == "SemiAutoBot") {
                        GenericRobots[i] = new SemiAutoBot("SemiAutoBot",oldName, oldX, oldY, oldLives, oldShells);
                    }
                    else if (chosenType == "ThirtyShotBot") {
                        GenericRobots[i] = new ThirtyShotBot("ThirtyShotBot",oldName, oldX, oldY, oldLives, oldShells);
                        RobotShells[i] = 30;
                    }
                    else if (chosenType == "PreciseShotBot") {
                        GenericRobots[i] = new PreciseShotBot("PreciseShotBot",oldName, oldX, oldY, oldLives, oldShells);
                    }
                    else if (chosenType == "ScoutBot") {
                        GenericRobots[i] = new ScoutBot("ScoutBot",oldName, oldX, oldY, oldLives, oldShells);
                    }
                    else if (chosenType == "TrackBot") {
                        GenericRobots[i] = new TrackBot("TrackBot",oldName, oldX, oldY, oldLives, oldShells);
                    }
                    else if (chosenType == "EagleEyeBot") {
                        GenericRobots[i] = new EagleEyeBot("EagleEyeBot",oldName, oldX, oldY, oldLives, oldShells);
                    }
                    else {
                        // fallback to generic robot
                        GenericRobots[i] = new GenericRobot("GenericRobot",oldName, oldX, oldY, oldLives, oldShells);
                    }

                    // Update robot type for the attacker
                    RobotType[i] = chosenType;
                }

                RobotUpgrade[i]= false;
                UpgradeCount[i] += 1;
            }

            // Print the aftermath of the current robot's actions
            if(GenericRobots[i]->ShowX() != -5){
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
        }

        counter += 1;

        cout << "\nEnter anything to continue...\n";
        getline(cin,user_input);
        cout << "\n";
    }
};
