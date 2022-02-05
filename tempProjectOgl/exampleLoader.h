#pragma once
#ifndef exampleLoader_H
#define exampleLoader_H

#include <iostream>
#include <string>
#include <Headers.h>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <cstdlib>
#include <vector>
//#include <glm/gtx/io.hpp>
#include <stdio.h>
#include <sstream>
#include <stb_image.h>
#include <filesystem>

using namespace std;
namespace fss = std::filesystem;

class exampleLoader {

public:
	ifstream fileobj, filemtl;
	Shader shader;

	string str, str1;
	string path;
	string file_mtl_name;
	string file_mtl_path;
	string picture_name;
	
	//vector<string> texture_map;
	//vector<unsigned int> texture_id;

	struct Materials
	{
		string material_name;

		vector<string> texture_map;
		vector<unsigned int> texture_id;

		vector<string>type_parameter_int;
		vector<float>value_parameter_int;

		vector<string>type_parameter_vec3;
		vector<glm::vec3>value_parameter_vec3;
	}mat;
	vector<Materials> material;

	
	struct Meshes
	{
		vector<glm::vec3> vertices_pos;
		vector<glm::vec3> vertices_norm;
		vector<glm::vec2> vertices_tex;
	}mesh;
	
	vector<Meshes> meshes;

	vector<unsigned int>indices_v_f;
	vector<unsigned int>indices_vn_f;
	vector<unsigned int>indices_vt_f;

	vector<unsigned int> indices;

	vector<glm::vec3> vertices_v;
	vector<glm::vec3> vertices_vn;
	vector<glm::vec3> vertices_vt;

	vector<float> vertices;
	vector<float> vvv;

	vector<vector<float>> verts;

	vector<string> usemtl_names;

	vector<int> sizes_of_meshes;
	
	float vs[288];
	vector<string> maps_types{ "map_Kd","map_Ks", "map_Bump", "map_Ka", "map_Ns", "map_d", "map_Ke" };
	float tmp_vert_x, tmp_vert_y, tmp_vert_z;


	exampleLoader(string path,string file_mtl_path)
	{
		this->path = path;
		this->file_mtl_path = file_mtl_path;
		fileobj.open(path);
		check_file_open(fileobj);
		
		ReadFileObj();
		
		//FillDataForBuff();
	

		setupVertices();
	}
	
