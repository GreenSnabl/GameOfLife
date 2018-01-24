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
#include "GameCurses.h"
#include <iostream>
#include <iomanip>
#include "Bitmap.h"
#include <unistd.h>

using std::cout;
using std::left;
using std::setw;



GameOfLife::GameOfLife(int** arr, int width, int height) : m_width{width}, m_height{height}, edges{false}, torus{false}, generation_count{0}
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

GameOfLife::GameOfLife(const std::string& fname, int height, int width, int img_scaling) : m_width{width}, m_height{height}, edges{false}, torus{false}, generation_count{0}
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

bool GameOfLife::checkNeighbors(int height, int width, bool populated) {
    int cnt = 0;
    if (populated) --cnt;

    if (torus) {
        for (int i = height - 1; i <= height + 1; ++i)
            for (int j = width - 1; j <= width + 1; ++j)
                if (is_alive(torus_border(i, m_height), torus_border(j, m_width))) ++cnt;

    } else if (edges) {
        for (int i = low_border(height); i <= high_border(height, m_height - 1); ++i) {
            for (int j = low_border(width); j <= high_border(width, m_width - 1); ++j)
                if (is_alive(i, j)) ++cnt;
        }
    } else {
        for (int i = height - 1; i <= height + 1; ++i) {
            for (int j = width - 1; j <= width + 1; ++j)
                if (is_alive(i, j)) ++cnt;
        }
    }
    if (populated && (cnt == 2 || cnt == 3)) return true;
    if (!populated && cnt == 3) return true;
    return false;
}

void GameOfLife::newGeneration() {


    if (!edges) {
        for (int i = 1; i < m_height - 1; ++i)
            for (int j = 1; j < m_width - 1; ++j) {
                if (checkNeighbors(i, j, (m_data[i][j] == 1))) m_data_new[i][j] = 1;
                else m_data_new[i][j] = 0;
            }
    } else {
        for (int i = 0; i < m_height; ++i)
            for (int j = 0; j < m_width; ++j)
                if (checkNeighbors(i, j, (m_data[i][j] == 1))) m_data_new[i][j] = 1;
                else m_data_new[i][j] = 0;
    }
    for (int i = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j)
            m_data[i][j] = m_data_new[i][j];
    ++generation_count;
}

void GameOfLife::set_mode(int index) {
    switch (index) {
        case 1:
        {
            edges = false;
            torus = false;
            break;
        }
        case 2:
        {
            edges = true;
            torus = false;
            break;
        }
        case 3:
        {
            edges = true;
            torus = true;
            break;
        }
    }
}

const int GameOfLife::get_height() const {
    return m_height;
}

const int GameOfLife::get_width() const {
    return m_width;
}

const bool GameOfLife::is_alive(int height, int width) const {
    if (height >= 0 && height < m_height && width >= 0 && width < m_width) return (m_data[height][width] == 1);
}

const int GameOfLife::get_generation_count() const
{
    return generation_count;
}


int low_border(int index) {
    if (index == 0) return 0;
    return index - 1;
}

int high_border(int index, int max) {
    if (index == max) return max;
    return index + 1;
};

int torus_border(int index, int max) {
    //if (index == -1) return max - 1;
    //else if (index == max) return 0;
    //return index;
    
    return (index + max) % max;
};



void print(const GameOfLife& game) {
    for (int i = 0; i < game.get_height(); ++i) {
        for (int j = 0; j < game.get_width(); ++j) {
            if (game.is_alive(i, j)) cout << setw(2) << left << "o";
            else cout << setw(2) << left << ".";
        }
        cout << "\n";
    }
    cout << "\n";
}

void simulate(GameOfLife& game) {

    print(game);
    int i = 0;
    while (i++ < 140) {
        print(game);
        usleep(150000);
        game.newGeneration();
    }
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

    GameOfLife game(arr2, 32, 32);
    GameCurses gameCurses(arr2, 32, 32);
    simulate(game);
    gameCurses.play();
    for (int i = 0; i < 32; ++i)
        delete[] arr2[i];
    delete[] arr2;
}
