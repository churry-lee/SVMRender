#ifndef _SVMTEX_HPP_
#define _SVMTEX_HPP_

#include "svmCore.hpp"
#include "svmProgram.hpp"
#include "svmMesh.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

class Texture
{
public:
    Texture() = default;
    ~Texture() = default;
    static void Init(int texUnitID, GLuint & textureID);

private:
};

#endif //_SVMTEX_HPP_
