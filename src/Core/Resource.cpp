#include "Resource.h"

#include "Util.h"

Resource::Resource(const std::string& path, const std::string& fileName, const std::string& extension)
    : isDir(fileName.empty() && extension.empty()), m_fullPath(path + fileName), m_extension(extension)
{
}

void Resource::setExtension(const std::string& extension)
{
    m_extension = extension;
    isDir = m_extension.empty();
}

std::string Resource::getPath() const
{
    std::string path = m_fullPath;
    if(!m_extension.empty())
        path += "." + m_extension;

    return path;
}

bool Resource::operator==(const Resource& r) const
{
    return (m_fullPath == r.m_fullPath);
}

Resource Resource::getNth(unsigned int i)
{
    return Resource(m_fullPath, std::to_string(i), m_extension);
}
