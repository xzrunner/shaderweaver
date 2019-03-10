#include "shaderweaver/node/Function.h"

namespace sw
{
namespace node
{

Function::Function()
    : Node("Function")
{
}

std::string Function::GetBody() const
{
    std::string str_params;
    for (int i = 0, n = m_imports.size(); i < n; ++i) {
        str_params += cpputil::StringHelper::Format("#%s#", m_imports[i].var.GetName().c_str());
        if (i != n - 1) {
            str_params += ", ";
        }
    }
    std::string str_func_call = cpputil::StringHelper::Format("%s(%s);\n", GetName().c_str(), str_params.c_str());
    if (m_exports.empty())
    {
        return str_func_call;
    }
    else
    {
        return cpputil::StringHelper::Format("#%s# = %s", m_exports[0].var.GetName().c_str(), str_func_call.c_str());
    }
}

}
}