/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameCurses.h
 * Author: dennis
 *
 * Created on January 5, 2018, 11:56 PM
 */

#ifndef GAMECURSES_H
#define GAMECURSES_H
#include "GameOfLife.h"



class GameCurses{
public:
    GameCurses();
    GameCurses(int** arr, int height, int width);
    GameCurses(const std::string &fname, int height, int width, int scaling);
    ~GameCurses();
    
    void play();
    void set_mode(int index);

private:
    void control(int key);
    void printFrame();
    void cycle_mode_up();
    void cycle_mode_down();
    void speed_up();
    void speed_down();
    const std::string get_mode() const;
    
    int speed;
    int mode;
    GameOfLife game;
};






#endif /* GAMECURSES_H */

