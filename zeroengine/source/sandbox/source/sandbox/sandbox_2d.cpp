#include "sandbox/sandbox_2d.h"

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f) {}

void Sandbox2D::onAttach() {
    GET_TEXTURE_POOL().registerTex(
        std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/bella.png");
    GET_TEXTURE_POOL().registerTex(
        std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/asoul.png");
    GET_TEXTURE_POOL().registerTex(
        "SpriteSheet",
        std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/game/texture/sprite_sheet.png");
    GET_TEXTURE_POOL().registerTex(
        std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/icon.png");

    m_texture_stair = Zero::SubTexture2D::createFromCoords(
        GET_TEXTURE_POOL().getTextureFromName("SpriteSheet"),
        {7, 6},
        {128.0f, 128.0f});

    m_texture_bush = Zero::SubTexture2D::createFromCoords(
        GET_TEXTURE_POOL().getTextureFromName("SpriteSheet"),
        {2, 3},
        {256.0f, 128.0f});

    m_camera_controller.setZoomLevel(1.0f);
}

void Sandbox2D::onDetach() {
}

void Sandbox2D::onUpdate(Zero::TimeStep timestep) {
    // update
    {
        ZE_PROFILE_SCOPE("CameraController::onUpdate");
        m_camera_controller.onUpdate(timestep);
    }

    // render
    // {
    //     Zero::RenderCommand::setClearColor(clear_color);
    //     Zero::RenderCommand::clear();
    // }

    {
        ZE_PROFILE_SCOPE("Renderer2D::Render");
        Zero::Renderer2D::beginScene(m_camera_controller.getCamera());

        Zero::Renderer2D::drawQuad(
            {-0.8f, 0.0f},
            {1.0f, 1.0f},
            0.0f,
            {1.0f, 1.0f, 1.0f, 1.0f},
            GET_TEXTURE_POOL().getTexIndexFromName("bella"));

        Zero::Renderer2D::drawQuad(
            position,
            {1.0f, 1.0f},
            0.0f,
            {1.0f, 1.0f, 1.0f, 1.0f},
            GET_TEXTURE_POOL().getTexIndexFromName("bella"));

        Zero::Renderer2D::drawQuad(
            {0.8f, 0.0f},
            {1.0f, 1.0f},
            0.0f,
            {1.0f, 1.0f, 1.0f, 1.0f},
            GET_TEXTURE_POOL().getTexIndexFromName("bella"));

        Zero::Renderer2D::drawCellQuad(
            {0.0f, 2.0f, 0.0f},
            {1.0f, 1.0f},
            0.0f,
            m_texture_stair);

        Zero::Renderer2D::drawCellQuad(
            {0.0f, -2.0f, 0.0f},
            {2.0f, 1.0f},
            0.0f,
            m_texture_bush);

        Zero::Renderer2D::drawQuad(
            {0.0f, 0.0f, 0.2f},
            {10.0f, 10.0f},
            0.0f,
            {0.8f, 0.8f, 0.8f, 1.0f},
            GET_TEXTURE_POOL().getTexIndexFromName("asoul"), 5.0f);

        for (float y = -5.0f; y < 5.0f; y += 0.5f) {
            for (float x = -5.0f; x < 5.0f; x += 0.5f) {
                Zero::Renderer2D::drawQuad(
                    {x, y, 0.1f},
                    {0.45f, 0.45f},
                    0.0f,
                    {(x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 1.0f, 0.8f});
            }
        }

        Zero::Renderer2D::endScene();
    }
}

void Sandbox2D::onImGuiRender() {
    ZE_PROFILE_FUNCTION();
}

void Sandbox2D::onEvent(Zero::Event& event) {
    m_camera_controller.onEvent(event);
}
