#version 120
uniform vec2 iResolution;
uniform float iGlobalTime;
uniform float bpm;
uniform float volume;
uniform sampler2DRect result;

float beat(float t, float damp){
    float dt = 60.0/bpm;
    float fracture = mod(t, dt);
    return exp(-damp*fracture);
}

float encode(vec2 p){
    return iResolution.x*p.y + p.x-iResolution.x/2.0;
}

vec2 decode(float i){
    float x = mod(i, iResolution.x);
    return vec2(x, i/iResolution.x);
}


vec4 effect_shift(){
    float shift = 15.0*beat(iGlobalTime, 5.0);
    vec4 color;
    vec2 shift_x = mod(gl_FragCoord.xy+vec2(shift,     0)+iResolution, iResolution);
    vec2 shift_y = mod(gl_FragCoord.xy+vec2(-shift,-shift)+iResolution, iResolution);
    vec2 shift_z = mod(gl_FragCoord.xy+vec2(shift, shift)+iResolution, iResolution);
    color.r = texture2DRect(result, shift_x).r;
    color.g = texture2DRect(result, shift_y).g;
    color.b = texture2DRect(result, shift_z).b;
    color.a = 1.0;
    return color;
}

vec4 effect_mosaic(){
    float tile_size = 1.0+63.0*beat(iGlobalTime/8.0, 20.0);
    vec4 color;
    vec2 p = gl_FragCoord.xy-mod(gl_FragCoord.xy, tile_size);
    color = texture2DRect(result, p);
    return color;
}

vec4 effect_glitch(){
    float shift = beat(iGlobalTime/2.0, 40.0);
    vec4 color;
    float i = encode(gl_FragCoord.xy);
    vec2 p = decode(i-sin(iGlobalTime/2.0+gl_FragCoord.y/3.4)*shift*220.0);
    color = texture2DRect(result, p);
    return color;   
}

void main()
{
    gl_FragColor = effect_glitch();
    gl_FragColor = mix(gl_FragColor, effect_mosaic(), 0.7);
    gl_FragColor = mix(gl_FragColor, effect_shift(), 0.6);
}
