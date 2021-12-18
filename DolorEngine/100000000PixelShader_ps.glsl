#if 0
	***	hlms_uv_count0	2
	***	hlms_pose	0
	***	uv_emissive	0
	***	first_valid_detail_map_nm	4
	***	fresnel_scalar	0
	***	envprobe_map_sampler	1
	***	hlms_uv_count	1
	***	uv_detail_nm1	0
	***	hlms_lights_spotparams	0
	***	uv_detail_nm2	0
	***	envMapRegSampler	1
	***	emissive_map_sampler	1
	***	BRDF_Default	1
	***	uv_specular	0
	***	hlms_disable_stage	0
	***	NumPoseWeightVectors	0
	***	specular_map_sampler	1
	***	hlms_forwardplus_flipY	0
	***	texcoord	4
	***	glsl	635204550
	***	hlms_high_quality	0
	***	NumPoseWeightVectorsB	0
	***	hlms_lights_directional	0
	***	samplerStateStart	1
	***	alpha_test	0
	***	roughness_map_sampler	1
	***	GGX_height_correlated	1
	***	glsles	1070293233
	***	metallic_workflow	0
	***	hw_gamma_write	1
	***	uv_detail_weight	0
	***	detail_map_nm2_sampler	1
	***	cubemaps_as_diffuse_gi	1
	***	uv_detail_nm3	0
	***	NumPoseWeightVectorsC	0
	***	hlms_pose_normals	0
	***	hlms_pose_half	0
	***	hlms_lights_spot	1
	***	GL_ARB_shading_language_420pack	1
	***	detail_map1_sampler	1
	***	uv_normal	0
	***	detail_map_nm3_sampler	1
	***	hlms_alphablend	0
	***	MoreThanOnePose	-1
	***	uv_detail3	0
	***	detail_map3_sampler	1
	***	normal_map	0
	***	NumPoseWeightVectorsA	0
	***	uv_detail1	0
	***	fresnel_workflow	0
	***	perceptual_roughness	1
	***	alpha_test_shadow_caster_only	0
	***	materials_per_buffer	240
	***	hw_gamma_read	1
	***	hlms_skeleton	0
	***	hlms_pssm_splits_minus_one	-1
	***	syntax	635204550
	***	NeedsMoreThan1BonePerVertex	-1
	***	hlms_lights_point	1
	***	diffuse_map_sampler	1
	***	metal	-1698855755
	***	GL_ARB_base_instance	1
	***	detail_map_nm1_sampler	1
	***	s_lights_directional_non_caster	1
	***	uv_detail0	0
	***	uv_diffuse	0
	***	uv_detail_nm0	0
	***	detail_weight_map_sampler	1
	***	GL3+	450
	***	hlms_lights_attenuation	0
	***	detail_map_nm0_sampler	1
	***	hlms_render_depth_only	0
	***	normal_map_tex_sampler	1
	***	uv_detail2	0
	***	MoreThanOnePoseWeightVector	-1
	***	detail_map2_sampler	1
	***	detail_map0_sampler	1
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




	#define UV_DIFFUSE(x) (x)
	#define UV_NORMAL(x) (x)
	#define UV_SPECULAR(x) (x)
	#define UV_ROUGHNESS(x) (x)
	#define UV_DETAIL_WEIGHT(x) (x)
	#define UV_DETAIL0(x) (x)
	#define UV_DETAIL1(x) (x)
	#define UV_DETAIL2(x) (x)
	#define UV_DETAIL3(x) (x)
	#define UV_DETAIL_NM0(x) (x)
	#define UV_DETAIL_NM1(x) (x)
	#define UV_DETAIL_NM2(x) (x)
	#define UV_DETAIL_NM3(x) (x)
	#define UV_EMISSIVE(x) (x)
	


layout(std140) uniform;


	
		
			layout(location = 0, index = 0) out vec4 outColour;
		
		
		
	












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


	
	
