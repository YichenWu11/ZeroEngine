#include "sandbox/example_layer.h"
#include "sandbox/scripts.h"

int* g_player_money = nullptr;

void ExampleLayer::onAttach() {
    LOG_TRACE("attach");

    Zero::Application::get().getResourceMngr()->add<Zero::ResourceType::TileSheet>(
        Zero::Application::get().getConfigMngr()->getAssetFolder() / "desc/tilesheet/player.json");
    Zero::Application::get().getResourceMngr()->add<Zero::ResourceType::Texture>(
        Zero::Application::get().getConfigMngr()->getAssetFolder() / "desc/tex/asoul_desc.json");
    Zero::Application::get().getResourceMngr()->add<Zero::ResourceType::Texture>(
        Zero::Application::get().getConfigMngr()->getAssetFolder() / "desc/tex/bella_desc.json");
    Zero::Application::get().getResourceMngr()->add<Zero::ResourceType::Texture>(
        Zero::Application::get().getConfigMngr()->getAssetFolder() / "desc/tex/box_desc.json");
    Zero::Application::get().getResourceMngr()->add<Zero::ResourceType::Texture>(
        Zero::Application::get().getConfigMngr()->getAssetFolder() / "desc/tex/money_desc.json");

    m_world.addScene("main").setActiveScene("main").onResize(
        Zero::Application::get().getWindow()->getWidth(),
        Zero::Application::get().getWindow()->getHeight());

    auto&& player = m_world.getActiveScene()->createEntity("player");
    player.addComponent<Zero::CameraComponent>().camera.setOrthographicSize(5.0f);
    player.addComponent<Zero::TileComponent>("player");
    {
        auto& transform = player.getComponent<Zero::TransformComponent>();
    }
    player.addComponent<Zero::NativeScriptComponent>().bind<PlayerController>();
    {
        auto& rigidbody             = player.addComponent<Zero::Rigidbody2DComponent>();
        rigidbody.type              = Zero::Rigidbody2DComponent::BodyType::Dynamic;
        rigidbody.is_fixed_rotation = true;
        rigidbody.collider.createBox(0.5f, 0.5f);
    }

    auto&& square = m_world.getActiveScene()->createEntity("square");
    square.addComponent<Zero::SpriteComponent>();
    square.addComponent<Zero::Rigidbody2DComponent>().collider.createBox(1.0f, 1.0f);
    {
        auto& transform         = square.getComponent<Zero::TransformComponent>();
        transform.translation.y = -1.5f;
        transform.rotation.z    = 0.3f;
    }

    auto&& another_square = m_world.getActiveScene()->createEntity("another_square");
    another_square.addComponent<Zero::SpriteComponent>();
    another_square.addComponent<Zero::Rigidbody2DComponent>().collider.createBox(1.0f, 1.0f);
    {
        auto& transform         = another_square.getComponent<Zero::TransformComponent>();
        transform.translation.y = -1.5f;
        transform.translation.x = -1.0f;
        transform.rotation.z    = 0.3f;
    }

    int  money_count = 0;
    auto addMoney    = [=](float x, float y) {
        auto&& money = m_world.getActiveScene()->createEntity(std::format("money_{}", money_count));
        {
            auto& sprite     = money.addComponent<Zero::SpriteComponent>();
            sprite.tex_index = Zero::Application::get().getResourceMngr()->index<Zero::ResourceType::Texture>("money");
        }
        {
            auto& rigidbody               = money.addComponent<Zero::Rigidbody2DComponent>();
            rigidbody.is_fixed_rotation   = true;
            rigidbody.collider.is_trigger = true;
            rigidbody.collider.createBox(0.1f, 0.1f);
        }
        {
            auto& transform         = money.getComponent<Zero::TransformComponent>();
            transform.translation.x = x;
            transform.translation.y = y;
            transform.scale.x       = 0.1;
            transform.scale.y       = 0.1;
        }
    };

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            addMoney(i * 0.2 - 1.0, j * 0.2);

    // Zero::SceneSerializer serializer(m_world.getActiveScene());
    // serializer.deserialize(Zero::Application::get().getConfigMngr()->getAssetFolder() / "scene/scene.json");
}

void ExampleLayer::onDetach() {
    LOG_TRACE("detach");
}

void ExampleLayer::onUpdate(Zero::TimeStep timestep) {
    m_world.onUpdateRuntime(timestep);
}

void ExampleLayer::onImGuiRender() {
    Zero::Widgets::drawButton(
        "Button",
        Vector2{200.0f, 100.0f},
        Color{0.5f, 0.5f, 0.5f, 0.8f},
        Color{0.8f, 0.8f, 0.8f, 1.0f},
        Color{0.4f, 0.4f, 0.4f, 1.0f},
        []() {
        LOG_INFO("Button Clicked");
        });

    if (g_player_money)
        Zero::Widgets::drawText(std::format("Move with W,A,S,D. Have a try!!! [ Your money: {} ]", *g_player_money));
}

void ExampleLayer::onEvent(Zero::Event& event) {
    Zero::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Zero::WindowResizeEvent>(BIND_EVENT_FN(ExampleLayer::onWindowResizeEvent));
}

bool ExampleLayer::onWindowResizeEvent(Zero::WindowResizeEvent& event) {
    m_world.onResize(event.getWidth(), event.getHeight());

    return false;
}
