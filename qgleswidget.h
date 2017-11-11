#ifndef QGLESWIDGET_H
#define QGLESWIDGET_H

#include <QWidget>
#include <FreeImage.h>
#include "tool/CELLMath.hpp"
#include "tool/program_p2_c4.h"
#define MESA_EGL_NO_X11_HEADERS
#include <EGL/egl.h>


typedef struct
{
    CELL::float2 pos;
    CELL::float2 uv;
    CELL::float2 uv1;
    CELL::Rgba4Byte color;
}Vertex;

class QGLESWIDGET : public QWidget
{
    Q_OBJECT
public:
    explicit QGLESWIDGET(QWidget *parent = 0);
    ~QGLESWIDGET();

    //! 窗口的高度
    int         _width;
    //! 窗口的宽度
    int         _height;
    /// for gles2.0
    EGLConfig   _config;
    EGLSurface  _surface;
    EGLContext  _context;
    EGLDisplay  _display;

    //! 增加shader
    PROGRAM_P2_C4   _shader;
    unsigned int    _textureId;
    unsigned int    _textureId1;

    //对外链接API，初始化opengl
    bool init_QGW(std::vector<QString> fileName);

    //! 初始化 OpenGLES2.0
    bool    initOpenGLES20();
    //! 销毁OpenGLES2.0
    void    destroyOpenGLES20();
    virtual void render();
    virtual unsigned int loadTexture(const char* fileName);


    //对外链接API，实现显示功能
    void drawImage();
signals:

public slots:
};

#endif // QGLESWIDGET_H