	void ReadFileObj()
	{
		unsigned tmp_ind;
		vector <unsigned int> index_tmp;
		int count_obj = 0, tmp_obj = -1,usemtl = 0,tmp_mtl = -1;
		int tmp_size = 0;
		str = "";
		string mtl;

		while (!fileobj.eof())
		{
			
			str1 = "";
			string tmp_str = "mtllib";

			getline(fileobj, str);
			int found = str.find(tmp_str);
			if (found != string::npos)
			{
				file_mtl_name = "";
				for (int i = tmp_str.size() + 1; i < str.size(); i++)
				{
					file_mtl_name += str[i];
				}
				ReadFileMtl();
			}


			if (str.find("o ") != string::npos)
			{
				/*if (count_obj == tmp_obj)
				{
					FillDataForBuff();
					sizes_of_meshes.push_back(vertices.size());
					indices_v_f.clear();
					indices_vn_f.clear();
					indices_vt_f.clear();
				}*/
				tmp_obj = count_obj;
				count_obj++;
			}

			if (fileobj.eof())
			{
				FillDataForBuff();
				verts.push_back(vertices);

				//meshes.push_back(mesh);
				sizes_of_meshes.push_back(indices_v_f.size() * 8);
				indices_v_f.clear();
				indices_vn_f.clear();
				indices_vt_f.clear();
				vertices.clear();
				usemtl_names.push_back(mtl);			
			}

			if (fileobj.eof() && usemtl_names.size() == 0)
			{
				FillDataForBuff();
				verts.push_back(vertices);

				//meshes.push_back(mesh);
				sizes_of_meshes.push_back(indices_v_f.size() * 8);
				indices_v_f.clear();
				indices_vn_f.clear();
				indices_vt_f.clear();
				vertices.clear();
				usemtl_names.push_back(mtl);
			}

			if (str.find("usemtl") != string::npos)
			{

				if (usemtl == tmp_mtl)
				{
					usemtl_names.push_back(mtl);

					FillDataForBuff();
					verts.push_back(vertices);
					sizes_of_meshes.push_back(indices_v_f.size()*8);
					indices_v_f.clear();
					indices_vn_f.clear();
					indices_vt_f.clear();
					vertices.clear();
				}
				usemtl++;
				tmp_mtl = usemtl;
				mtl = str.substr(str.find("usemtl") + 7);
			}

			if (str.c_str()[0] == 'v' && str.c_str()[1] == ' ')
			{
				/*for (int i = 2; i < str.size(); i++)
				{
					if (str[i] != ' ')
					{
						str1 += str[i];
						if (i == str.size() - 1)
						{
							vertices_fl.push_back(stof(str1));
						}
					}
					else
					{
						vertices_fl.push_back(stof(str1));
						str1 = "";
					}

				}*/
				sscanf_s(str.c_str(), "%*c%f%f%f", &tmp_vert_x, &tmp_vert_y, &tmp_vert_z);
				vertices_v.push_back(glm::vec3(tmp_vert_x, tmp_vert_y, tmp_vert_z));
			}

			if (str.c_str()[0] == 'v' && str.c_str()[1] == 't')
			{
				/*for (int i = 3; i < str.size(); i++)
				{
					if (str[i] != ' ')
					{
						str1 += str[i];
						if (i == str.size() - 1)
						{
							vertices_fl.push_back(stof(str1));
						}
					}
					else
					{
						vertices_fl.push_back(stof(str1));
						str1 = "";
					}

				}*/
				sscanf_s(str.c_str(), "%*c%*c%f%f", &tmp_vert_x, &tmp_vert_y);
				vertices_vt.push_back(glm::vec3(tmp_vert_x, tmp_vert_y,0.0f));
				//vertices_vt.push_back(glm::vec2(tmp_vert_x, tmp_vert_y));
			}

			if (str.c_str()[0] == 'v' && str.c_str()[1] == 'n')
			{
				/*for (int i = 3; i < str.size(); i++)
				{
					if (str[i] != ' ')
					{
						str1 += str[i];
						if (i == str.size() - 1)
						{
							vertices_fl.push_back(stof(str1));
						}
					}
					else
					{
						vertices_fl.push_back(stof(str1));
						str1 = "";
					}

				}*/
				sscanf_s(str.c_str(), "%*c%*c%f%f%f", &tmp_vert_x, &tmp_vert_y, &tmp_vert_z);
				//glm::vec3 a = normalize_vec3(glm::vec3(tmp_vert_x, tmp_vert_y, tmp_vert_z));
				vertices_vn.push_back(glm::vec3(tmp_vert_x, tmp_vert_y, tmp_vert_z));

			}

			if (str.c_str()[0] == 'f' && str.c_str()[1] == ' ')
			{
				str.erase(0, 1);
				str.erase(0, 1);
				int checkIndex = 0;

				for (int i = 0; i < str.size(); i++)
				{
					if (str[i] == '/')checkIndex++;
				}

				if (checkIndex == 3) // obj file has no normal coords, but it is a triangle face
				{
					index_tmp.resize(9);
					sscanf_s(str.c_str(), "%d/%d %d/%d %d/%d", &index_tmp[0], &index_tmp[1], &index_tmp[3], &index_tmp[4],
						&index_tmp[6], &index_tmp[7]);
					index_tmp[2] = 1; index_tmp[5] = 1; index_tmp[8] = 1; // just to avoid errors

					for (int i = 0; i < 7; i += 3)
					{
						indices_v_f.push_back(index_tmp[i] - 1);
						indices_vt_f.push_back(index_tmp[i + 1] - 1);
						indices_vn_f.push_back(index_tmp[i + 2] - 1);
					}

				}
				else if (checkIndex == 4) //obj file has no normal coords, but it is a quad face(need to triangulate)
				{
					index_tmp.resize(12);
					sscanf_s(str.c_str(), "%d/%d %d/%d %d/%d %d/%d", &index_tmp[0], &index_tmp[1], &index_tmp[3], &index_tmp[4],
						&index_tmp[6], &index_tmp[7], &index_tmp[9], &index_tmp[10]);
					index_tmp[2] = 1; index_tmp[5] = 1; index_tmp[8] = 1; index_tmp[11] = 1; // just to avoid errors

					for (int i = 0; i < 7; i += 3)
					{
						indices_v_f.push_back(index_tmp[i] - 1);
						indices_vt_f.push_back(index_tmp[i + 1] - 1);
						indices_vn_f.push_back(index_tmp[i + 2] - 1);
					}
					for (int i = 6; i < 10; i += 3)
					{
						indices_v_f.push_back(index_tmp[i] - 1);
						indices_vt_f.push_back(index_tmp[i + 1] - 1);
						indices_vn_f.push_back(index_tmp[i + 2] - 1);
					}
					indices_v_f.push_back(index_tmp[0] - 1);
					indices_vt_f.push_back(index_tmp[1] - 1);
					indices_vn_f.push_back(index_tmp[2] - 1);
				}
				else if (checkIndex == 6) // triangle face
				{
					index_tmp.resize(9);

					if (str.find("//") != string::npos)// obj file has no texture coords
					{
						sscanf_s(str.c_str(), "%d//%d %d//%d %d//%d", &index_tmp[0], &index_tmp[2], &index_tmp[3],
							&index_tmp[5], &index_tmp[6], &index_tmp[8]);

						index_tmp[1] = 1; index_tmp[4] = 1; index_tmp[7] = 1; //just to avoid errors
					}
					else {
						sscanf_s(str.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", &index_tmp[0], &index_tmp[1],
							&index_tmp[2], &index_tmp[3], &index_tmp[4], &index_tmp[5], &index_tmp[6], &index_tmp[7], &index_tmp[8]);
					}
					for (int i = 0; i < 7; i += 3)
					{
						indices_v_f.push_back(index_tmp[i] - 1);
						indices_vt_f.push_back(index_tmp[i + 1] - 1);
						indices_vn_f.push_back(index_tmp[i + 2] - 1);
					}

				}
				else if (checkIndex == 8) // quad face, need to triangulate
				{
					index_tmp.resize(12);

					if (str.find("//") != string::npos) // obj file has no texture coords
					{
						sscanf_s(str.c_str(), "%d//%d %d//%d %d//%d %d//%d", &index_tmp[0], &index_tmp[2], &index_tmp[3],
							&index_tmp[5], &index_tmp[6], &index_tmp[8], &index_tmp[9], &index_tmp[11]);
						index_tmp[1] = 1; index_tmp[4] = 1; index_tmp[7] = 1; index_tmp[10] = 1;
					}
					else {
						sscanf_s(str.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &index_tmp[0], &index_tmp[1],
							&index_tmp[2], &index_tmp[3], &index_tmp[4], &index_tmp[5], &index_tmp[6], &index_tmp[7], &index_tmp[8], &index_tmp[9],
							&index_tmp[10], &index_tmp[11]);
					}
					for (int i = 0; i < 7; i += 3)
					{
						indices_v_f.push_back(index_tmp[i] - 1);
						indices_vt_f.push_back(index_tmp[i + 1] - 1);
						indices_vn_f.push_back(index_tmp[i + 2] - 1);
					}

					for (int i = 6; i < 10; i += 3)
					{
						indices_v_f.push_back(index_tmp[i] - 1);
						indices_vt_f.push_back(index_tmp[i + 1] - 1);
						indices_vn_f.push_back(index_tmp[i + 2] - 1);
					}
					indices_v_f.push_back(index_tmp[0] - 1);
					indices_vt_f.push_back(index_tmp[1] - 1);
					indices_vn_f.push_back(index_tmp[2] - 1);

				}
			}

		}
		
	
		/*for (int i = 0; i < vertices_v.size(); i++)
		{
			vvv.push_back(vertices_v[i].x);
			vvv.push_back(vertices_v[i].y);
			vvv.push_back(vertices_v[i].z);
		}*/
	}
	
