#include <cstdint>
#include <string>
#include <fstream>
#include "../Colour/Colour.h"
#include "PPM.h"

PPM::PPM(const uint32_t width, const uint32_t height, const uint32_t max_colours)
{
	m_width = width;
	m_height = height;
	m_maxColours = max_colours;

	//For efficiancy when appending colours to the vector we are going to reserve the maximium space it can take up
	const uint32_t max_size = width * height;
	m_data.reserve(max_size);
}

uint32_t PPM::getWidth() const
{
	return m_width;
}

uint32_t PPM::getHeight() const
{
	return m_height;
}

std::unique_ptr<std::string> PPM::convertToString() const
{
	std::unique_ptr<std::string> res { new std::string() };

	//Create Header
	res->append("P3\n");
	res->append(std::to_string(m_width) + ' ' + std::to_string(m_height) + "\n");
	res->append(std::to_string(m_maxColours) + "\n");

	//Append Data
	for (const auto& col : m_data)
	{
		res->append(col.asString() + "\n");
	}


	return res;
}

bool PPM::outputToFile(const std::string& file_path) const
{
    try {
        const std::unique_ptr<std::string> ppmData = convertToString();

        std::ofstream file(file_path);

        if (!file.is_open()) {
            return false;
        }
        file << *ppmData;

        file.close();

        return true;
    }
    catch (...) {
        return false;
    }
}
