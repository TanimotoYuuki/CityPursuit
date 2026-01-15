//スプライトの定数バッファ
cbuffer cb : register(b0)
{
    float4x4 mvp; //MVP行列
    float4 mulColor; //乗算カラー
};

//ゴッドレイの定数バッファ
cbuffer GodRayCb : register(b1)
{
    float strength; // ブラーの強さ
    float2 center; // ブラーの中心座標
}

//頂点シェーダーの入力
struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};

//ピクセルシェーダーの入力
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D<float4> maskTexture : register(t0);
sampler Sampler : register(s0);

//乱数生成関数
float rnd(float2 uv, float seed)
{
    return frac(sin(dot(uv + seed, float2(12.9898, 78.233))) * 43758.5453 + seed);
}

//頂点シェーダー
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

//ピクセルシェーダー
float4 PSMain(PSInput In) : SV_Target0
{
    //出力色の初期化
    float3 destColor = float3(0.0, 0.0, 0.0);
    
    //乱数によるオフセット
    float random = rnd(In.uv, 0.0f);
    
    //現在のUV座標
    float2 fc = In.uv;
    
    //光源中心から現在のピクセルへのベクトル
    float2 fcc = fc - center.xy;
        
    // ループ回数などの定数
    const float loopCount = 30.0;
    const float nFrag = 1.0 / loopCount; // 正規化用係数
    
    float totalWeight = 0.0;
    
    //ゴットブラー処理のループ
    for (float i = 0.0; i <= loopCount; i += 1.0)
    {
        float percent = (i + random) * nFrag;
        float weight = percent - percent * percent;
        
        //サンプリング位置を中心に向かってずらす
        float2 t = fc - fcc * percent * strength * nFrag;
        
        //テクスチャサンプリングと加算
        destColor += maskTexture.Sample(Sampler, t).rgb * weight;
        
        //重みの合計を記録
        totalWeight += weight;
    }
    
    return float4(destColor / totalWeight, 1.0f);
}