// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;

// Configuration of the font texture
// Number of characters per row
const int num_columns = 18;
// Number of rows in the texture
const int num_rows = 7;
// Size of a character in texture units
float char_width = 1.0/float(num_columns);
float char_height = 1.0/float(num_rows);

// Text input
uniform int text_len;
uniform int text_content[40];

void main()
{

    // Check if we are writing on this sprite
    if (text_len > 0){
        // Find which character we are writing in the block covered by
        // this fragment
        // Get the index of the character in the text string according
        // to the x texture coordinate
        int string_index = int(floor(uv_interp.x*float(text_len)));

        // Get the character code
        int char_index = text_content[string_index] - 32; // Map space character to 0

        // Get character's row and column in the font texture
        int row = char_index / num_columns;
        int col = char_index - row*num_columns;

        // Map the fragment texture coordinates to a value between 0 and
        // 1 for a character
        vec2 cuv;
        cuv.x = uv_interp.x*float(text_len) - float(string_index);
        cuv.y = uv_interp.y;
        // Map the value between 0 and 1 to a coordinate in the font
        // texture
        vec2 fuv;
        fuv.x = (cuv.x - 0.15 + col)*char_width;
        fuv.y = (cuv.y + row)*char_height;
        // Draw character
        vec4 color = texture2D(onetex, fuv);
        //gl_FragColor = vec4(color.r, color.g, color.b, color.a);
        gl_FragColor = vec4(color.r, color.g, color.b, 1.0);

		 //Check for transparency
		if(color.a < 0.77)
		{
			 discard;
		}
    } 
}
