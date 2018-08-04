#include "main.h"

#ifndef FLOOR_H
#define FLOOR_H

class Floor {
public:
    Floor() {};
    Floor (float, float, float, color_t );
    glm::vec3 position;
    void draw(glm::mat4 VP);
    bounding_box_rectangle_t bounding_box(float height);
private:
    VAO *object;
};

#endif
