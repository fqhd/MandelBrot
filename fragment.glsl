precision highp float;
const int max_iterations = 200;
const float infinit_value = 50.0;
const float box_size = 1000.0;
const float mapped_value = 2.0;

float map(float value, float i_start, float i_stop, float o_start, float o_stop) {
    return o_start + (o_stop - o_start) * ((value - i_start) / (i_stop - i_start));
}

float myClamp(float a, float b, float c){
    if(a <= b){
        return b;
    }else if(a >= c){
        return c;
    }
    return a;
}

int mod(int x, int y){
    return x - int(floor(float(x)/float(y)));
}

vec3 HSVtoRGB(float h, float s, float v) {
    float r, g, b, i, f, p, q, t;
    i = floor(h * 6.0);
    f = h * 6.0 - i;
    p = v * (1.0 - s);
    q = v * (1.0 - f * s);
    t = v * (1.0 - (1.0 - f) * s);

    int index = mod(int(i), 6);

    if(i == 0.0){
        r = v;
        g = t;
        b = p;
    }else if(i == 1.0){
        r = q;
        g = v;
        b = p;
    }else if(i == 2.0){
        r = p;
        g = v;
        b = t;
    }else if(i == 3.0){
        r = p;
        g = q;
        b = v;
    }else if(i == 4.0){
        r = t;
        g = p;
        b = v;
    }else if(i == 5.0){
        r = v;
        g = p;
        b = q;
    }

    return vec3(r, g, b);
}

uniform float zoom;

void main(){
    float e = box_size / zoom;
    float p = box_size - e;
    float x = 79.0 + (gl_FragCoord.x / zoom) + p/2.0;
    float y = 124.0 + (gl_FragCoord.y / zoom) + p/2.0;

    float a = map(x, 0.0, box_size, -mapped_value, mapped_value);
    float b = map(y, 0.0, box_size, -mapped_value, mapped_value);

    float cA = a;
    float cB = b;
    int n;
    for(int z = 0; z < max_iterations; z++){
        float re = a*a - b*b;
        float im = 2.0*a*b;
        a = re + cA;
        b = im + cB;
        if(a*a + b*b > infinit_value){
            break;
        }
        n = z;
    }

    float hue = float(n) / float(max_iterations);
    float saturation = 1.0;
    float value;
    if(n < max_iterations - 1){
        value = 1.0;
    }else{
        value = 0.0;
    }

    vec3 color = HSVtoRGB(hue, saturation, value);

    gl_FragColor = vec4(color, 1.0);
}