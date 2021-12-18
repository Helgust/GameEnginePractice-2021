#if 0
	***	hlms_uv_count0	2
	***	hlms_pose	0
	***	uv_emissive	0
	***	first_valid_detail_map_nm	4
	***	fresnel_scalar	0
	***	hlms_uv_count	1
	***	uv_detail_nm1	0
	***	hlms_lights_spotparams	0
	***	uv_detail_nm2	0
	***	BRDF_Default	1
	***	uv_specular	0
	***	hlms_forwardplus_flipY	0
	***	glsl	635204550
	***	hlms_high_quality	0
	***	hlms_lights_directional	0
	***	samplerStateStart	1
	***	alpha_test	0
	***	GGX_height_correlated	1
	***	glsles	1070293233
	***	metallic_workflow	0
	***	hw_gamma_write	1
	***	uv_detail_weight	0
	***	cubemaps_as_diffuse_gi	1
	***	uv_detail_nm3	0
	***	hlms_pose_normals	0
	***	hlms_pose_half	0
	***	hlms_lights_spot	1
	***	GL_ARB_shading_language_420pack	1
	***	uv_normal	0
	***	hlms_alphablend	0
	***	uv_detail3	0
	***	normal_map	0
	***	uv_detail1	0
	***	fresnel_workflow	0
	***	perceptual_roughness	1
	***	alpha_test_shadow_caster_only	0
	***	materials_per_buffer	240
	***	hw_gamma_read	1
	***	hlms_skeleton	0
	***	syntax	635204550
	***	hlms_lights_point	1
	***	metal	-1698855755
	***	GL_ARB_base_instance	1
	***	s_lights_directional_non_caster	1
	***	uv_detail0	0
	***	uv_diffuse	0
	***	uv_detail_nm0	0
	***	GL3+	450
	***	hlms_lights_attenuation	0
	***	hlms_render_depth_only	0
	***	uv_detail2	0
	***	hlms_qtangent	1
	***	uv_roughness	0
	***	receive_shadows	1
	***	needs_view_dir	1
	***	use_planar_reflections	0
	***	hlsl	-334286542
	***	normal_weight	0
	***	GL_ARB_texture_buffer_range	1
	DONE DUMPING PROPERTIES
	DONE DUMPING PIECES
#endif


    #version 430 core





    #extension GL_ARB_shading_language_420pack: require
    #define layout_constbuffer(x) layout( std140, x )





	#define bufferFetch texelFetch
	#define structuredBufferFetch texelFetch



	#define min3( a, b, c ) min( a, min( b, c ) )
	#define max3( a, b, c ) max( a, max( b, c ) )


#define float2 vec2
#define float3 vec3
#define float4 vec4

#define int2 ivec2
#define int3 ivec3
#define int4 ivec4

#define uint2 uvec2
#define uint3 uvec3
#define uint4 uvec4

#define float2x2 mat2
#define float3x3 mat3
#define float4x4 mat4
#define ogre_float4x3 mat3x4

#define ushort uint
#define ushort3 uint3
#define ushort4 uint4

//Short used for read operations. It's an int in GLSL & HLSL. An ushort in Metal
#define rshort int
#define rshort2 int2
#define rint int
//Short used for write operations. It's an int in GLSL. An ushort in HLSL & Metal
#define wshort2 int2
#define wshort3 int3

#define toFloat3x3( x ) mat3( x )
#define buildFloat3x3( row0, row1, row2 ) mat3( row0, row1, row2 )

#define mul( x, y ) ((x) * (y))
#define saturate(x) clamp( (x), 0.0, 1.0 )
#define lerp mix
#define rsqrt inversesqrt
#define INLINE
#define NO_INTERPOLATION_PREFIX flat
#define NO_INTERPOLATION_SUFFIX

#define finalDrawId drawId
#define PARAMS_ARG_DECL
#define PARAMS_ARG

#define inVs_vertexId gl_VertexID
#define inVs_vertex vertex
#define inVs_blendWeights blendWeights
#define inVs_blendIndices blendIndices
#define inVs_qtangent qtangent


	#define inVs_drawId drawId



	#define inVs_uv0 uv0

#define outVs_Position gl_Position
#define outVs_viewportIndex gl_ViewportIndex
#define outVs_clipDistance0 gl_ClipDistance[0]

#define gl_SampleMaskIn0 gl_SampleMaskIn[0]
#define reversebits bitfieldReverse

