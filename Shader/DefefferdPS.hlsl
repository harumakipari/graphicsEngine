#include "FullScreenQuad.hlsli"
#include "Constants.hlsli"
#include "imageBasedLighting.hlsli"
#include "BidirectionalReflectanceDistributionFunction.hlsli"

Texture2D colorMap : register(t0);
Texture2D positionMap : register(t1);
Texture2D normalMap : register(t2);
Texture2D aaaMap : register(t3);
SamplerState linearBorderBlackSamplerState : register(s3);

//cbuffer LIGHT_CONSTANT_BUFFER : register(b1)
//{
	
//}

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = colorMap.Sample(linearBorderBlackSamplerState, pin.texcoord);
    float3 normal = normalMap.Sample(linearBorderBlackSamplerState, pin.texcoord).xyz;
    float3 position = positionMap.Sample(linearBorderBlackSamplerState, pin.texcoord).xyz;
    //float3 lig = 0.0f;
    //lig += max(0.0f, dot(normal * -1.0, lightDirection.xyz)) * colorLight.xyz;
    //lig += 1.0f;
    float4 finalColor = color;
    //finalColor.xyz *= lig;
    float3 aaa = aaaMap.Sample(linearBorderBlackSamplerState, pin.texcoord).xyz;
    finalColor.xyz *= aaa;
    //float metallicFactor = m.pbrMetallicRoughness.metallicFactor;
    float metallicFactor = 0.5f;
    
    
    float3 diffuse = aaa;
    float3 specular = 0;
    float3 N = normalMap.Sample(linearBorderBlackSamplerState, pin.texcoord).xyz;
    const float3 V = normalize(cameraPositon.xyz - position.xyz);
    const float3 f0 = lerp(0.04, color.rgb, metallicFactor);
    const float3 f90 = 1.0;
    const float alphaRoughness = 1.0f;
    const float3 cDiff = 0.5f;
    //const float alphaRoughness = roughnessFactor * roughnessFactor;
    //const float3 cDiff = lerp(basecolorFactor.rgb, 0.0, metallicFactor);

#if 1   
    //ƒeƒNƒXƒ`ƒƒ‚ð“\‚é
    // Loop for shading process for each light 
    float3 L = normalize(-lightDirection.xyz);
    float3 Li = float3(colorLight.x, colorLight.y, colorLight.z) * colorLight.w; // Radiance of the light 
    //float3 Li = float3(1.0, 1.0, 1.0); // Radiance of the light 
    //const float NoL = max(0.0, dot(N, L));
    float NoL = max(0, 0.5 * dot(N, L) + 0.5);
    const float NoV = max(0.0, dot(N, V));
    if (NoL > 0.0 || NoV > 0.0)
    {
        const float3 R = reflect(-L, N);
        const float3 H = normalize(V + L);
        
        const float NoH = max(0.0, dot(N, H));
        const float HoV = max(0.0, dot(H, V));
        
        diffuse += Li * NoL * BrdfLambertian(f0, f90, cDiff, HoV);
        specular += Li * NoL * BrdfSpecularGgx(f0, f90, alphaRoughness, HoV, NoL, NoV, NoH);
    }
#endif
    //float roughnessFactor = m.pbrMetallicRoughness.roughnessFactor;
    float roughnessFactor = 0.5f;
    
    //return basecolorFactor;
    
#if 1   //ŠO‚Ì”wŒi‚ðˆÚ‚·
    diffuse += IblRadianceLambertian(N, V, roughnessFactor, cDiff, f0) * iblIntensity;
    //specular += IblRadianceGgx(N, V, roughnessFactor, f0);
    specular += IblRadianceGgx(N, V, roughnessFactor, f0) * iblIntensity;
#endif
    
    
    float3 emmisive = float3(1.0, 1.0, 1.0);
    //float3 emmisive = emmisiveFactor;
    
    //diffuse = lerp(diffuse, diffuse * occlusionFactor, occlusionStrength);
    //specular = lerp(specular, specular * occlusionFactor, occlusionStrength);

    //return float4(diffuse + specular + emmisive, basecolorFactor.a) * basecolorFactor;
    return float4(diffuse + specular + emmisive, 1.0) * color;
    
    
    return finalColor;
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}