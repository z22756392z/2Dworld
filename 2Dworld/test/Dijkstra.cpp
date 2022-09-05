#include "Dijkstra.h"
#include <queue>
#include "imgui-master/imgui.h"

namespace test {
    void Linear(const glm::vec2& a, const glm::vec2& b, const glm::vec4& color, vector<vertices>& vertices, vector<unsigned int>& indices) {
        int i = vertices.size();
        vertices.resize(vertices.size() + 2);

        vertices[i].position = a;
        vertices[i].color = color;
        vertices[i + 1].position = b;
        vertices[i + 1].color = color;

        indices.push_back(i);
        indices.push_back(i + 1);
    }

	Dijkstra::Dijkstra(){
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        vector<Node> nodes;
        nodes.emplace_back(-0.6, 0.5, 0.1);
        nodes.emplace_back(-0.2, 0.5, 0.1);
        nodes.emplace_back(0.2, 0.5, 0.1);
        nodes.emplace_back(0.6, 0.5, 0.1);
        nodes.emplace_back(-0.6, 0, 0.1);
        nodes.emplace_back(-0.2, 0, 0.1);
        nodes.emplace_back(0.2, 0, 0.1);
        nodes.emplace_back(0.6, 0, 0.1);
        nodes.emplace_back(-0.6, -0.5, 0.1);
        nodes.emplace_back(-0.2, -0.5, 0.1);
        nodes.emplace_back(0.2, -0.5, 0.1);
        nodes.emplace_back(0.6, -0.5, 0.1);
        
        _graph = new Graph(nodes);
        _graph->addEdge(0, 1, 2);
        _graph->addEdge(1, 2, 3);
        _graph->addEdge(2, 3, 1);
        _graph->addEdge(4, 5, 1);
        _graph->addEdge(5, 6, 3);
        _graph->addEdge(6, 7, 3);
        _graph->addEdge(8, 9, 3);
        _graph->addEdge(9, 10, 3);
        _graph->addEdge(10, 11, 1);

        _graph->addEdge(0, 4, 3);
        _graph->addEdge(1, 5, 5);
        _graph->addEdge(2, 6, 2);
        _graph->addEdge(3, 7, 5);
        _graph->addEdge(4, 8, 4);
        _graph->addEdge(5, 9, 2);
        _graph->addEdge(6, 10, 3);
        _graph->addEdge(7, 11, 5);

        vbo = new VertexBuffer();
        vao = new VertexArray();
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(4);
        vao->AddBuffer(*vbo, layout);
        ibo = new IndexBuffer();
        shader = new Shader("res/shaders/Line.shader");

        vao->Unbind();
        vbo->Unbind();
        ibo->Unbind();
        shader->Unbind();
	}

	Dijkstra::~Dijkstra() {

	}

