#include <karin/system/event.h>

namespace karin
{
bool isApplicationEvent(const Event& event)
{
    return std::holds_alternative<ActionEvent>(event) || std::holds_alternative<TaskEvent>(event);
}
}
