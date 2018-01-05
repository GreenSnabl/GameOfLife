/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameOfLife.h
 * Author: dennis
 *
 * Created on December 24, 2017, 4:34 PM
 */

#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <string>

class GameOfLife {
public:
    GameOfLife();
    GameOfLife(const std::string &fname, int height, int width, int img_scaling);
    GameOfLife(int** arr, int width, int height);
    ~GameOfLife();

    void simulateCurses();
    void simulateConsole();

    void set_mode(int index);


private:

    void newGeneration();
    void showWorldConsole();
    void showWorldNCurses();
    bool checkNeighbors(int col, int row, bool populated);


    bool torus;
    bool edges;

    int m_width;
    int m_height;
    int** m_data;
    int** m_data_new;
};


int low_border(int index);
int high_border(int index, int max);
int torus_border(int index, int max);
void loesung();




#endif /* GAMEOFLIFE_H */