	void Dijkstra::onUpdate() {
        _graph->OnUpdate();
        glm::vec4 color{ 1.0f,0.0f,0.0f,1.0f };

        if (path.size() != 0) {
            for (int i = 1; i < path.size(); i++) {
                Linear(glm::vec2(_graph->nodes[path[i - 1]].x, _graph->nodes[path[i - 1]].y),
                    glm::vec2(_graph->nodes[path[i]].x, _graph->nodes[path[i]].y),
                    color, _vertices,_indices);
            }
            
            vbo->Allocate(_vertices.size() * sizeof(vertices));//allocate memory
            ibo->Allocate(_indices.size());
            GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size() * sizeof(vertices), _vertices.data()));//upload data
            GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _indices.size() * sizeof(unsigned int), _indices.data()));
            vbo->Unbind();
            ibo->Unbind();

            _vertices.clear();
            _indices.clear();
        }
	}

	void Dijkstra::onRender() {
        //GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        _graph->Render();
        if (_graph->dijkstraOpen && path.size() != 0) {
            Renderer renderer;
            renderer.DrawLine(*vao, *ibo, *shader, NULL);
        }
	}

	void Dijkstra::OnImguiRender() {
        _graph->OnImguiRender(path);
	}


    Graph::Graph(vector<Node> Nodes) :
        nodes(Nodes), V(Nodes.size()), 
        graph(vector<vector<int>>(V,vector<int>(V,0))),
        isNodeUIRender(new vector<int>(V,0)),
        nodesOpen(0),isAddOpen(0),dijkstraOpen(0),
        startingNode(0),endNode(V - 1)
    {
        int stride = 8, offset[5] = { 0,0,1,1,0 };
        vector<float> vertices(V * stride * 4);
        vector<unsigned int>indices(V * 6);

        int counter = 0;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < 4; j++) {
                float x = nodes[i].x, y = nodes[i].y, size = nodes[i].size;
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

        vbo = new VertexBuffer(&vertices[0], sizeof(float) * stride * 4 * V);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(4);
        layout.Push<float>(2);
        vao = new VertexArray();
        vao->AddBuffer(*vbo, layout);
        ibo = new IndexBuffer(&indices[0], V * 6);

        shader = new Shader("res/shaders/PosColorTex.shader");
        shader->Bind();

        texture = new Texture("res/textures/circle.png");
        texture->Bind();
        shader->SetUniform1i("u_Texture", 0);

        vao->Unbind();
        vbo->Unbind();
        ibo->Unbind();
        texture->Unbind();
        shader->Unbind();


        lineVbo = new VertexBuffer();
        layout = VertexBufferLayout();
        layout.Push<float>(2);
        layout.Push<float>(4);

        lineVao = new VertexArray();
        lineVao->AddBuffer(*lineVbo, layout);
        lineIbo = new IndexBuffer();
        lineShader = new Shader("res/shaders/Line.shader");

        lineVao->Unbind();
        lineVbo->Unbind();
        lineIbo->Unbind();
        lineShader->Unbind();
    }

    Graph::~Graph() {

    }

    void Graph::addEdge(int u, int v, int w) {
        graph[u][v] = w;
        graph[v][u] = w;
    }

    int Graph::minDistance(vector<int>& dist, vector<bool>& sptSet)
    {
        // Initialize min value
        int min = INT_MAX, min_index;
        for (int i = 0; i < V; i++)
            if (sptSet[i] == false && dist[i] <= min)
                min = dist[i], min_index = i;
        return min_index;
    }

    // Function to print shortest path from source to j using
    // parent array
    void Graph::printPath(vector<int>& parent, int j, vector<int>& path)
    {
        // Base Case : If j is source
        if (parent[j] == -1)
            return;
        printPath(parent, parent[j],path);
        printf("%d " , j);
        path.push_back(j);
    }

    // A utility function to print the constructed distance
    // array
    void Graph::printSolution(vector<int>& dist, int n, vector<int>& parent,vector<vector<int>>& paths)
    {
        int src = 0;
        cout << "Vertex\t Distance\tPath";
        for (int i = 1; i < V; i++) {
            printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i],
                src);
            printPath(parent, i,paths[i]);
        }
    }

    // Function that implements Dijkstra's single source
    // shortest path algorithm for a graph represented using
    // adjacency matrix representation
    int Graph::dijkstra(vector<vector<int>>& graph, int src,int end, vector<int>& path)
    {
        // The output array. dist[i] will hold the shortest
        // distance from src to i
        vector<int> dist(V, INT_MAX);

        // sptSet[i] will true if vertex i is included / in
        // shortest path tree or shortest distance from src to i
        // is finalized
        vector<bool> sptSet(V,false);

        // Parent array to store shortest path tree
        vector<int> parent(V, -1);

        // Distance of source vertex from itself is always 0
        dist[src] = 0;

        // Find shortest path for all vertices
        for (int count = 0; count < V - 1; count++) {
            // Pick the minimum distance vertex from the set of
            // vertices not yet processed. u is always equal to
            // src in first iteration.
            int u = minDistance(dist, sptSet);
            // Mark the picked vertex as processed
            sptSet[u] = true;
            // Update dist value of the adjacent vertices of the
            // picked vertex.
            for (int v = 0; v < V; v++)
                // Update dist[v] only if is not in sptSet,
                // there is an edge from u to v, and total
                // weight of path from src to v through u is
                // smaller than current value of dist[v]
                if (!sptSet[v] && graph[u][v]
                    && dist[u] + graph[u][v] < dist[v]) {
                    parent[v] = u;
                    dist[v] = dist[u] + graph[u][v];
                }
        }
        // print the constructed distance array
        if (dist[end] > 10000 || dist[end] < 0) {
            cout << "Node " << src << " Can't reach " << end << endl;
            return -1;
        }
        else {
            cout << "Node\t Distance\tPath";
            printf("\n%d -> %d \t\t %d\t\t%d ", src, end, dist[end], src);
            path.push_back(src);
            printPath(parent, end, path);
            cout << endl;
            return dist[end];
        }
    }

    bool FindPair(vector<pair<int, int>>& selected, pair<int, int> f) {
        pair<int, int> f2 = make_pair(f.second, f.first);
        for (auto i = selected.begin(); i != selected.end(); i++) {
            if (*i == f || *i == f2) return true;
        }
        return false;
    }
    void Graph::OnUpdate() {
        glm::vec4 color;
        glm::vec4 color1{ 1,1,1,1 };
        glm::vec4 color2{ 1,1,0,1 };
        for (int i = 0; i < V; i++) {
            if (isNodeUIRender->at(i) == 1) {
                color = color2;
            }
            else color = color1;
            for (int j = i; j < V; j++) {
                if (graph[i][j] == 0)    continue;
                if (isNodeUIRender->at(j) == 1) {
                    swap(color, color2);
                    Linear(glm::vec2(nodes[i].x, nodes[i].y), glm::vec2(nodes[j].x, nodes[j].y), color, m_vertices, m_indices);
                    swap(color, color2);
                }
                else {
                    Linear(glm::vec2(nodes[i].x, nodes[i].y), glm::vec2(nodes[j].x, nodes[j].y), color, m_vertices, m_indices);
                }
               
            }
        }


        lineVbo->Allocate(m_vertices.size() * sizeof(vertices));//allocate memory
        lineIbo->Allocate(m_indices.size());
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(vertices), m_vertices.data()));//upload data
        GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(unsigned int), m_indices.data()));
        lineVbo->Unbind();
        lineIbo->Unbind();

        m_vertices.clear();
        m_indices.clear();
    }

    void Graph::Render() {
        Renderer renderer;
        texture->Bind();
        renderer.Draw(*vao, *ibo, *shader);
        renderer.DrawLine(*lineVao, *lineIbo, *lineShader,NULL);
    }

    void Graph::OnImguiRender(vector<int>& path) {
        list<pair<int, int>>::iterator j;
        
        {
            if (ImGui::Button("Nodes")) {
                nodesOpen ^= 1;
            }
            OnImguiRenderNodes();
        }

        {
            if(ImGui::Button("Dijkstra")) {
                dijkstraOpen ^= 1;
            }
            OnImguiRenderDijkstra(path);
        }
    }

    void Graph::OnImguiRenderNodes() {
        if (!nodesOpen) {
            return;
        }
        for (int i = 0; i < V; i++) {
            if (ImGui::Button(("Node " + to_string(i)).c_str())) {
                isNodeUIRender->at(i) ^= 1;
            }
            OnImguiRenderNode(i);
        }
    }

    void Graph::OnImguiRenderNode(int i) {
        if (isNodeUIRender->at(i) != 1) {
            return;
        }
        string currentNode = to_string(i), content = "", connectedNode = "", weight = "", id = "";
        int counter = 0;

        if (ImGui::BeginListBox(("Node " + currentNode).c_str(), ImVec2(700, 200))) {
            for (int j = 0; j < V; j++) {
                if (graph[i][j] == 0)    continue;

                connectedNode = to_string(j);
                weight = to_string(graph[i][j]);
                id = to_string(counter);
                
                int temp = j;
                content = currentNode + " Connect to " + connectedNode;
                if (ImGui::SliderInt(content.c_str(), &temp, 0, V-1, to_string( temp).c_str())) {
                    swap(graph[i][j], graph[i][temp]);
                    graph[i][j] = 0;
                    graph[j][i] = 0;
                    graph[temp][i] = graph[i][temp];
                }
                content = "Weight: " + weight + "###"+currentNode+connectedNode;
                if(ImGui::SliderInt(content.c_str(), &graph[i][j], 1, 99, weight.c_str())){
                    graph[j][i] = graph[i][j];
                }

                content = "Delete connection : " + currentNode + " to " + connectedNode + "###" + currentNode + connectedNode;
                if (ImGui::Button(content.c_str())) {
                    graph[i][j] = 0;
                    graph[j][i] = 0;
                }
                counter++;
            }

            if (ImGui::Button("Add")) {
                isAddOpen ^= 1;
                toAddIndex = 0;
                toAddWeight = 99;
            }
            if (isAddOpen) {
                ImGui::SliderInt("Connect to ", &toAddIndex, 0, V - 1, to_string(toAddIndex).c_str());
                ImGui::SliderInt("Weight is ", &toAddWeight, 1, 99, to_string(toAddWeight).c_str());
                if (ImGui::Button("Compelete")) {
                    addEdge(i, toAddIndex, toAddWeight);
                    isAddOpen = 0;
                }
            }
            ImGui::EndListBox();
        }
    }

    void Graph::OnImguiRenderDijkstra(vector<int>& path) {
        if (!dijkstraOpen)   return;

        ImGui::SliderInt("Starting node: ", &startingNode, 0, V - 1, to_string(startingNode).c_str());
        ImGui::SliderInt("End node: ", &endNode, 0, V - 1, to_string(endNode).c_str());
        if (ImGui::Button("Start")) {
            path.clear();
            totalCost = dijkstra(graph, startingNode, endNode, path);
        }
        string content;
        if (path.size() == 0 && totalCost == -1) {
            content = "Node " + to_string(startingNode) + " Can't reach " + to_string(endNode);
            ImGui::Text(content.c_str());
        }
        else if(path.size() != 0){
            content = "Node\t Distance\tPath\n" + to_string(startingNode) + " -> " + to_string(endNode) + " \t \t " + to_string(totalCost) + "\t\t";
            for (int num : path) {
                content += " " + to_string(num);
            }
            ImGui::Text(content.c_str());
        }
    }
}