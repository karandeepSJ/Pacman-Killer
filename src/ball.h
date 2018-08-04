#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float , float , float , float, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void jump(float,float,int);
    float speed;
    float v;
    float u;
    float rad;
    float init_pos;
    color_t color;
    bounding_box_circle_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
