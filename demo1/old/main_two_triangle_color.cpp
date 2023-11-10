#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height ){
    glad_glViewport(0,0,width,height);
}

//一个顶点着色器源码
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";


//一个片段着色器源码 -橙色
const char *fragmentShader1Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

//一个片段着色器源码 -黄色
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//核心模式

    GLFWwindow* window=glfwCreateWindow(800, 600, "Snow", NULL,NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Shader and Program
    // create
    unsigned int vertexShader=glad_glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderOrange=glad_glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderYellow=glad_glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramOrange=glad_glCreateProgram();
    unsigned int shaderProgramYellow=glad_glCreateProgram();

    // compile
    glad_glShaderSource(vertexShader,1,&vertexShaderSource,NULL);//第二参数指定了传递的源码字符串数量，这里只有一个。
    glad_glCompileShader(vertexShader);
    glad_glShaderSource(fragmentShaderOrange,1,&fragmentShader1Source,NULL);
    glad_glCompileShader(fragmentShaderOrange);
    glad_glShaderSource(fragmentShaderYellow,1,&fragmentShader2Source,NULL);
    glad_glCompileShader(fragmentShaderYellow);

    //link
    glad_glAttachShader(shaderProgramOrange,vertexShader);
    glad_glAttachShader(shaderProgramOrange,fragmentShaderOrange);
    glad_glLinkProgram(shaderProgramOrange);

    glad_glAttachShader(shaderProgramYellow,vertexShader);
    glad_glAttachShader(shaderProgramYellow,fragmentShaderYellow);
    glad_glLinkProgram(shaderProgramYellow);

    // set vertex data
    float firstTriangle[] = {
        0.7f,0.8f,0.0f,
        0.4f,0.3f,0.0f,
        -0.2f,-0.2f,0.0f,
    };

    float secondTriangle[] = {
        0.4f,0.3f,0.1f,
        0.3f,0.4f,0.1f,
        -0.3f,-0.3f,0.1f,
    };

    //2数组 数组对象、缓冲对象
    unsigned int VAOs[2],VBOs[2];
    glad_glGenVertexArrays(2,VAOs);
    glad_glGenBuffers(2,VBOs);

    // draw process
    // bind data
    // draw firstTriangle
    glad_glBindVertexArray(VAOs[0]);
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(firstTriangle),firstTriangle,GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);

    // draw secondTriangle
    glad_glBindVertexArray(VAOs[1]);
    glad_glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(secondTriangle),secondTriangle,GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glad_glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glad_glClearColor(0.1f,0.3f,0.4f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glad_glUseProgram(shaderProgramOrange);
        glad_glBindVertexArray(VAOs[0]);
        glad_glDrawArrays(GL_TRIANGLES,0,3);//第二个参数指定了顶点数组的起始索引，我们这里填0。最后一个参数指定我们打算绘制多少个顶点

        glad_glUseProgram(shaderProgramYellow);
        glad_glBindVertexArray(VAOs[1]);
        glad_glDrawArrays(GL_TRIANGLES,0,3);

        // glfwSwapBuffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glad_glDeleteVertexArrays(2,VAOs);
    glad_glDeleteBuffers(2,VBOs);
    glad_glDeleteProgram(shaderProgramOrange);
    glad_glDeleteProgram(shaderProgramYellow);
    glfwTerminate();

    
    return 0;
}