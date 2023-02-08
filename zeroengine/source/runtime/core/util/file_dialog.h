#pragma once

#define GET_FILE_DIALOG() ::Zero::FileDialog::getInstance()

namespace Zero {
    class FileDialog {
    public:
        static FileDialog& getInstance() {
            static FileDialog instance;
            return instance;
        }

        FileDialog(const FileDialog&)            = delete;
        FileDialog& operator=(const FileDialog&) = delete;

        std::string openFile(const char* filter);
        std::string saveFile(const char* filter);

    private:
        FileDialog()  = default;
        ~FileDialog() = default;
    };
} // namespace Zero