#include "Traveling_salesman_problem.h"
#include "imgui-master/imgui.h"
#include <fstream>
#include <sstream>
#include <math.h>

test::Traveling_salesman_problem::Traveling_salesman_problem()
{
}

test::Traveling_salesman_problem::~Traveling_salesman_problem()
{
    _city_coordiates = ParseData("res/test/Traveling_salesman_data.txt");


    int stride = 8, offset[5] = { 0,0,1,1,0 };
    std::vector<float> vertices(_city_coordiates.size() * stride * 4);
    std::vector<unsigned int>indices(_city_coordiates.size() * 6);

    int counter = 0;
    float scale = 0.0001f;
    for (int i = 0; i < _city_coordiates.size(); i++) {
        for (int j = 0; j < 4; j++) {
            float x = _city_coordiates[i].first * scale, y = _city_coordiates[i].second * scale, size = 0.005;
            vertices[i * stride * 4 + stride * j] = x + offset[j] * size;
            vertices[i * stride * 4 + stride * j + 1] = y + offset[j + 1] * size;
            vertices[i * stride * 4 + stride * j + 2] = 1;
            vertices[i * stride * 4 + stride * j + 3] = 1;
            vertices[i * stride * 4 + stride * j + 4] = 1;
            vertices[i * stride * 4 + stride * j + 5] = 1;
            vertices[i * stride * 4 + stride * j + 6] = offset[j];
            vertices[i * stride * 4 + stride * j + 7] = offset[j + 1];
        }
        indices[i * 6] = counter;
        indices[i * 6 + 1] = counter + 1;
        indices[i * 6 + 2] = counter + 2;
        indices[i * 6 + 3] = counter;
        indices[i * 6 + 4] = counter + 2;
        indices[i * 6 + 5] = counter + 3;
        counter += 4;
    }


    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    _vbo = new VertexBuffer(&vertices[0], sizeof(float) * stride * 4 * _city_coordiates.size());
    _vao = new VertexArray();
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(4);
    layout.Push<float>(2);
    _vao->AddBuffer(*_vbo, layout);
    _ibo = new IndexBuffer(&indices[0], _city_coordiates.size() * 6);
    _shader = new Shader("res/shaders/PosColorTex.shader");
    _shader->Bind();

    _texture = new Texture("res/textures/circle.png");
    _texture->Bind();
    _shader->SetUniform1i("u_Texture", 0);
}

void test::Traveling_salesman_problem::onUpdate()
{
}

void test::Traveling_salesman_problem::onRender()
{
    Renderer renderer;
    _shader->Bind();
    _texture->Bind();
    renderer.Draw(*_vao, *_ibo, *_shader);
}

void test::Traveling_salesman_problem::OnImguiRender()
{
}

float test::Traveling_salesman_problem::distance(const std::pair<int, int> a, const std::pair<int, int> b)
{
    float dx = a.first - b.first , dy = a.second - b.second;
    
    return pow(dx * dx + dy * dy, 0.5f);
}

std::vector<std::pair<int, int>> test::Traveling_salesman_problem::ParseData(const std::string filePath)
{
    std::ifstream stream(filePath);

    std::string line;
    std::vector<std::pair<int,int>> city_coordinates;
    while (getline(stream, line)) {
        std::istringstream iss(line);
        int n,n2;

        while (iss >> n >> n2)
        {
            city_coordinates.emplace_back(n, n2);
        }
    }
    return std::vector<std::pair<int, int>>();
}
