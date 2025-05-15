#ifndef RENDERER_H_IS_INCLUDED

#include <string>
#include "ysgl.h"

class RendererBase
{
public:
    GLuint programIdent;
    GLuint vertexShaderIdent,fragmentShaderIdent;

	bool CompileFile(std::string vtxShaderFn,std::string fragShaderFn);
    bool Compile(const std::string &vtxShaderSource,const std::string &fragShaderSource);
protected:
    bool CompileShader(int shaderIdent);
    bool LinkShader(void);
    virtual void CacheAttributeAndUniformIdent(void)=0;
};

class PassThroughRenderer : public RendererBase
{
public:
    GLuint attribVertexPos;
    virtual void CacheAttributeAndUniformIdent(void);
};

#endif
