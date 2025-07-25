
// Macro commands and imports
#include <iostream>
#include <list>

#define GLEW_STATIC 1 
#include <GL/glew.h>  

#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>                  
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/common.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//using relevant namespaces
using namespace glm;
using namespace std;



// Declaring our functions and structs
GLuint loadTexture(const char *filename);

const char* getVertexShaderSource();

const char* getFragmentShaderSource();

const char* getTexturedVertexShaderSource();

const char* getTexturedFragmentShaderSource();

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource);


struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
    : position(_position), color(_color), uv(_uv) {}
    
    vec3 position;
    vec3 color;
    vec2 uv;
};



// Textured Cube model with position, color, and UV coordinates
const TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            color
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)), //left - red
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
    
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // far - blue
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // bottom - turquoise
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
    
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)), // near - green
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // right - purple
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3( 0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)), // top - yellow
    TexturedColoredVertex(vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    
    TexturedColoredVertex(vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f))
};

// String version of vertex shader
const char *getVertexShaderSource()
{
    return "#version 330 core\n"
           "layout (location = 0) in vec3 aPos;"
           "layout (location = 1) in vec3 aColor;"
           ""
           "uniform mat4 worldMatrix;"
           "uniform mat4 viewMatrix = mat4(1.0);" // default value for view matrix (identity)
           "uniform mat4 projectionMatrix = mat4(1.0);"
           ""
           "out vec3 vertexColor;"
           "void main()"
           "{"
           "   vertexColor = aColor;"
           "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
           "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
           "}";
}

// String version of fragment shader
const char *getFragmentShaderSource()
{
    return "#version 330 core\n"
           "in vec3 vertexColor;"
           "out vec4 FragColor;"
           "void main()"
           "{"
           "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
           "}";
}
// string version of textured shader with UV coordinates
const char* getTexturedVertexShaderSource()
{
        return
                "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;"
                "layout (location = 1) in vec3 aColor;"
                "layout (location = 2) in vec2 aUV;"
                ""
                "uniform mat4 worldMatrix;"
                "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
                "uniform mat4 projectionMatrix = mat4(1.0);"
                ""
                "out vec3 vertexColor;"
                "out vec2 vertexUV;"
                "void main()"
                "{"
                "   vertexColor = aColor;"
                "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
                "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
                "   vertexUV = aUV;"
                "}";
}

//  string version of textured vertex shader with UV coordinates
const char* getTexturedFragmentShaderSource()
{
    return
                "#version 330 core\n"
                "in vec3 vertexColor;"
                "in vec2 vertexUV;"
                "uniform sampler2D textureSampler;"
                ""
                "out vec4 FragColor;"
                "void main()"
                "{"
                "   vec4 textureColor = texture(textureSampler, vertexUV);"
                "   FragColor = textureColor * vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
                "}";
}

// compile and linking shaders
int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
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
}


// creating the cube texture cube vertex array object
int createTexturedCubeVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);
    
    
    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void*)(2*sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
                          );
    glEnableVertexAttribArray(2);
    
    return vertexArrayObject;
}

// setting the projection matrix
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

// setting the view matrix
void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

// setting the world matrix
void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}