	int GetMatSize()
	{
		return size_mat;
	}

	void FillDataForBuff()
	{
		if (vertices_vt.size() == 0)
			for (int i = 0; i < indices_v_f.size(); i++)
				vertices_vt.push_back(glm::vec3(0.0f));
		else if (vertices_vn.size() == 0)
			for (int i = 0; i < indices_v_f.size(); i++)
				vertices_vn.push_back(glm::vec3(0.0f));

		for (int i = 0; i < indices_v_f.size(); i++)
		{

			vertices.push_back(vertices_v[indices_v_f[i]].x);
			vertices.push_back(vertices_v[indices_v_f[i]].y);
			vertices.push_back(vertices_v[indices_v_f[i]].z);

			vertices.push_back(vertices_vn[indices_vn_f[i]].x);
			vertices.push_back(vertices_vn[indices_vn_f[i]].y);
			vertices.push_back(vertices_vn[indices_vn_f[i]].z);

			vertices.push_back(vertices_vt[indices_vt_f[i]].x);
			vertices.push_back(vertices_vt[indices_vt_f[i]].y);

			//mesh.vertices_pos.push_back(vertices_v[indices_v_f[i]]);
			//mesh.vertices_norm.push_back(vertices_vn[indices_vn_f[i]]);
			//mesh.vertices_tex.push_back(vertices_vt[indices_vt_f[i]]);

		}
	
	}

