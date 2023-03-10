#pragma once

namespace Zero {
    class Physics2DManager;
    class ContactListener2D;
    class Collider2D {
    public:
        friend class Physics2DManager;
        friend class ContactListener2D;
        Collider2D();
        void createBox(float w, float h);

        float getBoxWidth();
        float getBoxHeight();

        EntityIdType getEntity();
        bool         hasCollisionEnterEvent();
        EntityIdType getCollisionEnterEvent();
        bool         hasCollisionExitEvent();
        EntityIdType getCollisionExitEvent();

        bool stayWith(EntityIdType e);
        bool is_one_sided_collision{false};
        bool is_trigger{false};

    private:
        Scope<b2Shape>         m_b2shape;
        EntityIdType           m_entity_id{0};
        std::set<EntityIdType> m_collision_enter_event;
        std::set<EntityIdType> m_collision_exit_event;
        std::set<EntityIdType> m_collision_stay_event;
        union {
            struct {
                float w, h;
            } rect;
            struct {
                float r;
            } circle;
        } m_size{};
    };

} // namespace Zero