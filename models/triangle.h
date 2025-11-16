/**
 * @file triangle.h
 *
 * @brief Triangle model 
 *
 * @details  Command for rendering
 *           glDrawArrays(GL_TRIANGLES, 0, 6);
 *
 * @author Martin Nemec
 *
 * @year 2023
 **/


const float triangle[54] = {
    0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f
};