#pragma once

#include <glmlv/filesystem.hpp>
#include <glmlv/GLFWHandle.hpp>
#include <glmlv/GLProgram.hpp>
#include <glmlv/simple_geometry.hpp>
#include <glmlv/ViewController.hpp>
#include <glmlv/Image2DRGBA.hpp>

class Application
{

	struct Mesh {
		GLuint VBO;
		GLuint IBO;
		GLuint nbIndices;
		GLuint VAO;
		glm::vec3 materialKd;

		Mesh(GLuint VBO, GLuint IBO, GLuint nbIndices, GLuint VAO, glm::vec3 materialKd) :
			VBO(VBO), IBO(IBO), nbIndices(nbIndices), VAO(VAO), materialKd(materialKd) {
		}
	};

public:
    Application(int argc, char** argv);

    ~Application();

    int run();
private:
    const size_t m_nWindowWidth = 1280;
    const size_t m_nWindowHeight = 720;
    glmlv::GLFWHandle m_GLFWHandle{ (int) m_nWindowWidth, (int) m_nWindowHeight, "Template" }; // Note: the handle must be declared before the creation of any object managing OpenGL resource (e.g. GLProgram, GLShader)

	/* Paths const */
    const glmlv::fs::path m_AppPath;
    const std::string m_AppName;
    const std::string m_ImGuiIniFilename;
    const glmlv::fs::path m_ShadersRootPath;
	const glmlv::fs::path m_AssetsRootPath;

	/* Attribute locations const */
	const GLint POSITION_ATTR_LOCATION = 0;
	const GLint NORMAL_ATTR_LOCATION = 1;
	const GLint TEXCOORDS_ATTR_LOCATION = 2;

	/* Transform matrix */
    GLint MVPMatrixLoc;
    GLint MVMatrixLoc;
    GLint NormalMatrixLoc;


	/* Shader Program */
	glmlv::GLProgram m_program;


	/* Lighting variables */
	/* Directional Light */
	GLint m_uDirectionalLight_DirectionLocation;
	GLint m_uDirectionalLight_IntensityLocation;

	glm::vec3 m_directionalLight_Direction;
	GLfloat m_directionalLight_Intensity;
	glm::vec3 m_directionalLight_Color;

	/* Point Light */
	GLint m_uPointLight_PositionLocation;
	GLint m_uPointLight_IntensityLocation;

	glm::vec3 m_PointLight_Position;
	GLfloat m_PointLight_Intensity;
	glm::vec3 m_PointLight_Color;
	

	/* Material variables */
	GLint m_uKdLocation;

	
	/* Camera */
    glmlv::ViewController m_viewController;


	std::vector<Mesh> m_meshes;

};