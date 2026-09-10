#include "window.h"

Mat4 Window::viewportMatrix() const {
    float xMin = 0, xMax = width - 1;
    float yMin = 0, yMax = height - 1;
    
    Mat4 translation = Mat4::translation(Vec3(1, 1, 0));
    Mat4 scale = Mat4::scale(Vec3((xMax - xMin) / 2, (yMax - yMin) / 2, 1));
    Mat4 offset = Mat4::translation(Vec3(xMin, yMin, 0));
    return offset * scale * translation;
}
