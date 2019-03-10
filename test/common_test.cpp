#define CATCH_CONFIG_MAIN

#include <unirender/gl/RenderContext.h>
#include <shaderweaver/Evaluator.h>

#include <shaderweaver/node/ShaderUniform.h>
#include <shaderweaver/node/ShaderInput.h>
#include <shaderweaver/node/ShaderOutput.h>
#include <shaderweaver/node/PositionTransOld.h>
#include <shaderweaver/node/SampleTex2D.h>
#include <shaderweaver/node/ColorAddMul.h>
#include <shaderweaver/node/Add.h>
#include <shaderweaver/node/Vector1.h>
#include <shaderweaver/node/Vector3.h>
#include <shaderweaver/node/Vector4.h>
#include <shaderweaver/node/Blend.h>
#include <shaderweaver/node/Gray.h>
#include <shaderweaver/node/PositionTrans.h>
#include <shaderweaver/node/FragPosTrans.h>
#include <shaderweaver/node/NormalTrans.h>
#include <shaderweaver/node/Phong.h>
#include <shaderweaver/node/VertexShader.h>
#include <shaderweaver/node/FragmentShader.h>

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
	return RC->CreateShader(vert.GenShaderStr().c_str(), frag.GenShaderStr().c_str(), textures);
}

void debug_print(const sw::Evaluator& vert, const sw::Evaluator& frag)
{
	printf("//////////////////////////////////////////////////////////////////////////\n");
	printf("%s\n", vert.GenShaderStr().c_str());
	printf("//////////////////////////////////////////////////////////////////////////\n");
	printf("%s\n", frag.GenShaderStr().c_str());
	printf("//////////////////////////////////////////////////////////////////////////\n");
}

void add_vert_pos_trans(std::vector<sw::NodePtr>& nodes, std::vector<sw::NodePtr>& cache_nodes)
{
	auto projection = std::make_shared<sw::node::ShaderUniform>("u_projection", sw::t_mat4);
	auto modelview  = std::make_shared<sw::node::ShaderUniform>("u_modelview",  sw::t_mat4);
	auto position   = std::make_shared<sw::node::ShaderInput>  ("position",     sw::t_pos2);
    cache_nodes.push_back(projection);
    cache_nodes.push_back(modelview);
    cache_nodes.push_back(position);

	auto pos_trans  = std::make_shared<sw::node::PositionTransOld>(2);
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTransOld::ID_PROJ });
	sw::make_connecting({ modelview,  0 }, { pos_trans, sw::node::PositionTransOld::ID_MODELVIEW });
	sw::make_connecting({ position,   0 }, { pos_trans, sw::node::PositionTransOld::ID_POS });
    cache_nodes.push_back(pos_trans);

    auto vert_end = std::make_shared<sw::node::VertexShader>();
    sw::make_connecting({ pos_trans, 0 }, { vert_end, 0 });
	nodes.push_back(vert_end);
}

void add_vert_varying(std::vector<sw::NodePtr>& nodes, std::vector<sw::NodePtr>& cache_nodes, const std::string& name, uint32_t type)
{
	auto vert_in  = std::make_shared<sw::node::ShaderInput> (name, type);
	auto vert_out = std::make_shared<sw::node::ShaderOutput>("v_" + name, type);
	sw::make_connecting({ vert_in, 0 }, { vert_out, 0 });
	nodes.push_back(vert_out);

	cache_nodes.push_back(vert_in);
}

enum ShaderType
{
	ST_SHAPE = 0,
	ST_SPRITE,
	ST_MODEL,
};

void init_layout(ShaderType type)
{
	std::vector<ur::VertexAttrib> va_list;
	switch (type)
	{
	case ST_SHAPE:
		va_list.push_back(ur::VertexAttrib("position", 2, sizeof(float),   12, 0));
		va_list.push_back(ur::VertexAttrib("color",    4, sizeof(uint8_t), 12, 8));
		break;
	case ST_SPRITE:
		va_list.push_back(ur::VertexAttrib("position", 2, sizeof(float),   24, 0));
		va_list.push_back(ur::VertexAttrib("texcoord", 2, sizeof(float),   24, 8));
		va_list.push_back(ur::VertexAttrib("color",    4, sizeof(uint8_t), 24, 16));
		va_list.push_back(ur::VertexAttrib("additive", 4, sizeof(uint8_t), 24, 20));
		break;
	case ST_MODEL:
		va_list.push_back(ur::VertexAttrib("position", 3, 4, 32, 0));
		va_list.push_back(ur::VertexAttrib("normal",   3, 4, 32, 12));
		va_list.push_back(ur::VertexAttrib("texcoord", 2, 4, 32, 24));
		break;
	}
	RC->CreateVertexLayout(va_list);
}

