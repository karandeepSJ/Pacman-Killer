#include "pond.h"
#include "main.h"
Pond::Pond(float x, float y, float radius, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->radius=radius;
    GLfloat vertex_buffer_data_circle[2000];
    int idx=0;
    for(int i=180;i<=360;i++)
    {
      vertex_buffer_data_circle[idx++] = x;
      vertex_buffer_data_circle[idx++] = y;
      vertex_buffer_data_circle[idx++] = 0.0f;
      vertex_buffer_data_circle[idx++] = x+radius*cos(M_PI*i/180);
      vertex_buffer_data_circle[idx++] = y+radius*sin(M_PI*i/180);
      vertex_buffer_data_circle[idx++] = 0.0f;
      int j=i+1;
      vertex_buffer_data_circle[idx++] = x+radius*cos(M_PI*j/180);
      vertex_buffer_data_circle[idx++] = y+radius*sin(M_PI*j/180);
      vertex_buffer_data_circle[idx++] = 0.0f;
    }
    this->object = create3DObject(GL_TRIANGLES, 540, vertex_buffer_data_circle, color, GL_FILL);
}

void Pond::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
//    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
//    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
//    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
//    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bounding_box_circle_t Pond::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_circle_t bbox = { x, y, this->radius };
    return bbox;
}
