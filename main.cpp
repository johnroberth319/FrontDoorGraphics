// Compile: g++ main.cpp -o FrontDoorTest -lglfw -lGL -lGLEW -lSOIL -lassimp

#include <iostream>  // iostream include

// GLEW
#define GLEW_STATIC // Define glew_static
#include <GL/glew.h> // glew include

// GLFW
#include <GLFW/glfw3.h> // glfw include

// SOIL
#include <SOIL/SOIL.h> // soil include

// GLM Mathematics
#include <glm/glm.hpp> // glm include
#include <glm/gtc/matrix_transform.hpp> // glm matrix math include
#include <glm/gtc/type_ptr.hpp> // glm gtc include

// Other includes
#include "shader.h" // Include shader class
#include "Camera.h" // Include Camera class
#include "Model.h" // Include Model class

const GLuint WIDTH = 540, HEIGHT = 720; // Global variables for width and height of window
GLuint cameraMode = 0;

class RenderedObject {
    public:

        Model model = Model("FrontDoor.obj");
        glm::vec3 color;
        glm::vec3 lightingValues;

        RenderedObject() {
            model = Model("FrontDoor.obj");
            color = glm::vec3(1.0f, 1.0f, 1.0f);
            lightingValues = glm::vec3(0.5, 0.5, 16);
        };

        RenderedObject(string rObjName, glm::vec3 rObjColor, glm::vec3 rObjLightingValues) {
            model = Model(("ObjectsByMaterial/" + rObjName + ".obj").c_str());
            color = rObjColor;
            lightingValues = rObjLightingValues;
        };
};

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); // key_callback method
void do_movement(); // Movement method

Camera camera(glm::vec3(0.582551f, -0.869032f, -1.80866f)); // Sets iniital camera pos (0, 0, 0)
GLfloat lastX = WIDTH / 2.0; // Used for camera motion
GLfloat lastY = HEIGHT / 2.0; // Used for camera motion
bool keys[1024]; // Allowable number of key strokes

glm::vec3 lightPos(-3.0f, 2.0f, 0.0f); // Sets light position

GLfloat deltaTime = 0.0f; // Initialize deltaTime for camera movement
GLfloat lastFrame = 0.0f; // Initialize lastFrame for camera movement

