//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//

#include <iostream>
#include <list>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>


using namespace glm;
using namespace std;

class Projectile
{
public:
    Projectile(vec3 position, vec3 velocity, int shaderProgram) : mPosition(position), mVelocity(velocity)
    {
        mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    }
    
    void Update(float dt)
    {
        mPosition += mVelocity * dt;
    }
    
    void Draw() {
        // this is a bit of a shortcut, since we have a single vbo, it is already bound
        // let's just set the world matrix in the vertex shader
        
        mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
private:
    GLuint mWorldMatrixLocation;
    vec3 mPosition;
    vec3 mVelocity;
};


const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
                "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;"
                "layout (location = 1) in vec3 aColor;"
                ""
                "uniform mat4 worldMatrix;"
                "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
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


const char* getFragmentShaderSource()
{
    return
                "#version 330 core\n"
                "in vec3 vertexColor;"
                "out vec4 FragColor;"
                "void main()"
                "{"
                "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
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
    if (!success)
    {
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


int createVertexBufferObject()
{
    // Cube model
    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), //left - red
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        
        vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), // far - blue
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        
        vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        
        vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), // bottom - turquoise
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
        
        vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
        
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // near - green
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        
        vec3( 0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        
        vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), // right - purple
        vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        
        vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3( 0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
        
        vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), // top - yellow
        vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        
        vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f)
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

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          2*sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          2*sizeof(vec3),
                          (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(1);

    
    return vertexBufferObject;
}


