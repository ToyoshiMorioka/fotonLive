#version 120
uniform vec2 iResolution;
uniform float iGlobalTime;
uniform float bpm;
uniform float volume;
uniform sampler2DRect result;
uniform vec2 result_resolution;
uniform int state;

vec2 upCoord(){
    return gl_FragCoord.xy*result_resolution/iResolution;
}

float beat(float t, float damp){
    float dt = 60.0/bpm;
    float fracture = mod(t, dt);
    return exp(-damp*fracture);
}

float encode(vec2 p){
    return result_resolution.x*p.y + p.x;
}

vec2 decode(float i){
    float x = mod(i, result_resolution.x);
    return vec2(x, (i-x)/result_resolution.x);
}


vec4 effect_shift(){
    // float shift = 15.0*beat(iGlobalTime, 5.0);
    float shift = 4.0*volume;
    vec4 color;
    vec2 shift_x = mod(upCoord()+vec2(shift,     0)+iResolution, iResolution);
    vec2 shift_y = mod(upCoord()+vec2(-shift,-shift)+iResolution, iResolution);
    vec2 shift_z = mod(upCoord()+vec2(shift, shift)+iResolution, iResolution);
    color.r = texture2DRect(result, shift_x).r;
    color.g = texture2DRect(result, shift_y).g;
    color.b = texture2DRect(result, shift_z).b;
    color.a = 1.0;
    return color;
}

vec4 effect_mosaic(){
    float tile_size = 1.0+63.0*beat(iGlobalTime/16.0, 20.0);
    vec4 color;
    vec2 p = upCoord()-mod(upCoord(), tile_size);
    color = texture2DRect(result, p);
    return color;
}

vec4 effect_glitch(){
    vec4 color;
    float i = encode(upCoord());
    vec2 p = decode(i);
    color = texture2DRect(result, p);
    return color;   
}

bool bitState(int bitPos){
    int temp = state; //0101
    for(int i = 1; i < bitPos; ++i){
        int v = int(mod(temp, pow(2, i))); // 0001, 0000
        temp -= v; // 0100, 0100
    }
    if(int(mod(temp, pow(2, bitPos))) != 0) return true;
    else return false;
}

void main()
{
    float shift = 2.0*beat(iGlobalTime/8.0, 6.0);
    // float shift = 15.0*volume;
    gl_FragColor = texture2DRect(result, upCoord());
    if(bitState(1))gl_FragColor = mix(gl_FragColor, effect_glitch(), shift);
    if(bitState(2))gl_FragColor = mix(gl_FragColor, effect_mosaic(), 0.7);
    if(bitState(3))gl_FragColor = mix(gl_FragColor, effect_shift(), 0.6);
    // gl_FragColor = texture2DRect(result, upCoord());
}
