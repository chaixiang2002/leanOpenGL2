#include <cstddef>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"


void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

//顶点着色器的源代码
// 。在GLSL中一个向量有最多4个分量，每个分量值都代表空间中的一个坐标，它们可以通过vec.x、vec.y、vec.z和vec.w来获取。
// 注意vec.w分量不是用作表达空间中的位置的（我们处理的是3D不是4D），而是用在所谓透视除法(Perspective Division)上。
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

//在计算机图形中颜色被表示为有4个元素的数组：红色、绿色、蓝色和alpha(透明度)分量，每个分量的强度设置在0.0到1.0之间
// 比如说我们设置红为1.0f，绿为1.0f，我们会得到两个颜色的混合色，即黄色。这三种颜色分量的不同调配可以生成超过1600万种不同的颜色！
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main(){

    // 初始化和配置glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//使用glfwWindowHint函数来配置GLFW。:提示
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// PROFILE配置文件，使用的是核心模式(Core-profile)

    // 创建窗口
    GLFWwindow* window=glfwCreateWindow(800, 600, "SnowWindow", NULL, NULL);
    if (window==NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //  glad: 导入所有 OpenGL 函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 构建并编译 我们的着色器项目
    // vertex shader
    unsigned int vertexShader=glad_glCreateShader(GL_VERTEX_SHADER);
    glad_glShaderSource(vertexShader,1,&vertexShaderSource,NULL);//第二参数指定了传递的源码字符串数量，这里只有1个。
    glCompileShader(vertexShader);
    // 检查编译 错误
    int success;
    char infoLog[512];
    glad_glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glad_glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader片段着色器
    unsigned int fragmentShader=glad_glCreateShader(GL_FRAGMENT_SHADER);
    glad_glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glad_glCompileShader(fragmentShader);
    glad_glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glad_glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 链接着色器
    unsigned int shaderProgram=glad_glCreateProgram();// 创建着色器程序对象
    glad_glAttachShader(shaderProgram,vertexShader);
    glad_glAttachShader(shaderProgram,fragmentShader);
    glad_glLinkProgram(shaderProgram);

    // 检查链接错误
    glad_glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if (!success) {
        glad_glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 释放着色器
    glad_glDeleteShader(vertexShader);
    glad_glDeleteShader(fragmentShader);

    // 开启顶点数据和配置顶点属性
    float vertices[]={
        0.5f,0.5f,0.0f,
        -0.5f,0.5f,0.0f,
        -0.5f,-0.5f,0.0f,
        0.5f,-0.5f,0.0f
    };
    unsigned int indices[]={
        0,1,2,//第1个三角形，每个顶点
        0,3,2//第2个三角形
    };
    unsigned int VBO,VAO,EBO;
    glad_glGenVertexArrays(1,&VAO);
    glad_glGenBuffers(1,&VBO);//使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
    glad_glGenBuffers(1,&EBO);


    // 绑定顶点数组对象VAO，设置和绑定缓冲，配置顶点属性
    glad_glBindVertexArray(VAO);

    glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);//OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
    //OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
    //从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
    glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);//glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存
    //glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
    //第一个参数是目标缓冲的类型
    //第二个参数指定传输数据的大小
    //。第三个参数是我们希望发送的实际数据
//     第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
// GL_STATIC_DRAW ：数据不会或几乎不会改变。
// GL_DYNAMIC_DRAW：数据会被改变很多。
// GL_STREAM_DRAW ：数据每次绘制时都会改变。

    // 0. 复制顶点数组到缓冲中供OpenGL使用
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    //// 1. 设置顶点属性指针
    //glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）了：
    glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    //第一个参数指定我们要配置的顶点属性。
    //第二个参数指定顶点属性的大小
    //第三个参数指定数据的类型
    //第四参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
    // 第五个参数叫做步长(Stride)，连续的顶点属性组之间的间隔
    // 它表示位置数据在缓冲中起始位置的偏移量(Offset)。
    glad_glEnableVertexAttribArray(0);

    glad_glBindBuffer(GL_ARRAY_BUFFER,0);
    //注意，这是允许的，调用glVertexAttribPointer将VBO注册为顶点属性的绑定顶点缓冲对象，因此之后我们可以安全地解除绑定

    //记住:当VAO处于活动状态时，不要解除绑定EBO，因为绑定的元素缓冲区对象存储在VAO中;保持EBO的上限。
    // glBindBuffer (GL_ELEMENT_ARRAY_BUFFER 0);

//你可以在之后解除对VAO的绑定，这样其他VAO调用就不会意外地修改这个VAO，但这种情况很少发生。修改其他
// VAOs无论如何都需要调用glBindVertexArray，所以当不直接需要时，我们通常不会取消绑定VAOs(也不会取消绑定vbo)。
    glad_glBindVertexArray(0);

    //取消注释以绘制线框多边形。
// glPolygonMode (GL_FRONT_AND_BACK GL_LINE);


    while (!glfwWindowShouldClose(window)) {

        // input
        // -----
        processInput(window);

        // render
        // ------
        glad_glClearColor(0.2f,0.3f,0.3f,1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT);

        //画我们的第一个三角形
        glad_glUseProgram(shaderProgram);
        glad_glBindVertexArray(VAO);
        glad_glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        //望绘制的是一个三角形，这里传递GL_TRIANGLES给它。
        //第二个参数指定了顶点数组的起始索引，我们这里填0。
        //最后一个参数指定我们打算绘制多少个顶点，这里是3（我们只从我们的数据中渲染一个三角形，它只有3个顶点长）。


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //释放所有资源
    glad_glDeleteVertexArrays(1,&VAO);
    glad_glDeleteBuffers(1,&VBO);
    glad_glDeleteBuffers(1,&EBO);
    glad_glDeleteProgram(shaderProgram);
    
    //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glfwTerminate();
    

    return 0;
}