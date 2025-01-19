#version 450 core
out vec4 FragColor;

layout (std430, binding = 1) buffer TextBuffer {
	uint chars[];
};

in vec2 TexCoord;

uniform sampler2D fontAtlas;
uniform ivec2 WindowSize;

vec3 indexToColor(uint index, uint total) {
    float normalizedIndex = float(index) / float(total);
    return vec3(normalizedIndex, 1.0 - normalizedIndex, 0.5);
}

void main()
{
    // Monospace atlas data
    uint atlasSize = 512;
    uint atlasCharSizeH = 16;
    uint atlasCharSizeV = 32;
    uint atlasNumCharsHorizontal = atlasSize / atlasCharSizeH;
    uint atlasNumCharsVertical = atlasSize / atlasCharSizeV;

    // Viewport data
    uint viewportWidth = WindowSize.x;
    uint viewportHeight = WindowSize.y;

    //    uint viewportCharWidth = atlasCharSizeH;  // These can be uniforms for font scaling
    //    uint viewportCharHeight = atlasCharSizeV; // These can be uniforms for font scaling
    uint viewportCharWidth = 18;
    uint viewportCharHeight = 20;

    viewportWidth = viewportWidth - viewportWidth % viewportCharWidth;
    viewportHeight = viewportHeight - viewportHeight % viewportCharHeight;

    ivec2 viewportGlyphDimensions = ivec2(viewportCharWidth, viewportCharHeight);
    ivec2 atlasGlyphDimensions = ivec2(atlasCharSizeH, atlasCharSizeV);

    uint viewportNumCharsH = viewportWidth / viewportCharWidth;
    uint viewportNumCharsV = viewportHeight / viewportCharHeight;

    vec2 viewportDimensions = vec2(viewportWidth, viewportHeight);

    ivec2 texCoordsViewport = ivec2(floor(TexCoord * viewportDimensions)); // absolute pixels

    // Which character are we rendering
    // texCoordsViewport origin is bottom left, but index starts top left, thus invert y-axis
    uint invertedY = viewportHeight - texCoordsViewport.y;
    uint index = invertedY / viewportCharHeight * viewportNumCharsH + texCoordsViewport.x / viewportCharWidth;
    uint currentChar = chars[index];

    // Where is this character's origin in the atlas?
    uint horizontalIdxTL = currentChar % atlasNumCharsHorizontal;
    uint verticalIdxTl = currentChar / atlasNumCharsHorizontal;

    // Convert indices into atlas space coordinates BL
    uint characterPosX = horizontalIdxTL * atlasCharSizeH;
    uint characterPosY = atlasSize - (verticalIdxTl * atlasCharSizeV + atlasCharSizeV);
    vec2 characterOriginAtlasSpace = vec2(characterPosX, characterPosY);

    // Which pixel within our character do we need to render
    vec2 offsetFromCharOrigin = texCoordsViewport; // absolute pixels in viewport space
    offsetFromCharOrigin.x = texCoordsViewport.x % viewportCharWidth;
    offsetFromCharOrigin.y = texCoordsViewport.y % viewportCharHeight;
    offsetFromCharOrigin = (offsetFromCharOrigin / viewportGlyphDimensions) * atlasGlyphDimensions;

    FragColor = texture(fontAtlas, (characterOriginAtlasSpace + offsetFromCharOrigin) / atlasSize);

    // FragColor = vec4(indexToColor(uint(characterPosY), 512), 1.0);
    //FragColor = vec4(offsetFromCharOrigin, 1.0, 1.0);
    
}
