#version 120

#extension GL_ARB_texture_rectangle : enable
#extension GL_ARB_draw_buffers : enable

// ユーティリティのインクルード
#pragma include "util.frag"
// 4D Simplex Noiseのインクルード
#pragma include "noise4D.frag"

uniform sampler2DRect u_posAndAgeTex;
uniform sampler2DRect u_velAndMaxAgeTex;
//uniform sampler2DRect u_accTex;
uniform float u_time;
uniform float u_timestep;
uniform float u_scale;
uniform vec3 u_emitterPos;
uniform vec3 u_prevEmitterPos;
uniform float u_emitterRadius;
uniform vec3 u_gravity;
uniform float u_kick;

void main(void){
    vec2 st = gl_TexCoord[0].st;
    // 前の位置情報とパーティクル初期化からの経過時間を取得
    vec4 posAndAge = texture2DRect(u_posAndAgeTex,st);
    // 前の速度と生存期間を取得
    vec4 velAndMaxAge = texture2DRect(u_velAndMaxAgeTex,st);
    // 前の加速度を取得
//    vec4 accTex = texture2DRect(u_accTex,st);
    
    vec3 pos = posAndAge.xyz; // 前の位置
    vec3 vel = velAndMaxAge.xyz; // 前の速度
//    vec3 acc = accTex.xyz; // 前の加速度
    
    float age = posAndAge.w; // 経過時間
    float maxAge = velAndMaxAge.w; // 生存期間

    age++;
    // kickの瞬間にたくさん殺す
    if (u_kick > 0.0) {
        age += 10;
    }
    
    // パーティクルが生存期間を過ぎたら初期化
    if(age >= maxAge){
        age = 0;
        maxAge = 50.0 + 150.0 * random(pos.xx);
        float theta = 2.0 * PI * random(pos.yy);
        float phi = PI * random(pos.zz);
//        float r = u_emitterRadius * random(pos.xy);
        float r = (random(pos.xy) * 0.5 + 0.5) *u_emitterRadius;;
        float d = 100 * random(pos.xx);
//        pos = u_emitterPos + vec3(r * sin(theta) * cos(phi), 0, r * cos(theta) * sin(phi));
        
        // kickの瞬間は大きく
        if (u_kick > 0.0) {
            r = (random(pos.xy) * 0.1 + 1.0) *u_emitterRadius;
        }
        
        if (random(pos.yz) < 0.01) {
            r = 1.0;
        }
        
//        theta += snoise(vec4(pos.x * u_scale, pos.y * u_scale, pos.z * u_scale, 0.1352 * u_time * u_timestep));
        pos = u_emitterPos + vec3(r * sin(theta+phi), d, r * cos(theta+phi));
        vel.xyz = u_gravity * 100; //vec3(0,0,0);
    }
    
    // Curl Noiseで速度を更新
    vel.x += snoise(vec4(pos.x * u_scale, pos.y * u_scale, pos.z * u_scale, 0.1352 * u_time * u_timestep));
//    vel.y += snoise(vec4(pos.x * u_scale, pos.y * u_scale, pos.z * u_scale, 1.2814 * u_time * u_timestep));
    vel.z += snoise(vec4(pos.x * u_scale, pos.y * u_scale, pos.z * u_scale, 2.5564 * u_time * u_timestep));
    
    // 加速
    vel += u_gravity * (1.0 + random(pos.xy) );
    
    pos += vel;
    
    gl_FragData[0].rgba = vec4(pos, age); // 位置と経過時間を出力
    gl_FragData[1].rgba = vec4(vel, maxAge); //速度と生存期間を出力
//    gl_FragData[2].rgba = vec4(acc, 0); //加速度を出力
}
