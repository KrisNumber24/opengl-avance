#include "Application.hpp"

#include <iostream>

#include <imgui.h>
#include <glmlv/imgui_impl_glfw_gl3.hpp>

#include <glm/gtc/type_ptr.hpp>

int Application::run()
{
    float clearColor[3] = { 0, 0, 0 };
    // Loop until the user closes the window
    for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose(); ++iterationCount)
    {
        const auto seconds = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Put here rendering code

        glm::mat4 ProjMatrix = glm::perspective(
            70.f,
            (float) m_nWindowWidth / m_nWindowHeight,
            0.1f,
            100.0f);
        glm::mat4 ViewMatrix = this->m_viewController.getViewMatrix();



        glm::mat4 MVMatrix;
        glm::mat4 MVPMatrix;
        glm::mat4 NormalMatrix;

        glm::mat4 cubeMatrix = glm::translate(glm::mat4(1), glm::vec3(0.f, 0.f, -5.f));

        MVMatrix = ViewMatrix * cubeMatrix;
        MVPMatrix = ProjMatrix * MVMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(this->MVMatrixLoc, 
            1, 
            GL_FALSE,
            glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(this->MVPMatrixLoc, 
            1, 
            GL_FALSE,
            glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(this->NormalMatrixLoc, 
            1, 
            GL_FALSE,
            glm::value_ptr(NormalMatrix));


        glBindVertexArray(this->m_cubeVAO);

            glDrawElements(
            GL_TRIANGLES, 
            this->m_nbCubeIndex,
            GL_UNSIGNED_INT,
            nullptr);

        glBindVertexArray(0);

        glm::mat4 sphereMatrix = glm::translate(glm::mat4(1), glm::vec3(-1.f, 0.f, -10.f));
        
        MVMatrix = ViewMatrix * sphereMatrix;

        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(this->MVMatrixLoc, 
            1, 
            GL_FALSE,
            glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(this->MVPMatrixLoc, 
            1, 
            GL_FALSE,
            glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(this->NormalMatrixLoc, 
            1, 
            GL_FALSE,
            glm::value_ptr(NormalMatrix));

        glBindVertexArray(this->m_sphereVAO);

            glDrawElements(
            GL_TRIANGLES, 
            this->m_nbSphereIndex,
            GL_UNSIGNED_INT,
            nullptr);

        glBindVertexArray(0);
        

        // GUI code:
        ImGui_ImplGlfwGL3_NewFrame();

        {
            ImGui::Begin("GUI");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::ColorEditMode(ImGuiColorEditMode_RGB);
            if (ImGui::ColorEdit3("clearColor", clearColor)) {
                glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.f);
            }
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
    m_viewController { glmlv::ViewController(this->m_GLFWHandle.window()) }

{
    this->m_program = glmlv::compileProgram(
        { this->m_ShadersRootPath / "forward-renderer" / "forward.vs.glsl", 
          this->m_ShadersRootPath / "forward-renderer" / "forward.fs.glsl" });

    const GLint POSITION_ATTR_LOCATION = 0;
    const GLint NORMAL_ATTR_LOCATION = 1;
    const GLint TEX_ATTR_LOCATION = 2;

    this->MVPMatrixLoc = this->m_program.getUniformLocation("uModelViewProjMatrix");
    this->MVMatrixLoc = this->m_program.getUniformLocation("uModelViewMatrix");
    this->NormalMatrixLoc = this->m_program.getUniformLocation("uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    // First create a cube ! Yay ! o/

    auto cubeGeometry = glmlv::makeCube();
    this->m_nbCubeIndex = cubeGeometry.indexBuffer.size();

    // Set VBO
    glGenBuffers(1, &(this->m_cubeVBO));

    glBindBuffer(GL_ARRAY_BUFFER, this->m_cubeVBO);
    
        glBufferStorage(
            GL_ARRAY_BUFFER, 
            cubeGeometry.vertexBuffer.size() * sizeof(glmlv::Vertex3f3f2f), 
            cubeGeometry.vertexBuffer.data(),
            0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    // Set IBO
    glGenBuffers(1, &(this->m_cubeIBO));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_cubeIBO);

        glBufferStorage(
            GL_ELEMENT_ARRAY_BUFFER,
            cubeGeometry.indexBuffer.size() * sizeof(uint32_t),
            cubeGeometry.indexBuffer.data(),
            0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    // Set VAO
    glGenVertexArrays(1, &(this->m_cubeVAO));

    glBindVertexArray(this->m_cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->m_cubeVBO);

            glEnableVertexAttribArray(POSITION_ATTR_LOCATION);
            glVertexAttribPointer(
                POSITION_ATTR_LOCATION, 
                3, 
                GL_FLOAT, 
                GL_FALSE, 
                sizeof(glmlv::Vertex3f3f2f),
                (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, position));

            glEnableVertexAttribArray(NORMAL_ATTR_LOCATION);
            glVertexAttribPointer(
                NORMAL_ATTR_LOCATION,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glmlv::Vertex3f3f2f),
                (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, normal));

            glEnableVertexAttribArray(TEX_ATTR_LOCATION);
            glVertexAttribPointer(
                TEX_ATTR_LOCATION,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glmlv::Vertex3f3f2f),
                (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, texCoords));

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_cubeIBO);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    // Then create a sphere ! Yay ! o/
    auto sphereGeometry = glmlv::makeSphere(32);
    this->m_nbSphereIndex = sphereGeometry.indexBuffer.size();

    // Set VBO
    glGenBuffers(1, &(this->m_sphereVBO));

    glBindBuffer(GL_ARRAY_BUFFER, this->m_sphereVBO);
    
        glBufferStorage(
            GL_ARRAY_BUFFER, 
            sphereGeometry.vertexBuffer.size() * sizeof(glmlv::Vertex3f3f2f), 
            sphereGeometry.vertexBuffer.data(),
            0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    // Set IBO
    glGenBuffers(1, &(this->m_sphereIBO));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_sphereIBO);

        glBufferStorage(
            GL_ELEMENT_ARRAY_BUFFER,
            sphereGeometry.indexBuffer.size() * sizeof(uint32_t),
            sphereGeometry.indexBuffer.data(),
            0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    // Set VAO
    glGenVertexArrays(1, &(this->m_sphereVAO));

    glBindVertexArray(this->m_sphereVAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->m_sphereVBO);

            glEnableVertexAttribArray(POSITION_ATTR_LOCATION);
            glVertexAttribPointer(
                POSITION_ATTR_LOCATION, 
                3, 
                GL_FLOAT, 
                GL_FALSE, 
                sizeof(glmlv::Vertex3f3f2f),
                (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, position));

            glEnableVertexAttribArray(NORMAL_ATTR_LOCATION);
            glVertexAttribPointer(
                NORMAL_ATTR_LOCATION,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glmlv::Vertex3f3f2f),
                (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, normal));

            glEnableVertexAttribArray(TEX_ATTR_LOCATION);
            glVertexAttribPointer(
                TEX_ATTR_LOCATION,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glmlv::Vertex3f3f2f),
                (const GLvoid*) offsetof(glmlv::Vertex3f3f2f, texCoords));

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_sphereIBO);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    this->m_program.use();

    ImGui::GetIO().IniFilename = m_ImGuiIniFilename.c_str(); // At exit, ImGUI will store its windows positions in this file
}

Application::~Application() {
    if (m_cubeVBO) {
        glDeleteBuffers(1, &m_cubeVBO);
    }

    if (m_cubeIBO) {
        glDeleteBuffers(1, &m_cubeIBO);
    }

    if (m_cubeVAO) {
        glDeleteBuffers(1, &m_cubeVAO);
    }

    if (m_sphereVBO) {
        glDeleteBuffers(1, &m_sphereVBO);
    }

    if (m_sphereIBO) {
        glDeleteBuffers(1, &m_sphereIBO);
    }

    if (m_sphereVAO) {
        glDeleteBuffers(1, &m_sphereVAO);
    }

    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
}