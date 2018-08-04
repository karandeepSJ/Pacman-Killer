#include "floor.h"
#include "main.h"
Floor::Floor(float x, float y, float height, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    GLfloat vertex_buffer_data[] = {
        -40, -height/2 ,0,
        40, -height/2, 0,
        -40, height/2, 0,
        40, -height/2, 0,
        -40, height/2, 0,
        40, height/2, 0
    };
    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Floor::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= translate;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bounding_box_rectangle_t Floor::bounding_box(float height) {
    float x=this->position.x, y=this->position.y;
    bounding_box_rectangle_t bbox = { x, y, 8, height };
    return bbox;
}
