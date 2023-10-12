#include "../Math/Vector.h"
#include "../Ray/Ray.h"
#include "../Objects/Hittable.h"
#include "../Utility/Utility.h"

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

    scattered = Ray(rec.p, reflected + randomUnitVector() * m_fuzz);
    attenuation = m_albedo;

    return (scattered.direction.dot(rec.normal) > 0);
}

bool Dielectric::scatter(const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const
{
    attenuation = Colour(1.0, 1.0, 1.0);

    double refraction_ratio = rec.frontFace ? (1.0 / ir) : ir;

    vec3 unit_direction = r_in.direction.normalize();

    double cosTheta = std::min((-unit_direction).dot(rec.normal), 1.0);
    double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = refraction_ratio * sinTheta > 1.0;
    Vector3 direction;

    if (cannotRefract || reflectance(cosTheta, refraction_ratio) > genRandomDouble())
    {
        direction = unit_direction.reflect(rec.normal);
    }
    else
    {
        direction = unit_direction.refract(rec.normal, refraction_ratio);
    }

    scattered = Ray(rec.p, direction);
    return true;
}
