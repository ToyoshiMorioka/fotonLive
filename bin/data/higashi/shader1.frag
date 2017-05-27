#version 120
uniform float iGlobalTime;
uniform vec2 iResolution;
uniform float bpm;
uniform float history[64];
const float PI = 3.1415926;
const float P2 = 6.2831852;
const vec4 albedo = vec4(0.5, 0.0, 0.5, 1.0);
const float cam_dist = 47.0;

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

float beat(float t){
    float dt = 60.0/bpm;
    float fracture = mod(t, dt);
    float damp = 1/dt;
    return exp(-damp*fracture);
}

// float cubeSize(vec3 p){
//     float discreteDistance = length(floor(p));
//     return 0.3+0.2*sin(discreteDistance-2*iGlobalTime*bpm/60.0);
//     //return 0.4;
// }
float cubeSize(vec3 p){
    int i = int(4.0*mod(length(floor(p)), 16.0));
    return 0.1+0.9*history[i];
}

vec3 trans(vec3 p){
    return mod(p, 1.0) - 0.5;
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

float SDF(vec3 p){
    float dist_cubes = cubeSDF(trans(p), 0.1+0.4*cubeSize(p));
    float dist_bound = cubeSDF(p-vec3(-10.0, -10.0, -10.0), 22.0);
    dist_bound = unionSDF(dist_bound, cubeSDF(p-vec3(10.0, 10.0, 10.0), 22.0));
    return intersectSDF(dist_bound, dist_cubes);
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
    float beating = 5*beat(t);
    float psudo_t = t/3.0;
    float beat_cam = cam_dist+beating;
    return vec3(cam_dist*cos(psudo_t), -cam_dist*sin(psudo_t)/1.8, cam_dist*sin(psudo_t));
}

void main()
{
    vec2 p = 2.0*frac(gl_FragCoord.xy, iResolution)-1.0;
    float ratio = iResolution.x/iResolution.y;
    p.x = ratio*p.x;
    
    vec3 cPos =  path(-iGlobalTime);
    vec3 cDir =  -cPos/length(cPos);
    cDir /= length(cDir);
    vec3 cUp =   vec3(sin(iGlobalTime/20.0), cos(iGlobalTime/20.0), 0.0);
    vec3 cSide = cross(cDir, cUp);
    // vec3 cSide = path(iGlobalTime+0.1)-cPos;
    // cSide /= length(cSide);
    // vec3 cUp =   cross(cDir, cSide);
    float targetDepth = 1.0;
    vec3 ray = normalize(cSide*p.x + cUp*p.y + cDir*targetDepth);
  
    // marching loop
    float distance = 0.0;
    float rLen = 0.0;
    vec3 rPos = cPos;
     for(int i = 0; i < 32; ++i){
        distance = SDF(rPos);
        rLen += distance;
        rPos = cPos + ray*rLen;
    }
    if(abs(distance) < 0.001){
        vec3 normal = getNormal(rPos);
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
