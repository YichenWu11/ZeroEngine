#include "sandbox/example_layer.h"
#include "sandbox/scripts.h"

void ExampleLayer::onAttach() {
    m_world.addScene("main").setActiveScene("main");
    m_world.onResize(Zero::Application::get().getWindow()->getWidth(),
                     Zero::Application::get().getWindow()->getHeight());

    auto&& player = m_world.getActiveScene()->createEntity("player");
    player.addComponent<Zero::CameraComponent>().camera.setOrthographicSize(5.0f);
    player.addComponent<Zero::NativeScriptComponent>().bind<CameraController>();

    auto& player_sprite = player.addComponent<Zero::SpriteComponent>();
    player_sprite.tex_index =
        Zero::Application::get().getResourceMngr()->index<Zero::ResourceType::Texture>("bella");

    auto&& square        = m_world.getActiveScene()->createEntity("square");
    auto&& square_sprite = square.addComponent<Zero::SpriteComponent>();
    square_sprite.tex_index =
        Zero::Application::get().getResourceMngr()->index<Zero::ResourceType::Texture>("bella");
    // square.addComponent<Zero::NativeScriptComponent>().bind<TexMarchingScript>();
}

void ExampleLayer::onDetach() {
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

    Zero::Widgets::drawText("Hello world");
}

void ExampleLayer::onEvent(Zero::Event& event) {
    Zero::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Zero::WindowResizeEvent>(ZE_BIND_EVENT_FN(ExampleLayer::onWindowResizeEvent));
}

bool ExampleLayer::onWindowResizeEvent(Zero::WindowResizeEvent& event) {
    m_world.onResize(event.getWidth(), event.getHeight());

    return false;
}
