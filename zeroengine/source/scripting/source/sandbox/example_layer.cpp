#include "sandbox/example_layer.h"
#include "sandbox/scripts.h"

void ExampleLayer::onAttach() {
    LOG_TRACE("attach");

    Zero::Application::get().getResourceMngr()->add<Zero::ResourceType::TileSheet>(
        Zero::Application::get().getConfigMngr()->getAssetFolder() / "desc/tilesheet/player.json");
    Zero::Application::get().getResourceMngr()->add<Zero::ResourceType::Texture>(
        Zero::Application::get().getConfigMngr()->getAssetFolder() / "desc/tex/box_desc.json");

    m_world.addScene("main").setActiveScene("main").onResize(
        Zero::Application::get().getWindow()->getWidth(),
        Zero::Application::get().getWindow()->getHeight());

    m_world.getActiveScene()->setGravity({0.0f, 0.0f});

    auto&& camera = m_world.getActiveScene()->createEntity("camera");
    camera.addComponent<Zero::CameraComponent>().camera.setOrthographicSize(3.0f);

    auto&& player = m_world.getActiveScene()->createEntity("player");
    player.addComponent<Zero::TileComponent>("player");
    player.addComponent<Zero::LuaScriptComponent>(
        Zero::Application::get().getConfigMngr()->getScriptFolder() / "player_controller.lua");
    auto& player_rbc =
        player.addComponent<Zero::Rigidbody2DComponent>();
    player_rbc.type              = Zero::Rigidbody2DComponent::BodyType::Dynamic;
    player_rbc.is_fixed_rotation = true;
    player_rbc.show_box          = false;
    player_rbc.collider.createBox(0.35f, 0.4f);

    // Zero::SceneSerializer serializer(m_world.getActiveScene());
    // serializer.deserialize(Zero::Application::get().getConfigMngr()->getAssetFolder() / "scene/scene.json");

    m_world.onRuntimeStart();
}

void ExampleLayer::onDetach() {
    LOG_TRACE("detach");
}

void ExampleLayer::onUpdate(Zero::TimeStep timestep) {
    m_world.onUpdateRuntime(timestep);
}

void ExampleLayer::onImGuiRender() {
    Zero::Widgets::drawButton(
        "RefreshAll",
        Vector2{200.0f, 100.0f},
        Color{0.5f, 0.5f, 0.5f, 0.8f},
        Color{0.8f, 0.8f, 0.8f, 1.0f},
        Color{0.4f, 0.4f, 0.4f, 1.0f},
        []() {
        Zero::Application::get().getLuaInterpreter()->refreshAll();
        });
}

void ExampleLayer::onEvent(Zero::Event& event) {
    Zero::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Zero::WindowResizeEvent>(BIND_EVENT_FN(ExampleLayer::onWindowResizeEvent));
}

bool ExampleLayer::onWindowResizeEvent(Zero::WindowResizeEvent& event) {
    m_world.onResize(event.getWidth(), event.getHeight());
    return false;
}
