#include "ShaderParser.h"
#include "Utils.h"
#include "Logger.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

namespace Nova
{
	ShaderParser::ShaderParser()
	{
		// empty	
	}


	ShaderParser::~ShaderParser()
	{
		// empty
	}


	void ShaderParser::LoadShaderOBJSource(const std::string& filePath, ShaderOBJSource& shaderSource)
	{

		// open the shader file
		std::ifstream file(filePath);
		std::set<std::string> includes; // used to check if file allready included

		if (file.is_open())
		{
			shaderSource.filePath = filePath;

			// check what shader type it is
			std::vector<std::string> split;
			Utils::split(filePath, '.', split);

			if (split[1].find("vs") != std::string::npos)
			{
				shaderSource.type = GL_VERTEX_SHADER;
			}
			else 
			if (split[1].find("fs") != std::string::npos)
			{
				shaderSource.type = GL_FRAGMENT_SHADER;
			}
			else
			if (split[1].find("gs") != std::string::npos)
			{
				shaderSource.type = GL_GEOMETRY_SHADER;
			}
			else
			{
				shaderSource.type = -1;
				std::cout << "shader file type not supported - type: " << split[1] << std::endl;
				return;
			}

			
			
			std::string line; // current line being proccessed 
			while (getline(file, line))
			{
				if (line.find("#IncludeGuard") != std::string::npos)
				{
					std::vector<std::string> includePath;
					Utils::split(line, '"', includePath);

					// the file being included was not included yet
					if (includes.find(includePath[1]) == includes.end())
					{
						includes.insert(includePath[1]);
						Include(includePath[1], shaderSource,includes);
					}
					else // the file is allready included so do nothing
					{
					
					}
				}
				else // check for non prama includes
				if (line.find("#include") != std::string::npos)
				{
					
					std::vector<std::string> includePath;
					Utils::split(line, '"', includePath);

					Include(includePath[1],shaderSource,includes);
				}
				else
				{
					shaderSource.source += line + "\n";
					shaderSource.lineCount++;
				}	
			}
			file.close();
		}
		else
		{
			std::cout << "file not found: " << filePath << std::endl;
		}	

		// prints shader source once loaded
		/*std::cout << shaderSource.source << std::endl;
		std::cout << "------------------------------------------------" << std::endl;*/
	}


	void ShaderParser::Include(const std::string& filePath, ShaderOBJSource& shaderSource,std::set<std::string>& includes)
	{
		// check if the include file is .glsl
		std::vector<std::string> split;
		Utils::split(filePath, '.', split);

		if (split[1].find("glsl") != std::string::npos)
		{
			// open the shader file
			std::ifstream file(filePath);

			// get the current location of the originaly loaded shader file

			if (file.is_open())
			{
				std::string line;
				while (getline(file, line))
				{
					if (line.find("#IncludeGuard") != std::string::npos)
					{
						std::vector<std::string> includePath;
						Utils::split(line, '"', includePath);

						// the file being included was not included yet
						if (includes.find(includePath[1]) == includes.end())
						{
							includes.insert(includePath[1]);
							Include(includePath[1], shaderSource,includes);
						}
						else // the file is allready included so do nothing
						{

						}
					}
					else // check for non prama includes
						if (line.find("#include") != std::string::npos)
						{

							std::vector<std::string> includePath;
							Utils::split(line, '"', includePath);

							Include(includePath[1], shaderSource, includes);
						}
						else
						{
							shaderSource.source += line + "\n";
							shaderSource.lineCount++;
						}
				}
				file.close();
			}
			else
			{
				Logger::GetInstance().ErrorBlock("Shader Parser Error", "Error: file not found\nFile: " + filePath,true);
			}
		}
		else
		{
			Logger::GetInstance().ErrorBlock("Shader Parser Error", "Error: the included file must be of type .glsl\nFile: " + filePath,true);
		}
	}
}
