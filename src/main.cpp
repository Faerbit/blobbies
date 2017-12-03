#include "globals.h"

#include <glm/common.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "Blob.h"
#include "AiBlob.h"


// constants
static const std::chrono::duration<double, std::milli> MS_PER_FRAME {1.0/60.0};
static const int CIRCLE_RESOLUTION = 30;
static const int WIDTH = 1360;
static const int HEIGHT = 768;
static const std::string TITLE = "Faerbit's LD40 entry";

static const float SCALE = 10.0f;

static const char* vertexShaderSource =
"uniform mat4 MP;\n"
"attribute vec2 vPos;\n"
"void main()\n"
"{\n"
"   gl_Position = MP * vec4(vPos, 0.0, 1.0);\n"
"}\n";

static const char* fragmentShaderSource =
"uniform vec3 color;"
"void main()\n"
"{\n"
"   gl_FragColor = vec4(color, 1.0);\n"
"}\n";

void init() {
    blobs.clear();
    blobs.emplace_back(std::make_unique<PlayerBlob>());
    enemyCount = 0;
}

void update() {
    gameWidth = windowWidth/SCALE;
    gameHeight = windowHeight/SCALE;
    while(enemyCount < 10) {
        AiBlob::spawn();
        ++enemyCount;
    }
    auto blob = blobs.begin();
    while (blob != blobs.end()) {
        bool valid = (*blob)->update();
        if (valid) {
            ++blob;
        }
        else {
            blobs.erase(blob++);
            --enemyCount;
        }
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 perspective{
            glm::ortho(-gameWidth, gameWidth, -gameHeight, gameHeight)};

    for (auto& blob: blobs) {
        glm::mat4 model {
                glm::translate(glm::vec3(blob->getPos(), 0.0f)) *
                        glm::scale(glm::vec3(glm::vec2(blob->getSize()), 0.0f))};
        glm::mat4 mp {perspective * model};
        glUniformMatrix4fv(mpLocation, 1, GL_FALSE, glm::value_ptr(mp));
        glUniform3fv(colorLocation, 1, glm::value_ptr(blob->getColor()));
        glDrawArrays(GL_TRIANGLES, 0, circle.size());
    }
}

void error_callback(int error, const char* desc) {
    std::cerr << "Error :" << error << " " << desc << "\n";
}

void checkGLErrors() {
    GLenum error = glGetError();
    if (error) {
       switch(error) {
           case GL_INVALID_ENUM:
               std::cerr << "GL_INVALID_ENUM\n";
               break;
           case GL_INVALID_VALUE:
               std::cerr << "GL_INVALID_VALUE\n";
               break;
           case GL_INVALID_OPERATION:
               std::cerr << "GL_INVALID_OPERATION\n";
               break;
           case GL_INVALID_FRAMEBUFFER_OPERATION:
               std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION\n";
               break;
           case GL_OUT_OF_MEMORY:
               std::cerr << "GL_OUT_OF_MEMORY\n";
               break;
           default:
               std::cerr << "Unknown error\n";
       }
    }
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
       keyState.up = true ;
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        keyState.up = false;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        keyState.left = true ;
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        keyState.left = false;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        keyState.down = true ;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        keyState.down = false;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        keyState.right = true ;
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        keyState.right = false;
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        keyState.up = true ;
    }
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
        keyState.up = false;
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        keyState.left = true ;
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
        keyState.left = false;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        keyState.down = true ;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
        keyState.down = false;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        keyState.right = true ;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
        keyState.right = false;
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        init();
    }
}

void initCircle(int resolution) {
    circle.resize(resolution * 3);
    float angle = 0.0f;
    float nextAngle = 0.0f;
    for(int i = 0; i<resolution; i++) {
        angle = ((2.0f * PI)/resolution) * i;
        nextAngle = ((2.0f * PI)/resolution) * (i+1);
        circle.at((i * 3) + 0) = glm::vec2(0.0f, 0.0f);
        circle.at((i * 3) + 1) = glm::rotate(glm::vec2(1.0f, 0.0f), angle);
        circle.at((i * 3) + 2) = glm::rotate(glm::vec2(1.0f, 0.0f), nextAngle);
    }
}

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW!\n";
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(error_callback);

    //glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(),
            nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create window!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    initCircle(CIRCLE_RESOLUTION);

    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    glfwMakeContextCurrent(window);

    checkGLErrors();

    GLuint vertexBuffer = 0;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle)*circle.size(), &circle[0], GL_STATIC_DRAW);
    checkGLErrors();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkGLErrors();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkGLErrors();

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    checkGLErrors();

    mpLocation = glGetUniformLocation(program, "MP");
    colorLocation = glGetUniformLocation(program, "color");
    vPosLocation = glGetAttribLocation(program, "vPos");
    glEnableVertexAttribArray(vPosLocation);
    glVertexAttribPointer(vPosLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, nullptr);
    checkGLErrors();

    glUseProgram(program);

    init();

    while(!glfwWindowShouldClose(window)) {
        auto start = std::chrono::high_resolution_clock::now();
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

        if((*blobs.front()).getSize() * 0.9 < gameHeight) {
            update();
        }
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
        auto end = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(MS_PER_FRAME - (end - start));
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
