#include "shaderweaver/node/Custom.h"

namespace sw
{
namespace node
{

Custom::Custom()
    : Node("Custom")
{
}

std::string Custom::GetHeader() const
{
    std::string ret;
    if (!m_head_str.empty()) {
        ret += m_head_str;
        ret += "\n";
    }
    if (!m_body_str.empty()) {
        ret += m_body_str;
        ret += "\n";
    }
    return ret;
}

std::string Custom::GetBody() const
{
    std::string ret;

    if (!m_exports.empty()) {
        ret += "#" + m_exports[0].var.GetName() + "# = ";
    }
    ret += GetName() + "(";
    for (int i = 0, n = m_imports.size(); i < n; ++i) {
        auto& p = m_imports[i];
        if (i != 0) {
            ret += ", ";
        }
        ret += "#" + p.var.GetName() + "#";
    }
    ret += ");";

    return ret;
}

}
}