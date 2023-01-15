#include "runtime/core/common/base_application.h"

namespace Zero {
    BaseApplication g_App;

    IApplication* g_pApp = &g_App;
} // namespace Zero
