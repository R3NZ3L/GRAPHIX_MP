#include "main.h"
#include "Classes/Classes.h"
/*
    Global Variables
*/
// Screen width and height
float screenWidth = 1000.0f;
float screenHeight = 1000.0f;
Mode mode = Mode::TPS;
// Camera Positioning
OrthoCamera td_camera;
PerspectiveCamera tps_camera;
PerspectiveCamera fps_camera;

glm::vec3 tps_cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 fps_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 td_cameraPos = glm::vec3(0.0f, 20.0f, 0.0f);
glm::vec3 worldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

// Camera Rotation (yaw and pitch)
bool    firstMouse = true;
float   lastX = 500.0f, lastY = 500.0f,
yaw = -90.0f, pitch = 0.0f,
sensitivity = 0.1f;

// Camera Toggle
bool toggle_tps = true;
bool toggle_fps = false;
bool toggle_td = false;

// Player positioning
float moveX = 0.0f;
float moveY = 0.0f;
float moveZ = 0.0f;
int face = 0;
float rot_y = 90.0f;

// Player object
PlayerClass playerSub("3D/submarine/submarine.obj", glm::vec3(0.0f, 0.0f, 0.0f));

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Toggle TPS and FPS Camera
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (toggle_tps && !toggle_td) {
            toggle_tps = false;
            toggle_fps = true;
        }
        else if (toggle_fps && !toggle_td) {
            toggle_fps = false;
            toggle_tps = true;
        }
    }

    // Toggle TD Camera
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        if (toggle_td) {
            toggle_td = false;
        }
        else {
            toggle_td = true;
        }
    }

    // Submarine Forward/Backward Movement Controls
    if (key == GLFW_KEY_W) {
        if ((toggle_fps || toggle_tps) && !toggle_td) {
            moveZ -= 0.1f;
            tps_cameraPos -= glm::vec3(0.0f, 0.0f, 0.1f);
            fps_cameraPos -= glm::vec3(0.0f, 0.0f, 0.1f);

            playerSub.updatePosition(moveX, moveY, moveZ);
        }
    } else if (key == GLFW_KEY_S) {
        if ((toggle_fps || toggle_tps) && !toggle_td) {
            moveZ += 0.1f;
            tps_cameraPos += glm::vec3(0.0f, 0.0f, 0.1f);
            fps_cameraPos += glm::vec3(0.0f, 0.0f, 0.1f);

            playerSub.updatePosition(moveX, moveY, moveZ);
        }
    }
    
    // Submarine Turn Left/Turn Right Movement Controls
    if (key == GLFW_KEY_A) {
        if ((toggle_fps || toggle_tps) && !toggle_td) {
            moveX -= 0.1f;
            tps_cameraPos -= glm::vec3(0.1f, 0.0f, 0.0f);
            fps_cameraPos -= glm::vec3(0.1f, 0.0f, 0.0f);

            playerSub.updatePosition(moveX, moveY, moveZ);
        }
    } else if (key == GLFW_KEY_D) {
        if ((toggle_fps || toggle_tps) && !toggle_td) {
            moveX += 0.1f;
            tps_cameraPos += glm::vec3(0.1f, 0.0f, 0.0f);
            fps_cameraPos += glm::vec3(0.1f, 0.0f, 0.0f);

            playerSub.updatePosition(moveX, moveY, moveZ);
        }
    }

    // Submarine Ascend/Descend Movement Controls
    if (key == GLFW_KEY_Q) {
        if ((toggle_fps || toggle_tps) && !toggle_td) {
            if (moveY + 0.1f <= 0) {
                moveY += 0.1f;
                tps_cameraPos += glm::vec3(0.0f, 0.1f, 0.0f);
                fps_cameraPos += glm::vec3(0.0f, 0.1f, 0.0f);

                playerSub.updatePosition(moveX, moveY, moveZ);
            }
        }
    } else if (key == GLFW_KEY_E) {
        if ((toggle_fps || toggle_tps) && !toggle_td) {
            moveY -= 0.1f;
            tps_cameraPos -= glm::vec3(0.0f, 0.1f, 0.0f);
            fps_cameraPos -= glm::vec3(0.0f, 0.1f, 0.0f);

            playerSub.updatePosition(moveX, moveY, moveZ);
        }
    }

    /*
     * Since the Program will be capturing the mouse, ensure
     * that there is a way to conveniently close the program.
     */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
    /*
        Placing the cursor to the center of the screen (500.0f, 500.0f)
        when the user clicks on the window
    */
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    /*
        Calculating offsets on the X and Y-axis movement (according to a set sensitivity),
        as well as saving the last X and Y position of the cursor in case the window becomes out of focus.
    */
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;

    /*
        Setting limits on how tilted upwards or downwards can the camera be.
    */
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }

    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    /*
        Updating the position of the camera.
    */
    glm::vec3 direction = glm::vec3(
        (cos(glm::radians(yaw)) * cos(glm::radians(pitch))),
        (sin(glm::radians(pitch))),
        (sin(glm::radians(yaw)) * cos(glm::radians(pitch)))
    );

    tps_cameraPos = playerSub.getPosition() - glm::normalize(direction);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "No Man's Submarine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // -------------------------------------------------------
    // LOADING OBJECTS

    playerSub.loadObj();

    EnemyClass donut("3D/donut/Donut.obj", glm::vec3(0.0f, -5.0f, -10.0f));
    EnemyClass sphere("3D/sphere/Sphere.obj", glm::vec3(0.0f, -8.0f, 0.0f));
    EnemyClass car("3D/car/PASSAT_OBJ.obj", glm::vec3(10.0f, -10.0f, -15.0f));
    EnemyClass chest("3D/chest/TreasureChestSimple.obj", glm::vec3(-10.0f, -15.0f, -5.0f));
    EnemyClass solaire("3D/solaire/SOLAIRE.obj", glm::vec3(5.0f, -10.0f, 5.0f));
    EnemyClass cube("3D/cube/DADO.obj", glm::vec3(15.0f, -3.0f, 10.0f));
    
    donut.loadObj();
    sphere.loadObj();
    car.loadObj();
    chest.loadObj();
    solaire.loadObj();
    cube.loadObj();

    // -------------------------------------------------------
    // SETTING SKYBOX VERTICES AND INDICES
    /*
        These vertices and indices were taken from previous individual submissions
        and class hands-on activities
    */
    // Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, 
        1.f, -1.f, 1.f,  
        1.f, -1.f, -1.f, 
        -1.f, -1.f, -1.f,
        -1.f, 1.f, 1.f,  
        1.f, 1.f, 1.f,   
        1.f, 1.f, -1.f,  
        -1.f, 1.f, -1.f  
    };

    // Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    // -------------------------------------------------------

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // Initialize GLAD
    gladLoadGL();

    // Setting up input reading:
    // - for keyboard inputs
    glfwSetKeyCallback(window, Key_Callback);
    // - for mouse movement inputs
    glfwSetCursorPosCallback(window, Mouse_Callback);
    // Disables cursor when mouse input is used
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // -------------------------------------------------------
    // LOADING TEXTURES
    /*
        In calling the attachTexture() and attachNormalTexture() function, take note of the order:
            1st - base textures
            2nd - normal textures
    */

    playerSub.attachTexture("3D/submarine/submarine_submarine_BaseColor.png", GL_RGB);
    //playerSub.attachNormalTexture("3D/submarine/submarine_submarine_Normal.png", GL_RGB);
    playerSub.attachNormalTexture("3D/submarine/brickwall_normal.jpg", GL_RGB);

    donut.attachTexture("3D/donut/Gus.png", GL_RGBA);
    sphere.attachTexture("3D/sphere/lost_ark.jpg", GL_RGB);
    car.attachTexture("3D/car/myImage.png", GL_RGBA);
    chest.attachTexture("3D/chest/sad_gouba.png", GL_RGBA);
    solaire.attachTexture("3D/solaire/Peasant.jpg", GL_RGB);
    cube.attachTexture("3D/cube/DADO.jpg", GL_RGB);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // -------------------------------------------------------
    // CREATING OBJECT SHADERS

    ShaderClass obj_shaderProgram = ShaderClass("Shaders/objVert.vert", "Shaders/objFrag.frag");

    // -------------------------------------------------------
    // CREATING SKYBOX SHADERS

    ShaderClass skybox_shaderProgram = ShaderClass("Shaders/skybox.vert", "Shaders/skybox.frag");

    // -------------------------------------------------------
    // LOADING SKYBOX TEXTURES

    std::string facesSkybox[]{
        "Skybox/uw_rt.jpg",    // RIGHT
        "Skybox/uw_lf.jpg",    // LEFT
        "Skybox/uw_up.jpg",    // UP
        "Skybox/uw_dn.jpg",    // DOWN
        "Skybox/uw_bk.jpg",    // BACK
        "Skybox/uw_ft.jpg",    // FRONT
    };

    unsigned int skyboxTex;

    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    // Prevent pixelating
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Prevents tiling
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(
            facesSkybox[i].c_str(),
            &w,
            &h,
            &skyCChannel,
            0
        );

        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                w,
                h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
        }

        stbi_image_free(data);
    }

    stbi_set_flip_vertically_on_load(true);

    // -------------------------------------------------------
    // CREATING OBJECT VAOs and VBOs

    playerSub.createVAO_VBO();

    donut.createVAO_VBO();
    sphere.createVAO_VBO();
    car.createVAO_VBO();
    chest.createVAO_VBO();
    solaire.createVAO_VBO();
    cube.createVAO_VBO();

    // -------------------------------------------------------
    // CREATING SKYBOX VAO, VBO, and EBO

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(skyboxVertices),
        &skyboxVertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GL_FLOAT),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GL_INT) * 36,
        &skyboxIndices,
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);

    // -------------------------------------------------------
    // PROJECTION AND VIEW VARIABLES

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    // -------------------------------------------------------
    // OTHER VARIABLES

    long int    before = 0;
    long int    after;
    int         elapsed = 0;

    before = GetTickCount64();

    float theta = 0.0f;


    /// <summary>
    /// Uniform ORDER:
    /// phong,specstr
    /// ambstr,lumens,
    /// ambRGBA,lightrgba,
    /// direction
    /// </summary>
    obj_shaderProgram.use();
    GLint dirUnifs[7]{
    obj_shaderProgram.findUloc("dir_phong"),
    obj_shaderProgram.findUloc("dir_spec_str"),
    obj_shaderProgram.findUloc("dir_amb_str"),
    obj_shaderProgram.findUloc("dir_lumens"),
    obj_shaderProgram.findUloc("dir_amb_col"),
    obj_shaderProgram.findUloc("dir_color"),
    obj_shaderProgram.findUloc("dir_target")
    };

    lightBuilder* dir = new lightBuilder();
    dir->setAmbColor(new glm::vec3(0.2, 0.5, 0.9))
        ->setAmbStr(0.5)
        ->setLumens(1)
        ->setSpecPhong(10^1)
        ->setSpecStr(10)
        ->setLightVec(new glm::vec3(0,-1,0))
        ->setLightColor(new glm::vec3(0, 0, 0.2))
        ->placeUnifs(dirUnifs);

        GLint ptUnifs[7]{
    obj_shaderProgram.findUloc("pt_phong"),
    obj_shaderProgram.findUloc("pt_spec_str"),
    obj_shaderProgram.findUloc("pt_amb_str"),
    obj_shaderProgram.findUloc("pt_lumens"),
    obj_shaderProgram.findUloc("pt_amb_col"),
    obj_shaderProgram.findUloc("pt_color"),
    obj_shaderProgram.findUloc("pt_src")
        };
    lightBuilder* ptLight = new lightBuilder();

    ptLight->setAmbColor(new glm::vec3(1))
        ->setAmbStr(0.1)
        ->setLumens(1)
        ->setSpecPhong(1)
        ->setSpecStr(1)
        ->setLightVec(new glm::vec3(playerSub.getPosition()))
        ->setLightColor(new glm::vec3(0.3,0.5,0))
        ->placeUnifs(ptUnifs);
    GLint hasBmp = obj_shaderProgram.findUloc("hasBmp");
    GLint eyePos = obj_shaderProgram.findUloc("eyePos");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::vec3 lightPos = playerSub.getPosition();
        lightPos.z -= 0.5;

        // -----------------------------------------------------------------
        // CAMERA USE

        ptLight->setLightVec(&lightPos)->placeLight(ptUnifs[6]);
        if (toggle_tps && !toggle_td) {
            tps_camera.setCameraPos(tps_cameraPos + glm::vec3(0.1f, 0.0f, 0.0f));   // Slight adjustments to align with playerSub
            tps_camera.setCameraCenter(playerSub.getPosition() + glm::vec3(0.1f, 0.0f, 0.0f));
            tps_camera.setWorldUp(worldUp);
            tps_camera.setView();
            tps_camera.setProjection(60.0f, screenWidth, screenHeight);
            projectionMatrix = tps_camera.getProjectionMatrix();
            viewMatrix = tps_camera.getViewMatrix();
            glUniform3fv(eyePos, 1,glm::value_ptr(tps_camera.getCameraPos()));
            std::cout << "cam" << glGetError() << '\n';
        }
        else if (toggle_fps && !toggle_td) {
            fps_camera.setCameraPos(fps_cameraPos - glm::vec3(0.1f, 0.0f, 1.0f));   // Slight adjustments to align with playerSub
            fps_camera.setCameraCenter(playerSub.getPosition() - glm::vec3(0.0f, 0.0f, 5.0f));
            fps_camera.setWorldUp(worldUp);
            fps_camera.setView();
            fps_camera.setProjection(100.0f, screenWidth, screenHeight);
            projectionMatrix = fps_camera.getProjectionMatrix();
            viewMatrix = fps_camera.getViewMatrix();
        }
        else if (toggle_td) {
            // TODO: Implement Orthographic top-down camera
        }

        unsigned int projectionLoc = glGetUniformLocation(obj_shaderProgram.getShader(), "projection");
        unsigned int viewLoc = glGetUniformLocation(obj_shaderProgram.getShader(), "view");
        
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        
        // -----------------------------------------------------------------
        // RENDERING SKYBOX

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glUseProgram(skybox_shaderProgram.getShader());
        glBindVertexArray(skyboxVAO);

        glm::mat4 skybox_view = glm::mat4(1.0f);
        skybox_view = glm::mat4(glm::mat3(viewMatrix));

        unsigned int skybox_projectionLoc = glGetUniformLocation(skybox_shaderProgram.getShader(), "projection");
        glUniformMatrix4fv(skybox_projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int skybox_viewLoc = glGetUniformLocation(skybox_shaderProgram.getShader(), "view");
        glUniformMatrix4fv(skybox_viewLoc, 1, GL_FALSE, glm::value_ptr(skybox_view));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        // -----------------------------------------------------------------

        theta += 0.05f;

        // -----------------------------------------------------------------
        // RENDERING OBJECTS

        obj_shaderProgram.use();
        glUniform1i(hasBmp, GL_TRUE);

        std::cout << glGetError() << "true" << '\n';

        playerSub.draw(
            obj_shaderProgram.getShader(),  // Shader Program to use
            0.15f,                          // Scale
            0.0f, rot_y, 0.0f               // Rotation values for X, Y, and Z axes, respectively
        );
        
       
        glUniform1i(hasBmp, GL_FALSE);

        std::cout << "false" << glGetError() << '\n';

        donut.draw(
            obj_shaderProgram.getShader(),
            5.0f,
            theta, 0.0f, 0.0f
        );

        sphere.draw(
            obj_shaderProgram.getShader(),
            5.0f,
            0.0f, theta, 0.0f
        );

        car.draw(
            obj_shaderProgram.getShader(),
            0.015f,
            theta, theta, 0.0f
        );


        chest.draw(
            obj_shaderProgram.getShader(),
            0.01f,
            0.0f, 0.0f, theta
        );

        solaire.draw(
            obj_shaderProgram.getShader(),
            0.1f,
            0.0f, theta, theta
        );

        cube.draw(
            obj_shaderProgram.getShader(),
            0.5f,
            -theta, -theta, theta
        );
        
        // -----------------------------------------------------------------

        after = GetTickCount64();
        int temp = (after - before) / 1000;
        if (!(elapsed == temp)) {
            elapsed = temp;
            // Use cout to display playerPos on console
            cout << "Player Depth: " << playerSub.getDepth() << "\n";
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwWaitEventsTimeout(0.2);
    }

    // Cleanup
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &skyboxEBO);

    glfwTerminate();
    return 0;
}