RWStructuredBuffer<ParticleHeader> particleHeaderBuffer : register(u3);

bool comparer(in ParticleHeader x0, in ParticleHeader x1)
{
    //  生存は+1、死亡は0
    //  同じフラグであれば深度比較
    return (x0.alive > x1.alive || (x0.alive == x1.alive && x0.depth > x1.depth));
}