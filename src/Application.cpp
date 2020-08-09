#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize GLEW library */
    if (glewInit() != GLEW_OK)
        std::cout << "Error in GLEW init" << std::endl;
    
    std::cout << glGetString(GL_VERSION) << std::endl;

    unsigned int bufferTriangles;

    float positionsTriangles[12] = {
        -0.5f, 0.0f,
        -0.5f, -0.5f,
         0.0f, 0.0f,
         0.0f, 0.0f,
         0.0f, -0.5f,
        -0.5f, -0.5f
    };

    glGenBuffers(1, &bufferTriangles);
    glBindBuffer(GL_ARRAY_BUFFER, bufferTriangles);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positionsTriangles, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}