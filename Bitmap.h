/* 
 * File:   Bitmap.h
 * Author: Michael Roth
 *
 * Created on 3. September 2014, 07:58
 */

#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <iostream>
#include <string>

/** \brief Abstract Bitmap class
 * 
 * This class should not be used directly, as it has no sense of different
 * colour depths. Image data is seen on byte level only.
 */
class Bitmap
{
public:
	/**
	 * Cleans up dynamically genereted bitmap array
	 */
	virtual ~Bitmap();

	/**
	 * Loads image data from file, if and only if the member field m_fname
	 * is set, which happens whens using the string constructor. If m_fname
	 * is empty, this function ends.
	 */
	void load();
	/**
	 * Loads image from file with given name. Throws invalid_argument when
	 * file could not be opened and runtime_error when file is incomplete. No
	 * other checks are done.
	 * @param fname URL to file
	 */
	void load(const std::string& fname);
	/**
	 * Stores image data to file, if and only if member field m_fname is set,
	 * which happens when using the string constructor. if m_fname is empty,
	 * this function ends.
	 */
	void store() const;
	/**
	 * Stores image as bmp to file denoted by fname. Existing files will be
	 * overwritten
	 * @param fname URL to file
	 */
	void store(const std::string& fname) const;
        
        
        //const int get_width();
        //const int get_height(); 


protected:

	/**
	 * <p>magic: Header identifier, should be "BM" for Windows NT Bitmaps</p>
	 * <p>filesz: complete file size in bytes</p>
	 * <p>creator1 and creator2: reserved</p>
	 * <p>bmp_offset: Offset where pixel data array can be found</p>
	 */
	struct BmpFileHeader_t
	{
		// 2byte magic string
		uint8_t magic[2];
		// 4byte total file size in bytes
		uint32_t filesz;
		// 2byte reserved
		uint16_t creator1;
		// 2byte reserved
		uint16_t creator2;
		// 4byte byte offset of bmp raw data
		uint32_t bmp_offset;
	};

	/**
	 * <p>header_sz: Size of DIB Header in bytes. Use 40bytes for standard Windows NT DIB Header.</p>
	 * <p>width: the bitmap width in pixels (signed integer)</p>
	 * <p>height:the bitmap height in pixels (signed integer)</p>
	 * <p>nplanes: the number of color planes must be 1</p>
	 * <p>bitspp: the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.</p>
	 * <p>compress_type: Compression method used, 0 for none</p>
	 * <p>bmp_bytesz: Size of raw bitmap data, can be 0 if no compression is used</p>
	 * <p>hres and vres: horizontal and vertical resolution in pixel per meter</p>
	 * <p>ncolors: the number of colours in the colour palette</p>
	 * <p>nimpcolor: the number of important colours, 0 for every colour is important</p>
	 */
	struct DibHeader_t
	{
		// 4byte header size
		uint32_t header_sz;
		// 4byte image width
		int32_t width;
		// 4byte image height
		int32_t height;
		// 2byte number of image planes
		uint16_t nplanes;
		// 2byte bits per pixel
		uint16_t bitspp;
		// 4byte compression type
		uint32_t compress_type;
		// 4byte bytesize of bmp raw data
		uint32_t bmp_bytesz;
		// 4byte horizontal resolution
		int32_t hres;
		// 4byte vertical resolution
		int32_t vres;
		// 4byte number of colours in palette
		uint32_t ncolors;
		// 4byte number of important colours, 0 means all important
		uint32_t nimpcolors;
	};


	Bitmap();
	Bitmap(int32_t width, int32_t height);
	Bitmap(const std::string& fname);
	BmpFileHeader_t m_header;
	DibHeader_t m_dib;
	std::string m_fname;
	int32_t m_width;
	int32_t m_height;
	int32_t m_rowsize;
	uint8_t** m_data;

	void check(int32_t x, int32_t y) const;

private:
	void fillHeaders();
};

/**
 * Class holding single byte value denoting grey values ranging
 * from 0 to 255.
 */
class GreyPixel
{
public:
	uint8_t value;

	/**
	 * Constructs GreyPixel and sets internal value to given value
	 * @param value Grey value ranging from 0 to 255, 0 beeing black
	 */
	GreyPixel(uint8_t value) :
	value(value)
	{
	}

};

/**
 * Class holding three byte values denoting red, green and blue values
 * for creating a RGB-Value.
 */
class RgbPixel
{
public:
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	/**
	 * Conversion from GreyPixel. Sets alls red, green and blue values to
	 * grey value from given GreyPixel
	 * @param p GreyPixel to be copied
	 */
	RgbPixel(const GreyPixel& p) : red(p.value), green(p.value), blue(p.value)
	{

	}

	/**
	 * Instantiates a RgbPixel with given red, green and blue values
	 * @param blue Value of blue chanel from 0 to 255, 0 meaning black
	 * @param green Value of green chanel from 0 to 255, 0 meaning black
	 * @param red Value of green chanel from 0 to 255, 0 meaning black
	 */
	RgbPixel(uint8_t blue, uint8_t green, uint8_t red) :
	red(red), green(green), blue(blue)
	{
	}

};

