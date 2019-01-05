#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Custom : public sw::Node
{
public:
    Custom()
        : Node("Custom")
    {
    }

    void SetParams(const std::vector<Variable>& params)
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

    void SetReturn(const Variable& ret)
    {
        m_exports.clear();

        auto v = ret;
        auto type = v.GetType();
        type.region = VT_NODE_OUT;
        v.SetType(type);
        m_exports.push_back(v);
    }

    auto& GetHeadStr() const { return m_head_str; }
    void SetHeadStr(const std::string& str) { m_head_str = str; }

    auto& GetBodyStr() const { return m_body_str; }
    void SetBodyStr(const std::string& str) { m_body_str = str; }

private:
    std::string m_head_str;
    std::string m_body_str;

    RTTR_ENABLE(Node)

}; // Custom

}
}