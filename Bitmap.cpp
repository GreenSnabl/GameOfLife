/* 
 * File:   Bitmap.cpp
 * Author: Michael Roth
 * 
 * Created on 3. September 2014, 07:58
 */

#include "Bitmap.h"
#include <fstream>

#include <stdexcept>
#include <cstring>

//GreyPixel COLOR::BLACK = GreyPixel(0);

void Bitmap::fillHeaders() {
    m_header.magic[0] = 'B';
    m_header.magic[1] = 'M';
    m_header.filesz = 54; //just header and dib header
    m_header.creator1 = 0;
    m_header.creator2 = 0;
    m_header.bmp_offset = 54;

    m_dib.header_sz = 40;
    m_dib.nplanes = 1;
    m_dib.compress_type = 0;
    m_dib.hres = 2835; //72dpi
    m_dib.vres = 2835;
    m_dib.nimpcolors = 0;
}

Bitmap::Bitmap() {
    //assume image size of 0x0
    fillHeaders();
    m_dib.width = 0;
    m_dib.height = 0;
    m_dib.ncolors = 0;
    m_dib.bitspp = 8;
    m_dib.bmp_bytesz = 0;
    m_data = NULL;
    m_fname = "";
}

Bitmap::Bitmap(int32_t width, int32_t height) : m_width(width), m_height(height) {
    if (width < 0 || height < 0) {
        throw std::invalid_argument("No negative width and/or height allowed!");
    }

    fillHeaders();
    m_dib.width = m_width;
    m_dib.height = m_height;
    m_dib.ncolors = 0;
    m_dib.bitspp = 8;
    m_dib.bmp_bytesz = 0;
    m_data = NULL;
    m_fname = "";
}

Bitmap::Bitmap(const std::string& fname) {
    m_fname = fname;
    m_data = NULL;
    load(fname);
}

Bitmap::~Bitmap() {
    if (m_data != NULL) {
        for (int i = 0; i < m_height; i++) {
            delete[] m_data[i];
        }
        delete[] m_data;
    }
}

void Bitmap::store() const {
    if (m_fname != "") {
        store(m_fname);
    }
}

void Bitmap::store(const std::string& fname) const {
    std::ofstream out(fname.c_str());

    if (!out.good())
        throw std::invalid_argument("File could not be written!");

    //out.write(reinterpret_cast<const char*> (&m_header), 14);
    out.write(reinterpret_cast<const char*> (&m_header.magic), sizeof (m_header.magic));
    out.write(reinterpret_cast<const char*> (&m_header.filesz), sizeof (m_header.filesz));
    out.write(reinterpret_cast<const char*> (&m_header.creator1), sizeof (m_header.creator1));
    out.write(reinterpret_cast<const char*> (&m_header.creator2), sizeof (m_header.creator2));
    out.write(reinterpret_cast<const char*> (&m_header.bmp_offset), sizeof (m_header.bmp_offset));

    out.write(reinterpret_cast<const char*> (&m_dib), 40);

    switch (m_dib.bitspp) {
        case 24: break;
        case 8:
            uint8_t lut[4];
            lut[3] = 0; //reserved

            for (int row = 0; row < 256; row++) {
                lut[0] = row;
                lut[1] = row;
                lut[2] = row;
                out.write(reinterpret_cast<char*> (lut), 4);
            }

            break;
        default: throw std::runtime_error("Colour depth is not supported!");
    }

    for (int row = 0; row < m_height; row++) {
        out.write(reinterpret_cast<const char*> (m_data[row]), m_rowsize);
    }

    out.close();
}

void Bitmap::load() {
    if (m_fname != "") {
        load(m_fname);
    }
}

void Bitmap::load(const std::string& fname) {
    if (m_data != NULL) {
        for (int i = 0; i < m_height; i++) {
            delete[] m_data[i];
        }
        delete[] m_data;
    }

    std::ifstream in(fname.c_str());

    if (!in.good()) {
        throw std::invalid_argument("File could not be opened!");
    }

    //in.read(reinterpret_cast<char*> (&m_header), 14);

    in.read(reinterpret_cast<char*> (&m_header.magic), sizeof (m_header.magic));
    in.read(reinterpret_cast<char*> (&m_header.filesz), sizeof (m_header.filesz));
    in.read(reinterpret_cast<char*> (&m_header.creator1), sizeof (m_header.creator1));
    in.read(reinterpret_cast<char*> (&m_header.creator2), sizeof (m_header.creator2));
    in.read(reinterpret_cast<char*> (&m_header.bmp_offset), sizeof (m_header.bmp_offset));

    in.read(reinterpret_cast<char*> (&m_dib), 40);

    m_width = m_dib.width;
    m_height = m_dib.height;

    m_fname = fname;

    m_rowsize = m_width * (m_dib.bitspp / 8);
    if (m_rowsize % 4 != 0)
        m_rowsize += (4 - (m_rowsize % 4));

    in.seekg(m_header.bmp_offset);

    m_data = new uint8_t*[m_height];
    for (int i = 0; i < m_height; i++) {
        m_data[i] = new uint8_t[m_rowsize];
        in.read(reinterpret_cast<char*> (m_data[i]), m_rowsize);
        if (in.eof()) {
            throw std::runtime_error("End of file reached before all data was read!");
        }
    }

}

