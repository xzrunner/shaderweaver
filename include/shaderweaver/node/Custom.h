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

    void SetHeadStr(const std::string& str) { m_head_str = str; }
    void SetBodyStr(const std::string& str) { m_body_str = str; }

protected:
    virtual std::string GetHeader() const;
    virtual std::string GetBody() const;

private:
    std::string m_head_str;
    std::string m_body_str;

    RTTR_ENABLE(Node)

}; // Custom

}
}