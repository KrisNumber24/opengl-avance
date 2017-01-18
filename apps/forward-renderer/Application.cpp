#include "Application.hpp"

#include <iostream>

#include <imgui.h>
#include <glmlv/imgui_impl_glfw_gl3.hpp>

#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

int Application::run()
{
    float clearColor[3] = { 0, 0, 0 };
    // Loop until the user closes the window
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
        const auto seconds = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Put here rendering code

        const auto ProjMatrix = glm::perspective(
            70.f,
            (float) m_nWindowWidth / m_nWindowHeight,
            0.01f,
            100.0f);

        const auto ViewMatrix = this->m_viewController.getViewMatrix();

        glm::mat4 MVMatrix;
        glm::mat4 MVPMatrix;
        glm::mat4 NormalMatrix;

		/* Define light variables */
		/* I use default values define in .hpp */

		/* I have just to do these operations to convert light dir in viewSpace */
		/*glUniform3fv(this->directionalLightDirLoc,
			1,
			glm::value_ptr(glm::vec3(glm::vec4(glm::normalize(this->u_directionalLightDir), 0))));

		glUniform3fv(this->directionalLightIntensityLoc,
			1,
			glm::value_ptr(glm::vec3(this->u_directionalLightIntensity)));

		glUniform3fv(this->pointLightPositionLoc,
			1,
			glm::value_ptr(glm::vec3(glm::vec4(this->u_pointLightPosition, 1))));

		glUniform3fv(this->pointLightIntensityLoc,
			1,
			glm::value_ptr(glm::vec3(this->u_pointLightIntensity)));*/

		{
			/* First, draw the cube */
			/*this->m_cubeColor = glm::vec3(1, 1, 1);

			glActiveTexture(GL_TEXTURE0);
			glUniform1i(m_uCubeSamplerLoc, 0);
			glBindSampler(0, m_cubeSampler);
			

			glm::mat4 cubeMatrix = glm::translate(glm::mat4(1), glm::vec3(0.f, 0.f, -5.f));

			MVMatrix = ViewMatrix * cubeMatrix;
			MVPMatrix = ProjMatrix * MVMatrix;
			NormalMatrix = glm::transpose(glm::inverse(MVMatrix));*/

			/* Send light to fragment shader */

			//glUniformMatrix4fv(this->MVMatrixLoc,
			//	1,
			//	GL_FALSE,
			//	glm::value_ptr(MVMatrix));
			//glUniformMatrix4fv(this->MVPMatrixLoc,
			//	1,
			//	GL_FALSE,
			//	glm::value_ptr(MVPMatrix));
			//glUniformMatrix4fv(this->NormalMatrixLoc,
			//	1,
			//	GL_FALSE,
			//	glm::value_ptr(NormalMatrix));

			//glUniform3fv(this->kdLoc,
			//	1,
			//	glm::value_ptr(this->m_cubeColor));

			//glBindTexture(GL_TEXTURE_2D, m_cubeTex);
			//glBindVertexArray(this->m_cubeVAO);

			//glDrawElements(
			//	GL_TRIANGLES,
			//	this->m_nbCubeIndex,
			//	GL_UNSIGNED_INT,
			//	nullptr);

			//glBindVertexArray(0);
		}
		
		{
			/* Then, draw a sphere ! */

			//this->m_sphereColor = glm::vec3(1, 1, 1);

			//glActiveTexture(GL_TEXTURE0);
			//glUniform1i(m_uCubeSamplerLoc, 0);
			//glBindSampler(0, m_sphereSampler);
			//glBindTexture(GL_TEXTURE_2D, m_sphereTex);

			//glm::mat4 sphereMatrix = glm::translate(glm::mat4(1), glm::vec3(-1.f, 0.f, -10.f));

			//MVMatrix = ViewMatrix * sphereMatrix;

			//NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

			//glUniformMatrix4fv(this->MVMatrixLoc,
			//	1,
			//	GL_FALSE,
			//	glm::value_ptr(MVMatrix));
			//glUniformMatrix4fv(this->MVPMatrixLoc,
			//	1,
			//	GL_FALSE,
			//	glm::value_ptr(ProjMatrix * MVMatrix));
			//glUniformMatrix4fv(this->NormalMatrixLoc,
			//	1,
			//	GL_FALSE,
			//	glm::value_ptr(NormalMatrix));

			/* Send light to fragment shader */

			//glUniform3fv(this->kdLoc,
			//	1,
			//	glm::value_ptr(this->m_sphereColor));

			//glBindVertexArray(this->m_sphereVAO);

			//glDrawElements(
			//	GL_TRIANGLES,
			//	this->m_nbSphereIndex,
			//	GL_UNSIGNED_INT,
			//	nullptr);

			//glBindVertexArray(0);
		}        

        // GUI code:
        ImGui_ImplGlfwGL3_NewFrame();

        {
            ImGui::Begin("GUI");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::ColorEditMode(ImGuiColorEditMode_RGB);
            if (ImGui::ColorEdit3("clearColor", clearColor)) {
                glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.f);
            }
			/*ImGui::Text("Directional Light");
			float dirVal[3] = { this->u_directionalLightDir.x, this->u_directionalLightDir.y, this->u_directionalLightDir.z};
			if (ImGui::SliderFloat3("Direction", dirVal, -1.f, 1.f)) {
				this->u_directionalLightDir = glm::vec3(dirVal[0], dirVal[1], dirVal[2]);
			}*/
            ImGui::End();
        }

        const auto viewportSize = m_GLFWHandle.framebufferSize();
        glViewport(0, 0, viewportSize.x, viewportSize.y);
        ImGui::Render();

        /* Poll for and process events */
        glfwPollEvents();

        /* Swap front and back buffers*/
        m_GLFWHandle.swapBuffers();

        auto ellapsedTime = glfwGetTime() - seconds;
        auto guiHasFocus = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
        if (!guiHasFocus) {
            this->m_viewController.update(float(ellapsedTime));
        }
    }

    return 0;
}