void init_vert(ShaderType type, std::vector<sw::NodePtr>& vert_nodes, std::vector<sw::NodePtr>& cache_nodes)
{
	switch (type)
	{
	case ST_SHAPE:
		add_vert_pos_trans(vert_nodes, cache_nodes);
		add_vert_varying(vert_nodes, cache_nodes, "color", sw::t_col4);
		break;
	case ST_SPRITE:
		add_vert_pos_trans(vert_nodes, vert_nodes);
		add_vert_varying(vert_nodes, cache_nodes, "texcoord", sw::t_uv);
		add_vert_varying(vert_nodes, cache_nodes, "color",    sw::t_col4);
		add_vert_varying(vert_nodes, cache_nodes, "additive", sw::t_col4);
		break;
	}
}

sw::NodePtr init_frag(ShaderType type, std::vector<sw::NodePtr>& cache_nodes)
{
    sw::NodePtr ret = nullptr;
	switch (type)
	{
	case ST_SHAPE:
    {
        auto color = std::make_shared<sw::node::ShaderInput>("v_color", sw::t_col4);
        cache_nodes.push_back(color);
        ret = color;
    }
        break;
	case ST_SPRITE:
	{
		auto tex_sample  = std::make_shared<sw::node::SampleTex2D>();
		auto frag_in_tex = std::make_shared<sw::node::ShaderUniform>("u_texture0", sw::t_tex2d);
		auto frag_in_uv  = std::make_shared<sw::node::ShaderInput>  ("v_texcoord", sw::t_uv);
		sw::make_connecting({ frag_in_tex, 0 }, { tex_sample, sw::node::SampleTex2D::ID_TEX });
		sw::make_connecting({ frag_in_uv,  0 }, { tex_sample, sw::node::SampleTex2D::ID_UV });
        cache_nodes.push_back(tex_sample);
        cache_nodes.push_back(frag_in_tex);
        cache_nodes.push_back(frag_in_uv);

		auto col_add_mul = std::make_shared<sw::node::ColorAddMul>();
		auto frag_in_mul = std::make_shared<sw::node::ShaderInput>("v_color",    sw::t_col4);
		auto frag_in_add = std::make_shared<sw::node::ShaderInput>("v_additive", sw::t_col4);
		sw::make_connecting({ tex_sample,  0 }, { col_add_mul, sw::node::ColorAddMul::ID_COL });
		sw::make_connecting({ frag_in_mul, 0 }, { col_add_mul, sw::node::ColorAddMul::ID_MUL });
		sw::make_connecting({ frag_in_add, 0 }, { col_add_mul, sw::node::ColorAddMul::ID_ADD });
        cache_nodes.push_back(col_add_mul);
        cache_nodes.push_back(frag_in_mul);
        cache_nodes.push_back(frag_in_add);

        ret = col_add_mul;
	}
	default:
        assert(0);
	}
    return ret;
}

}

