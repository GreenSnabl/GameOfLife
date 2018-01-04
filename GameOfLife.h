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


class GameOfLife{
public:
    GameOfLife();
 //   GameOfLife(int width, int height);
    GameOfLife(int height, int width, const std::string &fname, int img_scaling);
    GameOfLife(int width, int height, int** arr);
    ~GameOfLife();
    
    void simulateCurses();
    void simulateConsole();
    
private:
    
    void newGeneration();
    void showWorldConsole();
    void showWorldNCurses();
    bool checkNeighbors(int col, int row, bool populated);
    
    int** m_data;
    int** m_data_new;
    int m_width;
    int m_height;
};


void loesung();

#endif /* GAMEOFLIFE_H */