#define outPs_colour0 outColour
#define OGRE_Sample( tex, sampler, uv ) texture( tex, uv )
#define OGRE_SampleLevel( tex, sampler, uv, lod ) textureLod( tex, uv, lod )
#define OGRE_SampleArray2D( tex, sampler, uv, arrayIdx ) texture( tex, vec3( uv, arrayIdx ) )
#define OGRE_SampleArray2DLevel( tex, sampler, uv, arrayIdx, lod ) textureLod( tex, vec3( uv, arrayIdx ), lod )
#define OGRE_SampleArrayCubeLevel( tex, sampler, uv, arrayIdx, lod ) textureLod( tex, vec4( uv, arrayIdx ), lod )
#define OGRE_SampleGrad( tex, sampler, uv, ddx, ddy ) textureGrad( tex, uv, ddx, ddy )
#define OGRE_SampleArray2DGrad( tex, sampler, uv, arrayIdx, ddx, ddy ) textureGrad( tex, vec3( uv, arrayIdx ), ddx, ddy )
#define OGRE_ddx( val ) dFdx( val )
#define OGRE_ddy( val ) dFdy( val )
#define OGRE_Load2D( tex, iuv, lod ) texelFetch( tex, iuv, lod )
#define OGRE_LoadArray2D( tex, iuv, arrayIdx, lod ) texelFetch( tex, ivec3( iuv, arrayIdx ), lod )
#define OGRE_Load2DMS( tex, iuv, subsample ) texelFetch( tex, iuv, subsample )

#define OGRE_Load3D( tex, iuv, lod ) texelFetch( tex, ivec3( iuv ), lod )

#define bufferFetch1( buffer, idx ) texelFetch( buffer, idx ).x

#define OGRE_SAMPLER_ARG_DECL( samplerName )
#define OGRE_SAMPLER_ARG( samplerName )

#define OGRE_Texture3D_float4 sampler3D

#define CONST_BUFFER( bufferName, bindingPoint ) layout_constbuffer(binding = bindingPoint) uniform bufferName
#define CONST_BUFFER_STRUCT_BEGIN( structName, bindingPoint ) layout_constbuffer(binding = bindingPoint) uniform structName
#define CONST_BUFFER_STRUCT_END( variableName ) variableName

#define FLAT_INTERPOLANT( decl, bindingPoint ) flat decl
#define INTERPOLANT( decl, bindingPoint ) decl

#define OGRE_OUT_REF( declType, variableName ) out declType variableName
#define OGRE_INOUT_REF( declType, variableName ) inout declType variableName

#define OGRE_ARRAY_START( type ) type[](
#define OGRE_ARRAY_END )



out gl_PerVertex
{
	vec4 gl_Position;

};

layout(std140) uniform;


	
		#define worldViewMat worldView
	

	
mat4 UNPACK_MAT4( samplerBuffer matrixBuf, uint pixelIdx )
{
	vec4 row0 = texelFetch( matrixBuf, int((pixelIdx) << 2u) );
	vec4 row1 = texelFetch( matrixBuf, int(((pixelIdx) << 2u) + 1u) );
	vec4 row2 = texelFetch( matrixBuf, int(((pixelIdx) << 2u) + 2u) );
	vec4 row3 = texelFetch( matrixBuf, int(((pixelIdx) << 2u) + 3u) );
    return mat4( row0, row1, row2, row3 );
}

	
mat3x4 UNPACK_MAT4x3( samplerBuffer matrixBuf, uint pixelIdx )
{
	vec4 row0 = texelFetch( matrixBuf, int((pixelIdx) << 2u) );
	vec4 row1 = texelFetch( matrixBuf, int(((pixelIdx) << 2u) + 1u) );
	vec4 row2 = texelFetch( matrixBuf, int(((pixelIdx) << 2u) + 2u) );
	return mat3x4( row0, row1, row2 );
}


	// START UNIFORM DECLARATION
	
struct ShadowReceiverData
{
	float4x4 texViewProj;

	float2 shadowDepthRange;
	float2 padding;
	float4 invShadowMapSize;
};

struct Light
{
	
		float4 position;	//.w contains the objLightMask
	
	float4 diffuse;		//.w contains numNonCasterDirectionalLights
	float3 specular;


#define lightTexProfileIdx spotDirection.w
};

#define numNonCasterDirectionalLights lights[0].diffuse.w

#define areaLightDiffuseMipmapStart areaApproxLights[0].diffuse.w
#define areaLightNumMipmapsSpecFactor areaApproxLights[0].specular.w

#define numAreaApproxLights areaApproxLights[0].doubleSided.y
#define numAreaApproxLightsWithMask areaApproxLights[0].doubleSided.z

#define numAreaLtcLights areaLtcLights[0].points[0].w
#define numAreaLtcLights areaLtcLights[0].points[0].w

struct AreaLight
{
	
		float4 position;	//.w contains the objLightMask
	
