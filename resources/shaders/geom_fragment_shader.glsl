// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;

uniform float time;

void main()
{
    // Sample texture
    vec4 color = texture2D(onetex, uv_interp);


    // Uncomment this part for drawing a line
    // Animate line direction
    /*
    float t = time;
    float dx = cos(t);
    float dy = sin(t);
    vec2 line_dir = vec2(dx, dy);
    line_dir = normalize(line_dir);

    // Point that line goes through
    vec2 line_pnt = vec2(0.5, 0.5);

    // Get normal to line (perpendicular vector)
    vec2 normal = vec2(line_dir.y, -line_dir.x);
    normal = normalize(normal);

    // Compute distance to line by projecting on normal
    float p = abs(dot(normal, uv_interp.xy - line_pnt));

    // Use smoothstep to get smoother transition from black to white
    float s = smoothstep(0.1, 0, p);
    gl_FragColor = vec4(s, s, s, 1.0);
    */


    // Uncomment this part for drawing a progress bar
    // You may need to shift it around
    /*
    float s1 = max(abs(uv_interp.x-0.5)/0.25, abs(uv_interp.y-0.5)/0.25);
    s1 = abs(s1 - 1);
    float iv1 = smoothstep(0.05, 0.0, s1);
   
    float health = 1 - min(time/10.0, 1);
    float h = health*0.25;
    float s2 = max(abs(uv_interp.x-0.5)/0.25, abs(uv_interp.y-0.5)/h);
    float iv2 = smoothstep(1, 0.99, s2);

    float iv = max(iv1, iv2);
    if (health < 0.3){
        gl_FragColor = vec4(iv, 0, 0, 1.0);
    } else {
        gl_FragColor = vec4(0, 0, iv, 1.0);
    }
    */


    // Uncomment this part to draw a pie chart
    
    // Center of the circle
    vec2 center = vec2(0.5, 0.5);
    // Texture coordinates of the fragment centered at the circle
    vec2 pt = uv_interp - center;
    // Radius and angle of the fragment
    float r = length(pt);
    float ang = atan(pt.y, pt.x);
    // Check if radius is inside the circle and angle is less than
    // an amount. This amount is currently animated, but could be 
    // passed as a uniform
    float pi = 3.141592;
    float amount = pi*(time/10.0);
    float iv = 0.0; // Intensity value
    if ((r < 0.25) && (ang < amount)){
        iv = 1.0;
    }
    gl_FragColor = vec4(iv, 0, 0, 1.0);

   
    // Uncomment this part for checkerboard texture
    /*
    float s = smoothstep(0.1, 0, abs(uv_interp.x - 0.5));
    s = mod(floor(10*uv_interp.x) + floor(10*uv_interp.y), 2.0);
    gl_FragColor = vec4(s, s, s, 1.0);
    */


    // Check for transparency
    // You can add any custom tests here
    /*if(iv < 0.5)
    {
         discard;
    }*/
}
