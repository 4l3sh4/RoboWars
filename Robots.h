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
        string name;
        int robo_position_x;
        int robo_position_y;
        int lives;
};

class GenericRobot: private Robot
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
        void RobotReset(){
            lives = 3;
            robo_position_x = -5;
            robo_position_y = -5;
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


        virtual void think(int x, int y, int index) override{
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
                cout << "GenericRobot " << name << " could not find any robots...";
                no_target = true;
                move_to(robo_position_x, robo_position_y, index);
            }
        };

        // Reminder to Alesha: Fix the AI for the move_to() action to avoid possible softlocking.
        virtual void move_to(int x, int y, int index) override{
            robot_blocked = false;

            if(no_target == true){
                for(int e = -1; e <= 1; ++e){
                    for(int f = -1; f <= 1; ++f){
                        robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+f,robo_position_y+e);
                        if(robot_check == false){
                            if(robo_position_x+f > 0 && robo_position_y+e > 0 && (robo_position_x == robo_position_x+f && robo_position_y == robo_position_y+e) == false){
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
            // Alesha to herself: There's GOT to be a better way to code this istg. FIX THIS.
            else if(move_check == false && no_target == false){
                if(robo_position_x>1 && robo_position_y>1){
                    for(int e = -1; e <= 1; ++e){
                        for(int f = -1; f <= 1; ++f){
                            // Checking a 3x3 area!
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+f, robo_position_y+e);

                            if(robot_check == false){
                                robo_position_x = robo_position_x + f;
                                robo_position_y = robo_position_y + e;
                                RobotPositionX[index] = robo_position_x;
                                RobotPositionY[index] = robo_position_y;
                                robot_blocked = false;
                                break;
                            }
                            else{
                                robot_blocked = true;
                            }
                        }
                        if(robot_check == false){
                            break;
                        }
                    }
                }
                else if(robo_position_x<=1 && robo_position_y>1){
                    for(int e = -1; e <= 0; ++e){
                        for(int f = 0; f <= 1; ++f){
                            // Checking a 3x3 area!
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+f, robo_position_y+e);

                            if(robot_check == false){
                                robo_position_x = robo_position_x + f;
                                robo_position_y = robo_position_y + e;
                                RobotPositionX[index] = robo_position_x;
                                RobotPositionY[index] = robo_position_y;
                                robot_blocked = false;
                                break;
                            }
                            else{
                                robot_blocked = true;
                            }
                        }
                        if(robot_check == false){
                            break;
                        }
                    }
                }
                else if(robo_position_x>1 && robo_position_y<=1){
                    for(int e = 0; e <= 1; ++e){
                        for(int f = -1; f <= 1; ++f){
                            // Checking a 3x3 area!
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+f, robo_position_y+e);

                            if(robot_check == false){
                                robo_position_x = robo_position_x + f;
                                robo_position_y = robo_position_y + e;
                                RobotPositionX[index] = robo_position_x;
                                RobotPositionY[index] = robo_position_y;
                                robot_blocked = false;
                                break;
                            }
                            else{
                                robot_blocked = true;
                            }
                        }
                        if(robot_check == false){
                            break;
                        }
                    }
                }
                else if(robo_position_x<= 1 && robo_position_y<=1){
                    for(int e = 0; e <= 1; ++e){
                        for(int f = 0; f <= 1; ++f){
                            // Checking a 3x3 area!
                            robot_check = findInListInt(RobotPositionX,RobotPositionY,robo_position_x+f, robo_position_y+e);

                            if(robot_check == false){
                                robo_position_x = robo_position_x + f;
                                robo_position_y = robo_position_y + e;
                                RobotPositionX[index] = robo_position_x;
                                RobotPositionY[index] = robo_position_y;
                                robot_blocked = false;
                                break;
                            }
                            else{
                                robot_blocked = true;
                            }
                        }
                        if(robot_check == false){
                            break;
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
                cout << "\nGenericRobot " << name << " moves to position " << robo_position_x << ", " << robo_position_y << "!";
            }
            else{
                cout << "\nGenericRobot " << name << " is blocked by another robot, so it stays at position " << robo_position_x << ", " << robo_position_y << ".";
            }
            robot_check = true;
        };

        virtual void fire(int x, int y, int index) override{
            cout << "\nGenericRobot " << name << " fires at position " << x << ", " << y << "!";

            rand_num_fire = (rand() % 100);

            // Check which robot got hit
            which_robot = findInListIndex(RobotPositionX,RobotPositionY,x,y);
            if(which_robot > -1 && rand_num_fire <= 70){
                RobotLives[which_robot] = RobotLives[which_robot] - 1;
                if(RobotLives[which_robot] == 1){
                    cout << "\n" << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at " << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << " now only has " << RobotLives[which_robot] << " life!";
                }
                else if(RobotLives[which_robot] == 0){
                    cout << "\n" << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at " << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << " is now DESTROYED!";
                }
                else if(RobotLives[which_robot] > 1){
                    cout << "\n" << RobotType[which_robot] << " " << RobotNames[which_robot] << " positioned at " << RobotPositionX[which_robot];
                    cout << ", " << RobotPositionY[which_robot] << " now only has " << RobotLives[which_robot] << " lives!";
                }
                how_many_lives = RobotLives[which_robot];
            }
            else if(which_robot > -1 && rand_num_fire > 70){
                cout << "\n...But GenericBot " << name << " completely misses!";
            }
        };

};

#endif
