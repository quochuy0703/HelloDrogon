#include <array>
#include <string>

namespace constants
{
    constexpr std::array<std::string, 2> EXCEPT_ROUTE = {"swagger", "api/auth"};

    enum class SAR_STEPS
    {
        START,
        CREATE_REQUEST,
        SECTION_APPROVAL,
        QC_CONFIRM,
        SECTION_FEEDBACK,
        QC_APPROVAL,
        SECTION_ADD_FILE,
        DEPARTMENT_QC_APPROVAL
    };
}