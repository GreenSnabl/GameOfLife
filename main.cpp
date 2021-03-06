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
    } else if (argc == 4 || argc == 5) {
        int height, width, scaling = 1;
        height = stoi(argv[1]);
        width = stoi(argv[2]);
        if (argc == 5) scaling = stoi(argv[4]);
        GameOfLife game(height, width, argv[3], scaling);
        game.simulateCurses();
    } else {
        cout << "\nUngültige Parameter\n";
    }

  
     //GameOfLife game01(32, 32, "GameOfLife00000.bmp", 20);
     //game01.simulateConsole(); 
     
    return 0;

}