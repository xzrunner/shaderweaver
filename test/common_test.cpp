#define CATCH_CONFIG_MAIN

#include <unirender/gl/RenderContext.h>
#include <sw/Evaluator.h>

#include <sw/node/Uniform.h>
#include <sw/node/Input.h>
#include <sw/node/Output.h>
#include <sw/node/PositionTrans.h>
#include <sw/node/Tex2DSample.h>
#include <sw/node/ColorAddMul.h>
#include <sw/node/Add.h>
#include <sw/node/Vector4.h>

#include <catch/catch.hpp>
#include <gl/glew.h>
#include <glfw3.h>

namespace
{

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

bool InitGL()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(100, 100, "rotate-crop", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	//// Initialize GLEW to setup the OpenGL Function pointers
	//if (glewInit() != GLEW_OK) {
	//	return -1;
	//}

	return true;
}

std::shared_ptr<ur::gl::RenderContext> RC = nullptr;

void init()
{
	static bool inited = false;
	if (!inited) {
		InitGL();
		RC = std::make_shared<ur::gl::RenderContext>(1024);
		inited = true;
	}
}

int create_shader(const sw::Evaluator& vert, const sw::Evaluator& frag)
{
	std::vector<std::string> textures;
	return RC->CreateShader(vert.GetShaderStr().c_str(), frag.GetShaderStr().c_str(), textures);
}

void debug_print(const sw::Evaluator& vert, const sw::Evaluator& frag)
{
	printf("//////////////////////////////////////////////////////////////////////////\n");
	printf("%s\n", vert.GetShaderStr().c_str());
	printf("//////////////////////////////////////////////////////////////////////////\n");
	printf("%s\n", frag.GetShaderStr().c_str());
	printf("//////////////////////////////////////////////////////////////////////////\n");
}

void add_vert_pos_trans(std::vector<sw::NodePtr>& nodes, std::vector<sw::NodePtr>& cache_nodes)
{
	auto projection = std::make_shared<sw::node::Uniform>("u_projection", sw::t_mat4);
	auto modelview = std::make_shared<sw::node::Uniform>("u_modelview", sw::t_mat4);
	auto position = std::make_shared<sw::node::Input>("position", sw::t_flt4);
	auto pos_trans = std::make_shared<sw::node::PositionTrans>();
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTrans::IN_PROJ });
	sw::make_connecting({ modelview,  0 }, { pos_trans, sw::node::PositionTrans::IN_MODELVIEW });
	sw::make_connecting({ position,   0 }, { pos_trans, sw::node::PositionTrans::IN_POS });
	nodes.push_back(pos_trans);

	cache_nodes.push_back(projection);
	cache_nodes.push_back(modelview);
	cache_nodes.push_back(position);
}

void add_vert_varying(std::vector<sw::NodePtr>& nodes, std::vector<sw::NodePtr>& cache_nodes, const std::string& name, uint32_t type)
{
	auto vert_in  = std::make_shared<sw::node::Input> (name, type);
	auto vert_out = std::make_shared<sw::node::Output>("v_" + name, type);
	sw::make_connecting({ vert_in, 0 }, { vert_out, 0 });
	nodes.push_back(vert_out);

	cache_nodes.push_back(vert_in);
}

}

