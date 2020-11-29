#ifndef __MODULEPROGRAM_H__
#define __MODULEPROGRAM_H__
#include "Module.h"


class ModuleProgram :
    public Module

{
public:
    bool Init() override;
    update_status PreUpdate(float deltaTime) override;
    update_status Update(float deltaTime) override;
    update_status PostUpdate(float deltaTime) override;
    bool CleanUp() override;

    char* LoadShaderSource(const char* shaderFileName);

    unsigned CompileShader(unsigned type, const char* source);

    unsigned CreateProgram(unsigned vtxShader, unsigned frgShader);

    unsigned CreateProgramFromSource(const char* vtxShader, const char* frgShader);
    
};

#endif // __MODULEPROGRAM_H__