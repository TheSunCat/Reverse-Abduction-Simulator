#pragma once

#include <string>

class Resource
{
public:
    Resource(const std::string& path, const std::string& fileName = "", const std::string& extension = "");

    void setExtension(const std::string& extension);
    std::string getPath() const;

    Resource getNth(unsigned int i) const;

    bool isDir = false;

    bool operator==(const Resource& r) const;

private:
    std::string m_fullPath;
    std::string m_extension;
};
