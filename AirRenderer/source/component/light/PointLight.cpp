#include "include/component/light/PointLight.h"
#include "include/GameObject.h"

glm::vec3 PointLight::IncidentLight(glm::vec3& position, glm::mat4& viewMatrix)
{
    glm::vec4 lp = viewMatrix * gameObject->transform.worldMatrix * glm::vec4(0, 0, 0, 1);
    return position - glm::vec3(lp);
}

glm::vec3 PointLight::IncidentLight(glm::vec3& position)
{
    glm::vec4 lp = gameObject->transform.worldMatrix * glm::vec4(0, 0, 0, 1);
    return position - glm::vec3(lp);
}

Color PointLight::ColorIntensity(glm::vec3& incidentLight)
{
    float dis = glm::distance(incidentLight, glm::vec3(0, 0, 0));
    float max = std::max(emitterRadius, dis);
    float window = 1.0 - std::powf(dis / lightRadius, 4);
    window = window < 0 ? 0 : std::powf(window, 2);
    return baseColor * window * std::powf(5 / max, 2) * intensity;
}

Color PointLight::AmbintColorIntensity()
{
    return Color(0, 0, 0, 1);
}

PointLight::PointLight():Light("PointLight")
{
    this->emitterRadius = 0.01;
    this->lightRadius = 10;
    this->intensity = 2;
}
