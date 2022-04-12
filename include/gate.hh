#ifndef GATE_H
#define GATE_H
#include "entity.hh"

class Gate : public Entity {
public:
    Gate(GLuint texture, const glm::vec3& position, Game* game);
    inline virtual void Update(float dt) override {};
    virtual void Explode() override;
};
#endif // GATE_H