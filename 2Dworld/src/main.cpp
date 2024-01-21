/*  OpenGL is one of many API that allow us to access the graphic cards and control it to certain extend. 

    And OpenGL by its core is a specification ,not as a standalone library, therefore cannot be simply linked to our project.
    It specify that we should have this method, this function and ability to do something,
    based on the implementation that is written by manufacturer of that graphic cards or GPU.
    And Because of that everyone's implemenation is slighty different, something might work on NVdiva drivers, but same thing might look a bit different on others driver.

    In order to use mordern OpenGL, not the Window their own OpenGL header file which only go up OpenGL 1.1, I have to use GLEW.
    Because OpenGL function is not something I can download really. It is stuff in our graphic driver.
    So to use function, we need to acutally get into those drivers, pull out a function and call them.
    And there are a lot of functions, so we use GLEW to do it for us.

    GLFW is for creating window for us. Because creating window is very operation system specific. So I use GLEW to do it for me.
    And it is also quick to setup and easy to use.  test
*/

#pragma once
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "TestClearColor.h"
#include "TestTexture.h"
#include "Plot_of_Big_O_effeciency_measures.h"
#include "Chaos_Equations.h"
#include "TreeBuild.h"
#include "Dijkstra.h"
#include "GrayCodeHamiltonianPath.h"
#include "PlanerGraphFaces.h"
#include "EdgeDetection.h"
#include "SeamCarving.h"
#include "Traveling_salesman_problem.h"
#include "imgui-master/imgui.h"
#include "imgui-master/imgui_impl_glfw.h"
#include "imgui-master/imgui_impl_opengl3.h"

int main(int argc, char* argv[])
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);//compatibility  profile set Ver 
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1200, 650, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*fps*/
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout <<"OpenGL: "<<glGetString(GL_VERSION) << std::endl;
    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    
    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::EdgeDetection>("EdgeDetection");
    //testMenu->RegisterTest<test::TestTexture>("Texture");
    //testMenu->RegisterTest<test::Plot_of_Big_O_effeciency_measures>("Plot Of Effeciency Measures");
    testMenu->RegisterTest<test::Chaos_Equations>("Chaos Equations");
    testMenu->RegisterTest<test::SeamCarving>("Seam Carving");
    //testMenu->RegisterTest<test::TreeBuild>("Tree Build");
    testMenu->RegisterTest<test::Dijkstra>("Dijkstra");
    //testMenu->RegisterTest<test::Traveling_salesman_problem>("Traveling_salesman_problem");
    testMenu->RegisterTest<test::GrayCodeHamiltonianPath>("Gray code , Hamiltonain Path");
    //testMenu->RegisterTest<test::PlanerGraphFaces>("Planer graph, number of faces");
    GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
    
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        if (currentTest) {
            currentTest->onUpdate();
            currentTest->onRender();
            ImGui::Begin("Menu");
            if (currentTest != testMenu && ImGui::Button("<-")) {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImguiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    
        /* Poll for and process events */
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}