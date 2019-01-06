#pragma once

#include "shaderweaver/Node.h"

namespace sw
{
namespace node
{

class Custom : public sw::Node
{
public:
    Custom();

    void SetParams(const std::vector<Variable>& params);
    void SetReturn(const Variable& ret);

    void SetHeadStr(const std::string& str);
    void SetBodyStr(const std::string& str);

protected:
    virtual std::string GetHeader() const;
    virtual std::string GetBody() const;

private:
    static std::string FormatString(const std::string& str);

private:
    std::string m_head_str;
    std::string m_body_str;

    RTTR_ENABLE(Node)

}; // Custom

}
}