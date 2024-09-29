#version 330 core
layout(triangles) in;          // Input primitive type (triangles)
layout(triangle_strip, max_vertices = 3) out; // Output type

// Pass through
in vec2 fragTexCoord;
in vec4 fragColor; 


void main()
{
    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position; // Pass through vertex position
        EmitVertex(); // Emit the vertex
    }
    EndPrimitive(); // End the primitive (triangle)
}