// robots.h

#ifndef ROBOTS_H
#define ROBOTS_H
#include "Variables.h"
using namespace std;

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
        string type;
        string name;
        int robo_position_x;
        int robo_position_y;
        int lives;
        int shells;
        int respawn;
};

class GenericRobot: public Robot
{
    public:
        GenericRobot(string t, string n, int x, int y, int h, int s, int r){
            type = t;
            name = n;
            robo_position_x = x;
            robo_position_y = y;
            lives = h;
            shells = s;
            respawn = r;
        };

        void BattleStart(){
            cout << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] is ready for battle!" << "\n";
            outputFile << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] is ready for battle!" << "\n";
        }

        void LoseLives(){
            lives -= 1;
        }

        void LoseRespawns(){
            respawn -= 1;
        }

        // Resetting to robot to 3 lives, and letting it be out of the battlefield until the next turn.
        void RobotReset(int index){
            lives = 3;
            robo_position_x = -5;
            robo_position_y = -5;
            respawn -= 1; // Decrement respawn count
            shells = 10;
        }

        // Just printing the x-position, lawl.
        // Useful for debugging!
        int ShowX(){
            return robo_position_x;
        }

        void RespawnQuote(){
            if(respawn ==! 1){
                cout << type << " " << name << " has respawned at [" << robo_position_x << ", " << robo_position_y << "]! (" << respawn << " chances left)\n";
                outputFile << type << " " << name << " has respawned at [" << robo_position_x << ", " << robo_position_y << "]! (" << respawn << " chances left)\n";
            }
            else{
                cout << type << " " << name << " has respawned at [" << robo_position_x << ", " << robo_position_y << "]! (" << respawn << " chance left)\n";
                outputFile << type << " " << name << " has respawned at [" << robo_position_x << ", " << robo_position_y << "]! (" << respawn << " chance left)\n";
            }
        }

        void CannotRespawnQuote(){
            cout << type << " " << name << " must wait until the next turn to respawn...\n";
            outputFile << type << " " << name << " must wait until the next turn to respawn...\n";
        }

        void KilledOffQuote(){
            cout << type << " " << name << " is out of the game and can't respawn anymore!\n";
            outputFile << type << " " << name << " is out of the game and can't respawn anymore!\n";
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
                cout << type << " " << name << " is out of shells! Shutting down permanently.\n";
                outputFile << type << " " << name << " is out of shells! Shutting down permanently.\n";
                selfDestruct(index);
                return; // Stop execution if robot has no shells // picheolin
            }

            if (lives <= 0) {
                if (RobotRespawn[index] < 3) {
                    RobotReset(index); // respawns with 3 lives and moves off grid
                    return;
                } else {
                    cout << type << " " << name << " has no lives left and has now lost in the battlefield.\n";
                    outputFile << type << " " << name << " has no lives left and has now lost in the battlefield.\n";
                    selfDestruct(index);
                    return;
                }
            }

            cout << type << " " << name << " is strategising...\n";
            outputFile << type << " " << name << " is strategising...\n";
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

            cout << type << " " << name << " located at [" << robo_position_x << ", " << robo_position_y << "] will be looking at [" << rand_x << ", " << rand_y << "].\n";
            outputFile << type << " " << name << " located at [" << robo_position_x << ", " << robo_position_y << "] will be looking at [" << rand_x << ", " << rand_y << "].\n";

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
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " has been revealed at [" << after_rand_x << ", " << after_rand_y << "]!\n";
                    outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " has been revealed at [" << after_rand_x << ", " << after_rand_y << "]!\n";
                    *after_rand_x_ptr = after_rand_x;
                    *after_rand_y_ptr = after_rand_y;
                    // Do this if the robot's target is within range of its shooting
                    if(robo_position_x == after_rand_x - 1 || robo_position_x == after_rand_x || robo_position_x == after_rand_x + 1){
                        if(robo_position_y == after_rand_y - 1 || robo_position_y == after_rand_y || robo_position_y == after_rand_y + 1){
                            move_check = false;
                            no_target = false;
                            fire(after_rand_x, after_rand_y, index);
                            move_to(after_rand_x, after_rand_y, index);
                        }
                    }
                    // Do this if the robot needs to move to get to its target, and then shoot
                    if(move_check == true){
                        if(robo_position_x == after_rand_x - 2 || robo_position_x == after_rand_x - 1 || robo_position_x == after_rand_x || robo_position_x == after_rand_x + 1 || robo_position_x == after_rand_x + 2){
                            if(robo_position_y == after_rand_y - 2 || robo_position_y == after_rand_y - 1 || robo_position_y == after_rand_y || robo_position_y == after_rand_y + 1 || robo_position_y == after_rand_y + 2){
                                no_target = false;
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
                cout << type << " " << name << " could not find any robots...\n";
                outputFile << type << " " << name << " could not find any robots...\n";
                no_target = true;
                move_to(robo_position_x, robo_position_y, index);
            }
        };

        // Reminder to Alesha: Fix the AI for the move_to() action to avoid possible softlocking.
        virtual void move_to(int x, int y, int index) override{
            robot_blocked = false;

            if(no_target == true || (move_check == false && no_target == false)){
                for(int e = -1; e <= 1; ++e){
                    for(int f = -1; f <= 1; ++f){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+f,robo_position_y+e);
                        if(robot_check == false){
                            if(robo_position_x+f > 0 && robo_position_y+e > 0 && robo_position_x+f < column && robo_position_y+e < row && (robo_position_x == robo_position_x+f && robo_position_y == robo_position_y+e) == false){
                                robot_check = false;
                                robo_position_x += f;
                                robo_position_y += e;
                                RobotPositionX[index] += f;
                                RobotPositionY[index] += e;
                                robot_blocked = false;
                                break;
                            }
                            else{
                                robot_blocked = true;
                                continue;
                            }
                        }
                        else{
                            robot_blocked = true;
                        }
                    }
                    if(robot_check == false && robot_blocked == false){
                        break;
                    }
                }
            }
            else if(move_check == true && no_target == false){
                // Top left
                if((robo_position_x == x + 2) && (robo_position_y == y + 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y-1);
                    if(robot_check == false){
                        robo_position_x -= 1;
                        robo_position_y -= 1;
                        RobotPositionX[index] -= 1;
                        RobotPositionY[index] -= 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                // Top right
                else if((robo_position_x == x - 2) && (robo_position_y == y + 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y-1);
                    if(robot_check == false){
                        robo_position_x += 1;
                        robo_position_y -= 1;
                        RobotPositionX[index] += 1;
                        RobotPositionY[index] -= 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                // Bottom left
                else if((robo_position_x == x + 2) && (robo_position_y == y - 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+1);
                    if(robot_check == false){
                        robo_position_x -= 1;
                        robo_position_y += 1;
                        RobotPositionX[index] -= 1;
                        RobotPositionY[index] += 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                // Bottom right
                else if((robo_position_x == x - 2) && (robo_position_y == y - 2)){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+1);
                    if(robot_check == false){
                        robo_position_x += 1;
                        robo_position_y += 1;
                        RobotPositionX[index] += 1;
                        RobotPositionY[index] += 1;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                // Top middle, left
                else if(robo_position_x == x + 1 && robo_position_y == y + 2){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y -= 1;
                            RobotPositionY[index] -= 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                            robot_blocked = true;
                        }
                    }
                }
                // Top middle, middle
                else if(robo_position_x == x && robo_position_y == y + 2){
                    if(robo_position_x == 1){
                        for(int i = 0; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                            if(robot_check == false){
                                robo_position_x += i;
                                RobotPositionX[index] += i;
                                robo_position_y -= 1;
                                RobotPositionY[index] -= 1;
                                robot_blocked = false;
                                break;
                            }
                            else{
                                robot_blocked = true;
                            }
                        }
                    }
                    else if(robo_position_x == column){
                        for(int i = -1; i <= 0; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                            if(robot_check == false){
                                robo_position_x += i;
                                RobotPositionX[index] += i;
                                robo_position_y -= 1;
                                RobotPositionY[index] -= 1;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    else{
                        for(int i = -1; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                            if(robot_check == false){
                                robo_position_x += i;
                                RobotPositionX[index] += i;
                                robo_position_y -= 1;
                                RobotPositionY[index] -= 1;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                }
                // Top middle, right
                else if(robo_position_x == x - 1 && robo_position_y == y + 2){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y -= 1;
                            RobotPositionY[index] -= 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                // Leftmost, top
                else if(robo_position_x == x + 2 && robo_position_y == y + 1){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x -= 1;
                            RobotPositionX[index] -= 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                // Leftmost, middle
                else if(robo_position_x == x + 2 && robo_position_y == y){
                    if(robo_position_y == 1){
                        for(int i = 0; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                            if(robot_check == false){
                                robo_position_x -= 1;
                                RobotPositionX[index] -= 1;
                                robo_position_y += i;
                                RobotPositionY[index] += i;
                                robot_blocked = false;
                                break;
                            }
                            else{
                                robot_blocked = true;
                            }
                        }
                    }
                    else if(robo_position_y == row){
                        for(int i = -1; i <= 0; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                            if(robot_check == false){
                                robo_position_x -= 1;
                                RobotPositionX[index] -= 1;
                                robo_position_y += i;
                                RobotPositionY[index] += i;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    else{
                        for(int i = -1; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                            if(robot_check == false){
                                robo_position_x -= 1;
                                RobotPositionX[index] -= 1;
                                robo_position_y += i;
                                RobotPositionY[index] += i;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                }
                // Leftmost, bottom
                else if(robo_position_x == x + 2 && robo_position_y == y - 1){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x -= 1;
                            RobotPositionX[index] -= 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                // Rightmost, top
                else if(robo_position_x == x - 2 && robo_position_y == y + 1){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x += 1;
                            RobotPositionX[index] += 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                // Rightmost, middle
                else if(robo_position_x == x - 2 && robo_position_y == y){
                    if(robo_position_y == 1){
                        for(int i = 0; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                            if(robot_check == false){
                                robo_position_x += 1;
                                RobotPositionX[index] += 1;
                                robo_position_y += i;
                                RobotPositionY[index] += i;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    else if(robo_position_y == row){
                        for(int i = -1; i <= 0; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                            if(robot_check == false){
                                robo_position_x += 1;
                                RobotPositionX[index] += 1;
                                robo_position_y += i;
                                RobotPositionY[index] += i;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    else{
                        for(int i = -1; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                            if(robot_check == false){
                                robo_position_x += 1;
                                RobotPositionX[index] += 1;
                                robo_position_y += i;
                                RobotPositionY[index] += i;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                }
                // Rightmost, bottom
                else if(robo_position_x == x - 2 && robo_position_y == y - 1){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x += 1;
                            RobotPositionX[index] += 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                // Bottom middle, left
                else if(robo_position_x == x + 1 && robo_position_y == y - 2){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y += 1;
                            RobotPositionY[index] += 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                // Bottom middle, middle
                else if(robo_position_x == x && robo_position_y == y - 2){
                    if(robo_position_x == 1){
                        for(int i = 0; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                            if(robot_check == false){
                                robo_position_x += i;
                                RobotPositionX[index] += i;
                                robo_position_y += 1;
                                RobotPositionY[index] += 1;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    else if(robo_position_x == column){
                        for(int i = -1; i <= 0; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                            if(robot_check == false){
                                robo_position_x += i;
                                RobotPositionX[index] += i;
                                robo_position_y += 1;
                                RobotPositionY[index] += 1;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    else{
                        for(int i = -1; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                            if(robot_check == false){
                                robo_position_x += i;
                                RobotPositionX[index] += i;
                                robo_position_y += 1;
                                RobotPositionY[index] += 1;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                }
                // Bottom middle, right
                else if(robo_position_x == x - 1 && robo_position_y == y - 2){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y += 1;
                            RobotPositionY[index] += 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
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
                cout << type << " " << name << " moves to position [" << robo_position_x << ", " << robo_position_y << "]!\n";
                outputFile << type << " " << name << " moves to position [" << robo_position_x << ", " << robo_position_y << "]!\n";
            }
            else{
                cout << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
                outputFile << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
            }
            robot_check = true;
        };

        virtual void fire(int x, int y, int index) override{
            cout << type << " " << name << " fires at position [" << x << ", " << y << "]!\n";
            outputFile << type << " " << name << " fires at position [" << x << ", " << y << "]!\n";

            // Decrease shells by 1 // picheolin
            shells -= 1;
            RobotShells[index] -= 1;

            rand_num_fire = (rand() % 100);

            // Check which robot got hit
            which_robot = findInListIndex(RobotPositionX,RobotPositionY,x,y);
            if(which_robot > -1 && rand_num_fire <= 70){
                if (RobotIsHiding[which_robot]) {
                    cout << "...But " << RobotNames[which_robot] << " is HIDING and takes NO DAMAGE!\n";
                    outputFile << "...But " << RobotNames[which_robot] << " is HIDING and takes NO DAMAGE!\n";
                    return;
                }
                RobotLives[which_robot] = RobotLives[which_robot] - 1;
                if(RobotLives[which_robot] == 1){
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " life!\n";
                    outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    outputFile << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " life!\n";
                }
                else if(RobotLives[which_robot] == 0){
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << "] is now DESTROYED!\n";
                    outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    outputFile << ", " << RobotPositionY[which_robot] << "] is now DESTROYED!\n";

                    RobotUpgrade[index] = true;
                }
                else if(RobotLives[which_robot] > 1){
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " lives!\n";
                    outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    outputFile << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " lives!\n";
                }
                how_many_lives = RobotLives[which_robot];
            }
            else if(which_robot > -1 && rand_num_fire > 70){
                cout << "...But " << type << " " << name << " completely misses!\n";
                outputFile << "...But " << type << " " << name << " completely misses!\n";
            }

            cout << type << " " << name << " has " << shells <<  " shells left!\n";
            outputFile << type << " " << name << " has " << shells <<  " shells left!\n";
        };

};

// Moving Bots
class HideBot : public GenericRobot {
private:
    int hide_uses = 3;
    bool is_hiding = false;

public:
    HideBot(string t, string n, int x, int y, int h, int s, int r) : GenericRobot(t, n, x, y, h, s, r) {
        cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] is ready with 3 hiding chances available!\n\n";
        outputFile << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] is ready with 3 hiding chances available!\n\n";
    }

    void think(int x, int y, int index) override {
        if (!RobotAlive[index]) return; // skip if robot is destructed

        if (RobotShells[index] == 0) {
            cout << type << " " << name << " is out of shells! Shutting down permanently.\n";
            outputFile << type << " " << name << " is out of shells! Shutting down permanently.\n";
            selfDestruct(index);
            return; // Stop execution if robot has no shells // picheolin
        }

        if (lives <= 0) {
            if (RobotRespawn[index] < 3) {
                RobotReset(index); // respawns with 3 lives and moves off grid
                return;
            } else {
                cout << type << " " << name << " has no lives left and is now lost in the battlefield.\n";
                outputFile << type << " " << name << " has no lives left and is now lost in the battlefield.\n";
                selfDestruct(index);
                return;
            }
        }

        cout << type << " " << name << " is strategising...\n";
        outputFile << type << " " << name << " is strategising...\n";
        how_many_lives = 3;

        if(hide_uses > 0 && (rand() % 3 == 0)) {
            is_hiding = true;
            hide_uses--;
            RobotIsHiding[index] = true;
            if(hide_uses >= 2 || hide_uses == 0){
                cout << type << " " << RobotNames[index] << " is HIDING this turn! (" << hide_uses << " hides left)\n";
                outputFile << type << " " << RobotNames[index] << " is HIDING this turn! (" << hide_uses << " hides left)\n";
            }
            else if(hide_uses == 1){
                cout << type << " " << RobotNames[index] << " is HIDING this turn! (" << hide_uses << " hide left)\n";
                outputFile << type << " " << RobotNames[index] << " is HIDING this turn! (" << hide_uses << " hide left)\n";
            }
        }
        else{
            is_hiding = false;
            RobotIsHiding[index] = false;
            if(hide_uses >= 2 || hide_uses == 0){
                cout << type << " " << RobotNames[index] << " is NOT hiding this turn. (" << hide_uses << " hides left)\n";
                outputFile << type << " " << RobotNames[index] << " is NOT hiding this turn. (" << hide_uses << " hides left)\n";
            }
            else if(hide_uses == 1){
                cout << type << " " << RobotNames[index] << " is NOT hiding this turn. (" << hide_uses << " hide left)\n";
                outputFile << type << " " << RobotNames[index] << " is NOT hiding this turn. (" << hide_uses << " hide left)\n";
            }
        }

        look(x, y, index); // continue with normal behavior
    }
};

class JumpBot : public GenericRobot {
    private:
        int jump_uses = 3;

    public:
        JumpBot(string t, string n, int x, int y, int h, int s, int r) : GenericRobot(t, n, x, y, h, s, r) {
            cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] initialized with 3 jumps available.\n\n";
            outputFile << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] initialized with 3 jumps available.\n\n";
        }

        // Override move_to
        void move_to(int x, int y, int index) override {
            if (jump_uses > 0) {
                // Find the nearest enemy robot
                int nearest_distance = 1e9;
                int target_x = -1, target_y = -1;

                for (int i = 0; i < RobotPositionX.size(); ++i) {
                    if (i == index || RobotPositionX[i] == -5) continue; // skip self and dead robots

                    int dist = abs(RobotPositionX[index] - RobotPositionX[i]) + abs(RobotPositionY[index] - RobotPositionY[i]);
                    if (dist < nearest_distance) {
                        nearest_distance = dist;
                        target_x = RobotPositionX[i];
                        target_y = RobotPositionY[i];
                    }
                }

                // Attempt to jump away from nearest enemy
                int attempts = 0;
                int new_x, new_y;
                do {
                    new_x = 1 + (rand() % column);
                    new_y = 1 + (rand() % row);

                    int new_dist = (target_x != -1) ? abs(new_x - target_x) + abs(new_y - target_y) : 0;

                    // Ensure jump is farther from enemy and not occupied
                    if (new_dist > nearest_distance && !findInListInt(RobotPositionX, RobotPositionY, new_x, new_y)) {
                        break;
                    }

                    attempts++;
                } while (attempts < 100);

                if (attempts >= 100) {
                    cout << type << " " << RobotNames[index] << " failed to jump — no unoccupied spot found.\n";
                    outputFile << type << " " << RobotNames[index] << " failed to jump — no unoccupied spot found.\n";
                    return;
                }
                if(jump_uses >= 2 || jump_uses == 0){
                    cout << type << " " << RobotNames[index] << " jumps from [" << RobotPositionX[index] << ", "
                         << RobotPositionY[index] << "] to [" << new_x << ", " << new_y << "]. "
                         << "(" << --jump_uses << " jumps left)\n";
                    outputFile << type << " " << RobotNames[index] << " jumps from [" << RobotPositionX[index] << ", "
                               << RobotPositionY[index] << "] to [" << new_x << ", " << new_y << "]. "
                               << "(" << --jump_uses << " jumps left)\n";
                    RobotPositionX[index] = new_x;
                    RobotPositionY[index] = new_y;
                    robo_position_x = new_x;
                    robo_position_y = new_y;
                }
                else{
                    cout << type << " " << RobotNames[index] << " jumps from [" << RobotPositionX[index] << ", "
                         << RobotPositionY[index] << "] to [" << new_x << ", " << new_y << "]. "
                         << "(" << --jump_uses << " jump left)\n";
                    outputFile << type << " " << RobotNames[index] << " jumps from [" << RobotPositionX[index] << ", "
                               << RobotPositionY[index] << "] to [" << new_x << ", " << new_y << "]. "
                               << "(" << --jump_uses << " jump left)\n";
                    RobotPositionX[index] = new_x;
                    RobotPositionY[index] = new_y;
                    robo_position_x = new_x;
                    robo_position_y = new_y;
                }
            }
            // Alesha: Fixed the else part, the past else literlly kept on repeating THIS CLASS' move_to(), not GenericBot's move_to() lmao.
            else {
                robot_blocked = false;

                if(no_target == true || (move_check == false && no_target == false)){
                    for(int e = -1; e <= 1; ++e){
                        for(int f = -1; f <= 1; ++f){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+f,robo_position_y+e);
                            if(robot_check == false){
                                if(robo_position_x+f > 0 && robo_position_y+e > 0 && robo_position_x+f < column && robo_position_y+e < row && (robo_position_x == robo_position_x+f && robo_position_y == robo_position_y+e) == false){
                                    robot_check = false;
                                    robo_position_x += f;
                                    robo_position_y += e;
                                    RobotPositionX[index] += f;
                                    RobotPositionY[index] += e;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                    robot_blocked = true;
                                    continue;
                                }
                            }
                            else{
                                robot_blocked = true;
                            }
                        }
                        if(robot_check == false && robot_blocked == false){
                            break;
                        }
                    }
                }
                else if(move_check == true && no_target == false){
                    // Top left
                    if((robo_position_x == x + 2) && (robo_position_y == y + 2)){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y-1);
                        if(robot_check == false){
                            robo_position_x -= 1;
                            robo_position_y -= 1;
                            RobotPositionX[index] -= 1;
                            RobotPositionY[index] -= 1;
                        }
                        else{
                            robot_blocked = true;
                        }
                    }
                    // Top right
                    else if((robo_position_x == x - 2) && (robo_position_y == y + 2)){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y-1);
                        if(robot_check == false){
                            robo_position_x += 1;
                            robo_position_y -= 1;
                            RobotPositionX[index] += 1;
                            RobotPositionY[index] -= 1;
                        }
                        else{
                            robot_blocked = true;
                        }
                    }
                    // Bottom left
                    else if((robo_position_x == x + 2) && (robo_position_y == y - 2)){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+1);
                        if(robot_check == false){
                            robo_position_x -= 1;
                            robo_position_y += 1;
                            RobotPositionX[index] -= 1;
                            RobotPositionY[index] += 1;
                        }
                        else{
                            robot_blocked = true;
                        }
                    }
                    // Bottom right
                    else if((robo_position_x == x - 2) && (robo_position_y == y - 2)){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+1);
                        if(robot_check == false){
                            robo_position_x += 1;
                            robo_position_y += 1;
                            RobotPositionX[index] += 1;
                            RobotPositionY[index] += 1;
                        }
                        else{
                            robot_blocked = true;
                        }
                    }
                    // Top middle, left
                    else if(robo_position_x == x + 1 && robo_position_y == y + 2){
                        for(int i = -1; i <= 0; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                            if(robot_check == false){
                                robo_position_x += i;
                                RobotPositionX[index] += i;
                                robo_position_y -= 1;
                                RobotPositionY[index] -= 1;
                                robot_blocked = false;
                                break;
                            }
                            else{
                                robot_blocked = true;
                            }
                        }
                    }
                    // Top middle, middle
                    else if(robo_position_x == x && robo_position_y == y + 2){
                        if(robo_position_x == 1){
                            for(int i = 0; i <= 1; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                                if(robot_check == false){
                                    robo_position_x += i;
                                    RobotPositionX[index] += i;
                                    robo_position_y -= 1;
                                    RobotPositionY[index] -= 1;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                    robot_blocked = true;
                                }
                            }
                        }
                        else if(robo_position_x == column){
                            for(int i = -1; i <= 0; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                                if(robot_check == false){
                                    robo_position_x += i;
                                    RobotPositionX[index] += i;
                                    robo_position_y -= 1;
                                    RobotPositionY[index] -= 1;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                robot_blocked = true;
                                }
                            }
                        }
                        else{
                            for(int i = -1; i <= 1; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                                if(robot_check == false){
                                    robo_position_x += i;
                                    RobotPositionX[index] += i;
                                    robo_position_y -= 1;
                                    RobotPositionY[index] -= 1;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                robot_blocked = true;
                                }
                            }
                        }
                    }
                    // Top middle, right
                    else if(robo_position_x == x - 1 && robo_position_y == y + 2){
                        for(int i = 0; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                            if(robot_check == false){
                                robo_position_x += i;
                                RobotPositionX[index] += i;
                                robo_position_y -= 1;
                                RobotPositionY[index] -= 1;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    // Leftmost, top
                    else if(robo_position_x == x + 2 && robo_position_y == y + 1){
                        for(int i = -1; i <= 0; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                            if(robot_check == false){
                                robo_position_x -= 1;
                                RobotPositionX[index] -= 1;
                                robo_position_y += i;
                                RobotPositionY[index] += i;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    // Leftmost, middle
                    else if(robo_position_x == x + 2 && robo_position_y == y){
                        if(robo_position_y == 1){
                            for(int i = 0; i <= 1; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                                if(robot_check == false){
                                    robo_position_x -= 1;
                                    RobotPositionX[index] -= 1;
                                    robo_position_y += i;
                                    RobotPositionY[index] += i;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                    robot_blocked = true;
                                }
                            }
                        }
                        else if(robo_position_y == row){
                            for(int i = -1; i <= 0; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                                if(robot_check == false){
                                    robo_position_x -= 1;
                                    RobotPositionX[index] -= 1;
                                    robo_position_y += i;
                                    RobotPositionY[index] += i;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                robot_blocked = true;
                                }
                            }
                        }
                        else{
                            for(int i = -1; i <= 1; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                                if(robot_check == false){
                                    robo_position_x -= 1;
                                    RobotPositionX[index] -= 1;
                                    robo_position_y += i;
                                    RobotPositionY[index] += i;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                robot_blocked = true;
                                }
                            }
                        }
                    }
                    // Leftmost, bottom
                    else if(robo_position_x == x + 2 && robo_position_y == y - 1){
                        for(int i = 0; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                            if(robot_check == false){
                                robo_position_x -= 1;
                                RobotPositionX[index] -= 1;
                                robo_position_y += i;
                                RobotPositionY[index] += i;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    // Rightmost, top
                    else if(robo_position_x == x - 2 && robo_position_y == y + 1){
                        for(int i = -1; i <= 0; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                            if(robot_check == false){
                                robo_position_x += 1;
                                RobotPositionX[index] += 1;
                                robo_position_y += i;
                                RobotPositionY[index] += i;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    // Rightmost, middle
                    else if(robo_position_x == x - 2 && robo_position_y == y){
                        if(robo_position_y == 1){
                            for(int i = 0; i <= 1; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                                if(robot_check == false){
                                    robo_position_x += 1;
                                    RobotPositionX[index] += 1;
                                    robo_position_y += i;
                                    RobotPositionY[index] += i;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                robot_blocked = true;
                                }
                            }
                        }
                        else if(robo_position_y == row){
                            for(int i = -1; i <= 0; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                                if(robot_check == false){
                                    robo_position_x += 1;
                                    RobotPositionX[index] += 1;
                                    robo_position_y += i;
                                    RobotPositionY[index] += i;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                robot_blocked = true;
                                }
                            }
                        }
                        else{
                            for(int i = -1; i <= 1; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                                if(robot_check == false){
                                    robo_position_x += 1;
                                    RobotPositionX[index] += 1;
                                    robo_position_y += i;
                                    RobotPositionY[index] += i;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                robot_blocked = true;
                                }
                            }
                        }
                    }
                    // Rightmost, bottom
                    else if(robo_position_x == x - 2 && robo_position_y == y - 1){
                        for(int i = 0; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                            if(robot_check == false){
                                robo_position_x += 1;
                                RobotPositionX[index] += 1;
                                robo_position_y += i;
                                RobotPositionY[index] += i;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    // Bottom middle, left
                    else if(robo_position_x == x + 1 && robo_position_y == y - 2){
                        for(int i = -1; i <= 0; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                            if(robot_check == false){
                                robo_position_x += i;
                                RobotPositionX[index] += i;
                                robo_position_y += 1;
                                RobotPositionY[index] += 1;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
                        }
                    }
                    // Bottom middle, middle
                    else if(robo_position_x == x && robo_position_y == y - 2){
                        if(robo_position_x == 1){
                            for(int i = 0; i <= 1; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                                if(robot_check == false){
                                    robo_position_x += i;
                                    RobotPositionX[index] += i;
                                    robo_position_y += 1;
                                    RobotPositionY[index] += 1;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                robot_blocked = true;
                                }
                            }
                        }
                        else if(robo_position_x == column){
                            for(int i = -1; i <= 0; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                                if(robot_check == false){
                                    robo_position_x += i;
                                    RobotPositionX[index] += i;
                                    robo_position_y += 1;
                                    RobotPositionY[index] += 1;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                robot_blocked = true;
                                }
                            }
                        }
                        else{
                            for(int i = -1; i <= 1; ++i){
                                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                                if(robot_check == false){
                                    robo_position_x += i;
                                    RobotPositionX[index] += i;
                                    robo_position_y += 1;
                                    RobotPositionY[index] += 1;
                                    robot_blocked = false;
                                    break;
                                }
                                else{
                                robot_blocked = true;
                                }
                            }
                        }
                    }
                    // Bottom middle, right
                    else if(robo_position_x == x - 1 && robo_position_y == y - 2){
                        for(int i = 0; i <= 1; ++i){
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                            if(robot_check == false){
                                robo_position_x += i;
                                RobotPositionX[index] += i;
                                robo_position_y += 1;
                                RobotPositionY[index] += 1;
                                robot_blocked = false;
                                break;
                            }
                            else{
                            robot_blocked = true;
                            }
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
                    cout << type << " " << name << " moves to position [" << robo_position_x << ", " << robo_position_y << "]!\n";
                    outputFile << type << " " << name << " moves to position [" << robo_position_x << ", " << robo_position_y << "]!\n";
                }
                else{
                    cout << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
                    outputFile << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
                }
                robot_check = true;
                }
      }

};

class SprintBot : public GenericRobot {
public:
    SprintBot(string t, string n, int x, int y, int h, int s, int r) : GenericRobot(t, n, x, y, h, s, r) {
        cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] can move up to two spaces, and jump over robots!\n\n";
        outputFile << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] can move up to two spaces, and jump over robots!\n\n";
    }

    void move_to(int x, int y, int index) override {
        robot_blocked = false;

        if(no_target == true || (move_check == false && no_target == false)){
            for(int e = -2; e <= 2; ++e){
                for(int f = -2; f <= 2; ++f){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+f,robo_position_y+e);
                    if(robot_check == false){
                        if(robo_position_x+f > 0 && robo_position_y+e > 0 && robo_position_x+f < column && robo_position_y+e < row && (robo_position_x == robo_position_x+f && robo_position_y == robo_position_y+e) == false){
                            robot_check = false;
                            robo_position_x += f;
                            robo_position_y += e;
                            RobotPositionX[index] += f;
                            RobotPositionY[index] += e;
                            robot_blocked = false;
                            break;
                        }
                        else{
                            robot_blocked = true;
                            continue;
                        }
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                if(robot_check == false && robot_blocked == false){
                    break;
                }
            }
        }
        else if(move_check == true && no_target == false){
            // Top left
            if((robo_position_x == x + 2) && (robo_position_y == y + 2)){
                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y-1);
                if(robot_check == false){
                    robo_position_x -= 1;
                    robo_position_y -= 1;
                    RobotPositionX[index] -= 1;
                    RobotPositionY[index] -= 1;
                }
                else{
                    robot_blocked = true;
                }
            }
            // Top right
            else if((robo_position_x == x - 2) && (robo_position_y == y + 2)){
                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y-1);
                if(robot_check == false){
                    robo_position_x += 1;
                    robo_position_y -= 1;
                    RobotPositionX[index] += 1;
                    RobotPositionY[index] -= 1;
                }
                else{
                    robot_blocked = true;
                }
            }
            // Bottom left
            else if((robo_position_x == x + 2) && (robo_position_y == y - 2)){
                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+1);
                if(robot_check == false){
                    robo_position_x -= 1;
                    robo_position_y += 1;
                    RobotPositionX[index] -= 1;
                    RobotPositionY[index] += 1;
                }
                else{
                    robot_blocked = true;
                }
            }
            // Bottom right
            else if((robo_position_x == x - 2) && (robo_position_y == y - 2)){
                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+1);
                if(robot_check == false){
                    robo_position_x += 1;
                    robo_position_y += 1;
                    RobotPositionX[index] += 1;
                    RobotPositionY[index] += 1;
                }
                else{
                    robot_blocked = true;
                }
            }
            // Top middle, left
            else if(robo_position_x == x + 1 && robo_position_y == y + 2){
                for(int i = -1; i <= 0; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                    if(robot_check == false){
                        robo_position_x += i;
                        RobotPositionX[index] += i;
                        robo_position_y -= 1;
                        RobotPositionY[index] -= 1;
                        robot_blocked = false;
                        break;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
            }
            // Top middle, middle
            else if(robo_position_x == x && robo_position_y == y + 2){
                if(robo_position_x == 1){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y -= 1;
                            RobotPositionY[index] -= 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                            robot_blocked = true;
                        }
                    }
                }
                else if(robo_position_x == column){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y -= 1;
                            RobotPositionY[index] -= 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else{
                    for(int i = -1; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y -= 1;
                            RobotPositionY[index] -= 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
            }
            // Top middle, right
            else if(robo_position_x == x - 1 && robo_position_y == y + 2){
                for(int i = 0; i <= 1; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                    if(robot_check == false){
                        robo_position_x += i;
                        RobotPositionX[index] += i;
                        robo_position_y -= 1;
                        RobotPositionY[index] -= 1;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Leftmost, top
            else if(robo_position_x == x + 2 && robo_position_y == y + 1){
                for(int i = -1; i <= 0; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                    if(robot_check == false){
                        robo_position_x -= 1;
                        RobotPositionX[index] -= 1;
                        robo_position_y += i;
                        RobotPositionY[index] += i;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Leftmost, middle
            else if(robo_position_x == x + 2 && robo_position_y == y){
                if(robo_position_y == 1){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x -= 1;
                            RobotPositionX[index] -= 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                            robot_blocked = true;
                        }
                    }
                }
                else if(robo_position_y == row){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x -= 1;
                            RobotPositionX[index] -= 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else{
                    for(int i = -1; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x -= 1;
                            RobotPositionX[index] -= 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
            }
            // Leftmost, bottom
            else if(robo_position_x == x + 2 && robo_position_y == y - 1){
                for(int i = 0; i <= 1; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                    if(robot_check == false){
                        robo_position_x -= 1;
                        RobotPositionX[index] -= 1;
                        robo_position_y += i;
                        RobotPositionY[index] += i;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Rightmost, top
            else if(robo_position_x == x - 2 && robo_position_y == y + 1){
                for(int i = -1; i <= 0; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                    if(robot_check == false){
                        robo_position_x += 1;
                        RobotPositionX[index] += 1;
                        robo_position_y += i;
                        RobotPositionY[index] += i;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Rightmost, middle
            else if(robo_position_x == x - 2 && robo_position_y == y){
                if(robo_position_y == 1){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x += 1;
                            RobotPositionX[index] += 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else if(robo_position_y == row){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x += 1;
                            RobotPositionX[index] += 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else{
                    for(int i = -1; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x += 1;
                            RobotPositionX[index] += 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
            }
            // Rightmost, bottom
            else if(robo_position_x == x - 2 && robo_position_y == y - 1){
                for(int i = 0; i <= 1; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                    if(robot_check == false){
                        robo_position_x += 1;
                        RobotPositionX[index] += 1;
                        robo_position_y += i;
                        RobotPositionY[index] += i;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Bottom middle, left
            else if(robo_position_x == x + 1 && robo_position_y == y - 2){
                for(int i = -1; i <= 0; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                    if(robot_check == false){
                        robo_position_x += i;
                        RobotPositionX[index] += i;
                        robo_position_y += 1;
                        RobotPositionY[index] += 1;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Bottom middle, middle
            else if(robo_position_x == x && robo_position_y == y - 2){
                if(robo_position_x == 1){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y += 1;
                            RobotPositionY[index] += 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else if(robo_position_x == column){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y += 1;
                            RobotPositionY[index] += 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else{
                    for(int i = -1; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y += 1;
                            RobotPositionY[index] += 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
            }
            // Bottom middle, right
            else if(robo_position_x == x - 1 && robo_position_y == y - 2){
                for(int i = 0; i <= 1; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                    if(robot_check == false){
                        robo_position_x += i;
                        RobotPositionX[index] += i;
                        robo_position_y += 1;
                        RobotPositionY[index] += 1;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
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
            cout << type << " " << name << " moves to position [" << robo_position_x << ", " << robo_position_y << "]!\n";
            outputFile << type << " " << name << " moves to position [" << robo_position_x << ", " << robo_position_y << "]!\n";
        }
        else{
            cout << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
            outputFile << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
        }
        robot_check = true;

    }
};

// Shooting Bots
class LongShotBot : public GenericRobot {
public:
    LongShotBot(string t, string n, int x, int y, int h, int s, int r) : GenericRobot(t, n, x, y, h, s, r) {
        cout << "LongShotBot " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] is deployed with long-range attack mode!\n\n";
        outputFile << "LongShotBot " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] is deployed with long-range attack mode!\n\n";
    }

    void look(int x, int y, int index) override {
        cout << type << " " << RobotNames[index] << " is scanning the battlefield...\n";
        outputFile << type << " " << RobotNames[index] << " is scanning the battlefield...\n";

        for (int tx = 1; tx <= column; ++tx) {
            for (int ty = 1; ty <= row; ++ty) {
                if (tx == RobotPositionX[index] && ty == RobotPositionY[index])
                    continue;

                int dx = abs(RobotPositionX[index] - tx);
                int dy = abs(RobotPositionY[index] - ty);
                int manhattan = dx + dy;

                if (manhattan <= 5 && findInListInt(RobotPositionX, RobotPositionY, tx, ty)) {
                    fire(tx, ty, index);
                    return;
                }
            }
        }

        cout << type << " " << RobotNames[index] << " finds no target in range.\n";
        outputFile << type << " " << RobotNames[index] << " finds no target in range.\n";
    }

    void fire(int x, int y, int attackerIndex) override {
        int dx = abs(RobotPositionX[attackerIndex] - x);
        int dy = abs(RobotPositionY[attackerIndex] - y);
        int manhattan_distance = dx + dy;

        if (manhattan_distance > 5) {
            cout << type << " " << RobotNames[attackerIndex] << " attempted to fire at [" << x << ", " << y << "] but it's out of range. (range = " << manhattan_distance << ")\n";
            outputFile << type << " " << RobotNames[attackerIndex] << " attempted to fire at [" << x << ", " << y << "] but it's out of range. (range = " << manhattan_distance << ")\n";
            return;
        }

        cout << type << " " << RobotNames[attackerIndex] << " fires from ["
             << RobotPositionX[attackerIndex] << ", " << RobotPositionY[attackerIndex] << "] to [" << x << ", " << y << "]!\n";
        outputFile << type << " " << RobotNames[attackerIndex] << " fires from ["
                   << RobotPositionX[attackerIndex] << ", " << RobotPositionY[attackerIndex] << "] to [" << x << ", " << y << "]!\n";

        // Decrease shells by 1 // picheolin
        shells -= 1;
        RobotShells[attackerIndex] -= 1;

        rand_num_fire = (rand() % 100);

        // Check which robot got hit
        which_robot = findInListIndex(RobotPositionX,RobotPositionY,x,y);
        if(which_robot > -1 && rand_num_fire <= 70){
            if (RobotIsHiding[which_robot]) {
                cout << "...But " << RobotNames[which_robot] << " is HIDING and takes NO DAMAGE!\n";
                outputFile << "...But " << RobotNames[which_robot] << " is HIDING and takes NO DAMAGE!\n";
                return;
            }
            RobotLives[which_robot] = RobotLives[which_robot] - 1;
            if(RobotLives[which_robot] == 1){
                cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                cout << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " life!\n";
                outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                outputFile << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " life!\n";
            }
            else if(RobotLives[which_robot] == 0){
                cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                cout << ", " << RobotPositionY[which_robot] << "] is now DESTROYED!\n";
                outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                outputFile << ", " << RobotPositionY[which_robot] << "] is now DESTROYED!\n";

                RobotUpgrade[attackerIndex] = true;
            }
            else if(RobotLives[which_robot] > 1){
                cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                cout << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " lives!\n";
                outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                outputFile << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " lives!\n";
            }
            how_many_lives = RobotLives[which_robot];
        }
        else if(which_robot > -1 && rand_num_fire > 70){
            cout << "...But " << type << " " << name << " completely misses!\n";
            outputFile << "...But " << type << " " << name << " completely misses!\n";
        }

        cout << type << " " << name << " has " << shells <<  " shells left!\n";
        outputFile << type << " " << name << " has " << shells <<  " shells left!\n";
    }
};

class SemiAutoBot : public GenericRobot {
public:
    SemiAutoBot(string t, string n, int x, int y, int h, int s, int r) : GenericRobot(t, n, x, y, h, s, r) {
        cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] initialized with rapid fire mode!\n\n";
        outputFile << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] initialized with rapid fire mode!\n\n";
    }

    void fire(int x, int y, int index) override {
        cout << type << " " << RobotNames[index] << " unleashes rapid fire at [" << x << ", " << y << "]!\n";
        outputFile << type << " " << RobotNames[index] << " unleashes rapid fire at [" << x << ", " << y << "]!\n";

        int hitCount = 0;

        for (int shot =1; shot <= 3; ++shot) {
            int chance = rand() % 100; // 0 to 99
            cout << "Shot " << shot << ": ";
            outputFile << "Shot " << shot << ": ";
            if (chance < 70) {
                cout << "Hit!\n";
                outputFile << "Hit!\n";
                hitCount++;
            }
            else {
                cout << "Miss!\n";
                outputFile << "Miss!\n";
            }
        }

        if (hitCount == 0) {
            cout << "All 3 shots missed!\n";
            outputFile << "All 3 shots missed!\n";
            return;
        }

        // Decrease shells by 1 // picheolin
        shells -= 1;
        RobotShells[index] -= 1;

        rand_num_fire = (rand() % 100);

        // Check which robot got hit
        int target_index = findInListIndex(RobotPositionX,RobotPositionY,x,y);
        if (target_index != -1) {
            if (RobotIsHiding[target_index]) {
                cout << "But " << RobotNames[target_index] << " is HIDING and takes NO DAMAGE!\n";
                outputFile << "But " << RobotNames[target_index] << " is HIDING and takes NO DAMAGE!\n";
                return;
            }

            RobotLives[target_index] -= hitCount;

            if (RobotLives[target_index] < 0) RobotLives[target_index] = 0;

            if(RobotLives[target_index] == 0){
                cout << RobotType[target_index] << " " << RobotNames[target_index] << " positioned at [" << RobotPositionX[target_index];
                cout << ", " << RobotPositionY[target_index] << "] is now DESTROYED!\n";
                outputFile << RobotType[target_index] << " " << RobotNames[target_index] << " positioned at [" << RobotPositionX[target_index];
                outputFile << ", " << RobotPositionY[target_index] << "] is now DESTROYED!\n";

                RobotUpgrade[index] = true;
            }
            else if(RobotLives[target_index] > 1){
                cout << RobotType[target_index] << " " << RobotNames[target_index] << " positioned at [" << RobotPositionX[target_index];
                cout << ", " << RobotPositionY[target_index] << "] now only has " << RobotLives[target_index] << " lives!\n";
                outputFile << RobotType[target_index] << " " << RobotNames[target_index] << " positioned at [" << RobotPositionX[target_index];
                outputFile << ", " << RobotPositionY[target_index] << "] now only has " << RobotLives[target_index] << " lives!\n";
            }


            cout << RobotType[target_index] << " " << RobotNames[target_index]
                 << " was hit " << hitCount << " times and now only has " << RobotLives[target_index] << " lives!\n";
            outputFile << RobotType[target_index] << " " << RobotNames[target_index]
                       << " was hit " << hitCount << " times and now only has " << RobotLives[target_index] << " lives!\n";

            how_many_lives = RobotLives[target_index]; // for respawn check
        }
        else {
            cout << "No robot was found at [" << x << ", " << y << "]!\n";
            outputFile << "No robot was found at [" << x << ", " << y << "]!\n";
        }

        cout << type << " " << name << " has " << shells <<  " shells left!\n";
        outputFile << type << " " << name << " has " << shells <<  " shells left!\n";
    }
};

class ThirtyShotBot: public GenericRobot{
    public:
    ThirtyShotBot(string t, string n, int x, int y, int h, int s, int r) : GenericRobot(t, n, x, y, h, s, r) {
        cout << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] with 30 shells is ready!\n\n";
        outputFile << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] with 30 shells is ready!\n\n";
    }
};

class PreciseShotBot : public GenericRobot {
public:
    PreciseShotBot(string t, string n, int x, int y, int h, int s, int r) : GenericRobot(t, n, x, y, h, s, r) {
        cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] can shoot with perfect aim!\n";
        outputFile << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] can shoot with perfect aim!\n";
    }

    void fire(int x, int y, int index) override{
        cout << type << " " << name << " fires at position [" << x << ", " << y << "]!\n";
        outputFile << type << " " << name << " fires at position [" << x << ", " << y << "]!\n";

        // Decrease shells by 1 // picheolin
        RobotShells[index] -= 1;

        // Check which robot got hit
        which_robot = findInListIndex(RobotPositionX,RobotPositionY,x,y);
        if(which_robot > -1){
            if (RobotIsHiding[which_robot]) {
                cout << "...But " << RobotNames[which_robot] << " is HIDING and takes NO DAMAGE!\n";
                outputFile << "...But " << RobotNames[which_robot] << " is HIDING and takes NO DAMAGE!\n";
                return;
            }
            RobotLives[which_robot] = RobotLives[which_robot] - 1;
            if(RobotLives[which_robot] == 1){
                cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                cout << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " life!\n";
                outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                outputFile << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " life!\n";
            }
            else if(RobotLives[which_robot] == 0){
                cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                cout << ", " << RobotPositionY[which_robot] << "] is now DESTROYED!\n";
                outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                outputFile << ", " << RobotPositionY[which_robot] << "] is now DESTROYED!\n";

                RobotUpgrade[index] = true;
            }
            else if(RobotLives[which_robot] > 1){
                cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                cout << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " lives!\n";
                outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                outputFile << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " lives!\n";
            }
            how_many_lives = RobotLives[which_robot];
        }
        else{
            cout << "...But " << type << " " << name << " completely misses!\n";
            outputFile << "...But " << type << " " << name << " completely misses!\n";
        }
    }
};

// Seeing Bots
class ScoutBot: public GenericRobot{
    public:

    int look_uses_left = 3;

    ScoutBot(string t, string n, int x, int y, int h, int s, int r) : GenericRobot(t, n, x, y, h, s, r) {
        cout << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] has been deployed with 3 whole-battlefield scan chances! \n\n";
        outputFile << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] has been deployed with 3 whole-battlefield scan chances! \n\n";
    }

    void look(int x, int y, int index) override {
        if (look_uses_left > 0) {
            look_uses_left--;
            if(look_uses_left != 1){
                cout << "\n" << type << " " << name << " is scanning the entire battlefield! (" << look_uses_left << " chances left)\n";
                outputFile << "\n" << type << " " << name << " is scanning the entire battlefield! (" << look_uses_left << " chances left)\n";
            }
            else{
                cout << "\n" << type << " " << name << " is scanning the entire battlefield! (" << look_uses_left << " chance left)\n";
                outputFile << "\n" << type << " " << name << " is scanning the entire battlefield! (" << look_uses_left << " chance left)\n";
            }

            bool found = false;
            for (int i = 0; i < RobotNames.size(); ++i) {
                if (i != index && RobotAlive[i]) {
                    cout << RobotType[i] << " " << RobotNames[i]
                         << " found at [" << RobotPositionX[i] << ", "
                         << RobotPositionY[i] << "] with "
                         << RobotLives[i] << " lives.\n";
                    outputFile << RobotType[i] << " " << RobotNames[i]
                               << " found at [" << RobotPositionX[i] << ", "
                               << RobotPositionY[i] << "] with "
                               << RobotLives[i] << " lives.\n";
                    found = true;
                }
            }

            if(found == true){
                cout << "\n" << type << " " << name << " will be searching for the closest robot to attack...\n";
                outputFile << "\n" << type << " " << name << " will be searching for the closest robot to attack...\n";
                no_target = true;
                move_check = true;
                for (int i = 0; i < RobotNames.size(); ++i) {
                    if (i != index && RobotAlive[i]) {
                        // Checking a 3x3 area!
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,RobotPositionX[i], RobotPositionY[i]);

                        if(robot_check == true){
                            if(robo_position_x <= RobotPositionX[i] + 2 && robo_position_x >= RobotPositionX[i] - 2 && robo_position_y <= RobotPositionY[i] + 2 && robo_position_y >= RobotPositionY[i] - 2){
                                cout << "\n" << type << " " << name << " decides to target " << RobotType[i] << " " << RobotNames[i] << "!\n";
                                outputFile << "\n" << type << " " << name << " decides to target " << RobotType[i] << " " << RobotNames[i] << "!\n";
                                // Do this if the robot's target is within range of its shooting
                                if(robo_position_x == RobotPositionX[i] - 1 || robo_position_x == RobotPositionX[i] || robo_position_x == RobotPositionX[i] + 1){
                                    if(robo_position_y == RobotPositionY[i] - 1 || robo_position_y == RobotPositionY[i] || robo_position_y == RobotPositionY[i] + 1){
                                        move_check = false;
                                        no_target = false;
                                        fire(RobotPositionX[i], RobotPositionY[i], index);
                                        move_to(RobotPositionX[i], RobotPositionY[i], index);
                                    }
                                }
                                // Do this if the robot needs to move to get to its target, and then shoot
                                if(move_check == true){
                                    if(robo_position_x == RobotPositionX[i] - 2 || robo_position_x == RobotPositionX[i] - 1 || robo_position_x == RobotPositionX[i] || robo_position_x == RobotPositionX[i] + 1 || robo_position_x == RobotPositionX[i] + 2){
                                        if(robo_position_y == RobotPositionY[i] - 2 || robo_position_y == RobotPositionY[i] - 1 || robo_position_y == RobotPositionY[i] || robo_position_y == RobotPositionY[i] + 1 || robo_position_y == RobotPositionY[i] + 2){
                                            no_target = false;
                                            move_to(RobotPositionX[i],RobotPositionY[i], index);
                                            if(robot_blocked == false)
                                                fire(RobotPositionX[i],RobotPositionY[i], index);
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                // No robots detected
                if(no_target == true){
                    cout << type << " " << name << " could not find any robots...\n";
                    outputFile << type << " " << name << " could not find any robots...\n";
                    no_target = true;
                    move_to(robo_position_x, robo_position_y, index);
                }
            }
            else {
                cout << "\n" << type << " " << name << "has detected no other robots on the battlefield.\n";
                outputFile << "\n" << type << " " << name << "has detected no other robots on the battlefield.\n";
            }
            // Ramblings of a sleep-deprived Alesha: PLEASEE don't just use look() it'll just use THIS... THIS CLASS' LOOK()!
            // Next time please just copy-paste the look() from GenericBot... I had to fix this sob.
        }
        else {
            cout << type << " " << name << " has no battlefield scans left. Performing normal look.\n";
            outputFile << type << " " << name << " has no battlefield scans left. Performing normal look.\n";
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

            cout << type << " " << name << " located at [" << robo_position_x << ", " << robo_position_y << "] will be looking at [" << rand_x << ", " << rand_y << "].\n";
            outputFile << type << " " << name << " located at [" << robo_position_x << ", " << robo_position_y << "] will be looking at [" << rand_x << ", " << rand_y << "].\n";

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
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " has been revealed at [" << after_rand_x << ", " << after_rand_y << "]!\n";
                    outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " has been revealed at [" << after_rand_x << ", " << after_rand_y << "]!\n";
                    *after_rand_x_ptr = after_rand_x;
                    *after_rand_y_ptr = after_rand_y;
                    // Do this if the robot's target is within range of its shooting
                    if(robo_position_x == after_rand_x - 1 || robo_position_x == after_rand_x || robo_position_x == after_rand_x + 1){
                        if(robo_position_y == after_rand_y - 1 || robo_position_y == after_rand_y || robo_position_y == after_rand_y + 1){
                            move_check = false;
                            no_target = false;
                            fire(after_rand_x, after_rand_y, index);
                            move_to(after_rand_x, after_rand_y, index);
                        }
                    }
                    // Do this if the robot needs to move to get to its target, and then shoot
                    if(move_check == true){
                        if(robo_position_x == after_rand_x - 2 || robo_position_x == after_rand_x - 1 || robo_position_x == after_rand_x || robo_position_x == after_rand_x + 1 || robo_position_x == after_rand_x + 2){
                            if(robo_position_y == after_rand_y - 2 || robo_position_y == after_rand_y - 1 || robo_position_y == after_rand_y || robo_position_y == after_rand_y + 1 || robo_position_y == after_rand_y + 2){
                                no_target = false;
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
                cout << type << " " << name << " could not find any robots...\n";
                outputFile << type << " " << name << " could not find any robots...\n";
                no_target = true;
                move_to(robo_position_x, robo_position_y, index);
            }
        }
    }
};

// Alesha: Sigh...
// Wrong use of the move_to() function, fixed it
class TrackBot : public GenericRobot {
private:
    int trackers_left = 3;
    vector<int> tracked_indices;  // stores indices of robots being tracked

public:
    TrackBot(string t, string n, int x, int y, int h, int s, int r) : GenericRobot(t, n, x, y, h, s, r) {
        cout << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] activated with target-seeking movement!\n\n" << endl;
        outputFile << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] activated with target-seeking movement!\n\n" << endl;
    }

    void look(int x, int y, int index) override {
        move_check = true;

        // First, try to find new robots to track if trackers are left
        if (trackers_left > 0) {
            for (int i = 0; i < RobotPositionX.size(); ++i) {
                if (i == index || !RobotAlive[i]) continue;

                // Check if already tracked
                if (find(tracked_indices.begin(), tracked_indices.end(), i) != tracked_indices.end())
                    continue;

                // Can "see" any robot in 3x3 radius (adjust this if you have proper line-of-sight logic)
                int dx = abs(robo_position_x - RobotPositionX[i]);
                int dy = abs(robo_position_y - RobotPositionY[i]);

                if (dx <= 3 && dy <= 3) {
                    tracked_indices.push_back(i);
                    trackers_left--;
                    break; // Only plant 1 tracker per turn
                }
            }

            // Try to follow the first alive robot being tracked
            for (int tracked : tracked_indices) {
                if (!RobotAlive[tracked]) continue;

                int target_x = RobotPositionX[tracked];
                int target_y = RobotPositionY[tracked];

                if(trackers_left != 1){
                    cout << type << " " << name << " planted a tracker on " << RobotNames[tracked]
                             << " (" << trackers_left << " trackers left)\n";
                    cout << type << " " << name << " is tracking " << RobotNames[tracked]
                         << " at [" << target_x << ", " << target_y << "]\n";
                    outputFile << type << " " << name << " planted a tracker on " << RobotNames[tracked]
                               << " (" << trackers_left << " trackers left)\n";
                    outputFile << type << " " << name << " is tracking " << RobotNames[tracked]
                               << " at [" << target_x << ", " << target_y << "]\n";
                }
                else{
                    cout << type << " " << name << " planted a tracker on " << RobotNames[tracked]
                             << " (" << trackers_left << " tracker left)\n";
                    cout << type << " " << name << " is tracking " << RobotNames[tracked]
                         << " at [" << target_x << ", " << target_y << "]\n";
                    outputFile << type << " " << name << " planted a tracker on " << RobotNames[tracked]
                               << " (" << trackers_left << " tracker left)\n";
                    outputFile << type << " " << name << " is tracking " << RobotNames[tracked]
                               << " at [" << target_x << ", " << target_y << "]\n";
                }

                no_target = false;

                if(robo_position_x == RobotPositionX[tracked] - 1 || robo_position_x == RobotPositionX[tracked] || robo_position_x == RobotPositionX[tracked] + 1){
                    if(robo_position_y == RobotPositionY[tracked] - 1 || robo_position_y == RobotPositionY[tracked] || robo_position_y == RobotPositionY[tracked] + 1){
                        move_check = false;
                        fire(target_x, target_y, index);
                        move_to(robo_position_x,robo_position_y,index);
                        return;
                    }
                }
                if(robo_position_x != RobotPositionX[tracked] - 1 || robo_position_x != RobotPositionX[tracked] || robo_position_x != RobotPositionX[tracked] + 1){
                    if(robo_position_y == RobotPositionY[tracked] - 1 || robo_position_y == RobotPositionY[tracked] || robo_position_y == RobotPositionY[tracked] + 1){
                        move_check = true;
                        move_to(target_x,target_y,index);
                        if(!robot_blocked)
                            fire(target_x, target_y, index);
                        return;
                    }
                }
                if(robo_position_x == RobotPositionX[tracked] - 1 || robo_position_x == RobotPositionX[tracked] || robo_position_x == RobotPositionX[tracked] + 1){
                    if(robo_position_y != RobotPositionY[tracked] - 1 || robo_position_y != RobotPositionY[tracked] || robo_position_y != RobotPositionY[tracked] + 1){
                        move_check = true;
                        move_to(target_x,target_y,index);
                        if(!robot_blocked)
                            fire(target_x, target_y, index);
                        return;
                    }
                }
                if(robo_position_x != RobotPositionX[tracked] - 1 || robo_position_x != RobotPositionX[tracked] || robo_position_x != RobotPositionX[tracked] + 1){
                    if(robo_position_y != RobotPositionY[tracked] - 1 || robo_position_y != RobotPositionY[tracked] || robo_position_y != RobotPositionY[tracked] + 1){
                        move_check = true;
                        move_to(target_x,target_y,index);
                        if(!robot_blocked)
                            fire(target_x, target_y, index);
                        return;
                    }
                }
            }

            // If nothing to track or fire, just look around
            cout << type << " " << name << " finds no tracked target! It freezes in place as it reflects upon its own loneliness.\n";
            outputFile << type << " " << name << " finds no tracked target! It freezes in place as it reflects upon its own loneliness.\n";

        }
        else{
            // Revert back to normal
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

            cout << type << " " << name << " located at [" << robo_position_x << ", " << robo_position_y << "] will be looking at [" << rand_x << ", " << rand_y << "].\n";
            outputFile << type << " " << name << " located at [" << robo_position_x << ", " << robo_position_y << "] will be looking at [" << rand_x << ", " << rand_y << "].\n";

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
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " has been revealed at [" << after_rand_x << ", " << after_rand_y << "]!\n";
                    outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " has been revealed at [" << after_rand_x << ", " << after_rand_y << "]!\n";
                    *after_rand_x_ptr = after_rand_x;
                    *after_rand_y_ptr = after_rand_y;
                    // Do this if the robot's target is within range of its shooting
                    if(robo_position_x == after_rand_x - 1 || robo_position_x == after_rand_x || robo_position_x == after_rand_x + 1){
                        if(robo_position_y == after_rand_y - 1 || robo_position_y == after_rand_y || robo_position_y == after_rand_y + 1){
                            move_check = false;
                            no_target = false;
                            fire(after_rand_x, after_rand_y, index);
                            move_to(after_rand_x, after_rand_y, index);
                        }
                    }
                    // Do this if the robot needs to move to get to its target, and then shoot
                    if(move_check == true){
                        if(robo_position_x == after_rand_x - 2 || robo_position_x == after_rand_x - 1 || robo_position_x == after_rand_x || robo_position_x == after_rand_x + 1 || robo_position_x == after_rand_x + 2){
                            if(robo_position_y == after_rand_y - 2 || robo_position_y == after_rand_y - 1 || robo_position_y == after_rand_y || robo_position_y == after_rand_y + 1 || robo_position_y == after_rand_y + 2){
                                no_target = false;
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
                cout << type << " " << name << " could not find any robots...\n";
                outputFile << type << " " << name << " could not find any robots...\n";
                no_target = true;
                move_to(robo_position_x, robo_position_y, index);
            }
        }
    }

    void move_to(int x, int y, int index) override {
        if(move_check == true && trackers_left > 0 && no_target == false){
            bool stop = false;
            while(stop == false){
                int move_x = robo_position_x;
                int move_y = robo_position_y;

                // Move toward the tracked robot
                if (robo_position_x < x) move_x++;
                else if (robo_position_x > x) move_x--;

                if (robo_position_y < y) move_y++;
                else if (robo_position_y > y) move_y--;

                // Avoid stepping on another robot
                if (!findInListInt(RobotPositionX, RobotPositionY, move_x, move_y)) {
                    RobotPositionX[index] = move_x;
                    RobotPositionY[index] = move_y;
                    robo_position_x = move_x;
                    robo_position_y = move_y;
                    cout << type << " " << name << " moves to [" << robo_position_x << ", " << robo_position_y << "]...\n";
                    outputFile << type << " " << name << " moves to [" << robo_position_x << ", " << robo_position_y << "]...\n";
                    if(robo_position_x == x - 1 || robo_position_x == x || robo_position_x == x + 1){
                           if(robo_position_y == y - 1 || robo_position_y == y || robo_position_y == y + 1){
                                stop = true;
                                robot_blocked = false;
                           }
                    }
                } else {
                    cout << type << " " << name << " tried to move but the space has already been occupied! So, it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
                    outputFile << type << " " << name << " tried to move but the space has already been occupied! So, it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
                    robot_blocked = true;
                    break;
                }
            }
        }
        else if(no_target == true || move_check == false && no_target == false){
            robot_blocked = true;
            for(int e = -1; e <= 1; ++e){
                for(int f = -1; f <= 1; ++f){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+f,robo_position_y+e);
                    if(robot_check == false){
                        if(robo_position_x+f > 0 && robo_position_y+e > 0 && robo_position_x+f < column && robo_position_y+e < row && (robo_position_x == robo_position_x+f && robo_position_y == robo_position_y+e) == false){
                            robot_check = false;
                            robo_position_x += f;
                            robo_position_y += e;
                            RobotPositionX[index] += f;
                            RobotPositionY[index] += e;
                            robot_blocked = false;
                            break;
                        }
                        else{
                            robot_blocked = true;
                            continue;
                        }
                    }
                    else{
                        robot_blocked = true;
                    }
                }
                if(robot_check == false && robot_blocked == false){
                    break;
                }
            }

            if(robot_blocked == false){
            cout << type << " " << name << " moves to position [" << robo_position_x << ", " << robo_position_y << "]!\n";
            outputFile << type << " " << name << " moves to position [" << robo_position_x << ", " << robo_position_y << "]!\n";
            }
            else{
                cout << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
                outputFile << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
            }
            robot_check = true;
        }
        else if(move_check == true && no_target == false && trackers_left <= 0){
            // Top left
            if((robo_position_x == x + 2) && (robo_position_y == y + 2)){
                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y-1);
                if(robot_check == false){
                    robo_position_x -= 1;
                    robo_position_y -= 1;
                    RobotPositionX[index] -= 1;
                    RobotPositionY[index] -= 1;
                }
                else{
                    robot_blocked = true;
                }
            }
            // Top right
            else if((robo_position_x == x - 2) && (robo_position_y == y + 2)){
                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y-1);
                if(robot_check == false){
                    robo_position_x += 1;
                    robo_position_y -= 1;
                    RobotPositionX[index] += 1;
                    RobotPositionY[index] -= 1;
                }
                else{
                    robot_blocked = true;
                }
            }
            // Bottom left
            else if((robo_position_x == x + 2) && (robo_position_y == y - 2)){
                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+1);
                if(robot_check == false){
                    robo_position_x -= 1;
                    robo_position_y += 1;
                    RobotPositionX[index] -= 1;
                    RobotPositionY[index] += 1;
                }
                else{
                    robot_blocked = true;
                }
            }
            // Bottom right
            else if((robo_position_x == x - 2) && (robo_position_y == y - 2)){
                robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+1);
                if(robot_check == false){
                    robo_position_x += 1;
                    robo_position_y += 1;
                    RobotPositionX[index] += 1;
                    RobotPositionY[index] += 1;
                }
                else{
                    robot_blocked = true;
                }
            }
            // Top middle, left
            else if(robo_position_x == x + 1 && robo_position_y == y + 2){
                for(int i = -1; i <= 0; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                    if(robot_check == false){
                        robo_position_x += i;
                        RobotPositionX[index] += i;
                        robo_position_y -= 1;
                        RobotPositionY[index] -= 1;
                        robot_blocked = false;
                        break;
                    }
                    else{
                        robot_blocked = true;
                    }
                }
            }
            // Top middle, middle
            else if(robo_position_x == x && robo_position_y == y + 2){
                if(robo_position_x == 1){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y -= 1;
                            RobotPositionY[index] -= 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                            robot_blocked = true;
                        }
                    }
                }
                else if(robo_position_x == column){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y -= 1;
                            RobotPositionY[index] -= 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else{
                    for(int i = -1; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y -= 1;
                            RobotPositionY[index] -= 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
            }
            // Top middle, right
            else if(robo_position_x == x - 1 && robo_position_y == y + 2){
                for(int i = 0; i <= 1; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y-1);
                    if(robot_check == false){
                        robo_position_x += i;
                        RobotPositionX[index] += i;
                        robo_position_y -= 1;
                        RobotPositionY[index] -= 1;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Leftmost, top
            else if(robo_position_x == x + 2 && robo_position_y == y + 1){
                for(int i = -1; i <= 0; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                    if(robot_check == false){
                        robo_position_x -= 1;
                        RobotPositionX[index] -= 1;
                        robo_position_y += i;
                        RobotPositionY[index] += i;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Leftmost, middle
            else if(robo_position_x == x + 2 && robo_position_y == y){
                if(robo_position_y == 1){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x -= 1;
                            RobotPositionX[index] -= 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                            robot_blocked = true;
                        }
                    }
                }
                else if(robo_position_y == row){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x -= 1;
                            RobotPositionX[index] -= 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else{
                    for(int i = -1; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x -= 1;
                            RobotPositionX[index] -= 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
            }
            // Leftmost, bottom
            else if(robo_position_x == x + 2 && robo_position_y == y - 1){
                for(int i = 0; i <= 1; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x-1,robo_position_y+i);
                    if(robot_check == false){
                        robo_position_x -= 1;
                        RobotPositionX[index] -= 1;
                        robo_position_y += i;
                        RobotPositionY[index] += i;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Rightmost, top
            else if(robo_position_x == x - 2 && robo_position_y == y + 1){
                for(int i = -1; i <= 0; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                    if(robot_check == false){
                        robo_position_x += 1;
                        RobotPositionX[index] += 1;
                        robo_position_y += i;
                        RobotPositionY[index] += i;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Rightmost, middle
            else if(robo_position_x == x - 2 && robo_position_y == y){
                if(robo_position_y == 1){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x += 1;
                            RobotPositionX[index] += 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else if(robo_position_y == row){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x += 1;
                            RobotPositionX[index] += 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else{
                    for(int i = -1; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                        if(robot_check == false){
                            robo_position_x += 1;
                            RobotPositionX[index] += 1;
                            robo_position_y += i;
                            RobotPositionY[index] += i;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
            }
            // Rightmost, bottom
            else if(robo_position_x == x - 2 && robo_position_y == y - 1){
                for(int i = 0; i <= 1; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+1,robo_position_y+i);
                    if(robot_check == false){
                        robo_position_x += 1;
                        RobotPositionX[index] += 1;
                        robo_position_y += i;
                        RobotPositionY[index] += i;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Bottom middle, left
            else if(robo_position_x == x + 1 && robo_position_y == y - 2){
                for(int i = -1; i <= 0; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                    if(robot_check == false){
                        robo_position_x += i;
                        RobotPositionX[index] += i;
                        robo_position_y += 1;
                        RobotPositionY[index] += 1;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }
            // Bottom middle, middle
            else if(robo_position_x == x && robo_position_y == y - 2){
                if(robo_position_x == 1){
                    for(int i = 0; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y += 1;
                            RobotPositionY[index] += 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else if(robo_position_x == column){
                    for(int i = -1; i <= 0; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y += 1;
                            RobotPositionY[index] += 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
                else{
                    for(int i = -1; i <= 1; ++i){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                        if(robot_check == false){
                            robo_position_x += i;
                            RobotPositionX[index] += i;
                            robo_position_y += 1;
                            RobotPositionY[index] += 1;
                            robot_blocked = false;
                            break;
                        }
                        else{
                        robot_blocked = true;
                        }
                    }
                }
            }
            // Bottom middle, right
            else if(robo_position_x == x - 1 && robo_position_y == y - 2){
                for(int i = 0; i <= 1; ++i){
                    robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+i,robo_position_y+1);
                    if(robot_check == false){
                        robo_position_x += i;
                        RobotPositionX[index] += i;
                        robo_position_y += 1;
                        RobotPositionY[index] += 1;
                        robot_blocked = false;
                        break;
                    }
                    else{
                    robot_blocked = true;
                    }
                }
            }

            if(robot_blocked == false){
            cout << type << " " << name << " moves to position [" << robo_position_x << ", " << robo_position_y << "]!\n";
            outputFile << type << " " << name << " moves to position [" << robo_position_x << ", " << robo_position_y << "]!\n";
            }
            else{
                cout << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
                outputFile << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
            }
            robot_check = true;
        }
    }
};

class EagleEyeBot : public GenericRobot {
public:
    // Constructor
    EagleEyeBot(string t, string n, int x, int y, int h, int s, int r) : GenericRobot(t, n, x, y, h, s, r) {
        cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] can LOOK at an extended 5x5 area!\n\n";
        outputFile << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] can LOOK at an extended 5x5 area!\n\n";
    }

    // Override look() with custom behavior
    void look(int x, int y, int index) override {
        cout << type << " " << name << " scans the battlefield with eagle-eyes!\n";
        outputFile << type << " " << name << " scans the battlefield with eagle-eyes!\n";

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

        cout << type << " " << name << " located at [" << robo_position_x << ", " << robo_position_y << "] will be looking at [" << rand_x << ", " << rand_y << "].\n";
        outputFile << type << " " << name << " located at [" << robo_position_x << ", " << robo_position_y << "] will be looking at [" << rand_x << ", " << rand_y << "].\n";

        for(int e = -2; e <= 2; ++e){
            for(int f = -2; f <= 2; ++f){
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
                cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " has been revealed at [" << after_rand_x << ", " << after_rand_y << "]!\n";
                outputFile << RobotType[which_robot] << " " << RobotNames[which_robot] << " has been revealed at [" << after_rand_x << ", " << after_rand_y << "]!\n";
                *after_rand_x_ptr = after_rand_x;
                *after_rand_y_ptr = after_rand_y;
                // Do this if the robot's target is within range of its shooting
                if(robo_position_x == after_rand_x - 1 || robo_position_x == after_rand_x || robo_position_x == after_rand_x + 1){
                    if(robo_position_y == after_rand_y - 1 || robo_position_y == after_rand_y || robo_position_y == after_rand_y + 1){
                        move_check = false;
                        no_target = false;
                        fire(after_rand_x, after_rand_y, index);
                        move_to(after_rand_x, after_rand_y, index);
                    }
                }
                // Do this if the robot needs to move to get to its target, and then shoot
                if(move_check == true){
                    if(robo_position_x == after_rand_x - 2 || robo_position_x == after_rand_x - 1 || robo_position_x == after_rand_x || robo_position_x == after_rand_x + 1 || robo_position_x == after_rand_x + 2){
                        if(robo_position_y == after_rand_y - 2 || robo_position_y == after_rand_y - 1 || robo_position_y == after_rand_y || robo_position_y == after_rand_y + 1 || robo_position_y == after_rand_y + 2){
                            no_target = false;
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
            cout << type << " " << name << " could not find any robots...\n";
            outputFile << type << " " << name << " could not find any robots...\n";
            no_target = true;
            move_to(robo_position_x, robo_position_y, index);
        }
    }
};

#endif
