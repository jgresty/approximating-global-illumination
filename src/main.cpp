#include "scene.h"
#include "the_render_manager.h"
#include "camera.h"
#include "controller.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

int
main(int argc, char* argv[]) {
    std::cout << "Loading GLFW..." << std::endl;
    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW" << std::endl;
        return 1;
    }

    int winWidth = 1280;
    int winHeight = 720;

    int testHeight, testWidth;

    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Voxels", NULL, NULL);
    if (!window) {
        std::cerr << "ERROR: could not open window" << std::endl;
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glfwSwapInterval(0);

    std::cout << "Loading GLEW..." << std::endl;
    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR: could not start GLEW" << std::endl;
        return 3;
    }

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version: " << version << std::endl;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 0, 0);

    glfwPollEvents();


    std::cout << "Loading shaders..." << std::endl;
    TheRenderManager::Instance()->Init(winWidth, winHeight);
    std::cout << "Creating framebuffer..." << std::endl;
    //TheRenderManager::Instance()->Use_defered();

    std::cout << "Loading scene..." << std::endl;;
    Scene scene;
    if (argc < 2) {
        std::cout << "No scene given, using res/models/crytek-sponza.obj" << std::endl;
        scene.Load_obj_file("res/models/crytek-sponza.obj");
    } else {
        scene.Load_obj_file(argv[1]);
    }
    TheRenderManager::Instance()->Set_scene(scene);

    TheRenderManager::Instance()->Init_voxelization(256);

    Camera camera;
    camera.Setup();
    camera.renderFar = scene.size*2;
    camera.renderNear = scene.size/100;

    Controller controller(window, &scene);
    controller.Set_camera(&camera);
    controller.speed = scene.size/5;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glPointSize(5);

    double currentTime, lastTime = 0;
    float deltaTime;


    std::cout << "Entering main loop" << std::endl;
    do {
        currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        controller.Get_input();
        controller.Update_view(deltaTime);

        TheRenderManager::Instance()->Voxelize();

        TheRenderManager::Instance()->Render(&camera);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // resize window
        glfwGetWindowSize(window, &testWidth, &testHeight);
        if (testWidth != winWidth || testHeight != winHeight) {
            winWidth = testWidth;
            winHeight = testHeight;
            TheRenderManager::Instance()->Set_render_size(winWidth, winHeight);
            camera.Set_aspect((float)winWidth/winHeight);
        }

    } while(!glfwWindowShouldClose(window));

    std::cout << "Window closed, shutting down" << std::endl;

    glfwTerminate();

    return 0;
}
