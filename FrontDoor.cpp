#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL/SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include <cmath>


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLfloat doorAngle = 0.0f;
bool movingDoor = false;
bool openTheDoor = false;
const GLfloat doorAngleSpeed = 1.5f;

GLfloat defaultWindowAngle[2] = {25.0f, 45.0f};
GLfloat windowAngle[2] = {25.0f, 45.0f};
bool movingBlinds[2] = {false, false};
bool openTheBlinds[2] = {false, false};

void moveDoor(bool open) {
    if (open) {
        movingDoor = true;
        openTheDoor = true;
    } else {
        movingDoor = true;
        openTheDoor = false;
    }
}

void openDoor() {
    if (doorAngle >= 90.0f) {
        movingDoor = false;
        return;
    } else {
        if ((doorAngle + doorAngleSpeed) < 90.0f ) {
            doorAngle += doorAngleSpeed;
        } else {
            doorAngle = 90.0f;
        }
    }
}

void closeDoor() {
    if (doorAngle <= 0.0f) {
        movingDoor = false;
        return;
    } else {
        if ((doorAngle - doorAngleSpeed) > 0.0f) {
            doorAngle -= doorAngleSpeed;
        } else {
            doorAngle = 0.0f;
        }
    }
}

void moveBlinds(int index, bool open) {
    if (open) {
        movingBlinds[index] = true;
        openTheBlinds[index] = true;
    } else {
        movingBlinds[index] = true;
        openTheBlinds[index] = false;
    }
}

void openBlinds(int index) {
    if (windowAngle[index] >= 90.0f) {
        movingBlinds[index] = false;
        return;
    } else {
        if ((windowAngle[index] + 2.0f) < 90.0f) {
            windowAngle[index] += 2.0f;
        } else {
            windowAngle[index] = 90.0f;
        }
    }
}

void resetBlinds(int index) {
    if (windowAngle[index] <= defaultWindowAngle[index]) {
        movingBlinds[index] = false;
        return;
    } else {
        if ((windowAngle[index] - 2.0f) > defaultWindowAngle[index]) {
            windowAngle[index] -= 2.0f;
        } else {
            windowAngle[index] = defaultWindowAngle[index];
        }
    }
}

