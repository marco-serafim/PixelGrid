#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "loader.h"
#include <math.h>

void processInput(GLFWwindow *window);


int pixels_x_and_y = 5;

// resizing function for the OpenGL window
void resizeFunction(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

// Sets the color for the vertex shader
void defColor(double R, double G, double B){
    glUniform3f(glGetUniformLocation(3, "inColor"), R, G, B);
}

// create grid of pixels (squares) on the screen
void createGrid(int pixels_x_and_y){
    glUniform1i(glGetUniformLocation(3, "pixels_each_side"), pixels_x_and_y);
    for(float j = -1; j < 1; j += 1/pixels_x_and_y){
        for(float i = -1; i < 1; i += 1/pixels_x_and_y){
            glBindVertexArray(1);
            glUniform2f(glGetUniformLocation(3, "position_of_pixel"), j, i);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);   
        }
    }
}
int main() {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL learning", NULL, NULL);  
    if(window == NULL){
        printf("Window creation failed!\n");
        glfwTerminate();  // Add glfwTerminate here
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resizeFunction);

    // check if glad is working

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad panic!\n");
        return -1;
    }
    // load the source code for shaders
    

    // ID declaratio
    unsigned int fragmentShader, vertexShader, shaderProgram;

    // creation and compilation of the vertex shader

    printf("passed");

    const char *fragmentShaderSource = loadText("shader.fs");
    const char *vertexShaderSource = loadText("shader.vs");

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for errors

    // declaration of variables used in the error checking

    char infoLog[512];
    int success;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex Shader panic! :%s", infoLog);
    }

    // creation and compilation of the fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Vertex Shader panic! :%s", infoLog);
    }

    // creation and linking of a new shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Shader Program panic! :%s", infoLog);
    }
    // delete the not used anymore shaders
    printf(fragmentShaderSource);
    printf(vertexShaderSource);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float pixel[] = {
        // positions          // colors         

        1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f, // top right
        -1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f, // bottom left
        1.0f,  -1.0f, 0.0f,   1.0f, 1.0f, 0.0f, // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // setting VBO's and VAO'S and EBO.
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // binding
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pixel), pixel, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // rendering loop
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);
        defColor (0.0f, 1.0f, 1.0f);

        glUniform1i(glGetUniformLocation(3, "pixels_each_side"), pixels_x_and_y);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
        
        int dx = 1 - pixels_x_and_y;
        for(int i = dx; i <= -dx; i += 2){
            for(int j = dx; j <= -dx; j += 2){
                glUniform2f(glGetUniformLocation(3, "position_of_pixel"), i, j);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
            }
        }

        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }      
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pixels_x_and_y++;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pixels_x_and_y--;
}

