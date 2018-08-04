#ifndef PLANK_H
#define PLANK_H
#include "main.h"
class Plank {
public:
    Plank() {}
    Plank(float , float, float , float , float, color_t);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float speed;
    float width;
    float angle;
    float height;
    bounding_box_plank_t bounding_box();
private:
    VAO *object;
};

#endif // PLANK_H

