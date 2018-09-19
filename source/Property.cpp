#include "Property.h"



ini::Property::Property(std::string property_name)
	: name{ std::move(property_name) }
	//value{ 0 }
{
}

ini::Property::~Property()
{
}

std::string ini::Property::getName() const
{
	return name;
}
