//#pragma once
//
//#include "sw/Node.h"
//#include "sw/VariableType.h"
//
//namespace sw
//{
//namespace node
//{
//
//class Vector4 : public Node
//{
//public:
//	Vector4(const std::string& name)
//		: Node("Vector4")
//	{
//		if (name.empty()) {
//			AddVariable(Variable(t_vec4 | t_out, "vec4"));
//		} else {
//			AddVariable(Variable(t_vec4 | t_out, name));
//		}
//	}
//
//protected:
//	virtual std::string GetBody() const override
//	{
//		return "";
//	}
//
//}; // Vector4
//
//}
//}