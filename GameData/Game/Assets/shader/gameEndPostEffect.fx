//スプライトの定数バッファ
cbuffer cb : register(b0)
{
    float4x4 mvp;       //MVP行列
    float4 mulColor;    //乗算カラー
};

//頂点シェーダーの入力
struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD;
};

//ピクセルシェーダーの入力
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
};

//ゲーム終了のときにするポストエフェクトの定数バッファ
cbuffer cb : register(b1)
{
    float drawingRate;//ポストエフェクト用イージング割合
    int drawingPostEffect;//描画するポストエフェクト
}

Texture2D<float4> g_sceneTexture : register(t0);
Texture2D<float4> g_bokeTexture : register(t1);

sampler g_sampler : register(s0);

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
    float4 originalColor = g_sceneTexture.Sample(g_sampler, In.uv);
    float4 bokeColor = g_bokeTexture.Sample(g_sampler, In.uv);
    float4 targetColor;
    if (drawingPostEffect == 0)
    {
        float y = 0.299f * bokeColor.r + 0.587f * bokeColor.g + 0.114f * bokeColor.b;
        float3 monochromeBoke = float3(y, y, y);
        targetColor = float4(monochromeBoke, bokeColor.a);
    }
    else
    {
        targetColor = bokeColor;
    }
    
    float4 finalColor = lerp(originalColor, targetColor, drawingRate);
    
    return finalColor;
}