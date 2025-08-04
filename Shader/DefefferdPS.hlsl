#include "FullScreenQuad.hlsli"
#include "Constants.hlsli"
#include "imageBasedLighting.hlsli"
#include "BidirectionalReflectanceDistributionFunction.hlsli"
#include "Lights.hlsli"

Texture2D normalMap : register(t0);
Texture2D msrMap : register(t1);    
Texture2D colorMap : register(t2);
Texture2D positionMap : register(t3);
Texture2D emmisiveMap : register(t4);
SamplerState linearBorderBlackSamplerState : register(s3);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = colorMap.Sample(linearBorderBlackSamplerState, pin.texcoord);
    float3 normal = normalMap.Sample(linearBorderBlackSamplerState, pin.texcoord).xyz;
    float3 position = positionMap.Sample(linearBorderBlackSamplerState, pin.texcoord).xyz;
    float4 finalColor = color;
    float4 msr = msrMap.Sample(linearBorderBlackSamplerState, pin.texcoord);
    float metallicFactor = msr.x;
    float roughnessFactor = msr.z;
    
    float3 emmisiveFactor = emmisiveMap.Sample(linearBorderBlackSamplerState, pin.texcoord).xyz;
    float3 N = normalMap.Sample(linearBorderBlackSamplerState, pin.texcoord).xyz;
    const float3 f0 = lerp(0.04, color.rgb, metallicFactor);
    const float3 f90 = 1.0;
    const float alphaRoughness = roughnessFactor * roughnessFactor;
    const float3 cDiff = lerp(color.rgb, 0.0, metallicFactor);
    const float3 P = position.xyz;
    const float3 V = normalize(cameraPositon.xyz - position.xyz);
    
    float3 diffuse = 0;
    float3 specular = 0;
    float3 L = normalize(-lightDirection.xyz);
    float3 Li = float3(colorLight.x, colorLight.y, colorLight.z) * colorLight.w; // Radiance of the light 
    float NoL = max(0, 0.5 * dot(N, L) + 0.5);

#if 1   
    //ƒeƒNƒXƒ`ƒƒ‚ð“\‚é
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
    
#if 1   //ŠO‚Ì”wŒi‚ðˆÚ‚·
    diffuse += IblRadianceLambertian(N, V, roughnessFactor, cDiff, f0) * iblIntensity;
    specular += IblRadianceGgx(N, V, roughnessFactor, f0) * iblIntensity;
#endif
    
    float occlusionFactor = msr.y;
    float occlusionStrength = msr.w;
    float3 emmisive = emmisiveFactor;
    
    diffuse = lerp(diffuse, diffuse * occlusionFactor, occlusionStrength);
    specular = lerp(specular, specular * occlusionFactor, occlusionStrength);

    //return float4(diffuse + specular + emmisive, basecolorFactor.a) * basecolorFactor;
    return float4(diffuse + specular + emmisive, 1.0) * color;
    
    
    return finalColor;
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}