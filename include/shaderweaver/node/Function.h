#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/typedef.h"

#include <cpputil/StringHelper.h>

namespace sw
{
namespace node
{

class Function : public sw::Node
{
public:
    Function();

    void SetEndNodes(const std::vector<NodePtr>& end_nodes) {
        m_end_nodes = end_nodes;
    }
    auto& GetEndNodes() const { return m_end_nodes; }

protected:
    virtual std::string GetBody() const override;

private:
    std::vector<NodePtr> m_end_nodes;

    RTTR_ENABLE(Node)

}; // Function

}
}