	void ReadFileMtl()
	{

		filemtl.open(file_mtl_path + "/" + file_mtl_name);
		check_file_open(filemtl);

		int count_mat = 0,tmp=-1;
		int sum = -1;

		while (!filemtl.eof())
		{

			str = "";
			getline(filemtl, str);
			int found;
			found = str.find("newmtl");

			if (found != string::npos)
			{
				if (tmp == count_mat)
				{
					material.push_back(mat);

					mat.type_parameter_int.clear();
					mat.type_parameter_vec3.clear();
					mat.value_parameter_int.clear();
					mat.value_parameter_vec3.clear();
					mat.texture_map.clear();
					mat.texture_id.clear();
				}
				sum++;
				count_mat++;
				tmp = count_mat;
				mat.material_name = str.substr(7);
			}

			if (filemtl.eof())
			{
				material.push_back(mat);
			}
			if (filemtl.eof() && material.size()==0)
			{
				material.push_back(mat);
			}

			string s = str.substr(0, 2);
			if (s == "Ns" || s == "d " || s == "Ni")
			{
				float value = stof(str.substr(2));
				mat.value_parameter_int.push_back(value);
				if (s == "d ") s = "d";
				mat.type_parameter_int.push_back(s);
			}
			else if (s == "Ka" || s == "Kd" || s == "Ks" || s == "Ke")
			{
				sscanf_s(str.c_str(), "%*c%*c%*c%f%f%f", &tmp_vert_x, &tmp_vert_y, &tmp_vert_z);
				if (glm::vec3(tmp_vert_x, tmp_vert_y, tmp_vert_z) != glm::vec3(0.0f))
				{
					mat.value_parameter_vec3.push_back(glm::vec3(tmp_vert_x,tmp_vert_y,tmp_vert_z));
					mat.type_parameter_vec3.push_back(s);
				}
				else {
					mat.value_parameter_vec3.push_back(glm::vec3(1.0f));
					mat.type_parameter_vec3.push_back(s);
				}
				
			}
			
			found = str.find("map_");
			if (found != string::npos)
			{
				for (int i = 0; i < maps_types.size(); i++)
				{
					found = str.find(maps_types[i]);
					if (found != string::npos)
					{
						found = str.find_last_of("\\");
						int found1 = str.find_last_of("/");
						if (found != string::npos || found1 != string::npos)
						{
							picture_name = str.substr(maps_types[i].size() + 1);
							picture_name = fss::path(picture_name).filename().string();
							//mat.texture_map.push_back(picture_name);
							mat.texture_map.push_back(maps_types[i]);
							mat.texture_id.push_back(loadTexture());
						}
						else if(str.substr(maps_types[i].size() + 1).size() > 4 && found == string::npos||found1 == string::npos)
						{
							picture_name = str.substr(maps_types[i].size() + 1);
							//mat.texture_map.push_back(picture_name);
							mat.texture_map.push_back(maps_types[i]);
							mat.texture_id.push_back(loadTexture());
						}
						
						
					}
				}
			}
			
		}
	}

	
	void Draw(Shader shader1)
	{
		shader1.setBool("tex_valid", false);
		int offset = 0;
		for (int i = 0; i < verts.size(); i++)
		{
			for (int j = 0, k = 0; j < material.size(); j++)
			{
				if (material[j].material_name == usemtl_names[i]) // òóò áåäà ÁÎËÜØÎÉ ÊÎÑßÊ, ÒÛ ÏÐÎÏÓÑÊÀÅØÜ ÌÍÎÃÎ ÂÅÐÒÅÊÑÎÂ
				{
					if (material[j].texture_map.size() > 0)
					{
						shader1.setBool("tex_valid", true);

						for (int i = 0; i < material[j].type_parameter_int.size(); i++)
						{
							shader1.setFloat(material[j].type_parameter_int[i], material[j].value_parameter_int[i]);
						}
						for (int i = 0; i < material[j].type_parameter_vec3.size(); i++)
						{
							shader1.setVec3(material[j].type_parameter_vec3[i], material[j].value_parameter_vec3[i]);
						}

						for (int i = 0; i < maps_types.size(); i++)
						{
							shader1.setBool(maps_types[i] + "_valid", false);
						}

						for (int i = 0; i < material[j].texture_map.size(); i++)
						{
							glActiveTexture(GL_TEXTURE0 + k);

							shader1.setInt(material[j].texture_map[i], k);
							shader1.setBool(material[j].texture_map[i] + "_valid", true);
							//shader.setInt("texture_diffuse", k);
							glBindTexture(GL_TEXTURE_2D, material[j].texture_id[i]);

							k++;
						}
					}
					else {

						for (int i = 0; i < material[j].type_parameter_int.size(); i++)
						{

							shader1.setFloat(material[j].type_parameter_int[i], material[j].value_parameter_int[i]);
							//shader1.setFloat("color_param_f", material[j].value_parameter_int[i]);
						}
						for (int i = 0; i < material[j].type_parameter_vec3.size(); i++)
						{
							shader1.setVec3(material[j].type_parameter_vec3[i], material[j].value_parameter_vec3[i]);
							//shader1.setVec3("color_diffuse_v3", material[j].value_parameter_vec3[i]);
						}
					}
				}
			}
			glBindVertexArray(VAO[i]);

			glDrawArrays(GL_TRIANGLES, 0, verts[i].size());
			glBindVertexArray(0);
			glActiveTexture(GL_TEXTURE0);
		}
	}
	

