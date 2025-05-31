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
};

class GenericRobot: public Robot
{
    public:
        GenericRobot(string t, string n, int x, int y, int h, int s){
            type = t;
            name = n;
            robo_position_x = x;
            robo_position_y = y;
            lives = h;
            shells = s;
        };

        void BattleStart(){
            cout << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] is ready for battle!" << "\n";
        }

        void LoseLives(){
            lives -= 1;
        }

        // Resetting to robot to 3 lives, and letting it be out of the battlefield until the next turn.
        void RobotReset(int index){
            lives = 3;
            robo_position_x = -5;
            robo_position_y = -5;
            RobotRespawn[index] += 1; // Increment respawn count
            shells = 10;
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
            cout << type << " " << name << " has respawned at [" << robo_position_x << ", " << robo_position_y << "]!\n";
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
                selfDestruct(index);
                return; // Stop execution if robot has no shells // picheolin
            }

            if (lives <= 0) {
                if (RobotRespawn[index] < 3) {
                    RobotReset(index); // respawns with 3 lives and moves off grid
                    return;
                } else {
                    cout << type << " " << name << " has no lives left and is now lost in the battlefield.\n";
                    selfDestruct(index);
                    return;
                }
            }

            cout << type << " " << name << " is strategising...\n";
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
            }
            else{
                cout << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
            }
            robot_check = true;
        };

        virtual void fire(int x, int y, int index) override{
            cout << type << " " << name << " fires at position [" << x << ", " << y << "]!\n";

            // Decrease shells by 1 // picheolin
            shells -= 1;
            RobotShells[index] -= 1;

            rand_num_fire = (rand() % 100);

            // Check which robot got hit
            which_robot = findInListIndex(RobotPositionX,RobotPositionY,x,y);
            if(which_robot > -1 && rand_num_fire <= 70){
                if (RobotIsHiding[which_robot]) {
                    cout << "...But " << RobotNames[which_robot] << " is HIDING and takes NO DAMAGE!\n";
                    return;
                }
                RobotLives[which_robot] = RobotLives[which_robot] - 1;
                if(RobotLives[which_robot] == 1){
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " life!\n";
                }
                else if(RobotLives[which_robot] == 0){
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << "] is now DESTROYED!\n";

                    RobotUpgrade[index] = true;
                }
                else if(RobotLives[which_robot] > 1){
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " lives!\n";
                }
                how_many_lives = RobotLives[which_robot];
            }
            else if(which_robot > -1 && rand_num_fire > 70){
                cout << "...But " << type << " " << name << " completely misses!\n";
            }

            cout << type << " " << name << " has " << shells <<  " shells left!\n";
        };

};

// Moving Bots
class HideBot : public GenericRobot {
private:
    int hide_uses = 3;
    bool is_hiding = false;

public:
    HideBot(string t, string n, int x, int y, int h, int s) : GenericRobot(t, n, x, y, h, s) {
        cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] is ready with 3 hiding chances available!\n";
    }

    void think(int x, int y, int index) override {
        if (hide_uses > 0 && (rand() % 3 == 0)) { // 1 in 3 chance to hide to prevent the bot from too overpowered
            is_hiding = true;
            hide_uses--;
            RobotIsHiding[index] = true; // update global status
            cout << type << " " << RobotNames[index] << " is HIDING this turn! (" << hide_uses << " hides left)\n";
        }
        else {
            is_hiding = false;
            RobotIsHiding[index] = false; // not hiding
            cout << type << " " << RobotNames[index] << " is NOT hiding this turn.\n";
        }

        look(x, y, index);
    }
};

class JumpBot : public GenericRobot {
    private:
        int jump_uses = 3;

    public:
        JumpBot(string t, string n, int x, int y, int h, int s) : GenericRobot(t, n, x, y, h, s) {
            cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] initialized with 3 jumps available.\n";
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
                cout << type << " " << RobotNames[index] << " failed to jump — no safe spot found.\n";
                return;
            }

            cout << type << " " << RobotNames[index] << " jumps from [" << RobotPositionX[index] << ", "
                 << RobotPositionY[index] << "] to [" << new_x << ", " << new_y << "]. "
                 << "(" << --jump_uses << " jumps left)\n";
            RobotPositionX[index] = new_x;
            RobotPositionY[index] = new_y;
        }
        else {
            move_to(x, y, index); // Normal move after all jumps used
        }
      }

};