TEST_CASE("shape2d") {
	init();

	// layout
	init_layout(ST_SHAPE);

	// vert
	std::vector<sw::NodePtr> cache_nodes;

	std::vector<sw::NodePtr> vert_nodes;
	init_vert(ST_SHAPE, vert_nodes, cache_nodes);

	// frag
	auto frag_node = init_frag(ST_SHAPE, cache_nodes);
    auto frag_end = std::make_shared<sw::node::FragmentShader>();
    sw::make_connecting({ frag_node, 0 }, { frag_end, 0 });

	// end
	sw::Evaluator vert(vert_nodes);
	sw::Evaluator frag({ frag_end });

	//debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}

TEST_CASE("sprite") {
	init();

	// layout
	std::vector<ur::VertexAttrib> va_list;
	va_list.push_back(ur::VertexAttrib("position", 2, sizeof(float), 16, 0));
	va_list.push_back(ur::VertexAttrib("texcoord", 2, sizeof(float), 16, 8));
	RC->CreateVertexLayout(va_list);

	// vert
	std::vector<sw::NodePtr> cache_nodes;
	std::vector<sw::NodePtr> vert_nodes;

	auto projection = std::make_shared<sw::node::ShaderUniform>("u_projection", sw::t_mat4);
	auto modelview  = std::make_shared<sw::node::ShaderUniform>("u_modelview",  sw::t_mat4);
	auto position   = std::make_shared<sw::node::ShaderInput>  ("position",     sw::t_pos2);
	auto pos_trans  = std::make_shared<sw::node::PositionTransOld>(2);
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTransOld::ID_PROJ });
	sw::make_connecting({ modelview,  0 }, { pos_trans, sw::node::PositionTransOld::ID_MODELVIEW });
	sw::make_connecting({ position,   0 }, { pos_trans, sw::node::PositionTransOld::ID_POS });
    cache_nodes.push_back(projection);
    cache_nodes.push_back(modelview);
    cache_nodes.push_back(position);
    cache_nodes.push_back(pos_trans);

    auto vert_end = std::make_shared<sw::node::VertexShader>();
    sw::make_connecting({ pos_trans, 0 }, { vert_end, 0 });
	vert_nodes.push_back(vert_end);

	add_vert_varying(vert_nodes, cache_nodes, "texcoord", sw::t_uv);

	// frag
	auto tex_sample = std::make_shared<sw::node::SampleTex2D>();
	auto frag_in_tex = std::make_shared<sw::node::ShaderUniform>("u_texture0", sw::t_tex2d);
	auto frag_in_uv = std::make_shared<sw::node::ShaderInput>("v_texcoord", sw::t_uv);
	sw::make_connecting({ frag_in_tex, 0 }, { tex_sample, sw::node::SampleTex2D::ID_TEX });
	sw::make_connecting({ frag_in_uv,  0 }, { tex_sample, sw::node::SampleTex2D::ID_UV });
	cache_nodes.push_back(tex_sample);
	cache_nodes.push_back(frag_in_tex);
	cache_nodes.push_back(frag_in_uv);

    auto frag_end = std::make_shared<sw::node::FragmentShader>();
    sw::make_connecting({ tex_sample, 0 }, { frag_end, 0 });

	// end

	sw::Evaluator vert(vert_nodes);
	sw::Evaluator frag({ frag_end });

	//debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}

