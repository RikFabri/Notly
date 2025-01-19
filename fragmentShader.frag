#version 450 core
out vec4 FragColor;

layout (std430, binding = 1) buffer TextBuffer {
	uint chars[];
};

in vec2 TexCoord;

uniform sampler2D fontAtlas;

//vec3 indexToColor(uint index) {
//    return vec3(mod(float(index), 1.0), mod(float(index) * 0.5, 1.0), mod(float(index) * 0.25, 1.0));
//}

vec3 indexToColor(uint index, uint total) {
    float normalizedIndex = float(index) / float(total);
    return vec3(normalizedIndex, 1.0 - normalizedIndex, 0.5);
}

//vec3 indexToColor(uint index) {
//    return vec3(
//        mod(float(index), 256.0),            // Red channel
//        mod(float(index) * 1.5, 256.0),      // Green channel
//        mod(float(index) * 2.5, 256.0)       // Blue channel
//    );
//}

void main()
{
    // Viewport data
    uint viewportWidth = 480;
    uint viewportHeight = 512;
    uint viewportCharWidth = 32;

    uint viewportCharHeight = viewportCharWidth;
    uint viewportNumCharsH = viewportWidth / viewportCharWidth;
    uint viewportNumCharsV = viewportHeight / viewportCharHeight;


    vec2 viewportDimensions = vec2(viewportWidth, viewportHeight);

    ivec2 texCoordsViewport = ivec2(floor(TexCoord * viewportDimensions)); // absolute pixels

    // Monospace atlas data
    uint atlasSize = 512;
    uint atlasNumChars = 16; // per dimension
    // uint atlasCharSize = atlasSize / atlasNumChars; // should be 32
    uint atlasCharSize = 32;

    // Which character are we rendering
    // texCoordsViewport origin is bottom left, but index starts top left, thus invert y-axis
    uint invertedY = viewportHeight - texCoordsViewport.y;
    uint index = invertedY / viewportCharHeight * viewportNumCharsH + texCoordsViewport.x / viewportCharWidth;
    uint currentChar = chars[index];

    // Where is this character's origin in the atlas?
    uint horizontalIdxTL = currentChar % atlasNumChars;
    uint verticalIdxTl = currentChar / atlasNumChars;

    // Convert indices into atlas space coordinates BL
    uint characterPosX = horizontalIdxTL * atlasCharSize;
    uint characterPosY = atlasSize - (verticalIdxTl * atlasCharSize + atlasCharSize);
    vec2 characterOriginAtlasSpace = vec2(characterPosX, characterPosY);


    // Here be dragons

    // Which pixel within our character do we need to render
    vec2 offsetFromCharOrigin = texCoordsViewport; // absolute pixels in viewport space
    offsetFromCharOrigin.x = texCoordsViewport.x % viewportCharWidth;
    offsetFromCharOrigin.y = texCoordsViewport.y % viewportCharHeight;
    offsetFromCharOrigin = (offsetFromCharOrigin / viewportDimensions) * float(atlasSize);

    FragColor = texture(fontAtlas, (characterOriginAtlasSpace + offsetFromCharOrigin) / atlasSize);

    // FragColor = vec4(indexToColor(uint(characterPosY), 512), 1.0);
    //FragColor = vec4(offsetFromCharOrigin, 1.0, 1.0);
    
}
