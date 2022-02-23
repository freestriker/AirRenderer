#include "include/component/light/DirectionalLight.h"
#include "include/core_object/GameObject.h"

glm::vec3 DirectionalLight::IncidentLight(glm::vec3& position, glm::mat4& viewMatrix)
{
    glm::vec4 ld = viewMatrix * gameObject->transform.worldMatrix * glm::vec4(0, 0, -1, 0);
    return glm::normalize(glm::vec3(ld));
}
glm::vec3 DirectionalLight::IncidentLight(glm::vec3& position)
{
    glm::vec4 ld = gameObject->transform.worldMatrix * glm::vec4(0, 0, -1, 0);
    return glm::normalize(glm::vec3(ld));
}
Color DirectionalLight::ColorIntensity(glm::vec3& incidentLight)
{
    return baseColor * intensity;
}
DirectionalLight::DirectionalLight():Light("DirectionalLight")
{
    intensity = 0.05;
}

Light* DirectionalLight::Clone()
{
    Light* light = new DirectionalLight(*this);
    light->gameObject = nullptr;
    return;
}
