/*
Last Date Modified: 1/9/2024

Description:

The objects are frozen in place until the user presses the “g” key and then 
objects start moving around at random speeds and rotating randomly. 
The objects’ movements are calculated in a thread. 
The objects are able to collide and bounce off each other. 
The objects are also confined to the space around the center of the scene.

*/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <cstdlib>
#include <random>
#include <functional>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

// Define the boundary of the object's movement.
#define xPositiveWall    14
#define xNegativeWall   -14
#define yPositiveWall    14
#define yNegativeWall   -14
#define zPositiveWall    15
#define zNegativeWall    3

extern int moveControl;

// Define the attributes of the object.
typedef struct item
{
    double objXPos;
    double objYPos;
    double objZPos;
    double objXSpeed;
    double objYSpeed;
    double objZSpeed;
    float objXRot;
    float objYRot;
    float objZRot;
    float objXRotSpeed;
    float objYRotSpeed;
    float objZRotSpeed;
}Item;

// Calculate the position and rotation of objects.
void kinematicTrajectory(Item * obj, int move)
{
    double distance1, distance2, distance3, distance4, distance5, distance6;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // If the user presses the g-key (move = 0), all objects will stop moving and rotating.
    if (move == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            obj[i].objXPos = obj[i].objXPos;
            obj[i].objYPos = obj[i].objYPos;
            obj[i].objZPos = obj[i].objZPos;
            obj[i].objXRot = obj[i].objXRot;
            obj[i].objYRot = obj[i].objYRot;
            obj[i].objZRot = obj[i].objZRot;
        }    
    }
    else
    {
        // 6 if-statements to handle the collision between objects.
        // Calculate the distance between two objects, if it is smaller than the threshold, the direction of the velocity of objects will change.
        distance1 = sqrt(pow(obj[0].objXPos - obj[1].objXPos, 2) + pow(obj[0].objYPos - obj[1].objYPos, 2) + pow(obj[0].objZPos - obj[1].objZPos, 2));
        distance2 = sqrt(pow(obj[0].objXPos - obj[2].objXPos, 2) + pow(obj[0].objYPos - obj[2].objYPos, 2) + pow(obj[0].objZPos - obj[2].objZPos, 2));
        distance3 = sqrt(pow(obj[0].objXPos - obj[3].objXPos, 2) + pow(obj[0].objYPos - obj[3].objYPos, 2) + pow(obj[0].objZPos - obj[3].objZPos, 2));
        distance4 = sqrt(pow(obj[1].objXPos - obj[2].objXPos, 2) + pow(obj[1].objYPos - obj[2].objYPos, 2) + pow(obj[1].objZPos - obj[2].objZPos, 2));
        distance5 = sqrt(pow(obj[1].objXPos - obj[3].objXPos, 2) + pow(obj[1].objYPos - obj[3].objYPos, 2) + pow(obj[1].objZPos - obj[3].objZPos, 2));
        distance6 = sqrt(pow(obj[2].objXPos - obj[3].objXPos, 2) + pow(obj[2].objYPos - obj[3].objYPos, 2) + pow(obj[2].objZPos - obj[3].objZPos, 2));
        
        // There are 6 if-statements to handle the object collision. After the collision, the direction will change and the speed is random.
        if (distance1 < 4.0) 
        {
            obj[0].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[0].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[0].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);

            obj[1].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[1].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[1].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);
        }

        if (distance2 < 4.0) 
        {
            obj[0].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[0].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[0].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);
            
            obj[2].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[2].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[2].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);
        }

        if (distance3 < 4.0) 
        {
            obj[0].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[0].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[0].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);

            obj[3].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[3].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[3].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);
        }

        if (distance4 < 4.0) 
        {
            obj[1].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[1].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[1].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);

            obj[2].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[2].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[2].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);
        }

        if (distance5 < 4.0) 
        {

            obj[1].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[1].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[1].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);

            obj[3].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[3].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[3].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);      
        }

        if (distance6 < 4.0) 
        {
            obj[2].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[2].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[2].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);

            obj[3].objXSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 1);
            obj[3].objYSpeed = (-1) * (2 * (((double)rand()) / RAND_MAX) - 0.5);
            obj[3].objZSpeed = (-1) * ((((double)rand()) / RAND_MAX) + 1);
        }

        // Update the position of all objects and handle the collision to wall.
        for (int i = 0; i < 4; i++)
        {
            // If the object is colliding with the wall at x-direction, the direction of speed will changed.
            if ((obj[i].objXPos <= xNegativeWall) || (obj[i].objXPos >= xPositiveWall))
            {   
                // If the collision happens, the x-direction of object's rotation will change.
                obj[i].objXRotSpeed = 0;
                obj[i].objYRotSpeed = (((double)rand()) / RAND_MAX) + 3;
                obj[i].objZRotSpeed = 0;

                // If the object is colliding with the x-direction wall, it will set the position at the boundary 
                // and change the direction of the speed.
                if (obj[i].objXPos < xNegativeWall)
                {
                    obj[i].objXPos = xNegativeWall;
                    if (obj[i].objXSpeed < 0)
                    {
                        obj[i].objXSpeed = (-1) * obj[i].objXSpeed;
                    }
                    
                }
                else if (obj[i].objXPos >= xPositiveWall)
                {
                    obj[i].objXPos = xPositiveWall;
                    if (obj[i].objXSpeed > 0)
                    {
                        obj[i].objXSpeed = (-1) * obj[i].objXSpeed;
                    }
                }
            }

            // If the object is colliding with the wall at y-direction, the direction of speed will changed.
            if ((obj[i].objYPos <= yNegativeWall) || (obj[i].objYPos >= yPositiveWall))
            {
                // If the collision happens, the y-direction of object's rotation will change.
                obj[i].objXRotSpeed = 0;
                obj[i].objYRotSpeed = 0;
                obj[i].objZRotSpeed = (((double)rand()) / RAND_MAX) + 3;

                // If the object is colliding with the y-direction wall, it will set the position at the boundary 
                // and change the direction of the speed.
                if (obj[i].objYPos < yNegativeWall)
                {
                    obj[i].objYPos = yNegativeWall;
                    if (obj[i].objYSpeed < 0)
                    {
                        obj[i].objYSpeed = (-1) * obj[i].objYSpeed;
                    }
                    
                }
                else if (obj[i].objYPos >= yPositiveWall)
                {
                    obj[i].objYPos = yPositiveWall;
                    if (obj[i].objYSpeed > 0)
                    {
                        obj[i].objYSpeed = (-1) * obj[i].objYSpeed;
                    }
                }
            }

            // If the object is colliding with the wall at z-direction, the direction of speed will changed.
            if ((obj[i].objZPos < zNegativeWall) || (obj[i].objZPos >= zPositiveWall))
            {
                // If the collision happens, the z-direction of object's rotation will change.
                obj[i].objXRotSpeed = (((double)rand()) / RAND_MAX) + 3;
                obj[i].objYRotSpeed = 0;
                obj[i].objZRotSpeed = 0;

                // If the object is colliding with the z-direction wall, it will set the position at the boundary 
                // and change the direction of the speed.
                if (obj[i].objZPos < zNegativeWall)
                {
                    obj[i].objZPos = zNegativeWall;
                    if (obj[i].objZSpeed < 0)
                    {
                        obj[i].objZSpeed = (-1) * obj[i].objZSpeed;
                    }
                    
                }
                else if (obj[i].objZPos >= zPositiveWall)
                {
                    obj[i].objZPos = zPositiveWall;
                    if (obj[i].objZSpeed > 0)
                    {
                        obj[i].objZSpeed = (-1) * obj[i].objZSpeed;
                    }
                }
            }


            // Update the rotation and handle the collision to the wall.
            obj[i].objXRot = obj[i].objXRot + obj[i].objXRotSpeed;
            obj[i].objYRot = obj[i].objYRot + obj[i].objYRotSpeed;
            obj[i].objZRot = obj[i].objZRot + obj[i].objZRotSpeed;

            // If the angle of the rotation is larger than 360 or smaller than 0,
            // the following if-else if statement will deal with it.
            if (obj[i].objXRot >= 360)
            {
                obj[i].objXRot = obj[i].objXRot - 360;
            }
            else if (obj[i].objXRot <= 0)
            {
                obj[i].objXRot = obj[i].objXRot + 360;
            }

            if (obj[i].objYRot >= 360)
            {
                obj[i].objYRot = obj[i].objYRot - 360;
            }
            else if (obj[i].objYRot <= 0)
            {
                obj[i].objYRot = obj[i].objYRot + 360;
            }

            if (obj[i].objZRot >= 360)
            {
                obj[i].objZRot = obj[i].objZRot - 360;
            }
            else if (obj[i].objZRot <= 0)
            {
                obj[i].objZRot = obj[i].objZRot + 360;
            }

            // Update the position of objects.
            obj[i].objXPos = obj[i].objXPos + (obj[i].objXSpeed * 0.05);
            obj[i].objYPos = obj[i].objYPos + (obj[i].objYSpeed * 0.02);
            obj[i].objZPos = obj[i].objZPos + (obj[i].objZSpeed * 0.1);
        }
    }
}

