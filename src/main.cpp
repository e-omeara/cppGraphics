

#include <iostream>
#include <ctime>
#include <cmath>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#define PLATFORM_OSX 1  // Define this to use OSX specific OpenGL settings, comment out for Windows
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language



using namespace std;

const char* getVertexShaderSource()
{
    // TODO - Insert Vertex Shaders here ...
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        "out vec3 vertexColor;"
        "void main()"
        "{"
        " vertexColor = aColor;"
        " gl_Position = vec4(aPos.x, - aPos.y, aPos.z, 1.0);"   //EXERCISE 2
        "}";
    //return "need to return vertex shader source code here...";
}


const char* getFragmentShaderSource()
{
    
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        " FragColor = vec4(vertexColor.r, vertexColor.g,"
        " vertexColor.b, 1.0f);" //EXERCISE 3
        "}";
    
}


int compileAndLinkShaders()
{
    
    // compile and link shader program
    // return shader program id
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;

    //return -1;

}

int createVertexBufferObject()
{
    
    // Upload geometry to GPU and return the Vertex Buffer Object ID
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    glm::vec3 vertexArray[] = {
    glm::vec3( 0.0f, 0.5f, 0.0f), // top center position
    glm::vec3( 1.0f, 0.0f, 0.0f), // top center color (red)
    glm::vec3( 0.5f, -0.5f, 0.0f), // bottom right
    glm::vec3( 0.0f, 1.0f, 0.0f), // bottom right color (green)
    glm::vec3(-0.5f, -0.5f, 0.0f), // bottom left
    glm::vec3( 0.0f, 0.0f, 1.0f), // bottom left color (blue)
    glm::vec3(0.0f, -0.5f, 0.0f), // bottom left
    glm::vec3( 0.0f, 0.0f, 0.0f), // bottom left color (blue)
    };
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
    // cont. next slide!
    // Specify where the data is in the VAO – this allows OpenGL to bind data to vertex shader attributes
    glVertexAttribPointer(0, // attribute 0 matches aPos in Vertex Shader
    4, // size
    GL_FLOAT, // type
    GL_FALSE, // normalized?
    2*sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
    (void*)0 // array buffer offset
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, // attribute 1 matches aColor in Vertex Shader
    4,
    GL_FLOAT,
    GL_FALSE,
    2*sizeof(glm::vec3),
    (void*)sizeof(glm::vec3) );
    glEnableVertexAttribArray(1);
    // color is offseted a vec3 (comes after position)
    return vertexBufferObject;
    
    //return -1;
}


int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();
    const float PI = 3.14159265358979323846f;

#if defined(PLATFORM_OSX)	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 01", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }





    
    
    
    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();
    
    // Define and upload geometry to the GPU here ...
    int vbo = createVertexBufferObject();
    
    
    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Each frame, reset color of each pixel to glClearColor
        // Adjust based on time to create a rainbow effect
        auto now = chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto milliseconds
        = chrono::duration_cast<chrono::milliseconds>(
              duration)
              .count();
        unsigned long millis = (unsigned long) milliseconds;
        float red = 2 * PI * (millis % 60000) / 59000.0f; // Normalize to [0, 1]
        red = (sin(red) + 1.0f) / 2.0f; // Normalize to [0, 1]
        float green = 2 * PI * ((millis + 5000) % 30000) / 29000.0f; // Normalize to [0, 1]
        green = (sin(green) + 1.0f) / 2.0f; // Normalize to [0, 1]
        float blue = 2 * PI * ((millis + 2000) % 20000) / 19000.0f; // Normalize to [0, 1]
        blue = (sin(blue) + 1.0f) / 2.0f; // Normalize to [0, 1]
        glClearColor(red, green, blue, 1.0f);


        //glClearColor(0.0f, 0.082f, 0.137f, 1.0f); //EXERCISE 1
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        // draw rainbow triangle
        glUseProgram(shaderProgram); // Use the shader program we created
        glBindVertexArray(vbo); // Bind the Vertex Array Object (VAO) that contains the vertex data
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle using the vertex data in the VAO
        glBindVertexArray(0); // Unbind the VAO


        // End frame
        glfwSwapBuffers(window);
        
        // Detect inputs
        glfwPollEvents();
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
    
    // Shutdown GLFW
    glfwTerminate();
    
	return 0;
}