// main function
int main(int argc, char *argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow *window = glfwCreateWindow(800, 600, "Project 1", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Disable mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Load Textures
    GLuint brickTextureID = loadTexture("Textures/brick.jpg");
    GLuint cementTextureID = loadTexture("Textures/cement.jpg");

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile and link shaders here ...
    int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
    int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());

    // We can set the shader once, since we have only one
    glUseProgram(colorShaderProgram);

    // Camera parameters for view transform
    vec3 cameraPosition(-25.0f, 20.0f, -5.0f);
    vec3 cameraLookAt(-1.0f, 0.0f, 5.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraSpeed = 5.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool cameraFirstPerson = true; // press 1 or 2 to toggle this variable

    // Spinning cube at camera position
    float spinningCubeAngle = 0.0f;

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,           // field of view in degrees
                                             800.0f / 600.0f, // aspect ratio
                                             0.01f, 100.0f);  // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(colorShaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,                // eye
                             cameraPosition + cameraLookAt, // center
                             cameraUp);                     // up

    GLuint viewMatrixLocation = glGetUniformLocation(colorShaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    // Set View and Projection matrices on both shaders
    setViewMatrix(colorShaderProgram, viewMatrix);
    setViewMatrix(texturedShaderProgram, viewMatrix);

    setProjectionMatrix(colorShaderProgram, projectionMatrix);
    setProjectionMatrix(texturedShaderProgram, projectionMatrix);

    // Define and upload geometry to the GPU here ...
    int vao = createTexturedCubeVertexArrayObject();

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    // Parameters to control robotic arm
    float bicepAngle = 25.0f;  // relative to vertical
    float forearmAngle = 0.0f; // relative to bicep
    float bicepLength = 20.0f;
    float forearmLength = 10.0f;


    // parameters for our target cube
    bool newCube = false;
    float cubeX = 5.f;
    float cubeY = 1.f;
    float cubeZ = 25.f;


    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // keeping track of time for adjustment with time purposes
        auto now = chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto milliseconds = chrono::duration_cast<chrono::milliseconds>(
                                duration)
                                .count();
        unsigned long millis = (unsigned long)milliseconds;


        // Enable our texture shader program, set the texture location, bind the brick texture
        glUseProgram(texturedShaderProgram);
        glActiveTexture(GL_TEXTURE0);
        GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        glBindTexture(GL_TEXTURE_2D, brickTextureID);
        glUniform1i(textureLocation, 0);                // Set our Texture sampler to user Texture Unit 0

        // Bind the appropriate VAO
        glBindVertexArray(vao);

        // Draw ground
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 0.02f, 1000.0f));
        setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        // draw bicep base
        glBindTexture(GL_TEXTURE_2D, cementTextureID);
        mat4 baseMatrix = translate(mat4(1.0f), vec3(5.0f, 1.f, 5.0f)) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.1f, 2.1f, 2.1f));
        setWorldMatrix(texturedShaderProgram, baseMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw bicep
        // translate in z sin(rotation)
        // translate in y cos(rotation)
        mat4 bicepMatrix = translate(mat4(1.0f), vec3(5.0f, bicepLength / 2 * cos(radians(bicepAngle)), 5.0f + bicepLength / 2 * sin(radians(bicepAngle)))) * rotate(mat4(1.0f), radians(bicepAngle), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, bicepLength, 1.0f));
        setWorldMatrix(texturedShaderProgram, bicepMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw the joint, angle following bicep angle
        float jointHeight = bicepLength * cos(radians(bicepAngle));
        float jointHorizontal = bicepLength * sin(radians(bicepAngle));
        vec3 jointPos = vec3(5.0f, jointHeight, 5.0f + jointHorizontal);
        mat4 hingeMatrix = translate(mat4(1.0f), vec3(5.0f, jointHeight, 5.0f + jointHorizontal)) * rotate(mat4(1.0f), radians(forearmAngle + bicepAngle), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.1f, 2.1f, 2.1f));
        setWorldMatrix(texturedShaderProgram, hingeMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Calculate forarm angle and draw
        float totalAngle = forearmAngle + bicepAngle;
        mat4 foreArmMatrix = translate(mat4(1.0f), vec3(5.0f, forearmLength / 2 * cos(radians(totalAngle)) + jointHeight, 5.0f + jointHorizontal + forearmLength / 2 * sin(radians(totalAngle)))) * rotate(mat4(1.0f), radians(totalAngle), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, forearmLength, 1.0f));
        setWorldMatrix(texturedShaderProgram, foreArmMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Calculate forarm angle and draw
        //float totalAngle = forearmAngle + bicepAngle;
        mat4 hammerMatrix = translate(mat4(1.0f), vec3(5.0f, forearmLength * cos(radians(totalAngle)) + jointHeight, 5.0f + jointHorizontal + forearmLength * sin(radians(totalAngle)))) * rotate(mat4(1.0f), radians(totalAngle), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 8.0f));
        setWorldMatrix(texturedShaderProgram, hammerMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        // calcualte new cube position if necessary
        // draw cube
        mat4 cubeMatrix = translate(mat4(1.0f), vec3(cubeX, cubeY, cubeZ)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f));
        setWorldMatrix(texturedShaderProgram, cubeMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //calcuate distance from hammer to cube
        




        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = true;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = false;
        }

        // This was solution for Lab02 - Moving camera exercise
        // We'll change this to be a first or third person camera
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;


        // Update camera horizontal and vertical angle using the mouse position
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = - mousePosX + lastMousePosX;
        double dy = - mousePosY + lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // convert to spherical coordinates
        const float cameraAngularSpeed = 6.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

        // Clamp vertical angle to -85,85
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        if (cameraHorizontalAngle > 360)
        {
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360)
        {
            cameraHorizontalAngle += 360;
        }

        float theta = radians(-cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi) * cosf(theta), -sinf(phi), -cosf(phi) * sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);


        // Use camera lookat and side vectors to update positions with ASDW
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition += cameraSideVector * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
        {
            cameraPosition -= cameraLookAt * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
        {
            cameraPosition += cameraLookAt * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move robot bicep to the left
        {

            if (bicepAngle > -50.0)
                bicepAngle = bicepAngle - 0.1;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move robot bicep to the right
        {
            if (bicepAngle < 50.0)
                bicepAngle = bicepAngle + 0.1;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move firearm up/to the left
        {
            if (forearmAngle > -110)
                forearmAngle += -0.1;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move forearm down/to the right
        {
            if (forearmAngle < 110)
                forearmAngle += 0.1;
        }


        // Set the view matrix for first person
        mat4 viewMatrix = mat4(1.0f);
        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp );
        

        
        setViewMatrix(colorShaderProgram, viewMatrix);
        setViewMatrix(texturedShaderProgram, viewMatrix);

        // left click action
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            const float projectileSpeed = 25.0f;
            cout << "left click !" << endl;
        }
        lastMouseLeftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    }

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}



GLuint loadTexture(const char *filename){
    //load textures with dimension data
    int width, height, nbChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nbChannels, 0); 
    if(!data){
        std::cerr << "Error loading texture file:" << filename << std::endl;
        return 0;
    }

    //create and bind textures
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    assert(textureId != 0);

    glBindTexture(GL_TEXTURE_2D, textureId);

    //set filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //upload texture to PU
    GLenum format = 0;
    if(nbChannels == 1)
        format = GL_RED;
    else if(nbChannels == 3)
        format = GL_RGB;
    else if(nbChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);


    //free resource
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureId;
}
