#ifndef MAGNET_H
#define MAGNET_H
#include "main.h"
class Magnet {
public:
    Magnet() {}
    Magnet(float , float , float, float, color_t);
    void draw(glm::mat4 VP);
    glm::vec3 position;
    float radius1,radius2;
    void set_position(float, float);
private:
    VAO *object;
};

#endif // MAGNET_H