TEST_CASE("shape2d") {
	init();

	// layout

	std::vector<ur::VertexAttrib> va_list;
	const int sz = 12;
	va_list.push_back(ur::VertexAttrib("position", 2, sizeof(float),   sz, 0));
	va_list.push_back(ur::VertexAttrib("color",    4, sizeof(uint8_t), sz, 8));

	auto layout_id = RC->CreateVertexLayout(va_list);
	RC->BindVertexLayout(layout_id);

	// vert

	std::vector<sw::NodePtr> vert_nodes;
	std::vector<sw::NodePtr> cache_nodes;
	add_vert_pos_trans(vert_nodes, cache_nodes);
	add_vert_varying(vert_nodes, cache_nodes, "color", sw::t_col4);

	// frag

	auto frag_in_col = std::make_shared<sw::node::Input>("v_color", sw::t_col4);

	// end

	sw::Evaluator vert(vert_nodes, sw::ST_VERT);
	sw::Evaluator frag({ frag_in_col }, sw::ST_FRAG);

	//debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}

TEST_CASE("sprite") {
	init();

	// layout

	std::vector<ur::VertexAttrib> va_list;
	const int sz = 24;
	va_list.push_back(ur::VertexAttrib("position", 2, sizeof(float),   sz, 0));
	va_list.push_back(ur::VertexAttrib("texcoord", 2, sizeof(float),   sz, 8));
	va_list.push_back(ur::VertexAttrib("color",    4, sizeof(uint8_t), sz, 16));
	va_list.push_back(ur::VertexAttrib("additive", 4, sizeof(uint8_t), sz, 20));

	auto layout_id = RC->CreateVertexLayout(va_list);
	RC->BindVertexLayout(layout_id);

	// vert

	std::vector<sw::NodePtr> vert_nodes;
	add_vert_pos_trans(vert_nodes, vert_nodes);
	add_vert_varying(vert_nodes, vert_nodes, "texcoord", sw::t_uv);
	add_vert_varying(vert_nodes, vert_nodes, "color",    sw::t_col4);
	add_vert_varying(vert_nodes, vert_nodes, "additive", sw::t_col4);

	// frag

	auto tex_sample  = std::make_shared<sw::node::Tex2DSample>();
	auto frag_in_tex = std::make_shared<sw::node::Uniform>("u_texture0", sw::t_tex2d);
	auto frag_in_uv  = std::make_shared<sw::node::Input>  ("v_texcoord", sw::t_uv);
	sw::make_connecting({ frag_in_tex, 0 }, { tex_sample, sw::node::Tex2DSample::IN_TEX });
	sw::make_connecting({ frag_in_uv,  0 }, { tex_sample, sw::node::Tex2DSample::IN_UV });

	auto col_add_mul = std::make_shared<sw::node::ColorAddMul>();
	auto frag_in_mul = std::make_shared<sw::node::Input>("v_color",    sw::t_col4);
	auto frag_in_add = std::make_shared<sw::node::Input>("v_additive", sw::t_col4);
	sw::make_connecting({ tex_sample,  0 }, { col_add_mul, sw::node::ColorAddMul::IN_COL });
	sw::make_connecting({ frag_in_mul, 0 }, { col_add_mul, sw::node::ColorAddMul::IN_MUL });
	sw::make_connecting({ frag_in_add, 0 }, { col_add_mul, sw::node::ColorAddMul::IN_ADD });

	// end

	sw::Evaluator vert(vert_nodes, sw::ST_VERT);
	sw::Evaluator frag({ col_add_mul }, sw::ST_FRAG);

	//debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}

TEST_CASE("rename") {
	init();

	// layout

	std::vector<ur::VertexAttrib> va_list;
	const int sz = 12;
	va_list.push_back(ur::VertexAttrib("position", 2, sizeof(float), sz, 0));
	va_list.push_back(ur::VertexAttrib("color", 4, sizeof(uint8_t), sz, 8));

	auto layout_id = RC->CreateVertexLayout(va_list);
	RC->BindVertexLayout(layout_id);

	// vert

	std::vector<sw::NodePtr> vert_nodes;
	add_vert_pos_trans(vert_nodes, vert_nodes);

	auto vert_color_in  = std::make_shared<sw::node::Input>("color", sw::t_col4);

	auto add0 = std::make_shared<sw::node::Add>();
	auto add0_a = std::make_shared<sw::node::Vector4>("", sm::vec4(0, 1, 2, 3));
	sw::make_connecting({ add0_a, 0 }, { add0, sw::node::Add::IN_A});
	sw::make_connecting({ vert_color_in, 0 }, { add0, sw::node::Add::IN_B });

	auto add1 = std::make_shared<sw::node::Add>();
	auto add1_b = std::make_shared<sw::node::Vector4>("", sm::vec4(4, 5, 6, 7));
	sw::make_connecting({ add0, 0 }, { add1, sw::node::Add::IN_A });
	sw::make_connecting({ add1_b, 0 }, { add1, sw::node::Add::IN_B });

	auto vert_color_out = std::make_shared<sw::node::Output>("v_color", sw::t_col4);
	sw::make_connecting({ add1, 0 }, { vert_color_out, 0 });
	vert_nodes.push_back(vert_color_out);

	// frag

	auto frag_in_col = std::make_shared<sw::node::Input>("v_color", sw::t_col4);

	// end

	sw::Evaluator vert(vert_nodes, sw::ST_VERT);
	sw::Evaluator frag({ frag_in_col }, sw::ST_FRAG);

	debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}