// This function is to change the internal light of the object randomly.
float randomLightIntensity()
{
    float intensity;
    std::srand(time(nullptr));

    intensity = rand() % 3;

    return intensity;
}


int main(void)
{
    float lightIntensity = 1;
    int counter = 2;

    // Initialise GLFW
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1920, 1080, "Final Project - 4 Objects Random Movement", NULL, NULL);
    if(window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) 
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Black background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS); 

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

    // Get a handle for our "JustGreen" and "LightComponent" uniform to control the light on the objects and the color of the xy-plane.
    GLuint JustGreen = glGetUniformLocation(programID, "JustGreen");
    GLuint LightComponent = glGetUniformLocation(programID, "LightComponent");
    //GLuint LightPower = glGetUniformLocation(programID, "LightPower");
    GLuint LightPower2 = glGetUniformLocation(programID, "LightPower2");
    glUseProgram(programID);

    // Get a handle for our buffers
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
    GLuint vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

    // Load the texture for the object.
    GLuint Texture = loadDDS("uvmap.DDS");

    // Load the texture for the floor.
    GLuint Texture2 = loadBMP_custom("spooky.bmp");
    
    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Read our .obj file
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool res = loadOBJ("suzanne.obj", vertices, uvs, normals);

    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

    // Load it into a VBO

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // The coordinates for the textured image.
    static const GLfloat g_vertex_buffer_data[] = 
    {
        15.0f, -15.0f, 0.0f,  
        -15.0f, -15.0f, 0.0f, 
        -15.0f, 15.0f, 0.0f,  
        15.0f, -15.0f, 0.0f,  
        15.0f, 15.0f, 0.0f,   
        -15.0f, 15.0f, 0.0f,  
    };

    // The UV coordinates for the textured image.
    static const GLfloat g_uv_buffer_data[] = 
    {
        1.0f, 0.0f,	
        0.0f, 0.0f,  	
   	    0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    // Generate and store the coordinates for the floor with a textured image.
    GLuint vertexbuffer2;
    glGenBuffers(1, &vertexbuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Generate and store the UV coordinates for the floor with a textured image.
    GLuint uvbuffer2;
	glGenBuffers(1, &uvbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);


    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    GLuint LightID2 = glGetUniformLocation(programID, "LightPosition_worldspace2");

    // Initialize 4 objects initial positions, rotations, and their random speed.
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    Item obj[4];   
    obj[0].objXPos = 4;
    obj[0].objYPos = 0.0;
    obj[0].objZPos = 3;
    obj[0].objXRot = 90;
    obj[0].objYRot = 90;
    obj[0].objZRot = 0;
    obj[1].objXPos = -4;
    obj[1].objYPos = 0.0;
    obj[1].objZPos = 3;
    obj[1].objXRot = 90;
    obj[1].objYRot = -90;
    obj[1].objZRot = 0;
    obj[2].objXPos = 0.0;
    obj[2].objYPos = 4;
    obj[2].objZPos = 3;
    obj[2].objXRot = 90;
    obj[2].objYRot = 180;
    obj[2].objZRot = 0;
    obj[3].objXPos = 0.0;
    obj[3].objYPos = -4;
    obj[3].objZPos = 3;
    obj[3].objXRot = 90;
    obj[3].objYRot = 0;
    obj[3].objZRot = 0;


    for (int i = 0; i < 4; ++i) 
    {
        obj[i].objXSpeed = 2 * (((double)rand()) / RAND_MAX) - 1;
        obj[i].objYSpeed = 2 * (((double)rand()) / RAND_MAX) - 1;
        obj[i].objZSpeed = (((double)rand()) / RAND_MAX) + 1;
        obj[i].objXRotSpeed = 0;
        obj[i].objYRotSpeed = 0;
        obj[i].objZRotSpeed = (((double)rand()) / RAND_MAX) + 1;
    }

    do
    {
        // This statement is used to change the light intensity randomly but make sure that 
        // the frequency of change is not too fast.
        if (moveControl != 0)
        {
            counter--;
            if (counter == 0)
            {
                lightIntensity = randomLightIntensity();
                counter = 2;
            }
        }

        // The calculation of the kinematics of objects is conducted by the thread.
        std::thread wk(kinematicTrajectory, obj, moveControl);
        wk.join();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glUseProgram(programID);

        ////// Start of the rendering of the first object //////
        
        glm::vec3 lightPos = glm::vec3(0,0,25);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        // Change the internal light's intensity randomly for the object.
        glUniform3f(LightID2, obj[0].objXPos, obj[0].objYPos, obj[0].objZPos);
        glUniform1f(LightPower2, lightIntensity);

        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]); // This one doesn't change between objects, so this can be done once for all objects that use "programID"
        
        // Set the kinetic matrix of the object.
        glm::mat4 ModelMatrix1 = glm::mat4(1.0);
        ModelMatrix1 = glm::translate(ModelMatrix1, glm::vec3(obj[0].objXPos, obj[0].objYPos, obj[0].objZPos));
        ModelMatrix1 = glm::rotate(ModelMatrix1, glm::radians(obj[0].objXRot), glm::vec3(1.0f, 0.0f, 0.0f));
        ModelMatrix1 = glm::rotate(ModelMatrix1, glm::radians(obj[0].objYRot), glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix1 = glm::rotate(ModelMatrix1, glm::radians(obj[0].objZRot), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 MVP1 = ProjectionMatrix * ViewMatrix * ModelMatrix1;
        

        // Send our transformation to the currently bound shader, in the "MVP1" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix1[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer
        (
            vertexPosition_modelspaceID, // The attribute we want to configure
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(vertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer
        (
            vertexUVID,                       // The attribute we want to configure
            2,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(vertexNormal_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer
        (
            vertexNormal_modelspaceID,        // The attribute we want to configure
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles !
        glUniform1i(JustGreen, 0);
        glDrawElements
        (
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT,   // type
            (void*)0           // element array buffer offset
        );

        ////// End of rendering of the first object //////

        ////// Start of the rendering of the second object //////

        // Set the kinetic matrix of the object.
        glm::mat4 ModelMatrix2 = glm::mat4(1.0);
        ModelMatrix2 = glm::translate(ModelMatrix2, glm::vec3(obj[1].objXPos, obj[1].objYPos, obj[1].objZPos));
        ModelMatrix2 = glm::rotate(ModelMatrix2, glm::radians(obj[1].objXRot), glm::vec3(1.0f, 0.0f, 0.0f));
        ModelMatrix2 = glm::rotate(ModelMatrix2, glm::radians(obj[1].objYRot), glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix2 = glm::rotate(ModelMatrix2, glm::radians(obj[1].objZRot), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 MVP2 = ProjectionMatrix * ViewMatrix * ModelMatrix2;


        // Send our transformation to the currently bound shader in the "MVP2" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix2[0][0]);

        // Change the internal light's intensity randomly for the object.
        glUniform3f(LightID2, obj[1].objXPos, obj[1].objYPos, obj[1].objZPos);
        glUniform1f(LightPower2, lightIntensity);

        // The rest is exactly the same as the first object
        
        // 1rst attribute buffer : vertices
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(vertexNormal_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
       

        // Draw the triangles !
        glUniform1i(JustGreen, 0);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        ////// End of rendering of the second object //////

        ////// Start of the rendering of the third object //////

        // Set the kinetic matrix of the object.
        glm::mat4 ModelMatrix3 = glm::mat4(1.0);
        ModelMatrix3 = glm::translate(ModelMatrix3, glm::vec3(obj[2].objXPos, obj[2].objYPos, obj[2].objZPos));
        ModelMatrix3 = glm::rotate(ModelMatrix3, glm::radians(obj[2].objXRot), glm::vec3(1.0f, 0.0f, 0.0f));
        ModelMatrix3 = glm::rotate(ModelMatrix3, glm::radians(obj[2].objYRot), glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix3 = glm::rotate(ModelMatrix3, glm::radians(obj[2].objZRot), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 MVP3 = ProjectionMatrix * ViewMatrix * ModelMatrix3;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP3[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix3[0][0]);

        // Change the internal light's intensity randomly for the object.
        glUniform3f(LightID2, obj[2].objXPos, obj[2].objYPos, obj[2].objZPos);
        glUniform1f(LightPower2, lightIntensity);
        
        // 1rst attribute buffer : vertices
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(vertexNormal_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles !
        glUniform1i(JustGreen, 0);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        ////// End of rendering of the third object //////

        ////// Start of the rendering of the forth object //////

        // Set the kinetic matrix of the object.
        glm::mat4 ModelMatrix4 = glm::mat4(1.0);
        ModelMatrix4 = glm::translate(ModelMatrix4, glm::vec3(obj[3].objXPos, obj[3].objYPos, obj[3].objZPos));
        ModelMatrix4 = glm::rotate(ModelMatrix4, glm::radians(obj[3].objXRot), glm::vec3(1.0f, 0.0f, 0.0f));
        ModelMatrix4 = glm::rotate(ModelMatrix4, glm::radians(obj[3].objYRot), glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix4 = glm::rotate(ModelMatrix4, glm::radians(obj[3].objZRot), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 MVP4 = ProjectionMatrix * ViewMatrix * ModelMatrix4;


        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP4[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix4[0][0]);

        // Change the internal light's intensity randomly for the object.
        glUniform3f(LightID2, obj[3].objXPos, obj[3].objYPos, obj[3].objZPos);
        glUniform1f(LightPower2, lightIntensity);
        
        // 1rst attribute buffer : vertices
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : UVs
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 3rd attribute buffer : normals
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(vertexNormal_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
       

        // Draw the triangles !
        glUniform1i(JustGreen, 0);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

        ////// End of rendering of the fourth object //////


        ////// Start of the rendering of the textured floor //////

        // Set the kinetic matrix of the floor.
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

        // added Bind our texture in Texture Unit 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Texture2);
        // added Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 1);

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
        glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // added 2nd attribute buffer : UVs
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
        glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Draw the triangleS !
        glUniform1i(JustGreen, 0);
        glDrawArrays(GL_TRIANGLES, 0, 2*3);

        ////// End of rendering of the xy-plane object //////

        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexUVID);
        glDisableVertexAttribArray(vertexNormal_modelspaceID);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteBuffers(1, &vertexbuffer2);
    glDeleteBuffers(1, &uvbuffer2);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
