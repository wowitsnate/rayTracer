#include <cstdint>
#include <string>
#include <fstream>
#include "../Colour/Colour.h"
#include "../Math/Interval.h"
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

std::unique_ptr<std::string> PPM::convertToString(int samplesPerPixel) const
{
	std::unique_ptr<std::string> res { new std::string() };

	//Create Header
	res->append("P3\n");
	res->append(std::to_string(m_width) + ' ' + std::to_string(m_height) + "\n");
	res->append(std::to_string(m_maxColours) + "\n");

	//Append Data
	for (auto col : m_data)
	{
		double scale = 1.0 / (double)samplesPerPixel;
		col *= scale;

		static const Interval intensity(0.000, 0.999);
		int r = static_cast<int>(255.0 * intensity.clamp(col.r));
		int g = static_cast<int>(255.0 * intensity.clamp(col.g));
		int b = static_cast<int>(255.0 * intensity.clamp(col.b));

		res->append(std::to_string(r) + ' ' + std::to_string(g) + ' ' + std::to_string(b) + "\n");
	}


	return res;
}

bool PPM::outputToFile(const std::string& file_path, int samplesPerPixel) const
{
    try {
        const std::unique_ptr<std::string> ppmData = convertToString(samplesPerPixel);

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
