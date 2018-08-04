#include "main.h"

#ifndef PORCUPINE_H
#define PORCUPINE_H


class Porcupine {
public:
    Porcupine() {}
    Porcupine(float, float , float , float , int, color_t color);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float speed;
    float width;
    float height;
    bounding_box_rectangle_t bounding_box();
private:
    VAO *object;
};

#endif // PORCUPINE_H
