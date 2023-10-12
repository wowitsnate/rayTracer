#include <iostream>
#include "../src/Math/Vector.h"
#include "../src/Objects/Hittable.h"
#include "../src/Colour/Colour.h"
#include "../src/PPM/PPM.h"
#include "../src/Camera/Camera.h"

#include "../src/Ray/Ray.h"




int main()
{
    std::unique_ptr<HittableList> GObjectList{new HittableList()};
    auto ppmOutputFile = PPM(256, 256, 255);

    const auto camData = CameraInitData
    {
        ppmOutputFile.getWidth(),
        ppmOutputFile.getHeight(),
        Vector3{0, 0, 0},
        16.0 / 9.0,
        1.0,
        2.0
    };

    auto camMain = Camera(camData);


    //Add All Out Objects
    GObjectList->addObject(std::make_shared<Sphere>(Vec3{ 0.0, 0.0, -1.0 }, 0.5));
    GObjectList->addObject(std::make_shared<Sphere>(Vec3{ 0.0, -100.5, -1.0 }, 100));

    camMain.render(GObjectList, ppmOutputFile);

	const bool sucsess = ppmOutputFile.outputToFile("C:\\Users\\nate\\Desktop\\Debug_Testing_Output\\test1.ppm");

    std::cout << "Did it work?: " << sucsess << std::endl;

    return 0;
}