	void Draw1(Shader shader1)
	{

		for (int j = 0, k = 0; j < material.size();j++)
		{
			if (material[j].texture_map.size() > 0)
			{
				shader1.setBool("tex_valid", true);

				for (int i = 0; i < material[j].type_parameter_int.size(); i++)
				{
					shader1.setFloat(material[j].type_parameter_int[i], material[j].value_parameter_int[i]);

				}
				for (int i = 0; i < material[j].type_parameter_vec3.size(); i++)
				{
					
					shader1.setVec3(material[j].type_parameter_vec3[i], material[j].value_parameter_vec3[i]);

				}

				for (int i = 0; i < maps_types.size(); i++)
				{
					shader1.setBool(maps_types[i] + "_valid", false);
				}

				for (int i = 0; i < material[j].texture_map.size(); i++)
				{
					
					glActiveTexture(GL_TEXTURE0 + k);

					shader1.setInt(material[j].texture_map[i], k);
					shader1.setBool(material[j].texture_map[i] + "_valid", true);
					//shader.setInt("texture_diffuse", k);
					glBindTexture(GL_TEXTURE_2D, material[j].texture_id[i]);

					k++;
				}
			}
			else {

				for (int i = 0; i < material[j].type_parameter_int.size(); i++)
				{
					
					shader1.setFloat(material[j].type_parameter_int[i], material[j].value_parameter_int[i]);
					//shader1.setFloat("color_param_f", material[j].value_parameter_int[i]);
				}
				for (int i = 0; i < material[j].type_parameter_vec3.size(); i++)
				{
					shader1.setVec3(material[j].type_parameter_vec3[i], material[j].value_parameter_vec3[i]);
					//shader1.setVec3("color_diffuse_v3", material[j].value_parameter_vec3[i]);
				}
			}

			//glBindVertexArray(cubeVAO);

			glDrawElements(GL_TRIANGLES, indices_v_f.size(), GL_UNSIGNED_INT, 0);

			//glDrawArrays(GL_TRIANGLES, 0, vertices.size());

			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);

		}
		
	}

	void closeFile()
	{
		fileobj.close();
		filemtl.close();
	}

	unsigned int loadTexture()
	{
		GLuint textureID;
		glGenTextures(1, &textureID);


		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load((file_mtl_path+"/"+picture_name).c_str(), &width, &height, &nrComponents, 0);

			if (data)
			{
				GLenum format;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;


				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
				stbi_image_free(data);
			}

		return textureID;
	}

