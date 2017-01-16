#pragma once

#include <glmlv/filesystem.hpp>
#include <glmlv/GLFWHandle.hpp>
#include <glmlv/GLProgram.hpp>
#include <glmlv/simple_geometry.hpp>
#include <glmlv/ViewController.hpp>
#include <glmlv/Image2DRGBA.hpp>

class Application
{
public:
    Application(int argc, char** argv);

    ~Application();

    int run();
private:
    const size_t m_nWindowWidth = 1280;
    const size_t m_nWindowHeight = 720;
    glmlv::GLFWHandle m_GLFWHandle{ m_nWindowWidth, m_nWindowHeight, "Template" }; // Note: the handle must be declared before the creation of any object managing OpenGL resource (e.g. GLProgram, GLShader)

    const glmlv::fs::path m_AppPath;
    const std::string m_AppName;
    const std::string m_ImGuiIniFilename;
    const glmlv::fs::path m_ShadersRootPath;
	const glmlv::fs::path m_AssetsRootPath;

	/* Transform matrix */
    GLint MVPMatrixLoc;
    GLint MVMatrixLoc;
    GLint NormalMatrixLoc;

	/* Lighting variables */
	GLint directionalLightDirLoc;
	GLint directionalLightIntensityLoc;
	glm::vec3 u_directionalLightDir = glm::vec3(0.f, 0.f, 1.f);
	float u_directionalLightIntensity = 1.f;
	
	GLint pointLightPositionLoc;
	GLint pointLightIntensityLoc;
	glm::vec3 u_pointLightPosition = glm::vec3(0.f, 0.f, 0.f);
	float u_pointLightIntensity = 1.f;

	GLint kdLoc;

    GLuint m_cubeVBO = 0;
    GLuint m_cubeIBO = 0;
    GLuint m_cubeVAO = 0;
    GLuint m_nbCubeIndex = 0;
	glm::vec3 m_cubeColor = glm::vec3(0, 0, 0);
	GLuint m_cubeTex;
	GLuint m_cubeSampler;
	GLint m_uCubeSamplerLoc;

    GLuint m_sphereVBO = 0;
    GLuint m_sphereIBO = 0;
    GLuint m_sphereVAO = 0;
    GLuint m_nbSphereIndex = 0;
	glm::vec3 m_sphereColor = glm::vec3(0, 0, 0);
	GLuint m_sphereTex;
	GLuint m_sphereSampler;
	GLint m_uSphereSamplerLoc;

    glmlv::ViewController m_viewController;

    glmlv::GLProgram m_program;

};