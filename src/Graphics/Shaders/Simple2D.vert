// Copyright (c) 2010-present Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

uniform mat4 mvp_matrix;

attribute vec4 color;
attribute vec2 vertex;

varying lowp vec4 v_color;

void main()
{
    v_color = color;
    gl_Position = mvp_matrix * vec4(vertex, 0.0, 1.0);
}
