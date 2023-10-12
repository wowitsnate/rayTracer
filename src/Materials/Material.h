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
	Metal(const Colour& a) : m_albedo(a) {}

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const override;

private:
	Colour m_albedo;

};

#endif