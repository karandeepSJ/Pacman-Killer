#include "porcupine.h"
#include "main.h"
Porcupine::Porcupine(float x, float y, float width, float height, int number, color_t color) {
    this->position = glm::vec3(x, y, 0);
    speed = 0.02;
    this->width=number * width;
    this->height=height;
    GLfloat vertex_buffer_data[50];
    int idx=0;
    for(int i=0;i<number;i++)
    {
      vertex_buffer_data[idx++] = 0.0f + (i*width);
      vertex_buffer_data[idx++] = 0.0f;
      vertex_buffer_data[idx++] = 0.0f;
      vertex_buffer_data[idx++] = (i*width) + (width/2);
      vertex_buffer_data[idx++] = height;
      vertex_buffer_data[idx++] = 0.0f;
      int j=i+1;
      vertex_buffer_data[idx++] = (i+1)*width;
      vertex_buffer_data[idx++] = 0.0f;
      vertex_buffer_data[idx++] = 0.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 3*number, vertex_buffer_data, color, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
//    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
//    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Porcupine::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Porcupine::tick()
{
    this->position.x += this->speed;
}

bounding_box_rectangle_t Porcupine::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_rectangle_t bbox = { x, y, this->width, this->height };
    return bbox;
}
