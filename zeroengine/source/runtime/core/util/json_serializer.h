#pragma once

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>

namespace Zero {
    class JsonSerialzer {
    public:
        static void serialze(const rapidjson::Document& doc, const std::filesystem::path& path) {
            FILE*   fp;
            errno_t err = fopen_s(&fp, path.string().c_str(), "wb");

            char                                          writeBuffer[65536];
            rapidjson::FileWriteStream                    os(fp, writeBuffer, sizeof(writeBuffer));
            rapidjson::Writer<rapidjson::FileWriteStream> write(os);
            doc.Accept(write);

            fclose(fp);
        }

        static void deserialze(rapidjson::Document& doc, const std::filesystem::path& path) {
            if (!std::filesystem::exists(path)) {
                LOG_WARN("The open_path doesn't exsit!");
                return;
            }

            FILE*                     fp;
            errno_t                   err = fopen_s(&fp, path.string().c_str(), "rb");
            char                      readBuffer[65536];
            rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

            doc.ParseStream(is);

            fclose(fp);
        }
    };
} // namespace Zero