// “_ŒõŒ¹
struct PointLights
{
    float4 position;
    float4 color;
    float range;
};

struct SpotLights
{
    float4 position;
    float4 direction;
    float4 color;
    float range;
    float innerCorn;
    float outerCorn;
};

