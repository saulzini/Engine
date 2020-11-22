﻿#pragma once

#include <string>
#include <GL/glew.h>
#include <IL/il.h>
#include "Application.h"
#include "ModuleEditor.h"
#include "ConsoleWindow.h"
#include <IL/ilut.h>
#include <IL/ilu.h>

enum class Strategy
{
	Normal,
	SameFolder,
	Textures,
	White
};
class TextureLoader
{
public:
	// TODO::Check if is the best option to left static the method
	TextureLoader(){};

	/// <summary>
	/// Method that returns 0 if an error has ocurred if not it returns the opengl id
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="generateMipMaps"></param>
	/// <returns></returns>
	static unsigned int LoadTexture2D(const std::string &filename, bool generateMipMaps = true)
	{

		ILboolean success = false;
		ILuint imageID;

		// generate an image name
		ilGenImages(1, &imageID);
		// bind it
		ilBindImage(imageID);
		// match image origin to OpenGLs
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		
		//Looping given the strategies defined
		std::vector<Strategy> types{
			Strategy::Normal,
			Strategy::SameFolder,
			Strategy::Textures,
			Strategy::White,
		};

		for (std::vector<Strategy>::iterator it = types.begin(); it != types.end() && !success; it++)
		{
			std::string buf("Begin loading texture ");
			std::string path("");
			switch (*it)
			{
			case Strategy::Normal:
				buf.append(std::string("normal strategy:").c_str());
				path.append(filename.c_str());
				break;
			case Strategy::SameFolder:
				buf.append(std::string("same folder strategy:").c_str());
				path.append(filename.c_str());
				break;
			case Strategy::Textures:
				buf.append(std::string("textures strategy: ").c_str());
				path.append(filename.c_str());
				break;
			}

			App->editor->consoleWindow->AddLog(buf.append( path.c_str()).c_str());
			// load  the image
			success = ilLoadImage((ILstring) path.c_str());
			// check to see if everything went OK
			if (!success)
			{
				//Check for error
				ILenum Error;
				while ((Error = ilGetError()) != IL_NO_ERROR)
				{
					buf = "Error Loading texture: ";
					App->editor->consoleWindow->AddLog(buf.append(iluErrorString(Error)).c_str());
				}
			}
		}

		//if not success find a white texture
		if (!success)
		{
			ilDeleteImages(1, &imageID); //deleting image from il
			return 0;
		}

		std::string buf = "Loaded texture:";
		App->editor->consoleWindow->AddLog(buf.append(filename.c_str()).c_str());

		/* Convert image to RGBA with unsigned byte data type */
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		//Open gl section
		glGenTextures(1, &imageID);			   // creating image in opengl
		glBindTexture(GL_TEXTURE_2D, imageID); //Bind texture
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			ilGetInteger(IL_IMAGE_BPP),
			ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT),
			0,
			ilGetInteger(IL_IMAGE_FORMAT),
			GL_UNSIGNED_BYTE,
			ilGetData());

		if (generateMipMaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //linear interpolation for magnification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //linear interpolation for minifiying filter

		ilDeleteImage(imageID); //release from il since we have it in opengl

		return imageID;
	}
};
