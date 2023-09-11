#pragma once

#include "base/ecs/System.hpp"
#include "Graphics/Shader.hpp"
#include <memory>


class Event;


class RenderSystem : public System
{
public:
	void Init();

	void Update(float dt);

private:
	void WindowSizeListener(Event& event);

	std::unique_ptr<Shader> shader;

	Entity mCamera;

	GLuint mVao{};
	GLuint mVboVertices{};
	GLuint mVboNormals{};
};