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
   
    void set_mode(int index);
    void newGeneration();
    const int get_generation_count() const;
    
    const int get_height() const;
    const int get_width() const;
    const bool is_alive(int height, int width) const;
    
private:
    
    bool checkNeighbors(int col, int row, bool populated);

    bool torus;
    bool edges;
    
    int generation_count = 0;
    int m_width;
    int m_height;
    int** m_data;
    int** m_data_new;
};



int low_border(int index);
int high_border(int index, int max);
int torus_border(int index, int max);

void loesung();
void simulate(GameOfLife& game);
void print(const GameOfLife& game);


#endif /* GAMEOFLIFE_H */

