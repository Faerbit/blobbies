#include "globals.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif


#include <glm/common.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include "Blob.h"
#include "AiBlob.h"
#include "PlayerBlob.h"
#include "font.h"


// constants
static const std::chrono::duration<double, std::milli> MS_PER_FRAME {1.0/60.0};
static const int CIRCLE_RESOLUTION = 50;
static const int WIDTH = 1360;
static const int HEIGHT = 768;
static const std::string TITLE = "Faerbit's LD40 entry: bloBBies";
static const std::string FONT = "NotoSans-Black.ttf";

static const float SCALE = 10.0f;

static const std::string vertexDefaultShaderSource =
#include "default.vert"
;

static const std::string fragmentDefaultShaderSource =
#include "default.frag"
;

static const std::string vertexTextShaderSource =
#include "text.vert"
;

static const std::string fragmentTextShaderSource =
#include "text.frag"
;

void checkGLErrors(const std::string& desc = "") {
    GLenum error = glGetError();
    if (error) {
        std::cerr << "GL error: ";
        switch(error) {
            case GL_INVALID_ENUM:
                std::cerr << "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                std::cerr << "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                std::cerr << "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                std::cerr << "GL_OUT_OF_MEMORY";
                break;
            default:
                std::cerr << "Unknown error";
        }
        if (desc != "") {
            std::cerr << " at "  << desc;
        }
        std::cerr << "\n";
    }
}

void checkShader(GLuint shader) {
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        //std::string log(errorLog.begin, errorLog.end()),
        std::cerr << "Error during shader compilation:\n" << &errorLog[0] << "\n";

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shader); // Don't leak the shader.
        return;
    }
}

void init() {
    gameIntro = true;
    gameRunning = true;
    gameStart = std::chrono::high_resolution_clock::now();
    blobs.clear();
    blobs.emplace_back(std::make_unique<PlayerBlob>());
    enemyCount = 0;
}