/**
 * 
 * \brief Class handling 24-bit RGB Images
 * 
 * This class handles a single Bitmap image with a colour depth of 24 bit, meaning
 * three byte values for red, green and blue channel. An Alpha Channel is not supported.
 */
class RgbImage : public Bitmap
{
public:
	/**
	 * Creates an instance with given width and height in pixels. The proper amount
	 * of memory is allocated, but not filled, so image data is kind of random at
	 * this point. This throws an invalid_argument exception if width and/or height
	 * is negative. Note that the standard actually supports negative height, but
	 * this implementation does not.
	 * @param width Desired image width in pixels
	 * @param height Desired image height in pixels
	 */
	RgbImage(int32_t width, int32_t height);

	/**
	 * Creates an instance by loading from the file given as parameter.
	 * This uses the Bitmap class' method load() to load that file.
	 * @param fname URL to file
	 */
	RgbImage(const std::string& fname);

	/**
	 * Gets a pixel from the Bitmap. Throws out_of_range if the desired pixel
	 * is not inside the actual Bitmap.
	 * @param x X position, 0 is left
	 * @param y Y position, 0 is up
	 * @return Copy of RgbPixel Element at (x,y)
	 */
	RgbPixel getPixel(int32_t x, int32_t y) const;

	/**
	 * Sets the desired pixel to the given value. Throws out_of_range and does
	 * not change anything if the desired location is outside the actual image.
	 * @param x X position, 0 is left
	 * @param y Y position, 0 is up
	 * @param val RgbPixel element that should be written
	 */
	void setPixel(int32_t x, int32_t y, RgbPixel val);

	/**
	 * This fills the whole image by calling setPixel for every valid (x,y)
	 * combination.
	 * @param p RgbPixel element that should be written
	 */
	void fill(RgbPixel p);

	/**
	 * Creates a scaled version of the image. Only upscaling is supported at
	 * the moment and the scaling factor has to an integral value
	 * @param s Scaling factor, positive, integral value
	 * @return New image with the desired scaling applied
	 */
	RgbImage scale(int s);
private:
	RgbImage();
};

class GreyImage : public Bitmap
{
public:
	/**
	 * Creates an instance with given width and height in pixels. The proper amount
	 * of memory is allocated, but not filled, so image data is kind of random at
	 * this point. This throws an invalid_argument exception if width and/or height
	 * is negative. Note that the standard actually supports negative height, but
	 * this implementation does not.
	 * <p> Note that although this uses a LUT, there is currently no way to edit the
	 * LUT. The LUT is created when the image is stored as a .bmp file and contains
	 * 256 entries for the 256 Greyscale colours.</p>
	 * @param width Desired image width in pixels
	 * @param height Desired image height in pixels
	 */
	GreyImage(int32_t width, int32_t height);
	/**
	 * Creates an instance by loading from the file given as parameter.
	 * This uses the Bitmap class' method load() to load that file.
	 * <p>If the file contained a LUT, this LUT is not loaded to memory.</p>
	 * @param fname URL to file
	 */
	GreyImage(const std::string& fname);
	/**
	 * Gets a pixel from the Bitmap. Throws out_of_range if the desired pixel
	 * is not inside the actual Bitmap.
	 * @param x X position, 0 is left
	 * @param y Y position, 0 is up
	 * @return Copy of GreyPixel Element at (x,y)
	 */
	GreyPixel getPixel(int32_t x, int32_t y) const;

	/**
	 * Sets the desired pixel to the given value. Throws out_of_range and does
	 * not change anything if the desired location is outside the actual image.
	 * @param x X position, 0 is left
	 * @param y Y position, 0 is up
	 * @param val GreyPixel element that should be written
	 */
	void setPixel(int32_t x, int32_t y, GreyPixel val);

	/**
	 * This fills the whole image by calling setPixel for every valid (x,y)
	 * combination.
	 * @param p GreyPixel element that should be written
	 */
	void fill(GreyPixel p);

	/**
	 * Creates a scaled version of the image. Only upscaling is supported at
	 * the moment and the scaling factor has to an integral value
	 * @param s Scaling factor, positive, integral value
	 * @return New image with the desired scaling applied
	 */
	GreyImage scale(int s);

private:
	GreyImage();
};

/**
 * Namespace containing different colour definitions:
 *  - Black
 *  - White
 *  - Grey
 *  - Lightgrey
 *  - Darkgrey
 *  - Blue
 *  - Red
 *  - Green
 *  - Magenta
 *  - Cyan
 *  - Yellow
 */
namespace COLOR
{
//A GreyPixel with value 0, can also be used where RgbPixel is applicable
static GreyPixel Black(0);
static GreyPixel White(255);
static GreyPixel Lightgrey(192);
static GreyPixel Darkgrey(64);
static GreyPixel Grey(128);

static RgbPixel Blue(0, 0, 255);
static RgbPixel Red(255, 0, 0);
static RgbPixel Green(0, 255, 0);
static RgbPixel Cyan(0, 255, 255);
static RgbPixel Magenta(255, 0, 255);
static RgbPixel Yellow(255, 255, 0);
}

#endif /* BITMAP_H */

