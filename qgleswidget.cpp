#include "qgleswidget.h"

QGLESWIDGET::QGLESWIDGET(QWidget *parent) : QWidget(parent)
{

}

QGLESWIDGET::~QGLESWIDGET()
{
    destroyOpenGLES20();
}

bool QGLESWIDGET::init_QGW(std::vector<QString> fileName)
{
    if(!initOpenGLES20())
    {
        return false;
    }
    _shader.initialize();
    QString name0 = fileName[0];
    QString name1 = fileName[1];
    _textureId  = loadTexture(name0.toLatin1().data());
    _textureId1 = loadTexture(name1.toLatin1().data());
}

bool QGLESWIDGET::initOpenGLES20()
{
    const EGLint attribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE,24,
        EGL_NONE
    };
    EGLint 	format(0);
    EGLint	numConfigs(0);
    EGLint  major;
    EGLint  minor;

    //! 1
    _display	    =	eglGetDisplay(EGL_DEFAULT_DISPLAY);

    //! 2init
    eglInitialize(_display, &major, &minor);

    //! 3
    eglChooseConfig(_display, attribs, &_config, 1, &numConfigs);

    eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format);
    //!!! 4 使opengl与qt的窗口进行绑定<this->winId()>
    _surface	    = 	eglCreateWindowSurface(_display, _config, this->winId(), NULL);

    //! 5
    EGLint attr[]   =   { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    _context 	    = 	eglCreateContext(_display, _config, 0, attr);
    //! 6
    if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE)
    {
        return false;
    }

    eglQuerySurface(_display, _surface, EGL_WIDTH,  &_width);
    eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);

    return  true;
}

void QGLESWIDGET::destroyOpenGLES20()
{
    if (_display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (_context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(_display, _context);
        }
        if (_surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(_display, _surface);
        }
        eglTerminate(_display);
    }
    _display    =   EGL_NO_DISPLAY;
    _context    =   EGL_NO_CONTEXT;
    _surface    =   EGL_NO_SURFACE;//asdsafsaf
}

void QGLESWIDGET::render()
{
    //! 清空缓冲区
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //! 视口，在Windows窗口指定的位置和大小上绘制OpenGL内容
    glViewport(0,0,_width,_height);

    //! 创建一个投影矩阵
    //当图像显示出来与坐标的位置出现导致现象时，可以改变投影矩阵的坐标，使之对齐
    CELL::matrix4   screenProj  =   CELL::ortho<float>(0,float(_width),float(_height),0,-100.0f,100);
    _shader.begin();
    {
        static float inc = 0;
        inc += 0.01f;
        float   x   =   50;
        float   y   =   50;
        float   w   =   400;
        float   h   =   400;
        Vertex  vect[]   =
        {
            CELL::float2(x,y),         CELL::float2(0,0),   CELL::float2(0+inc,0),    CELL::Rgba4Byte(255,0,0,255),
            CELL::float2(x + w,y),     CELL::float2(1,0),   CELL::float2(1+inc,0),    CELL::Rgba4Byte(0,255,0,255),
            CELL::float2(x,y + h),     CELL::float2(0,1),   CELL::float2(0+inc,1),    CELL::Rgba4Byte(0,0,255,255),
            CELL::float2(x + w, y + h),CELL::float2(1,1),   CELL::float2(1+inc,1),    CELL::Rgba4Byte(255,255,255,255),
        };
#if 0
        //!使用纹理1个
        {
            //!允许建立一个绑定到目标纹理的有名称的纹理
            glBindTexture(GL_TEXTURE_2D,_textureId);

            glUniformMatrix4fv(_shader._MVP, 1, false, screenProj.data());
            //!对纹理采样器变量进行设置
            glUniform1i(_shader._texture, 0);
            glVertexAttribPointer(_shader._position,2,GL_FLOAT,false,sizeof(Vertex),&(vect[0].pos));
            //!函数定义：指定了渲染时索引值为 index 的顶点属性数组的数据格式和位置
            //!参数1：指定要修改的顶点属性的索引值
            //!参数2：指定每个顶点属性的组件数量
            //!参数3：指定数组中每个组件的数据类型
            //!参数4：指定当被访问时，固定点数据值是否应该被归一化（GL_TRUE）或者直接转换为固定点值（GL_FALSE）
            //!参数5：指定连续顶点属性之间的偏移量
            //!参数6：指定第一个组件在数组的第一个顶点属性中的偏移量
            glVertexAttribPointer(_shader._uv,2,GL_FLOAT,false,sizeof(Vertex),&(vect[0].uv));
            //注意：由于颜色值是取的（0~255），而opengl需要的是浮点数，所以Rgba需要归一化到（0~1）
            glVertexAttribPointer(_shader._color,4,GL_UNSIGNED_BYTE,true,sizeof(Vertex),&(vect[0].color));
            glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        }
#else
        //!使用纹理2个
        //! 这里添加2个文理之后把shader里面的参数变化了，所以就不适用只有一张文理的了
        {
            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D,_textureId);

            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D,_textureId1);

            glUniformMatrix4fv(_shader._MVP, 1, false, screenProj.data());
            glUniform1i(_shader._texture, 0);
            glUniform1i(_shader._texture1, 1);

            glVertexAttribPointer(_shader._position,2,GL_FLOAT,false,sizeof(Vertex),&(vect[0].pos));
            glVertexAttribPointer(_shader._uv,2,GL_FLOAT,false,sizeof(Vertex),&(vect[0].uv));
            glVertexAttribPointer(_shader._uv1,2,GL_FLOAT,false,sizeof(Vertex),&(vect[0].uv1));
            glVertexAttribPointer(_shader._color,4,GL_UNSIGNED_BYTE,true,sizeof(Vertex),&(vect[0].color));
            glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        }
