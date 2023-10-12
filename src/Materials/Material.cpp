#include "../Math/Vector.h"
#include "../Ray/Ray.h"
#include "../Objects/Hittable.h"

#include "Material.h"

bool Lambertian::scatter(const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const
{
    auto scatter_direction = rec.normal + randomUnitVector();

    //goofy scatter direction
    if (scatter_direction.isZero())
        scatter_direction = rec.normal;


    scattered = Ray(rec.p, scatter_direction);
    attenuation = m_albedo;
    return true;
}

bool Metal::scatter(const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const
{
    vec3 reflected = r_in.direction.normalize().reflect(rec.normal);

    scattered = Ray(rec.p, reflected);
    attenuation = m_albedo;

    return true;
}