int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 03", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // @TODO 3 - Disable mouse cursor
    // ...
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();
    
    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);

    
    // Camera parameters for view transform
    vec3 cameraPosition(0.6f,1.0f,10.0f);
    vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    
    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable

    // Spinning cube at camera position
    float spinningCubeAngle = 0.0f;
    
    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
                                             800.0f / 600.0f,  // aspect ratio
                                             0.01f, 100.0f);   // near and far (near > 0)
    
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
                             cameraPosition + cameraLookAt,  // center
                             cameraUp ); // up
    
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    
    
    // Define and upload geometry to the GPU here ...
    int vao = createVertexBufferObject();
    
    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
    
    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    
    // @TODO 1 - Enable Depth Test
    // ...
    glEnable(GL_DEPTH_TEST);
    
    
    // Container for projectiles to be implemented in tutorial
    list<Projectile> projectileList;


    float bicepAngle = 25.0f; //relative to ground plane
    float forearmAngle = 0.0f;   //relative to bicep
    float bicepLength = 20.0f;
    float forearmLength = 10.0f;
    
    
    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor
         // @TODO 1
        // Add the GL_DEPTH_BUFFER_BIT to glClear – TODO 1
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     

        // @TODO 1 - Clear Depth Buffer Bit as well
        // ...
        //glClear(GL_COLOR_BUFFER_BIT);

        auto now = chrono::system_clock::now();
        // Convert the current time to time since epoch
        auto duration = now.time_since_epoch();
        // Convert duration to milliseconds
        auto milliseconds
        = chrono::duration_cast<chrono::milliseconds>(
              duration)
              .count();
        unsigned long millis = (unsigned long) milliseconds;
        float red = 2 * 3.14159 * (millis % 60000) / 59999.0f; // Normalize to [0, 1]
        red = (sin(red) + 1.0f) / 2.0f; // Normalize to [0, 1]
        red = red * 90;
        red = red - 45;
        //bicepAngle = red;
        //forearmAngle = red + 0.5;

        
        // Draw geometry
        glBindVertexArray(vao);

        // Draw ground
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 0.02f, 1000.0f));
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);
        
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        //draw bicep base
        mat4 pillarWorldMatrix = translate(mat4(1.0f), vec3(5.0f, 1.f, 5.0f)) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.1f, 2.1f, 2.1f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &pillarWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // Draw bicep
        // translate in z sin(rotation)
        //translate in y cos(rotation)
        pillarWorldMatrix = translate(mat4(1.0f), vec3(5.0f, bicepLength/2 * cos(radians(bicepAngle)), 5.0f + bicepLength/2 * sin(radians(bicepAngle)))) * rotate(mat4(1.0f), radians(bicepAngle), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, bicepLength, 1.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &pillarWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        float jointHeight = bicepLength * cos(radians(bicepAngle));
        float jointHorizontal = bicepLength * sin(radians(bicepAngle));
        pillarWorldMatrix = translate(mat4(1.0f), vec3(5.0f, jointHeight, 5.0f + jointHorizontal)) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.1f, 2.1f, 2.1f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &pillarWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // forearm
        float totalAngle = forearmAngle + bicepAngle;
        pillarWorldMatrix = translate(mat4(1.0f), vec3(5.0f, forearmLength/2 * cos(radians(totalAngle)) + jointHeight, 5.0f + jointHorizontal + forearmLength/2 * sin(radians(totalAngle)))) * rotate(mat4(1.0f), radians(totalAngle), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, forearmLength, 1.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &pillarWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //repeating pillars
        /*
        for (int i=0; i<20; ++i)
        {
            for (int j=0; j<20; ++j)
            {
                pillarWorldMatrix = translate(mat4(1.0f), vec3(- 100.0f + i * 10.0f, 5.0f, -100.0f + j * 10.0f)) * scale(mat4(1.0f), vec3(1.0f, 10.0f, 1.0f));
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &pillarWorldMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                
                pillarWorldMatrix = translate(mat4(1.0f), vec3(- 100.0f + i * 10.0f, 0.55f, -100.0f + j * 10.0f)) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.1f, 1.1f, 1.1f));
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &pillarWorldMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        */
        

        // @TODO 2 - Update and draw projectiles
        // ...
        for(list<Projectile>::iterator it = projectileList.begin(); it != projectileList.end(); it++){
            it->Update(dt);
            it->Draw();
        }
        
        
        // Spinning cube at camera position
        spinningCubeAngle += 180.0f * dt;
        
        // @TODO 7 - Draw in view space for first person camera
        if (cameraFirstPerson){
            mat4 spinningCubeWorldMatrix(1.0f);
            mat4 spinningCubeViewMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f)) *
                                            rotate(mat4(1.0f), radians(spinningCubeAngle), vec3(0.0f, 1.0f, 0.0f)) *
                                            scale(mat4(1.0f), vec3(0.01f, 0.01f, 0.01f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spinningCubeWorldMatrix[0][0]);
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &spinningCubeViewMatrix[0][0]);

        }
        else {
            // In third person view, let's draw the spinning cube in world space, like any other models
            mat4 spinningCubeWorldMatrix = translate(mat4(1.0f), cameraPosition) *
                                           rotate(mat4(1.0f), radians(spinningCubeAngle), vec3(0.0f, 1.0f, 0.0f)) *
                                           scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
            
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spinningCubeWorldMatrix[0][0]);
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);

        
        
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
        
        
        // @TODO 4 - Calculate mouse motion dx and dy
        //         - Update camera horizontal and vertical angle
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);
        
        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        //convert to spherical coordinates
        const float cameraAngularSpeed = 6.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

        //Clamp vertical angle to -85,85
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        if (cameraHorizontalAngle > 360){
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360){
            cameraHorizontalAngle += 360;
        }

        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);

        //...
        
        // @TODO 5 = use camera lookat and side vectors to update positions with ASDW
        // adjust code below
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

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move camera to the left
        {
            
            if(bicepAngle > -50.0)
                bicepAngle = bicepAngle - 0.1;
        }
        
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move camera to the right
        {
            if(bicepAngle < 50.0)
                bicepAngle = bicepAngle + 0.1;
        }
        
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move camera up
        {
            if(forearmAngle > -110)
                forearmAngle += -0.1;
        }
        
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move camera down
        {
            if(forearmAngle < 110)
            forearmAngle += 0.1;
        }
      
        // TODO 6
        // Set the view matrix for first and third person cameras
        // - In first person, camera lookat is set like below
        // - In third person, camera position is on a sphere looking towards center
        mat4 viewMatrix = mat4(1.0f);
        if(cameraFirstPerson){
            viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp );
        } else {
            float radius = 5.0f;
            glm::vec3 position = cameraPosition - radius * cameraLookAt;
            viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);
        }
        

        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        
        
        // @TODO 2 - Shoot Projectiles
        //
        // shoot projectiles on mouse left click
        // To detect onPress events, we need to check the last state and the current state to detect the state change
        // Otherwise, you would shoot many projectiles on each mouse press
        // ...
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            const float projectileSpeed = 25.0f;
            projectileList.push_back(Projectile(cameraPosition, projectileSpeed * cameraLookAt, shaderProgram));
        }
        lastMouseLeftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);


    }

    
    // Shutdown GLFW
    glfwTerminate();
    
	return 0;
}
