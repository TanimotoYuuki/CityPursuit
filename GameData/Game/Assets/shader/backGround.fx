////////////////////////////////
/// バックグラウンドシェーダー
////////////////////////////////

//頂点シェーダーの入力
struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

//ピクセルシェーダーの入力
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

//スプライトの定数バッファ
cbuffer cb : register(b0)
{
    float4x4 mvp;        //MVP関数
    float4 mulColor;     //乗算カラー
};

//バックグラウンドレンダーの定数バッファを取得
cbuffer BackGroundRenderCb : register(b1)
{
    bool  scrollFlag;               //背景をスクロールするか？
    float screenEdgeCorrection;     //画像の画面端補正
    float rate;                     //割合
    float texelSize;                //テクセルサイズ
}

Texture2D<float4> colorTexture : register(t0);
sampler Sampler : register(s0);
float4 CalcBoke(PSInput In, float texelSize, float4 color);

//頂点シェーダー
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    
    //背景をスクロールするなら
    if (scrollFlag == true)
    {
        //画面端の補正
        psIn.pos.x += screenEdgeCorrection;
        //プレイヤーが移動したら横にイージングする
        psIn.pos.x -= lerp(0.0f, screenEdgeCorrection * 2, rate);
    }
    
    return psIn;
}

//ピクセルシェーダー
float4 PSMain(PSInput In) : SV_Target0
{
    float4 color = colorTexture.Sample(Sampler, In.uv) * mulColor;
    
    //テクセルサイズが0.0fより大きかったら画像をぼかす処理をする
    if(texelSize>0.0f)
    {
        color = CalcBoke(In, texelSize, color);
    }
    
    return color;
}

//画像をぼかす処理
float4 CalcBoke(PSInput In, float texelSize, float4 color)
{
    float offsetU = texelSize / 1600.0f; //U方向のオフセット
    float offsetV = texelSize / 900.0f;  //V方向のオフセット
    
    //基準テクセルから右のテクセルのカラーをサンプリングする
    color += colorTexture.Sample(Sampler, In.uv + float2(offsetU, 0.0f)) * mulColor;

    //基準テクセルから左のテクセルのカラーをサンプリングする
    color += colorTexture.Sample(Sampler, In.uv + float2(-offsetU, 0.0f)) * mulColor;
    
    //基準テクセルから下のテクセルのカラーをサンプリングする
    color += colorTexture.Sample(Sampler, In.uv + float2(0.0f, offsetV)) * mulColor;
    
    //基準テクセルから上のテクセルのカラーをサンプリングする
    color += colorTexture.Sample(Sampler, In.uv + float2(0.0f, -offsetV)) * mulColor;
    
    //基準テクセルから右下のテクセルのカラーをサンプリングする
    color += colorTexture.Sample(Sampler, In.uv + float2(offsetU, offsetV)) * mulColor;
    
    //基準テクセルから右上のテクセルのカラーをサンプリングする
    color += colorTexture.Sample(Sampler, In.uv + float2(offsetU, -offsetV)) * mulColor;
    
    //基準テクセルから左下のテクセルのカラーをサンプリングする
    color += colorTexture.Sample(Sampler, In.uv + float2(-offsetU, offsetV)) * mulColor;
    
    //基準テクセルから左上のテクセルのカラーをサンプリングする
    color += colorTexture.Sample(Sampler, In.uv + float2(-offsetU, -offsetV)) * mulColor;
    
    //サンプリングしたカラーを平均化するために9で割る
    color /= 9.0f;
    
    //ぼかし処理をしたカラーを返す
    return color; 
}