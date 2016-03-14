// g buffer
//layout(binding=value) value = texture unit
// used layout binding so set uniform does not have to be called in application

layout(binding=0) uniform sampler2D gBufferPosition;
layout(binding=1) uniform sampler2D gBufferDiffuse;
layout(binding=2) uniform sampler2D gBufferNormal;

// will eventually be used for position reconstuction
//layout(binding=3) uniform sampler2D gBufferDepth;
