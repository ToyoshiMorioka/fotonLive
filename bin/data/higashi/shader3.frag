#version 120
uniform float iGlobalTime;
uniform vec2 iResolution;
uniform float bpm;
uniform float history[64];
const float PI = 3.1415926;
const float P2 = 6.2831852;
const vec4 albedo = vec4(0.5, 0.0, 0.5, 1.0);
const float cam_dist = 42.0;

//const vec3 lightDir = vec3(-0.577, 0.577, 0.577);
// Primitive
float planeSDF(vec3 p){
    // plane: y = 0
   return p.y;
}
// Boolean
float intersectSDF(float distA, float distB) {
    return max(distA, distB);
}

float unionSDF(float distA, float distB) {
    return min(distA, distB);
}

float differenceSDF(float distA, float distB) {
    return max(distA, -distB);
}

float smin(float distA, float distB, float k){
    float res = exp( -k*distA ) + exp( -k*distB );
    return -log( res )/k;
}

float beat(float t, float damp){
    float dt = 60.0/bpm;
    float fracture = mod(t, dt);
    return exp(-damp*fracture/dt);
}

float cubeSize(vec3 p){
    int i = int(clamp(length(floor(p))/2.0, 0.0, 64.0));
    return 0.1+0.25*history[i];
    //return 0.4;
}

int cubeHeight(vec3 p){
    vec2 pos = p.xz;
    int i = int(clamp(length(floor(p)), 0.0, 64.0));
    return int(1.0+20.0*history[i]);
}

vec3 trans(vec3 p){
    return mod(p, 1.0) - 0.5;
}

vec3 transXZ(vec3 p){
    vec3 gridPos = trans(p);
    gridPos.y = p.y;
    return gridPos;
}

float sphereSDF(vec3 p, float s){
    return length(p)-s;
}

float cubeSDF(vec3 p, float s){
    vec3 d = abs(p) - s;
    float insideDistance = min(max(d.x, max(d.y, d.z)), 0.0);
    float outsideDistance = length(max(d, 0.0));
    return insideDistance + outsideDistance;
}

float cubeSDF(vec3 p, vec3 s){
    vec3 d = abs(p) - s;
    float insideDistance = min(max(d.x, max(d.y, d.z)), 0.0);
    float outsideDistance = length(max(d, 0.0));
    return insideDistance + outsideDistance;
}


float SDF(vec3 p){
    float cubeSize = 0.2;
    float dist_cubes = cubeSDF(trans(p), 0.01*cubeHeight(p));
    // float dist_cubes = cubeSDF(trans(p), 0.01+0.25*beat(iGlobalTime/2.0-length(floor(p))/64.0, 8.0));
    return dist_cubes;
    // return intersectSDF(height_bound, dist_cubes);
}

vec3 getNormal(vec3 p){
  float d = 0.00001;
  return normalize(vec3(
                   SDF(p+vec3(d, 0.0, 0.0)) - SDF(p+vec3(-d, 0.0, 0.0)),
                   SDF(p+vec3(0.0, d, 0.0)) - SDF(p+vec3(0.0, -d, 0.0)),
                   SDF(p+vec3(0.0, 0.0, d)) - SDF(p+vec3(0.0, 0.0, -d))
                   ));
}

vec2 frac(vec2 a, vec2 b){
    return vec2(a.x/b.x, a.y/b.y);
}

vec3 path(float t){
    float psudo_t = t/3.0;
    return vec3(cam_dist*cos(psudo_t), -20.0, cam_dist*sin(psudo_t));
}

vec2 twist(vec2 pos, float angle){
    float ca = cos(angle);
    float sa = sin(angle);
    mat2 rot = mat2(ca,sa,-sa,ca);
    return rot*pos;
}

void main()
{
    vec2 p = 2.0*frac(gl_FragCoord.xy, iResolution)-1.0;
    float ratio = iResolution.x/iResolution.y;
    p.x = ratio*p.x;

    vec3 cPos =  path(iGlobalTime/10.0);
    vec3 cDir =  -cPos/length(cPos);
    vec3 cUp =   vec3(sin(iGlobalTime/10.0), cos(iGlobalTime/10.0), 0.0);
    vec3 cSide = cross(cDir, cUp);
    
    float targetDepth = 1.0;
    vec3 ray = normalize(cSide*p.x + cUp*p.y + cDir*targetDepth);
  
    // marching loop
    float distance = 0.0;
    float rLen = 0.0;
    vec3 rPos = cPos;
    for(int i = 0; i < 64; ++i){
        distance = SDF(rPos);
        rLen += distance;
        rPos = cPos + ray*rLen;
    }
    if(abs(distance) < 0.001){
        vec3 normal = getNormal(rPos);
        // vec3 posColor = vec3()
        // vec3 lightDir = vec3(-0.577, 0.577, 0.577);
        // float diff = clamp(dot(lightDir, normal), 0.25, 1.0);
        
        gl_FragColor = mix(albedo, vec4(normal, 1.0), 0.2);
    }else{
        float r = 0.0;
        float g = 0.3-0.05*p.y*sin(iGlobalTime/P2);
        float b = 0.5+0.2*p.x*cos(iGlobalTime/P2);
        gl_FragColor = vec4(r, g, b, 1.0);
    }
}
