#ifndef APP1_LIGHT_HPP
#define APP1_LIGHT_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "types.hpp"
#include "mesh.hpp"

struct PointLight {
    glm::vec3 position;

    glm::vec3 ambient = glm::vec3(1.0f)/5.0f;
    glm::vec3 diffuse = glm::vec3(1.0f)/2.0f;
    glm::vec3 specular = glm::vec3(1.0f);

    glm::vec3 attenuation = glm::vec3(0.5f, 0.05f, 0.02f);

    explicit PointLight(glm::vec3 _pos) : position(_pos) {}

    void loadIntoUniform(ShaderProgram &prog, const std::string &prefix) const {
        prog.loadUniform(prefix + ".position", position);
        prog.loadUniform(prefix + ".ambient", ambient);
        prog.loadUniform(prefix + ".diffuse", diffuse);
        prog.loadUniform(prefix + ".specular", specular);
        prog.loadUniform(prefix + ".attenuation", attenuation);
    }

    glm::vec3 getModelColor() const {
        float mx = glm::max(diffuse.x, glm::max(diffuse.y, diffuse.z));
        return diffuse / mx;
    }
};

struct Lights {
    std::vector<PointLight> pointLights;
    Mesh model;
    float scale;

    void loadPointIntoUniform(ShaderProgram &prog) const {
        for (unsigned i = 0; i < pointLights.size(); i++)
            pointLights[i].loadIntoUniform(prog, "pointLights[" + std::to_string(i) + "]");
    }

    void drawPointLights(const ShaderProgram &prog, const glm::mat4 &view, const glm::mat4 &proj) const {
        for (const PointLight &light: pointLights) {
            glm::mat4 mm = glm::scale(glm::translate(glm::mat4(1.0f), light.position), glm::vec3(scale));
            glm::mat4 mvp = proj * view * mm;
            prog.loadUniformInt("numPointLights", pointLights.size());
            model.drawUnlit(prog, mvp, light.getModelColor());
        }
    }

    explicit Lights(Mesh &&_model, float _scale) : model(std::move(_model)), scale(_scale) {}
};

#endif //APP1_LIGHT_HPP
