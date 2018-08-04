#include "ball.h"
#include "main.h"
Ball::Ball(float x, float y, float r, float hor_speed, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->color = color;
    this->rotation = 0;
    speed = hor_speed;
    v = 0;
    u = 0;
    rad = r;
    GLfloat vertex_buffer_data[4000];
    int idx=0;
    for(int i=0;i<=360;i++)
    {
      vertex_buffer_data[idx++] = 0.0f;
      vertex_buffer_data[idx++] = 0.0f;
      vertex_buffer_data[idx++] = 0.0f;
      vertex_buffer_data[idx++] = r*cos(M_PI*i/180);
      vertex_buffer_data[idx++] = r*sin(M_PI*i/180);
      vertex_buffer_data[idx++] = 0.0f;
      int j=i+1;
      vertex_buffer_data[idx++] = r*cos(M_PI*j/180);;
      vertex_buffer_data[idx++] = r*sin(M_PI*j/180);;
      vertex_buffer_data[idx++] = 0.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 1080, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick()
{
    this->position.x += speed;
}

void Ball::jump(float vel,float pos,int f) {
    if(this->u==0 || f==1)
    {
        this->u = vel;
        this->init_pos=pos;
        this->v = vel;
    }
}

bounding_box_circle_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y, r = this->rad;
    bounding_box_circle_t bbox = { x, y, r };
    return bbox;
}
