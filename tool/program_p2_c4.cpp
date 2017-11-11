#include "program_p2_c4.h"

PROGRAM_P2_C4::PROGRAM_P2_C4()
{
    _position   =   -1;
    _color      =   -1;
    _uv         =   -1;
    _uv1        =   -1;
    _MVP        =   -1;
    _texture    =   -1;
    _texture1   =   -1;
}

bool PROGRAM_P2_C4::initialize()
{
    //!使用（varying），是表示需要在（vs）和（ps）之间进行传递的变量，把需要传递的变量赋值，并重复使用（varying）语句即可
    const char* vs  =
    {
        "precision lowp float; "
        "uniform   mat4 _MVP;"
        "attribute vec2 _position;"
        "attribute vec2 _uv;"
        "attribute vec2 _uv1;"
        "attribute vec4 _color;"
        "varying   vec4 _outColor;"
        "varying   vec2 _outUV;"
        "varying   vec2 _outUV1;"
        "void main()"
        "{"
        "   vec4    pos =   vec4(_position,0,1);"
        "   _outColor   =   _color;"
        "   _outUV      =   _uv;"
        "   _outUV1     =   _uv1;"
        "   gl_Position =   _MVP * pos;"
        "}"
    };
    const char* ps  =
    {
        "precision  lowp float; "
        "uniform    sampler2D   _texture;"
        "uniform    sampler2D   _texture1;"
        "varying    vec4        _outColor;"
        "varying    vec2        _outUV;"
        "varying    vec2        _outUV1;"
        "void main()"
        "{"
        "   vec4  tColor0   =   texture2D(_texture,_outUV);"
        "   vec4  tColor1   =   texture2D(_texture1,_outUV1);"
        "   gl_FragColor    =   tColor0+tColor1;"
        "}"
    };

    bool    res =   createProgram(vs,ps);
    if(res)
    {
        _position   =   glGetAttribLocation(_programId,"_position");
        _color      =   glGetAttribLocation(_programId,"_color");
        _uv         =   glGetAttribLocation(_programId,"_uv");
        _uv1        =   glGetAttribLocation(_programId,"_uv1");
        _MVP        =   glGetUniformLocation(_programId,"_MVP");
        _texture    =   glGetUniformLocation(_programId,"_texture");
        _texture1   =   glGetUniformLocation(_programId,"_texture1");
    }
    return  res;
}

void PROGRAM_P2_C4::begin()
{
    glUseProgram(_programId);
    //!在显卡里面使用的局部变量，在使用时是需要进行使能和关闭的
    glEnableVertexAttribArray(_position);       
    glEnableVertexAttribArray(_color);
    glEnableVertexAttribArray(_uv);
    glEnableVertexAttribArray(_uv1);
}

void PROGRAM_P2_C4::end()
{
    glDisableVertexAttribArray(_position);
    glDisableVertexAttribArray(_color);
    glDisableVertexAttribArray(_uv);
    glDisableVertexAttribArray(_uv1);
    glUseProgram(0);
}