//Uniforms that change per Item/Entity, but change very infrequently
struct Material
{
	/* kD is already divided by PI to make it energy conserving.
	  (formula is finalDiffuse = NdotL * surfaceDiffuse / PI)
	*/
	float4 bgDiffuse;
	float4 kD; //kD.w is alpha_test_threshold
	float4 kS; //kS.w is roughness
	//Fresnel coefficient, may be per colour component (float3) or scalar (float)
	//F0.w is transparency
	float4 F0;
	float4 normalWeights;
	float4 cDetailWeights;
	float4 detailOffsetScale[4];
	float4 emissive;		//emissive.w contains mNormalMapWeight.
	float refractionStrength;
	float _padding10;
	float _padding11;
	float _padding12;
	float4 userValue[3];

	
		uint4 indices0_3;
		uint4 indices4_7;
	

	
};
	

	
		CONST_BUFFER( MaterialBuf, 1 )
		{
			Material materialArray[240];
		};
	

	
		//Uniforms that change per Item/Entity
		CONST_BUFFER( InstanceBuffer, 2 )
		{
			//.x =
			//The lower 9 bits contain the material's start index.
			//The higher 23 bits contain the world matrix start index.
			//
			//.y =
			//shadowConstantBias. Send the bias directly to avoid an
			//unnecessary indirection during the shadow mapping pass.
			//Must be loaded with uintBitsToFloat
			//
			//.z =
			//lightMask. Ogre must have been compiled with OGRE_NO_FINE_LIGHT_MASK_GRANULARITY
			
				uint4 worldMaterialIdx[4096];
			
		};
	
	


// END UNIFORM DECLARATION


	
		#define float_fresnel float
		#define make_float_fresnel( x ) x
	

	
	
		#define OGRE_DEPTH_CMP_GE( a, b ) (a) <= (b)
		#define OGRE_DEPTH_DEFAULT_CLEAR 0.0
	


	
		#define PASSBUF_ARG_DECL
		#define PASSBUF_ARG
	

	

	struct PixelData
	{
		
			float3 normal;
			
				#define geomNormal normal
			
			float4	diffuse;
			float3	specular;
			float	perceptualRoughness;
			float	roughness;
			float_fresnel	F0;

			
				float3	viewDir;
				float	NdotV;
			

			
		
	};

	#define SampleDetailWeightMap( tex, sampler, uv, arrayIdx ) OGRE_SampleArray2D( tex, sampler, uv, arrayIdx )
	
	
	
	
	
	

	

	

	

	
		
//Default BRDF
INLINE float3 BRDF( float3 lightDir, float3 lightDiffuse, float3 lightSpecular, PixelData pixelData )
{
    float3 halfWay = normalize( lightDir + pixelData.viewDir );
    float NdotL = saturate( dot( pixelData.normal, lightDir ) );
    float NdotH = saturate( dot( pixelData.normal, halfWay ) );
    float VdotH = saturate( dot( pixelData.viewDir, halfWay ) );

    float sqR = pixelData.roughness * pixelData.roughness;

	//Roughness/Distribution/NDF term (GGX)
	//Formula:
	//	Where alpha = roughness
	//	R = alpha^2 / [ PI * [ ( NdotH^2 * (alpha^2 - 1) ) + 1 ]^2 ]
	float f = ( NdotH * sqR - NdotH ) * NdotH + 1.0;
	float R = sqR / (f * f + 1e-6f);

	//Geometric/Visibility term (Smith GGX Height-Correlated)

    float Lambda_GGXV = NdotL * sqrt( (-pixelData.NdotV * sqR + pixelData.NdotV) * pixelData.NdotV + sqR );
    float Lambda_GGXL = pixelData.NdotV * sqrt( (-NdotL * sqR + NdotL) * NdotL + sqR );

	float G = 0.5 / (( Lambda_GGXV + Lambda_GGXL + 1e-6f ) * 3.141592654);


	//Formula:
	//	fresnelS = lerp( (1 - V*H)^5, 1, F0 )
    float_fresnel fresnelS = pixelData.F0 + pow( 1.0 - VdotH, 5.0 ) * (1.0 - pixelData.F0);

	//We should divide Rs by PI, but it was done inside G for performance
    float3 Rs = ( fresnelS * (R * G) ) * pixelData.specular.xyz * lightSpecular;

	//Diffuse BRDF (*Normalized* Disney, see course_notes_moving_frostbite_to_pbr.pdf
	//"Moving Frostbite to Physically Based Rendering" Sebastien Lagarde & Charles de Rousiers)
    float energyBias	= pixelData.roughness * 0.5;
    float energyFactor	= lerp( 1.0, 1.0 / 1.51, pixelData.roughness );
    float fd90			= energyBias + 2.0 * VdotH * VdotH * pixelData.roughness;
	float lightScatter	= 1.0 + (fd90 - 1.0) * pow( 1.0 - NdotL, 5.0 );
    float viewScatter	= 1.0 + (fd90 - 1.0) * pow( 1.0 - pixelData.NdotV, 5.0 );

    
        float fresnelD = 1.0f - fresnelS;
    

	//We should divide Rd by PI, but it is already included in kD
    float3 Rd = (lightScatter * viewScatter * energyFactor * fresnelD) * pixelData.diffuse.xyz * lightDiffuse;

	return NdotL * (Rs + Rd);
}

		
		
	

	
	
	
	
	



