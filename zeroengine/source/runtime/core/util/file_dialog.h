#pragma once

#include "runtime/core/util/singleton.h"

#define GET_FILE_DIALOG() ::Zero::FileDialog::getInstance()

namespace Zero {
    class FileDialog : public Singleton<FileDialog> {
    public:
        std::string openFile(const char* filter);
        std::string saveFile(const char* filter);
    };
} // namespace Zero