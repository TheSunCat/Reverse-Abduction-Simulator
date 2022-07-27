#include "Resource.h"

#include "Util.h"

Resource::Resource(const std::string& path, const std::string& name) : fullPath(Util::path(path + name))
{
}

std::string Resource::getResourcePath() const
{
    return fullPath;
}

bool Resource::operator==(const Resource& r) const
{
    return (fullPath == r.fullPath);
}
