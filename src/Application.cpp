#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int compileShader(unsigned int type, const std::string& source) 
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed in compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

static int createShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    return program;
}

static void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

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

    /* Bind a simple function on keyPressed */
    glfwSetKeyCallback(window, keyPressed);

    /* Initialize GLEW library */
    if (glewInit() != GLEW_OK)
        std::cout << "Error in GLEW init" << std::endl;
    
    std::cout << glGetString(GL_VERSION) << std::endl;

    unsigned int bufferTriangles;

    float positionsTriangles[12] = {
         -0.25f, 0.25f,
         -0.25f, -0.25f,
         0.25f, 0.25f,
         0.25f, -0.25f,
         -0.25f, -0.25f,
         0.25f, 0.25f,
    };

    // step 1 we generate buffer. In this case one buffer.   
    glGenBuffers(1, &bufferTriangles);

    // step 2 we bind generated buffer to a target
    glBindBuffer(GL_ARRAY_BUFFER, bufferTriangles);

    /* step 3 we assign data to a buffer and describe the size of it, give it data. 
       Also we specify as close as possible its usage type and frequency. */
    glNamedBufferData(bufferTriangles, sizeof(float) * 12, &positionsTriangles, GL_STREAM_DRAW);

    //then we enable vertex attribute array so we can use that binded buffer of GL_ARRAY_BUFFER
    glEnableVertexAttribArray(0);

    //and finally we describe how vertex should go through buffer data, from which position, what type of data and data stride
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader = 
        "#version 440 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragmentShader = 
        "#version 440 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}