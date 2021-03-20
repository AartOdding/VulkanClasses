#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>



int main()
{
    glfwSetErrorCallback([](int errorCode, const char* errorDescription) {
        std::cerr << "GLFW error " << errorCode << ": " << errorDescription << std::endl;
    });

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_DECORATED, true);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, true);

    auto window = glfwCreateWindow(720, 480, "Project name", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create window." << std::endl;
        glfwTerminate();
        std::exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    if (!gladLoadGL())
    {
        std::cerr << "Failed to load OpenGL." << std::endl;
        glfwTerminate();
        std::exit(1);
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


        glClearColor(0.1, 0, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
