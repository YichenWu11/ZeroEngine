#include "sandbox/example_layer.h"
#include "sandbox/scripts.h"

void ExampleLayer::onAttach() {
    GET_TEXTURE_POOL().registerTex(
        GET_CONFIG_MNGR().getAssetFolder() / "texture/common/bella.png");
    GET_TEXTURE_POOL().registerTex(
        GET_CONFIG_MNGR().getAssetFolder() / "texture/common/asoul.png");

    m_world.addScene("main").setActiveScene("main");
    m_world.onResize(Zero::Application::get().getWindow().getWidth(),
                     Zero::Application::get().getWindow().getHeight());

    auto&& player = m_world.getActiveScene()->createEntity("player");
    player.addComponent<Zero::CameraComponent>().camera.setOrthographicSize(5.0f);
    player.addComponent<Zero::NativeScriptComponent>().bind<CameraController>();

    auto& player_sprite     = player.addComponent<Zero::SpriteComponent>();
    player_sprite.tex_index = GET_TEXTURE_POOL().getTexIndexFromName("bella");

    auto&& square = m_world.getActiveScene()->createEntity("square");
    square.addComponent<Zero::SpriteComponent>();
}

void ExampleLayer::onDetach() {
}

void ExampleLayer::onUpdate(Zero::TimeStep timestep) {
    m_world.onUpdateRuntime(timestep);
}

void ExampleLayer::onImGuiRender() {
    ImGuiWindowFlags window_flag =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground;
}

void ExampleLayer::onEvent(Zero::Event& event) {
    Zero::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Zero::WindowResizeEvent>(ZE_BIND_EVENT_FN(ExampleLayer::onWindowResizeEvent));
}

bool ExampleLayer::onWindowResizeEvent(Zero::WindowResizeEvent& event) {
    m_world.onResize(event.getWidth(), event.getHeight());

    return false;
}