#endif
    }
    _shader.end();
}

unsigned int QGLESWIDGET::loadTexture(const char *fileName)
{
    unsigned int textureId   =   0;
    //1 获取图片格式
    FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);

    //2 加载图片
    FIBITMAP    *dib = FreeImage_Load(fifmt, fileName,0);

    //3 转化为rgb 24色
    dib     =   FreeImage_ConvertTo24Bits(dib);

    //4 获取数据指针
    BYTE    *pixels =   (BYTE*)FreeImage_GetBits(dib);

    int     width   =   FreeImage_GetWidth(dib);
    int     height  =   FreeImage_GetHeight(dib);

    for (size_t i = 0 ;i < width * height * 3 ; i+=3 )
    {
        BYTE temp       =   pixels[i];
        pixels[i]       =   pixels[i + 2];
        pixels[i + 2]   =   temp;
    }
    /**
        *   产生一个纹理Id,可以认为是纹理句柄，后面的操作将书用这个纹理id
        */
    glGenTextures( 1, &textureId );

    /**
        *   使用这个纹理id,或者叫绑定(关联)
        */
    glBindTexture( GL_TEXTURE_2D, textureId );
    /**
        *   指定纹理的放大,缩小滤波，使用线性方式，即当图片放大的时候插值方式
        */
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    /**
        *   将图片的rgb数据上传给opengl.
        */
    glTexImage2D(
                GL_TEXTURE_2D,      //! 指定是二维图片
                0,                  //! 指定为第一级别，纹理可以做mipmap,即lod,离近的就采用级别大的，远则使用较小的纹理
                GL_RGB,             //! 纹理的使用的存储格式
                width,              //! 宽度，老一点的显卡，不支持不规则的纹理，即宽度和高度不是2^n。
                height,             //! 宽度，老一点的显卡，不支持不规则的纹理，即宽度和高度不是2^n。
                0,                  //! 是否存在边
                GL_RGB,             //! 数据的格式，bmp中，windows,操作系统中存储的数据是bgr格式
                GL_UNSIGNED_BYTE,   //! 数据是8bit数据
                pixels
                );
    /**
        *   释放内存
        */
    FreeImage_Unload(dib);

    return  textureId;
}

void QGLESWIDGET::drawImage()
{
    render();
    eglSwapBuffers(_display,_surface);
}
