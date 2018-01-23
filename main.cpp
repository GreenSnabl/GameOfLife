/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: dennis
 *
 * Created on January 3, 2018, 8:57 PM
 */
#include "GameCurses.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, char** argv) {
    
        if (argc == 1) {
            loesung();
        } else {
            int height, width, mode = 0, scaling = 1;
            height = stoi(argv[2]);
            width = stoi(argv[3]);
            if (argc == 5) scaling = stoi(argv[4]);
            GameCurses game(argv[1], height, width, scaling);
            game.play();
        }

/*
    loesung();
    GameCurses game01("GameOfLife00000.bmp", 32, 32, 20);
    game01.play();
*/
    return 0;

}