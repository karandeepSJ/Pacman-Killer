#include "magnet.h"
#include "main.h"
Magnet::Magnet(float x, float y, float radius1, float radius2,color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->radius1=radius1;
    this->radius2=radius2;
    GLfloat vertex_buffer_data_circle[2000];
    int idx=0;
    for(int i=90;i<=270;i++)
    {
      vertex_buffer_data_circle[idx++] = 0.0f;
      vertex_buffer_data_circle[idx++] = 0.0f;
      vertex_buffer_data_circle[idx++] = 0.0f;
      vertex_buffer_data_circle[idx++] = radius1*cos(M_PI*i/180);
      vertex_buffer_data_circle[idx++] = radius2*sin(M_PI*i/180);
      vertex_buffer_data_circle[idx++] = 0.0f;
      int j=i+1;
      vertex_buffer_data_circle[idx++] = radius1*cos(M_PI*j/180);
      vertex_buffer_data_circle[idx++] = radius2*sin(M_PI*j/180);
      vertex_buffer_data_circle[idx++] = 0.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 540, vertex_buffer_data_circle, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
//    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
//    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
void Magnet::set_position(float x,float y)
{
    this->position.x=x;
    this->position.y=y;
}
