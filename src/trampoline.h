#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H
#include "main.h"
class Trampoline {
public:
    Trampoline() {}
    Trampoline(float , float , float, color_t);
    void draw(glm::mat4 VP);
    glm::vec3 position;
    float radius;
     bounding_box_circle_t bounding_box();
private:
    VAO *object;
};

#endif // TRAMPOLINE_H

