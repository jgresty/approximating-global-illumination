#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vertPos[];
in vec3 vertNorm[];
in vec2 texCoord[];

out vec3 pos;
out vec3 norm;
out vec2 tex;

flat out int axis; // x=0, y=1, z=2
flat out vec4 bounds;

uniform int gridSize;

uniform mat4 xproj;
uniform mat4 yproj;
uniform mat4 zproj;

void main() {

    // find maximum projection axis
    mat4 projection;

    vec3 faceNormal = normalize(cross(vertPos[1]-vertPos[0], vertPos[2]-vertPos[0]));
    float sizex = abs(faceNormal.x);
    float sizey = abs(faceNormal.y);
    float sizez = abs(faceNormal.z);

    if (sizex > sizey && sizex > sizez) {
        projection = xproj;
        axis = 0;
    } else if (sizey > sizez) {
        projection = yproj;
        axis = 1;
    } else {
        projection = zproj;
        axis = 2;
    }

    vec4 vert[3];
    vert[0] = projection * gl_in[0].gl_Position;
    vert[1] = projection * gl_in[1].gl_Position;
    vert[2] = projection * gl_in[2].gl_Position;

    vec4 boundsbox;

    boundsbox.xy = min(min(vert[0].xy, vert[1].xy), vert[2].xy);
    boundsbox.zw = max(max(vert[0].xy, vert[1].xy), vert[2].xy);

    float displacement = 1.0/gridSize;
    boundsbox.xy -= displacement;
    boundsbox.zw += displacement;

    bounds = boundsbox;

    gl_Position = vert[0];
    pos = vert[0].xyz;
    norm = vertNorm[0];
    tex = texCoord[0];
    EmitVertex();

    gl_Position = vert[1];
    pos = vert[1].xyz;
    norm = vertNorm[1];
    tex = texCoord[1];
    EmitVertex();

    gl_Position = vert[2];
    pos = vert[2].xyz;
    norm = vertNorm[2];
    tex = texCoord[2];
    EmitVertex();

    EndPrimitive();
}
