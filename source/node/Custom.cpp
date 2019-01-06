#include "shaderweaver/node/Custom.h"

#include <cpputil/StringHelper.h>

namespace sw
{
namespace node
{

Custom::Custom()
    : Node("Custom")
{
}

void Custom::SetParams(const std::vector<Variable>& params)
{
    m_imports.clear();
    m_imports.reserve(params.size());
    for (auto& var : params)
    {
        auto v = var;
        auto type = v.GetType();
        type.region = VT_NODE_IN;
        v.SetType(type);
        m_imports.push_back(v);
    }
}

void Custom::SetReturn(const Variable& ret)
{
    m_exports.clear();

    auto v = ret;
    auto type = v.GetType();
    type.region = VT_NODE_OUT;
    v.SetType(type);
    m_exports.push_back(v);
}

void Custom::SetHeadStr(const std::string& str)
{
    m_head_str = FormatString(str);
}

void Custom::SetBodyStr(const std::string& str)
{
    m_body_str = FormatString(str);
}

std::string Custom::FormatString(const std::string& str)
{
    std::string ret = str;
    cpputil::StringHelper::ReplaceAll(ret, "\\n", "\n");
    cpputil::StringHelper::ReplaceAll(ret, "\\t", "\t");
    return ret;
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