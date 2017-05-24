#version 120
#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect tex;
uniform vec4 lineColor;

#define PI 3.14159265358979

float random(vec2 n)
{
    return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}
float random(float n)
{
    return fract(sin(dot(n, 12.9898))* 43758.5453);
}
float random(float n, float scale)
{
    return fract(sin(dot(n, 12.9898))* 43758.5453*scale);
}
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec2 st;

void main() {
    st = gl_TexCoord[0].st;
    
    vec4 oriColor = texture2DRect(tex,st);
    
    
    
    if(oriColor.r !=oriColor.g || oriColor.r !=oriColor.b || oriColor.g !=oriColor.b){
        gl_FragColor = oriColor;
        
    }else{
        
        float e_step = 1.0;
        
        vec4 x_minus1 = texture2DRect(tex, vec2(st.x - 1 * e_step, st.y));
        vec4 x_plus1 = texture2DRect(tex, vec2(st.x + 1 * e_step, st.y));
        vec4 y_minus1 = texture2DRect(tex, vec2(st.x, st.y - 1 * e_step));
        vec4 y_plus1 = texture2DRect(tex, vec2(st.x, st.y + 1 * e_step));

        
//        if(oriColor!=x_minus1||oriColor!=x_plus1||oriColor!=y_minus1||oriColor!=y_plus1){
        if(x_minus1!=x_plus1||y_minus1!=y_plus1){

                gl_FragColor = lineColor;
        }else{
            gl_FragColor = vec4(0.0,0.0,0.0,0.0);
        }
        
    }
    
    
}
