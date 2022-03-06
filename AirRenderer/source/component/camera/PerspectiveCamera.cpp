#include <include/component/camera/PerspectiveCamera.h>
#include <algorithm>
PerspectiveCamera::PerspectiveCamera() : Camera("Perspective")
{
    this->nearFlat = 2;
    this->farFlat = 100;
    this->aspectRatio = 16.0 / 9.0;
    this->fovAngle = 60;
    this->needPerspectiveCorrection = true;
}

glm::mat4 PerspectiveCamera::ProjectionMatrix()
{	
    double pi = std::acos(-1.0);
    double halfFov = fovAngle * pi / 360.0;
    double cot = 1.0 / std::tan(halfFov);

        return glm::mat4(
        cot / aspectRatio, 0, 0, 0,
        0, cot, 0, 0,
        0, 0, -(farFlat + nearFlat) / farFlat, -1,
        0, 0, -nearFlat, 0
    );
}

void PerspectiveCamera::ClipPlanes(glm::vec4* planes)
{
    double pi = std::acos(-1.0);
    double halfFov = fovAngle * pi / 360.0;
    planes[0] = glm::vec4(-nearFlat, 0, -nearFlat * std::tan(halfFov), 0);
    planes[1] = glm::vec4(nearFlat, 0, -nearFlat * std::tan(halfFov), 0);
    planes[2] = glm::vec4(-nearFlat, 0, -aspectRatio * nearFlat * std::tan(halfFov), 0);
    planes[3] = glm::vec4(nearFlat, 0, -aspectRatio * nearFlat * std::tan(halfFov), 0);
    planes[4] = glm::vec4(0, 0, -1, -nearFlat);
    planes[5] = glm::vec4(0, 0, 1, farFlat);
}
