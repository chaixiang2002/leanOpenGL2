#include <cstddef>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// #include "check_gl.hpp"
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

// 这个帧缓冲大小函数需要一个GLFWwindow作为它的第一个参数，
// 以及两个整数表示窗口的新维度。每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理。
void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

//GLFW的glfwGetKey函数，它需要一个窗口以及一个按键作为输入。
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(){

    // 调用glfwInit函数来初始化GLFW
    glfwInit();
    // 使用glfwWindowHint函数来配置GLFW。
    // glfwWindowHint函数的第一个参数代表选项的名称，
    // 我们可以从很多以GLFW_开头的枚举值中选择；
    //第二个参数接受一个整型，用来设置这个选项的值。
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//使用glfwWindowHint函数来配置GLFW。:提示
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// PROFILE配置文件，使用的是核心模式(Core-profile)
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GLFW_TRUE);//FORWARD_COMPAT 先前兼容

    //创建一个窗口对象
    /*
    glfwCreateWindow函数需要窗口的宽和高作为它的前两个参数。
    第三个参数表示这个窗口的名称（标题），这里我们使用"LearnOpenGL"，
    当然你也可以使用你喜欢的名称。最后两个参数我们暂时忽略。
    这个函数将会返回一个GLFWwindow对象，我们会在其它的GLFW操作中使用到。
    创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。
    */
    GLFWwindow* window=glfwCreateWindow(800, 600, "SnowWindow", NULL, NULL);
    if (window==NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数：
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // // 在我们开始渲染之前还有一件重要的事情要做，
    // // 我们必须告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)，
    // // 这样OpenGL才只能知道怎样根据窗口大小显示数据和坐标。
    // // 我们可以通过调用glViewport函数来设置窗口的维度(Dimension)：
    // // 前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
    // glViewport(0,0,800,600);


    
    // 我们需要在程序中添加一个while循环，我们可以把它称之为渲染循环(Render Loop)，
    // 它能在我们让GLFW退出前一直保持运行。下面几行的代码就实现了一个简单的渲染循环：
    //1. glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
    //2.  glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
    //3. glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
    while (!glfwWindowShouldClose(window)) {

        // input
        // -----
        processInput(window);

        // render
        // ------
        // 为了测试一切都正常工作，我们使用一个自定义的颜色清空屏幕。在每个新的渲染迭代开始的时候我们总是希望清屏，否则我们仍能看见上一次迭代的渲染结果（这可能是你想要的效果，但通常这不是）。我们可以通过调用glClear函数来清空屏幕的颜色缓冲，它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲，可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT。由于现在我们只关心颜色值，所以我们只清空颜色缓冲。
        glad_glClearColor(0.2f,0.3f,0.3f,1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glfwTerminate();
    
    return 0;
}