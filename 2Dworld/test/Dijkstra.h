#pragma once
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Vertex.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include <vector>
#include <math.h>
#include <string>
#include <set>
using namespace std;



namespace test {
	#define MAXNODE 100
	#define INF 0x3f3f3f3f // The distance to other vertices is initialized as infinite  
	// iPair ==> Integer Pair  
	typedef pair<int, int> iPair;
	class Node {
	public:
		float x, y,size;
		Node(float x ,float y, float size):x(x),y(y),size(size){}
	};
	struct vertices {
		glm::vec2 position;
		glm::vec4 color;
	};
	class Graph // Graph structure  
	{
		int V; // No. of vertices in the graph
		vector<vector<int>> graph;
		vector<int>* isNodeUIRender;
		int nodesOpen;
		int isAddOpen;
		int startingNode;
		int endNode;
		int toAddIndex;
		int toAddWeight;
		int totalCost;
		vector<vertices> m_vertices;
		vector<unsigned int> m_indices;
		VertexArray* vao;
		VertexBuffer* vbo;
		IndexBuffer* ibo;
		Shader* shader;
		VertexArray* lineVao;
		VertexBuffer* lineVbo;
		IndexBuffer* lineIbo;
		Shader* lineShader;
		Texture* texture;
		int minDistance(vector<int>& dist, vector<bool>& sptSet);
		void printPath(vector<int>& parent, int j, vector<int>& path);
		
		void OnImguiRenderNodes();
		void OnImguiRenderNode(int i);
		void OnImguiRenderDijkstra(vector<int>& path);
	public:
		// Constructor that accept number of vertices in graph  
		Graph(vector<Node> Nodes); // allocate the vertex memory
		~Graph();
		void addEdge(int u, int v, int w); // add edges in the graph
		void printSolution(vector<int>& dist, int n, vector<int>& parent,vector<vector<int>>& paths);
		// prints shortest path from s  
		int dijkstra(vector<vector<int>>& graph, int src,int end, vector<int>& path);

		void OnUpdate();
		void Render();
		void OnImguiRender(vector<int>& path);

		vector<Node> nodes;
		int dijkstraOpen;
	};
	class Dijkstra:public Test
	{

	public:
		Dijkstra();
		~Dijkstra();

		void onUpdate();
		void onRender();
		void OnImguiRender();
	
	private:
		Graph* _graph;
		vector<int> path;
		VertexArray* vao;
		VertexBuffer* vbo;
		IndexBuffer* ibo;
		Shader* shader;
		vector<vertices> _vertices;
		vector<unsigned int> _indices;
	};

};