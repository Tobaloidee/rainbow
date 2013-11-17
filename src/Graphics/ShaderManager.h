// Copyright (c) 2010-14 Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#ifndef GRAPHICS_SHADERMANAGER_H_
#define GRAPHICS_SHADERMANAGER_H_

#include "Common/Vector.h"
#include "Graphics/OpenGL.h"

namespace Renderer
{
	bool init();
	void release();
}

namespace Shader
{
	enum Attribute
	{
		VERTEX,
		COLOR,
		TEXCOORD
	};

	enum Type
	{
		FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		VERTEX_SHADER = GL_VERTEX_SHADER,
	#ifdef GL_GEOMETRY_SHADER
		GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
	#endif
		INVALID = -1
	};
}

class ShaderManager : private NonCopyable<ShaderManager>
{
	friend bool Renderer::init();
	friend void Renderer::release();

public:
	static ShaderManager *Instance;

	/// Returns the real program id.
	inline unsigned int get_program(const unsigned int pid) const;

	/// Creates program.
	/// \param shaders  Shader ids to attach.
	/// \param count    Number of shader ids.
	/// \return Unique program identifier.
	int create_program(const int *shaders, const size_t count);

	/// Creates shader.
	/// \param type    Shader type.
	/// \param source  Shader source code.
	/// \return Unique shader identifier.
	int create_shader(int type, const char *source);

	/// Resets to default shaders.
	inline void reset();

	/// Sets viewport.
	void set(const float width, const float height);

	/// Sets orthographic projection.
	void set_projection(const float left, const float right, const float bottom, const float top);

	/// Activates program.
	void use(const int program);

	inline operator bool() const;

private:
	struct Program
	{
		unsigned int id;
	} active;  ///< Currently active program.

	Vector<unsigned int> shaders;
	Vector<Program> programs;

	float ortho[16];

	ShaderManager(const char **shaders = nullptr, const size_t count = 0);
	~ShaderManager();
};

unsigned int ShaderManager::get_program(const unsigned int pid) const
{
	return this->programs[pid].id;
}

void ShaderManager::reset()
{
	this->use(0);
}

ShaderManager::operator bool() const
{
	return this->programs.size();
}

#endif