private:
	vector<unsigned int> VAO, VBO;
	//unsigned int cubeVAO, cubeVBO, EBO,colorVBO;
	int size_mat = 0;

	void check_file_open(ifstream &file)
	{
		if (!file.is_open())
		{
			cout << "File is not opened.\n";
		}
		else {
			cout << "File is opened.\n";
		}
		
	}
	
	void setupVertices()
	{
		VAO.resize(verts.size());
		VBO.resize(verts.size());
		glGenVertexArrays(verts.size(), &VAO[0]);
		glGenBuffers(verts.size(), &VBO[0]);
		//glGenBuffers(1, &EBO);

		for (int i = 0; i < verts.size(); i++)
		{
			glBindVertexArray(VAO[i]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
			glBufferData(GL_ARRAY_BUFFER, verts[i].size() * sizeof(float), &verts[i][0], GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);

		}
		/*glBindVertexArray(cubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

		//glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_v.size()*sizeof(glm::vec3), &vertices_v[0]);
		//glBufferSubData(GL_ARRAY_BUFFER, vertices_v.size() * sizeof(glm::vec3), vertices_vn.size()*sizeof(glm::vec3), &vertices_vn[0]);
		//glBufferSubData(GL_ARRAY_BUFFER, (vertices_v.size()+vertices_vn.size()) * sizeof(glm::vec3), vertices_vt.size()*sizeof(glm::vec3), &vertices_vt[0]);


		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0][0], GL_STATIC_DRAW);

		//glBufferData(GL_ARRAY_BUFFER, meshes.size() * sizeof(Meshes), &meshes[0], GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_v_f.size() * sizeof(unsigned int), &indices_v_f[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		*/
	}

	/*glm::vec3 normalize_vec3(glm::vec3 vf_vec)
	{
		glm::vec3 tmp_vec;
		float length = sqrt((vf_vec.x*vf_vec.x) + (vf_vec.y * vf_vec.y) + (vf_vec.z * vf_vec.z));
		if (vf_vec.x < 0)
			tmp_vec = glm::vec3(-(vf_vec.x / length), (vf_vec.y / length), (vf_vec.z / length));
		else if (vf_vec.y < 0)
			tmp_vec = glm::vec3((vf_vec.x / length), -(vf_vec.y / length), (vf_vec.z / length));
		else if (vf_vec.z < 0)
			tmp_vec = glm::vec3((vf_vec.x / length), (vf_vec.y / length), -(vf_vec.z / length));
		else
			tmp_vec = glm::vec3((vf_vec.x / length), (vf_vec.y / length), (vf_vec.z / length));

		return tmp_vec;
	}*/

};

#endif exampleLoader_H