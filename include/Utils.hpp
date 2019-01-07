#ifndef UTILITIES_H
#define UTILITIES_H

#include <Config.hpp>
#include <Math.hpp>

#include <sstream>
#include <vector>

class Mesh;

namespace Utils
{
	inline std::vector<std::string> GetResourcePaths()
	{
		static std::vector<std::string> paths;

		if (paths.empty()) {
			std::stringstream ss(RESOURCE_PATH);
			std::string path;
			while (std::getline(ss, path, ':')) {
				paths.push_back(path);
			}
			paths.push_back(".");
		}

		return paths;
	}

	std::string GetBasename(const std::string& path);
	std::string GetDirname(const std::string& path);
	std::string GetExtension(const std::string& path);

	Mesh* Get2DMesh(glm::vec4 screenCords, glm::vec4 textureCords);

}

#endif // UTILITIES_H