
/////////////////////
// INPUT VARIABLES //
/////////////////////
in lowp vec3 inputPosition;
in lowp vec3 normal;
in lowp vec4 boneIndices; // 4 incices for 4 bones control
in lowp vec4 boneWeights; // 4 weights(influences) for one bone.

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
smooth out vec2 texCoord;
smooth out vec3 outNormal;

uniform SceneMatrices
{
	uniform mat4 projectionMatrix;
} sm;

uniform mat4 modelViewMatrix;

uniform SkinningMatrices
{
	uniform mat4 mat[64];	//16 floats (16 * 4 = 64)
} skin;



////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////	
void main(void)
{
	vec4 pos = vec4(0,0,0,1);
	for (int i = 0; i < 4; ++i)
	{
		pos += boneWeights[i] * ( vec4(inputPosition, 1.0f) * skin.mat[int(boneIndices[i])]);
	}

	// vec4 pos = boneWeights[0] * (skin.mat[int(boneIndices[0])] * vec4(inputPosition, 1.0f));


	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = sm.projectionMatrix * (modelViewMatrix * vec4(pos.xyz, 1.0f));
	outNormal = normalize(mat3(modelViewMatrix) * normal);
};