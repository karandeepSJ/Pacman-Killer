#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct color_t {
    int r;
    int g;
    int b;
};

// nonedit.cpp
GLFWwindow *initGLFW(int width, int height);
GLuint     LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat *color_buffer_data, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const color_t color, GLenum fill_mode = GL_FILL);
void       draw3DObject(struct VAO *vao);

// input.cpp
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void keyboardChar(GLFWwindow *window, unsigned int key);
void mouseButton(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// other_handlers.cpp
void error_callback(int error, const char *description);
void quit(GLFWwindow *window);
void reshapeWindow(GLFWwindow *window, int width, int height);

// Types
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int    NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint    MatrixID;
};

extern GLMatrices Matrices;

// ---- Logic ----

enum direction_t { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };

struct bounding_box_rectangle_t {
    float x;
    float y;
    float width;
    float height;
};

struct bounding_box_circle_t {
    float x;
    float y;
    float r;
};
struct bounding_box_plank_t {
    float x;
    float y;
    float m;
    float height;
    float angle;
    float width;
};

void inc_score(color_t);
bool detect_collision_ball_ball(bounding_box_circle_t a, bounding_box_circle_t b);
bool detect_collision_ball_tramp(bounding_box_circle_t p, bounding_box_circle_t t);
bool detect_collision_ball_floor(bounding_box_circle_t a, bounding_box_rectangle_t b);
bool detect_collision_ball_pond(bounding_box_circle_t a);
bool detect_collision_ball_plank(bounding_box_circle_t a, bounding_box_plank_t c);
bool detect_collision_ball_porc(bounding_box_circle_t a, bounding_box_rectangle_t b);
extern float screen_zoom, screen_center_x, screen_center_y;
extern int auto_pan,pause;
void reset_screen();

// ---- Colors ----
extern const color_t COLOR_RED;
extern const color_t COLOR_GREEN;
extern const color_t COLOR_BLACK;
extern const color_t COLOR_BACKGROUND;
extern const color_t COLOR_BROWN;
extern const color_t COLOR_BLUE;
extern const color_t COLOR_YELLOW;
extern const color_t COLOR_PURPLE;
extern const color_t COLOR_LIGHT_BLUE;
extern const color_t COLOR_WHITE;
extern const color_t COLOR_SILVER;
#endif
