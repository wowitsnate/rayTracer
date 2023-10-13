#include <iostream>
#include <chrono>

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

    int ImageWidth = 1920;


    const auto camData = CameraInitData
    {
        ImageWidth,
        16.0 / 9.0,
        20,
        2000,
        300,
        Point3(13.0,2.0,3.0),
        Point3(0.0, 0.0, 0.0),
        Vector3(0.0, 1.0, 0.0),
        0.6,
        10.0
    };

    auto camMain = Camera(camData);
    auto ppmOutputFile = PPM(camMain.m_imageWidth, camMain.m_imageHeight, 255);
    

    //Materials
    auto ground_material = std::make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
    GObjectList->addObject(std::make_shared<Sphere>(point3(0.0, -1000.0, 0.0), 1000.0, ground_material));

    //Add All Out Objects
    
    
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = genRandomDouble();
            point3 center(a + 0.9 * genRandomDouble(), 0.2, b + 0.9 * genRandomDouble());

            if ((center - point3(4.0, 0.2, 0.0)).magnitude() > 0.9) {
	            std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Colour::randColour() * Colour::randColour();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    GObjectList->addObject((std::make_shared<Sphere>(center, 0.2, sphere_material)));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Colour::randColour(0.5, 1);
                    auto fuzz = genRandomDoubleRange(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    GObjectList->addObject((std::make_shared<Sphere>(center, 0.2, sphere_material)));
                }
                else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    GObjectList->addObject((std::make_shared<Sphere>(center, 0.2, sphere_material)));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    GObjectList->addObject(std::make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
    GObjectList->addObject(std::make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
    GObjectList->addObject(std::make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    auto start = std::chrono::high_resolution_clock::now();

    camMain.render(GObjectList, ppmOutputFile);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);

	const bool sucsess = ppmOutputFile.outputToFile("C:\\Users\\nate\\Desktop\\Debug_Testing_Output\\test1.ppm", camData.samplesPerPixel);

    std::cout << "\nDid it work?: " << sucsess << std::endl;
    std::cout << "time taken (microseconds): " << duration.count() << std::endl;
    system("pause");

    return 0;
}
