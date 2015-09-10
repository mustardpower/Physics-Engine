#include <Windows.h>
#include <WinGDI.h>
#include "GeoModel3D.h"

GLuint GeoModel3D::NUMBER_OF_MODELS = 0;
GLuint GLModel3DData::NUMBER_OF_MESHES = 0;
GeoModel3D::GeoModel3D(std::string file_name)
{
	model_id = ++NUMBER_OF_MODELS;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, (file_name+".obj").c_str());
	std::cout<<err<<std::endl;

	for(size_t i = 0;i<shapes.size();i++ )
	{
		GLModel3DData new_mesh;
		GLuint texture;

		// each mesh is made up of a number of shapes	
		tinyobj::mesh_t current_mesh = shapes[i].mesh;

		if(current_mesh.material_ids[0]>=0) // no texture = -1 in material ids
		{
            tinyobj::material_t& material = materials[current_mesh.material_ids[0]];
			std::string color_map = material.diffuse_texname;
			if (!color_map.empty()) 
			{
				std::cout<<"Loading texture: "<<color_map<<std::endl;
				texture = loadTexture(color_map);
				new_mesh.addTexture(texture);
			}
			else
			{
				std::cout<<"Could not find a diffuse texture!"<<std::endl;
			}
        }

		new_mesh.addMeshData(current_mesh);
		meshes.push_back(new_mesh);
	}

	if(err.empty())
	{
		std::cout << "# of shapes    : " << shapes.size() << std::endl;
		std::cout << "# of materials : " << materials.size() << std::endl;
	}
}

GLuint GeoModel3D::getModelID()
{
	return model_id;
}
std::vector<GLModel3DData> GeoModel3D::retrieveMeshes()
{
	return meshes;
}

GLuint GeoModel3D::loadTexture (std::string file_name)
{
	if(!wglGetCurrentContext())
	{
		printf("No OpenGL context\n");
	}

	glEnable(GL_TEXTURE_2D);

	GLuint tex_id = SOIL_load_OGL_texture(
					file_name.c_str(),
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					NULL
					);

    if( 0 == tex_id )
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result());
    }

	return tex_id;
}