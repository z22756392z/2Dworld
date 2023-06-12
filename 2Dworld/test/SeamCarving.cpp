#include "SeamCarving.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include <imgui-master/imgui.h>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace test {
    SeamCarving::SeamCarving()
        : m_va(), m_vb(nullptr), m_ib(nullptr), m_shader(nullptr),
        m_threshold(0.0f), m_seam_carve_number(0), m_buffer(nullptr),
        m_width(0), m_height(0), m_isEnergyMap(true)
    {
        float position[] = {
            0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3,
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_vb = new VertexBuffer(position, sizeof(float) * 4 * 4);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_va = new VertexArray();
        m_va->AddBuffer(*m_vb, layout);
        m_ib = new IndexBuffer(indices, 6);

        m_shader = new Shader("res/shaders/SeamCarving.shader");
        m_shader->Bind();

        m_texture = new Texture("res/textures/squirrel.jpg");
        m_width = m_texture->GetWidth();
        m_height = m_texture->GetHeight();
        m_buffer = new unsigned char[m_width * m_height * 4];
        std::memcpy(m_buffer, m_texture->GetImageBuffer(), m_width * m_height * 4);

        CalculateEnergyMap();
        DrawTexture();
        
        m_texture->Bind();
        m_shader->SetUniform1i("u_Texture", 0);

        m_va->Unbind();
        m_vb->Unbind();
        m_ib->Unbind();
        m_texture->Unbind();
        m_shader->Unbind();
    }

    SeamCarving::~SeamCarving()
    {
        delete[] m_buffer;
    }

    void SeamCarving::onUpdate()
    {
        float dx = static_cast<float>(m_width) / static_cast<float>(m_texture->GetWidth());


        float position[] = {
            0.0f, 0.0f, 0.0f, 0.0f,
            dx, 0.0f, 1.0f, 0.0f,
            dx, 1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f
        };

        m_vb->Bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 4 * 4, position));//upload data
        m_vb->Unbind();
    }

    void SeamCarving::onRender()
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer renderer;
        m_texture->Bind();
        renderer.Draw(*m_va, *m_ib, *m_shader);
    }

    void SeamCarving::OnImguiRender()
    {
        if (ImGui::SliderInt("Seam Carving Number:", &m_seam_carve_number, 0, m_texture->GetWidth())) {
            if (m_texture->GetWidth() - m_width > m_seam_carve_number) {
                m_width = m_texture->GetWidth();
                m_height = m_texture->GetHeight();
                std::memcpy(m_buffer, m_texture->GetImageBuffer(), m_width * m_height * 4);
            }
            
            CalculateEnergyMap();
            RemoveSeams();
            DrawTexture();
        }

        if (ImGui::Button(m_isEnergyMap ? "Energy map" : "Seam Carving")) {
            m_isEnergyMap = !m_isEnergyMap;
            DrawTexture();
        }
    }

    void SeamCarving::CalculateEnergyMap()
    {
        m_energyMap.resize(m_height, std::vector<float>(m_width, 0.0f));

        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                m_energyMap[y][x] = CalculatePixelEnergy(x, y);
            }
        }
    }

    float SeamCarving::CalculatePixelEnergy(const int x, const int y)
    {
        glm::vec4 luminance(0.2126f, 0.7152f, 0.0722f,1);

        float left = glm::dot(GetBufferColor(x - 1, y), luminance);
        float right = glm::dot(GetBufferColor(x + 1, y), luminance);
        float top = glm::dot(GetBufferColor(x, y - 1), luminance);
        float bottom = glm::dot(GetBufferColor(x, y + 1), luminance);

        float gx = (right - left) * 0.5f;
        float gy = (bottom - top) * 0.5f;

        float energy = std::sqrt(gx * gx + gy * gy);

        return energy;
    }

    void SeamCarving::RemoveSeams()
    {
        std::vector<int> seam;
        for (int i = m_texture->GetWidth() - m_width; i < m_seam_carve_number; i++) {
            seam = FindLowestEnergySeam();
            RemoveSeamFromImage(seam);
            UpdateEnergyMap(seam);
        }
    }

    std::vector<int> SeamCarving::FindLowestEnergySeam()
    {
        std::vector<int> seamIndices;
        std::vector<std::vector<float>> dp(m_height, std::vector<float>(m_width, 0.0f));

        // Copy the first row of energy map to dynamic programming table
        std::copy(m_energyMap[0].begin(), m_energyMap[0].end() - (m_texture->GetWidth() - m_width), dp[0].begin());

        // Calculate the dynamic programming table
        for (int y = 1; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                float left = (x == 0) ? INT_MAX : dp[y - 1][x - 1];
                float middle = dp[y - 1][x];
                float right = (x == m_width - 1) ? INT_MAX : dp[y - 1][x + 1];

                dp[y][x] = m_energyMap[y][x] + std::min({ left, middle, right });
            }
        }

        // Find the minimum energy path in the last row
        int minEnergyIndex = std::min_element(dp[m_height - 1].begin(), dp[m_height - 1].end()) - dp[m_height - 1].begin();

        // Backtrack to find the seam indices
        seamIndices.push_back(minEnergyIndex);
        for (int y = m_height - 2; y >= 0; y--) {
            float currEnergy = dp[y][minEnergyIndex];

            int leftIndex = (minEnergyIndex > 0) ? minEnergyIndex - 1 : minEnergyIndex;
            int rightIndex = (minEnergyIndex < m_width - 1) ? minEnergyIndex + 1 : minEnergyIndex;

            float leftEnergy = dp[y][leftIndex];
            float rightEnergy = dp[y][rightIndex];

            if (leftEnergy < currEnergy) {
                minEnergyIndex = leftIndex;
            }
            else if (rightEnergy < currEnergy) {
                minEnergyIndex = rightIndex;
            }

            seamIndices.push_back(minEnergyIndex);
        }

        std::reverse(seamIndices.begin(), seamIndices.end());
        return seamIndices;
    }

    void SeamCarving::RemoveSeamFromImage(const std::vector<int>& seam)
    {
        std::vector<std::vector<unsigned char>> buffer(m_height, std::vector<unsigned char>(4 * (m_width - 1)));

        for (int y = 0; y < m_height; y++) {
            int seamIndex = seam[y];
            std::memcpy(buffer[y].data(), m_buffer + (y * m_width + 0) * 4, seamIndex * 4);
            std::memcpy(buffer[y].data() + seamIndex * 4, m_buffer + (y * m_width + seamIndex + 1) * 4, (m_width - seamIndex - 1) * 4);
        }
        
        m_width--;

        for (int y = 0; y < m_height; y++) {
            std::memcpy(m_buffer + (y * m_width) * 4, buffer[y].data(), m_width * 4);
        }
    }



    void SeamCarving::UpdateEnergyMap(const std::vector<int>& seam)
    {
        for (int y = 0; y < m_height; y++) {
            int seamIndex = seam[y];

            // Shift the remaining energies
            std::copy(m_energyMap[y].begin() + seamIndex + 1, m_energyMap[y].end(), m_energyMap[y].begin() + seamIndex);

            // Calculate energy for the new pixel
            if (seamIndex > 0) {
                m_energyMap[y][seamIndex - 1] = CalculatePixelEnergy(seamIndex - 1, y);
            }
            if (seamIndex < m_width) {
                m_energyMap[y][seamIndex] = CalculatePixelEnergy(seamIndex, y);
            }
        }
    }
    void SeamCarving::DrawTexture() {
        if (m_isEnergyMap) {
            DrawEnergyMap();
        }
        else {
            std::vector<int> seam = FindLowestEnergySeam();
            DrawNextSeam(seam);
        }
    }

    void SeamCarving::DrawNextSeam(const std::vector<int>& seam) {
        std::vector<glm::vec4> originalColor(m_height, glm::vec4(0,0,0,0));
        for (int y = 0; y < m_height; y++) {
            int seamIndex = seam[y];
            originalColor[y] = GetBufferColor(seamIndex, y);
            SetBufferColor(seamIndex, y, glm::vec4(0.5f, 0.5f, 0, 1));

        }
        m_texture->SubBuffer(m_width, m_height, m_buffer);
        for (int y = 0; y < m_height; y++) {
            int seamIndex = seam[y];
            SetBufferColor(seamIndex, y, originalColor[y]);
        }
    }

    void SeamCarving::DrawEnergyMap() {
        std::vector<std::vector<unsigned char>> buffer(m_height, std::vector<unsigned char>(4 * m_width));

        for (int y = 0; y < m_height; y++) {
            std::memcpy(buffer[y].data(), m_buffer + (y * m_width) * 4, m_width * 4);
        }

        float maxEnergy = 1;
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                float value = m_energyMap[y][x] / maxEnergy;
                SetBufferColor(x, y, glm::vec4(value, value, value, value));
            }
        }

        m_texture->SubBuffer(m_width, m_height, m_buffer);

        for (int y = 0; y < m_height; y++) {
            std::memcpy(m_buffer + (y * m_width) * 4, buffer[y].data(), m_width * 4);
        }
    }

    void SeamCarving::SetBufferColor(const int x, const int y, const glm::vec4& color)
    {
        if (CheckBound(x, y)) {
            return;
        }

        int index = (y * m_width + x) * 4;
        m_buffer[index + 0] = static_cast<unsigned char>(color.r * 255);
        m_buffer[index + 1] = static_cast<unsigned char>(color.g * 255);
        m_buffer[index + 2] = static_cast<unsigned char>(color.b * 255);
        m_buffer[index + 3] = static_cast<unsigned char>(color.a * 255);
    }

    glm::vec4 SeamCarving::GetBufferColor(const int x, const int y) const
    {
        if (CheckBound(x, y)) {
            return glm::vec4(0,0,0,0);
        }
        int index = (y * m_width + x) * 4;
        float r = static_cast<float>(m_buffer[index + 0]) / 255.0f;
        float g = static_cast<float>(m_buffer[index + 1]) / 255.0f;
        float b = static_cast<float>(m_buffer[index + 2]) / 255.0f;
        float a = static_cast<float>(m_buffer[index + 3]) / 255.0f;
        return glm::vec4(r, g, b, a);
    }


    bool SeamCarving::CheckBound(const int x, const int y) const {
        return (x < 0 || x >= m_width || y < 0 || y >= m_height);
    }
}
