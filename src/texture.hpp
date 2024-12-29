#pragma once

#include <string>

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void use() const;

private:
	unsigned int m_id;

	void create();
	void load(const std::string& path) const;
};