void Bitmap::check(int32_t x, int32_t y) const {
    if (x < 0 || x >= m_width) {
        throw std::out_of_range("x argument is out of range");
    }

    if (y < 0 || y >= m_height) {
        throw std::out_of_range("y argument is out of range");
    }
}

GreyImage::GreyImage() : Bitmap() {
    m_dib.bitspp = 8;

    //add size of lut
    m_header.filesz += 256 * 4;
    m_header.bmp_offset += 256 * 4;
}

GreyImage::GreyImage(int32_t width, int32_t height) : Bitmap(width, height) {
    m_dib.bitspp = 8;
    m_rowsize = m_width * (m_dib.bitspp / 8);
    if (m_rowsize % 4 != 0)
        m_rowsize += (4 - (m_rowsize % 4));

    m_data = new uint8_t*[m_height];
    for (int i = 0; i < m_height; i++) {
        m_data[i] = new uint8_t[m_rowsize];
        memset(m_data[i], 0, m_rowsize);
    }

    m_dib.bmp_bytesz = m_rowsize * m_height;
    m_header.filesz += m_dib.bmp_bytesz;

    //add size of lut
    m_header.filesz += 256 * 4;
    m_header.bmp_offset += 256 * 4;
}

GreyImage::GreyImage(const std::string& fname) : Bitmap(fname) {

}

GreyPixel GreyImage::getPixel(int32_t x, int32_t y) const {
    check(x, y);
    GreyPixel* tmp;
    tmp = reinterpret_cast<GreyPixel*> (m_data[m_height - y - 1]);
    tmp = &tmp[x];
    return *tmp;

}

void GreyImage::setPixel(int32_t x, int32_t y, GreyPixel val) {
    check(x, y);
    GreyPixel* tmp;
    tmp = reinterpret_cast<GreyPixel*> (m_data[m_height - y - 1]);
    tmp = &tmp[x];
    *tmp = val;
}

void GreyImage::fill(GreyPixel p) {
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            setPixel(x, y, p);
        }
    }
}

GreyImage GreyImage::scale(int s) {
    GreyImage img(m_width * s, m_height * s);

    for (int x = 0; x < img.m_width; x++) {
        for (int y = 0; y < img.m_height; y++) {
            img.setPixel(x, y, getPixel(x / s, y / s));
        }
    }

    return img;
}

RgbImage::RgbImage() : Bitmap() {
    m_dib.bitspp = 24;
}

RgbImage::RgbImage(const std::string& fname) : Bitmap(fname) {

}

RgbImage::RgbImage(int32_t width, int32_t height) : Bitmap(width, height) {
    m_dib.bitspp = 24;
    m_rowsize = m_width * (m_dib.bitspp / 8);
    if (m_rowsize % 4 != 0)
        m_rowsize += (4 - (m_rowsize % 4));

    m_data = new uint8_t*[m_height];
    for (int i = 0; i < m_height; i++) {
        m_data[i] = new uint8_t[m_rowsize];
        memset(m_data[i], 0, m_rowsize);
    }
    m_dib.bmp_bytesz = m_rowsize * m_height;
    m_header.filesz += m_dib.bmp_bytesz;
}

RgbPixel RgbImage::getPixel(int32_t x, int32_t y) const {
    check(x, y);
    RgbPixel* tmp;
    tmp = reinterpret_cast<RgbPixel*> (m_data[m_height - y - 1]);
    tmp = &tmp[x];
    return *tmp;
}

void RgbImage::setPixel(int32_t x, int32_t y, RgbPixel val) {
    check(x, y);
    RgbPixel* tmp;
    tmp = reinterpret_cast<RgbPixel*> (m_data[m_height - y - 1]);
    tmp = &tmp[x];
    *tmp = val;
}

void RgbImage::fill(RgbPixel p) {
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            setPixel(x, y, p);
        }
    }
}

RgbImage RgbImage::scale(int s) {
    RgbImage img(m_width * s, m_height * s);

    for (int x = 0; x < img.m_width; x++) {
        for (int y = 0; y < img.m_height; y++) {
            img.setPixel(x, y, getPixel(x / s, y / s));
        }
    }

    return img;
}









