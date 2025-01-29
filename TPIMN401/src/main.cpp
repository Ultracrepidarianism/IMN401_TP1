#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
#include <glm.hpp>
#include <gtc/constants.hpp>
#include "utils.hpp"
#include <chrono>

namespace IMN401 {

    void printProgramError(GLuint program)
    {
        GLuint isLinked;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            std::cout << "Error : " + std::string(infoLog.begin(), infoLog.end()) + "\n";

            // We don't need the program anymore.
            glDeleteProgram(program);
            return;
        }
        else
            std::cout << "Shader compilation : OK" << std::endl;


    }

    void printPipelineError(GLuint pipeline)
    {
        GLuint isValid;
        glGetProgramPipelineiv(pipeline, GL_VALIDATE_STATUS, (int*)&isValid);
        if (isValid == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramPipelineiv(pipeline, GL_INFO_LOG_LENGTH, &maxLength);
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramPipelineInfoLog(pipeline, maxLength, &maxLength, &infoLog[0]);

            std::cout << "Error : " + std::string(infoLog.begin(), infoLog.end()) + "\n";
            // We don't need the program anymore.
            glDeleteProgram(pipeline);
            return;
        }
        else
            std::cout << "Pipeline : OK" << std::endl;

    }


    int main()
    {
        // Init GLFW
        glfwInit();

        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return EXIT_FAILURE;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        const int width = 800;
        const int height = 800;

        GLFWwindow* window = glfwCreateWindow(width, height, "TP - From Scratch", NULL, NULL);
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, NULL);


        // Load all OpenGL functions using the glfw loader function
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize OpenGL context" << std::endl;
            glfwTerminate();
            return EXIT_FAILURE;
        }


        // shaders
        std::string strVS = readFile("shaders/triangle-vs.glsl");
        const GLchar* vsCode = strVS.c_str();
        std::string strFS = readFile("shaders/triangle-fs.glsl");
        const GLchar* fsCode = strFS.c_str();


        



        // Initialization
        // =====================
        // TODO: init buffers, shaders, etc.
        // cf. https://www.khronos.org/files/opengl46-quick-reference-card.pdf

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);



        std::vector<glm::vec3> sommets;
		sommets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));                                                                 // On défini le point central qui sera utilisé dans tous les triangles et son index de sommet sera 0
        const float rayon = 0.5f;
        const int nbPoints = 30;

        for (int i = 0; i < nbPoints; ++i) {
            float angle = 2.0f * glm::pi<float>() * i / nbPoints;                                                       // On sépare 2pi (360) en 16 pour trouver l'angle lié au centre de chaque triangle
			float x = rayon * cos(angle);															                    // On trouve les coordonnées x et y de chaque point rayon * cos(angle) et rayon * sin(angle)
            float y = rayon * sin(angle);
            sommets.push_back(glm::vec3(x, y, 0.0f));
        }

        GLuint vertexBuffer;
		glCreateBuffers(1, &vertexBuffer);    							                                                // Créer un buffer
		glNamedBufferData(vertexBuffer, sizeof(glm::vec3) * sommets.size(), sommets.data(), GL_STATIC_DRAW);            // Insérer les sommets dans le buffer directement

        std::vector<glm::ivec3> indices;
       
        for (int i = 1; i <= nbPoints; i++) {
			indices.push_back(glm::ivec3(0, i,(i % nbPoints) + 1));
        }
		//indices.push_back(glm::ivec3(0, 1, nbPoints));																	// Tableau des faces, on crée tous nos triangles

        GLuint EBO_indices;
        glCreateBuffers(1, &EBO_indices);
        glNamedBufferData(EBO_indices, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);


        GLuint vertexArray;
        glCreateVertexArrays(1, &vertexArray);                                                                          // Créer le vertex array
        glVertexArrayAttribFormat(vertexArray, 0, 3, GL_FLOAT, GL_FALSE, 0);                                            // Spécifier qu'on s'attend à 3 floats sur le channel 0
		glEnableVertexArrayAttrib(vertexArray, 0);                                                                      // Activer le channel 0
        glVertexArrayVertexBuffer(vertexArray, 0, vertexBuffer, 0, sizeof(glm::vec3));                                  // Spécifier le Vertex Buffer Object à lire et la taille qu'il aura pour le channel 0
		glVertexArrayAttribBinding(vertexArray, 0, 0);  														        // Lier le channel 0 au buffer 0      
		glVertexArrayElementBuffer(vertexArray, EBO_indices);   											            // Lier l'array des indices au vertex array   

        GLuint vertexShaderProgram = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vsCode);
        GLuint fragmentShaderProgram = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fsCode);

        GLuint pipeline;
        glCreateProgramPipelines(1, &pipeline);
        glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertexShaderProgram);
        glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragmentShaderProgram);

        if (glGetError() != GL_NO_ERROR) {
            std::cerr << "OpenGL error" << std::endl;
            return EXIT_FAILURE;
        }




        GLuint vShaderTimeAddr = glGetUniformLocation(vertexShaderProgram, "time");


        // Rendering loop
        while (!glfwWindowShouldClose(window))
        {
            // Handle events
            glfwPollEvents();

            // ==================
            // TODO: render here !
            glClear(GL_COLOR_BUFFER_BIT);
            glProgramUniform1f(vertexShaderProgram, vShaderTimeAddr, clock() / 500.0f);                                 // Plus vite ou lent dépendant de la division effectuée
            glBindProgramPipeline(pipeline);
            glBindVertexArray(vertexArray);

            glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);                                       
            // ==================

            glfwSwapBuffers(window);
        }

        // Clean up
        glfwTerminate();

        return EXIT_SUCCESS;
    }
}

int main()
{
    return IMN401::main();
}