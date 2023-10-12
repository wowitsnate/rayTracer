#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

class HitRecord;

class Material
{
public:
	virtual ~Material() = default;

	virtual bool scatter( const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const Colour& a) : m_albedo(a) {};

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const override;

private:
	Colour m_albedo;
};

class Metal : public Material
{
public:
	Metal(const Colour& a, double f) : m_albedo(a), m_fuzz(f) {}

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const override;

private:
	Colour m_albedo;
	double m_fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(double RefractiveIndex) : ir(RefractiveIndex) {}

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const override;
private:
	double ir; //Refractive Index

	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

#endif