// Window dimensions
const GLuint WIDTH = 450, HEIGHT = 600;
GLuint cameraMode = 0; // 0 - Default, 1 - Front, 2- Side

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Front Door Demo", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);


    // Build and compile our shader program
    Shader ourShader("FrontDoorDefault.vs", "FrontDoorDefault.frag");

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {

        // OpenGL draws in triangles.

        // This means that to draw a quad,
        // we need to make two triangles.
        // This is why most components are split
        // into two portions (triangles).

        // Draw Citro 1/2
        0.15f, -5.5f, -4.1f, 0.0f, 0.0f,
        0.15f, 4.35f, -4.1f, 0.0f, 1.0f,
        3.925f, 4.35f, -4.1f, 1.0f, 1.0f,
        // Draw Citro 2/2
        0.15f, -5.5f, -4.1f, 0.0f, 0.0f,
        3.925f, -5.5f, -4.1f, 1.0f, 0.0f,
        3.925f, 4.35f, -4.1f, 1.0f, 1.0f,


        // Draw Floor 1/2
        -3.4f, -5.4f, -4.0f, 0.0f, 1.0f,
        4.075f, -5.4f, -4.0f, 1.0f, 1.0f,
        -3.4f, -5.4f, 0.0f, 0.0f, 0.0f,
        // Draw Floor 2/2
        -3.4f, -5.4f, 0.0f, 0.0f, 0.0f,
        4.075f, -5.4f, -4.0f, 1.0f, 1.0f,
        4.075f, -5.4f, 0.0f, 1.0f, 0.0f,

        // Draw Ceiling 1/2
        -3.4f, 5.4f, -4.0f, 0.0f, 1.0f,
        4.075f, 5.4f, -4.0f, 1.0f, 1.0f,
        -3.4f, 5.4f, 0.0f, 0.0f, 0.0f,
        // Draw Ceiling 2/2
        -3.4f, 5.4f, 0.0f, 0.0f, 0.0f,
        4.075f, 5.4f, -4.0f, 1.0f, 1.0f,
        4.075f, 5.4f, 0.0f, 1.0f, 0.0f,

        // Draw Wall Below Window 1/2
        -3.4f, -5.2f, -4.0f, 0.0f, 0.0f,
        -3.4f, -3.4f, -4.0f, 0.0f, 0.5f,
        0.0f, -3.4f, -4.0f, 1.0f, 0.5f,
        // Draw Wall Below Window 2/2
        -3.4f, -5.2f, -4.0f, 0.0f, 0.0f,
        0.0f, -5.2f, -4.0f, 1.0f, 0.0f,
        0.0f, -3.4f, -4.0f, 1.0f, 0.5f,

        // Draw Wall Left of Window 1/2
        -3.4f, -3.4f, -4.0f, 0.0f, 0.25f,
        -3.4f, 4.3f, -4.0f, 0.0f, 0.75f,
        -2.575f, 4.3f, -4.0f, 1.0, 0.75f,
        // Draw Wall Left of Window 2/2
        -3.4f, -3.4f, -4.0f, 0.0f, 0.25f,
        -2.575f, -3.4f, -4.0f, 1.0f, 0.75f,
        -2.575f, 4.3f, -4.0f, 1.0f, 0.75f,

        // Draw Wall Right of Window 1/2
        -0.275, -3.4f, -4.0f, 0.0f, 0.25f,
        -0.275f, 4.3f, -4.0f, 0.0f, 0.75f,
        0.0f, 4.3f, -4.0f, 1.0, 0.75f,
        // Draw Wall Right of Window 2/2
        -0.275, -3.4f, -4.0f, 0.0f, 0.25f,
        0.0f, -3.4f, -4.0f, 1.0f, 0.75f,
        0.0f, 4.3f, -4.0f, 1.0f, 0.75f,

        // Draw Wall Above Window 1/2
        -3.4f, 4.3f, -4.0f, 0.0f, 0.25f,
        -3.4f, 4.5f, -4.0f, 0.0f, 0.75f,
        0.0f, 4.5f, -4.0f, 1.0, 0.75f,
        // Draw Wall Above Window 2/2
        -3.4f, 4.3f, -4.0f, 0.0f, 0.25f,
        0.0f, 4.3f, -4.0f, 1.0f, 0.25f,
        0.0f, 4.5f, -4.0f, 1.0f, 0.75f,

        // Draw Front Wall Above 1/2
        -3.4f, 4.5f, -4.0f, 0.0f, 0.5f,
        -3.4f, 5.4f, -4.0f, 0.0f, 1.0f,
        4.075f, 5.4f, -4.0f, 1.0f, 1.0f,
        // Draw Front Wall Above 2/2
        -3.4f, 4.5f, -4.0f, 0.0f, 0.5f,
        4.075f, 4.5f, -4.0f, 1.0f, 0.5f,
        4.075f, 5.4f, -4.0f, 1.0f, 1.0f,

        // Draw Wall Between Front & Closet 1/2
        4.075f, -5.4f, -4.0f, 0.0f, 0.0f,
        4.075f, 5.4f, -4.0f, 0.0f, 1.0f,
        4.075f, 5.4f, -3.8f, 0.5f, 1.0f,
        // Draw Wall Between Front & Closet 2/2
        4.075f, -5.4f, -4.0f, 0.0f, 0.0f,
        4.075f, -5.4f, -3.8f, 0.5f, 0.0f,
        4.075f, 5.4f, -3.8f, 0.5f, 1.0f,

        // Draw Wall Above Closet 1/2
        4.075f, 2.85f, -3.8f, 0.5f, 0.5f,
        4.075f, 5.4f, -3.8f, 0.5f, 1.0f,
        4.075f, 5.4f, 0.0f, 1.0f, 1.0f,
        // Draw Wall Above Closet 2/2
        4.075f, 2.85f, -3.8f, 0.5f, 0.5f,
        4.075f, 2.85f, 0.0f, 1.0f, 0.5f,
        4.075f, 5.4f, 0.0f, 1.0f, 1.0f,

        // Draw Front Door Frame - Front Face 1/5, Left 1/2
        0.0f, -5.4f, -3.95f, 0.0f, 0.0f,
        0.0f, 4.25f, -3.95f, 0.0f, 1.0f,
        0.25f, 4.25f, -3.95f, 1.0f, 1.0f,
        // Draw Front Door Frame - Front Face 1/5, Left 2/2
        0.0f, -5.4f, -3.95f, 0.0f, 0.0f,
        0.25f, -5.4f, -3.95f, 1.0f, 0.0f,
        0.25f, 4.25f, -3.95f, 1.0f, 1.0f,
        // Draw Front Door Frame - Front Face 2/5, Upper Left Corner - Lower
        0.0f, 4.25f, -3.95f, 0.0f, 0.0f,
        0.0f, 4.5f, -3.95f, 0.0f, 1.0f,
        0.25f, 4.25f, -3.95f, 1.0f, 0.0f,
        // Draw Front Door Frame - Front Face 2/5, Upper Left Corner - Upper
        0.0f, 4.5f, -3.95f, 0.0f, 1.0f,
        0.25f, 4.5f, -3.95f, 1.0f, 1.0f,
        0.25f, 4.25f, -3.95f, 1.0f, 0.0f,
        // Draw Front Door Frame - Front Face 3/5, Upper 1/2
        0.25f, 4.25f, -3.95f, 0.0f, 0.0f,
        0.25f, 4.5f, -3.95f, 0.0f, 1.0f,
        3.825f, 4.5f, -3.95f, 1.0f, 1.0f,
        // Draw Front Door Frame - Front Face 3/5, Upper 2/2
        0.25f, 4.25f, -3.95f, 0.0f, 0.0f,
        3.825f, 4.25f, -3.95f, 0.0f, 1.0f,
        3.825f, 4.5f, -3.95f, 1.0f, 1.0f,
        // Draw Front Door Frame - Front Face 4/5, Upper Right Corner - Upper
        3.825f, 4.25f, -3.95f, 0.0f, 1.0f,
        3.825f, 4.5f, -3.95f, 0.0f, 1.0f,
        4.075f, 4.5f, -3.95f, 1.0f, 1.0f,
        // Draw Front Door Frame - Front Face 4/5, Upper Right Corner - Lower
        3.825f, 4.25f, -3.95f, 0.0f, 0.0f,
        4.075f, 4.5f, -3.95f, 1.0f, 1.0f,
        4.075f, 4.25f, -3.95f, 1.0f, 0.0f,
        // Draw Front Door Frame - Front Face 5/5, Right 1/2
        3.825f, -5.4f, -3.95f, 0.0f, 0.0f,
        3.825f, 4.25f, -3.95f, 0.0f, 1.0f,
        4.075f, 4.25f, -3.95f, 1.0f, 1.0f,
        // Draw Front Door Frame - Front Face 5/5, Right 2/2
        3.825f, -5.4f, -3.95f, 0.0f, 0.0f,
        4.075f, -5.4f, -3.95f, 1.0f, 0.0f,
        4.075f, 4.25f, -3.95f, 1.0f, 1.0f,

        // Draw Front Door Frame - Inner Left Side 1/2
        0.25f, -5.4f, -4.1f, 0.0f, 0.0f,
        0.25f, -5.4f, -3.95f, 1.0f, 0.0f,
        0.25f, 4.25f, -3.95f, 1.0f, 1.0f,
        // Draw Front Door Frame - Inner Left Side 2/2
        0.25f, -5.4f, -4.1f, 0.0f, 0.0f,
        0.25f, 4.25f, -3.95f, 0.0f, 1.0f,
        0.25f, 4.25f, -3.95f, 1.0f, 1.0f,

        // Draw Front Door Frame - Inner Top Side 1/2
        0.25f, 4.25f, -4.1f, 0.0f, 0.0f,
        0.25f, 4.25f, -3.95f, 0.0f, 1.0f,
        3.825f, 4.25f, -3.95f, 1.0f, 1.0f,
        // Draw Front Door Frame - Inner Top Side 2/2
        0.25f, 4.25f, -4.1f, 0.0f, 0.0f,
        3.825f, 4.25f, -4.1f, 1.0f, 0.0f,
        3.825f, 4.25f, -3.95f, 1.0f, 1.0f,

        // Draw Front Door Frame - Inner Right Side 1/2
        3.825f, 4.25f, -4.1f, 0.0f, 0.0f,
        3.825f, 4.25f, -3.95f, 0.0f, 1.0f,
        3.825f, -5.4f, -3.95f, 1.0f, 1.0f,
        // Draw Front Door Frame - Inner Right Side 2/2
        3.825f, 4.25f, -4.1f, 0.0f, 0.0f,
        3.825f, -5.4f, -4.1f, 0.0f, 1.0f,
        3.825f, -5.4f, -3.95f, 1.0f, 1.0f,


        // Draw Front Door Frame - Left Side 1/2
        0.0f, -5.4f, -3.95f, 0.0f, 0.0f,
        0.0f, -5.4f, -4.0f, 0.0f, 1.0f,
        0.0f, 4.5f, -4.0f, 1.0f, 1.0f,
        // Draw Front Door Frame - Left Side 2/2
        0.0f, -5.4f, -3.95f, 0.0f, 0.0f,
        0.0f, 4.5f, -3.95f, 1.0f, 0.0f,
        0.0f, 4.5f, -4.0f, 1.0f, 1.0f,

        // Draw Front Door Frame - Top Side 1/2
        0.0f, 4.5f, -3.95f, 0.0f, 0.0f,
        0.0f, 4.5f, -4.0f, 0.0f, 1.0f,
        4.075f, 4.5f, -4.0f, 1.0f, 1.0f,
        // Draw Front Door Frame - Top Side 2/2
        0.0f, 4.5f, -3.95f, 0.0f, 0.0f,
        4.075f, 4.5f, -3.95f, 1.0f, 0.0f,
        4.075f, 4.5f, -4.0f, 1.0f, 1.0f,

        // Draw Front Door Frame - Right Side 1/2
        4.075f, 4.5f, -3.95f, 0.0f, 0.0f,
        4.075f, 4.5f, -4.0f, 0.0f, 1.0f,
        4.075f, -5.4f, -4.0f, 1.0f, 1.0f,
        // Draw Front Door Frame - Right Side 2/2
        4.075f, 4.5f, -3.95f, 0.0f, 0.0f,
        4.075f, -5.4f, -3.95f, 1.0f, 0.0f,
        4.075f, -5.4f, -4.0f, 1.0f, 1.0f,


        // Draw Window Frame - Outer Lip - Left Side 1/2
        -2.575f, -3.4f, -4.1f, 0.0f, 0.0f,
        -2.575f, 4.3f, -4.1f, 0.0f, 1.0f,
        -2.575f, 4.3f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Outer Lip - Left Side 2/2
        -2.575f, -3.4f, -4.1f, 0.0f, 0.0f,
        -2.575f, -3.4f, -3.95f, 1.0f, 0.0f,
        -2.575f, 4.3f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Outer Lip - Top Side 1/2
        -2.575f, 4.3f, -3.95f, 0.0f, 0.0f,
        -2.575f, 4.3f, -4.1f, 0.0f, 1.0f,
        -0.275f, 4.3f, -4.1f, 1.0f, 1.0f,
        // Draw Window Frame - Outer Lip - Top Side 2/2
        -2.575f, 4.3f, -3.95f, 0.0f, 0.0f,
        -0.275f, 4.3f, -3.95f, 1.0f, 0.0f,
        -0.275f, 4.3f, -4.1f, 1.0f, 1.0f,

        // Draw Window Frame - Outer Lip - Right Side 1/2
        -0.275f, -3.4f, -3.95f, 0.0f, 0.0f,
        -0.275f, 4.3f, -3.95f, 0.0f, 1.0f,
        -0.275f, 4.3f, -4.1f, 1.0f, 1.0f,
        // Draw Window Frame - Outer Lip - Right Side 2/2
        -0.275f, -3.4f, -3.95f, 0.0f, 0.0f,
        -0.275f, -3.4f, -4.1f, 1.0f, 0.0f,
        -0.275f, 4.3f, -4.1f, 1.0f, 1.0f,

        // Draw Window Frame - Outer Lip - Bottom Side 1/2
        -2.575f, -3.4f, -4.1f, 0.0f, 0.0f,
        -2.575f, -3.4f, -3.95f, 0.0f, 1.0f,
        -0.275f, -3.4f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Outer Lip - Bottom Side 2/2
        -2.575f, -3.4f, -4.1f, 0.0f, 0.0f,
        -0.275f, -3.4f, -4.1f, 1.0f, 0.0f,
        -0.275f, -3.4f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Outer Lip - Front Face 1/8 - Bottom 1/2
        -2.475f, -3.4f, -3.95f, 0.0f, 0.0f,
        -2.475f, -3.3f, -3.95f, 0.0f, 1.0f,
        -0.375f, -3.3f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Outer Lip - Front Face 1/8 - Bottom 2/2
        -2.475f, -3.4f, -3.95f, 0.0f, 0.0f,
        -0.375f, -3.4f, -3.95f, 1.0f, 0.0f,
        -0.375f, -3.3f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Outer Lip - Front Face 2/8 - Bottom Right Corner - Lower
        -0.375f, -3.4f, -3.95f, 0.0f, 0.0f,
        -0.375f, -3.3f, -3.95f, 0.0f, 1.0f,
        -0.275f, -3.4f, -3.95f, 1.0f, 0.0f,
        // Draw Window Frame - Outer Lip - Front Face 2/8 - Bottom Right Corner - Upper
        -0.375f, -3.3f, -3.95f, 0.0f, 1.0f,
        -0.275f, -3.3f, -3.95f, 1.0f, 1.0f,
        -0.275f, -3.4f, -3.95f, 1.0f, 0.0f,

        // Draw Window Frame - Outer Lip - Front Face 3/8 - Right 1/2
        -0.375f, -3.3f, -3.95f, 0.0f, 0.0f,
        -0.375f, 4.2f, -3.95f, 0.0f, 1.0f,
        -0.275f, 4.2f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Outer Lip - Front Face 3/8 - Right 2/2
        -0.375f, -3.3f, -3.95f, 0.0f, 0.0f,
        -0.275f, -3.3f, -3.95f, 1.0f, 0.0f,
        -0.275f, 4.2f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Outer Lip - Front Face 4/8 - Top Right Corner - Upper
        -0.375f, 4.2f, -3.95f, 0.0f, 0.0f,
        -0.375f, 4.3f, -3.95f, 0.0f, 1.0f,
        -0.275f, 4.3f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Outer Lip - Front Face 4/8 - Top Right Corner - Lower
        -0.375f, 4.2f, -3.95f, 0.0f, 0.0f,
        -0.275f, 4.2f, -3.95f, 1.0f, 0.0f,
        -0.275f, 4.3f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Outer Lip - Front Face 5/8 - Top 1/2
        -2.475f, 4.2f, -3.95f, 0.0f, 0.0f,
        -2.475f, 4.3f, -3.95f, 0.0f, 1.0f,
        -0.375f, 4.3f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Outer Lip - Front Face 5/8 - Top 1/2
        -2.475f, 4.2f, -3.95f, 0.0f, 0.0f,
        -0.375f, 4.2f, -3.95f, 1.0f, 0.0f,
        -0.375f, 4.3f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Outer Lip - Front Face 6/8 - Top Left Corner - Lower
        -2.575f, 4.2f, -3.95f, 0.0f, 0.0f,
        -2.575f, 4.3f, -3.95f, 0.0f, 1.0f,
        -2.475f, 4.2f, -3.95f, 1.0f, 0.0f,
        // Draw Window Frame - Outer Lip - Front Face 6/8 - Top Left Corner - Upper
        -2.575f, 4.3f, -3.95f, 0.0f, 1.0f,
        -2.475f, 4.3f, -3.95f, 1.0f, 1.0f,
        -2.475f, 4.2f, -3.95f, 1.0f, 0.0f,

        // Draw Window Frame - Outer Lip - Front Face 7/8 - Left 1/2
        -2.575f, -3.3f, -3.95f, 0.0f, 0.0f,
        -2.575f, 4.2f, -3.95f, 0.0f, 1.0f,
        -2.475f, 4.2f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Outer Lip - Front Face 7/8 - Left 2/2
        -2.575f, -3.3f, -3.95f, 0.0f, 0.0f,
        -2.475f, -3.3f, -3.95f, 1.0f, 0.0f,
        -2.475f, 4.2f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Outer Lip - Front Face 8/8 - Bottom Left Corner - Upper
        -2.575f, -3.4f, -3.95f, 0.0f, 0.0f,
        -2.575f, -3.3f, -3.95f, 0.0f, 1.0f,
        -2.475f, -3.3f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Outer Lip - Front Face 8/8 - Bottom Left Corner - Lower
        -2.575f, -3.4f, -3.95f, 0.0f, 0.0f,
        -2.475f, -3.4f, -3.95f, 1.0f, 0.0f,
        -2.475f, -3.3f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Lip - Front Face 1/8 - Bottom 1/2
        -2.425f, -3.3f, -3.95f, 0.0f, 0.0f,
        -2.425f, -3.25f, -3.9f, 0.0f, 1.0f,
        -0.425f, -3.25f, -3.9f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Lip - Front Face 1/8 - Bottom 2/2
        -2.425f, -3.3f, -3.95f, 0.0f, 0.0f,
        -0.425f, -3.3f, -3.95f, 1.0f, 0.0f,
        -0.425f, -3.25f, -3.9f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Lip - Front Face 2/8 - Right 1/2
        -0.425f, -3.25f, -3.9f, 0.0f, 0.0f,
        -0.425f, 4.15f, -3.9f, 0.0f, 1.0f,
        -0.375f, 4.15f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Lip - Front Face 2/8 - Right 2/2
        -0.425f, -3.25f, -3.9f, 0.0f, 0.0f,
        -0.375f, -3.25f, -3.95f, 1.0f, 0.0f,
        -0.375f, 4.15f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Lip - Front Face 3/8 - Left 1/2
        -2.475f, -3.25f, -3.95f, 0.0f, 0.0f,
        -2.475f, 4.15f, -3.95f, 0.0f, 1.0f,
        -2.425f, 4.15f, -3.9f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Lip - Front Face 3/8 - Left 2/2
        -2.475f, -3.25f, -3.95f, 0.0f, 0.0f,
        -2.425f, -3.25f, -3.9f, 1.0f, 0.0f,
        -2.425f, 4.15f, -3.9f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Lip - Front Face 4/8 - Top 1/2
        -2.425f, 4.15f, -3.9f, 0.0f, 0.0f,
        -2.425f, 4.2f, -3.95f, 0.0f, 1.0f,
        -0.425f, 4.2f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Lip - Front Face 4/8 - Top 2/2
        -2.425f, 4.15f, -3.9f, 0.0f, 0.0f,
        -0.425f, 4.15f, -3.9f, 1.0f, 0.0f,
        -0.425f, 4.2f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Lip - Front Face 5/8 - Bottom Right - Lower
        -0.425f, -3.3f, -3.95f, 0.0f, 0.0f,
        -0.425f, -3.25f, -3.9f, 0.0f, 1.0f,
        -0.375f, -3.3f, -3.95f, 1.0f, 0.0f,
        // Draw Window Frame - Inner Lip - Front Face 5/8 - Bottom Right - Upper
        -0.425f, -3.25f, -3.9f, 0.0f, 1.0f,
        -0.375f, -3.25f, -3.95f, 1.0f, 1.0f,
        -0.375f, -3.3f, -3.95f, 1.0f, 0.0f,

        // Draw Window Frame - Inner Lip - Front Face 6/8 - Top Right - Upper
        -0.425f, 4.15f, -3.9f, 0.0f, 0.0f,
        -0.425f, 4.2f, -3.95f, 0.0f, 1.0f,
        -0.375f, 4.2f, -3.95f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Lip - Front Face 6/8 - Top Right - Lower
        -0.425f, 4.15f, -3.9f, 0.0f, 0.0f,
        -0.375f, 4.15f, -3.95f, 1.0f, 0.0f,
        -0.375f, 4.2f, -3.95f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Lip - Front Face 7/8 - Top Left - Lower
        -2.475f, 4.15f, -3.95f, 0.0f, 0.0f,
        -2.475f, 4.2f, -3.95f, 0.0f, 1.0f,
        -2.425f, 4.15f, -3.9f, 1.0f, 0.0f,
        // Draw Window Frame - Inner Lip - Front Face 7/8 - Top Left - Upper
        -2.475f, 4.2f, -3.95f, 0.0f, 1.0f,
        -2.425f, 4.2f, -3.95f, 1.0f, 1.0f,
        -2.425f, 4.15f, -3.9f, 1.0f, 0.0f,

        // Draw Window Frame - Inner Lip - Front Face 8/8 - Bottom Left - Upper
        -2.475f, -3.3f, -3.95f, 0.0f, 0.0f,
        -2.475f, -3.25f, -3.95f, 0.0f, 1.0f,
        -2.425f, -3.25f, -3.9f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Lip - Front Face 8/8 - Bottom Left - Lower
        -2.475f, -3.3f, -3.95f, 0.0f, 0.0f,
        -2.425f, -3.3f, -3.95f, 1.0f, 0.0f,
        -2.425f, -3.25f, -3.9f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Extended - Front Face 1/8 - Bottom 1/2
        -2.375f, -3.25f, -3.9f, 0.0f, 0.0f,
        -2.375f, -3.2f, -3.875f, 0.0f, 1.0f,
        -0.475f, -3.2f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Extended - Front Face 1/8 - Bottom 2/2
        -2.375f, -3.25f, -3.9f, 0.0f, 0.0f,
        -0.475f, -3.25f, -3.9f, 1.0f, 0.0f,
        -0.475f, -3.2f, -3.875f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Extended - Front Face 2/8 - Right 1/2
        -0.475f, -3.2f, -3.875f, 0.0f, 0.0f,
        -0.475f, 4.1f, -3.875f, 0.0f, 1.0f,
        -0.425f, 4.1f, -3.9f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Extended - Front Face 2/8 - Right 2/2
        -0.475f, -3.2f, -3.875f, 0.0f, 0.0f,
        -0.425f, -3.2f, -3.9f, 1.0f, 0.0f,
        -0.425f, 4.1f, -3.9f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Extended - Front Face 3/8 - Top 1/2
        -2.375f, 4.1f, -3.875f, 0.0f, 0.0f,
        -2.375f, 4.15f, -3.9f, 0.0f, 1.0f,
        -0.475f, 4.15f, -3.9f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Extended - Front Face 3/8 - Top 2/2
        -2.375f, 4.1f, -3.875f, 0.0f, 0.0f,
        -0.475f, 4.1f, -3.875f, 1.0f, 0.0f,
        -0.475f, 4.15f, -3.9f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Extended - Front Face 4/8 - Left 1/2
        -2.425f, -3.2f, -3.9f, 0.0f, 0.0f,
        -2.425f, 4.1f, -3.9f, 0.0f, 1.0f,
        -2.375f, 4.1f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Extended - Front Face 4/8 - Left 2/2
        -2.425f, -3.2f, -3.9f, 0.0f, 0.0f,
        -2.375f, -3.2f, -3.875f, 1.0f, 0.0f,
        -2.375f, 4.1f, -3.875f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Extended - Front Face 5/8 - Bottom Right - Lower
        -0.475f, -3.25f, -3.9f, 0.0f, 0.0f,
        -0.475f, -3.2f, -3.875f, 0.0f, 1.0f,
        -0.425f, -3.25f, -3.9f, 1.0f, 0.0f,
        // Draw Window Frame - Inner Extended - Front Face 5/8 - Bottom Right - Upper
        -0.475f, -3.2f, -3.875f, 0.0f, 1.0f,
        -0.425f, -3.2f, -3.9f, 1.0f, 1.0f,
        -0.425f, -3.25f, -3.9f, 1.0f, 0.0f,

        // Draw Window Frame - Inner Extended - Front Face 6/8 - Top Right - Upper
        -0.475f, 4.1f, -3.875f, 0.0f, 0.0f,
        -0.475f, 4.15f, -3.9f, 0.0f, 1.0f,
        -0.425f, 4.15f, -3.9f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Extended - Front Face 6/8 - Top Right - Lower
        -0.475f, 4.1f, -3.875f, 0.0f, 0.0f,
        -0.425f, 4.1f, -3.9f, 1.0f, 0.0f,
        -0.425f, 4.15f, -3.9f, 1.0f, 1.0f,

        // Draw Window Frame - Inner Extended - Front Face 7/8 - Top Left - Lower
        -2.425f, 4.1f, -3.9f, 0.0f, 0.0f,
        -2.425f, 4.15f, -3.9f, 0.0f, 1.0f,
        -2.375f, 4.1f, -3.875f, 1.0f, 0.0f,
        // Draw Window Frame - Inner Extended - Front Face 7/8 - Top Left - Upper
        -2.425f, 4.15f, -3.9f, 0.0f, 1.0f,
        -2.375f, 4.15f, -3.9f, 1.0f, 1.0f,
        -2.375f, 4.1f, -3.875f, 1.0f, 0.0f,

        // Draw Window Frame - Inner Extended - Front Face 8/8 - Bottom Left - Upper
        -2.425f, -3.25f, -3.9f, 0.0f, 0.0f,
        -2.425f, -3.2f, -3.9f, 0.0f, 1.0f,
        -2.375f, -3.2f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Inner Extended - Front Face 8/8 - Bottom Left - Lower
        -2.425f, -3.25f, -3.9f, 0.0f, 0.0f,
        -2.375f, -3.25f, -3.9f, 1.0f, 0.0f,
        -2.375f, -3.2f, -3.875f, 1.0f, 1.0f,

        // Draw Window Frame - Inner - Front Face 1/8 - Bottom 1/2
        -2.275f, -3.2f, -3.875f, 0.0f, 0.0f,
        -2.275f, -3.1f, -3.875f, 0.0f, 1.0f,
        -0.575f, -3.1f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Inner - Front Face 1/8 - Bottom 2/2
        -2.275f, -3.2f, -3.875f, 0.0f, 0.0f,
        -0.575f, -3.2f, -3.875f, 1.0f, 0.0f,
        -0.575f, -3.1f, -3.875f, 1.0f, 1.0f,

        // Draw Window Frame - Inner - Front Face 2/8 - Right 1/2
        -0.575f, -3.1f, -3.875f, 0.0f, 0.0f,
        -0.575f, 4.0f, -3.875f, 0.0f, 1.0f,
        -0.475f, 4.0f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Inner - Front Face 2/8 - Right 2/2
        -0.575f, -3.1f, -3.875f, 0.0f, 0.0f,
        -0.475f, -3.1f, -3.875f, 1.0f, 0.0f,
        -0.475f, 4.0f, -3.875f, 1.0f, 1.0f,

        // Draw Window Frame - Inner - Front Face 3/8 - Top 1/2
        -2.275f, 4.0f, -3.875f, 0.0f, 0.0f,
        -2.275f, 4.1f, -3.875f, 0.0f, 1.0f,
        -0.575f, 4.1f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Inner - Front Face 3/8 - Top 2/2
        -2.275f, 4.0f, -3.875f, 0.0f, 0.0f,
        -0.575f, 4.0f, -3.875f, 1.0f, 0.0f,
        -0.575f, 4.1f, -3.875f, 1.0f, 1.0f,

        // Draw Window Frame - Inner - Front Face 4/8 - Left 1/2
        -2.375f, -3.1f, -3.875f, 0.0f, 0.0f,
        -2.375f, 4.0f, -3.875f, 0.0f, 1.0f,
        -2.275f, 4.0f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Inner - Front Face 4/8 - Left 2/2
        -2.375f, -3.1f, -3.875f, 0.0f, 0.0f,
        -2.275f, -3.1f, -3.875f, 1.0f, 0.0f,
        -2.275f, 4.0f, -3.875f, 1.0f, 1.0f,

        // Draw Window Frame - Inner - Front Face 5/8 - Bottom Right - Lower
        -0.575f, -3.2f, -3.875f, 0.0f, 0.0f,
        -0.575f, -3.1f, -3.875f, 0.0f, 1.0f,
        -0.475f, -3.2f, -3.875f, 1.0f, 0.0f,
        // Draw Window Frame - Inner - Front Face 5/8 - Bottom Right - Upper
        -0.575f, -3.1f, -3.875f, 0.0f, 1.0f,
        -0.475f, -3.1f, -3.875f, 1.0f, 1.0f,
        -0.475f, -3.2f, -3.875f, 1.0f, 0.0f,

        // Draw Window Frame - Inner - Front Face 6/8 - Top Right - Upper
        -0.575f, 4.0f, -3.875f, 0.0f, 0.0f,
        -0.575f, 4.1f, -3.875f, 0.0f, 1.0f,
        -0.475f, 4.1f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Inner - Front Face 6/8 - Top Right - Lower
        -0.575f, 4.0f, -3.875f, 0.0f, 0.0f,
        -0.475f, 4.0f, -3.875f, 1.0f, 0.0f,
        -0.475f, 4.1f, -3.875f, 1.0f, 1.0f,

        // Draw Window Frame - Inner - Front Face 7/8 - Top Left - Lower
        -2.375f, 4.000f, -3.875f, 0.0f, 0.0f,
        -2.375f, 4.100f, -3.875f, 0.0f, 1.0f,
        -2.275f, 4.000f, -3.875f, 1.0f, 0.0f,
        // Draw Window Frame - Inner - Front Face 7/8 - Top Left - Upper
        -2.375f, 4.100f, -3.875f, 0.0f, 1.0f,
        -2.275f, 4.100f, -3.875f, 1.0f, 1.0f,
        -2.275f, 4.000f, -3.875f, 1.0f, 0.0f,

        // Draw Window Frame - Inner - Front Face 8/8 - Bottom Left - Upper
        -2.375f, -3.2f, -3.875f, 0.0f, 0.0f,
        -2.375f, -3.1f, -3.875f, 0.0f, 1.0f,
        -2.275f, -3.1f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Inner - Front Face 8/8 - Bottom Left - Lower
        -2.375f, -3.2f, -3.875f, 0.0f, 0.0f,
        -2.275f, -3.2f, -3.875f, 1.0f, 0.0f,
        -2.275f, -3.1f, -3.875f, 1.0f, 1.0f,
        
        // Draw Window Frame - Side Panel - Left 1/2
        -2.26f, -3.1f, -3.875f, 0.0f, 0.0f,
        -2.26f, 4.0f, -3.875f, 0.0f, 1.0f,
        -2.065f, 4.0f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Side Panel - Left 2/2
        -2.26f, -3.1f, -3.875f, 0.0f, 0.0f,
        -2.065f, -3.1f, -3.875f, 1.0f, 0.0f,
        -2.065f, 4.0f, -3.875f, 1.0f, 1.0f,

        // Draw Window Frame - Side Panel - Right 1/2
        -0.785f, -3.1f, -3.875f, 0.0f, 0.0f,
        -0.785f, 4.0f, -3.875f, 0.0f, 1.0f,
        -0.59f, 4.0f, -3.875f, 1.0f, 1.0f,
        // Draw Window Frame - Side Panel - Right 2/2
        -0.785f, -3.1f, -3.875f, 0.0f, 0.0f,
        -0.59f, -3.1f, -3.875f, 1.0f, 0.0f,
        -0.59f, 4.0f, -3.875f, 1.0f, 1.0f,

        // Draw Window Frame - Side Panel - Top 1/2
        -2.065f, 3.57f, -3.9f, 0.0f, 0.0f,
        -2.065f, 3.97f, -3.9f, 0.0f, 1.0f,
        -0.785f, 3.97f, -3.9f, 1.0f, 1.0f,
        // Draw Window Frame - Side Panel - Top 2/2
        -2.065f, 3.57f, -3.9f, 0.0f, 0.0f,
        -0.785f, 3.57f, -3.9f, 1.0f, 0.0f,
        -0.785f, 3.97f, -3.9f, 1.0f, 1.0f,

        // Draw Window Frame - Side Panel - Bottom 1/2
        -2.065f, -3.13f, -3.9f, 0.0f, 0.0f,
        -2.065f, -2.73f, -3.9f, 0.0f, 1.0f,
        -0.785f, -2.73f, -3.9f, 1.0f, 1.0f,
        // Draw Window Frame - Side Panel - Bottom 2/2
        -2.065f, -3.13f, -3.9f, 0.0f, 0.0f,
        -0.785f, -3.13f, -3.9f, 1.0f, 0.0f,
        -0.785f, -2.73f, -3.9f, 1.0f, 1.0f,

        // Draw Window Frame - Side Panel - Middle 1/2
        -2.065f, 0.27f, -3.9f, 0.0f, 0.0f,
        -2.065f, 0.57f, -3.9f, 0.0f, 1.0f,
        -0.785f, 0.57f, -3.9f, 1.0f, 1.0f,
        // Draw Window Frame - Side Panel - Middle 2/2
        -2.065f, 0.27f, -3.9f, 0.0f, 0.0f,
        -0.785f, 0.27f, -3.9f, 1.0f, 0.0f,
        -0.785f, 0.57f, -3.9f, 1.0f, 1.0f,



        // Draw Front Door - Front
        -3.575f, -4.75f, -0.05f, 0.0f, 0.0f,
        -3.575f, 4.79f, -0.05f, 0.0f, 1.0f,
        0.0f, 4.79f, -0.05f, 1.0f, 1.0f,

        -3.575f, -4.75f, -0.05f, 0.0f, 0.0f,
        0.0f, -4.75f, -0.05f, 1.0f, 0.0f,
        0.0f, 4.79f, -0.05f, 1.0f, 1.0f,

        // Draw Front Door - Left
        -3.575f, -4.75f, -0.25f, 0.0f, 0.0f,
        -3.575f, 4.79f, -0.25f, 0.0f, 1.0f,
        -3.575f, 4.79f, -0.05f, 1.0f, 1.0f,

        -3.575f, -4.75f, -0.25f, 0.0f, 0.0f,
        -3.575f, -4.75f, -0.05f, 1.0f, 0.0f,
        -3.575f, 4.79f, -0.05f, 1.0f, 1.0f,

        // Draw Front Door - Right
        0.0f, -4.75f, -0.05f, 0.0f, 0.0f,
        0.0f, 4.79f, -0.05f, 0.0f, 1.0f,
        0.0f, 4.79f, -0.25f, 1.0f, 1.0f,

        0.0f, -4.75f, -0.05f, 0.0f, 0.0f,
        0.0f, -4.75f, -0.25f, 1.0f, 0.0f,
        0.0f, 4.79f, -0.25f, 1.0f, 1.0f,

        // Draw Front Door - Top
        -3.575f, 4.79f, -0.05f, 0.0f, 0.0f,
        -3.575f, 4.79f, -0.25f, 0.0f, 1.0f,
        0.0f, 4.79f, -0.25f, 1.0f, 1.0f,

        -3.575f, 4.79f, -0.05f, 0.0f, 0.0f,
        0.0f, 4.79f, -0.05f, 1.0f, 0.0f,
        0.0f, 4.79f, -0.25f, 1.0f, 1.0f,


        // Draw Front Door - Back
        0.0f, -4.75f, -0.25f, 0.0f, 0.0f,
        0.0f, 4.79f, -0.25f, 0.0f, 1.0f,
        -3.575f, 4.79f, -0.25f, 1.0f, 1.0f,

        0.0f, -4.75f, -0.25f, 0.0f, 0.0f,
        -3.575f, -4.75f, -0.25f, 1.0f, 0.0f,
        -3.575f, 4.79f, -0.25f, 1.0f, 1.0f,


        // Draw Front Door Seal - Front
        -3.575f, -4.8, -0.035f, 0.0f, 0.0f,
        -3.575f, -4.725f, -0.035f, 0.0f, 1.0f,
        0.0f, -4.725f, -0.035f, 1.0f, 1.0f,

        -3.575f, -4.8, -0.035f, 0.0f, 0.0f,
        0.0f, -4.8, -0.035f, 1.0f, 0.0f,
        0.0f, -4.725f, -0.035f, 1.0f, 1.0f,

        // Draw Front Door Seal - Back
        0.0f, -4.8f, -0.265f, 0.0f, 0.0f,
        0.0f, -4.725f, -0.265f, 0.0f, 1.0f,
        -3.575f, -4.725f, -0.265f, 1.0f, 1.0f,

        0.0f, -4.8f, -0.265f, 0.0f, 0.0f,
        -3.575f, -4.8f, -0.265f, 1.0f, 0.0f,
        -3.575f, -4.725f, -0.265f, 1.0f, 1.0f,

        // Draw Wall Guard Below Window 1/3 - Front
        -3.4f, -5.4f, -3.925f, 0.0f, 0.0f,
        -3.4f, -5.2f, -3.925f, 0.0f, 1.0f,
        0.0f, -5.2f, -3.925f, 1.0f, 1.0f,
        
        -3.4f, -5.4f, -3.925f, 0.0f, 0.0f,
        0.0f, -5.4f, -3.925f, 1.0f, 0.0f,
        0.0f, -5.2f, -3.925f, 1.0f, 1.0f,

        // Draw Wall Guard Below Window 2/3 - Top
        -3.4f, -5.2f, -3.925f, 0.0f, 0.0f,
        -3.4f, -5.2f, -4.0f, 0.0f, 1.0f,
        0.0f, -5.2f, -4.0f, 1.0f, 1.0f,
        
        -3.4f, -5.2f, -3.925f, 0.0f, 0.0f,
        0.0f, -5.2f, -3.925f, 1.0f, 0.0f,
        0.0f, -5.2f, -4.0f, 1.0f, 1.0f,

        // Draw Wall Guard Below Window 2/3 - Right
        0.0f, -5.4f, -3.925f, 0.0f, 0.0f,
        0.0f, -5.2f, -3.925f, 0.0f, 1.0f,
        0.0f, -5.2f, -4.0f, 1.0f, 1.0f,
        
        0.0f, -5.4f, -3.925f, 0.0f, 0.0f,
        0.0f, -5.4f, -4.0f, 1.0f, 0.0f,
        0.0f, -5.2f, -4.0f, 1.0f, 1.0f,



        // Draw Frame Above Closet 1/2
        4.075f, 3.0f, -3.8f, 0.5f, 1.0f,
        4.075f, 3.0f, 0.0f, 1.0f, 1.0f,
        4.075f, 2.75f, -3.8f, 0.5f, 0.5f,   
        // Draw Frame Above Closet 2/2
        4.075f, 2.75f, -3.8f, 0.5f, 0.5f,
        4.075f, 2.75f, 0.0f, 1.0f, 0.5f,
        4.075f, 3.0f, 0.0f, 1.0f, 1.0f,

        // Draw Frame To the Side of Closet 1/2
        4.075f, 3.0f, -3.8f, 0.5f, 1.0f,
        4.075f, 3.0f, -3.5f, 1.0f, 1.0f,
        4.075f, -5.5f, -3.8f, 0.5f, 0.5f,   
        // Draw Frame To the Side of Closet 2/2
        4.075f, -5.5f, -3.8f, 0.5f, 0.5f,
        4.075f, -5.5f, -3.5f, 1.0f, 0.5f,
        4.075f, 3.0f, -3.5f, 1.0f, 1.0f,

        // Draw Closet Door 1/2
        4.075f, 2.7f, -3.4f, 0.5f, 1.0f,
        4.075f, 2.9f, 3.0f, 1.0f, 1.0f,
        4.075f, -5.7f, -3.4f, 0.5f, 0.5f,   
        // Draw Closet Door 2/2
        4.075f, -5.5f, -3.4f, 0.5f, 0.5f,
        4.075f, -5.5f, 3.0f, 1.0f, 0.5f,
        4.075f, 2.7f, 3.0f, 1.0f, 1.0f,

        
        // Blender - Window Blind
        0.637499988079071f, 2.7866009943977588e-08f, 0.02499997243285179f, 0.0f, 0.0f,
        -0.637499988079071f, 0.034140780568122864f, 0.0245196595788002f, 0.0f, 1.0f,
        -0.637499988079071f, -2.7866009943977588e-08f, 0.02500002831220627f, 1.0f, 1.0f,
        0.637499988079071f, 0.03414083272218704f, 0.024519603699445724f, 0.0f, 0.0f,
        -0.637499988079071f, 0.06696957349777222f, 0.023097015917301178f, 0.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, 0.0245196595788002f, 1.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 0.0f, 0.0f,
        -0.637499988079071f, 0.09722476452589035f, 0.020786767825484276f, 0.0f, 1.0f,
        -0.637499988079071f, 0.06696957349777222f, 0.023097015917301178f, 1.0f, 1.0f,
        0.637499988079071f, 0.09722482413053513f, 0.0207867119461298f, 0.0f, 0.0f,
        -0.637499988079071f, 0.12374365329742432f, 0.01767769828438759f, 0.0f, 1.0f,
        -0.637499988079071f, 0.09722476452589035f, 0.020786767825484276f, 1.0f, 1.0f,
        0.637499988079071f, 0.12374371290206909f, 0.01767764240503311f, 0.0f, 0.0f,
        -0.637499988079071f, 0.14550714194774628f, 0.013889283873140812f, 0.0f, 1.0f,
        -0.637499988079071f, 0.12374365329742432f, 0.01767769828438759f, 1.0f, 1.0f,
        0.637499988079071f, 0.14550720155239105f, 0.013889227993786335f, 0.0f, 0.0f,
        -0.637499988079071f, 0.16167888045310974f, 0.009567114524543285f, 0.0f, 1.0f,
        -0.637499988079071f, 0.14550714194774628f, 0.013889283873140812f, 1.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, 0.009567058645188808f, 0.0f, 0.0f,
        -0.637499988079071f, 0.1716373860836029f, 0.004877286031842232f, 0.0f, 1.0f,
        -0.637499988079071f, 0.16167888045310974f, 0.009567114524543285f, 1.0f, 1.0f,
        0.637499988079071f, 0.17163744568824768f, 0.004877230152487755f, 0.0f, 0.0f,
        -0.637499988079071f, 0.17499996721744537f, 2.7866009943977588e-08f, 0.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, 0.004877286031842232f, 1.0f, 1.0f,
        0.637499988079071f, 0.17500002682209015f, -2.7866009943977588e-08f, 0.0f, 0.0f,
        -0.637499988079071f, 0.1716373860836029f, -0.004877230152487755f, 0.0f, 1.0f,
        -0.637499988079071f, 0.17499996721744537f, 2.7866009943977588e-08f, 1.0f, 1.0f,
        0.637499988079071f, 0.17163744568824768f, -0.004877286031842232f, 0.0f, 0.0f,
        -0.637499988079071f, 0.16167888045310974f, -0.009567058645188808f, 0.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, -0.004877230152487755f, 1.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, -0.009567114524543285f, 0.0f, 0.0f,
        -0.637499988079071f, 0.14550714194774628f, -0.013889227993786335f, 0.0f, 1.0f,
        -0.637499988079071f, 0.16167888045310974f, -0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, 0.14550720155239105f, -0.013889283873140812f, 0.0f, 0.0f,
        -0.637499988079071f, 0.12374365329742432f, -0.01767764240503311f, 0.0f, 1.0f,
        -0.637499988079071f, 0.14550714194774628f, -0.013889227993786335f, 1.0f, 1.0f,
        0.637499988079071f, 0.12374371290206909f, -0.01767769828438759f, 0.0f, 0.0f,
        -0.637499988079071f, 0.09722476452589035f, -0.0207867119461298f, 0.0f, 1.0f,
        -0.637499988079071f, 0.12374365329742432f, -0.01767764240503311f, 1.0f, 1.0f,
        0.637499988079071f, 0.09722482413053513f, -0.020786767825484276f, 0.0f, 0.0f,
        -0.637499988079071f, 0.06696957349777222f, -0.0230969600379467f, 0.0f, 1.0f,
        -0.637499988079071f, 0.09722476452589035f, -0.0207867119461298f, 1.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, -0.023097015917301178f, 0.0f, 0.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 0.0f, 1.0f,
        -0.637499988079071f, 0.06696957349777222f, -0.0230969600379467f, 1.0f, 1.0f,
        0.637499988079071f, 0.03414083272218704f, -0.0245196595788002f, 0.0f, 0.0f,
        -0.637499988079071f, -2.7866009943977588e-08f, -0.02499997243285179f, 0.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 1.0f, 1.0f,
        0.637499988079071f, 2.7866009943977588e-08f, -0.02500002831220627f, 0.0f, 0.0f,
        -0.637499988079071f, -0.03414083272218704f, -0.024519603699445724f, 0.0f, 1.0f,
        -0.637499988079071f, -2.7866009943977588e-08f, -0.02499997243285179f, 1.0f, 1.0f,
        0.637499988079071f, -0.034140780568122864f, -0.0245196595788002f, 0.0f, 0.0f,
        -0.637499988079071f, -0.06696963310241699f, -0.0230969600379467f, 0.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, -0.024519603699445724f, 1.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, -0.023097015917301178f, 0.0f, 0.0f,
        -0.637499988079071f, -0.09722482413053513f, -0.0207867119461298f, 0.0f, 1.0f,
        -0.637499988079071f, -0.06696963310241699f, -0.0230969600379467f, 1.0f, 1.0f,
        0.637499988079071f, -0.09722476452589035f, -0.020786767825484276f, 0.0f, 0.0f,
        -0.637499988079071f, -0.12374371290206909f, -0.01767764240503311f, 0.0f, 1.0f,
        -0.637499988079071f, -0.09722482413053513f, -0.0207867119461298f, 1.0f, 1.0f,
        0.637499988079071f, -0.12374365329742432f, -0.01767769828438759f, 0.0f, 0.0f,
        -0.637499988079071f, -0.14550720155239105f, -0.013889227993786335f, 0.0f, 1.0f,
        -0.637499988079071f, -0.12374371290206909f, -0.01767764240503311f, 1.0f, 1.0f,
        0.637499988079071f, -0.14550714194774628f, -0.013889283873140812f, 0.0f, 0.0f,
        -0.637499988079071f, -0.16167894005775452f, -0.009567058645188808f, 0.0f, 1.0f,
        -0.637499988079071f, -0.14550720155239105f, -0.013889227993786335f, 1.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, -0.009567114524543285f, 0.0f, 0.0f,
        -0.637499988079071f, -0.17163744568824768f, -0.004877230152487755f, 0.0f, 1.0f,
        -0.637499988079071f, -0.16167894005775452f, -0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, -0.1716373860836029f, -0.004877286031842232f, 0.0f, 0.0f,
        -0.637499988079071f, -0.17500002682209015f, 2.7866009943977588e-08f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, -0.004877230152487755f, 1.0f, 1.0f,
        0.637499988079071f, -0.17499996721744537f, -2.7866009943977588e-08f, 0.0f, 0.0f,
        -0.637499988079071f, -0.17163744568824768f, 0.004877286031842232f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17500002682209015f, 2.7866009943977588e-08f, 1.0f, 1.0f,
        0.637499988079071f, -0.1716373860836029f, 0.004877230152487755f, 0.0f, 0.0f,
        -0.637499988079071f, -0.16167894005775452f, 0.009567114524543285f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, 0.004877286031842232f, 1.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 0.0f, 0.0f,
        -0.637499988079071f, -0.14550720155239105f, 0.013889283873140812f, 0.0f, 1.0f,
        -0.637499988079071f, -0.16167894005775452f, 0.009567114524543285f, 1.0f, 1.0f,
        0.637499988079071f, -0.14550714194774628f, 0.013889227993786335f, 0.0f, 0.0f,
        -0.637499988079071f, -0.12374371290206909f, 0.01767769828438759f, 0.0f, 1.0f,
        -0.637499988079071f, -0.14550720155239105f, 0.013889283873140812f, 1.0f, 1.0f,
        0.637499988079071f, -0.12374365329742432f, 0.01767764240503311f, 0.0f, 0.0f,
        -0.637499988079071f, -0.09722482413053513f, 0.020786767825484276f, 0.0f, 1.0f,
        -0.637499988079071f, -0.12374371290206909f, 0.01767769828438759f, 1.0f, 1.0f,
        0.637499988079071f, -0.09722476452589035f, 0.0207867119461298f, 0.0f, 0.0f,
        -0.637499988079071f, -0.06696963310241699f, 0.023097015917301178f, 0.0f, 1.0f,
        -0.637499988079071f, -0.09722482413053513f, 0.020786767825484276f, 1.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, -0.023097015917301178f, 0.0f, 0.0f,
        0.637499988079071f, 0.16167894005775452f, -0.009567114524543285f, 0.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 1.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, 0.0230969600379467f, 0.0f, 0.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 0.0f, 1.0f,
        -0.637499988079071f, -0.06696963310241699f, 0.023097015917301178f, 1.0f, 1.0f,
        0.637499988079071f, -0.034140780568122864f, 0.024519603699445724f, 0.0f, 0.0f,
        -0.637499988079071f, -2.7866009943977588e-08f, 0.02500002831220627f, 0.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 1.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 0.0f, 0.0f,
        -0.637499988079071f, -0.17163744568824768f, -0.004877230152487755f, 0.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 1.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 0.0f, 0.0f,
        0.637499988079071f, 0.03414083272218704f, 0.024519603699445724f, 0.0f, 1.0f,
        0.637499988079071f, 2.7866009943977588e-08f, 0.02499997243285179f, 1.0f, 1.0f,
        0.637499988079071f, 2.7866009943977588e-08f, 0.02499997243285179f, 0.0f, 0.0f,
        0.637499988079071f, -0.034140780568122864f, 0.024519603699445724f, 0.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 1.0f, 1.0f,
        0.637499988079071f, -0.034140780568122864f, 0.024519603699445724f, 0.0f, 0.0f,
        0.637499988079071f, -0.06696957349777222f, 0.0230969600379467f, 0.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 1.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, 0.0230969600379467f, 0.0f, 0.0f,
        0.637499988079071f, -0.09722476452589035f, 0.0207867119461298f, 0.0f, 1.0f,
        0.637499988079071f, -0.12374365329742432f, 0.01767764240503311f, 1.0f, 1.0f,
        0.637499988079071f, -0.12374365329742432f, 0.01767764240503311f, 0.0f, 0.0f,
        0.637499988079071f, -0.14550714194774628f, 0.013889227993786335f, 0.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 0.0f, 0.0f,
        0.637499988079071f, -0.1716373860836029f, 0.004877230152487755f, 0.0f, 1.0f,
        0.637499988079071f, -0.17499996721744537f, -2.7866009943977588e-08f, 1.0f, 1.0f,
        0.637499988079071f, -0.17499996721744537f, -2.7866009943977588e-08f, 0.0f, 0.0f,
        0.637499988079071f, -0.1716373860836029f, -0.004877286031842232f, 0.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, -0.1716373860836029f, -0.004877286031842232f, 0.0f, 0.0f,
        0.637499988079071f, -0.16167888045310974f, -0.009567114524543285f, 0.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, -0.009567114524543285f, 0.0f, 0.0f,
        0.637499988079071f, -0.14550714194774628f, -0.013889283873140812f, 0.0f, 1.0f,
        0.637499988079071f, -0.12374365329742432f, -0.01767769828438759f, 1.0f, 1.0f,
        0.637499988079071f, -0.12374365329742432f, -0.01767769828438759f, 0.0f, 0.0f,
        0.637499988079071f, -0.09722476452589035f, -0.020786767825484276f, 0.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, -0.023097015917301178f, 1.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, -0.023097015917301178f, 0.0f, 0.0f,
        0.637499988079071f, -0.034140780568122864f, -0.0245196595788002f, 0.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, -0.023097015917301178f, 1.0f, 1.0f,
        0.637499988079071f, -0.034140780568122864f, -0.0245196595788002f, 0.0f, 0.0f,
        0.637499988079071f, 2.7866009943977588e-08f, -0.02500002831220627f, 0.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, -0.023097015917301178f, 1.0f, 1.0f,
        0.637499988079071f, 2.7866009943977588e-08f, -0.02500002831220627f, 0.0f, 0.0f,
        0.637499988079071f, 0.03414083272218704f, -0.0245196595788002f, 0.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, -0.023097015917301178f, 1.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, -0.023097015917301178f, 0.0f, 0.0f,
        0.637499988079071f, 0.09722482413053513f, -0.020786767825484276f, 0.0f, 1.0f,
        0.637499988079071f, 0.12374371290206909f, -0.01767769828438759f, 1.0f, 1.0f,
        0.637499988079071f, 0.12374371290206909f, -0.01767769828438759f, 0.0f, 0.0f,
        0.637499988079071f, 0.14550720155239105f, -0.013889283873140812f, 0.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, -0.009567114524543285f, 1.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, -0.009567114524543285f, 0.0f, 0.0f,
        0.637499988079071f, 0.17163744568824768f, -0.004877286031842232f, 0.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, 0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, 0.17163744568824768f, -0.004877286031842232f, 0.0f, 0.0f,
        0.637499988079071f, 0.17500002682209015f, -2.7866009943977588e-08f, 0.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, 0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, 0.17500002682209015f, -2.7866009943977588e-08f, 0.0f, 0.0f,
        0.637499988079071f, 0.17163744568824768f, 0.004877230152487755f, 0.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, 0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, 0.009567058645188808f, 0.0f, 0.0f,
        0.637499988079071f, 0.14550720155239105f, 0.013889227993786335f, 0.0f, 1.0f,
        0.637499988079071f, 0.12374371290206909f, 0.01767764240503311f, 1.0f, 1.0f,
        0.637499988079071f, 0.12374371290206909f, 0.01767764240503311f, 0.0f, 0.0f,
        0.637499988079071f, 0.09722482413053513f, 0.0207867119461298f, 0.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 1.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, 0.0230969600379467f, 0.0f, 0.0f,
        0.637499988079071f, -0.12374365329742432f, 0.01767764240503311f, 0.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 1.0f, 1.0f,
        0.637499988079071f, -0.12374365329742432f, 0.01767764240503311f, 0.0f, 0.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 0.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 1.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, -0.009567114524543285f, 0.0f, 0.0f,
        0.637499988079071f, -0.12374365329742432f, -0.01767769828438759f, 0.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, -0.12374365329742432f, -0.01767769828438759f, 0.0f, 0.0f,
        0.637499988079071f, -0.06696957349777222f, -0.023097015917301178f, 0.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, -0.023097015917301178f, 0.0f, 0.0f,
        0.637499988079071f, 0.12374371290206909f, -0.01767769828438759f, 0.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, -0.023097015917301178f, 1.0f, 1.0f,
        0.637499988079071f, 0.12374371290206909f, -0.01767769828438759f, 0.0f, 0.0f,
        0.637499988079071f, 0.16167894005775452f, -0.009567114524543285f, 0.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, -0.023097015917301178f, 1.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, 0.009567058645188808f, 0.0f, 0.0f,
        0.637499988079071f, 0.12374371290206909f, 0.01767764240503311f, 0.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, -0.009567114524543285f, 1.0f, 1.0f,
        0.637499988079071f, 0.12374371290206909f, 0.01767764240503311f, 0.0f, 0.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 0.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, -0.009567114524543285f, 1.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 0.0f, 0.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 0.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, -0.023097015917301178f, 1.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 0.0f, 0.0f,
        -0.637499988079071f, -2.7866009943977588e-08f, 0.02500002831220627f, 0.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, 0.0245196595788002f, 1.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, 0.0245196595788002f, 0.0f, 0.0f,
        -0.637499988079071f, 0.06696957349777222f, 0.023097015917301178f, 0.0f, 1.0f,
        -0.637499988079071f, 0.09722476452589035f, 0.020786767825484276f, 1.0f, 1.0f,
        -0.637499988079071f, 0.09722476452589035f, 0.020786767825484276f, 0.0f, 0.0f,
        -0.637499988079071f, 0.12374365329742432f, 0.01767769828438759f, 0.0f, 1.0f,
        -0.637499988079071f, 0.14550714194774628f, 0.013889283873140812f, 1.0f, 1.0f,
        -0.637499988079071f, 0.14550714194774628f, 0.013889283873140812f, 0.0f, 0.0f,
        -0.637499988079071f, 0.16167888045310974f, 0.009567114524543285f, 0.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, 0.004877286031842232f, 1.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, 0.004877286031842232f, 0.0f, 0.0f,
        -0.637499988079071f, 0.17499996721744537f, 2.7866009943977588e-08f, 0.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, -0.004877230152487755f, 1.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, -0.004877230152487755f, 0.0f, 0.0f,
        -0.637499988079071f, 0.16167888045310974f, -0.009567058645188808f, 0.0f, 1.0f,
        -0.637499988079071f, 0.14550714194774628f, -0.013889227993786335f, 1.0f, 1.0f,
        -0.637499988079071f, 0.14550714194774628f, -0.013889227993786335f, 0.0f, 0.0f,
        -0.637499988079071f, 0.12374365329742432f, -0.01767764240503311f, 0.0f, 1.0f,
        -0.637499988079071f, 0.09722476452589035f, -0.0207867119461298f, 1.0f, 1.0f,
        -0.637499988079071f, 0.09722476452589035f, -0.0207867119461298f, 0.0f, 0.0f,
        -0.637499988079071f, 0.06696957349777222f, -0.0230969600379467f, 0.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 1.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 0.0f, 0.0f,
        -0.637499988079071f, -2.7866009943977588e-08f, -0.02499997243285179f, 0.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, -0.024519603699445724f, 1.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, -0.024519603699445724f, 0.0f, 0.0f,
        -0.637499988079071f, -0.06696963310241699f, -0.0230969600379467f, 0.0f, 1.0f,
        -0.637499988079071f, -0.09722482413053513f, -0.0207867119461298f, 1.0f, 1.0f,
        -0.637499988079071f, -0.09722482413053513f, -0.0207867119461298f, 0.0f, 0.0f,
        -0.637499988079071f, -0.12374371290206909f, -0.01767764240503311f, 0.0f, 1.0f,
        -0.637499988079071f, -0.14550720155239105f, -0.013889227993786335f, 1.0f, 1.0f,
        -0.637499988079071f, -0.14550720155239105f, -0.013889227993786335f, 0.0f, 0.0f,
        -0.637499988079071f, -0.16167894005775452f, -0.009567058645188808f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, -0.004877230152487755f, 1.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, -0.004877230152487755f, 0.0f, 0.0f,
        -0.637499988079071f, -0.17500002682209015f, 2.7866009943977588e-08f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, 0.004877286031842232f, 1.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, 0.004877286031842232f, 0.0f, 0.0f,
        -0.637499988079071f, -0.16167894005775452f, 0.009567114524543285f, 0.0f, 1.0f,
        -0.637499988079071f, -0.14550720155239105f, 0.013889283873140812f, 1.0f, 1.0f,
        -0.637499988079071f, -0.14550720155239105f, 0.013889283873140812f, 0.0f, 0.0f,
        -0.637499988079071f, -0.12374371290206909f, 0.01767769828438759f, 0.0f, 1.0f,
        -0.637499988079071f, -0.09722482413053513f, 0.020786767825484276f, 1.0f, 1.0f,
        -0.637499988079071f, -0.09722482413053513f, 0.020786767825484276f, 0.0f, 0.0f,
        -0.637499988079071f, -0.06696963310241699f, 0.023097015917301178f, 0.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 1.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 0.0f, 0.0f,
        -0.637499988079071f, 0.034140780568122864f, 0.0245196595788002f, 0.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, 0.004877286031842232f, 1.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, 0.0245196595788002f, 0.0f, 0.0f,
        -0.637499988079071f, 0.09722476452589035f, 0.020786767825484276f, 0.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, 0.004877286031842232f, 1.0f, 1.0f,
        -0.637499988079071f, 0.09722476452589035f, 0.020786767825484276f, 0.0f, 0.0f,
        -0.637499988079071f, 0.14550714194774628f, 0.013889283873140812f, 0.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, 0.004877286031842232f, 1.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, 0.004877286031842232f, 0.0f, 0.0f,
        -0.637499988079071f, 0.1716373860836029f, -0.004877230152487755f, 0.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 1.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, -0.004877230152487755f, 0.0f, 0.0f,
        -0.637499988079071f, 0.14550714194774628f, -0.013889227993786335f, 0.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 1.0f, 1.0f,
        -0.637499988079071f, 0.14550714194774628f, -0.013889227993786335f, 0.0f, 0.0f,
        -0.637499988079071f, 0.09722476452589035f, -0.0207867119461298f, 0.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 1.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 0.0f, 0.0f,
        -0.637499988079071f, -0.03414083272218704f, -0.024519603699445724f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, -0.004877230152487755f, 1.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, -0.024519603699445724f, 0.0f, 0.0f,
        -0.637499988079071f, -0.09722482413053513f, -0.0207867119461298f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, -0.004877230152487755f, 1.0f, 1.0f,
        -0.637499988079071f, -0.09722482413053513f, -0.0207867119461298f, 0.0f, 0.0f,
        -0.637499988079071f, -0.14550720155239105f, -0.013889227993786335f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, -0.004877230152487755f, 1.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, -0.004877230152487755f, 0.0f, 0.0f,
        -0.637499988079071f, -0.17163744568824768f, 0.004877286031842232f, 0.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 1.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, 0.004877286031842232f, 0.0f, 0.0f,
        -0.637499988079071f, -0.14550720155239105f, 0.013889283873140812f, 0.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 1.0f, 1.0f,
        -0.637499988079071f, -0.14550720155239105f, 0.013889283873140812f, 0.0f, 0.0f,
        -0.637499988079071f, -0.09722482413053513f, 0.020786767825484276f, 0.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 1.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 0.0f, 0.0f,
        -0.637499988079071f, 0.1716373860836029f, 0.004877286031842232f, 0.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 1.0f, 1.0f,
        0.637499988079071f, 2.7866009943977588e-08f, 0.02499997243285179f, 0.0f, 0.0f,
        0.637499988079071f, 0.03414083272218704f, 0.024519603699445724f, 0.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, 0.0245196595788002f, 1.0f, 1.0f,
        0.637499988079071f, 0.03414083272218704f, 0.024519603699445724f, 0.0f, 0.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 0.0f, 1.0f,
        -0.637499988079071f, 0.06696957349777222f, 0.023097015917301178f, 1.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, 0.0230969600379467f, 0.0f, 0.0f,
        0.637499988079071f, 0.09722482413053513f, 0.0207867119461298f, 0.0f, 1.0f,
        -0.637499988079071f, 0.09722476452589035f, 0.020786767825484276f, 1.0f, 1.0f,
        0.637499988079071f, 0.09722482413053513f, 0.0207867119461298f, 0.0f, 0.0f,
        0.637499988079071f, 0.12374371290206909f, 0.01767764240503311f, 0.0f, 1.0f,
        -0.637499988079071f, 0.12374365329742432f, 0.01767769828438759f, 1.0f, 1.0f,
        0.637499988079071f, 0.12374371290206909f, 0.01767764240503311f, 0.0f, 0.0f,
        0.637499988079071f, 0.14550720155239105f, 0.013889227993786335f, 0.0f, 1.0f,
        -0.637499988079071f, 0.14550714194774628f, 0.013889283873140812f, 1.0f, 1.0f,
        0.637499988079071f, 0.14550720155239105f, 0.013889227993786335f, 0.0f, 0.0f,
        0.637499988079071f, 0.16167894005775452f, 0.009567058645188808f, 0.0f, 1.0f,
        -0.637499988079071f, 0.16167888045310974f, 0.009567114524543285f, 1.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, 0.009567058645188808f, 0.0f, 0.0f,
        0.637499988079071f, 0.17163744568824768f, 0.004877230152487755f, 0.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, 0.004877286031842232f, 1.0f, 1.0f,
        0.637499988079071f, 0.17163744568824768f, 0.004877230152487755f, 0.0f, 0.0f,
        0.637499988079071f, 0.17500002682209015f, -2.7866009943977588e-08f, 0.0f, 1.0f,
        -0.637499988079071f, 0.17499996721744537f, 2.7866009943977588e-08f, 1.0f, 1.0f,
        0.637499988079071f, -0.17499996721744537f, -2.7866009943977588e-08f, 0.0f, 0.0f,
        0.637499988079071f, -0.1716373860836029f, 0.004877230152487755f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, 0.004877286031842232f, 1.0f, 1.0f,
        0.637499988079071f, -0.1716373860836029f, 0.004877230152487755f, 0.0f, 0.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 0.0f, 1.0f,
        -0.637499988079071f, -0.16167894005775452f, 0.009567114524543285f, 1.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, 0.009567058645188808f, 0.0f, 0.0f,
        0.637499988079071f, -0.14550714194774628f, 0.013889227993786335f, 0.0f, 1.0f,
        -0.637499988079071f, -0.14550720155239105f, 0.013889283873140812f, 1.0f, 1.0f,
        0.637499988079071f, -0.14550714194774628f, 0.013889227993786335f, 0.0f, 0.0f,
        0.637499988079071f, -0.12374365329742432f, 0.01767764240503311f, 0.0f, 1.0f,
        -0.637499988079071f, -0.12374371290206909f, 0.01767769828438759f, 1.0f, 1.0f,
        0.637499988079071f, -0.12374365329742432f, 0.01767764240503311f, 0.0f, 0.0f,
        0.637499988079071f, -0.09722476452589035f, 0.0207867119461298f, 0.0f, 1.0f,
        -0.637499988079071f, -0.09722482413053513f, 0.020786767825484276f, 1.0f, 1.0f,
        0.637499988079071f, -0.09722476452589035f, 0.0207867119461298f, 0.0f, 0.0f,
        0.637499988079071f, -0.06696957349777222f, 0.0230969600379467f, 0.0f, 1.0f,
        -0.637499988079071f, -0.06696963310241699f, 0.023097015917301178f, 1.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, 0.0230969600379467f, 0.0f, 0.0f,
        0.637499988079071f, -0.034140780568122864f, 0.024519603699445724f, 0.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, 0.0245196595788002f, 1.0f, 1.0f,
        0.637499988079071f, -0.034140780568122864f, 0.024519603699445724f, 0.0f, 0.0f,
        0.637499988079071f, 2.7866009943977588e-08f, 0.02499997243285179f, 0.0f, 1.0f,
        -0.637499988079071f, -2.7866009943977588e-08f, 0.02500002831220627f, 1.0f, 1.0f,
        0.637499988079071f, 0.17500002682209015f, -2.7866009943977588e-08f, 0.0f, 0.0f,
        0.637499988079071f, 0.17163744568824768f, -0.004877286031842232f, 0.0f, 1.0f,
        -0.637499988079071f, 0.1716373860836029f, -0.004877230152487755f, 1.0f, 1.0f,
        0.637499988079071f, 0.17163744568824768f, -0.004877286031842232f, 0.0f, 0.0f,
        0.637499988079071f, 0.16167894005775452f, -0.009567114524543285f, 0.0f, 1.0f,
        -0.637499988079071f, 0.16167888045310974f, -0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, 0.16167894005775452f, -0.009567114524543285f, 0.0f, 0.0f,
        0.637499988079071f, 0.14550720155239105f, -0.013889283873140812f, 0.0f, 1.0f,
        -0.637499988079071f, 0.14550714194774628f, -0.013889227993786335f, 1.0f, 1.0f,
        0.637499988079071f, 0.14550720155239105f, -0.013889283873140812f, 0.0f, 0.0f,
        0.637499988079071f, 0.12374371290206909f, -0.01767769828438759f, 0.0f, 1.0f,
        -0.637499988079071f, 0.12374365329742432f, -0.01767764240503311f, 1.0f, 1.0f,
        0.637499988079071f, 0.12374371290206909f, -0.01767769828438759f, 0.0f, 0.0f,
        0.637499988079071f, 0.09722482413053513f, -0.020786767825484276f, 0.0f, 1.0f,
        -0.637499988079071f, 0.09722476452589035f, -0.0207867119461298f, 1.0f, 1.0f,
        0.637499988079071f, 0.09722482413053513f, -0.020786767825484276f, 0.0f, 0.0f,
        0.637499988079071f, 0.06696963310241699f, -0.023097015917301178f, 0.0f, 1.0f,
        -0.637499988079071f, 0.06696957349777222f, -0.0230969600379467f, 1.0f, 1.0f,
        0.637499988079071f, 0.06696963310241699f, -0.023097015917301178f, 0.0f, 0.0f,
        0.637499988079071f, 0.03414083272218704f, -0.0245196595788002f, 0.0f, 1.0f,
        -0.637499988079071f, 0.034140780568122864f, -0.024519603699445724f, 1.0f, 1.0f,
        0.637499988079071f, 0.03414083272218704f, -0.0245196595788002f, 0.0f, 0.0f,
        0.637499988079071f, 2.7866009943977588e-08f, -0.02500002831220627f, 0.0f, 1.0f,
        -0.637499988079071f, -2.7866009943977588e-08f, -0.02499997243285179f, 1.0f, 1.0f,
        0.637499988079071f, 2.7866009943977588e-08f, -0.02500002831220627f, 0.0f, 0.0f,
        0.637499988079071f, -0.034140780568122864f, -0.0245196595788002f, 0.0f, 1.0f,
        -0.637499988079071f, -0.03414083272218704f, -0.024519603699445724f, 1.0f, 1.0f,
        0.637499988079071f, -0.034140780568122864f, -0.0245196595788002f, 0.0f, 0.0f,
        0.637499988079071f, -0.06696957349777222f, -0.023097015917301178f, 0.0f, 1.0f,
        -0.637499988079071f, -0.06696963310241699f, -0.0230969600379467f, 1.0f, 1.0f,
        0.637499988079071f, -0.06696957349777222f, -0.023097015917301178f, 0.0f, 0.0f,
        0.637499988079071f, -0.09722476452589035f, -0.020786767825484276f, 0.0f, 1.0f,
        -0.637499988079071f, -0.09722482413053513f, -0.0207867119461298f, 1.0f, 1.0f,
        0.637499988079071f, -0.09722476452589035f, -0.020786767825484276f, 0.0f, 0.0f,
        0.637499988079071f, -0.12374365329742432f, -0.01767769828438759f, 0.0f, 1.0f,
        -0.637499988079071f, -0.12374371290206909f, -0.01767764240503311f, 1.0f, 1.0f,
        0.637499988079071f, -0.12374365329742432f, -0.01767769828438759f, 0.0f, 0.0f,
        0.637499988079071f, -0.14550714194774628f, -0.013889283873140812f, 0.0f, 1.0f,
        -0.637499988079071f, -0.14550720155239105f, -0.013889227993786335f, 1.0f, 1.0f,
        0.637499988079071f, -0.14550714194774628f, -0.013889283873140812f, 0.0f, 0.0f,
        0.637499988079071f, -0.16167888045310974f, -0.009567114524543285f, 0.0f, 1.0f,
        -0.637499988079071f, -0.16167894005775452f, -0.009567058645188808f, 1.0f, 1.0f,
        0.637499988079071f, -0.16167888045310974f, -0.009567114524543285f, 0.0f, 0.0f,
        0.637499988079071f, -0.1716373860836029f, -0.004877286031842232f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17163744568824768f, -0.004877230152487755f, 1.0f, 1.0f,
        0.637499988079071f, -0.1716373860836029f, -0.004877286031842232f, 0.0f, 0.0f,
        0.637499988079071f, -0.17499996721744537f, -2.7866009943977588e-08f, 0.0f, 1.0f,
        -0.637499988079071f, -0.17500002682209015f, 2.7866009943977588e-08f, 1.0f, 1.0f,

        // Window Blind Rod
        0.0f, 5.463922647663821e-10f, -0.01250006165355444f, 0.0f, 0.0f,
        0.0f, 2.799999952316284f, 0.01250006165355444f, 0.0f, 1.0f,
        0.0f, 2.799999952316284f, -0.01249993871897459f, 1.0f, 1.0f,
        0.0f, -5.463922647663821e-10f, 0.01249993871897459f, 0.0f, 0.0f,
        0.02500000037252903f, 2.799999952316284f, 0.01250006165355444f, 0.0f, 1.0f,
        0.0f, 2.799999952316284f, 0.01250006165355444f, 1.0f, 1.0f,
        0.02500000037252903f, 5.463922647663821e-10f, -0.01250006165355444f, 0.0f, 0.0f,
        0.0f, 2.799999952316284f, -0.01249993871897459f, 0.0f, 1.0f,
        0.02500000037252903f, 2.799999952316284f, -0.01249993871897459f, 1.0f, 1.0f,
        0.02500000037252903f, 2.799999952316284f, 0.01250006165355444f, 0.0f, 0.0f,
        0.0f, 2.799999952316284f, -0.01249993871897459f, 0.0f, 1.0f,
        0.0f, 2.799999952316284f, 0.01250006165355444f, 1.0f, 1.0f,
        0.0f, -5.463922647663821e-10f, 0.01249993871897459f, 0.0f, 0.0f,
        0.02500000037252903f, 5.463922647663821e-10f, -0.01250006165355444f, 0.0f, 1.0f,
        0.02500000037252903f, -5.463922647663821e-10f, 0.01249993871897459f, 1.0f, 1.0f,
        0.02499999850988388f, -5.463922647663821e-10f, 0.012499937787652016f, 0.0f, 0.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 0.0f, 1.0f,
        0.02499999850988388f, 2.799999952316284f, 0.012500060722231865f, 1.0f, 1.0f,
        0.027438629418611526f, -5.358935517563168e-10f, 0.012259753420948982f, 0.0f, 0.0f,
        0.029783543199300766f, 2.799999952316284f, 0.01154855452477932f, 0.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 1.0f, 1.0f,
        0.029783543199300766f, -5.048006457286647e-10f, 0.01154843159019947f, 0.0f, 0.0f,
        0.031944628804922104f, 2.799999952316284f, 0.010393430478870869f, 0.0f, 1.0f,
        0.029783543199300766f, 2.799999952316284f, 0.01154855452477932f, 1.0f, 1.0f,
        0.031944628804922104f, -4.5430859074713226e-10f, 0.01039330754429102f, 0.0f, 0.0f,
        0.03383883461356163f, 2.799999952316284f, 0.008838895708322525f, 0.0f, 1.0f,
        0.031944628804922104f, 2.799999952316284f, 0.010393430478870869f, 1.0f, 1.0f,
        0.03383883461356163f, -3.863576125695545e-10f, 0.008838772773742676f, 0.0f, 0.0f,
        0.0353933684527874f, 2.799999952316284f, 0.006944688502699137f, 0.0f, 1.0f,
        0.03383883461356163f, 2.799999952316284f, 0.008838895708322525f, 1.0f, 1.0f,
        0.0353933684527874f, -3.0355939983905955e-10f, 0.006944566499441862f, 0.0f, 0.0f,
        0.036548491567373276f, 2.799999952316284f, 0.0047836038284003735f, 0.0f, 1.0f,
        0.0353933684527874f, 2.799999952316284f, 0.006944688502699137f, 1.0f, 1.0f,
        0.036548491567373276f, -2.0909540765501333e-10f, 0.004783481825143099f, 0.0f, 0.0f,
        0.03725981339812279f, 2.799999952316284f, 0.002438690047711134f, 0.0f, 1.0f,
        0.036548491567373276f, 2.799999952316284f, 0.0047836038284003735f, 1.0f, 1.0f,
        0.03725981339812279f, -1.0659584326333515e-10f, 0.002438567578792572f, 0.0f, 0.0f,
        0.03749999776482582f, 2.799999952316284f, 6.119594075926216e-08f, 0.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, 0.002438690047711134f, 1.0f, 1.0f,
        0.03749999776482582f, 0.0f, -6.119594075926216e-08f, 0.0f, 0.0f,
        0.03725981339812279f, 2.799999952316284f, -0.002438567578792572f, 0.0f, 1.0f,
        0.03749999776482582f, 2.799999952316284f, 6.119594075926216e-08f, 1.0f, 1.0f,
        0.03725981339812279f, 1.0659584326333515e-10f, -0.002438690047711134f, 0.0f, 0.0f,
        0.036548491567373276f, 2.799999952316284f, -0.004783481825143099f, 0.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, -0.002438567578792572f, 1.0f, 1.0f,
        0.036548491567373276f, 2.0909540765501333e-10f, -0.0047836038284003735f, 0.0f, 0.0f,
        0.0353933684527874f, 2.799999952316284f, -0.006944566499441862f, 0.0f, 1.0f,
        0.036548491567373276f, 2.799999952316284f, -0.004783481825143099f, 1.0f, 1.0f,
        0.0353933684527874f, 3.0355939983905955e-10f, -0.006944688502699137f, 0.0f, 0.0f,
        0.03383883461356163f, 2.799999952316284f, -0.008838772773742676f, 0.0f, 1.0f,
        0.0353933684527874f, 2.799999952316284f, -0.006944566499441862f, 1.0f, 1.0f,
        0.03383883461356163f, 3.863576125695545e-10f, -0.008838895708322525f, 0.0f, 0.0f,
        0.031944628804922104f, 2.799999952316284f, -0.01039330754429102f, 0.0f, 1.0f,
        0.03383883461356163f, 2.799999952316284f, -0.008838772773742676f, 1.0f, 1.0f,
        0.031944628804922104f, 4.5430859074713226e-10f, -0.010393430478870869f, 0.0f, 0.0f,
        0.029783543199300766f, 2.799999952316284f, -0.01154843159019947f, 0.0f, 1.0f,
        0.031944628804922104f, 2.799999952316284f, -0.01039330754429102f, 1.0f, 1.0f,
        0.029783543199300766f, 5.048006457286647e-10f, -0.01154855452477932f, 0.0f, 0.0f,
        0.027438629418611526f, 2.799999952316284f, -0.012259753420948982f, 0.0f, 1.0f,
        0.029783543199300766f, 2.799999952316284f, -0.01154843159019947f, 1.0f, 1.0f,
        0.027438629418611526f, 5.358935517563168e-10f, -0.012259876355528831f, 0.0f, 0.0f,
        0.02499999850988388f, 2.799999952316284f, -0.012499937787652016f, 0.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, -0.012259753420948982f, 1.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 0.0f, 0.0f,
        0.03725981339812279f, 2.799999952316284f, -0.002438567578792572f, 0.0f, 1.0f,
        0.031944628804922104f, 2.799999952316284f, -0.01039330754429102f, 1.0f, 1.0f,
        0.031944628804922104f, 4.5430859074713226e-10f, -0.010393430478870869f, 0.0f, 0.0f,
        0.03725981339812279f, 1.0659584326333515e-10f, -0.002438690047711134f, 0.0f, 1.0f,
        0.0353933684527874f, -3.0355939983905955e-10f, 0.006944566499441862f, 1.0f, 1.0f,
        0.0f, 5.463922647663821e-10f, -0.01250006165355444f, 0.0f, 0.0f,
        0.0f, -5.463922647663821e-10f, 0.01249993871897459f, 0.0f, 1.0f,
        0.0f, 2.799999952316284f, 0.01250006165355444f, 1.0f, 1.0f,
        0.0f, -5.463922647663821e-10f, 0.01249993871897459f, 0.0f, 0.0f,
        0.02500000037252903f, -5.463922647663821e-10f, 0.01249993871897459f, 0.0f, 1.0f,
        0.02500000037252903f, 2.799999952316284f, 0.01250006165355444f, 1.0f, 1.0f,
        0.02500000037252903f, 5.463922647663821e-10f, -0.01250006165355444f, 0.0f, 0.0f,
        0.0f, 5.463922647663821e-10f, -0.01250006165355444f, 0.0f, 1.0f,
        0.0f, 2.799999952316284f, -0.01249993871897459f, 1.0f, 1.0f,
        0.02500000037252903f, 2.799999952316284f, 0.01250006165355444f, 0.0f, 0.0f,
        0.02500000037252903f, 2.799999952316284f, -0.01249993871897459f, 0.0f, 1.0f,
        0.0f, 2.799999952316284f, -0.01249993871897459f, 1.0f, 1.0f,
        0.0f, -5.463922647663821e-10f, 0.01249993871897459f, 0.0f, 0.0f,
        0.0f, 5.463922647663821e-10f, -0.01250006165355444f, 0.0f, 1.0f,
        0.02500000037252903f, 5.463922647663821e-10f, -0.01250006165355444f, 1.0f, 1.0f,
        0.02499999850988388f, -5.463922647663821e-10f, 0.012499937787652016f, 0.0f, 0.0f,
        0.027438629418611526f, -5.358935517563168e-10f, 0.012259753420948982f, 0.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 1.0f, 1.0f,
        0.027438629418611526f, -5.358935517563168e-10f, 0.012259753420948982f, 0.0f, 0.0f,
        0.029783543199300766f, -5.048006457286647e-10f, 0.01154843159019947f, 0.0f, 1.0f,
        0.029783543199300766f, 2.799999952316284f, 0.01154855452477932f, 1.0f, 1.0f,
        0.029783543199300766f, -5.048006457286647e-10f, 0.01154843159019947f, 0.0f, 0.0f,
        0.031944628804922104f, -4.5430859074713226e-10f, 0.01039330754429102f, 0.0f, 1.0f,
        0.031944628804922104f, 2.799999952316284f, 0.010393430478870869f, 1.0f, 1.0f,
        0.031944628804922104f, -4.5430859074713226e-10f, 0.01039330754429102f, 0.0f, 0.0f,
        0.03383883461356163f, -3.863576125695545e-10f, 0.008838772773742676f, 0.0f, 1.0f,
        0.03383883461356163f, 2.799999952316284f, 0.008838895708322525f, 1.0f, 1.0f,
        0.03383883461356163f, -3.863576125695545e-10f, 0.008838772773742676f, 0.0f, 0.0f,
        0.0353933684527874f, -3.0355939983905955e-10f, 0.006944566499441862f, 0.0f, 1.0f,
        0.0353933684527874f, 2.799999952316284f, 0.006944688502699137f, 1.0f, 1.0f,
        0.0353933684527874f, -3.0355939983905955e-10f, 0.006944566499441862f, 0.0f, 0.0f,
        0.036548491567373276f, -2.0909540765501333e-10f, 0.004783481825143099f, 0.0f, 1.0f,
        0.036548491567373276f, 2.799999952316284f, 0.0047836038284003735f, 1.0f, 1.0f,
        0.036548491567373276f, -2.0909540765501333e-10f, 0.004783481825143099f, 0.0f, 0.0f,
        0.03725981339812279f, -1.0659584326333515e-10f, 0.002438567578792572f, 0.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, 0.002438690047711134f, 1.0f, 1.0f,
        0.03725981339812279f, -1.0659584326333515e-10f, 0.002438567578792572f, 0.0f, 0.0f,
        0.03749999776482582f, 0.0f, -6.119594075926216e-08f, 0.0f, 1.0f,
        0.03749999776482582f, 2.799999952316284f, 6.119594075926216e-08f, 1.0f, 1.0f,
        0.03749999776482582f, 0.0f, -6.119594075926216e-08f, 0.0f, 0.0f,
        0.03725981339812279f, 1.0659584326333515e-10f, -0.002438690047711134f, 0.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, -0.002438567578792572f, 1.0f, 1.0f,
        0.03725981339812279f, 1.0659584326333515e-10f, -0.002438690047711134f, 0.0f, 0.0f,
        0.036548491567373276f, 2.0909540765501333e-10f, -0.0047836038284003735f, 0.0f, 1.0f,
        0.036548491567373276f, 2.799999952316284f, -0.004783481825143099f, 1.0f, 1.0f,
        0.036548491567373276f, 2.0909540765501333e-10f, -0.0047836038284003735f, 0.0f, 0.0f,
        0.0353933684527874f, 3.0355939983905955e-10f, -0.006944688502699137f, 0.0f, 1.0f,
        0.0353933684527874f, 2.799999952316284f, -0.006944566499441862f, 1.0f, 1.0f,
        0.0353933684527874f, 3.0355939983905955e-10f, -0.006944688502699137f, 0.0f, 0.0f,
        0.03383883461356163f, 3.863576125695545e-10f, -0.008838895708322525f, 0.0f, 1.0f,
        0.03383883461356163f, 2.799999952316284f, -0.008838772773742676f, 1.0f, 1.0f,
        0.03383883461356163f, 3.863576125695545e-10f, -0.008838895708322525f, 0.0f, 0.0f,
        0.031944628804922104f, 4.5430859074713226e-10f, -0.010393430478870869f, 0.0f, 1.0f,
        0.031944628804922104f, 2.799999952316284f, -0.01039330754429102f, 1.0f, 1.0f,
        0.031944628804922104f, 4.5430859074713226e-10f, -0.010393430478870869f, 0.0f, 0.0f,
        0.029783543199300766f, 5.048006457286647e-10f, -0.01154855452477932f, 0.0f, 1.0f,
        0.029783543199300766f, 2.799999952316284f, -0.01154843159019947f, 1.0f, 1.0f,
        0.029783543199300766f, 5.048006457286647e-10f, -0.01154855452477932f, 0.0f, 0.0f,
        0.027438629418611526f, 5.358935517563168e-10f, -0.012259876355528831f, 0.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, -0.012259753420948982f, 1.0f, 1.0f,
        0.027438629418611526f, 5.358935517563168e-10f, -0.012259876355528831f, 0.0f, 0.0f,
        0.02499999850988388f, 5.463922647663821e-10f, -0.012500060722231865f, 0.0f, 1.0f,
        0.02499999850988388f, 2.799999952316284f, -0.012499937787652016f, 1.0f, 1.0f,
        0.02499999850988388f, 2.799999952316284f, -0.012499937787652016f, 0.0f, 0.0f,
        0.02499999850988388f, 2.799999952316284f, 0.012500060722231865f, 0.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 1.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 0.0f, 0.0f,
        0.029783543199300766f, 2.799999952316284f, 0.01154855452477932f, 0.0f, 1.0f,
        0.031944628804922104f, 2.799999952316284f, 0.010393430478870869f, 1.0f, 1.0f,
        0.031944628804922104f, 2.799999952316284f, 0.010393430478870869f, 0.0f, 0.0f,
        0.03383883461356163f, 2.799999952316284f, 0.008838895708322525f, 0.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 1.0f, 1.0f,
        0.03383883461356163f, 2.799999952316284f, 0.008838895708322525f, 0.0f, 0.0f,
        0.0353933684527874f, 2.799999952316284f, 0.006944688502699137f, 0.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 1.0f, 1.0f,
        0.0353933684527874f, 2.799999952316284f, 0.006944688502699137f, 0.0f, 0.0f,
        0.036548491567373276f, 2.799999952316284f, 0.0047836038284003735f, 0.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, 0.002438690047711134f, 1.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, 0.002438690047711134f, 0.0f, 0.0f,
        0.03749999776482582f, 2.799999952316284f, 6.119594075926216e-08f, 0.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, -0.002438567578792572f, 1.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, -0.002438567578792572f, 0.0f, 0.0f,
        0.036548491567373276f, 2.799999952316284f, -0.004783481825143099f, 0.0f, 1.0f,
        0.0353933684527874f, 2.799999952316284f, -0.006944566499441862f, 1.0f, 1.0f,
        0.0353933684527874f, 2.799999952316284f, -0.006944566499441862f, 0.0f, 0.0f,
        0.03383883461356163f, 2.799999952316284f, -0.008838772773742676f, 0.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, -0.002438567578792572f, 1.0f, 1.0f,
        0.03383883461356163f, 2.799999952316284f, -0.008838772773742676f, 0.0f, 0.0f,
        0.031944628804922104f, 2.799999952316284f, -0.01039330754429102f, 0.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, -0.002438567578792572f, 1.0f, 1.0f,
        0.031944628804922104f, 2.799999952316284f, -0.01039330754429102f, 0.0f, 0.0f,
        0.029783543199300766f, 2.799999952316284f, -0.01154843159019947f, 0.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, -0.012259753420948982f, 1.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, -0.012259753420948982f, 0.0f, 0.0f,
        0.02499999850988388f, 2.799999952316284f, -0.012499937787652016f, 0.0f, 1.0f,
        0.031944628804922104f, 2.799999952316284f, -0.01039330754429102f, 1.0f, 1.0f,
        0.02499999850988388f, 2.799999952316284f, -0.012499937787652016f, 0.0f, 0.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 0.0f, 1.0f,
        0.031944628804922104f, 2.799999952316284f, -0.01039330754429102f, 1.0f, 1.0f,
        0.0353933684527874f, 2.799999952316284f, 0.006944688502699137f, 0.0f, 0.0f,
        0.03725981339812279f, 2.799999952316284f, 0.002438690047711134f, 0.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 1.0f, 1.0f,
        0.03725981339812279f, 2.799999952316284f, 0.002438690047711134f, 0.0f, 0.0f,
        0.03725981339812279f, 2.799999952316284f, -0.002438567578792572f, 0.0f, 1.0f,
        0.027438629418611526f, 2.799999952316284f, 0.012259876355528831f, 1.0f, 1.0f,
        0.029783543199300766f, -5.048006457286647e-10f, 0.01154843159019947f, 0.0f, 0.0f,
        0.027438629418611526f, -5.358935517563168e-10f, 0.012259753420948982f, 0.0f, 1.0f,
        0.031944628804922104f, -4.5430859074713226e-10f, 0.01039330754429102f, 1.0f, 1.0f,
        0.027438629418611526f, -5.358935517563168e-10f, 0.012259753420948982f, 0.0f, 0.0f,
        0.02499999850988388f, -5.463922647663821e-10f, 0.012499937787652016f, 0.0f, 1.0f,
        0.031944628804922104f, -4.5430859074713226e-10f, 0.01039330754429102f, 1.0f, 1.0f,
        0.02499999850988388f, -5.463922647663821e-10f, 0.012499937787652016f, 0.0f, 0.0f,
        0.02499999850988388f, 5.463922647663821e-10f, -0.012500060722231865f, 0.0f, 1.0f,
        0.031944628804922104f, 4.5430859074713226e-10f, -0.010393430478870869f, 1.0f, 1.0f,
        0.02499999850988388f, 5.463922647663821e-10f, -0.012500060722231865f, 0.0f, 0.0f,
        0.027438629418611526f, 5.358935517563168e-10f, -0.012259876355528831f, 0.0f, 1.0f,
        0.031944628804922104f, 4.5430859074713226e-10f, -0.010393430478870869f, 1.0f, 1.0f,
        0.027438629418611526f, 5.358935517563168e-10f, -0.012259876355528831f, 0.0f, 0.0f,
        0.029783543199300766f, 5.048006457286647e-10f, -0.01154855452477932f, 0.0f, 1.0f,
        0.031944628804922104f, 4.5430859074713226e-10f, -0.010393430478870869f, 1.0f, 1.0f,
        0.031944628804922104f, 4.5430859074713226e-10f, -0.010393430478870869f, 0.0f, 0.0f,
        0.03383883461356163f, 3.863576125695545e-10f, -0.008838895708322525f, 0.0f, 1.0f,
        0.03725981339812279f, 1.0659584326333515e-10f, -0.002438690047711134f, 1.0f, 1.0f,
        0.03383883461356163f, 3.863576125695545e-10f, -0.008838895708322525f, 0.0f, 0.0f,
        0.0353933684527874f, 3.0355939983905955e-10f, -0.006944688502699137f, 0.0f, 1.0f,
        0.03725981339812279f, 1.0659584326333515e-10f, -0.002438690047711134f, 1.0f, 1.0f,
        0.0353933684527874f, 3.0355939983905955e-10f, -0.006944688502699137f, 0.0f, 0.0f,
        0.036548491567373276f, 2.0909540765501333e-10f, -0.0047836038284003735f, 0.0f, 1.0f,
        0.03725981339812279f, 1.0659584326333515e-10f, -0.002438690047711134f, 1.0f, 1.0f,
        0.03725981339812279f, 1.0659584326333515e-10f, -0.002438690047711134f, 0.0f, 0.0f,
        0.03749999776482582f, 0.0f, -6.119594075926216e-08f, 0.0f, 1.0f,
        0.03725981339812279f, -1.0659584326333515e-10f, 0.002438567578792572f, 1.0f, 1.0f,
        0.03725981339812279f, -1.0659584326333515e-10f, 0.002438567578792572f, 0.0f, 0.0f,
        0.036548491567373276f, -2.0909540765501333e-10f, 0.004783481825143099f, 0.0f, 1.0f,
        0.03725981339812279f, 1.0659584326333515e-10f, -0.002438690047711134f, 1.0f, 1.0f,
        0.036548491567373276f, -2.0909540765501333e-10f, 0.004783481825143099f, 0.0f, 0.0f,
        0.0353933684527874f, -3.0355939983905955e-10f, 0.006944566499441862f, 0.0f, 1.0f,
        0.03725981339812279f, 1.0659584326333515e-10f, -0.002438690047711134f, 1.0f, 1.0f,
        0.0353933684527874f, -3.0355939983905955e-10f, 0.006944566499441862f, 0.0f, 0.0f,
        0.03383883461356163f, -3.863576125695545e-10f, 0.008838772773742676f, 0.0f, 1.0f,
        0.02499999850988388f, -5.463922647663821e-10f, 0.012499937787652016f, 1.0f, 1.0f,
        0.03383883461356163f, -3.863576125695545e-10f, 0.008838772773742676f, 0.0f, 0.0f,
        0.031944628804922104f, -4.5430859074713226e-10f, 0.01039330754429102f, 0.0f, 1.0f,
        0.02499999850988388f, -5.463922647663821e-10f, 0.012499937787652016f, 1.0f, 1.0f,
        0.02499999850988388f, -5.463922647663821e-10f, 0.012499937787652016f, 0.0f, 0.0f,
        0.031944628804922104f, 4.5430859074713226e-10f, -0.010393430478870869f, 0.0f, 1.0f,
        0.0353933684527874f, -3.0355939983905955e-10f, 0.006944566499441862f, 1.0f, 1.0f,

        1.787500023841858f, 0.02500000037252903f, -7.813410718426894e-08f, 0.0f, 0.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 0.0f, 1.0f,
        -1.787500023841858f, 0.02500000037252903f, 7.813410718426894e-08f, 1.0f, 1.0f,
        1.787500023841858f, 0.024519631639122963f, -0.012680948711931705f, 0.0f, 0.0f,
        -1.787500023841858f, 0.02309698797762394f, -0.02487434633076191f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 1.0f, 1.0f,
        1.787500023841858f, 0.02309698797762394f, -0.024874502792954445f, 0.0f, 0.0f,
        -1.787500023841858f, 0.020786739885807037f, -0.0361119881272316f, 0.0f, 1.0f,
        -1.787500023841858f, 0.02309698797762394f, -0.02487434633076191f, 1.0f, 1.0f,
        1.787500023841858f, 0.020786739885807037f, -0.03611214458942413f, 0.0f, 0.0f,
        -1.787500023841858f, 0.01767767034471035f, -0.045961860567331314f, 0.0f, 1.0f,
        -1.787500023841858f, 0.020786739885807037f, -0.0361119881272316f, 1.0f, 1.0f,
        1.787500023841858f, 0.01767767034471035f, -0.04596201702952385f, 0.0f, 0.0f,
        -1.787500023841858f, 0.013889255933463573f, -0.05404544249176979f, 0.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, -0.045961860567331314f, 1.0f, 1.0f,
        1.787500023841858f, 0.013889255933463573f, -0.054045598953962326f, 0.0f, 0.0f,
        -1.787500023841858f, 0.009567086584866047f, -0.06005208566784859f, 0.0f, 1.0f,
        -1.787500023841858f, 0.013889255933463573f, -0.05404544249176979f, 1.0f, 1.0f,
        1.787500023841858f, 0.009567086584866047f, -0.06005224213004112f, 0.0f, 0.0f,
        -1.787500023841858f, 0.004877258092164993f, -0.0637509673833847f, 0.0f, 1.0f,
        -1.787500023841858f, 0.009567086584866047f, -0.06005208566784859f, 1.0f, 1.0f,
        1.787500023841858f, 0.004877258092164993f, -0.06375111639499664f, 0.0f, 0.0f,
        -1.787500023841858f, 0.0f, -0.06499992311000824f, 0.0f, 1.0f,
        -1.787500023841858f, 0.004877258092164993f, -0.0637509673833847f, 1.0f, 1.0f,
        1.787500023841858f, 0.02500000037252903f, -7.813410718426894e-08f, 0.0f, 0.0f,
        1.787500023841858f, 0.024519631639122963f, -0.012680948711931705f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 1.0f, 1.0f,
        1.787500023841858f, 0.024519631639122963f, -0.012680948711931705f, 0.0f, 0.0f,
        1.787500023841858f, 0.02309698797762394f, -0.024874502792954445f, 0.0f, 1.0f,
        -1.787500023841858f, 0.02309698797762394f, -0.02487434633076191f, 1.0f, 1.0f,
        1.787500023841858f, 0.02309698797762394f, -0.024874502792954445f, 0.0f, 0.0f,
        1.787500023841858f, 0.020786739885807037f, -0.03611214458942413f, 0.0f, 1.0f,
        -1.787500023841858f, 0.020786739885807037f, -0.0361119881272316f, 1.0f, 1.0f,
        1.787500023841858f, 0.020786739885807037f, -0.03611214458942413f, 0.0f, 0.0f,
        1.787500023841858f, 0.01767767034471035f, -0.04596201702952385f, 0.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, -0.045961860567331314f, 1.0f, 1.0f,
        1.787500023841858f, 0.01767767034471035f, -0.04596201702952385f, 0.0f, 0.0f,
        1.787500023841858f, 0.013889255933463573f, -0.054045598953962326f, 0.0f, 1.0f,
        -1.787500023841858f, 0.013889255933463573f, -0.05404544249176979f, 1.0f, 1.0f,
        1.787500023841858f, 0.013889255933463573f, -0.054045598953962326f, 0.0f, 0.0f,
        1.787500023841858f, 0.009567086584866047f, -0.06005224213004112f, 0.0f, 1.0f,
        -1.787500023841858f, 0.009567086584866047f, -0.06005208566784859f, 1.0f, 1.0f,
        1.787500023841858f, 0.009567086584866047f, -0.06005224213004112f, 0.0f, 0.0f,
        1.787500023841858f, 0.004877258092164993f, -0.06375111639499664f, 0.0f, 1.0f,
        -1.787500023841858f, 0.004877258092164993f, -0.0637509673833847f, 1.0f, 1.0f,
        1.787500023841858f, 0.004877258092164993f, -0.06375111639499664f, 0.0f, 0.0f,
        1.787500023841858f, 0.0f, -0.06500007212162018f, 0.0f, 1.0f,
        -1.787500023841858f, 0.0f, -0.06499992311000824f, 1.0f, 1.0f,
        1.787500023841858f, 0.0f, 0.06499992311000824f, 0.0f, 0.0f,
        1.787500023841858f, 0.004877258092164993f, 0.0637509673833847f, 0.0f, 1.0f,
        -1.787500023841858f, 0.004877258092164993f, 0.06375111639499664f, 1.0f, 1.0f,
        1.787500023841858f, 0.004877258092164993f, 0.0637509673833847f, 0.0f, 0.0f,
        1.787500023841858f, 0.009567086584866047f, 0.06005208566784859f, 0.0f, 1.0f,
        -1.787500023841858f, 0.009567086584866047f, 0.06005224213004112f, 1.0f, 1.0f,
        1.787500023841858f, 0.009567086584866047f, 0.06005208566784859f, 0.0f, 0.0f,
        1.787500023841858f, 0.013889255933463573f, 0.05404544249176979f, 0.0f, 1.0f,
        -1.787500023841858f, 0.013889255933463573f, 0.054045598953962326f, 1.0f, 1.0f,
        1.787500023841858f, 0.013889255933463573f, 0.05404544249176979f, 0.0f, 0.0f,
        1.787500023841858f, 0.01767767034471035f, 0.045961860567331314f, 0.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, 0.04596201702952385f, 1.0f, 1.0f,
        1.787500023841858f, 0.01767767034471035f, 0.045961860567331314f, 0.0f, 0.0f,
        1.787500023841858f, 0.020786739885807037f, 0.0361119881272316f, 0.0f, 1.0f,
        -1.787500023841858f, 0.020786739885807037f, 0.03611214458942413f, 1.0f, 1.0f,
        1.787500023841858f, 0.020786739885807037f, 0.0361119881272316f, 0.0f, 0.0f,
        1.787500023841858f, 0.02309698797762394f, 0.02487434633076191f, 0.0f, 1.0f,
        -1.787500023841858f, 0.02309698797762394f, 0.024874502792954445f, 1.0f, 1.0f,
        1.787500023841858f, 0.02309698797762394f, -0.024874502792954445f, 0.0f, 0.0f,
        1.787500023841858f, 0.024519631639122963f, -0.012680948711931705f, 0.0f, 1.0f,
        1.787500023841858f, 0.02500000037252903f, -7.813410718426894e-08f, 1.0f, 1.0f,
        1.787500023841858f, 0.024519631639122963f, 0.01268079224973917f, 0.0f, 0.0f,
        1.787500023841858f, 0.02500000037252903f, -7.813410718426894e-08f, 0.0f, 1.0f,
        -1.787500023841858f, 0.02500000037252903f, 7.813410718426894e-08f, 1.0f, 1.0f,
        1.787500023841858f, 0.0f, 0.06499992311000824f, 0.0f, 0.0f,
        -1.787500023841858f, 0.004877258092164993f, 0.06375111639499664f, 0.0f, 1.0f,
        -1.787500023841858f, 0.0f, 0.06500007212162018f, 1.0f, 1.0f,
        1.787500023841858f, 0.004877258092164993f, 0.0637509673833847f, 0.0f, 0.0f,
        -1.787500023841858f, 0.009567086584866047f, 0.06005224213004112f, 0.0f, 1.0f,
        -1.787500023841858f, 0.004877258092164993f, 0.06375111639499664f, 1.0f, 1.0f,
        1.787500023841858f, 0.009567086584866047f, 0.06005208566784859f, 0.0f, 0.0f,
        -1.787500023841858f, 0.013889255933463573f, 0.054045598953962326f, 0.0f, 1.0f,
        -1.787500023841858f, 0.009567086584866047f, 0.06005224213004112f, 1.0f, 1.0f,
        1.787500023841858f, 0.013889255933463573f, 0.05404544249176979f, 0.0f, 0.0f,
        -1.787500023841858f, 0.01767767034471035f, 0.04596201702952385f, 0.0f, 1.0f,
        -1.787500023841858f, 0.013889255933463573f, 0.054045598953962326f, 1.0f, 1.0f,
        1.787500023841858f, 0.01767767034471035f, 0.045961860567331314f, 0.0f, 0.0f,
        -1.787500023841858f, 0.020786739885807037f, 0.03611214458942413f, 0.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, 0.04596201702952385f, 1.0f, 1.0f,
        1.787500023841858f, 0.020786739885807037f, 0.0361119881272316f, 0.0f, 0.0f,
        -1.787500023841858f, 0.02309698797762394f, 0.024874502792954445f, 0.0f, 1.0f,
        -1.787500023841858f, 0.020786739885807037f, 0.03611214458942413f, 1.0f, 1.0f,
        1.787500023841858f, 0.02500000037252903f, -7.813410718426894e-08f, 0.0f, 0.0f,
        1.787500023841858f, 0.024519631639122963f, 0.01268079224973917f, 0.0f, 1.0f,
        1.787500023841858f, 0.01767767034471035f, 0.045961860567331314f, 1.0f, 1.0f,
        1.787500023841858f, 0.02309698797762394f, 0.02487434633076191f, 0.0f, 0.0f,
        -1.787500023841858f, 0.024519631639122963f, 0.012680948711931705f, 0.0f, 1.0f,
        -1.787500023841858f, 0.02309698797762394f, 0.024874502792954445f, 1.0f, 1.0f,
        1.787500023841858f, 0.024519631639122963f, 0.01268079224973917f, 0.0f, 0.0f,
        -1.787500023841858f, 0.02500000037252903f, 7.813410718426894e-08f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, 0.012680948711931705f, 1.0f, 1.0f,
        1.787500023841858f, 0.02309698797762394f, 0.02487434633076191f, 0.0f, 0.0f,
        1.787500023841858f, 0.024519631639122963f, 0.01268079224973917f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, 0.012680948711931705f, 1.0f, 1.0f,
        1.787500023841858f, 0.02500000037252903f, -7.813410718426894e-08f, 0.0f, 0.0f,
        1.787500023841858f, 0.0f, 0.06499992311000824f, 0.0f, 1.0f,
        1.787500023841858f, 0.013889255933463573f, -0.054045598953962326f, 1.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 0.0f, 0.0f,
        -1.787500023841858f, 0.0f, 0.06500007212162018f, 0.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, 0.04596201702952385f, 1.0f, 1.0f,
        1.787500023841858f, 0.024519631639122963f, 0.01268079224973917f, 0.0f, 0.0f,
        1.787500023841858f, 0.02309698797762394f, 0.02487434633076191f, 0.0f, 1.0f,
        1.787500023841858f, 0.01767767034471035f, 0.045961860567331314f, 1.0f, 1.0f,
        1.787500023841858f, 0.02309698797762394f, 0.02487434633076191f, 0.0f, 0.0f,
        1.787500023841858f, 0.020786739885807037f, 0.0361119881272316f, 0.0f, 1.0f,
        1.787500023841858f, 0.01767767034471035f, 0.045961860567331314f, 1.0f, 1.0f,
        1.787500023841858f, 0.01767767034471035f, 0.045961860567331314f, 0.0f, 0.0f,
        1.787500023841858f, 0.013889255933463573f, 0.05404544249176979f, 0.0f, 1.0f,
        1.787500023841858f, 0.009567086584866047f, 0.06005208566784859f, 1.0f, 1.0f,
        1.787500023841858f, 0.009567086584866047f, 0.06005208566784859f, 0.0f, 0.0f,
        1.787500023841858f, 0.004877258092164993f, 0.0637509673833847f, 0.0f, 1.0f,
        1.787500023841858f, 0.01767767034471035f, 0.045961860567331314f, 1.0f, 1.0f,
        1.787500023841858f, 0.004877258092164993f, 0.0637509673833847f, 0.0f, 0.0f,
        1.787500023841858f, 0.0f, 0.06499992311000824f, 0.0f, 1.0f,
        1.787500023841858f, 0.01767767034471035f, 0.045961860567331314f, 1.0f, 1.0f,
        1.787500023841858f, 0.0f, 0.06499992311000824f, 0.0f, 0.0f,
        1.787500023841858f, 0.0f, -0.06500007212162018f, 0.0f, 1.0f,
        1.787500023841858f, 0.004877258092164993f, -0.06375111639499664f, 1.0f, 1.0f,
        1.787500023841858f, 0.004877258092164993f, -0.06375111639499664f, 0.0f, 0.0f,
        1.787500023841858f, 0.009567086584866047f, -0.06005224213004112f, 0.0f, 1.0f,
        1.787500023841858f, 0.013889255933463573f, -0.054045598953962326f, 1.0f, 1.0f,
        1.787500023841858f, 0.013889255933463573f, -0.054045598953962326f, 0.0f, 0.0f,
        1.787500023841858f, 0.01767767034471035f, -0.04596201702952385f, 0.0f, 1.0f,
        1.787500023841858f, 0.020786739885807037f, -0.03611214458942413f, 1.0f, 1.0f,
        1.787500023841858f, 0.020786739885807037f, -0.03611214458942413f, 0.0f, 0.0f,
        1.787500023841858f, 0.02309698797762394f, -0.024874502792954445f, 0.0f, 1.0f,
        1.787500023841858f, 0.013889255933463573f, -0.054045598953962326f, 1.0f, 1.0f,
        1.787500023841858f, 0.02309698797762394f, -0.024874502792954445f, 0.0f, 0.0f,
        1.787500023841858f, 0.02500000037252903f, -7.813410718426894e-08f, 0.0f, 1.0f,
        1.787500023841858f, 0.013889255933463573f, -0.054045598953962326f, 1.0f, 1.0f,
        1.787500023841858f, 0.0f, 0.06499992311000824f, 0.0f, 0.0f,
        1.787500023841858f, 0.004877258092164993f, -0.06375111639499664f, 0.0f, 1.0f,
        1.787500023841858f, 0.013889255933463573f, -0.054045598953962326f, 1.0f, 1.0f,
        1.787500023841858f, 0.02500000037252903f, -7.813410718426894e-08f, 0.0f, 0.0f,
        1.787500023841858f, 0.01767767034471035f, 0.045961860567331314f, 0.0f, 1.0f,
        1.787500023841858f, 0.0f, 0.06499992311000824f, 1.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, 0.012680948711931705f, 0.0f, 0.0f,
        -1.787500023841858f, 0.02500000037252903f, 7.813410718426894e-08f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 1.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 0.0f, 0.0f,
        -1.787500023841858f, 0.02309698797762394f, -0.02487434633076191f, 0.0f, 1.0f,
        -1.787500023841858f, 0.020786739885807037f, -0.0361119881272316f, 1.0f, 1.0f,
        -1.787500023841858f, 0.020786739885807037f, -0.0361119881272316f, 0.0f, 0.0f,
        -1.787500023841858f, 0.01767767034471035f, -0.045961860567331314f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 1.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, -0.045961860567331314f, 0.0f, 0.0f,
        -1.787500023841858f, 0.013889255933463573f, -0.05404544249176979f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 1.0f, 1.0f,
        -1.787500023841858f, 0.013889255933463573f, -0.05404544249176979f, 0.0f, 0.0f,
        -1.787500023841858f, 0.009567086584866047f, -0.06005208566784859f, 0.0f, 1.0f,
        -1.787500023841858f, 0.004877258092164993f, -0.0637509673833847f, 1.0f, 1.0f,
        -1.787500023841858f, 0.004877258092164993f, -0.0637509673833847f, 0.0f, 0.0f,
        -1.787500023841858f, 0.0f, -0.06499992311000824f, 0.0f, 1.0f,
        -1.787500023841858f, 0.0f, 0.06500007212162018f, 1.0f, 1.0f,
        -1.787500023841858f, 0.0f, 0.06500007212162018f, 0.0f, 0.0f,
        -1.787500023841858f, 0.004877258092164993f, 0.06375111639499664f, 0.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, 0.04596201702952385f, 1.0f, 1.0f,
        -1.787500023841858f, 0.004877258092164993f, 0.06375111639499664f, 0.0f, 0.0f,
        -1.787500023841858f, 0.009567086584866047f, 0.06005224213004112f, 0.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, 0.04596201702952385f, 1.0f, 1.0f,
        -1.787500023841858f, 0.009567086584866047f, 0.06005224213004112f, 0.0f, 0.0f,
        -1.787500023841858f, 0.013889255933463573f, 0.054045598953962326f, 0.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, 0.04596201702952385f, 1.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, 0.04596201702952385f, 0.0f, 0.0f,
        -1.787500023841858f, 0.020786739885807037f, 0.03611214458942413f, 0.0f, 1.0f,
        -1.787500023841858f, 0.02309698797762394f, 0.024874502792954445f, 1.0f, 1.0f,
        -1.787500023841858f, 0.02309698797762394f, 0.024874502792954445f, 0.0f, 0.0f,
        -1.787500023841858f, 0.024519631639122963f, 0.012680948711931705f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 1.0f, 1.0f,
        -1.787500023841858f, 0.013889255933463573f, -0.05404544249176979f, 0.0f, 0.0f,
        -1.787500023841858f, 0.004877258092164993f, -0.0637509673833847f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 1.0f, 1.0f,
        -1.787500023841858f, 0.004877258092164993f, -0.0637509673833847f, 0.0f, 0.0f,
        -1.787500023841858f, 0.0f, 0.06500007212162018f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 1.0f, 1.0f,
        -1.787500023841858f, 0.01767767034471035f, 0.04596201702952385f, 0.0f, 0.0f,
        -1.787500023841858f, 0.02309698797762394f, 0.024874502792954445f, 0.0f, 1.0f,
        -1.787500023841858f, 0.024519631639122963f, -0.01268079224973917f, 1.0f, 1.0f,

        -3.3450000286102295f, -0.09999996423721313f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3206136226654053f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3157365322113037f, -0.061528801918029785f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3206136226654053f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2971646785736084f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.287597417831421f, -0.09567224979400635f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2971646785736084f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2755537033081055f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.261664390563965f, -0.1511181890964508f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2755537033081055f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2566115856170654f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.23893404006958f, -0.22573590278625488f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2566115856170654f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2410662174224854f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2202796936035156f, -0.31665781140327454f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2410662174224854f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2295150756835938f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.206418037414551f, -0.42038989067077637f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2295150756835938f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2224018573760986f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.1978821754455566f, -0.5329457521438599f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2224018573760986f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2200000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.194999933242798f, -0.6499999761581421f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2200000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2224018573760986f, -0.7572996616363525f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.1978821754455566f, -0.7670542001724243f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2224018573760986f, -0.7572996616363525f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2295150756835938f, -0.8604758977890015f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.206418037414551f, -0.8796100616455078f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2295150756835938f, -0.8604758977890015f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2410662174224854f, -0.9555636644363403f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2202796936035156f, -0.983342170715332f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2410662174224854f, -0.9555636644363403f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2566115856170654f, -1.0389087200164795f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.23893404006958f, -1.0742640495300293f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2566115856170654f, -1.0389087200164795f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2755537033081055f, -1.107308268547058f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.261664390563965f, -1.1488817930221558f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2755537033081055f, -1.107308268547058f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2971646785736084f, -1.1581337451934814f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.287597417831421f, -1.2043277025222778f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2971646785736084f, -1.1581337451934814f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3206136226654053f, -1.18943190574646f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3157365322113037f, -1.2384711503982544f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3206136226654053f, -1.18943190574646f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3450000286102295f, -1.2000000476837158f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3450000286102295f, -1.25f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3450000286102295f, -1.2000000476837158f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3693864345550537f, -1.18943190574646f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3742635250091553f, -1.2384711503982544f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3693864345550537f, -1.18943190574646f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3928353786468506f, -1.1581337451934814f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.402402639389038f, -1.2043277025222778f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3928353786468506f, -1.1581337451934814f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4144463539123535f, -1.107308268547058f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.428335666656494f, -1.1488817930221558f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4144463539123535f, -1.107308268547058f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4333884716033936f, -1.0389087200164795f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.451066017150879f, -1.0742640495300293f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4333884716033936f, -1.0389087200164795f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4489338397979736f, -0.9555636644363403f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4697203636169434f, -0.983342170715332f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4489338397979736f, -0.9555636644363403f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4604849815368652f, -0.8604758977890015f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.483582019805908f, -0.8796100616455078f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4604849815368652f, -0.8604758977890015f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4675981998443604f, -0.7572996616363525f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4921178817749023f, -0.7670542001724243f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4675981998443604f, -0.7572996616363525f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4700000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.495000123977661f, -0.6499999761581421f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4700000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4675981998443604f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4921178817749023f, -0.5329457521438599f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4675981998443604f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4604849815368652f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.483582019805908f, -0.42038989067077637f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4604849815368652f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4489338397979736f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4697203636169434f, -0.31665781140327454f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4489338397979736f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4333884716033936f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.451066017150879f, -0.22573590278625488f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4333884716033936f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4144463539123535f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.428335666656494f, -0.1511181890964508f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4144463539123535f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3928353786468506f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.402402639389038f, -0.09567224979400635f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3928353786468506f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3693864345550537f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3742635250091553f, -0.061528801918029785f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3693864345550537f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3450000286102295f, -0.09999996423721313f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3450000286102295f, -0.04999995231628418f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3157365322113037f, -0.061528801918029785f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.3206136226654053f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3450000286102295f, -0.04999995231628418f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3450000286102295f, -0.09999996423721313f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3693864345550537f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3450000286102295f, -0.04999995231628418f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.287597417831421f, -0.09567224979400635f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2971646785736084f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3157365322113037f, -0.061528801918029785f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3693864345550537f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3742635250091553f, -0.061528801918029785f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.3450000286102295f, -0.04999995231628418f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4921178817749023f, -0.7670542001724243f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.495000123977661f, -0.6499999761581421f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4700000286102295f, -0.6499999761581421f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3693864345550537f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3928353786468506f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3742635250091553f, -0.061528801918029785f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.261664390563965f, -0.1511181890964508f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2755537033081055f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.287597417831421f, -0.09567224979400635f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2224018573760986f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2295150756835938f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.206418037414551f, -0.42038989067077637f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2971646785736084f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3206136226654053f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3157365322113037f, -0.061528801918029785f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3928353786468506f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4144463539123535f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.402402639389038f, -0.09567224979400635f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3206136226654053f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3693864345550537f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3450000286102295f, -0.09999996423721313f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2971646785736084f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3928353786468506f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3206136226654053f, -0.11056804656982422f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.402402639389038f, -1.2043277025222778f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.428335666656494f, -1.1488817930221558f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4144463539123535f, -1.107308268547058f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2755537033081055f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3928353786468506f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2971646785736084f, -0.14186620712280273f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2755537033081055f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4144463539123535f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3928353786468506f, -0.14186620712280273f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2755537033081055f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2971646785736084f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.287597417831421f, -0.09567224979400635f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.23893404006958f, -0.22573590278625488f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2566115856170654f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.261664390563965f, -0.1511181890964508f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4144463539123535f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4333884716033936f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.428335666656494f, -0.1511181890964508f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.261664390563965f, -1.1488817930221558f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2755537033081055f, -1.107308268547058f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2566115856170654f, -1.0389087200164795f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2566115856170654f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4144463539123535f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2755537033081055f, -0.1926916539669037f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2566115856170654f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4333884716033936f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4144463539123535f, -0.1926916539669037f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4489338397979736f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4697203636169434f, -0.31665781140327454f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.451066017150879f, -0.22573590278625488f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2202796936035156f, -0.31665781140327454f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2410662174224854f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.23893404006958f, -0.22573590278625488f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4333884716033936f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4489338397979736f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.451066017150879f, -0.22573590278625488f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4144463539123535f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.428335666656494f, -0.1511181890964508f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.402402639389038f, -0.09567224979400635f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2410662174224854f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4333884716033936f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2566115856170654f, -0.2610912322998047f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2410662174224854f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4489338397979736f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4333884716033936f, -0.2610912322998047f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.451066017150879f, -1.0742640495300293f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.4697203636169434f, -0.983342170715332f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4489338397979736f, -0.9555636644363403f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.206418037414551f, -0.42038989067077637f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2295150756835938f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2202796936035156f, -0.31665781140327454f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4489338397979736f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4604849815368652f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4697203636169434f, -0.31665781140327454f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2202796936035156f, -0.983342170715332f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2410662174224854f, -0.9555636644363403f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2295150756835938f, -0.8604758977890015f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2295150756835938f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4489338397979736f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2410662174224854f, -0.34443631768226624f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2295150756835938f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4604849815368652f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4489338397979736f, -0.34443631768226624f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3157365322113037f, -1.2384711503982544f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.3206136226654053f, -1.18943190574646f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2971646785736084f, -1.1581337451934814f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.1978821754455566f, -0.5329457521438599f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2224018573760986f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.206418037414551f, -0.42038989067077637f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4604849815368652f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4675981998443604f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.483582019805908f, -0.42038989067077637f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.206418037414551f, -0.8796100616455078f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2295150756835938f, -0.8604758977890015f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2224018573760986f, -0.7572996616363525f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2224018573760986f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4604849815368652f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2295150756835938f, -0.4395240545272827f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2224018573760986f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4675981998443604f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4604849815368652f, -0.4395240545272827f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3450000286102295f, -1.25f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.3742635250091553f, -1.2384711503982544f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.3693864345550537f, -1.18943190574646f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.194999933242798f, -0.6499999761581421f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2200000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.1978821754455566f, -0.5329457521438599f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4675981998443604f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4700000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4921178817749023f, -0.5329457521438599f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.4144463539123535f, -1.107308268547058f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4333884716033936f, -1.0389087200164795f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2566115856170654f, -1.0389087200164795f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2200000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4675981998443604f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2224018573760986f, -0.5427002906799316f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2200000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4700000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4675981998443604f, -0.5427002906799316f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2200000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2224018573760986f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.1978821754455566f, -0.5329457521438599f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.1978821754455566f, -0.7670542001724243f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2200000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.194999933242798f, -0.6499999761581421f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.1978821754455566f, -0.7670542001724243f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2224018573760986f, -0.7572996616363525f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2200000286102295f, -0.6499999761581421f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2224018573760986f, -0.7572996616363525f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4700000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2200000286102295f, -0.6499999761581421f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2224018573760986f, -0.7572996616363525f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4675981998443604f, -0.7572996616363525f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4700000286102295f, -0.6499999761581421f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4675981998443604f, -0.7572996616363525f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4921178817749023f, -0.7670542001724243f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4700000286102295f, -0.6499999761581421f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4700000286102295f, -0.6499999761581421f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.495000123977661f, -0.6499999761581421f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4921178817749023f, -0.5329457521438599f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.1978821754455566f, -0.7670542001724243f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.206418037414551f, -0.8796100616455078f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.2224018573760986f, -0.7572996616363525f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2295150756835938f, -0.8604758977890015f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4675981998443604f, -0.7572996616363525f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2224018573760986f, -0.7572996616363525f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2295150756835938f, -0.8604758977890015f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4604849815368652f, -0.8604758977890015f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4675981998443604f, -0.7572996616363525f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4604849815368652f, -0.8604758977890015f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.483582019805908f, -0.8796100616455078f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4675981998443604f, -0.7572996616363525f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.483582019805908f, -0.8796100616455078f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.4921178817749023f, -0.7670542001724243f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4675981998443604f, -0.7572996616363525f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4675981998443604f, -0.5427002906799316f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4921178817749023f, -0.5329457521438599f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.483582019805908f, -0.42038989067077637f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.206418037414551f, -0.8796100616455078f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2202796936035156f, -0.983342170715332f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.2295150756835938f, -0.8604758977890015f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2410662174224854f, -0.9555636644363403f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4604849815368652f, -0.8604758977890015f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2295150756835938f, -0.8604758977890015f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2410662174224854f, -0.9555636644363403f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4489338397979736f, -0.9555636644363403f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4604849815368652f, -0.8604758977890015f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4489338397979736f, -0.9555636644363403f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4697203636169434f, -0.983342170715332f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4604849815368652f, -0.8604758977890015f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4697203636169434f, -0.983342170715332f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.483582019805908f, -0.8796100616455078f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4604849815368652f, -0.8604758977890015f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4604849815368652f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.483582019805908f, -0.42038989067077637f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4697203636169434f, -0.31665781140327454f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2202796936035156f, -0.983342170715332f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.23893404006958f, -1.0742640495300293f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.2410662174224854f, -0.9555636644363403f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2566115856170654f, -1.0389087200164795f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4489338397979736f, -0.9555636644363403f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2410662174224854f, -0.9555636644363403f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2566115856170654f, -1.0389087200164795f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4333884716033936f, -1.0389087200164795f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4489338397979736f, -0.9555636644363403f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4333884716033936f, -1.0389087200164795f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.451066017150879f, -1.0742640495300293f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4489338397979736f, -0.9555636644363403f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2410662174224854f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2566115856170654f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.23893404006958f, -0.22573590278625488f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.23893404006958f, -1.0742640495300293f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2566115856170654f, -1.0389087200164795f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2410662174224854f, -0.9555636644363403f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.23893404006958f, -1.0742640495300293f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.261664390563965f, -1.1488817930221558f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.2566115856170654f, -1.0389087200164795f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2755537033081055f, -1.107308268547058f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4144463539123535f, -1.107308268547058f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2566115856170654f, -1.0389087200164795f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3928353786468506f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3693864345550537f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3206136226654053f, -0.11056804656982422f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4144463539123535f, -1.107308268547058f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.428335666656494f, -1.1488817930221558f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4333884716033936f, -1.0389087200164795f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.4333884716033936f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.451066017150879f, -0.22573590278625488f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.428335666656494f, -0.1511181890964508f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.2566115856170654f, -0.2610912322998047f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2755537033081055f, -0.1926916539669037f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.261664390563965f, -0.1511181890964508f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.261664390563965f, -1.1488817930221558f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.287597417831421f, -1.2043277025222778f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.2755537033081055f, -1.107308268547058f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2971646785736084f, -1.1581337451934814f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.4144463539123535f, -1.107308268547058f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2755537033081055f, -1.107308268547058f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2971646785736084f, -1.1581337451934814f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3928353786468506f, -1.1581337451934814f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.4144463539123535f, -1.107308268547058f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3928353786468506f, -1.1581337451934814f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.402402639389038f, -1.2043277025222778f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4144463539123535f, -1.107308268547058f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.2295150756835938f, -0.4395240545272827f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.2410662174224854f, -0.34443631768226624f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2202796936035156f, -0.31665781140327454f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.287597417831421f, -1.2043277025222778f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.2971646785736084f, -1.1581337451934814f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2755537033081055f, -1.107308268547058f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.287597417831421f, -1.2043277025222778f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.3157365322113037f, -1.2384711503982544f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.2971646785736084f, -1.1581337451934814f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3206136226654053f, -1.18943190574646f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3928353786468506f, -1.1581337451934814f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.2971646785736084f, -1.1581337451934814f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3206136226654053f, -1.18943190574646f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3693864345550537f, -1.18943190574646f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3928353786468506f, -1.1581337451934814f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3693864345550537f, -1.18943190574646f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3742635250091553f, -1.2384711503982544f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.3928353786468506f, -1.1581337451934814f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3742635250091553f, -1.2384711503982544f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.402402639389038f, -1.2043277025222778f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.3928353786468506f, -1.1581337451934814f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3157365322113037f, -1.2384711503982544f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.3450000286102295f, -1.25f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.3206136226654053f, -1.18943190574646f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3450000286102295f, -1.2000000476837158f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3693864345550537f, -1.18943190574646f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3206136226654053f, -1.18943190574646f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3450000286102295f, -1.2000000476837158f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3450000286102295f, -1.25f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.3693864345550537f, -1.18943190574646f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.3928353786468506f, -0.14186620712280273f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.402402639389038f, -0.09567224979400635f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.3742635250091553f, -0.061528801918029785f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3206136226654053f, -0.11056804656982422f, 0.02500000223517418f, 0.0f, 0.0f,
        -3.3450000286102295f, -0.09999996423721313f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3450000286102295f, -0.04999995231628418f, -0.05000000074505806f, 1.0f, 1.0f,
        -3.3450000286102295f, -1.25f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.3450000286102295f, -1.2000000476837158f, 0.02500000223517418f, 0.0f, 1.0f,
        -3.3206136226654053f, -1.18943190574646f, 0.02500000223517418f, 1.0f, 1.0f,
        -3.428335666656494f, -1.1488817930221558f, -0.05000000074505806f, 0.0f, 0.0f,
        -3.451066017150879f, -1.0742640495300293f, -0.05000000074505806f, 0.0f, 1.0f,
        -3.4333884716033936f, -1.0389087200164795f, 0.02500000223517418f, 1.0f, 1.0f,
    };

    // World space positions of our objects
    glm::vec3 objectPositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f), // Scene
        glm::vec3(3.815f,  -0.55f,  -3.95f), // Front Door
        glm::vec3(-1.425f,  -2.58f,  -3.95f), // Bottom Lower Window Blind
        glm::vec3(-1.425f,  0.72f,  -3.95f), // Bottom Upper Window Blind
        glm::vec3(2.0375f,  -5.4f,  -4.065f), // Front Door Floor Seal
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO


    // Load and create a texture 
    GLuint texture1, textureFloor, textureCeiling, textureCitro, textureSeal, textureGuard, textureHandle;
    // ====================
    // Texture 1
    // ====================
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image("OrangeTexture.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    // ====================
    // Texture for Floor
    // ====================
    glGenTextures(1, &textureFloor);
    glBindTexture(GL_TEXTURE_2D, textureFloor);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("WoodenTexture.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ====================
    // Texture for Ceiling
    // ====================
    glGenTextures(1, &textureCeiling);
    glBindTexture(GL_TEXTURE_2D, textureCeiling);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("BlankTexture.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ====================
    // Texture for Citro
    // ====================
    glGenTextures(1, &textureCitro);
    glBindTexture(GL_TEXTURE_2D, textureCitro);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("CitroTeaching.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ====================
    // Texture for Seal
    // ====================
    glGenTextures(1, &textureSeal);
    glBindTexture(GL_TEXTURE_2D, textureSeal);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("BlackTexture.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ====================
    // Texture for Guard
    // ====================
    glGenTextures(1, &textureGuard);
    glBindTexture(GL_TEXTURE_2D, textureGuard);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("YellowTexture.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ====================
    // Texture for Handle
    // ====================
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    image = SOIL_load_image("BronzeTexture.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(188.0f/255.0f, 239/255.0f, 248/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program, "wallTexture"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureFloor);
        glUniform1i(glGetUniformLocation(ourShader.Program, "floorTexture"), 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureCeiling);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ceilingTexture"), 2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, textureCitro);
        glUniform1i(glGetUniformLocation(ourShader.Program, "citroTexture"), 3);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textureSeal);
        glUniform1i(glGetUniformLocation(ourShader.Program, "sealTexture"), 4);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, textureGuard);
        glUniform1i(glGetUniformLocation(ourShader.Program, "guardTexture"), 5);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, textureHandle);
        glUniform1i(glGetUniformLocation(ourShader.Program, "handleTexture"), 6);

        // Activate shader
        ourShader.Use();

        // Create transformations for the view and projection (as before)
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection;

        switch(cameraMode) {
            case 0:
                view = glm::translate(view, glm::vec3(-1.1f, -0.375f, -6.35f));
                view = glm::rotate(view, glm::radians(2.25f), glm::vec3(0.0f, 1.0f, 0.0f));
                projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
                break;
            case 1:
                view = glm::translate(view, glm::vec3(-1.5f, 0.0f, -8.0f));
                projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
                break;
            case 2:
                view = glm::translate(view, glm::vec3(3.0f, 0.0f, -9.0f));
                view = glm::rotate(view, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
                break;
            case 3:
                view = glm::translate(view, glm::vec3(3.8f, 0.0f, -6.0f));
                view = glm::rotate(view, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
                break;
            case 4:
                view = glm::translate(view, glm::vec3(-1.2f, -0.375f, -1.0f));
                view = glm::rotate(view, glm::radians(2.25f), glm::vec3(0.0f, 1.0f, 0.0f));
                projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
                break;
        }
        
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        GLint textureTypeLoc = glGetUniformLocation(ourShader.Program, "textureType");

        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, objectPositions[0]);
        GLfloat angle = 0.0f;
        model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        glUniform1i(textureTypeLoc, 10); 
        glDrawArrays(GL_TRIANGLES, 0 * 6, 1 * 6); // First 6 sets correspond to Citro

        glUniform1i(textureTypeLoc, 1);
        glDrawArrays(GL_TRIANGLES, 1 * 6, 1 * 6);  // Next 6 sets correspond to the floor

        glUniform1i(textureTypeLoc, 2);
        glDrawArrays(GL_TRIANGLES, 2 * 6, 1 * 6);  // Next 6 sets correspond to the ceiling

        glUniform1i(textureTypeLoc, 0);
        glDrawArrays(GL_TRIANGLES, 3 * 6, 7 * 6);  // Next 7 sets correspond to the walls

        glUniform1i(textureTypeLoc, 2);
        glDrawArrays(GL_TRIANGLES, 10 * 6, 5 * 6);  // Front Door Frame Triangles
        glDrawArrays(GL_TRIANGLES, 15 * 6, 3 * 6); 
        glDrawArrays(GL_TRIANGLES, 18 * 6, 3 * 6);  

        glDrawArrays(GL_TRIANGLES, 21 * 6, 4 * 6);  // Window Frame Triangles
        glDrawArrays(GL_TRIANGLES, 25 * 6, 8 * 6); 
        glDrawArrays(GL_TRIANGLES, 33 * 6, 8 * 6);  
        glDrawArrays(GL_TRIANGLES, 41 * 6, 8 * 6);
        glDrawArrays(GL_TRIANGLES, 49 * 6, 8 * 6);
        glDrawArrays(GL_TRIANGLES, 57 * 6, 5 * 6);

        glUniform1i(textureTypeLoc, 4); // Front Wall Guard Triangles
        glDrawArrays(GL_TRIANGLES, 69 * 6, 3 * 6);

        glUniform1i(textureTypeLoc, 2);
        glDrawArrays(GL_TRIANGLES, 72 * 6, 3 * 6); // Closet Door Triangles

        glUniform1i(textureTypeLoc, 1); // Door Seal Triangles
        glm::mat4 doorSeal = glm::mat4(1.0f);
        doorSeal = glm::translate(doorSeal, objectPositions[4]);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(doorSeal));
        glDrawArrays(GL_TRIANGLES, 173 * 6, 31 * 6);

        if (movingBlinds[0]) {
            switch(openTheBlinds[0]){
                case true:
                    openBlinds(0);
                    break;
                case false:
                    resetBlinds(0);
                    break;
            }
        }
        if (movingBlinds[1]) {
            switch(openTheBlinds[1]){
                case true:
                    openBlinds(1);
                    break;
                case false:
                    resetBlinds(1);
                    break;
            }
        }
        glUniform1i(textureTypeLoc, 2);
        // Lower Blinds
        for (int i = 0; i < 10; i++) {
            glm::mat4 windowBlind = glm::mat4(1.0f);
            windowBlind = glm::translate(windowBlind, objectPositions[2] + glm::vec3(0.0f, 0.3f * i, 0.0f));
            windowBlind = glm::rotate(windowBlind, glm::radians(windowAngle[0]), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(windowBlind));
            glDrawArrays(GL_TRIANGLES, 75 * 6, 62 * 6);
        }
        // Upper Blinds
        for (int i = 0; i < 10; i++) {
            glm::mat4 windowBlind = glm::mat4(1.0f);
            windowBlind = glm::translate(windowBlind, objectPositions[3] + glm::vec3(0.0f, 0.3f * i, 0.0f));
            windowBlind = glm::rotate(windowBlind, glm::radians(windowAngle[1]), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(windowBlind));
            glDrawArrays(GL_TRIANGLES, 75 * 6, 62 * 6);
        }

        // Blind Rods
        for (int i = 0; i < 2; i++) {
            glm::mat4 windowRod = glm::mat4(1.0f);
            glm::vec3 blindOffset = glm::vec3(0.0f, -0.07f + (0.19f * std::cos(glm::radians(windowAngle[i]))), 0.19f * std::sin(glm::radians(windowAngle[i])));
            glm::vec3 adjustedPos = glm::vec3(objectPositions[i+2] + blindOffset);
            windowRod = glm::translate(windowRod, adjustedPos);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(windowRod));
            glDrawArrays(GL_TRIANGLES, 137 * 6, 36 * 6);
        }


        // Front Door
        if (movingDoor) {
            switch(openTheDoor){
                case true:
                    openDoor();
                    break;
                case false:
                    closeDoor();
                    break;
            }
        }
        glUniform1i(textureTypeLoc, 2);
        glm::mat4 door = glm::mat4(1.0f);
        door = glm::translate(door, objectPositions[1]);
        door = glm::rotate(door, glm::radians(doorAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(door));
        glDrawArrays(GL_TRIANGLES, 62 * 6, 5 * 6);
        // Door Seal
        glUniform1i(textureTypeLoc, 3); 
        glDrawArrays(GL_TRIANGLES, 67 * 6, 2 * 6);
        // Door Handle
        glUniform1i(textureTypeLoc, 5);
        glDrawArrays(GL_TRIANGLES, 204 * 6, 63 * 6);


        // End of Drawing
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        cameraMode = (cameraMode + 1) % 5;

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
        moveDoor(true);
    else if (key == GLFW_KEY_L && action == GLFW_PRESS)
        moveDoor(false);

    if (key == GLFW_KEY_U && action == GLFW_PRESS)
        moveBlinds(0, true);
    else if (key == GLFW_KEY_J && action == GLFW_PRESS)
        moveBlinds(0, false);

    if (key == GLFW_KEY_I && action == GLFW_PRESS)
        moveBlinds(1, true);
    else if (key == GLFW_KEY_K && action == GLFW_PRESS)
        moveBlinds(1, false);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
