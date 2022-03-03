#include <include/component/camera/OrthographicCamera.h>

OrthographicCamera::OrthographicCamera(): Camera("Orthographic")
{
    this->nearFlat = 2;
    this->farFlat = 100;
    this->aspectRatio = 16.0 / 9.0;
    this->size = 9;
}
glm::mat4 OrthographicCamera::ProjectionMatrix()
{
    return glm::mat4(
        1.0 / (aspectRatio * size), 0, 0, 0,
        0, 1.0 / size, 0, 0,
        0, 0, -1.0 / (farFlat/* - nearFlat*/), 0,
        0, 0, -nearFlat / (farFlat/* - nearFlat*/), 1
    );
}

void OrthographicCamera::ClipPlanes(glm::vec4* planes)
{
    planes[0] = glm::vec4(-1, 0, 0, aspectRatio * size);
    planes[1] = glm::vec4(1, 0, 0, aspectRatio * size);
    planes[2] = glm::vec4(0, -1, 0, size);
    planes[3] = glm::vec4(0, 1, 0, size);
    planes[4] = glm::vec4(0, 0, -1, -nearFlat);
    planes[5] = glm::vec4(0, 0, 1, farFlat);
}
