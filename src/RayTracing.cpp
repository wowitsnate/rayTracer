#include <iostream>
#include "../src/Math/Vector.h"
#include "../src/Objects/Hittable.h"
#include "../src/Colour/Colour.h"
#include "../src/PPM/PPM.h"
#include "../src/Camera/Camera.h"
#include "../src/Materials/Material.h"

#include "../src/Ray/Ray.h"
#include "Utility/Utility.h"


int main()
{
    std::unique_ptr<HittableList> GObjectList{new HittableList()};

    int ImageWidth = 400;


    const auto camData = CameraInitData
    {
        ImageWidth,
        16.0 / 9.0,
        20,
        500,
        150,
        Point3(-2.0, 2.0, 1.0),
        Point3(0.0, 0.0, -1.0),
        Vector3(0.0, 1.0, 0.0)
    };

    auto camMain = Camera(camData);
    auto ppmOutputFile = PPM(camMain.m_imageWidth, camMain.m_imageHeight, 255);
    

    //Materials
    auto material_ground = std::make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Colour(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<Dielectric>(1.5);
    auto material_right = std::make_shared<Metal>(Colour(0.8, 0.6, 0.2), 0.0);


    //Add All Out Objects
    
    GObjectList->addObject(std::make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    GObjectList->addObject(std::make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    GObjectList->addObject(std::make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    GObjectList->addObject(std::make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
    GObjectList->addObject(std::make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));


    camMain.render(GObjectList, ppmOutputFile);

	const bool sucsess = ppmOutputFile.outputToFile("C:\\Users\\nate\\Desktop\\Debug_Testing_Output\\test1.ppm", camData.samplesPerPixel);

    std::cout << "Did it work?: " << sucsess << std::endl;

    return 0;
}
