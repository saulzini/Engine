#include "Model.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Application.h"
#include "ModuleEditor.h"
#include "ConsoleWindow.h"
#include "Globals.h"
#include "Vertex.h"
#include "Assimp/vector3.h"
#include "Assimp/vector2.h"
Model::Model(std::string path)
{
	textureLoader = new Texture2D();
	if (path == "") {
		return;
	}
	this->LoadModel(path);
}
void Model::Draw(unsigned int shader)
{
	for (GLuint i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	// If the import failed, report it
	std::string buf("Loading model:");
	App->editor->consoleWindow->AddLog( buf.append(path.c_str()).c_str() );

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		buf = "Error loading:";
		App->editor->consoleWindow->AddLog( buf.append(path.c_str()).append(importer.GetErrorString()).c_str() );
		//LOG("Error loading %s: %s", path, importer.GetErrorString());
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);

	buf = "End loading model:";
	App->editor->consoleWindow->AddLog(buf.append(path.c_str()).c_str());

}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(ProcessMesh(mesh, scene));
	}
	// process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh,const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		aiVector3t<float> vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;

		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) {
			aiVector2t<float> vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = aiVector2t<float>(0,0);
		}
		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j] );
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

//		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type,std::string typeName) {
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		GLboolean skip = false;

		for (GLuint j = 0; j < textures_loaded.size(); j++) {
			if (textures_loaded[j].path == str.C_Str()) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}

			
		}
		if (!skip) {
			Texture texture;
			texture.id = textureLoader->LoadTexture(str.C_Str());
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}