void update() {
    gameWidth = windowWidth/SCALE;
    gameHeight = windowHeight/SCALE;
    while(enemyCount < MAX_ENEMY_COUNT) {
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
    glBindBuffer(GL_ARRAY_BUFFER, circleVbo);
    glEnableVertexAttribArray(vPosLocation);
    glVertexAttribPointer(vPosLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, nullptr);
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


void renderText(const std::string& text, int line = 0) {
    float sx = 2.0 / windowWidth;
    float sy = 2.0 / windowHeight;
    float x = (-(text.length()/53.0)) + 8 * sx;
    float y = -line * 60 * sy;
    glBindBuffer(GL_ARRAY_BUFFER, textVbo);
    glEnableVertexAttribArray(textCoordLocation);
    glVertexAttribPointer(textCoordLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    FT_GlyphSlot g = ftFace->glyph;
    for(const char& c : text) {
        if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load char " << c << "\n";
            continue;
        }

        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_ALPHA,
                g->bitmap.width,
                g->bitmap.rows,
                0,
                GL_ALPHA,
                GL_UNSIGNED_BYTE,
                g->bitmap.buffer
        );

        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;

        GLfloat box[4][4] = {
            {x2,     -y2    , 0, 0},
            {x2 + w, -y2    , 1, 0},
            {x2,     -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        x += (g->advance.x/64) * sx;
        y += (g->advance.y/64) * sy;
    }
}

void error_callback(int error, const char* desc) {
    std::cerr << "Error :" << error << " " << desc << "\n";
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
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        gameIntro = false;
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

GLuint compileShader(const GLenum shaderType, const std::string& src) {
    GLuint handle = glCreateShader(shaderType);
#ifdef __EMSCRIPTEN__
    const std::string shaderPrefix("precision mediump float;\n");
#else
    const std::string shaderPrefix("");
#endif
    const std::string shaderStr = shaderPrefix + src;
    const char* shaderSrc = shaderStr.c_str();
    glShaderSource(handle, 1, &shaderSrc, nullptr);
    glCompileShader(handle);
    checkShader(handle);
    checkGLErrors("after compiling shader" + std::to_string(shaderType));
    return handle;
}

void gameLoop() {
#ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size("#canvas", &windowWidth, &windowHeight);
#else
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
#endif

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(defaultProgram);
    render();
    glUseProgram(textProgram);
    if (gameIntro) {
        renderText("bloBBies", -1);
        renderText("Evade the orange blobbies!", 1);
        renderText("Control with WASD or the arrow keys", 2);
        renderText("Press Space to start, Esc to exit", 3);
    }
    else {
        if ((*blobs.front()).getSize() * MAX_SIZE < gameHeight) {
            update();
        } else {
            if (gameRunning) {
                gameRunning = false;
                gameLength = std::chrono::high_resolution_clock::now() - gameStart;
            }
            std::ostringstream msg;
            msg << "You lasted for: ";
            int secs = std::chrono::duration_cast<std::chrono::seconds>(gameLength).count();
            int mins = secs / 60;
            msg << mins << ":" << std::setfill('0') << std::setw(2) << secs % 60;
            renderText(msg.str());
            renderText("Press \"R\" to restart", 1);
        }
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
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

#ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size("#canvas", &windowWidth, &windowHeight);
    window = glfwCreateWindow(windowWidth, windowHeight, TITLE.c_str(),
                              nullptr, nullptr);
#else
    window = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(),
            nullptr, nullptr);
#endif

    if (!window) {
        std::cerr << "Failed to create window!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }


    if (FT_Init_FreeType (&ftLibrary)) {
        std::cerr << "Cannot initialize FreeType\n";
        return EXIT_FAILURE;
    }

    if (FT_New_Memory_Face(ftLibrary, NotoSans_Black_ttf, NotoSans_Black_ttf_len, 0, &ftFace)) {
        std::cerr << "Error loading font\n";
        return EXIT_FAILURE;
    }

    FT_Set_Pixel_Sizes(ftFace, 0, 48);

#ifdef __EMSCRIPTEN__
    int result = emscripten_webgl_enable_extension(emscripten_webgl_get_current_context(), "OES_texture_float");
    if (result < 0) {
        std::cerr << "Could not load extension \"OES_texture_float\"\n";
        emscripten_run_script("alert('Could not load extension \"OES_texture_float\". Game will not work.')");
    }
    result = emscripten_webgl_enable_extension(emscripten_webgl_get_current_context(), "OES_texture_float_linear");
    if (result < 0) {
        std::cerr << "Could not load extension \"OES_texture_float_linear\"\n";
        emscripten_run_script("alert('Could not load extension \"OES_texture_float_linear\". Game will not work.')");
    }
#endif

    initCircle(CIRCLE_RESOLUTION);

    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    glfwMakeContextCurrent(window);

    checkGLErrors();

    glGenBuffers(1, &circleVbo);
    glBindBuffer(GL_ARRAY_BUFFER, circleVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle)*circle.size(), &circle[0], GL_STATIC_DRAW);
    checkGLErrors();

    GLuint vertexDefaultShader = compileShader(GL_VERTEX_SHADER, vertexDefaultShaderSource);
    GLuint fragmentDefaultShader = compileShader(GL_FRAGMENT_SHADER, fragmentDefaultShaderSource);

    defaultProgram = glCreateProgram();
    glAttachShader(defaultProgram, vertexDefaultShader);
    glAttachShader(defaultProgram, fragmentDefaultShader);
    glLinkProgram(defaultProgram);
    checkGLErrors();

    mpLocation = glGetUniformLocation(defaultProgram, "MP");
    colorLocation = glGetUniformLocation(defaultProgram, "color");
    vPosLocation = glGetAttribLocation(defaultProgram, "vPos");
    checkGLErrors();

    GLuint vertexTextShader = compileShader(GL_VERTEX_SHADER, vertexTextShaderSource);
    GLuint fragmentTextShader = compileShader(GL_FRAGMENT_SHADER, fragmentTextShaderSource);

    textProgram = glCreateProgram();
    glAttachShader(textProgram, vertexTextShader);
    glAttachShader(textProgram, fragmentTextShader);
    glLinkProgram(textProgram);
    checkGLErrors("text program");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    checkGLErrors("blend");

    GLuint textTex;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textTex);
    checkGLErrors("texture0");
    glBindTexture(GL_TEXTURE_2D, textTex);
    checkGLErrors("texture1");
    GLint textTexUniformLoc = glGetUniformLocation(textProgram, "texture");
    checkGLErrors("textureLoc");
    glUseProgram(textProgram);
    glUniform1i(textTexUniformLoc, 0);
    checkGLErrors("texture3");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkGLErrors("texture2");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    checkGLErrors("unpack");

    glGenBuffers(1, &textVbo);
    textCoordLocation = glGetAttribLocation(textProgram, "coord");

    init();

    // === REALLY dirty last minute hacks ===
#ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size("#canvas", &windowWidth, &windowHeight);
#else
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
#endif
    glUseProgram(defaultProgram);
    render();
    update();
    init();

    // === please NEVER do something like this ===

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(gameLoop, 60, false);
#else
    while(!glfwWindowShouldClose(window)) {
        auto start = std::chrono::high_resolution_clock::now();
        gameLoop();
        auto end = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(MS_PER_FRAME - (end - start));
    }

    glfwTerminate();
    return EXIT_SUCCESS;
#endif
}