Application::Application(int argc, char** argv):
    m_AppPath { glmlv::fs::path{ argv[0] } },
    m_AppName { m_AppPath.stem().string() },
    m_ImGuiIniFilename { m_AppName + ".imgui.ini" },
    m_ShadersRootPath { m_AppPath.parent_path() / "shaders" },
    m_viewController { glmlv::ViewController(this->m_GLFWHandle.window()) },
	m_AssetsRootPath {	m_AppPath.parent_path() / "assets" }

{
    this->m_program = glmlv::compileProgram(
        { this->m_ShadersRootPath / "forward-renderer" / "forward.vs.glsl", 
          this->m_ShadersRootPath / "forward-renderer" / "forward.fs.glsl" });

	/* Get matrix locations */
    this->MVPMatrixLoc = this->m_program.getUniformLocation("uModelViewProjMatrix");
    this->MVMatrixLoc = this->m_program.getUniformLocation("uModelViewMatrix");
    this->NormalMatrixLoc = this->m_program.getUniformLocation("uNormalMatrix");

	/* Get lighting component locations */
	m_uDirectionalLight_DirectionLocation =	m_program.getUniformLocation("uDirectionalLightDir");
	m_uDirectionalLight_IntensityLocation = m_program.getUniformLocation("uDirectionnalLightIntensity");

	m_uPointLight_PositionLocation =	m_program.getUniformLocation("uPointLightPosition");
	m_uPointLight_IntensityLocation =	m_program.getUniformLocation("uPointLightIntensity");

	m_uKdLocation = m_program.getUniformLocation("uKd");


	/* Load obj file */
	auto inputFile = m_AssetsRootPath / m_AppName / "models" / "mustang" / "obj.obj";
	std::string inputFileStr = inputFile.generic_string();
	
	tinyobj::attrib_t attrib; // Contains vertices (pos), normals and texcoords
	std::vector<tinyobj::shape_t> shapes; // Contains all of the separate objects and their faces
	std::vector<tinyobj::material_t> materials;

	std::string loadObjError;

	auto loadObjSuccess = tinyobj::LoadObj(&attrib, &shapes, &materials, &loadObjError, inputFileStr.c_str());
	
	if (!loadObjError.empty()) {
		std::cerr << loadObjError << std::endl;
	}

	if (!loadObjSuccess) {
		exit(1);
	}

	/* Go fill buffers ! o/ */

	std::vector<glmlv::Vertex3f3f2f> vertices;
	std::vector<int> indices;

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		
		glm::vec3 kD(1, 1, 1);

		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				float nx = attrib.normals[3 * idx.normal_index + 0];
				float ny = attrib.normals[3 * idx.normal_index + 1];
				float nz = attrib.normals[3 * idx.normal_index + 2];
				float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				float ty = attrib.texcoords[2 * idx.texcoord_index + 1];

				vertices.push_back(
					glmlv::Vertex3f3f2f(
						glm::vec3(vx, vy, vz),
						glm::vec3(nx, ny, nz),
						glm::vec2(tx, ty)
					)
				);

				indices.push_back((int) index_offset + v);
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}

		// Set VBO
		GLuint currentMeshVBO;

		glGenBuffers(1, &currentMeshVBO);

		glBindBuffer(GL_ARRAY_BUFFER, currentMeshVBO);

			glBufferStorage(
				GL_ARRAY_BUFFER,
				vertices.size() * sizeof(glmlv::Vertex3f3f2f),
				vertices.data(),
				0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Set IBO
		GLuint currentMeshIBO;

		glGenBuffers(1, &currentMeshIBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMeshIBO);

			glBufferStorage(
				GL_ELEMENT_ARRAY_BUFFER,
				indices.size() * sizeof(uint32_t),
				indices.data(),
				0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Set VAO
		GLuint currentMeshVAO;

		glGenVertexArrays(1, &currentMeshVAO);

		glBindVertexArray(currentMeshVAO);

			glBindBuffer(GL_ARRAY_BUFFER, currentMeshVBO);

				glEnableVertexAttribArray(POSITION_ATTR_LOCATION);
				glVertexAttribPointer(
					POSITION_ATTR_LOCATION,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(glmlv::Vertex3f3f2f),
					(const GLvoid*)offsetof(glmlv::Vertex3f3f2f, position));

				glEnableVertexAttribArray(NORMAL_ATTR_LOCATION);
				glVertexAttribPointer(
					NORMAL_ATTR_LOCATION,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(glmlv::Vertex3f3f2f),
					(const GLvoid*)offsetof(glmlv::Vertex3f3f2f, normal));

				glEnableVertexAttribArray(TEXCOORDS_ATTR_LOCATION);
				glVertexAttribPointer(
					TEXCOORDS_ATTR_LOCATION,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(glmlv::Vertex3f3f2f),
					(const GLvoid*)offsetof(glmlv::Vertex3f3f2f, texCoords));

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMeshIBO);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		m_meshes.push_back(Mesh(currentMeshVBO,
			currentMeshIBO,
			indices.size(),
			currentMeshVAO,
			kD
		));


		this->m_program.use();

		glEnable(GL_DEPTH_TEST);

	}
	
	/* Define textures */
	/*auto cubeTexImage = glmlv::readImage(m_AssetsRootPath / m_AppName / "textures" / "pika.png");
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_cubeTex);
	glBindTexture(GL_TEXTURE_2D, m_cubeTex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, cubeTexImage.width(), cubeTexImage.height());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, cubeTexImage.width(), cubeTexImage.height(), GL_RGBA, GL_UNSIGNED_BYTE, cubeTexImage.data());
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenSamplers(1, &m_cubeSampler);
	glSamplerParameteri(m_cubeSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(m_cubeSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

	//m_uCubeSamplerLoc = m_program.getUniformLocation("uKdSampler");

    ImGui::GetIO().IniFilename = m_ImGuiIniFilename.c_str(); // At exit, ImGUI will store its windows positions in this file
}

Application::~Application() {
    

    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
}