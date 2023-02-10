
#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>


namespace Renderer
{
	ResourceManager::ResourceManager(const std::string& executablePath)
	{
		std::size_t found = executablePath.find_last_of("/\\");
		m_path = executablePath.substr(0, found);
		std::cout << "executablePath runfile: " << executablePath << std::endl;
		std::cout << "Path runfile: " << m_path << std::endl;
	}
	std::string ResourceManager::getFileString(const std::string& relativeFilePath) const
	{
		std::ifstream f;
		f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
		if(f.is_open())
		{
			std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
			return std::string{};
		}
		std::stringstream buffer;
		buffer << f.rdbuf();
		return buffer.str();

	}

	std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertexStringBuf = getFileString(vertexPath);
		if(vertexStringBuf.empty())
		{
			std::cerr << "No vertex shader!" << std::endl;
			return nullptr;
		}

		std::string fragmentStringBuf = getFileString(fragmentPath);
		if(fragmentStringBuf.empty())
		{
			std::cerr << "No fragment shader!" << std::endl;
			return nullptr;
		}

		std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexStringBuf, fragmentStringBuf)).first->second;
		if (newShader->isCompiled())
		{
			return newShader;
		}
		std::cerr << "Can`t load shader program:\n"
				  << "Vertex: " << vertexPath << "\n"
				  << "Fragment: " << fragmentPath << std::endl;

		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
	{
		ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
		if (it != m_shaderPrograms.end())
		{
			return  it->second;
		}
		std::cerr << "Can`t find the shader program: " << shaderName << std::endl;
		return nullptr;
	}
}