TEST_CASE("sprite with color") {
	init();

	// layout
	init_layout(ST_SPRITE);

	// vert
	std::vector<sw::NodePtr> cache_nodes;

	std::vector<sw::NodePtr> vert_nodes;
	init_vert(ST_SPRITE, vert_nodes, cache_nodes);

	// frag
	auto frag_node = init_frag(ST_SPRITE, cache_nodes);
    auto frag_end = std::make_shared<sw::node::FragmentShader>();
    sw::make_connecting({ frag_node, 0 }, { frag_end, 0 });

	// end

	sw::Evaluator vert(vert_nodes);
	sw::Evaluator frag({ frag_end });

	//debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}

//TEST_CASE("blend") {
//	init();
//
//	// layout
//	std::vector<ur::VertexAttrib> va_list;
//	const int sz = 32;
//	va_list.push_back(ur::VertexAttrib("position",      2, sizeof(float),   sz, 0));
//	va_list.push_back(ur::VertexAttrib("texcoord",      2, sizeof(float),   sz, 8));
//	va_list.push_back(ur::VertexAttrib("texcoord_base", 2, sizeof(float),   sz, 16));
//	va_list.push_back(ur::VertexAttrib("color",         4, sizeof(uint8_t), sz, 24));
//	va_list.push_back(ur::VertexAttrib("additive",      4, sizeof(uint8_t), sz, 28));
//
//	RC->CreateVertexLayout(va_list);
//
//	// vert
//
//	std::vector<sw::NodePtr> cache_nodes;
//
//	std::vector<sw::NodePtr> vert_nodes;
//	add_vert_pos_trans(vert_nodes, vert_nodes);
//	add_vert_varying(vert_nodes, cache_nodes, "texcoord",      sw::t_uv);
//	add_vert_varying(vert_nodes, cache_nodes, "texcoord_base", sw::t_uv);
//	add_vert_varying(vert_nodes, cache_nodes, "color",         sw::t_col4);
//	add_vert_varying(vert_nodes, cache_nodes, "additive",      sw::t_col4);
//
//	// frag
//
//	auto tex_sample  = std::make_shared<sw::node::SampleTex2D>();
//	auto frag_in_tex = std::make_shared<sw::node::ShaderUniform>("u_texture0", sw::t_tex2d);
//	auto frag_in_uv  = std::make_shared<sw::node::ShaderInput>  ("v_texcoord", sw::t_uv);
//	sw::make_connecting({ frag_in_tex, 0 }, { tex_sample, sw::node::SampleTex2D::ID_TEX });
//	sw::make_connecting({ frag_in_uv,  0 }, { tex_sample, sw::node::SampleTex2D::ID_UV });
//
//	auto col_add_mul = std::make_shared<sw::node::ColorAddMul>();
//	auto frag_in_mul = std::make_shared<sw::node::ShaderInput>("v_color", sw::t_col4);
//	auto frag_in_add = std::make_shared<sw::node::ShaderInput>("v_additive", sw::t_col4);
//	sw::make_connecting({ tex_sample,  0 }, { col_add_mul, sw::node::ColorAddMul::ID_COL });
//	sw::make_connecting({ frag_in_mul, 0 }, { col_add_mul, sw::node::ColorAddMul::ID_MUL });
//	sw::make_connecting({ frag_in_add, 0 }, { col_add_mul, sw::node::ColorAddMul::ID_ADD });
//
//	auto base_tex_sample  = std::make_shared<sw::node::SampleTex2D>();
//	auto frag_in_base_tex = std::make_shared<sw::node::ShaderUniform>("u_texture1",      sw::t_tex2d);
//	auto frag_in_bae_uv   = std::make_shared<sw::node::ShaderInput>  ("v_texcoord_base", sw::t_uv);
//	sw::make_connecting({ frag_in_base_tex, 0 }, { base_tex_sample, sw::node::SampleTex2D::ID_TEX });
//	sw::make_connecting({ frag_in_bae_uv,  0 },  { base_tex_sample, sw::node::SampleTex2D::ID_UV });
//
//	auto blend = std::make_shared<sw::node::Blend>();
//	auto blend_mode = std::make_shared<sw::node::ShaderUniform>("u_mode", sw::t_int1);
//	sw::make_connecting({ base_tex_sample,  0 }, { blend, sw::node::Blend::ID_BASE4 });
//	sw::make_connecting({ col_add_mul,  0 },     { blend, sw::node::Blend::ID_BLEND4 });
//	sw::make_connecting({ blend_mode,  0 },      { blend, sw::node::Blend::ID_MODE });
//
//	// end
//
//	sw::Evaluator vert(vert_nodes, sw::ST_VERT);
//	sw::Evaluator frag({ blend }, sw::ST_FRAG);
//
//	//debug_print(vert, frag);
//	int shader = create_shader(vert, frag);
//
//	REQUIRE(shader != 0);
//}

TEST_CASE("gray") {
	init();

	// layout
	init_layout(ST_SPRITE);

	// vert
	std::vector<sw::NodePtr> cache_nodes;

	std::vector<sw::NodePtr> vert_nodes;
	init_vert(ST_SPRITE, vert_nodes, cache_nodes);

	// frag
	auto frag_node = init_frag(ST_SPRITE, cache_nodes);

	auto gray = std::make_shared<sw::node::Gray>();
	sw::make_connecting({ frag_node,  0 }, { gray, 0 });
    cache_nodes.push_back(gray);

    auto frag_end = std::make_shared<sw::node::FragmentShader>();
    sw::make_connecting({ gray, 0 }, { frag_end, 0 });

	// end
	sw::Evaluator vert(vert_nodes);
	sw::Evaluator frag({ frag_end });

	//debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}

TEST_CASE("rename") {
	init();

	// layout
	init_layout(ST_SHAPE);

	// vert
	std::vector<sw::NodePtr> vert_nodes;
	add_vert_pos_trans(vert_nodes, vert_nodes);

	auto vert_color_in  = std::make_shared<sw::node::ShaderInput>("color", sw::t_col4);

	auto add0 = std::make_shared<sw::node::Add>();
	auto add0_a = std::make_shared<sw::node::Vector4>("", sm::vec4(0, 1, 2, 3));
	sw::make_connecting({ add0_a, 0 }, { add0, sw::node::Add::ID_A});
	sw::make_connecting({ vert_color_in, 0 }, { add0, sw::node::Add::ID_B });

	auto add1 = std::make_shared<sw::node::Add>();
	auto add1_b = std::make_shared<sw::node::Vector4>("", sm::vec4(4, 5, 6, 7));
	sw::make_connecting({ add0, 0 }, { add1, sw::node::Add::ID_A });
	sw::make_connecting({ add1_b, 0 }, { add1, sw::node::Add::ID_B });

	auto vert_color_out = std::make_shared<sw::node::ShaderOutput>("v_color", sw::t_col4);
	sw::make_connecting({ add1, 0 }, { vert_color_out, 0 });
	vert_nodes.push_back(vert_color_out);

	// frag
	auto frag_in_col = std::make_shared<sw::node::ShaderInput>("v_color", sw::t_col4);
    auto frag_end = std::make_shared<sw::node::FragmentShader>();
    sw::make_connecting({ frag_in_col, 0 }, { frag_end, 0 });

	// end

	sw::Evaluator vert(vert_nodes);
	sw::Evaluator frag({ frag_end });

	//debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}

TEST_CASE("type conv") {
	init();

	// layout
	init_layout(ST_SHAPE);

	// vert
	std::vector<sw::NodePtr> vert_nodes;
	add_vert_pos_trans(vert_nodes, vert_nodes);

	auto vert_color_in  = std::make_shared<sw::node::ShaderInput>("color", sw::t_col4);

	auto vec3 = std::make_shared<sw::node::Vector3>("", sm::vec3(0.1f, 0.1f, 0.1f));
	auto vert_color_out = std::make_shared<sw::node::ShaderOutput>("v_color", sw::t_col4);
	sw::make_connecting({ vec3, 0 }, { vert_color_out, 0 });
	vert_nodes.push_back(vert_color_out);

	// frag
	auto frag_in_col = std::make_shared<sw::node::ShaderInput>("v_color", sw::t_col4);
    auto frag_end = std::make_shared<sw::node::FragmentShader>();
    sw::make_connecting({ frag_in_col, 0 }, { frag_end, 0 });

	// end

	sw::Evaluator vert(vert_nodes);
	sw::Evaluator frag({ frag_end });

	//debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}

TEST_CASE("phong") {
	init();

	// layout
	init_layout(ST_MODEL);

	// vert
	std::vector<sw::NodePtr> cache_nodes;

	std::vector<sw::NodePtr> vert_nodes;

	auto projection = std::make_shared<sw::node::ShaderUniform>("u_projection", sw::t_mat4);
	auto view       = std::make_shared<sw::node::ShaderUniform>("u_view",       sw::t_mat4);
	auto model      = std::make_shared<sw::node::ShaderUniform>("u_model",      sw::t_mat4);

	auto position = std::make_shared<sw::node::ShaderInput>("position", sw::t_flt4);
	auto normal   = std::make_shared<sw::node::ShaderInput>("normal",   sw::t_nor3);

	auto pos_trans = std::make_shared<sw::node::PositionTrans>(4);
	sw::make_connecting({ projection, 0 }, { pos_trans, sw::node::PositionTrans::ID_PROJ });
	sw::make_connecting({ view, 0 },       { pos_trans, sw::node::PositionTrans::ID_VIEW });
	sw::make_connecting({ model, 0 },      { pos_trans, sw::node::PositionTrans::ID_MODEL });
	sw::make_connecting({ position, 0 },   { pos_trans, sw::node::PositionTrans::ID_POS });
    cache_nodes.push_back(pos_trans);

	auto frag_pos_trans = std::make_shared<sw::node::FragPosTrans>();
	sw::make_connecting({ model, 0 },    { frag_pos_trans, sw::node::FragPosTrans::ID_MODEL });
	sw::make_connecting({ position, 0 }, { frag_pos_trans, sw::node::FragPosTrans::ID_POS });
	vert_nodes.push_back(frag_pos_trans);

	auto norm_trans = std::make_shared<sw::node::NormalTrans>();
	sw::make_connecting({ model, 0 },  { norm_trans, sw::node::NormalTrans::ID_NORMAL_MAT });
	sw::make_connecting({ normal, 0 }, { norm_trans, sw::node::NormalTrans::ID_NORMAL });
	vert_nodes.push_back(norm_trans);

	add_vert_varying(vert_nodes, cache_nodes, "frag_pos", sw::t_flt3);
	add_vert_varying(vert_nodes, cache_nodes, "normal",   sw::t_nor3);
	add_vert_varying(vert_nodes, cache_nodes, "texcoord", sw::t_uv);

    auto vert_end = std::make_shared<sw::node::VertexShader>();
    sw::make_connecting({ pos_trans, 0 }, { vert_end, 0 });
    vert_nodes.push_back(vert_end);

	// frag
	auto phong = std::make_shared<sw::node::Phong>();

	auto frag_in_pos = std::make_shared<sw::node::ShaderInput>("v_frag_pos", sw::t_flt3);
	auto frag_in_nor = std::make_shared<sw::node::ShaderInput>("v_normal",   sw::t_nor3);
	sw::make_connecting({ frag_in_pos, 0 }, { phong, sw::node::Phong::ID_FRAG_POS });
	sw::make_connecting({ frag_in_nor, 0 }, { phong, sw::node::Phong::ID_NORMAL });

	auto mat_diffuse   = std::make_shared<sw::node::Vector3>("mat_diffuse", sm::vec3(0.1f, 0.1f, 0.1f));
	auto mat_specular  = std::make_shared<sw::node::Vector3>("mat_specular", sm::vec3(0.2f, 0.2f, 0.2f));
	auto mat_emission  = std::make_shared<sw::node::Vector3>("mat_emission", sm::vec3(0.3f, 0.3f, 0.3f));
	auto mat_shininess = std::make_shared<sw::node::Vector1>("mat_shininess", 0.4f);
	sw::make_connecting({ mat_diffuse, 0 },   { phong, sw::node::Phong::ID_MAT_DIFFUSE });
	sw::make_connecting({ mat_specular, 0 },  { phong, sw::node::Phong::ID_MAT_SPECULAR });
	sw::make_connecting({ mat_emission, 0 },  { phong, sw::node::Phong::ID_MAT_EMISSION });
	sw::make_connecting({ mat_shininess, 0 }, { phong, sw::node::Phong::ID_MAT_SHININESS });

	auto lit_pos      = std::make_shared<sw::node::Vector3>("lit_pos", sm::vec3(0.5f, 0.5f, 0.5f));
	auto lit_ambient  = std::make_shared<sw::node::Vector1>("lit_ambient", 0.6f);
	auto lit_diffuse  = std::make_shared<sw::node::Vector1>("lit_diffuse", 0.7f);
	auto lit_specular = std::make_shared<sw::node::Vector1>("lit_specular", 0.8f);
	sw::make_connecting({ lit_pos, 0 },      { phong, sw::node::Phong::ID_LIT_POSITION });
	sw::make_connecting({ lit_ambient, 0 },  { phong, sw::node::Phong::ID_LIT_AMBIENT });
	sw::make_connecting({ lit_diffuse, 0 },  { phong, sw::node::Phong::ID_LIT_DIFFUSE });
	sw::make_connecting({ lit_specular, 0 }, { phong, sw::node::Phong::ID_LIT_SPECULAR });

	auto view_pos = std::make_shared<sw::node::Vector3>("view_pos", sm::vec3(0.5f, 0.5f, 0.5f));
	sw::make_connecting({ view_pos, 0 }, { phong, sw::node::Phong::ID_VIEW_POS });

    auto frag_end = std::make_shared<sw::node::FragmentShader>();
    sw::make_connecting({ phong, 0 }, { frag_end, 0 });

	// end
	sw::Evaluator vert(vert_nodes);
	sw::Evaluator frag({ frag_end });

	//debug_print(vert, frag);
	int shader = create_shader(vert, frag);

	REQUIRE(shader != 0);
}