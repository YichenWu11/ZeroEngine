#include "runtime/function/physics/collider2d.h"

namespace Zero {

    Collider2D::Collider2D() {
    }

    void Collider2D::createBox(float w, float h) {
        Scope<b2PolygonShape> b2shape = CreateScope<b2PolygonShape>();
        b2shape->SetAsBox(w / 2, h / 2);
        m_size.rect.w = w;
        m_size.rect.h = h;
        m_b2shape     = std::move(b2shape);
    }

    float Collider2D::getBoxWidth() {
        return m_size.rect.w;
    }

    float Collider2D::getBoxHeight() {
        return m_size.rect.h;
    }

    EntityIdType Collider2D::getEntity() {
        return m_entity_id;
    }

    bool Collider2D::hasCollisionEnterEvent() {
        return !m_collision_enter_event.empty();
    }

    EntityIdType Collider2D::getCollisionEnterEvent() {
        EntityIdType e = *m_collision_enter_event.begin();
        m_collision_enter_event.erase(m_collision_enter_event.begin());
        m_collision_stay_event.insert(e);
        return e;
    }

    bool Collider2D::hasCollisionExitEvent() {
        return !m_collision_exit_event.empty();
    }

    EntityIdType Collider2D::getCollisionExitEvent() {
        EntityIdType e = *m_collision_exit_event.begin();
        m_collision_exit_event.erase(m_collision_exit_event.begin());
        auto e_in_stay = m_collision_stay_event.find(e);
        if (e_in_stay != m_collision_stay_event.end()) {
            m_collision_stay_event.erase(e_in_stay);
        }
        return e;
    }

    bool Collider2D::stayWith(EntityIdType e) {
        return (m_collision_stay_event.find(e) != m_collision_stay_event.end()) || (m_collision_enter_event.find(e) != m_collision_enter_event.end());
    }

} // namespace Zero