int main() {
    // Init GLFW
    glfwInit(); // Initialize GLFW
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set major context version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set minor context version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set profiles
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Set resizable to false

    // Create GLFWwindow
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Project Front FrontDoor", nullptr, nullptr); // Create window
    glfwMakeContextCurrent(window); // Make context method

    // Set required callback functions
    glfwSetKeyCallback(window, key_callback); // Set key_callback method

    glewExperimental = GL_TRUE; // Set glew to experimental

    glewInit(); // Initialize GLEW

    glViewport(0, 0, WIDTH, HEIGHT); // Define viewport dimensions

    glEnable(GL_DEPTH_TEST); // Set up OpenGL options


    // Create Objects

    vector<RenderedObject> rObjects;

    rObjects.push_back(RenderedObject("BlackMatteObjects", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("BlackWoolObjects", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("BronzeObjects", glm::vec3(160.0f/255.0f, 82.0f/255.0f, 45.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("DullBrownObjects", glm::vec3(210.0f/255.0f, 180.0f/255.0f, 140.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("GoldObjects", glm::vec3(218.0f/255.0f, 165.0f/255.0f, 32.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));

    rObjects.push_back(RenderedObject("LightWoodObjects", glm::vec3(255.0f/255.0f, 235.0f/255.0f, 205.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("LightYellowObjects", glm::vec3(255.0f/255.0f, 255.0f/255.0f, 224.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("MetalObjects", glm::vec3(100.0f/255.0f, 100.0f/255.0f, 100.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("OrangeObjects", glm::vec3(255.0f/255.0f, 165.0f/255.0f, 0.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("WhiteMatteObjects", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.1f, 16.0f)));

    rObjects.push_back(RenderedObject("WhitePlasterObjects", glm::vec3(248.0f/255.0f, 248.0f/255.0f, 255.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("WhitePlasticObjects", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("WhiteWoodObjects", glm::vec3(255.0f/255.0f, 250.0f/255.0f, 250.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("WoodObjects", glm::vec3(245.0f/255.0f, 222.0f/255.0f, 179.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));
    rObjects.push_back(RenderedObject("WoodTileObjects", glm::vec3(245.0f/255.0f, 222.0f/255.0f, 179.0f/255.0f), glm::vec3(0.5f, 0.1f, 16.0f)));


    // Shaders for Each Model
    Shader frontDoorShader("FrontDoor.vs", "FrontDoor.frag");

    GLfloat vertices[] = {
        // Coordinates: 3 Position, 3 Color, 2 Texture
        
        // Using Models Only, No Vertices
    };

    GLuint VBO, VAO; // Initialize VBO, VAO
    glGenVertexArrays(1, &VAO); // Generate VAO
    glGenBuffers(1, &VBO); // Generate VBO

    glBindVertexArray(VAO);  // Bind VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Buffer Data

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Set vertex attribute pointer for pos
    glEnableVertexAttribArray(0); // Enable vertex attrib array with 0
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Set vertex attrib pointer for texcoord
    glEnableVertexAttribArray(2); // Enable with 2
    // Normalized Position Attribute
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Set normalized pos vertex attrib
    glEnableVertexAttribArray(4); // Enable with 4

    glBindVertexArray(0); // Unbind VAO

    // Game Loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate deltaTime for camera movement
        GLfloat currentFrame = glfwGetTime(); // Get current time
        deltaTime = currentFrame - lastFrame; // Calculate change in time
        lastFrame = currentFrame; // Set last frame to current frame

        // Check for events
        glfwPollEvents(); // Callback glfwPollEvents to check for events
        do_movement(); // Callback do_movement()

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers

        // Initialize Camera
        glm::mat4 view = glm::mat4(1.0f); // Initialize view to identity
        view = camera.GetViewMatrix(); // Set view based on camera
        glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f); // Initialize projection using initial values
        glm::mat4 model = glm::mat4(1.0f); // Initialize model to be 4x4 identity

        frontDoorShader.Use();

        for(size_t i = 0; i < rObjects.size(); ++i){
                
            GLint frontDoorColorLoc = glGetUniformLocation(frontDoorShader.Program, "objectColor"); // Retrieve frontDoorColor location
            GLint lightColorLoc = glGetUniformLocation(frontDoorShader.Program, "lightColor"); // Reset lightColor location
            GLint lightPosLoc = glGetUniformLocation(frontDoorShader.Program, "lightPos"); // Reset lightPos location
            GLint viewPosLoc = glGetUniformLocation(frontDoorShader.Program, "viewPos"); // Reset viewPos location

            GLint ambStrLoc = glGetUniformLocation(frontDoorShader.Program, "ambientStrength"); // Reset viewPos location
            GLint specStrLoc = glGetUniformLocation(frontDoorShader.Program, "specularStrength"); // Reset viewPos location
            GLint shineLoc = glGetUniformLocation(frontDoorShader.Program, "shininess"); // Reset viewPos location

            glUniform3f(frontDoorColorLoc, rObjects[i].color.r, rObjects[i].color.g, rObjects[i].color.b); // Pass color to uniform
            glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // Pass light color to uniform
            glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z); // Pass light position to uniform
            glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z); // Pass camera position to uniform

            glUniform1f(ambStrLoc, rObjects[i].lightingValues.x);
            glUniform1f(specStrLoc, rObjects[i].lightingValues.y);
            glUniform1f(shineLoc, rObjects[i].lightingValues.z);

            GLint modelLoc = glGetUniformLocation(frontDoorShader.Program, "model"); // Reset view location for frontDoorShader
            GLint viewLoc = glGetUniformLocation(frontDoorShader.Program, "view"); // Reset view location for frontDoorShader
            GLint projLoc = glGetUniformLocation(frontDoorShader.Program, "projection"); // Reset view location for frontDoorShader

            glm::mat4 view_frontDoor = view; // Create mat4 view_frontDoor using generic view identity
            view_frontDoor = glm::translate(view_frontDoor, glm::vec3(0.0f, 0.0f, -10.0f)); // Translate frontDoor back
            view_frontDoor = glm::scale(view_frontDoor, glm::vec3(1.0f, 1.0f, 1.0f)); // Increase height of frontDoor

            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_frontDoor)); // Pass view_frontDoor to shader
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection to shader
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Pass moel to shader

            rObjects[i].model.Draw(frontDoorShader); // Draw obj model
        }

        glBindVertexArray(0); // Bind zero at end
        glfwSwapBuffers(window); // Swap screen buffers

    }
    // Deallocate resources
    glDeleteVertexArrays(1, &VAO); // Deallocate vertex arrays
    glDeleteBuffers(1, &VBO); // Deallocate buffers
    glfwTerminate(); // Terminate window
    return 0; // Returns 0 for end of int main()

}

// Method for key input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { // If ESC pressed
        glfwSetWindowShouldClose(window, GL_TRUE); // Close window
    } if (key >= 0 && key < 1024) { // Allow for 1024 key presses
        if (action == GLFW_PRESS) { // If pressed
            keys[key] = true; // Set keys[key] = true [key pressed]
        } else if (action == GLFW_RELEASE) { // If released
            keys[key] = false; // Set keys[key] = false [key not pressed]
        }
    }
}

// Initiates movement based on keyboard input
void do_movement() {
    
    if (keys[GLFW_KEY_UP]) { // If up arrow
        camera.ProcessKeyboard(FORWARD, deltaTime); // Move camera forward with callback
    } else if (keys[GLFW_KEY_DOWN]) { // If down arrow
        camera.ProcessKeyboard(BACKWARD, deltaTime); // Move camera backward with callback
    } else if (keys[GLFW_KEY_J]) { // If comma --> less than symbol
        camera.ProcessKeyboard(UPROLL, deltaTime); // Move camera in positive roll with callback
    } else if (keys[GLFW_KEY_L]) { // If period --> greater than symbol
        camera.ProcessKeyboard(DOWNROLL, deltaTime); // Move camera in negative roll with callback
    }

    if (keys[GLFW_KEY_W]) { // If down arrow
        camera.ProcessKeyboard(UPPITCH, deltaTime); // Move camera in positive pitch with callback
    } else if (keys[GLFW_KEY_S]) { // If up arrow
        camera.ProcessKeyboard(DOWNPITCH, deltaTime); // Move camera in negative pitch with callback
    } else if (keys[GLFW_KEY_D]) { // If right arrow
        camera.ProcessKeyboard(UPYAW, deltaTime); // Move camera in positive yaw with callback
    } else if (keys[GLFW_KEY_A]) { // If left arrow
        camera.ProcessKeyboard(DOWNYAW, deltaTime); // Move camera in negative yaw with callback
    }

    if (keys[GLFW_KEY_RIGHT]) { // If right arrow
        camera.ProcessKeyboard(RIGHT, deltaTime); // Translate camera right with callback
    }
    else if (keys[GLFW_KEY_LEFT]) { // If left arrow
        camera.ProcessKeyboard(LEFT, deltaTime); // Translate camera left with callback
    }
    else if (keys[GLFW_KEY_I]) { // If up arrow
        camera.ProcessKeyboard(UP, deltaTime); // Translate camera up with callback
    }
    else if (keys[GLFW_KEY_K]) { // If down arrow
        camera.ProcessKeyboard(DOWN, deltaTime); // Translate camera down with callback
    }

    if (keys[GLFW_KEY_R]) { // If r is pressed
        camera.ResetCamera(); // Reset camera with callback
    }

    if (keys[GLFW_KEY_1]) { // If r is pressed
        lightPos = glm::vec3(-3.0f, 2.0f, 0.0f);
    } else if (keys[GLFW_KEY_2]) { // If r is pressed
        lightPos = glm::vec3(3.0f, 2.0f, 0.0f);
    } else if (keys[GLFW_KEY_3]) { // If r is pressed
        lightPos = glm::vec3(-3.0f, 5.0f, 0.0f);
    } else if (keys[GLFW_KEY_4]) { // If r is pressed
        lightPos = glm::vec3(-228.25f, 43.25f, -12.0f);
    }
    
}