
#include "../Math/Vector.h"





#include "Hittable.h"
#include <cmath>


bool Sphere::intersects(const Ray& r, Interval ray_t, HitRecord& rec)
{
    vec3 oc = r.origin - m_center;
    auto a = r.direction.squaredNorm();
    auto half_b = oc.dot(r.direction);
    auto c = oc.squaredNorm() - (m_radius * m_radius);

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (!ray_t.surrounds(root)) { 
        root = (-half_b + sqrtd) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    const vec3 outward_normal = (rec.p - m_center) / m_radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}


void HittableList::addObject(std::shared_ptr<Hittable> obj)
{
    m_list.push_back(obj);
}

bool HittableList::intersects(const Ray& r, const Interval ray_t, HitRecord& rec)
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (const auto& object : m_list) {
        if (object->intersects(r, Interval(ray_t.min, closest_so_far), temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

void HittableList::clear()
{
    m_list.clear();
}

void HitRecord::set_face_normal(const Ray& r, const vec3& outward_normal)
{
    frontFace = r.direction.dot(outward_normal) < 0;
    normal = frontFace ? outward_normal : -outward_normal;
}

