#ifndef POND_H
#define POND_H
#include "main.h"
class Pond {
public:
    Pond() {}
    Pond(float , float , float, color_t);
    void draw(glm::mat4 VP);
    glm::vec3 position;
    float radius;
     bounding_box_circle_t bounding_box();
private:
    VAO *object;
};

#endif // POND_H