class SprintBot : public GenericRobot {
public:
    SprintBot(string t, string n, int x, int y, int h, int s) : GenericRobot(t, n, x, y, h, s) {
        cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] can move up to two spaces, and jump over robots!\n";
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
        }
        else{
            cout << type << " " << name << " is blocked by another robot, so it stays at position [" << robo_position_x << ", " << robo_position_y << "].\n";
        }
        robot_check = true;

    }
};

// Shooting Bots
class LongShotBot : public GenericRobot {
public:
    LongShotBot(string t, string n, int x, int y, int h, int s) : GenericRobot(t, n, x, y, h, s) {
        cout << "LongShotBot " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] is deployed with long-range attack mode!\n";
    }

    void fire(int x, int y, int attackerIndex) override {
        int dx = abs(RobotPositionX[attackerIndex] - x);
        int dy = abs(RobotPositionY[attackerIndex] - y);
        int manhattan_distance = dx + dy;

        if (manhattan_distance > 5) {
            cout << type << " " << RobotNames[attackerIndex] << " attempted to fire at [" << x << ", " << y << "] but it's out of range [range = " << manhattan_distance << "].\n";
            return;
        }

        cout << type << " " << RobotNames[attackerIndex] << " fires from ["
             << RobotPositionX[attackerIndex] << ", " << RobotPositionY[attackerIndex] << "] to [" << x << ", " << y << "]!\n";

        int target_index = findInListIndex(RobotPositionX, RobotPositionY, x, y);

        if (target_index != -1) {
            if (RobotIsHiding[target_index]) {
                cout << "...But " << RobotNames[target_index] << " is HIDING and takes NO DAMAGE!\n";
                return;
            }

            RobotLives[target_index] -= 1;
            if (RobotLives[target_index] < 0) RobotLives[target_index] = 0;

            cout << RobotType[target_index] << " " << RobotNames[target_index]
                 << " now has " << RobotLives[target_index] << " lives!\n";

            how_many_lives = RobotLives[target_index];
        }
        else {
            cout << "No robot was found at [" << x << ", " << y << "]!\n";
        }
    }

    void think(int x, int y, int index) override {
        cout << type << " " << RobotNames[index] << " is scanning the battlefield...\n";

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
    }
};

class SemiAutoBot : public GenericRobot {
public:
    SemiAutoBot(string t, string n, int x, int y, int h, int s) : GenericRobot(t, n, x, y, h, s) {
        cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] initialized with rapid fire mode!\n";
    }

    void fire(int x, int y, int index) override {
        cout << type << " " << RobotNames[index] << " unleashes rapid fire at [" << x << ", " << y << "]!\n";

        int hitCount = 0;

        for (int shot =1; shot <= 3; ++shot) {
            int chance = rand() % 100; // 0 to 99
            cout << "Shot " << shot << ": ";
            if (chance < 70) {
                cout << "Hit!\n";
                hitCount++;
            }
            else {
                cout << "Miss!\n";
            }
        }

        if (hitCount == 0) {
            cout << "All 3 shots missed!\n";
            return;
        }

        // Check which robot is hit
        int target_index = findInListIndex(RobotPositionX, RobotPositionY, x, y);
        if (target_index != -1) {
            if (RobotIsHiding[target_index]) {
                cout << "But " << RobotNames[target_index] << " is HIDING and takes NO DAMAGE!\n";
                return;
            }

            RobotLives[target_index] -= hitCount;

            if (RobotLives[target_index] < 0) RobotLives[target_index] = 0;

            cout << RobotType[target_index] << " " << RobotNames[target_index]
                 << " was hit " << hitCount << " times and now has " << RobotLives[target_index] << " lives!\n";

            how_many_lives = RobotLives[target_index]; // for respawn check
        }
        else {
            cout << "No robot was found at [" << x << ", " << y << "]!\n";
        }
    }
};

class ThirtyShotBot: public GenericRobot{
    public:
    ThirtyShotBot(string t, string n, int x, int y, int h, int s) : GenericRobot(t, n, x, y, h, s) {
        cout << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] with 30 shells is ready!\n";
    }
};

