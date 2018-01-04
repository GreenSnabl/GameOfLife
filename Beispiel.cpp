/* 
 * File:   main.cpp
 * Author: Michael Roth
 *
 * Created on 3. September 2014, 07:52
 */

#include <cstdlib>
#include <iostream>
#include "Bitmap.h"
#include "GameOfLife.h"

#include <fstream>

using std::cout;
using std::cin;
using std::endl;



/*
void beispiel
{

    RgbImage r1(300, 300);
    RgbPixel pix(0,200,100);
    RgbPixel pix2(200, 10, 0);
    r1.fill(pix);
    
    for(int i = 0; i < 300; ++i)
        for(int j = 0; j < 300; ++j)
        {
            if(((i + j) % 5 == 0 || (i - j) % 5 == 0) && i * j != 0) r1.setPixel(i, j, pix2);                    
        }
    
    r1.scale(20).store("cool.bmp");
     
    
 
    //RgbImage img1("bsp.bmp");
    //img1.scale(20).store("scaled.bmp");
    
   
	//Einfaches Farbbild mit ein paar Testpixeln
	RgbImage rgb(15, 16);
	rgb.fill(RgbPixel(200, 200, 200));
	rgb.setPixel(0, 0, COLOR::Black);
	rgb.setPixel(1, 1, COLOR::Darkgrey);
	rgb.setPixel(2, 2, COLOR::Grey);
	rgb.setPixel(3, 3, COLOR::Lightgrey);
	rgb.setPixel(4, 4, COLOR::White);
	rgb.setPixel(5, 5, COLOR::Red);
	rgb.setPixel(6, 6, COLOR::Green);
	rgb.setPixel(7, 7, COLOR::Blue);

	//Als Datei speichern
	rgb.store("test.bmp");
	
	//Skalierte Version erstellen und speichern
	RgbImage tmp = rgb.scale(20);
	tmp.store("test-scale-20.bmp");
	
	//Alternative: Anonymes Objekt
	rgb.scale(10).store("test-scale-10.bmp");
	
	GreyImage g1(13, 13);
	g1.fill(COLOR::Black);
	g1.setPixel(0, 0, COLOR::Black);
	g1.setPixel(1, 1, COLOR::Darkgrey);
	g1.setPixel(2, 2, COLOR::Grey);
	g1.setPixel(3, 3, COLOR::Lightgrey);
	g1.setPixel(4, 4, COLOR::White);

	g1.store("lut1.bmp");

	GreyImage g2("lut1.bmp");
	g2.store("lut2.bmp");
}

*/