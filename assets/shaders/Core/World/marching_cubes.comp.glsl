#version 430 core
layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(binding = 0, r32f) uniform readonly image3D density_tex;
layout(binding = 1) uniform usampler1D edge_table;
layout(binding = 2) uniform isampler2D tri_table;

layout(std430, binding = 0) buffer vertex_buffer {
    vec4 vertices[];
};
layout(std430, binding = 1) buffer index_buffer {
    uint indices[];
};

layout(binding = 0, offset = 0) uniform atomic_uint triangle_count;

uniform float isolevel;
uniform int chunk_size;

// https://paulbourke.net/geometry/polygonise/
const ivec3 corner_offsets[8] = ivec3[8](
    ivec3(0, 0, 0), ivec3(1, 0, 0),
    ivec3(1, 0, 1), ivec3(0, 0, 1),
    ivec3(0, 1, 0), ivec3(1, 1, 0),
    ivec3(1, 1, 1), ivec3(0, 1, 1)
);

const int edge_corners[12][2] = {
    {0, 1}, {1, 3},
    {3, 2}, {2, 0},
    {4, 5}, {5, 7},
    {7, 6}, {6, 4},
    {0, 4}, {1, 5},
    {3, 7}, {2, 6},
};

// https://github.com/SebLague/Marching-Cubes/blob/master/Assets/Scripts/Compute/Includes/MarchTables.compute
const int cornerIndexAFromEdge[12] = {
    0, 1, 2, 3, 4, 5,
    6, 7, 0, 1, 2, 3
};

const int cornerIndexBFromEdge[12] = {
    1, 2, 3, 0, 5, 6,
    7, 4, 4, 5, 6, 7
};


vec3 interpolateVerts(vec4 v1, vec4 v2) {
    float t = (isolevel - v1.w) / (v2.w - v1.w);
    return v1.xyz + t * (v2.xyz-v1.xyz);
}


void main() {
    if (gl_GlobalInvocationID.x >= (chunk_size - 1) ||
        gl_GlobalInvocationID.y >= (chunk_size - 1) ||
        gl_GlobalInvocationID.z >= (chunk_size - 1)) {

        return;
    }

    ivec3 cube_pos = ivec3(gl_GlobalInvocationID);
    vec4 cube_corners[8];

    for (int i = 0; i < 8; i++){
        cube_corners[i] = vec4(cube_pos + corner_offsets[i], imageLoad(density_tex, (cube_pos + corner_offsets[i])).r );
    }

    int cube_index = 0;
    if (cube_corners[0].w < isolevel) cube_index |= 1;
    if (cube_corners[1].w < isolevel) cube_index |= 2;
    if (cube_corners[2].w < isolevel) cube_index |= 4;
    if (cube_corners[3].w < isolevel) cube_index |= 8;
    if (cube_corners[4].w < isolevel) cube_index |= 16;
    if (cube_corners[5].w < isolevel) cube_index |= 32;
    if (cube_corners[6].w < isolevel) cube_index |= 64;
    if (cube_corners[7].w < isolevel) cube_index |= 128;

    uint edge_mask = texelFetch(edge_table, int(cube_index), 0).r;
    if (edge_mask == 0u) return;

    for (int i = 0; texelFetch(tri_table, ivec2(i, cube_index),0).r != -1; i +=3) {
        int a0 = cornerIndexAFromEdge[texelFetch(tri_table, ivec2(i, cube_index), 0 ).r];
        int b0 = cornerIndexBFromEdge[texelFetch(tri_table, ivec2(i, cube_index), 0 ).r];

        int a1 = cornerIndexAFromEdge[texelFetch(tri_table, ivec2(i + 1, cube_index), 0 ).r];
        int b1 = cornerIndexBFromEdge[texelFetch(tri_table, ivec2(i + 1, cube_index), 0 ).r];

        int a2 = cornerIndexAFromEdge[texelFetch(tri_table, ivec2(i + 2, cube_index), 0 ).r];
        int b2 = cornerIndexBFromEdge[texelFetch(tri_table, ivec2(i + 2, cube_index), 0 ).r];

        uint triangle_idx = atomicCounterIncrement(triangle_count);
        uint vertex_offset = triangle_idx * 3u;

        vertices[vertex_offset + 0u] = vec4(interpolateVerts(cube_corners[a0], cube_corners[b0]),1.0);
        vertices[vertex_offset + 1u] = vec4(interpolateVerts(cube_corners[a1], cube_corners[b1]),1.0);
        vertices[vertex_offset + 2u] = vec4(interpolateVerts(cube_corners[a2], cube_corners[b2]),1.0);

        indices[vertex_offset + 0u] = vertex_offset + 0u;
        indices[vertex_offset + 1u] = vertex_offset + 1u;
        indices[vertex_offset + 2u] = vertex_offset + 2u;
    }
}

