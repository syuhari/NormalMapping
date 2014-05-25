#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform sampler2D u_normalMapTexture;
uniform vec3 u_lightNormal;

void main()
{
    
    // 法線テクスチャから色情報(法線)を取得
    vec4 normalMap = texture2D( u_normalMapTexture, v_texCoord );
    //0からの距離になっているので、-0.5で中心点をずらして正確な方向にする
    normalMap.xyz = normalMap.xyz - 0.5;
    
    // 画像テクスチャから、ポリゴンに貼る色を取得
    gl_FragColor = texture2D( u_texture, v_texCoord );
    // rgbのみ(aは除外)、法線情報とライトの向きから明るさを計算して、画像の色に掛け合わせる
    vec3 ret = gl_FragColor.rgb * max( 0.1, dot( normalMap.rgb, u_lightNormal ) )*3.0;   //倍率は結果を見ながら調整
    gl_FragColor.rgb = ret.rgb;
    
}