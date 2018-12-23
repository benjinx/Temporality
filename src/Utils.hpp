#ifndef UTILITIES_H
#define UTILITIES_H

#include "Config.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class GameObject;
struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
class Mesh;

enum LogLevel
{
	LOG_INFO = 0,
	LOG_WARNING,
	LOG_ERROR,
};

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

	unsigned char* LoadPng(std::string filename, int& w, int& h, int& bpp);
	void FreePng(unsigned char* img);
	GLuint LoadTexture(std::string filename);

	std::string GetBasename(const std::string& path);
	std::string GetDirname(const std::string& path);

	Mesh* Get2DMeshData(glm::vec4 screenCords, glm::vec4 textureCords);

	//void Log(LogLevel flag, std::string infoStr);
}

#endif // UTILITIES_H
