#ifdef GL_ES
precision highp float;
#endif

uniform float time;
uniform vec2 resolution;
uniform vec4 mouse;
uniform sampler2D tex0;
uniform sampler2D tex1;

void main(void)
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    vec2 d = uv - vec2(0.5, 0.5);
    float l = length(d);
    float t = (time/1.5 - floor(time/1.5))*1.5;
    float theta = clamp((l-t) * 3.14 * 10.0, -3.14/2.0, 3.14/2.0);
    float h = cos(theta) * 0.2; 
    float dl = - h * tan(13.0/45.0 * atan(sin(theta)));
    vec3 col = texture2D(tex0,vec2(uv.x + dl/l * (uv.x - 0.5), 
               uv.y + dl/l * (uv.y - 0.5))).xyz;

    gl_FragColor = vec4(col,1.0);
}