	float4 diffuse;		//[0].w contains diffuse mipmap start
	float4 specular;	//[0].w contains mipmap scale
	float4 attenuation;	//.w contains texture array idx
	//Custom 2D Shape:
	//  direction.xyz direction
	//  direction.w invHalfRectSize.x
	//  tangent.xyz tangent
	//  tangent.w invHalfRectSize.y
	float4 direction;
	float4 tangent;
	float4 doubleSided;	//.y contains numAreaApproxLights
						//.z contains numAreaApproxLightsWithMask
	
};

struct AreaLtcLight
{
	
		float4 position;	//.w contains the objLightMask
	
	float4 diffuse;			//.w contains attenuation range
	float4 specular;		//.w contains doubleSided
	float4 points[4];		//.w contains numAreaLtcLights
							//points[1].w, points[2].w, points[3].w contain obbFadeFactorLtc.xyz
	
};





//Uniforms that change per pass
CONST_BUFFER_STRUCT_BEGIN( PassBuffer, 0 )
{
	//Vertex shader (common to both receiver and casters)

	float4x4 viewProj;







	//Vertex shader
	float4x4 view;
	

	

	//-------------------------------------------------------------------------

	//Pixel shader
	float3x3 invViewMatCubemap;


	float4 pccVctMinDistance_invPccVctInvDistance_rightEyePixelStartX_envMapNumMipmaps;

	float4 aspectRatio_planarReflNumMips_unused2;

	float2 invWindowRes;
	float2 windowResolution;













	Light lights[1];
	
	
// !use_light_buffers





	



	
	

	

#define pccVctMinDistance		pccVctMinDistance_invPccVctInvDistance_rightEyePixelStartX_envMapNumMipmaps.x
#define invPccVctInvDistance	pccVctMinDistance_invPccVctInvDistance_rightEyePixelStartX_envMapNumMipmaps.y
#define rightEyePixelStartX		pccVctMinDistance_invPccVctInvDistance_rightEyePixelStartX_envMapNumMipmaps.z
#define envMapNumMipmaps		pccVctMinDistance_invPccVctInvDistance_rightEyePixelStartX_envMapNumMipmaps.w

#define aspectRatio			aspectRatio_planarReflNumMips_unused2.x
#define planarReflNumMips	aspectRatio_planarReflNumMips_unused2.y
}
CONST_BUFFER_STRUCT_END( passBuf );



#define light0Buf		passBuf
#define light1Buf		passBuf
#define light2Buf		passBuf

// use_light_buffers


	
	
	// END UNIFORM DECLARATION

	
		
vec3 xAxis( vec4 qQuat )
{
	float fTy  = 2.0 * qQuat.y;
	float fTz  = 2.0 * qQuat.z;
	float fTwy = fTy * qQuat.w;
	float fTwz = fTz * qQuat.w;
	float fTxy = fTy * qQuat.x;
	float fTxz = fTz * qQuat.x;
	float fTyy = fTy * qQuat.y;
	float fTzz = fTz * qQuat.z;

	return vec3( 1.0-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy );
}

		
	



in vec4 vertex;


in vec4 qtangent;






	in vec2 uv0;


	in uint drawId;





	out block
	{
		
	
		
			FLAT_INTERPOLANT( ushort drawId, 0 );
		
	

	
		
			INTERPOLANT( float3 pos, 1 );
			INTERPOLANT( float3 normal, 2 );
			
		
		
			INTERPOLANT( float2 uv0, 3 );

		
		
		

		
	
	

	} outVs;


// START UNIFORM GL DECLARATION
/*layout(binding = 0) */uniform samplerBuffer worldMatBuf;


// END UNIFORM GL DECLARATION

void main()
{

    
	
	#define inputPos inVs_vertex
	//This newline is required else the Hlms parser will put '#define a #define b' in the same line
	#define inputNormal normal

	
		ogre_float4x3 worldMat = UNPACK_MAT4x3( worldMatBuf, inVs_drawId << 1u);
		
			float4x4 worldView = UNPACK_MAT4( worldMatBuf, (inVs_drawId << 1u) + 1u );
		

		float4 worldPos = float4( mul(inVs_vertex, worldMat).xyz, 1.0f );
	

	
		//Decode qTangent to TBN with reflection
		float3 normal		= xAxis( normalize( inVs_qtangent ) );
		
	

	
	
	
	
	//Lighting is in view space
		outVs.pos		= mul( inputPos, worldViewMat ).xyz;
		outVs.normal	= mul( inputNormal, toFloat3x3( worldViewMat ) );
	
	
        
			
				outVs_Position = mul( worldPos, passBuf.viewProj );
			
		
	


	
		

		

		
	
	

	/// hlms_uv_count will be 0 on shadow caster passes w/out alpha test
	
		outVs.uv0 = inVs_uv0;


	
		outVs.drawId = inVs_drawId;
	


	

	

	

	
}
