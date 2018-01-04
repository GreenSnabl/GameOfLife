/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameOfLife.cpp
 * Author: dennis
 * 
 * Created on December 24, 2017, 4:34 PM
 */

#include "GameOfLife.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <curses.h>
#include "Bitmap.h"

using std::cout;
using std::left;
using std::setw;



GameOfLife::GameOfLife(int width, int height, int** arr) : m_width{width}, m_height{height}
{
    m_data = new int*[m_height];
    m_data_new = new int*[m_height];
    for (int i = 0; i < m_height; ++i) {
        m_data[i] = new int[m_width];
        m_data_new[i] = new int[m_width];
    }
    for (int i = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j)
            m_data[i][j] = m_data_new[i][j] = arr[i][j];
}

GameOfLife::GameOfLife(int height, int width, const std::string& fname, int img_scaling) : m_width{width}, m_height{height}
{
    GreyImage img(m_width, m_height);
    img.load(fname);

    m_data = new int*[m_height];
    m_data_new = new int*[m_height];
    for (int i = 0; i < m_height; ++i) {
        m_data[i] = new int[m_width];
        m_data_new[i] = new int[m_width];
    }
    for (int i = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j)
            if (static_cast<int> (img.getPixel(j * img_scaling, i * img_scaling).value) < 128) m_data[i][j] = m_data_new[i][j] = 1;
            else m_data[i][j] = m_data_new[i][j] = 0;
}

GameOfLife::~GameOfLife() {
    for (int i = 0; i < m_height; ++i) {
        delete[] m_data[i];
        delete[] m_data_new[i];
    }
    delete[] m_data;
    delete[] m_data_new;
}

bool GameOfLife::checkNeighbors(int col, int row, bool populated) {
    int cnt = 0;
    if (populated) --cnt;
    for (int i = col - 1; i <= col + 1; ++i) {
        for (int j = row - 1; j <= row + 1; ++j)
            if (m_data[i][j] == 1) ++cnt;
    }
    if (populated && (cnt == 2 || cnt == 3)) return true;
    if (!populated && cnt == 3) return true;
    return false;
}

void GameOfLife::newGeneration() {
    // In this version fields on edges will not be checked

    for (int i = 1; i < m_height - 1; ++i) // since there are only 5 neighbors instead of 8
        for (int j = 1; j < m_width - 1; ++j) {
            if (checkNeighbors(i, j, (m_data[i][j] == 1))) m_data_new[i][j] = 1;
            else m_data_new[i][j] = 0;
        }
    for (int i = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j)
            m_data[i][j] = m_data_new[i][j];
}

/*

void GameOfLife::newGeneration() {
    int new_world[m_height][m_width] = {0};                 // In this version fields on edges will not be checked
    
    for (int i = 1; i < m_height - 1; ++i)                  // since there are only 5 neighbors instead of 8
        for (int j = 1; j < m_width - 1; ++j) {
            if (checkNeighbors(i, j, (m_data[i][j] == 1)))
                new_world[i][j] = 1;
        }
    for (int i = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j)
            m_data[i][j] = new_world[i][j];
}
 */
void GameOfLife::showWorldConsole() {
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if (m_data[i][j] == 1) cout << setw(2) << left << "o";
            else cout << setw(2) << left << ".";
        }
        cout << "\n";
    }
    cout << "\n";
}

void GameOfLife::simulateConsole() {

    showWorldConsole();
    int i = 0;
    while (i < 140) {
        usleep(150000);
        newGeneration();
        showWorldConsole();
        ++i;
    }
}

void GameOfLife::showWorldNCurses() {
    
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    for (int i = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j) {
            if (m_data[i][j] == 0) {
                attron(COLOR_PAIR(1));
                mvaddch(i + 2, j + 2, ' ');
                attron(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                mvaddch(i + 2, j + 2, ' ');
                attroff(COLOR_PAIR(2));
            }
        }
    attron(COLOR_PAIR(2));
    mvaddstr(m_height + 2, 2, "Press ENTER to quit");
    attroff(COLOR_PAIR(2));

}

void GameOfLife::simulateCurses() {
    
    int key = 0;
    const int ENTER = 10;
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    start_color();
    showWorldNCurses();

    while (key != ENTER) {
        usleep(150000);
        newGeneration();
        showWorldNCurses();
        refresh();
        timeout(0);
        key = getch();
    }

    endwin();
}

void loesung() {
    int arr[32][32] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
        {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0},
        {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    int** arr2 = new int*[32];
    for (int i = 0; i < 32; ++i)
        arr2[i] = new int[32];

    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            arr2[i][j] = arr[i][j];

    GameOfLife g(32, 32, arr2);

    g.simulateCurses();

    GameOfLife g2(32, 32, arr2);
    g2.simulateConsole();

    for (int i = 0; i < 32; ++i)
        delete[] arr2[i];
    delete[] arr2;

}










/*GameOfLife::GameOfLife() : m_width{10}, m_height{10}
{
    m_data = new int*[m_height];
    for (int i = 0; i < m_height; ++i)
        m_data[i] = new int[m_width];
} */

/*GameOfLife::GameOfLife(int width, int height) : m_width{width}, m_height{height}
{
    m_data = new int*[m_height];
    for (int i = 0; i < m_height; ++i)
        m_data[i] = new int[m_width];        
        
}*/