class PreciseShotBot : public GenericRobot {
public:
    PreciseShotBot(string t, string n, int x, int y, int h, int s) : GenericRobot(t, n, x, y, h, s) {
        cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] can shoot with perfect aim!\n";
    }

    void fire(int x, int y, int index) override{
            cout << type << " " << name << " fires at position [" << x << ", " << y << "]!\n";

            // Decrease shells by 1 // picheolin
            RobotShells[index] -= 1;

            // Check which robot got hit
            which_robot = findInListIndex(RobotPositionX,RobotPositionY,x,y);
            if(which_robot > -1){
                if (RobotIsHiding[which_robot]) {
                    cout << "...But " << RobotNames[which_robot] << " is HIDING and takes NO DAMAGE!\n";
                    return;
                }
                RobotLives[which_robot] = RobotLives[which_robot] - 1;
                if(RobotLives[which_robot] == 1){
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " life!\n";
                }
                else if(RobotLives[which_robot] == 0){
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << "] is now DESTROYED!\n";

                    RobotUpgrade[index] = true;
                }
                else if(RobotLives[which_robot] > 1){
                    cout << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at [" << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << "] now only has " << RobotLives[which_robot] << " lives!\n";
                }
                how_many_lives = RobotLives[which_robot];
            }
            else{
                cout << "...But " << type << " " << name << " completely misses!\n";
            }
        }
};

// Seeing Bots
class ScoutBot: public GenericRobot{
    public:

    int look_uses_left = 3;

    ScoutBot(string t, string n, int x, int y, int h, int s) : GenericRobot(t, n, x, y, h, s) {
        cout << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] has been deployed with 3 whole-battlefield scan chances! \n";
    }

    void look(int x, int y, int index) override {
        if (look_uses_left > 0) {
            cout << "\n" << type << " " << name << " is scanning the entire battlefield!\n";
            look_uses_left--;

            bool found = false;
            for (int i = 0; i < RobotNames.size(); ++i) {
                if (i != index && RobotAlive[i]) {
                    cout << RobotType[i] << " " << RobotNames[i]
                         << " found at [" << RobotPositionX[i] << ", "
                         << RobotPositionY[i] << "] with "
                         << RobotLives[i] << " lives.\n";
                    found = true;
                }
            }

            if (!found) {
                cout << "No other robots detected on the battlefield.\n";
            }

        } else {
            cout << type << " " << name << " has no battlefield scans left. Performing normal look.\n";
            look(x, y, index);  // fallback to base behavior
        }
    }
};

class TrackBot : public GenericRobot {
public:
    TrackBot(string t, string n, int x, int y, int h, int s) : GenericRobot(t, n, x, y, h, s) {
        cout << type << " " << name << " at position [" << robo_position_x << ", " << robo_position_y << "] activated with target-seeking movement!\n";
    }

    void think(int x, int y, int index) override {
        int self_x = RobotPositionX[index];
        int self_y = RobotPositionY[index];
        int closestDist = 1000;
        int target_x = -1, target_y = -1;

        // Find the nearest robot (excluding self)
        for (int i = 0; i < RobotPositionX.size(); ++i) {
            if (i == index || RobotPositionX[i] == -5) continue;

            int dist = abs(self_x - RobotPositionX[i]) + abs(self_y - RobotPositionY[i]);

            // Saves closest robot's position
            if (dist < closestDist) {
                closestDist = dist;
                target_x = RobotPositionX[i];
                target_y = RobotPositionY[i];
            }
        }

        if (target_x == -1 || target_y == -1) {
            cout << type << " " << RobotNames[index] << " found no targets to track.\n";
            return;
        }

        // Move one step toward the target
        int move_x = self_x;
        int move_y = self_y;

        if (self_x < target_x) move_x++;
        else if (self_x > target_x) move_x--;

        if (self_y < target_y) move_y++;
        else if (self_y > target_y) move_y--;

        // Avoid stepping on another robot
        if (!findInListInt(RobotPositionX, RobotPositionY, move_x, move_y)) {
            move_to(move_x, move_y, index);
            RobotPositionX[index] = move_x;
            RobotPositionY[index] = move_y;
        } else {
            cout << type << " " << RobotNames[index] << " tried to move but the position was occupied.\n";
        }

        // After moving, attempt to fire
        look(RobotPositionX[index], RobotPositionY[index], index);
    }
};

class EagleEyeBot : public GenericRobot {
public:
    // Constructor
    EagleEyeBot(string t, string n, int x, int y, int h, int s) : GenericRobot(t, n, x, y, h, s) {
        cout << type << " " << n << " at position [" << robo_position_x << ", " << robo_position_y << "] can LOOK at an extended 5x5 area!\n";
    }

    // Override look() with custom behavior
    void look(int x, int y, int index) override {
        cout << type << " " << name << " scans the battlefield with eagle-eyes!\n";

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
            no_target = true;
            move_to(robo_position_x, robo_position_y, index);
        }
    }
};

#endif
