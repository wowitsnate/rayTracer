#pragma once

#ifndef PPM_H
#define PPM_H
#include <memory>
#include <vector>

/*
 * PPM Image Files Are Organized in Such A Manner:
 *
 * P3 #This is the header indicating its a PPM File Type
 * X Y #The size of the image (for example 3 2 would be an image with 3 horizontal rows and 2 vertical rows)
 * ColourMax #This is the maximum value for each colour (usually 255). This is the end of the header
 * R G B
 * R G B
 * R G B
 * #etc... each colour is placed on a new line. Not shown but there should be \n for each of the lines
 */

class PPM
{
private:
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_maxColours;

public:
	std::vector<Colour<int>> m_data;

public:
	PPM(uint32_t width, uint32_t height, uint32_t max_colours = 255);

	uint32_t getWidth() const;
	uint32_t getHeight() const;

public:
	std::unique_ptr<std::string> convertToString() const;
	bool outputToFile(const std::string& file_path) const;

};

#endif

