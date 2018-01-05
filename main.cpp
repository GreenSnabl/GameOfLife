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

#include "GameOfLife.h"
#include "Bitmap.h"
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
        GameOfLife game(argv[1], height, width, scaling);
        if (argv[4] != nullptr) game.set_mode(stoi(argv[4]));
        game.simulateCurses();
    }

  
     //GameOfLife game01(32, 32, "GameOfLife00000.bmp", 20);
     //game01.simulateConsole(); 
     
    return 0;

}