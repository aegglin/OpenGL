#pragma once
#include "Test.h"

namespace test {
	class TestClearColor : public Test {
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float clearColor[4]; // array of RGBA on the stack (doesn't need to be destroyed manually)
	};
}