in block
{

	
		
			FLAT_INTERPOLANT( ushort drawId, 0 );
		
	

	
		
			INTERPOLANT( float3 pos, 1 );
			INTERPOLANT( float3 normal, 2 );
			
		
		
			INTERPOLANT( float2 uv0, 3 );

		
		
		

		
	
	

} inPs;










	


















void main()
{
    
	
	
	

	
		PixelData pixelData;

		
	
		
            ushort materialId	= worldMaterialIdx[inPs.drawId].x & 0x1FFu;
            #define material materialArray[materialId]
		
	

		
	
		
		
		
		
		
		
	

		
	
		
		
		
		
		
		
		
		
		
	

		
		

		
	


		
	/// Sample detail maps and weight them against the weight map in the next foreach loop.
	


		
			
	/// DIFFUSE MAP
	
		/// If there are no diffuse maps, we must initialize it to some value.
		pixelData.diffuse.xyzw = material.bgDiffuse.xyzw;
	

	/// Blend the detail diffuse maps with the main diffuse.
	

	/// Apply the material's diffuse over the textures
	pixelData.diffuse.xyz *= material.kD.xyz;
	

	

		

		
	/// SPECUlAR MAP
	pixelData.specular.xyz = material.kS.xyz;
	
		pixelData.F0 = material.F0.x;
		
		
	
	

		
	/// ROUGHNESS MAP
	pixelData.perceptualRoughness = material.kS.w;
	
	
		pixelData.roughness = max( pixelData.perceptualRoughness * pixelData.perceptualRoughness, 0.001f );
	




		

		
			
	
		// Geometric normal
		pixelData.normal = normalize( inPs.normal ) ;
	

			
	/// If there is no normal map, the first iteration must
	/// initialize pixelData.normal instead of try to merge with it.
	
		
		
	

	

	

	/// Blend the detail normal maps with the main normal.
	
	


			

			

			

			

		

		
			
	//Everything's in Camera space
	
		
			pixelData.viewDir	= normalize( -inPs.pos );
		
		pixelData.NdotV		= saturate( dot( pixelData.normal, pixelData.viewDir ) );
	

	
		float3 finalColour = float3(0, 0, 0);
	

	
		float3 lightDir;
		float fDistance;
		float3 tmpColour;
		float spotCosAngle;
	
	

	


			

			
				
	
	

	
		
			
				finalColour += BRDF( light0Buf.lights[0].position.xyz, light0Buf.lights[0].diffuse.xyz, light0Buf.lights[0].specular, pixelData );
	

			

			
	

			
	


			
			

			

			

			
			

			
			

			
	


			

			

			

			
	

	

	


			

			
		///!hlms_prepass

	///!hlms_normal || hlms_qtangent

	
		
			
				
					outPs_colour0.xyz	= finalColour;
				

				
					outPs_colour0.w		= 1.0;
				

							
				
			
		
	

	
}
///hlms_shadowcaster
