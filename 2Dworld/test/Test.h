#pragma once
#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace test {
	class Test {
	public:
		Test() {};
		virtual ~Test() {};

		virtual void onUpdate() {};
		virtual void onRender() {};
		virtual void OnImguiRender() {};
	};

	class TestMenu : public Test
	{
	private: 
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
		Test*& m_CurrnetTest;
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImguiRender() override;
		
		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	};
}