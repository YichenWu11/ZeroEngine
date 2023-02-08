#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>

#include "runtime/function/scene/components.h"
#include "runtime/function/scene/entity.h"
#include "runtime/function/scene/scene_serializer.h"

using namespace rapidjson;

namespace Zero {
    static Value getRapidJsonVec3Value(const Vector3& vec3, Document::AllocatorType& allocator) {
        Value r{kArrayType};
        r.PushBack(vec3.x, allocator);
        r.PushBack(vec3.y, allocator);
        r.PushBack(vec3.z, allocator);
        return r;
    }

    static Value getRapidJsonVec4Value(const Vector4& vec4, Document::AllocatorType& allocator) {
        Value r{kArrayType};
        r.PushBack(vec4.x, allocator);
        r.PushBack(vec4.y, allocator);
        r.PushBack(vec4.z, allocator);
        r.PushBack(vec4.w, allocator);
        return r;
    }

    static void serializeEntity(Entity entity, Value& entities, Document::AllocatorType& allocator) {
        Value entity_obj(kObjectType);
        entity_obj.SetObject();

        // TODO:guid
        Value entity_id(kStringType);
        entity_id.SetString("GUID");
        entity_obj.AddMember("EntityID", entity_id, allocator);

        if (entity.hasComponent<NameComponent>()) {
            Value component(kObjectType);
            component.SetObject();

            auto& name_component = entity.getComponent<NameComponent>();

            Value name{kStringType};
            name.SetString(name_component.name.c_str(), name_component.name.length(), allocator);
            component.AddMember("name", name, allocator);

            entity_obj.AddMember("NameComponent", component, allocator);
        }

        if (entity.hasComponent<TransformComponent>()) {
            Value component(kObjectType);
            component.SetObject();

            auto& trans_component = entity.getComponent<TransformComponent>();

            Value t(getRapidJsonVec3Value(trans_component.translation, allocator), allocator);
            Value r(getRapidJsonVec3Value(trans_component.rotation, allocator), allocator);
            Value s(getRapidJsonVec3Value(trans_component.scale, allocator), allocator);

            component.AddMember("translation", t, allocator);
            component.AddMember("rotation", r, allocator);
            component.AddMember("scale", s, allocator);

            entity_obj.AddMember("TransformComponent", component, allocator);
        }

        if (entity.hasComponent<SpriteComponent>()) {
            Value component(kObjectType);
            component.SetObject();

            auto& sprite_component = entity.getComponent<SpriteComponent>();

            Value color(getRapidJsonVec4Value(sprite_component.color, allocator));
            Value tex_index(kNumberType);
            tex_index.SetUint(sprite_component.tex_index);
            Value tiling_factor(kNumberType);
            tiling_factor.SetFloat(sprite_component.tiling_factor);

            component.AddMember("color", color, allocator);
            component.AddMember("tex_index", tex_index, allocator);
            component.AddMember("tiling_factor", tiling_factor, allocator);

            entity_obj.AddMember("SpriteComponent", component, allocator);
        }

        if (entity.hasComponent<CameraComponent>()) {
            Value component(kObjectType);
            component.SetObject();

            auto& camera_component = entity.getComponent<CameraComponent>();
            auto& camera           = camera_component.camera;

            {
                Value projectionType{kNumberType};
                projectionType.SetUint((uint32_t)camera.getProjectionType());

                Value perspectiveFOV{kNumberType};
                perspectiveFOV.SetFloat(camera.getPerspectiveVerticalFOV());
                Value perspectiveNear{kNumberType};
                perspectiveNear.SetFloat(camera.getPerspectiveNearClip());
                Value perspectiveFar{kNumberType};
                perspectiveFar.SetFloat(camera.getPerspectiveFarClip());

                Value orthographicSize{kNumberType};
                orthographicSize.SetFloat(camera.getOrthographicSize());
                Value orthographicNear{kNumberType};
                orthographicNear.SetFloat(camera.getOrthographicNearClip());
                Value orthographicFar{kNumberType};
                orthographicFar.SetFloat(camera.getOrthographicFarClip());

                component.AddMember("projectionType", projectionType, allocator);
                component.AddMember("perspectiveFOV", perspectiveFOV, allocator);
                component.AddMember("perspectiveNear", perspectiveNear, allocator);
                component.AddMember("perspectiveFar", perspectiveFar, allocator);
                component.AddMember("orthographicSize", orthographicSize, allocator);
                component.AddMember("orthographicNear", orthographicNear, allocator);
                component.AddMember("orthographicFar", orthographicFar, allocator);
            }

            Value is_current{
                (camera_component.is_current) ? kTrueType : kFalseType};
            Value is_fixed_aspectRatio{
                (camera_component.is_fixed_aspectRatio) ? kTrueType : kFalseType};

            component.AddMember("is_current", is_current, allocator);
            component.AddMember("is_fixed_aspectRatio", is_fixed_aspectRatio, allocator);

            entity_obj.AddMember("CameraComponent", component, allocator);
        }

        entities.PushBack(entity_obj, allocator);
    }

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene) :
        m_scene(scene) {
    }

    void SceneSerializer::serialize(const std::filesystem::path& path) {
        Document doc;
        doc.SetObject();
        Document::AllocatorType& allocator = doc.GetAllocator();

        Value scene(kStringType);
        scene.SetString("Untitled");
        doc.AddMember("Scene", scene, allocator);

        Value entities(kArrayType);

        m_scene->m_registry.each([&](auto entityID) {
            Entity entity = {entityID, m_scene.get()};
            if (!entity)
                return;

            serializeEntity(entity, entities, allocator);
        });

        doc.AddMember("Entities", entities, allocator);

        FILE*   fp;
        errno_t err = fopen_s(&fp, path.string().c_str(), "wb");

        char                    writeBuffer[65536];
        FileWriteStream         os(fp, writeBuffer, sizeof(writeBuffer));
        Writer<FileWriteStream> write(os);
        doc.Accept(write);

        fclose(fp);
    }

    void SceneSerializer::serializerRuntime(const std::filesystem::path& path) {
        // do nothing
    }

    bool SceneSerializer::deserialize(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) {
            LOG_WARN("The open_path doesn't exsit!");
            return false;
        }

        FILE*          fp;
        errno_t        err = fopen_s(&fp, path.string().c_str(), "rb");
        char           readBuffer[65536];
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));

        Document document;
        document.ParseStream(is);

        if (!document.HasMember("Scene")) {
            LOG_WARN("Invalid Scene Json Format!");
            return false;
        }

        if (!document.HasMember("Entities")) {
            LOG_WARN("Invalid Scene Json Format!");
            return false;
        }

        // ZE_ASSERT(document.HasMember("Scene") && "Invalid Scene Json Format!");
        // ZE_ASSERT(document.HasMember("Entities") && "Invalid Scene Json Format!");

        const Value& entities = document["Entities"];

        for (SizeType i = 0; i < entities.Size(); i++) {
            // TODO: guid
            auto guid = entities[i]["EntityID"].GetString();

            std::string entity_name;

            if (entities[i].HasMember("NameComponent")) {
                entity_name = entities[i]["NameComponent"]["name"].GetString();
            }

            LOG_TRACE("Deserialized entity with ID = {0}, name = {1}", guid, entity_name);

            Entity deserialized_entity = m_scene->createEntity(entity_name);

            if (entities[i].HasMember("TransformComponent")) {
                auto& trans_component = deserialized_entity.getComponent<TransformComponent>();

                trans_component.translation = Vector3{
                    entities[i]["TransformComponent"]["translation"][0].GetFloat(),
                    entities[i]["TransformComponent"]["translation"][1].GetFloat(),
                    entities[i]["TransformComponent"]["translation"][2].GetFloat()};

                trans_component.rotation = Vector3{
                    entities[i]["TransformComponent"]["rotation"][0].GetFloat(),
                    entities[i]["TransformComponent"]["rotation"][1].GetFloat(),
                    entities[i]["TransformComponent"]["rotation"][2].GetFloat()};

                trans_component.scale = Vector3{
                    entities[i]["TransformComponent"]["scale"][0].GetFloat(),
                    entities[i]["TransformComponent"]["scale"][1].GetFloat(),
                    entities[i]["TransformComponent"]["scale"][2].GetFloat()};
            }

            if (entities[i].HasMember("SpriteComponent")) {
                deserialized_entity.addComponent<SpriteComponent>();
                auto& sprite_component = deserialized_entity.getComponent<SpriteComponent>();

                sprite_component.color = Vector4{
                    entities[i]["SpriteComponent"]["color"][0].GetFloat(),
                    entities[i]["SpriteComponent"]["color"][1].GetFloat(),
                    entities[i]["SpriteComponent"]["color"][2].GetFloat(),
                    entities[i]["SpriteComponent"]["color"][3].GetFloat()};

                sprite_component.tex_index     = entities[i]["SpriteComponent"]["tex_index"].GetUint();
                sprite_component.tiling_factor = entities[i]["SpriteComponent"]["tiling_factor"].GetFloat();
            }

            if (entities[i].HasMember("CameraComponent")) {
                deserialized_entity.addComponent<CameraComponent>();
                auto& camera_component = deserialized_entity.getComponent<CameraComponent>();
                auto& camera           = camera_component.camera;

                camera.setProjectionType((SceneCamera::ProjectionType)entities[i]["CameraComponent"]["projectionType"].GetUint());

                camera.setPerspectiveVerticalFOV(entities[i]["CameraComponent"]["perspectiveFOV"].GetFloat());
                camera.setPerspectiveNearClip(entities[i]["CameraComponent"]["perspectiveNear"].GetFloat());
                camera.setPerspectiveFarClip(entities[i]["CameraComponent"]["perspectiveFar"].GetFloat());

                camera.setOrthographicSize(entities[i]["CameraComponent"]["orthographicSize"].GetFloat());
                camera.setOrthographicNearClip(entities[i]["CameraComponent"]["orthographicNear"].GetFloat());
                camera.setOrthographicFarClip(entities[i]["CameraComponent"]["orthographicFar"].GetFloat());

                camera_component.is_current           = entities[i]["CameraComponent"]["is_current"].GetBool();
                camera_component.is_fixed_aspectRatio = entities[i]["CameraComponent"]["is_fixed_aspectRatio"].GetBool();
            }
        }

        fclose(fp);
        return true;
    }

    bool SceneSerializer::deserializerRuntime(const std::filesystem::path& path) {
        // do nothing
        return false;
    }

} // namespace Zero
