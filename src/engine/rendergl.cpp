// rendergl.cpp: core opengl rendering stuff

#include "engine.h"

#include "targeting.h" // INTENSITY
#include "client_system.h"

bool hasVAO = false, hasTR = false, hasTSW = false, hasFBO = false, hasAFBO = false, hasDS = false, hasTF = false, hasCBF = false, hasS3TC = false, hasFXT1 = false, hasLATC = false, hasRGTC = false, hasAF = false, hasFBB = false, hasFBMS = false, hasTMS = false, hasMSS = false, hasFBMSBS = false, hasNVFBMSC = false, hasNVTMS = false, hasUBO = false, hasMBR = false, hasDB2 = false, hasDBB = false, hasTG = false, hasTQ = false, hasPF = false, hasTRG = false, hasTI = false, hasHFV = false, hasHFP = false, hasDBT = false, hasDC = false, hasDBGO = false, hasEGPU4 = false, hasGPU4 = false, hasGPU5 = false, hasEAL = false, hasCR = false, hasOQ2 = false, hasCB = false, hasCI = false;
bool mesa = false, intel = false, amd = false, nvidia = false;

int hasstencil = 0;

VAR(renderpath, 1, 0, 0);
VAR(glversion, 1, 0, 0);
VAR(glslversion, 1, 0, 0);

// GL_EXT_timer_query
PFNGLGETQUERYOBJECTI64VEXTPROC glGetQueryObjecti64v_  = NULL;
PFNGLGETQUERYOBJECTUI64VEXTPROC glGetQueryObjectui64v_ = NULL;

// GL_EXT_framebuffer_object
PFNGLBINDRENDERBUFFERPROC        glBindRenderbuffer_        = NULL;
PFNGLDELETERENDERBUFFERSPROC     glDeleteRenderbuffers_     = NULL;
PFNGLGENFRAMEBUFFERSPROC         glGenRenderbuffers_        = NULL;
PFNGLRENDERBUFFERSTORAGEPROC     glRenderbufferStorage_     = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC  glCheckFramebufferStatus_  = NULL;
PFNGLBINDFRAMEBUFFERPROC         glBindFramebuffer_         = NULL;
PFNGLDELETEFRAMEBUFFERSPROC      glDeleteFramebuffers_      = NULL;
PFNGLGENFRAMEBUFFERSPROC         glGenFramebuffers_         = NULL;
PFNGLFRAMEBUFFERTEXTURE1DPROC    glFramebufferTexture1D_    = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC    glFramebufferTexture2D_    = NULL;
PFNGLFRAMEBUFFERTEXTURE3DPROC    glFramebufferTexture3D_    = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer_ = NULL;
PFNGLGENERATEMIPMAPPROC          glGenerateMipmap_          = NULL;

// GL_EXT_framebuffer_blit
PFNGLBLITFRAMEBUFFERPROC         glBlitFramebuffer_         = NULL;

// GL_EXT_framebuffer_multisample
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample_ = NULL;

// GL_ARB_texture_multisample
PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample_ = NULL;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample_ = NULL;
PFNGLGETMULTISAMPLEFVPROC      glGetMultisamplefv_      = NULL;
PFNGLSAMPLEMASKIPROC           glSampleMaski_           = NULL;

// GL_ARB_sample_shading
PFNGLMINSAMPLESHADINGPROC glMinSampleShading_ = NULL;

// GL_ARB_draw_buffers_blend
PFNGLBLENDEQUATIONIPROC         glBlendEquationi_         = NULL;
PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei_ = NULL;
PFNGLBLENDFUNCIPROC             glBlendFunci_             = NULL;
PFNGLBLENDFUNCSEPARATEIPROC     glBlendFuncSeparatei_     = NULL;

// GL_NV_framebuffer_multisample_coverage
PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC glRenderbufferStorageMultisampleCoverageNV_ = NULL;

// GL_NV_texture_multisample
PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC     glTexImage2DMultisampleCoverageNV_     = NULL;
PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC     glTexImage3DMultisampleCoverageNV_     = NULL;
PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC         glTextureImage2DMultisampleNV_         = NULL;
PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC         glTextureImage3DMultisampleNV_         = NULL;
PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC glTextureImage2DMultisampleCoverageNV_ = NULL;
PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC glTextureImage3DMultisampleCoverageNV_ = NULL;

// OpenGL 1.3
#ifdef WIN32
PFNGLACTIVETEXTUREPROC    glActiveTexture_    = NULL;

PFNGLBLENDEQUATIONEXTPROC glBlendEquation_ = NULL;
PFNGLBLENDCOLOREXTPROC    glBlendColor_    = NULL;

PFNGLTEXIMAGE3DPROC        glTexImage3D_        = NULL;
PFNGLTEXSUBIMAGE3DPROC     glTexSubImage3D_     = NULL;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D_ = NULL;

PFNGLCOMPRESSEDTEXIMAGE3DPROC    glCompressedTexImage3D_    = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC    glCompressedTexImage2D_    = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DPROC    glCompressedTexImage1D_    = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D_ = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D_ = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D_ = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEPROC   glGetCompressedTexImage_   = NULL;

PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements_ = NULL;
#endif

// OpenGL 2.1
#ifndef __APPLE__
PFNGLMULTIDRAWARRAYSPROC   glMultiDrawArrays_   = NULL;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements_ = NULL;

PFNGLBLENDFUNCSEPARATEPROC     glBlendFuncSeparate_     = NULL;
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate_ = NULL;
PFNGLSTENCILOPSEPARATEPROC     glStencilOpSeparate_     = NULL;
PFNGLSTENCILFUNCSEPARATEPROC   glStencilFuncSeparate_   = NULL;
PFNGLSTENCILMASKSEPARATEPROC   glStencilMaskSeparate_   = NULL;

PFNGLGENBUFFERSPROC       glGenBuffers_       = NULL;
PFNGLBINDBUFFERPROC       glBindBuffer_       = NULL;
PFNGLMAPBUFFERPROC        glMapBuffer_        = NULL;
PFNGLUNMAPBUFFERPROC      glUnmapBuffer_      = NULL;
PFNGLBUFFERDATAPROC       glBufferData_       = NULL;
PFNGLBUFFERSUBDATAPROC    glBufferSubData_    = NULL;
PFNGLDELETEBUFFERSPROC    glDeleteBuffers_    = NULL;
PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData_ = NULL;

PFNGLGENQUERIESPROC        glGenQueries_        = NULL;
PFNGLDELETEQUERIESPROC     glDeleteQueries_     = NULL;
PFNGLBEGINQUERYPROC        glBeginQuery_        = NULL;
PFNGLENDQUERYPROC          glEndQuery_          = NULL;
PFNGLGETQUERYIVPROC        glGetQueryiv_        = NULL;
PFNGLGETQUERYOBJECTIVPROC  glGetQueryObjectiv_  = NULL;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv_ = NULL;

PFNGLCREATEPROGRAMPROC            glCreateProgram_            = NULL;
PFNGLDELETEPROGRAMPROC            glDeleteProgram_            = NULL;
PFNGLUSEPROGRAMPROC               glUseProgram_               = NULL;
PFNGLCREATESHADERPROC             glCreateShader_             = NULL;
PFNGLDELETESHADERPROC             glDeleteShader_             = NULL;
PFNGLSHADERSOURCEPROC             glShaderSource_             = NULL;
PFNGLCOMPILESHADERPROC            glCompileShader_            = NULL;
PFNGLGETSHADERIVPROC              glGetShaderiv_              = NULL;
PFNGLGETPROGRAMIVPROC             glGetProgramiv_             = NULL;
PFNGLATTACHSHADERPROC             glAttachShader_             = NULL;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog_        = NULL;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog_         = NULL;
PFNGLLINKPROGRAMPROC              glLinkProgram_              = NULL;
PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation_       = NULL;
PFNGLUNIFORM1FPROC                glUniform1f_                = NULL;
PFNGLUNIFORM2FPROC                glUniform2f_                = NULL;
PFNGLUNIFORM3FPROC                glUniform3f_                = NULL;
PFNGLUNIFORM4FPROC                glUniform4f_                = NULL;
PFNGLUNIFORM1FVPROC               glUniform1fv_               = NULL;
PFNGLUNIFORM2FVPROC               glUniform2fv_               = NULL;
PFNGLUNIFORM3FVPROC               glUniform3fv_               = NULL;
PFNGLUNIFORM4FVPROC               glUniform4fv_               = NULL;
PFNGLUNIFORM1IPROC                glUniform1i_                = NULL;
PFNGLUNIFORM2IPROC                glUniform2i_                = NULL;
PFNGLUNIFORM3IPROC                glUniform3i_                = NULL;
PFNGLUNIFORM4IPROC                glUniform4i_                = NULL;
PFNGLUNIFORM1IVPROC               glUniform1iv_               = NULL;
PFNGLUNIFORM2IVPROC               glUniform2iv_               = NULL;
PFNGLUNIFORM3IVPROC               glUniform3iv_               = NULL;
PFNGLUNIFORM4IVPROC               glUniform4iv_               = NULL;
PFNGLUNIFORMMATRIX2FVPROC         glUniformMatrix2fv_         = NULL;
PFNGLUNIFORMMATRIX3FVPROC         glUniformMatrix3fv_         = NULL;
PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv_         = NULL;
PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation_       = NULL;
PFNGLGETACTIVEUNIFORMPROC         glGetActiveUniform_         = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray_  = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray_ = NULL;

PFNGLVERTEXATTRIB1FPROC           glVertexAttrib1f_           = NULL;
PFNGLVERTEXATTRIB1FVPROC          glVertexAttrib1fv_          = NULL;
PFNGLVERTEXATTRIB1SPROC           glVertexAttrib1s_           = NULL;
PFNGLVERTEXATTRIB1SVPROC          glVertexAttrib1sv_          = NULL;
PFNGLVERTEXATTRIB2FPROC           glVertexAttrib2f_           = NULL;
PFNGLVERTEXATTRIB2FVPROC          glVertexAttrib2fv_          = NULL;
PFNGLVERTEXATTRIB2SPROC           glVertexAttrib2s_           = NULL;
PFNGLVERTEXATTRIB2SVPROC          glVertexAttrib2sv_          = NULL;
PFNGLVERTEXATTRIB3FPROC           glVertexAttrib3f_           = NULL;
PFNGLVERTEXATTRIB3FVPROC          glVertexAttrib3fv_          = NULL;
PFNGLVERTEXATTRIB3SPROC           glVertexAttrib3s_           = NULL;
PFNGLVERTEXATTRIB3SVPROC          glVertexAttrib3sv_          = NULL;
PFNGLVERTEXATTRIB4FPROC           glVertexAttrib4f_           = NULL;
PFNGLVERTEXATTRIB4FVPROC          glVertexAttrib4fv_          = NULL;
PFNGLVERTEXATTRIB4SPROC           glVertexAttrib4s_           = NULL;
PFNGLVERTEXATTRIB4SVPROC          glVertexAttrib4sv_          = NULL;
PFNGLVERTEXATTRIB4BVPROC          glVertexAttrib4bv_          = NULL;
PFNGLVERTEXATTRIB4IVPROC          glVertexAttrib4iv_          = NULL;
PFNGLVERTEXATTRIB4UBVPROC         glVertexAttrib4ubv_         = NULL;
PFNGLVERTEXATTRIB4UIVPROC         glVertexAttrib4uiv_         = NULL;
PFNGLVERTEXATTRIB4USVPROC         glVertexAttrib4usv_         = NULL;
PFNGLVERTEXATTRIB4NBVPROC         glVertexAttrib4Nbv_         = NULL;
PFNGLVERTEXATTRIB4NIVPROC         glVertexAttrib4Niv_         = NULL;
PFNGLVERTEXATTRIB4NUBPROC         glVertexAttrib4Nub_         = NULL;
PFNGLVERTEXATTRIB4NUBVPROC        glVertexAttrib4Nubv_        = NULL;
PFNGLVERTEXATTRIB4NUIVPROC        glVertexAttrib4Nuiv_        = NULL;
PFNGLVERTEXATTRIB4NUSVPROC        glVertexAttrib4Nusv_        = NULL;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer_      = NULL;

PFNGLUNIFORMMATRIX2X3FVPROC       glUniformMatrix2x3fv_       = NULL;
PFNGLUNIFORMMATRIX3X2FVPROC       glUniformMatrix3x2fv_       = NULL;
PFNGLUNIFORMMATRIX2X4FVPROC       glUniformMatrix2x4fv_       = NULL;
PFNGLUNIFORMMATRIX4X2FVPROC       glUniformMatrix4x2fv_       = NULL;
PFNGLUNIFORMMATRIX3X4FVPROC       glUniformMatrix3x4fv_       = NULL;
PFNGLUNIFORMMATRIX4X3FVPROC       glUniformMatrix4x3fv_       = NULL;

PFNGLDRAWBUFFERSPROC glDrawBuffers_ = NULL;
#endif

// OpenGL 3.0
PFNGLGETSTRINGIPROC           glGetStringi_           = NULL;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation_ = NULL;
PFNGLUNIFORM1UIPROC           glUniform1ui_           = NULL;
PFNGLUNIFORM2UIPROC           glUniform2ui_           = NULL;
PFNGLUNIFORM3UIPROC           glUniform3ui_           = NULL;
PFNGLUNIFORM4UIPROC           glUniform4ui_           = NULL;
PFNGLUNIFORM1UIVPROC          glUniform1uiv_          = NULL;
PFNGLUNIFORM2UIVPROC          glUniform2uiv_          = NULL;
PFNGLUNIFORM3UIVPROC          glUniform3uiv_          = NULL;
PFNGLUNIFORM4UIVPROC          glUniform4uiv_          = NULL;
PFNGLCLEARBUFFERIVPROC        glClearBufferiv_        = NULL;
PFNGLCLEARBUFFERUIVPROC       glClearBufferuiv_       = NULL;
PFNGLCLEARBUFFERFVPROC        glClearBufferfv_        = NULL;
PFNGLCLEARBUFFERFIPROC        glClearBufferfi_        = NULL;

// GL_EXT_draw_buffers2
PFNGLCOLORMASKIPROC glColorMaski_ = NULL;
PFNGLENABLEIPROC    glEnablei_    = NULL;
PFNGLDISABLEIPROC   glDisablei_   = NULL;

// GL_NV_conditional_render
PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender_ = NULL;
PFNGLENDCONDITIONALRENDERPROC   glEndConditionalRender_   = NULL;

// GL_EXT_texture_integer
PFNGLTEXPARAMETERIIVPROC     glTexParameterIiv_     = NULL;
PFNGLTEXPARAMETERIUIVPROC    glTexParameterIuiv_    = NULL;
PFNGLGETTEXPARAMETERIIVPROC  glGetTexParameterIiv_  = NULL;
PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv_ = NULL;
PFNGLCLEARCOLORIIEXTPROC     glClearColorIi_        = NULL;
PFNGLCLEARCOLORIUIEXTPROC    glClearColorIui_       = NULL;

// GL_ARB_uniform_buffer_object
PFNGLGETUNIFORMINDICESPROC       glGetUniformIndices_       = NULL;
PFNGLGETACTIVEUNIFORMSIVPROC     glGetActiveUniformsiv_     = NULL;
PFNGLGETUNIFORMBLOCKINDEXPROC    glGetUniformBlockIndex_    = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv_ = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC     glUniformBlockBinding_     = NULL;
PFNGLBINDBUFFERBASEPROC          glBindBufferBase_          = NULL;
PFNGLBINDBUFFERRANGEPROC         glBindBufferRange_         = NULL;

// GL_ARB_copy_buffer
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData_ = NULL;

// GL_EXT_depth_bounds_test
PFNGLDEPTHBOUNDSEXTPROC glDepthBounds_ = NULL;

// GL_ARB_color_buffer_float
PFNGLCLAMPCOLORPROC glClampColor_ = NULL;

// GL_ARB_debug_output
PFNGLDEBUGMESSAGECONTROLARBPROC  glDebugMessageControl_  = NULL;
PFNGLDEBUGMESSAGEINSERTARBPROC   glDebugMessageInsert_   = NULL;
PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallback_ = NULL;
PFNGLGETDEBUGMESSAGELOGARBPROC   glGetDebugMessageLog_   = NULL;

// GL_ARB_map_buffer_range
PFNGLMAPBUFFERRANGEPROC         glMapBufferRange_         = NULL;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange_ = NULL;

// GL_ARB_vertex_array_object
PFNGLBINDVERTEXARRAYPROC    glBindVertexArray_    = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays_ = NULL;
PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays_    = NULL;
PFNGLISVERTEXARRAYPROC      glIsVertexArray_      = NULL;

// GL_ARB_copy_image
PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData_ = NULL;

void *getprocaddress(const char *name)
{
    return SDL_GL_GetProcAddress(name);
}

VAR(glerr, 0, 0, 1);

void glerror(const char *file, int line, GLenum error)
{
    const char *desc = "unknown";
    switch(error)
    {
    case GL_NO_ERROR: desc = "no error"; break;
    case GL_INVALID_ENUM: desc = "invalid enum"; break;
    case GL_INVALID_VALUE: desc = "invalid value"; break;
    case GL_INVALID_OPERATION: desc = "invalid operation"; break;
    case GL_STACK_OVERFLOW: desc = "stack overflow"; break;
    case GL_STACK_UNDERFLOW: desc = "stack underflow"; break;
    case GL_OUT_OF_MEMORY: desc = "out of memory"; break;
    }
    printf("GL error: %s:%d: %s (%x)\n", file, line, desc, error);
}

VAR(amd_pf_bug, 0, 0, 1);
VAR(mesa_texrectoffset_bug, 0, 0, 1);
VAR(useubo, 1, 0, 0);
VAR(usetexgather, 1, 0, 0);
VAR(usetexcompress, 1, 0, 0);

static bool checkseries(const char *s, const char *name, int low, int high)
{
    if(name) s = strstr(s, name);
    if(!s) return false;
    while(*s && !isdigit(*s)) ++s;
    if(!*s) return false;
    int n = 0;
    while(isdigit(*s)) n = n*10 + (*s++ - '0');
    return n >= low && n <= high;
}

VAR(dbgexts, 0, 0, 1);

hashset<const char *> glexts;

void parseglexts()
{
    if(glversion >= 300)
    {
        GLint numexts = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numexts);
        loopi(numexts)
        {
            const char *ext = (const char *)glGetStringi_(GL_EXTENSIONS, i);
            glexts.add(newstring(ext));
        }
    }
    else
    {
        const char *exts = (const char *)glGetString(GL_EXTENSIONS);
        for(;;)
        {
            while(*exts == ' ') exts++;
            if(!*exts) break;
            const char *ext = exts;
            while(*exts && *exts != ' ') exts++;
            if(exts > ext) glexts.add(newstring(ext, size_t(exts-ext)));
        }
    }
}

bool hasext(const char *ext)
{
    return glexts.access(ext)!=NULL;
}

void gl_checkextensions()
{
    const char *vendor = (const char *)glGetString(GL_VENDOR);
    const char *renderer = (const char *)glGetString(GL_RENDERER);
    const char *version = (const char *)glGetString(GL_VERSION);
    conoutf(CON_INIT, "Renderer: %s (%s)", renderer, vendor);
    conoutf(CON_INIT, "Driver: %s", version);

#ifdef __APPLE__
    // extern int mac_osversion();
    // int osversion = mac_osversion();  /* 0x0A0600 = 10.6, assumed minimum */
#endif

    if(strstr(renderer, "Mesa") || strstr(version, "Mesa"))
    {
        mesa = true;
        if(strstr(renderer, "Intel")) intel = true;
    }
    else if(strstr(vendor, "NVIDIA"))
        nvidia = true;
    else if(strstr(vendor, "ATI") || strstr(vendor, "Advanced Micro Devices"))
        amd = true;
    else if(strstr(vendor, "Intel"))
        intel = true;

    uint glmajorversion, glminorversion;
    if(sscanf(version, " %u.%u", &glmajorversion, &glminorversion) != 2) glversion = 100;
    else glversion = glmajorversion*100 + glminorversion*10;

    if(glversion < 210) fatal("OpenGL 2.1 or greater is required!");

#ifdef WIN32
    glActiveTexture_ =            (PFNGLACTIVETEXTUREPROC)            getprocaddress("glActiveTexture");

    glBlendEquation_ =            (PFNGLBLENDEQUATIONPROC)            getprocaddress("glBlendEquation");
    glBlendColor_ =               (PFNGLBLENDCOLORPROC)               getprocaddress("glBlendColor");

    glTexImage3D_ =               (PFNGLTEXIMAGE3DPROC)               getprocaddress("glTexImage3D");
    glTexSubImage3D_ =            (PFNGLTEXSUBIMAGE3DPROC)            getprocaddress("glTexSubImage3D");
    glCopyTexSubImage3D_ =        (PFNGLCOPYTEXSUBIMAGE3DPROC)        getprocaddress("glCopyTexSubImage3D");

    glCompressedTexImage3D_ =     (PFNGLCOMPRESSEDTEXIMAGE3DPROC)     getprocaddress("glCompressedTexImage3D");
    glCompressedTexImage2D_ =     (PFNGLCOMPRESSEDTEXIMAGE2DPROC)     getprocaddress("glCompressedTexImage2D");
    glCompressedTexImage1D_ =     (PFNGLCOMPRESSEDTEXIMAGE1DPROC)     getprocaddress("glCompressedTexImage1D");
    glCompressedTexSubImage3D_ =  (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)  getprocaddress("glCompressedTexSubImage3D");
    glCompressedTexSubImage2D_ =  (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)  getprocaddress("glCompressedTexSubImage2D");
    glCompressedTexSubImage1D_ =  (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)  getprocaddress("glCompressedTexSubImage1D");
    glGetCompressedTexImage_ =    (PFNGLGETCOMPRESSEDTEXIMAGEPROC)    getprocaddress("glGetCompressedTexImage");

    glDrawRangeElements_ =        (PFNGLDRAWRANGEELEMENTSPROC)        getprocaddress("glDrawRangeElements");
#endif

#ifndef __APPLE__
    glMultiDrawArrays_ =          (PFNGLMULTIDRAWARRAYSPROC)          getprocaddress("glMultiDrawArrays");
    glMultiDrawElements_ =        (PFNGLMULTIDRAWELEMENTSPROC)        getprocaddress("glMultiDrawElements");

    glBlendFuncSeparate_ =        (PFNGLBLENDFUNCSEPARATEPROC)        getprocaddress("glBlendFuncSeparate");
    glBlendEquationSeparate_ =    (PFNGLBLENDEQUATIONSEPARATEPROC)    getprocaddress("glBlendEquationSeparate");
    glStencilOpSeparate_ =        (PFNGLSTENCILOPSEPARATEPROC)        getprocaddress("glStencilOpSeparate");
    glStencilFuncSeparate_ =      (PFNGLSTENCILFUNCSEPARATEPROC)      getprocaddress("glStencilFuncSeparate");
    glStencilMaskSeparate_ =      (PFNGLSTENCILMASKSEPARATEPROC)      getprocaddress("glStencilMaskSeparate");

    glGenBuffers_ =               (PFNGLGENBUFFERSPROC)               getprocaddress("glGenBuffers");
    glBindBuffer_ =               (PFNGLBINDBUFFERPROC)               getprocaddress("glBindBuffer");
    glMapBuffer_ =                (PFNGLMAPBUFFERPROC)                getprocaddress("glMapBuffer");
    glUnmapBuffer_ =              (PFNGLUNMAPBUFFERPROC)              getprocaddress("glUnmapBuffer");
    glBufferData_ =               (PFNGLBUFFERDATAPROC)               getprocaddress("glBufferData");
    glBufferSubData_ =            (PFNGLBUFFERSUBDATAPROC)            getprocaddress("glBufferSubData");
    glDeleteBuffers_ =            (PFNGLDELETEBUFFERSPROC)            getprocaddress("glDeleteBuffers");
    glGetBufferSubData_ =         (PFNGLGETBUFFERSUBDATAPROC)         getprocaddress("glGetBufferSubData");

    glGetQueryiv_ =               (PFNGLGETQUERYIVPROC)               getprocaddress("glGetQueryiv");
    glGenQueries_ =               (PFNGLGENQUERIESPROC)               getprocaddress("glGenQueries");
    glDeleteQueries_ =            (PFNGLDELETEQUERIESPROC)            getprocaddress("glDeleteQueries");
    glBeginQuery_ =               (PFNGLBEGINQUERYPROC)               getprocaddress("glBeginQuery");
    glEndQuery_ =                 (PFNGLENDQUERYPROC)                 getprocaddress("glEndQuery");
    glGetQueryObjectiv_ =         (PFNGLGETQUERYOBJECTIVPROC)         getprocaddress("glGetQueryObjectiv");
    glGetQueryObjectuiv_ =        (PFNGLGETQUERYOBJECTUIVPROC)        getprocaddress("glGetQueryObjectuiv");

    glCreateProgram_ =            (PFNGLCREATEPROGRAMPROC)            getprocaddress("glCreateProgram");
    glDeleteProgram_ =            (PFNGLDELETEPROGRAMPROC)            getprocaddress("glDeleteProgram");
    glUseProgram_ =               (PFNGLUSEPROGRAMPROC)               getprocaddress("glUseProgram");
    glCreateShader_ =             (PFNGLCREATESHADERPROC)             getprocaddress("glCreateShader");
    glDeleteShader_ =             (PFNGLDELETESHADERPROC)             getprocaddress("glDeleteShader");
    glShaderSource_ =             (PFNGLSHADERSOURCEPROC)             getprocaddress("glShaderSource");
    glCompileShader_ =            (PFNGLCOMPILESHADERPROC)            getprocaddress("glCompileShader");
    glGetShaderiv_ =              (PFNGLGETSHADERIVPROC)              getprocaddress("glGetShaderiv");
    glGetProgramiv_ =             (PFNGLGETPROGRAMIVPROC)             getprocaddress("glGetProgramiv");
    glAttachShader_ =             (PFNGLATTACHSHADERPROC)             getprocaddress("glAttachShader");
    glGetProgramInfoLog_ =        (PFNGLGETPROGRAMINFOLOGPROC)        getprocaddress("glGetProgramInfoLog");
    glGetShaderInfoLog_ =         (PFNGLGETSHADERINFOLOGPROC)         getprocaddress("glGetShaderInfoLog");
    glLinkProgram_ =              (PFNGLLINKPROGRAMPROC)              getprocaddress("glLinkProgram");
    glGetUniformLocation_ =       (PFNGLGETUNIFORMLOCATIONPROC)       getprocaddress("glGetUniformLocation");
    glUniform1f_ =                (PFNGLUNIFORM1FPROC)                getprocaddress("glUniform1f");
    glUniform2f_ =                (PFNGLUNIFORM2FPROC)                getprocaddress("glUniform2f");
    glUniform3f_ =                (PFNGLUNIFORM3FPROC)                getprocaddress("glUniform3f");
    glUniform4f_ =                (PFNGLUNIFORM4FPROC)                getprocaddress("glUniform4f");
    glUniform1fv_ =               (PFNGLUNIFORM1FVPROC)               getprocaddress("glUniform1fv");
    glUniform2fv_ =               (PFNGLUNIFORM2FVPROC)               getprocaddress("glUniform2fv");
    glUniform3fv_ =               (PFNGLUNIFORM3FVPROC)               getprocaddress("glUniform3fv");
    glUniform4fv_ =               (PFNGLUNIFORM4FVPROC)               getprocaddress("glUniform4fv");
    glUniform1i_ =                (PFNGLUNIFORM1IPROC)                getprocaddress("glUniform1i");
    glUniform2i_ =                (PFNGLUNIFORM2IPROC)                getprocaddress("glUniform2i");
    glUniform3i_ =                (PFNGLUNIFORM3IPROC)                getprocaddress("glUniform3i");
    glUniform4i_ =                (PFNGLUNIFORM4IPROC)                getprocaddress("glUniform4i");
    glUniform1iv_ =               (PFNGLUNIFORM1IVPROC)               getprocaddress("glUniform1iv");
    glUniform2iv_ =               (PFNGLUNIFORM2IVPROC)               getprocaddress("glUniform2iv");
    glUniform3iv_ =               (PFNGLUNIFORM3IVPROC)               getprocaddress("glUniform3iv");
    glUniform4iv_ =               (PFNGLUNIFORM4IVPROC)               getprocaddress("glUniform4iv");
    glUniformMatrix2fv_ =         (PFNGLUNIFORMMATRIX2FVPROC)         getprocaddress("glUniformMatrix2fv");
    glUniformMatrix3fv_ =         (PFNGLUNIFORMMATRIX3FVPROC)         getprocaddress("glUniformMatrix3fv");
    glUniformMatrix4fv_ =         (PFNGLUNIFORMMATRIX4FVPROC)         getprocaddress("glUniformMatrix4fv");
    glBindAttribLocation_ =       (PFNGLBINDATTRIBLOCATIONPROC)       getprocaddress("glBindAttribLocation");
    glGetActiveUniform_ =         (PFNGLGETACTIVEUNIFORMPROC)         getprocaddress("glGetActiveUniform");
    glEnableVertexAttribArray_ =  (PFNGLENABLEVERTEXATTRIBARRAYPROC)  getprocaddress("glEnableVertexAttribArray");
    glDisableVertexAttribArray_ = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) getprocaddress("glDisableVertexAttribArray");

    glVertexAttrib1f_ =           (PFNGLVERTEXATTRIB1FPROC)           getprocaddress("glVertexAttrib1f");
    glVertexAttrib1fv_ =          (PFNGLVERTEXATTRIB1FVPROC)          getprocaddress("glVertexAttrib1fv");
    glVertexAttrib1s_ =           (PFNGLVERTEXATTRIB1SPROC)           getprocaddress("glVertexAttrib1s");
    glVertexAttrib1sv_ =          (PFNGLVERTEXATTRIB1SVPROC)          getprocaddress("glVertexAttrib1sv");
    glVertexAttrib2f_ =           (PFNGLVERTEXATTRIB2FPROC)           getprocaddress("glVertexAttrib2f");
    glVertexAttrib2fv_ =          (PFNGLVERTEXATTRIB2FVPROC)          getprocaddress("glVertexAttrib2fv");
    glVertexAttrib2s_ =           (PFNGLVERTEXATTRIB2SPROC)           getprocaddress("glVertexAttrib2s");
    glVertexAttrib2sv_ =          (PFNGLVERTEXATTRIB2SVPROC)          getprocaddress("glVertexAttrib2sv");
    glVertexAttrib3f_ =           (PFNGLVERTEXATTRIB3FPROC)           getprocaddress("glVertexAttrib3f");
    glVertexAttrib3fv_ =          (PFNGLVERTEXATTRIB3FVPROC)          getprocaddress("glVertexAttrib3fv");
    glVertexAttrib3s_ =           (PFNGLVERTEXATTRIB3SPROC)           getprocaddress("glVertexAttrib3s");
    glVertexAttrib3sv_ =          (PFNGLVERTEXATTRIB3SVPROC)          getprocaddress("glVertexAttrib3sv");
    glVertexAttrib4f_ =           (PFNGLVERTEXATTRIB4FPROC)           getprocaddress("glVertexAttrib4f");
    glVertexAttrib4fv_ =          (PFNGLVERTEXATTRIB4FVPROC)          getprocaddress("glVertexAttrib4fv");
    glVertexAttrib4s_ =           (PFNGLVERTEXATTRIB4SPROC)           getprocaddress("glVertexAttrib4s");
    glVertexAttrib4sv_ =          (PFNGLVERTEXATTRIB4SVPROC)          getprocaddress("glVertexAttrib4sv");
    glVertexAttrib4bv_ =          (PFNGLVERTEXATTRIB4BVPROC)          getprocaddress("glVertexAttrib4bv");
    glVertexAttrib4iv_ =          (PFNGLVERTEXATTRIB4IVPROC)          getprocaddress("glVertexAttrib4iv");
    glVertexAttrib4ubv_ =         (PFNGLVERTEXATTRIB4UBVPROC)         getprocaddress("glVertexAttrib4ubv");
    glVertexAttrib4uiv_ =         (PFNGLVERTEXATTRIB4UIVPROC)         getprocaddress("glVertexAttrib4uiv");
    glVertexAttrib4usv_ =         (PFNGLVERTEXATTRIB4USVPROC)         getprocaddress("glVertexAttrib4usv");
    glVertexAttrib4Nbv_ =         (PFNGLVERTEXATTRIB4NBVPROC)         getprocaddress("glVertexAttrib4Nbv");
    glVertexAttrib4Niv_ =         (PFNGLVERTEXATTRIB4NIVPROC)         getprocaddress("glVertexAttrib4Niv");
    glVertexAttrib4Nub_ =         (PFNGLVERTEXATTRIB4NUBPROC)         getprocaddress("glVertexAttrib4Nub");
    glVertexAttrib4Nubv_ =        (PFNGLVERTEXATTRIB4NUBVPROC)        getprocaddress("glVertexAttrib4Nubv");
    glVertexAttrib4Nuiv_ =        (PFNGLVERTEXATTRIB4NUIVPROC)        getprocaddress("glVertexAttrib4Nuiv");
    glVertexAttrib4Nusv_ =        (PFNGLVERTEXATTRIB4NUSVPROC)        getprocaddress("glVertexAttrib4Nusv");
    glVertexAttribPointer_ =      (PFNGLVERTEXATTRIBPOINTERPROC)      getprocaddress("glVertexAttribPointer");

    glUniformMatrix2x3fv_ =       (PFNGLUNIFORMMATRIX2X3FVPROC)       getprocaddress("glUniformMatrix2x3fv");
    glUniformMatrix3x2fv_ =       (PFNGLUNIFORMMATRIX3X2FVPROC)       getprocaddress("glUniformMatrix3x2fv");
    glUniformMatrix2x4fv_ =       (PFNGLUNIFORMMATRIX2X4FVPROC)       getprocaddress("glUniformMatrix2x4fv");
    glUniformMatrix4x2fv_ =       (PFNGLUNIFORMMATRIX4X2FVPROC)       getprocaddress("glUniformMatrix4x2fv");
    glUniformMatrix3x4fv_ =       (PFNGLUNIFORMMATRIX3X4FVPROC)       getprocaddress("glUniformMatrix3x4fv");
    glUniformMatrix4x3fv_ =       (PFNGLUNIFORMMATRIX4X3FVPROC)       getprocaddress("glUniformMatrix4x3fv");

    glDrawBuffers_ =              (PFNGLDRAWBUFFERSPROC)              getprocaddress("glDrawBuffers");
#endif

    if(glversion >= 300)
    {
        glGetStringi_ =            (PFNGLGETSTRINGIPROC)          getprocaddress("glGetStringi");
    }

    const char *glslstr = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    conoutf(CON_INIT, "GLSL: %s", glslstr ? glslstr : "unknown");

    uint glslmajorversion, glslminorversion;
    if(glslstr && sscanf(glslstr, " %u.%u", &glslmajorversion, &glslminorversion) == 2) glslversion = glslmajorversion*100 + glslminorversion;

    if(glslversion < 120) fatal("GLSL 1.20 or greater is required!");

    parseglexts();

    GLint texsize = 0, texunits = 0, vtexunits = 0, cubetexsize = 0, oqbits = 0, drawbufs = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texsize);
    hwtexsize = texsize;
    if(hwtexsize < 4096)
        fatal("Large texture support is required!");
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texunits);
    hwtexunits = texunits;
    if(hwtexunits < 16)
        fatal("Hardware does not support at least 16 texture units.");
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &vtexunits);
    hwvtexunits = vtexunits;
    if(hwvtexunits < 4)
        fatal("Hardware does not support at least 4 vertex texture units.");
    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &cubetexsize);
    hwcubetexsize = cubetexsize;
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &drawbufs);
    if(drawbufs < 4) fatal("Hardware does not support at least 4 draw buffers.");
    glGetQueryiv_(GL_SAMPLES_PASSED, GL_QUERY_COUNTER_BITS, &oqbits);
    if(!oqbits)
    {
        conoutf(CON_WARN, "WARNING: No occlusion query support!");
        extern int vacubesize, oqfrags;
        vacubesize = 64;
        oqfrags = 0;
    }

    if(glversion >= 300 || hasext("GL_ARB_vertex_array_object"))
    {
        glBindVertexArray_ =    (PFNGLBINDVERTEXARRAYPROC)   getprocaddress("glBindVertexArray");
        glDeleteVertexArrays_ = (PFNGLDELETEVERTEXARRAYSPROC)getprocaddress("glDeleteVertexArrays");
        glGenVertexArrays_ =    (PFNGLGENVERTEXARRAYSPROC)   getprocaddress("glGenVertexArrays");
        glIsVertexArray_ =      (PFNGLISVERTEXARRAYPROC)     getprocaddress("glIsVertexArray");
        hasVAO = true;
        if(glversion < 300 && dbgexts) conoutf(CON_INIT, "Using GL_ARB_vertex_array_object extension.");
    }
    else if(hasext("GL_APPLE_vertex_array_object"))
    {
        glBindVertexArray_ =    (PFNGLBINDVERTEXARRAYPROC)   getprocaddress("glBindVertexArrayAPPLE");
        glDeleteVertexArrays_ = (PFNGLDELETEVERTEXARRAYSPROC)getprocaddress("glDeleteVertexArraysAPPLE");
        glGenVertexArrays_ =    (PFNGLGENVERTEXARRAYSPROC)   getprocaddress("glGenVertexArraysAPPLE");
        glIsVertexArray_ =      (PFNGLISVERTEXARRAYPROC)     getprocaddress("glIsVertexArrayAPPLE");
        hasVAO = true;
        if(dbgexts) conoutf(CON_INIT, "Using GL_APPLE_vertex_array_object extension.");
    }

    if(glversion >= 300)
    {
        hasTF = hasTRG = hasRGTC = hasPF = hasHFV = hasHFP = true;

        glBindFragDataLocation_ = (PFNGLBINDFRAGDATALOCATIONPROC)getprocaddress("glBindFragDataLocation");
        glUniform1ui_ =           (PFNGLUNIFORM1UIPROC)          getprocaddress("glUniform1ui");
        glUniform2ui_ =           (PFNGLUNIFORM2UIPROC)          getprocaddress("glUniform2ui");
        glUniform3ui_ =           (PFNGLUNIFORM3UIPROC)          getprocaddress("glUniform3ui");
        glUniform4ui_ =           (PFNGLUNIFORM4UIPROC)          getprocaddress("glUniform4ui");
        glUniform1uiv_ =          (PFNGLUNIFORM1UIVPROC)         getprocaddress("glUniform1uiv");
        glUniform2uiv_ =          (PFNGLUNIFORM2UIVPROC)         getprocaddress("glUniform2uiv");
        glUniform3uiv_ =          (PFNGLUNIFORM3UIVPROC)         getprocaddress("glUniform3uiv");
        glUniform4uiv_ =          (PFNGLUNIFORM4UIVPROC)         getprocaddress("glUniform4uiv");
        glClearBufferiv_ =        (PFNGLCLEARBUFFERIVPROC)       getprocaddress("glClearBufferiv");
        glClearBufferuiv_ =       (PFNGLCLEARBUFFERUIVPROC)      getprocaddress("glClearBufferuiv");
        glClearBufferfv_ =        (PFNGLCLEARBUFFERFVPROC)       getprocaddress("glClearBufferfv");
        glClearBufferfi_ =        (PFNGLCLEARBUFFERFIPROC)       getprocaddress("glClearBufferfi");
        hasGPU4 = true;

        if(hasext("GL_EXT_gpu_shader4"))
        {
            hasEGPU4 = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_gpu_shader4 extension.");
        }

        glClampColor_ = (PFNGLCLAMPCOLORPROC)getprocaddress("glClampColor");
        hasCBF = true;

        glColorMaski_ = (PFNGLCOLORMASKIPROC)getprocaddress("glColorMaski");
        glEnablei_ =    (PFNGLENABLEIPROC)   getprocaddress("glEnablei");
        glDisablei_ =   (PFNGLENABLEIPROC)   getprocaddress("glDisablei");
        hasDB2 = true;

        glBeginConditionalRender_ = (PFNGLBEGINCONDITIONALRENDERPROC)getprocaddress("glBeginConditionalRender");
        glEndConditionalRender_ =   (PFNGLENDCONDITIONALRENDERPROC)  getprocaddress("glEndConditionalRender");
        hasCR = true;

        glTexParameterIiv_ =     (PFNGLTEXPARAMETERIIVPROC)    getprocaddress("glTexParameterIiv");
        glTexParameterIuiv_ =    (PFNGLTEXPARAMETERIUIVPROC)   getprocaddress("glTexParameterIuiv");
        glGetTexParameterIiv_ =  (PFNGLGETTEXPARAMETERIIVPROC) getprocaddress("glGetTexParameterIiv");
        glGetTexParameterIuiv_ = (PFNGLGETTEXPARAMETERIUIVPROC)getprocaddress("glGetTexParameterIuiv");
        hasTI = true;
    }
    else
    {
        if(hasext("GL_ARB_texture_float"))
        {
            hasTF = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_texture_float extension.");
        }
        if(hasext("GL_ARB_texture_rg"))
        {
            hasTRG = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_texture_rg extension.");
        }
        if(hasext("GL_ARB_texture_compression_rgtc") || hasext("GL_EXT_texture_compression_rgtc"))
        {
            hasRGTC = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_texture_compression_rgtc extension.");
        }
        if(hasext("GL_EXT_packed_float"))
        {
            hasPF = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_packed_float extension.");
        }
        if(hasext("GL_EXT_gpu_shader4"))
        {
            glBindFragDataLocation_ = (PFNGLBINDFRAGDATALOCATIONPROC)getprocaddress("glBindFragDataLocationEXT");
            glUniform1ui_ =           (PFNGLUNIFORM1UIPROC)          getprocaddress("glUniform1uiEXT");
            glUniform2ui_ =           (PFNGLUNIFORM2UIPROC)          getprocaddress("glUniform2uiEXT");
            glUniform3ui_ =           (PFNGLUNIFORM3UIPROC)          getprocaddress("glUniform3uiEXT");
            glUniform4ui_ =           (PFNGLUNIFORM4UIPROC)          getprocaddress("glUniform4uiEXT");
            glUniform1uiv_ =          (PFNGLUNIFORM1UIVPROC)         getprocaddress("glUniform1uivEXT");
            glUniform2uiv_ =          (PFNGLUNIFORM2UIVPROC)         getprocaddress("glUniform2uivEXT");
            glUniform3uiv_ =          (PFNGLUNIFORM3UIVPROC)         getprocaddress("glUniform3uivEXT");
            glUniform4uiv_ =          (PFNGLUNIFORM4UIVPROC)         getprocaddress("glUniform4uivEXT");
            hasEGPU4 = hasGPU4 = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_gpu_shader4 extension.");
        }
        if(hasext("GL_ARB_color_buffer_float"))
        {
            glClampColor_ = (PFNGLCLAMPCOLORPROC)getprocaddress("glClampColorARB");
            hasCBF = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_color_buffer_float extension.");
        }
        if(hasext("GL_EXT_draw_buffers2"))
        {
            glColorMaski_ = (PFNGLCOLORMASKIPROC)getprocaddress("glColorMaskIndexedEXT");
            glEnablei_ =    (PFNGLENABLEIPROC)   getprocaddress("glEnableIndexedEXT");
            glDisablei_ =   (PFNGLENABLEIPROC)   getprocaddress("glDisableIndexedEXT");
            hasDB2 = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_draw_buffers2 extension.");
        }
        if(hasext("GL_NV_conditional_render"))
        {
            glBeginConditionalRender_ = (PFNGLBEGINCONDITIONALRENDERPROC)getprocaddress("glBeginConditionalRenderNV");
            glEndConditionalRender_ =   (PFNGLENDCONDITIONALRENDERPROC)  getprocaddress("glEndConditionalRenderNV");
            hasCR = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_NV_conditional_render extension.");
        }
        if(hasext("GL_EXT_texture_integer"))
        {
            glTexParameterIiv_ =     (PFNGLTEXPARAMETERIIVPROC)    getprocaddress("glTexParameterIivEXT");
            glTexParameterIuiv_ =    (PFNGLTEXPARAMETERIUIVPROC)   getprocaddress("glTexParameterIuivEXT");
            glGetTexParameterIiv_ =  (PFNGLGETTEXPARAMETERIIVPROC) getprocaddress("glGetTexParameterIivEXT");
            glGetTexParameterIuiv_ = (PFNGLGETTEXPARAMETERIUIVPROC)getprocaddress("glGetTexParameterIuivEXT");
            glClearColorIi_ =        (PFNGLCLEARCOLORIIEXTPROC)    getprocaddress("glClearColorIiEXT");
            glClearColorIui_ =       (PFNGLCLEARCOLORIUIEXTPROC)   getprocaddress("glClearColorIuiEXT");
            hasTI = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_texture_integer extension.");
        }
        if(hasext("GL_NV_half_float"))
        {
            hasHFV = hasHFP = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_NV_half_float");
        }
        else
        {
            if(hasext("GL_ARB_half_float_vertex"))
            {
                hasHFV = true;
                if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_half_float_vertex extension.");
            }
            if(hasext("GL_ARB_half_float_pixel"))
            {
                hasHFP = true;
                if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_half_float_pixel extension.");
            }
        }
    }

    if(!hasHFV) fatal("Half-precision floating-point support is required!");

    if(glversion >= 300 || hasext("GL_ARB_framebuffer_object"))
    {
        glBindRenderbuffer_        = (PFNGLBINDRENDERBUFFERPROC)       getprocaddress("glBindRenderbuffer");
        glDeleteRenderbuffers_     = (PFNGLDELETERENDERBUFFERSPROC)    getprocaddress("glDeleteRenderbuffers");
        glGenRenderbuffers_        = (PFNGLGENFRAMEBUFFERSPROC)        getprocaddress("glGenRenderbuffers");
        glRenderbufferStorage_     = (PFNGLRENDERBUFFERSTORAGEPROC)    getprocaddress("glRenderbufferStorage");
        glCheckFramebufferStatus_  = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) getprocaddress("glCheckFramebufferStatus");
        glBindFramebuffer_         = (PFNGLBINDFRAMEBUFFERPROC)        getprocaddress("glBindFramebuffer");
        glDeleteFramebuffers_      = (PFNGLDELETEFRAMEBUFFERSPROC)     getprocaddress("glDeleteFramebuffers");
        glGenFramebuffers_         = (PFNGLGENFRAMEBUFFERSPROC)        getprocaddress("glGenFramebuffers");
        glFramebufferTexture1D_    = (PFNGLFRAMEBUFFERTEXTURE1DPROC)   getprocaddress("glFramebufferTexture1D");
        glFramebufferTexture2D_    = (PFNGLFRAMEBUFFERTEXTURE2DPROC)   getprocaddress("glFramebufferTexture2D");
        glFramebufferTexture3D_    = (PFNGLFRAMEBUFFERTEXTURE3DPROC)   getprocaddress("glFramebufferTexture3D");
        glFramebufferRenderbuffer_ = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)getprocaddress("glFramebufferRenderbuffer");
        glGenerateMipmap_          = (PFNGLGENERATEMIPMAPPROC)         getprocaddress("glGenerateMipmap");
        glBlitFramebuffer_         = (PFNGLBLITFRAMEBUFFERPROC)        getprocaddress("glBlitFramebuffer");
        glRenderbufferStorageMultisample_ = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)getprocaddress("glRenderbufferStorageMultisample");

        hasAFBO = hasFBO = hasFBB = hasFBMS = hasDS = true;
        if(glversion < 300 && dbgexts) conoutf(CON_INIT, "Using GL_ARB_framebuffer_object extension.");
    }
    else if(hasext("GL_EXT_framebuffer_object"))
    {
        glBindRenderbuffer_        = (PFNGLBINDRENDERBUFFERPROC)       getprocaddress("glBindRenderbufferEXT");
        glDeleteRenderbuffers_     = (PFNGLDELETERENDERBUFFERSPROC)    getprocaddress("glDeleteRenderbuffersEXT");
        glGenRenderbuffers_        = (PFNGLGENFRAMEBUFFERSPROC)        getprocaddress("glGenRenderbuffersEXT");
        glRenderbufferStorage_     = (PFNGLRENDERBUFFERSTORAGEPROC)    getprocaddress("glRenderbufferStorageEXT");
        glCheckFramebufferStatus_  = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) getprocaddress("glCheckFramebufferStatusEXT");
        glBindFramebuffer_         = (PFNGLBINDFRAMEBUFFERPROC)        getprocaddress("glBindFramebufferEXT");
        glDeleteFramebuffers_      = (PFNGLDELETEFRAMEBUFFERSPROC)     getprocaddress("glDeleteFramebuffersEXT");
        glGenFramebuffers_         = (PFNGLGENFRAMEBUFFERSPROC)        getprocaddress("glGenFramebuffersEXT");
        glFramebufferTexture1D_    = (PFNGLFRAMEBUFFERTEXTURE1DPROC)   getprocaddress("glFramebufferTexture1DEXT");
        glFramebufferTexture2D_    = (PFNGLFRAMEBUFFERTEXTURE2DPROC)   getprocaddress("glFramebufferTexture2DEXT");
        glFramebufferTexture3D_    = (PFNGLFRAMEBUFFERTEXTURE3DPROC)   getprocaddress("glFramebufferTexture3DEXT");
        glFramebufferRenderbuffer_ = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)getprocaddress("glFramebufferRenderbufferEXT");
        glGenerateMipmap_          = (PFNGLGENERATEMIPMAPPROC)         getprocaddress("glGenerateMipmapEXT");
        hasFBO = true;
        if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_framebuffer_object extension.");

        if(hasext("GL_EXT_framebuffer_blit"))
        {
            glBlitFramebuffer_     = (PFNGLBLITFRAMEBUFFERPROC)        getprocaddress("glBlitFramebufferEXT");
            hasFBB = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_framebuffer_blit extension.");
        }
        if(hasext("GL_EXT_framebuffer_multisample"))
        {
            glRenderbufferStorageMultisample_ = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)getprocaddress("glRenderbufferStorageMultisampleEXT");
            hasFBMS = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_framebuffer_multisample extension.");
        }
        if(hasext("GL_EXT_packed_depth_stencil") || hasext("GL_NV_packed_depth_stencil"))
        {
            hasDS = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_packed_depth_stencil extension.");
        }
    }
    else fatal("Framebuffer object support is required!");

    if(glversion >= 300 || hasext("GL_ARB_map_buffer_range"))
    {
        glMapBufferRange_         = (PFNGLMAPBUFFERRANGEPROC)        getprocaddress("glMapBufferRange");
        glFlushMappedBufferRange_ = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)getprocaddress("glFlushMappedBufferRange");
        hasMBR = true;
        if(glversion < 300 && dbgexts) conoutf(CON_INIT, "Using GL_ARB_map_buffer_range.");
    }

    if(glversion >= 310 || hasext("GL_ARB_uniform_buffer_object"))
    {
        glGetUniformIndices_       = (PFNGLGETUNIFORMINDICESPROC)      getprocaddress("glGetUniformIndices");
        glGetActiveUniformsiv_     = (PFNGLGETACTIVEUNIFORMSIVPROC)    getprocaddress("glGetActiveUniformsiv");
        glGetUniformBlockIndex_    = (PFNGLGETUNIFORMBLOCKINDEXPROC)   getprocaddress("glGetUniformBlockIndex");
        glGetActiveUniformBlockiv_ = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)getprocaddress("glGetActiveUniformBlockiv");
        glUniformBlockBinding_     = (PFNGLUNIFORMBLOCKBINDINGPROC)    getprocaddress("glUniformBlockBinding");
        glBindBufferBase_          = (PFNGLBINDBUFFERBASEPROC)         getprocaddress("glBindBufferBase");
        glBindBufferRange_         = (PFNGLBINDBUFFERRANGEPROC)        getprocaddress("glBindBufferRange");

        useubo = 1;
        hasUBO = true;
        if(glversion < 310 && dbgexts) conoutf(CON_INIT, "Using GL_ARB_uniform_buffer_object extension.");
    }

    if(glversion >= 310 || hasext("GL_ARB_texture_rectangle"))
    {
        hasTR = true;
        if(glversion < 310 && dbgexts) conoutf(CON_INIT, "Using GL_ARB_texture_rectangle extension.");
    }
    else fatal("Texture rectangle support is required!");

    if(glversion >= 310 || hasext("GL_ARB_copy_buffer"))
    {
        glCopyBufferSubData_ = (PFNGLCOPYBUFFERSUBDATAPROC)getprocaddress("glCopyBufferSubData");
        hasCB = true;
        if(glversion < 310 && dbgexts) conoutf(CON_INIT, "Using GL_ARB_copy_buffer extension.");
    }

    if(glversion >= 320 || hasext("GL_ARB_texture_multisample"))
    {
        glTexImage2DMultisample_ = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)getprocaddress("glTexImage2DMultisample");
        glTexImage3DMultisample_ = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)getprocaddress("glTexImage3DMultisample");
        glGetMultisamplefv_      = (PFNGLGETMULTISAMPLEFVPROC)     getprocaddress("glGetMultisamplefv");
        glSampleMaski_           = (PFNGLSAMPLEMASKIPROC)          getprocaddress("glSampleMaski");
        hasTMS = true;
        if(glversion < 320 && dbgexts) conoutf(CON_INIT, "Using GL_ARB_texture_multisample extension.");
    }
    if(hasext("GL_EXT_framebuffer_multisample_blit_scaled"))
    {
        hasFBMSBS = true;
        if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_framebuffer_multisample_blit_scaled extension.");
    }
    if(hasext("GL_NV_framebuffer_multisample_coverage"))
    {
        glRenderbufferStorageMultisampleCoverageNV_ = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC)getprocaddress("glRenderbufferStorageMultisampleCoverageNV");
        hasNVFBMSC = true;
    }
    if(hasext("GL_NV_texture_multisample"))
    {
        glTexImage2DMultisampleCoverageNV_     = (PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC)    getprocaddress("glTexImage2DMultisampleCoverageNV");
        glTexImage3DMultisampleCoverageNV_     = (PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC)    getprocaddress("glTexImage3DMultisampleCoverageNV");
        glTextureImage2DMultisampleNV_         = (PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC)        getprocaddress("glTextureImage2DMultisampleNV");
        glTextureImage3DMultisampleNV_         = (PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC)        getprocaddress("glTextureImage3DMultisampleNV");
        glTextureImage2DMultisampleCoverageNV_ = (PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC)getprocaddress("glTextureImage2DMultisampleCoverageNV");
        glTextureImage3DMultisampleCoverageNV_ = (PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC)getprocaddress("glTextureImage3DMultisampleCoverageNV");
        hasNVTMS = true;
    }

    if(hasext("GL_EXT_timer_query") || hasext("GL_ARB_timer_query"))
    {
        if(hasext("GL_EXT_timer_query"))
        {
            glGetQueryObjecti64v_ =  (PFNGLGETQUERYOBJECTI64VEXTPROC)  getprocaddress("glGetQueryObjecti64vEXT");
            glGetQueryObjectui64v_ = (PFNGLGETQUERYOBJECTUI64VEXTPROC) getprocaddress("glGetQueryObjectui64vEXT");
            if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_timer_query extension.");
        }
        else
        {
            glGetQueryObjecti64v_ =  (PFNGLGETQUERYOBJECTI64VEXTPROC)  getprocaddress("glGetQueryObjecti64v");
            glGetQueryObjectui64v_ = (PFNGLGETQUERYOBJECTUI64VEXTPROC) getprocaddress("glGetQueryObjectui64v");
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_timer_query extension.");
        }
        hasTQ = true;
    }

    if(hasext("GL_EXT_texture_compression_s3tc"))
    {
        hasS3TC = true;
#ifdef __APPLE__
        usetexcompress = 1;
#else
        if(!mesa) usetexcompress = 2;
#endif
        if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_texture_compression_s3tc extension.");
    }
    else if(hasext("GL_EXT_texture_compression_dxt1") && hasext("GL_ANGLE_texture_compression_dxt3") && hasext("GL_ANGLE_texture_compression_dxt5"))
    {
        hasS3TC = true;
        if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_texture_compression_dxt1 extension.");
    }
    if(hasext("GL_3DFX_texture_compression_FXT1"))
    {
        hasFXT1 = true;
        if(mesa) usetexcompress = max(usetexcompress, 1);
        if(dbgexts) conoutf(CON_INIT, "Using GL_3DFX_texture_compression_FXT1.");
    }
    if(hasext("GL_EXT_texture_compression_latc"))
    {
        hasLATC = true;
        if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_texture_compression_latc extension.");
    }

    if(hasext("GL_EXT_texture_filter_anisotropic"))
    {
       GLint val = 0;
       glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &val);
       hwmaxaniso = val;
       hasAF = true;
       if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_texture_filter_anisotropic extension.");
    }

    if(hasext("GL_EXT_depth_bounds_test"))
    {
        glDepthBounds_ = (PFNGLDEPTHBOUNDSEXTPROC) getprocaddress("glDepthBoundsEXT");
        hasDBT = true;
        if(dbgexts) conoutf(CON_INIT, "Using GL_EXT_depth_bounds_test extension.");
    }

    if(glversion >= 320 || hasext("GL_ARB_depth_clamp"))
    {
        hasDC = true;
        if(glversion < 320 && dbgexts) conoutf(CON_INIT, "Using GL_ARB_depth_clamp extension.");
    }
    else if(hasext("GL_NV_depth_clamp"))
    {
        hasDC = true;
        if(dbgexts) conoutf(CON_INIT, "Using GL_NV_depth_clamp extension.");
    }

    if(glversion >= 330)
    {
        hasTSW = hasEAL = hasOQ2 = true;
    }
    else
    {
        if(hasext("GL_ARB_texture_swizzle") || hasext("GL_EXT_texture_swizzle"))
        {
            hasTSW = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_texture_swizzle extension.");
        }
        if(hasext("GL_ARB_explicit_attrib_location"))
        {
            hasEAL = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_explicit_attrib_location extension.");
        }
        if(hasext("GL_ARB_occlusion_query2"))
        {
            hasOQ2 = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_occlusion_query2 extension.");
        }
    }

    if(glversion >= 400)
    {
        hasTG = hasGPU5 = true;

        glMinSampleShading_ = (PFNGLMINSAMPLESHADINGPROC)getprocaddress("glMinSampleShading");
        hasMSS = true;

        glBlendEquationi_ =         (PFNGLBLENDEQUATIONIPROC)        getprocaddress("glBlendEquationi");
        glBlendEquationSeparatei_ = (PFNGLBLENDEQUATIONSEPARATEIPROC)getprocaddress("glBlendEquationSeparatei");
        glBlendFunci_ =             (PFNGLBLENDFUNCIPROC)            getprocaddress("glBlendFunci");
        glBlendFuncSeparatei_ =     (PFNGLBLENDFUNCSEPARATEIPROC)    getprocaddress("glBlendFuncSeparatei");
        hasDBB = true;
    }
    else
    {
        if(hasext("GL_ARB_texture_gather"))
        {
            hasTG = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_texture_gather extension.");
        }
        if(hasext("GL_ARB_gpu_shader5"))
        {
            hasGPU5 = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_gpu_shader5 extension.");
        }
        if(hasext("GL_ARB_sample_shading"))
        {
            glMinSampleShading_ = (PFNGLMINSAMPLESHADINGPROC)getprocaddress("glMinSampleShadingARB");
            hasMSS = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_sample_shading extension.");
        }
        if(hasext("GL_ARB_draw_buffers_blend"))
        {
            glBlendEquationi_ =         (PFNGLBLENDEQUATIONIPROC)        getprocaddress("glBlendEquationiARB");
            glBlendEquationSeparatei_ = (PFNGLBLENDEQUATIONSEPARATEIPROC)getprocaddress("glBlendEquationSeparateiARB");
            glBlendFunci_ =             (PFNGLBLENDFUNCIPROC)            getprocaddress("glBlendFunciARB");
            glBlendFuncSeparatei_ =     (PFNGLBLENDFUNCSEPARATEIPROC)    getprocaddress("glBlendFuncSeparateiARB");
            hasDBB = true;
            if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_draw_buffers_blend extension.");
        }
    }
    if(hasTG) usetexgather = hasGPU5 && !intel && !nvidia ? 2 : 1;

    if(hasext("GL_ARB_debug_output"))
    {
        glDebugMessageControl_ =  (PFNGLDEBUGMESSAGECONTROLARBPROC) getprocaddress("glDebugMessageControlARB");
        glDebugMessageInsert_ =   (PFNGLDEBUGMESSAGEINSERTARBPROC)  getprocaddress("glDebugMessageInsertARB");
        glDebugMessageCallback_ = (PFNGLDEBUGMESSAGECALLBACKARBPROC)getprocaddress("glDebugMessageCallbackARB");
        glGetDebugMessageLog_ =   (PFNGLGETDEBUGMESSAGELOGARBPROC)  getprocaddress("glGetDebugMessageLogARB");

        hasDBGO = true;
        if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_debug_output extension.");
    }

    if(hasext("GL_ARB_copy_image"))
    {
        glCopyImageSubData_ = (PFNGLCOPYIMAGESUBDATAPROC)getprocaddress("glCopyImageSubData");

        hasCI = true;
        if(dbgexts) conoutf(CON_INIT, "Using GL_ARB_copy_image extension.");
    }
    else if(hasext("GL_NV_copy_image"))
    {
        glCopyImageSubData_ = (PFNGLCOPYIMAGESUBDATAPROC)getprocaddress("glCopyImageSubDataNV");

        hasCI = true;
        if(dbgexts) conoutf(CON_INIT, "Using GL_NV_copy_image extension.");
    }

    extern int msaadepthstencil, gdepthstencil, glineardepth, msaalineardepth, batchsunlight, smgather, rhrect;
    if(amd)
    {
        msaalineardepth = glineardepth = 1; // reading back from depth-stencil still buggy on newer cards, and requires stencil for MSAA
        msaadepthstencil = gdepthstencil = 1; // some older AMD GPUs do not support reading from depth-stencil textures, so only use depth-stencil renderbuffer for now
        if(checkseries(renderer, "Radeon HD", 4000, 5199)) amd_pf_bug = 1;
        rhrect = 1; // bad cpu stalls on Catalyst 13.x when trying to use 3D textures previously bound to FBOs
    }
    else if(nvidia)
    {
    }
    else if(intel)
    {
        glineardepth = 1; // causes massive slowdown in windows driver (and sometimes in linux driver) if not using linear depth
        if(mesa) batchsunlight = 0; // causes massive slowdown in linux driver
        smgather = 1; // native shadow filter is slow
        if(mesa) mesa_texrectoffset_bug = 1; // mesa i965 driver has buggy textureOffset with texture rectangles
    }
}

ICOMMAND(glext, "s", (char *ext), intret(hasext(ext) ? 1 : 0));

struct timer
{
    enum { MAXQUERY = 4 };

    const char *name;
    bool gpu;
    GLuint query[MAXQUERY];
    int waiting;
    uint starttime;
    float result, print;
};
static vector<timer> timers;
static vector<int> timerorder;
static int timercycle = 0;

extern int usetimers;

timer *findtimer(const char *name, bool gpu)
{
    loopv(timers) if(!strcmp(timers[i].name, name) && timers[i].gpu == gpu)
    {
        timerorder.removeobj(i);
        timerorder.add(i);
        return &timers[i];
    }
    timerorder.add(timers.length());
    timer &t = timers.add();
    t.name = name;
    t.gpu = gpu;
    memset(t.query, 0, sizeof(t.query));
    if(gpu) glGenQueries_(timer::MAXQUERY, t.query);
    t.waiting = 0;
    t.starttime = 0;
    t.result = -1;
    t.print = -1;
    return &t;
}

timer *begintimer(const char *name, bool gpu)
{
    if(!usetimers || viewidx || inbetweenframes || (gpu && (!hasTQ || deferquery))) return NULL;
    timer *t = findtimer(name, gpu);
    if(t->gpu)
    {
        deferquery++;
        glBeginQuery_(GL_TIME_ELAPSED_EXT, t->query[timercycle]);
        t->waiting |= 1<<timercycle;
    }
    else t->starttime = getclockmillis();
    return t;
}

void endtimer(timer *t)
{
    if(!t) return;
    if(t->gpu)
    {
        glEndQuery_(GL_TIME_ELAPSED_EXT);
        deferquery--;
    }
    else t->result = max(float(getclockmillis() - t->starttime), 0.0f);
}

void synctimers()
{
    timercycle = (timercycle + 1) % timer::MAXQUERY;

    loopv(timers)
    {
        timer &t = timers[i];
        if(t.waiting&(1<<timercycle))
        {
            GLint available = 0;
            while(!available)
                glGetQueryObjectiv_(t.query[timercycle], GL_QUERY_RESULT_AVAILABLE, &available);
            GLuint64EXT result = 0;
            glGetQueryObjectui64v_(t.query[timercycle], GL_QUERY_RESULT, &result);
            t.result = max(float(result) * 1e-6f, 0.0f);
            t.waiting &= ~(1<<timercycle);
        }
        else t.result = -1;
    }
}

void cleanuptimers()
{
    loopv(timers)
    {
        timer &t = timers[i];
        if(t.gpu) glDeleteQueries_(timer::MAXQUERY, t.query);
    }
    timers.shrink(0);
    timerorder.shrink(0);
}

VARFN(timer, usetimers, 0, 0, 1, cleanuptimers());
VAR(frametimer, 0, 0, 1);
int framemillis = 0; // frame time (ie does not take into account the swap)

void printtimers(int conw, int conh)
{
    if(!frametimer && !usetimers) return;

    static int lastprint = 0;
    int offset = 0;
    if(frametimer)
    {
        static int printmillis = 0;
        if(totalmillis - lastprint >= 200) printmillis = framemillis;
        draw_textf("frame time %i ms", conw-20*FONTH, conh-FONTH*3/2-offset*9*FONTH/8, printmillis);
        offset++;
    }
    if(usetimers) loopv(timerorder)
    {
        timer &t = timers[timerorder[i]];
        if(t.print < 0 ? t.result >= 0 : totalmillis - lastprint >= 200) t.print = t.result;
        if(t.print < 0 || (t.gpu && !(t.waiting&(1<<timercycle)))) continue;
        draw_textf("%s%s %5.2f ms", conw-20*FONTH, conh-FONTH*3/2-offset*9*FONTH/8, t.name, t.gpu ? "" : " (cpu)", t.print);
        offset++;
    }
    if(totalmillis - lastprint >= 200) lastprint = totalmillis;
}

void gl_resize()
{
    gl_setupframe();
    glViewport(0, 0, hudw, hudh);
}

void gl_init()
{
    GLERROR;

    glClearColor(0, 0, 0, 0);
    glClearDepth(1);
    glClearStencil(0);
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, ~0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);

    renderpath = R_GLSLANG;

    gle::setup();
    setupshaders();
    setuptexcompress();

    GLERROR;

    gl_resize();

    static const char * const rpnames[1] = { "GLSL shader" };
    conoutf(CON_INIT, "Rendering using the OpenGL %s path.", rpnames[renderpath]);
}

VAR(wireframe, 0, 0, 1);

ICOMMAND(getcamyaw, "", (), floatret(camera1->yaw));
ICOMMAND(getcampitch, "", (), floatret(camera1->pitch));
ICOMMAND(getcamroll, "", (), floatret(ovr::modifyroll(camera1->roll)));
ICOMMAND(getcampos, "", (),
{
    defformatstring(pos, "%s %s %s", floatstr(camera1->o.x), floatstr(camera1->o.y), floatstr(camera1->o.z));
    result(pos);
});

/* OF */

CLUAICOMMAND(camera_get_yaw, float, (), {
    return camera1->yaw;
});
CLUAICOMMAND(camera_get_pitch, float, (), {
    return camera1->pitch;
});
CLUAICOMMAND(camera_get_roll, float, (), {
    return camera1->roll;
});
CLUAICOMMAND(camera_get_position, void, (float *val), {
    const vec& o = camera1->o;
    val[0] = o.x;
    val[1] = o.y;
    val[2] = o.z;
});
CLUAICOMMAND(camera_get, void, (float *val), {
    const vec& o = camera1->o;
    val[0] = o.x;
    val[1] = o.y;
    val[2] = o.z;
    val[3] = camera1->yaw;
    val[4] = camera1->pitch;
    val[5] = camera1->roll;
})

vec worldpos, camdir, camright, camup;

/* lamiae */
void setcammatrix()
{
    // move from RH to Z-up LH quake style worldspace
    cammatrix = viewmatrix;
    cammatrix.rotate_around_y((!drawtex ? ovr::modifyroll(camera1->roll) : camera1->roll)*RAD);
    cammatrix.rotate_around_x(camera1->pitch*-RAD);
    cammatrix.rotate_around_z(camera1->yaw*-RAD);
    cammatrix.translate(vec(camera1->o).neg());

    if(!drawtex && ovr::enabled)
        cammatrix.jitter((viewidx ? -1 : 1) * ovr::viewoffset, 0);

    cammatrix.transposedtransformnormal(vec(viewmatrix.b), camdir);
    cammatrix.transposedtransformnormal(vec(viewmatrix.a).neg(), camright);
    cammatrix.transposedtransformnormal(vec(viewmatrix.c), camup);

    camprojmatrix.mul(projmatrix, cammatrix);
    invcammatrix.invert(cammatrix);
    invprojmatrix.invert(projmatrix);
    invcamprojmatrix.invert(camprojmatrix);

    if (!drawtex) {
        lua::push_external("cursor_get_position");
        lua_call(lua::L, 0, 2);

        float x = lua_tonumber(lua::L, -2);
        float y = lua_tonumber(lua::L, -1);
        lua_pop(lua::L, 2);

        vec dir1 = invcamprojmatrix.perspectivetransform(vec(x*2-1, 1-2*y, 2-1));
        vec dir2 = invcamprojmatrix.perspectivetransform(vec(x*2-1, 1-2*y, -1));

        dir1.sub(dir2).normalize();
        if (raycubepos(camera1->o, dir1, worldpos, 0, RAY_CLIPMAT|RAY_SKIPFIRST) == -1)
            worldpos = vec(camdir).mul(2 * worldsize).add(camera1->o);
    }
}

void setcamprojmatrix(bool init = true, bool flush = false)
{
    if(init)
    {
        setcammatrix();

        if(ovr::enabled && !drawtex)
            projmatrix.jitter((viewidx ? -1 : 1) * ovr::distortoffset, 0);
    }
    else camprojmatrix.mul(projmatrix, cammatrix);

    jitteraa(init);

    GLOBALPARAM(camprojmatrix, camprojmatrix);
    GLOBALPARAM(lineardepthscale, projmatrix.lineardepthscale()); //(invprojmatrix.c.z, invprojmatrix.d.z));

    if(flush && Shader::lastshader) Shader::lastshader->flushparams();
}

matrix4 hudmatrix, hudmatrixstack[64];
int hudmatrixpos = 0;

CLUAICOMMAND(hud_get_ss_x, float, (), return hudmatrix.a.x / 2.0f);
CLUAICOMMAND(hud_get_ss_y, float, (), return hudmatrix.b.y / 2.0f);
CLUAICOMMAND(hud_get_so_x, float, (), return hudmatrix.d.x / 2.0f + 0.5f);
CLUAICOMMAND(hud_get_so_y, float, (), return hudmatrix.d.y / 2.0f + 0.5f);

void resethudmatrix()
{
    hudmatrixpos = 0;
    if(ovr::enabled) ovr::ortho(hudmatrix);
    GLOBALPARAM(hudmatrix, hudmatrix);
}

void pushhudmatrix()
{
    if(hudmatrixpos >= 0 && hudmatrixpos < int(sizeof(hudmatrixstack)/sizeof(hudmatrixstack[0]))) hudmatrixstack[hudmatrixpos] = hudmatrix;
    ++hudmatrixpos;
}

void flushhudmatrix(bool flushparams)
{
    GLOBALPARAM(hudmatrix, hudmatrix);
    if(flushparams && Shader::lastshader) Shader::lastshader->flushparams();
}

void pophudmatrix(bool flush, bool flushparams)
{
    --hudmatrixpos;
    if(hudmatrixpos >= 0 && hudmatrixpos < int(sizeof(hudmatrixstack)/sizeof(hudmatrixstack[0])))
    {
        hudmatrix = hudmatrixstack[hudmatrixpos];
        if(flush) flushhudmatrix(flushparams);
    }
}

int vieww = -1, viewh = -1, viewidx = 0;
float curfov = 100, curavatarfov = 65, fovy, aspect;
int farplane;
VARP(zoominvel, 0, 40, 500);
VARP(zoomoutvel, 0, 50, 500);
VARP(zoomfov, 10, 42, 60);
VARP(fov, 10, 100, 150);
VAR(avatarzoomfov, 1, 1, 1);
VAR(avatarfov, 10, 40, 100);
FVAR(avatardepth, 0, 0.8f, 1);
FVARNP(aspect, forceaspect, 0, 0, 1e3f);

CLUAICOMMAND(aspect_get, float, (), return forceaspect;);

static float zoomprogress = 0;
VAR(zoom, -1, 0, 1);

void disablezoom()
{
    zoom = 0;
    zoomprogress = 0;
}

static float forced_camera_fov = -1;

void computezoom()
{
    /* OF */
    if (forced_camera_fov > 0) {
        curfov = forced_camera_fov;
        forced_camera_fov = -1;
        return;
    }

    if(!zoom) { zoomprogress = 0; curfov = fov; curavatarfov = avatarfov; return; }
    if(zoom > 0) zoomprogress = zoominvel ? min(zoomprogress + float(elapsedtime) / zoominvel, 1.0f) : 1;
    {
        zoomprogress = zoomoutvel ? max(zoomprogress - float(elapsedtime) / zoomoutvel, 0.0f) : 0;
        if(zoomprogress <= 0) zoom = 0;
    }
    curfov = zoomfov*zoomprogress + fov*(1 - zoomprogress);
    curavatarfov = avatarzoomfov*zoomprogress + avatarfov*(1 - zoomprogress);
}

FVARP(zoomsens, 1e-4f, 4.5f, 1e4f);
FVARP(zoomaccel, 0, 0, 1000);
VARP(zoomautosens, 0, 1, 1);
FVARP(sensitivity, 1e-4f, 10, 1e4f);
FVARP(sensitivityscale, 1e-4f, 100, 1e4f);
VARP(invmouse, 0, 0, 1);
FVARP(mouseaccel, 0, 0, 1000);

/* OF: start camera code */

VAR(thirdperson, 0, 0, 2);
/* Smoothing factor for the smooth camera. 0 means no smoothing */
FVAR(smoothcamera, 0, 0.2, 100.0);
/* 1 means the camera is 100% away from the closest obstacle (and therefore
 * on the player).
 */
FVAR(thirdpersonavoid, 0, 0.33, 1);
FVAR(thirdpersondistance, 0, 50, 200);
FVAR(thirdpersonup, -50, 10, 50);
FVAR(thirdpersonside, -25, 0, 25);
physent *camera1 = NULL;
bool detachedcamera = false;
bool isthirdperson() { return player!=camera1 || detachedcamera; }

void fixcamerarange()
{
    const float MAXPITCH = 90.0f;
    if(camera1->pitch>MAXPITCH) camera1->pitch = MAXPITCH;
    if(camera1->pitch<-MAXPITCH) camera1->pitch = -MAXPITCH;
    while(camera1->yaw<0.0f) camera1->yaw += 360.0f;
    while(camera1->yaw>=360.0f) camera1->yaw -= 360.0f;
}

void modifyorient(float yaw, float pitch)
{
    // OF: Let scripts customize mousemoving
    if (lua::L) {
        if (lua::push_external("input_mouse_move")) {
            lua_pushnumber(lua::L, yaw);
            lua_pushnumber(lua::L, pitch);
            lua_call(lua::L, 2, 2);
            camera1->yaw   += lua_tonumber(lua::L, -2);
            camera1->pitch += lua_tonumber(lua::L, -1);
            lua_pop(lua::L, 2);
        } else {
            camera1->yaw   += yaw;
            camera1->pitch += pitch;
        }
        fixcamerarange();
        if(camera1!=player && !detachedcamera)
        {
            player->yaw   = camera1->yaw;
            player->pitch = camera1->pitch;
        }
    }
}

void mousemove(int dx, int dy)
{
    float cursens = sensitivity, curaccel = mouseaccel;
    if(zoom)
    {
        if(zoomautosens)
        {
            cursens = float(sensitivity*zoomfov)/fov;
            curaccel = float(mouseaccel*zoomfov)/fov;
        }
        else
        {
            cursens = zoomsens;
            curaccel = zoomaccel;
        }
    }
    if(curaccel && curtime && (dx || dy)) cursens += curaccel * sqrtf(dx*dx + dy*dy)/curtime;
    cursens /= sensitivityscale;
    modifyorient(dx*cursens, dy*cursens*(invmouse ? 1 : -1));
}

#define MIN_CAMERA_MOVE_ITERS 8

int saved_tpdist;

void prepare_character_view() {
    player->pitch  = 0;
    camera1->pitch = 0;
    camera1->yaw   = camera1->yaw;

    saved_tpdist = thirdpersondistance;
    thirdpersondistance = MIN_CAMERA_MOVE_ITERS * 3;
}

void stop_character_view() {
    thirdpersondistance = saved_tpdist;
}

enum { FORCE_POS = 1<<0, FORCE_YAW = 1<<1, FORCE_PITCH = 1<<2, FORCE_ROLL = 1<<3 };

static physent forced_camera;
static int force_flags = 0;
static int saved_thirdperson = -1;

void force_position(float x, float y, float z) {
    force_flags |= FORCE_POS;
    forced_camera.o = vec(x, y, z);
    if (!thirdperson && saved_thirdperson == -1) {
        saved_thirdperson = thirdperson;
        thirdperson = 1;
    }
}
CLUAICOMMAND(camera_force_position, void, (float x, float y, float z), {
    force_position(x, y, z);
});

#define FORCE_PROP(name, flag) \
void force_##name(float name) { \
    force_flags |= flag; \
    forced_camera.name = name; \
    if (!thirdperson && saved_thirdperson == -1) { \
        saved_thirdperson = thirdperson; \
        thirdperson = 1; \
    } \
} \
CLUAICOMMAND(camera_force_##name, void, (float v), { \
    force_##name(v); \
});
FORCE_PROP(yaw, FORCE_YAW)
FORCE_PROP(pitch, FORCE_PITCH)
FORCE_PROP(roll, FORCE_ROLL)

void force_fov(float fov) {
    if (fov == -1) return;
    forced_camera_fov = fov;
    if (!thirdperson && saved_thirdperson == -1) {
        saved_thirdperson = thirdperson;
        thirdperson = 1;
    }
}
CLUAICOMMAND(camera_force_fov, void, (float fov), {
    force_fov(fov);
});

void force_camera(float x, float y, float z, float yaw, float pitch,
float roll, float fov) {
    force_flags = 0;
    force_position(x, y, z);
    force_yaw(yaw);
    force_pitch(pitch);
    force_roll(roll);
    force_fov(fov);
}
CLUAICOMMAND(camera_force, void, (float x, float y, float z, float yaw,
float pitch, float roll, float fov), {
    force_camera(x, y, z, yaw, pitch, roll, fov);
});

void position_camera(physent* camera1) {
    logger::log(logger::INFO, "position_camera");
    INDENT_LOG(logger::INFO);

    if (force_flags) {
        if (force_flags&FORCE_POS) {
            camera1->o = forced_camera.o; force_flags &= ~FORCE_POS;
        }
        if (force_flags&FORCE_YAW) {
            camera1->yaw = forced_camera.yaw; force_flags &= ~FORCE_YAW;
        }
        if (force_flags&FORCE_PITCH) {
            camera1->pitch = forced_camera.pitch; force_flags &= ~FORCE_PITCH;
        }
        if (force_flags&FORCE_ROLL) {
            camera1->roll = forced_camera.roll; force_flags &= ~FORCE_ROLL;
        }
        return; /* next time */
    }

    if (saved_thirdperson != -1) {
        thirdperson = saved_thirdperson;
        saved_thirdperson = -1;
    }

    float saved_camera_speed = camera1->maxspeed;
    camera1->maxspeed = 50;

    matrix3 orient;
    orient.identity();
    orient.rotate_around_z(camera1->yaw*RAD);
    orient.rotate_around_x(camera1->pitch*RAD);
    orient.rotate_around_y(ovr::modifyroll(camera1->roll)*-RAD);
    vec dir = vec(orient.b).neg(), side = vec(orient.a).neg(), up = orient.c;

    if(game::collidecamera()) {
        if(thirdpersonup)
        {
            vec pos = camera1->o;
            float dist = fabs(thirdpersonup);
            if(thirdpersonup < 0) up.neg();
            movecamera(camera1, up, dist, 1);
            movecamera(camera1, up, clamp(dist - camera1->o.dist(pos), 0.0f, 1.0f), 0.1f);
        }

        if(thirdpersonside)
        {
            vec pos = camera1->o;
            float dist = fabs(thirdpersonside);
            if(thirdpersonside < 0) side.neg();
            movecamera(camera1, side, dist, 1);
            movecamera(camera1, side, clamp(dist - camera1->o.dist(pos), 0.0f, 1.0f), 0.1f);
        }

        vec camorigin = camera1->o;
        movecamera(camera1, dir, thirdpersondistance, 1);
        movecamera(camera1, dir, clamp(thirdpersondistance
            - camera1->o.dist(camorigin), 0.0f, 1.0f), 0.1f);

        if (smoothcamera) {
            float intended_dist = camera1->o.dist(camorigin)*(1.0f-thirdpersonavoid);
            static float last_dist = 5;
            float distfactor = clamp(1.0f - (curtime/1000.0f)/smoothcamera, 0.0f, 1.0f);
            float actual_dist = distfactor*last_dist + (1-distfactor)*intended_dist;

            camera1->o = camorigin;
            movecamera(camera1, dir, actual_dist, 1);
            movecamera(camera1, dir, clamp(actual_dist
                - camera1->o.dist(camorigin), 0.0f, 1.0f), 0.1f);
            last_dist = actual_dist;
        }
    } else {
        if(thirdpersonup) camera1->o.add(vec(up).mul(thirdpersonup));
        if(thirdpersonside) camera1->o.add(vec(side).mul(thirdpersonside));
        camera1->o.add(vec(dir).mul(thirdpersondistance));
    }

    camera1->maxspeed = saved_camera_speed;

    static physent acam;
    static vec last_playerpos;

    vec tmp(acam.o);
    tmp.sub(camera1->o);

    acam.normalize_yaw(camera1->yaw);
    acam.normalize_pitch(camera1->pitch);

    extern int freecursor, freeeditcursor;
    if (smoothcamera && (editmode ? freeeditcursor >= 2 : freecursor >= 2)
    && tmp.magnitude() < (50 * (player->radius))
    && fabs(camera1->yaw   - acam.yaw)   < 30.0f
    && fabs(camera1->pitch - acam.pitch) < 30.0f) {
        float camfactor = clamp(1.0f - (curtime/1000.0f)/smoothcamera, 0.0f, 1.0f);

        vec tmp = player->o;
        tmp.sub(last_playerpos);
        acam.o.add(tmp); // Prevent camera stutter

        acam.o.mul(camfactor);
        tmp = camera1->o;
        tmp.mul(1-camfactor);
        acam.o.add(tmp);

        acam.yaw = camfactor*acam.yaw + (1-camfactor)*camera1->yaw;
        acam.pitch = camfactor*acam.pitch + (1-camfactor)*camera1->pitch;

        camera1->o = acam.o;
        camera1->yaw = acam.yaw;
        camera1->pitch = acam.pitch;
    } else {
        acam.o = camera1->o;
        acam.yaw = camera1->yaw;
        acam.pitch = camera1->pitch;
    }

    last_playerpos = player->o;
}

void recomputecamera()
{
    game::setupcamera();
    computezoom();

    bool shoulddetach = thirdperson > 1 || game::detachcamera();
    if(!thirdperson && !shoulddetach)
    {
        camera1 = player;
        detachedcamera = false;
    }
    else
    {
        static physent tempcamera;
        camera1 = &tempcamera;
        if(detachedcamera && shoulddetach) camera1->o = player->o;
        else
        {
            /* OF */
            /*if (!is_character_viewing())*/ *camera1 = *player;
            detachedcamera = shoulddetach;
        }
        camera1->reset();
        camera1->type = ENT_CAMERA;
        camera1->move = -1;
        camera1->eyeheight = camera1->aboveeye = camera1->radius = camera1->xradius = camera1->yradius = 2;

        position_camera(camera1);
    }

    setviewcell(camera1->o);
}

/* OF: end camera code */

float calcfrustumboundsphere(float nearplane, float farplane,  const vec &pos, const vec &view, vec &center)
{
    if(drawtex == DRAWTEX_MINIMAP)
    {
        center = minimapcenter;
        return minimapradius.magnitude();
    }

    float width = tan(fov/2.0f*RAD), height = width / aspect,
          cdist = ((nearplane + farplane)/2)*(1 + width*width + height*height);
    if(cdist <= farplane)
    {
        center = vec(view).mul(cdist).add(pos);
        return vec(width*nearplane, height*nearplane, cdist-nearplane).magnitude();
    }
    else
    {
        center = vec(view).mul(farplane).add(pos);
        return vec(width*farplane, height*farplane, 0).magnitude();
    }
}

extern const matrix4 viewmatrix(vec(-1, 0, 0), vec(0, 0, 1), vec(0, -1, 0));
extern const matrix4 invviewmatrix(vec(-1, 0, 0), vec(0, 0, -1), vec(0, 1, 0));
matrix4 cammatrix, projmatrix, camprojmatrix, invcammatrix, invcamprojmatrix, invprojmatrix;

FVAR(nearplane, 0.01f, 0.54f, 2.0f);

vec calcavatarpos(const vec &pos, float dist)
{
    vec eyepos;
    cammatrix.transform(pos, eyepos);
    GLdouble ydist = nearplane * tan(curavatarfov/2*RAD), xdist = ydist * aspect;
    vec4 scrpos;
    scrpos.x = eyepos.x*nearplane/xdist;
    scrpos.y = eyepos.y*nearplane/ydist;
    scrpos.z = (eyepos.z*(farplane + nearplane) - 2*nearplane*farplane) / (farplane - nearplane);
    scrpos.w = -eyepos.z;

    vec worldpos = invcamprojmatrix.perspectivetransform(scrpos);
    vec dir = vec(worldpos).sub(camera1->o).rescale(dist);
    return dir.add(camera1->o);
}

void renderavatar()
{
    if(!isthirdperson())
    {
        matrix4 oldprojmatrix = projmatrix;
        float avatarfovy = curavatarfov;
        if(ovr::enabled && ovr::fov) avatarfovy *= ovr::fov/fov;
        projmatrix.perspective(avatarfovy, aspect, nearplane, farplane);
        projmatrix.scalez(avatardepth);
        setcamprojmatrix(false);
        game::renderavatar();
        projmatrix = oldprojmatrix;
        setcamprojmatrix(false);
    }
}

FVAR(polygonoffsetfactor, -1e4f, -3.0f, 1e4f);
FVAR(polygonoffsetunits, -1e4f, -3.0f, 1e4f);
FVAR(depthoffset, -1e4f, 0.01f, 1e4f);

matrix4 nooffsetmatrix;

void enablepolygonoffset(GLenum type)
{
    if(!depthoffset)
    {
        glPolygonOffset(polygonoffsetfactor, polygonoffsetunits);
        glEnable(type);
        return;
    }

    projmatrix = nojittermatrix;
    nooffsetmatrix = projmatrix;
    projmatrix.d.z += depthoffset * projmatrix.c.z;
    setcamprojmatrix(false, true);
}

void disablepolygonoffset(GLenum type)
{
    if(!depthoffset)
    {
        glDisable(type);
        return;
    }

    projmatrix = nooffsetmatrix;
    setcamprojmatrix(false, true);
}

bool calcspherescissor(const vec &center, float size, float &sx1, float &sy1, float &sx2, float &sy2, float &sz1, float &sz2)
{
    vec e;
    cammatrix.transform(center, e);
    if(e.z > 2*size) { sx1 = sy1 = sz1 = 1; sx2 = sy2 = sz2 = -1; return false; }
    if(drawtex == DRAWTEX_MINIMAP)
    {
        vec dir(size, size, size);
        if(projmatrix.a.x < 0) dir.x = -dir.x;
        if(projmatrix.b.y < 0) dir.y = -dir.y;
        if(projmatrix.c.z < 0) dir.z = -dir.z;
        sx1 = max(projmatrix.a.x*(e.x - dir.x) + projmatrix.d.x, -1.0f);
        sx2 = min(projmatrix.a.x*(e.x + dir.x) + projmatrix.d.x, 1.0f);
        sy1 = max(projmatrix.b.y*(e.y - dir.y) + projmatrix.d.y, -1.0f);
        sy2 = min(projmatrix.b.y*(e.y + dir.y) + projmatrix.d.y, 1.0f);
        sz1 = max(projmatrix.c.z*(e.z - dir.z) + projmatrix.d.z, -1.0f);
        sz2 = min(projmatrix.c.z*(e.z + dir.z) + projmatrix.d.z, 1.0f);
        return sx1 < sx2 && sy1 < sy2 && sz1 < sz2;
    }
    float zzrr = e.z*e.z - size*size,
          dx = e.x*e.x + zzrr, dy = e.y*e.y + zzrr,
          focaldist = 1.0f/tan(fovy*0.5f*RAD);
    sx1 = sy1 = -1;
    sx2 = sy2 = 1;
    #define CHECKPLANE(c, dir, focaldist, low, high) \
    do { \
        float nzc = (cz*cz + 1) / (cz dir drt) - cz, \
              pz = (d##c)/(nzc*e.c - e.z); \
        if(pz > 0) \
        { \
            float c = (focaldist)*nzc, \
                  pc = pz*nzc; \
            if(pc < e.c) low = c; \
            else if(pc > e.c) high = c; \
        } \
    } while(0)
    if(dx > 0)
    {
        float cz = e.x/e.z, drt = sqrtf(dx)/size;
        CHECKPLANE(x, -, focaldist/aspect, sx1, sx2);
        CHECKPLANE(x, +, focaldist/aspect, sx1, sx2);
        if(ovr::enabled)
        {
            float offset = (viewidx ? -1 : 1) * ovr::distortoffset;
            if(sx1 > -1) sx1 += offset;
            if(sx2 < 1) sx2 += offset;
        }
    }
    if(dy > 0)
    {
        float cz = e.y/e.z, drt = sqrtf(dy)/size;
        CHECKPLANE(y, -, focaldist, sy1, sy2);
        CHECKPLANE(y, +, focaldist, sy1, sy2);
    }
    float z1 = min(e.z + size, -1e-3f - nearplane), z2 = min(e.z - size, -1e-3f - nearplane);
    sz1 = (z1*projmatrix.c.z + projmatrix.d.z) / (z1*projmatrix.c.w + projmatrix.d.w);
    sz2 = (z2*projmatrix.c.z + projmatrix.d.z) / (z2*projmatrix.c.w + projmatrix.d.w);
    return sx1 < sx2 && sy1 < sy2 && sz1 < sz2;
}

bool calcbbscissor(const ivec &bbmin, const ivec &bbmax, float &sx1, float &sy1, float &sx2, float &sy2)
{
#define ADDXYSCISSOR(p) do { \
        if(p.z >= -p.w) \
        { \
            float x = p.x / p.w, y = p.y / p.w; \
            sx1 = min(sx1, x); \
            sy1 = min(sy1, y); \
            sx2 = max(sx2, x); \
            sy2 = max(sy2, y); \
        } \
    } while(0)
    vec4 v[8];
    sx1 = sy1 = 1;
    sx2 = sy2 = -1;
    camprojmatrix.transform(vec(bbmin.x, bbmin.y, bbmin.z), v[0]);
    ADDXYSCISSOR(v[0]);
    camprojmatrix.transform(vec(bbmax.x, bbmin.y, bbmin.z), v[1]);
    ADDXYSCISSOR(v[1]);
    camprojmatrix.transform(vec(bbmin.x, bbmax.y, bbmin.z), v[2]);
    ADDXYSCISSOR(v[2]);
    camprojmatrix.transform(vec(bbmax.x, bbmax.y, bbmin.z), v[3]);
    ADDXYSCISSOR(v[3]);
    camprojmatrix.transform(vec(bbmin.x, bbmin.y, bbmax.z), v[4]);
    ADDXYSCISSOR(v[4]);
    camprojmatrix.transform(vec(bbmax.x, bbmin.y, bbmax.z), v[5]);
    ADDXYSCISSOR(v[5]);
    camprojmatrix.transform(vec(bbmin.x, bbmax.y, bbmax.z), v[6]);
    ADDXYSCISSOR(v[6]);
    camprojmatrix.transform(vec(bbmax.x, bbmax.y, bbmax.z), v[7]);
    ADDXYSCISSOR(v[7]);
    if(sx1 > sx2 || sy1 > sy2) return false;
    loopi(8)
    {
        const vec4 &p = v[i];
        if(p.z >= -p.w) continue;
        loopj(3)
        {
            const vec4 &o = v[i^(1<<j)];
            if(o.z <= -o.w) continue;
#define INTERPXYSCISSOR(p, o) do { \
            float t = (p.z + p.w)/(p.z + p.w - o.z - o.w), \
                  w = p.w + t*(o.w - p.w), \
                  x = (p.x + t*(o.x - p.x))/w, \
                  y = (p.y + t*(o.y - p.y))/w; \
            sx1 = min(sx1, x); \
            sy1 = min(sy1, y); \
            sx2 = max(sx2, x); \
            sy2 = max(sy2, y); \
        } while(0)
            INTERPXYSCISSOR(p, o);
        }
    }
    sx1 = max(sx1, -1.0f);
    sy1 = max(sy1, -1.0f);
    sx2 = min(sx2, 1.0f);
    sy2 = min(sy2, 1.0f);
    return true;
}

bool calcspotscissor(const vec &origin, float radius, const vec &dir, int spot, const vec &spotx, const vec &spoty, float &sx1, float &sy1, float &sx2, float &sy2, float &sz1, float &sz2)
{
    float spotscale = radius * tan360(spot);
    vec up = vec(spotx).mul(spotscale), right = vec(spoty).mul(spotscale), center = vec(dir).mul(radius).add(origin);
#define ADDXYZSCISSOR(p) do { \
        if(p.z >= -p.w) \
        { \
            float x = p.x / p.w, y = p.y / p.w, z = p.z / p.w; \
            sx1 = min(sx1, x); \
            sy1 = min(sy1, y); \
            sz1 = min(sz1, z); \
            sx2 = max(sx2, x); \
            sy2 = max(sy2, y); \
            sz2 = max(sz2, z); \
        } \
    } while(0)
    vec4 v[5];
    sx1 = sy1 = sz1 = 1;
    sx2 = sy2 = sz2 = -1;
    camprojmatrix.transform(vec(center).sub(right).sub(up), v[0]);
    ADDXYZSCISSOR(v[0]);
    camprojmatrix.transform(vec(center).add(right).sub(up), v[1]);
    ADDXYZSCISSOR(v[1]);
    camprojmatrix.transform(vec(center).sub(right).add(up), v[2]);
    ADDXYZSCISSOR(v[2]);
    camprojmatrix.transform(vec(center).add(right).add(up), v[3]);
    ADDXYZSCISSOR(v[3]);
    camprojmatrix.transform(origin, v[4]);
    ADDXYZSCISSOR(v[4]);
    if(sx1 > sx2 || sy1 > sy2 || sz1 > sz2) return false;
    loopi(4)
    {
        const vec4 &p = v[i];
        if(p.z >= -p.w) continue;
        loopj(2)
        {
            const vec4 &o = v[i^(1<<j)];
            if(o.z <= -o.w) continue;
#define INTERPXYZSCISSOR(p, o) do { \
            float t = (p.z + p.w)/(p.z + p.w - o.z - o.w), \
                  w = p.w + t*(o.w - p.w), \
                  x = (p.x + t*(o.x - p.x))/w, \
                  y = (p.y + t*(o.y - p.y))/w; \
            sx1 = min(sx1, x); \
            sy1 = min(sy1, y); \
            sz1 = min(sz1, -1.0f); \
            sx2 = max(sx2, x); \
            sy2 = max(sy2, y); \
        } while(0)
            INTERPXYZSCISSOR(p, o);
        }
        if(v[4].z > -v[4].w) INTERPXYZSCISSOR(p, v[4]);
    }
    if(v[4].z < -v[4].w) loopj(4)
    {
        const vec4 &o = v[j];
        if(o.z <= -o.w) continue;
        INTERPXYZSCISSOR(v[4], o);
    }
    sx1 = max(sx1, -1.0f);
    sy1 = max(sy1, -1.0f);
    sz1 = max(sz1, -1.0f);
    sx2 = min(sx2, 1.0f);
    sy2 = min(sy2, 1.0f);
    sz2 = min(sz2, 1.0f);
    return true;
}

void screenquad()
{
    gle::defvertex(2);
    gle::begin(GL_TRIANGLE_STRIP);
    gle::attribf(1, -1);
    gle::attribf(-1, -1);
    gle::attribf(1, 1);
    gle::attribf(-1, 1);
    gle::end();
    gle::disable();
}

#define SCREENQUAD1(x1, y1, x2, y2, sx1, sy1, sx2, sy2) { \
    gle::defvertex(2); \
    gle::deftexcoord0(); \
    gle::begin(GL_TRIANGLE_STRIP); \
    gle::attribf(x2, y1); gle::attribf(sx2, sy1); \
    gle::attribf(x1, y1); gle::attribf(sx1, sy1); \
    gle::attribf(x2, y2); gle::attribf(sx2, sy2); \
    gle::attribf(x1, y2); gle::attribf(sx1, sy2); \
    gle::end(); \
}

void screenquad(float sw, float sh)
{
    SCREENQUAD1(-1, -1, 1, 1, 0, 0, sw, sh);
    gle::disable();
}

void screenquadflipped(float sw, float sh)
{
    SCREENQUAD1(-1, -1, 1, 1, 0, sh, sw, 0);
    gle::disable();
}

void screenquadreorient(float sw, float sh, bool flipx, bool flipy, bool swapxy)
{
    float sx1 = 0, sy1 = 0, sx2 = sw, sy2 = sh;
    if(swapxy) swap(sx2, sy2);
    if(flipx) swap(sx1, sx2);
    if(flipy) swap(sy1, sy2);
    gle::defvertex(2);
    gle::deftexcoord0();
    gle::begin(GL_TRIANGLE_STRIP);
    if(swapxy)
    {
        gle::attribf( 1, -1); gle::attribf(sy1, sx2);
        gle::attribf(-1, -1); gle::attribf(sy1, sx1);
        gle::attribf( 1,  1); gle::attribf(sy2, sx2);
        gle::attribf(-1,  1); gle::attribf(sy2, sx1);
    }
    else
    {
        gle::attribf( 1, -1); gle::attribf(sx2, sy1);
        gle::attribf(-1, -1); gle::attribf(sx1, sy1);
        gle::attribf( 1,  1); gle::attribf(sx2, sy2);
        gle::attribf(-1,  1); gle::attribf(sx1, sy2);
    }
    gle::end();
    gle::disable();
}

#define SCREENQUAD2(x1, y1, x2, y2, sx1, sy1, sx2, sy2, tx1, ty1, tx2, ty2) { \
    gle::defvertex(2); \
    gle::deftexcoord0(); \
    gle::deftexcoord1(); \
    gle::begin(GL_TRIANGLE_STRIP); \
    gle::attribf(x2, y1); gle::attribf(sx2, sy1); gle::attribf(tx2, ty1); \
    gle::attribf(x1, y1); gle::attribf(sx1, sy1); gle::attribf(tx1, ty1); \
    gle::attribf(x2, y2); gle::attribf(sx2, sy2); gle::attribf(tx2, ty2); \
    gle::attribf(x1, y2); gle::attribf(sx1, sy2); gle::attribf(tx1, ty2); \
    gle::end(); \
}

void screenquad(float sw, float sh, float sw2, float sh2)
{
    SCREENQUAD2(-1, -1, 1, 1, 0, 0, sw, sh, 0, 0, sw2, sh2);
    gle::disable();
}

void screenquadoffset(float x, float y, float w, float h)
{
    SCREENQUAD1(-1, -1, 1, 1, x, y, x+w, y+h);
    gle::disable();
}

void screenquadoffset(float x, float y, float w, float h, float x2, float y2, float w2, float h2)
{
    SCREENQUAD2(-1, -1, 1, 1, x, y, x+w, y+h, x2, y2, x2+w2, y2+h2);
    gle::disable();
}

void hudquad(float x, float y, float w, float h, float tx, float ty, float tw, float th)
{
    SCREENQUAD1(x, y, x+w, y+h, tx, ty, tx+tw, ty+th);
}

void debugquad(float x, float y, float w, float h, float tx, float ty, float tw, float th)
{
    SCREENQUAD1(x, y, x+w, y+h, tx, ty+th, tx+tw, ty);
    gle::disable();
}

VARR(fog, 16, 4000, 1000024);
bvec fogcolorv(0x80, 0x99, 0xB3);
HVARFR(fogcolor, 0, 0x8099B3, 0xFFFFFF,
{
    fogcolorv = bvec((fogcolor>>16)&0xFF, (fogcolor>>8)&0xFF, fogcolor&0xFF);
});
VAR(fogoverlay, 0, 1, 1);

static float findsurface(int fogmat, const vec &v, int &abovemat)
{
    fogmat &= MATF_VOLUME;
    ivec o(v), co;
    int csize;
    do
    {
        cube &c = lookupcube(o, 0, co, csize);
        int mat = c.material&MATF_VOLUME;
        if(mat != fogmat)
        {
            abovemat = isliquid(mat) ? c.material : MAT_AIR;
            return o.z;
        }
        o.z = co.z + csize;
    }
    while(o.z < worldsize);
    abovemat = MAT_AIR;
    return worldsize;
}

static void blendfog(int fogmat, float below, float blend, float logblend, float &start, float &end, vec &fogc)
{
    switch(fogmat&MATF_VOLUME)
    {
        case MAT_WATER:
        {
            const bvec &wcol = getwatercolorv(fogmat), &wdeepcol = getwaterdeepcolorv(fogmat);
            int wfog = getwaterfog(fogmat), wdeep = getwaterdeep(fogmat);
            float deepfade = clamp(below/max(wdeep, wfog), 0.0f, 1.0f);
            vec color;
            color.lerp(wcol.tocolor(), wdeepcol.tocolor(), deepfade);
            fogc.add(vec(color).mul(blend));
            end += logblend*min(fog, max(wfog*2, 16));
            break;
        }

        case MAT_LAVA:
        {
            const bvec &lcol = getlavacolorv(fogmat);
            int lfog = getlavafog(fogmat);
            fogc.add(lcol.tocolor().mul(blend));
            end += logblend*min(fog, max(lfog*2, 16));
            break;
        }

        default:
            fogc.add(fogcolorv.tocolor().mul(blend));
            start += logblend*(fog+64)/8;
            end += logblend*fog;
            break;
    }
}

vec curfogcolor(0, 0, 0);

void setfogcolor(const vec &v)
{
    GLOBALPARAM(fogcolor, v);
}

void zerofogcolor()
{
    setfogcolor(vec(0, 0, 0));
}

void resetfogcolor()
{
    setfogcolor(curfogcolor);
}

FVAR(fogintensity, 0, 0.15f, 1);

float calcfogdensity(float dist)
{
    return log(fogintensity)/(M_LN2*dist);
}

FVAR(fogcullintensity, 0, 1e-3f, 1);

float calcfogcull()
{
    return log(fogcullintensity) / (M_LN2*calcfogdensity(fog - (fog+64)/8));
}

static void setfog(int fogmat, float below = 0, float blend = 1, int abovemat = MAT_AIR)
{
    float start = 0, end = 0;
    float logscale = 256, logblend = log(1 + (logscale - 1)*blend) / log(logscale);

    curfogcolor = vec(0, 0, 0);
    blendfog(fogmat, below, blend, logblend, start, end, curfogcolor);
    if(blend < 1) blendfog(abovemat, 0, 1-blend, 1-logblend, start, end, curfogcolor);
    curfogcolor.mul(ldrscale);

    GLOBALPARAM(fogcolor, curfogcolor);

    float fogdensity = calcfogdensity(end-start);
    GLOBALPARAMF(fogdensity, fogdensity, 1/exp(M_LN2*start*fogdensity));
}

static void blendfogoverlay(int fogmat, float below, float blend, vec &overlay)
{
    float maxc;
    switch(fogmat&MATF_VOLUME)
    {
        case MAT_WATER:
        {
            const bvec &wcol = getwatercolorv(fogmat), &wdeepcol = getwaterdeepcolorv(fogmat);
            int wfog = getwaterfog(fogmat), wdeep = getwaterdeep(fogmat);
            float deepfade = clamp(below/max(wdeep, wfog), 0.0f, 1.0f);
            vec color = vec(wcol.r, wcol.g, wcol.b).lerp(vec(wdeepcol.r, wdeepcol.g, wdeepcol.b), deepfade);
            overlay.add(color.div(min(32.0f + max(color.r, max(color.g, color.b))*7.0f/8.0f, 255.0f)).max(0.4f).mul(blend));
            break;
        }

        case MAT_LAVA:
        {
            const bvec &lcol = getlavacolorv(fogmat);
            maxc = max(lcol.r, max(lcol.g, lcol.b));
            overlay.add(vec(lcol.r, lcol.g, lcol.b).div(min(32.0f + maxc*7.0f/8.0f, 255.0f)).max(0.4f).mul(blend));
            break;
        }

        default:
            overlay.add(blend);
            break;
    }
}

void drawfogoverlay(int fogmat, float fogbelow, float fogblend, int abovemat)
{
    SETSHADER(fogoverlay);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    vec overlay(0, 0, 0);
    blendfogoverlay(fogmat, fogbelow, fogblend, overlay);
    blendfogoverlay(abovemat, 0, 1-fogblend, overlay);

    gle::color(overlay);
    screenquad();

    glDisable(GL_BLEND);
}

int drawtex = 0;

GLuint minimaptex = 0;
vec minimapcenter(0, 0, 0), minimapradius(0, 0, 0), minimapscale(0, 0, 0);

void clearminimap()
{
    if(minimaptex) { glDeleteTextures(1, &minimaptex); minimaptex = 0; }
}

VARR(minimapheight, 0, 0, 2<<16);
bvec minimapcolorv(0, 0, 0);
HVARFR(minimapcolor, 0, 0, 0xFFFFFF,
{
    minimapcolorv = bvec((minimapcolor>>16)&0xFF, (minimapcolor>>8)&0xFF, minimapcolor&0xFF);
});
VARR(minimapclip, 0, 0, 1);
VARFP(minimapsize, 7, 8, 10, { if(minimaptex) drawminimap(); });

void bindminimap()
{
    glBindTexture(GL_TEXTURE_2D, minimaptex);
}

void clipminimap(ivec &bbmin, ivec &bbmax, cube *c = worldroot, const ivec &co = ivec(0, 0, 0), int size = worldsize>>1)
{
    loopi(8)
    {
        ivec o(i, co, size);
        if(c[i].children) clipminimap(bbmin, bbmax, c[i].children, o, size>>1);
        else if(!isentirelysolid(c[i]) && (c[i].material&MATF_CLIP)!=MAT_CLIP)
        {
            loopk(3) bbmin[k] = min(bbmin[k], o[k]);
            loopk(3) bbmax[k] = max(bbmax[k], o[k] + size);
        }
    }
}

void drawminimap()
{
    if(!game::needminimap()) { clearminimap(); return; }

    GLERROR;
    renderprogress(0, "generating mini-map...", 0, !renderedframe);

    drawtex = DRAWTEX_MINIMAP;

    GLERROR;
    gl_setupframe(true);

    int size = 1<<minimapsize, sizelimit = min(hwtexsize, min(gw, gh));
    while(size > sizelimit) size /= 2;
    if(!minimaptex) glGenTextures(1, &minimaptex);

    extern vector<vtxarray *> valist;
    ivec bbmin(worldsize, worldsize, worldsize), bbmax(0, 0, 0);
    loopv(valist)
    {
        vtxarray *va = valist[i];
        loopk(3)
        {
            if(va->geommin[k]>va->geommax[k]) continue;
            bbmin[k] = min(bbmin[k], va->geommin[k]);
            bbmax[k] = max(bbmax[k], va->geommax[k]);
        }
    }
    if(minimapclip)
    {
        ivec clipmin(worldsize, worldsize, worldsize), clipmax(0, 0, 0);
        clipminimap(clipmin, clipmax);
        loopk(2) bbmin[k] = max(bbmin[k], clipmin[k]);
        loopk(2) bbmax[k] = min(bbmax[k], clipmax[k]);
    }

    minimapradius = vec(bbmax).sub(vec(bbmin)).mul(0.5f);
    minimapcenter = vec(bbmin).add(minimapradius);
    minimapradius.x = minimapradius.y = max(minimapradius.x, minimapradius.y);
    minimapscale = vec((0.5f - 1.0f/size)/minimapradius.x, (0.5f - 1.0f/size)/minimapradius.y, 1.0f);

    physent *oldcamera = camera1;
    static physent cmcamera;
    cmcamera = *player;
    cmcamera.reset();
    cmcamera.type = ENT_CAMERA;
    cmcamera.o = vec(minimapcenter.x, minimapcenter.y, minimapheight > 0 ? minimapheight : minimapcenter.z + minimapradius.z + 1);
    cmcamera.yaw = 0;
    cmcamera.pitch = -90;
    cmcamera.roll = 0;
    camera1 = &cmcamera;
    setviewcell(vec(-1, -1, -1));

    float oldldrscale = ldrscale, oldldrscaleb = ldrscaleb;
    int oldvieww = vieww, oldviewh = viewh;
    vieww = viewh = size;

    float zscale = max(float(minimapheight), minimapcenter.z + minimapradius.z + 1) + 1;

    projmatrix.ortho(-minimapradius.x, minimapradius.x, -minimapradius.y, minimapradius.y, 0, 2*zscale);
    setcamprojmatrix();

    resetlights();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    xtravertsva = xtraverts = glde = gbatches = vtris = vverts = 0;
    flipqueries();

    ldrscale = 1;
    ldrscaleb = ldrscale/255;

    visiblecubes(false);
    collectlights();
    findmaterials();

    rendergbuffer();

    rendershadowatlas();

    shademinimap(vec(minimapcolorv.x*ldrscaleb, minimapcolorv.y*ldrscaleb, minimapcolorv.z*ldrscaleb));

    if(minimapheight > 0 && minimapheight < minimapcenter.z + minimapradius.z)
    {
        camera1->o.z = minimapcenter.z + minimapradius.z + 1;
        projmatrix.ortho(-minimapradius.x, minimapradius.x, -minimapradius.y, minimapradius.y, -zscale, zscale);
        setcamprojmatrix();
        rendergbuffer(false);
        shademinimap();
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    vieww = oldvieww;
    viewh = oldviewh;
    ldrscale = oldldrscale;
    ldrscaleb = oldldrscaleb;

    camera1 = oldcamera;
    drawtex = 0;

    createtexture(minimaptex, size, size, NULL, 3, 1, GL_RGB5, GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat border[4] = { minimapcolorv.x/255.0f, minimapcolorv.y/255.0f, minimapcolorv.z/255.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint fbo = 0;
    glGenFramebuffers_(1, &fbo);
    glBindFramebuffer_(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D_(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, minimaptex, 0);
    copyhdr(size, size, fbo);
    glBindFramebuffer_(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers_(1, &fbo);

    glViewport(0, 0, hudw, hudh);
}

void drawcubemap(int size, const vec &o, float yaw, float pitch, const cubemapside &side, bool onlysky)
{
    drawtex = DRAWTEX_ENVMAP;

    physent *oldcamera = camera1;
    static physent cmcamera;
    cmcamera = *player;
    cmcamera.reset();
    cmcamera.type = ENT_CAMERA;
    cmcamera.o = o;
    cmcamera.yaw = yaw;
    cmcamera.pitch = pitch;
    cmcamera.roll = 0;
    camera1 = &cmcamera;
    setviewcell(camera1->o);

    float fogmargin = 1 + WATER_AMPLITUDE + nearplane;
    int fogmat = lookupmaterial(vec(camera1->o.x, camera1->o.y, camera1->o.z - fogmargin))&(MATF_VOLUME|MATF_INDEX), abovemat = MAT_AIR;
    float fogbelow = 0;
    if(isliquid(fogmat&MATF_VOLUME))
    {
        float z = findsurface(fogmat, vec(camera1->o.x, camera1->o.y, camera1->o.z - fogmargin), abovemat) - WATER_OFFSET;
        if(camera1->o.z < z + fogmargin)
        {
            fogbelow = z - camera1->o.z;
        }
        else fogmat = abovemat;
    }
    else fogmat = MAT_AIR;
    setfog(abovemat);

    float oldaspect = aspect, oldfovy = fovy, oldfov = curfov, oldldrscale = ldrscale, oldldrscaleb = ldrscaleb;
    int oldfarplane = farplane, oldvieww = vieww, oldviewh = viewh;
    curfov = fovy = 90;
    aspect = 1;
    farplane = worldsize*2;
    vieww = viewh = size;
    projmatrix.perspective(fovy, aspect, nearplane, farplane);
    setcamprojmatrix();

    resetlights();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    xtravertsva = xtraverts = glde = gbatches = vtris = vverts = 0;
    flipqueries();

    ldrscale = 1;
    ldrscaleb = ldrscale/255;

    visiblecubes(onlysky);

    if(onlysky)
    {
        preparegbuffer();
        GLERROR;

        shadesky();
        GLERROR;
    }
    else
    {
        rendergbuffer();
        GLERROR;

        renderradiancehints();
        GLERROR;

        rendershadowatlas();
        GLERROR;

        shadegbuffer();
        GLERROR;

        if(fogmat)
        {
            setfog(fogmat, fogbelow, 1, abovemat);

            renderwaterfog(fogmat, fogbelow);

            setfog(fogmat, fogbelow, clamp(fogbelow, 0.0f, 1.0f), abovemat);
        }

        rendertransparent();
        GLERROR;
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    aspect = oldaspect;
    fovy = oldfovy;
    curfov = oldfov;
    farplane = oldfarplane;
    vieww = oldvieww;
    viewh = oldviewh;
    ldrscale = oldldrscale;
    ldrscaleb = oldldrscaleb;

    camera1 = oldcamera;
    drawtex = 0;
}

namespace modelpreview
{
    physent *oldcamera;
    physent camera;

    float oldaspect, oldfovy, oldfov, oldldrscale, oldldrscaleb;
    int oldfarplane, oldvieww, oldviewh;

    int x, y, w, h;
    bool background, scissor;

    void start(int x, int y, int w, int h, bool background, bool scissor)
    {
        modelpreview::x = x;
        modelpreview::y = y;
        modelpreview::w = w;
        modelpreview::h = h;
        modelpreview::background = background;
        modelpreview::scissor = scissor;

        setupgbuffer();

        useshaderbyname("modelpreview");

        drawtex = DRAWTEX_MODELPREVIEW;

        oldcamera = camera1;
        camera = *camera1;
        camera.reset();
        camera.type = ENT_CAMERA;
        camera.o = vec(0, 0, 0);
        camera.yaw = 0;
        camera.pitch = 0;
        camera.roll = 0;
        camera1 = &camera;

        oldaspect = aspect;
        oldfovy = fovy;
        oldfov = curfov;
        oldldrscale = ldrscale;
        oldldrscaleb = ldrscaleb;
        oldfarplane = farplane;
        oldvieww = vieww;
        oldviewh = viewh;

        aspect = w/float(h);
        fovy = 90;
        curfov = 2*atan2(tan(fovy/2*RAD), 1/aspect)/RAD;
        farplane = 1024;
        vieww = min(gw, w);
        viewh = min(gh, h);
        ldrscale = 1;
        ldrscaleb = ldrscale/255;

        projmatrix.perspective(fovy, aspect, nearplane, farplane);
        setcamprojmatrix();

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        preparegbuffer();

        resetmodelbatches();
    }

    void end()
    {
        rendermodelbatches();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        shademodelpreview(x, y, w, h, background, scissor);

        aspect = oldaspect;
        fovy = oldfovy;
        curfov = oldfov;
        farplane = oldfarplane;
        vieww = oldvieww;
        viewh = oldviewh;
        ldrscale = oldldrscale;
        ldrscaleb = oldldrscaleb;

        camera1 = oldcamera;
        drawtex = 0;
    }
}

int xtraverts, xtravertsva;

void gl_drawview()
{
    GLuint scalefbo = shouldscale();
    if(scalefbo) { vieww = gw; viewh = gh; }

    float fogmargin = 1 + WATER_AMPLITUDE + nearplane;
    int fogmat = lookupmaterial(vec(camera1->o.x, camera1->o.y, camera1->o.z - fogmargin))&(MATF_VOLUME|MATF_INDEX), abovemat = MAT_AIR;
    float fogbelow = 0;
    if(isliquid(fogmat&MATF_VOLUME))
    {
        float z = findsurface(fogmat, vec(camera1->o.x, camera1->o.y, camera1->o.z - fogmargin), abovemat) - WATER_OFFSET;
        if(camera1->o.z < z + fogmargin)
        {
            fogbelow = z - camera1->o.z;
        }
        else fogmat = abovemat;
    }
    else fogmat = MAT_AIR;
    setfog(abovemat);
    //setfog(fogmat, fogbelow, 1, abovemat);

    farplane = worldsize*2;

    resetlights();

    projmatrix.perspective(fovy, aspect, nearplane, farplane);
    setcamprojmatrix();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    ldrscale = 0.5f;
    ldrscaleb = ldrscale/255;

    visiblecubes();

    if(wireframe && editmode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    rendergbuffer();

    if(wireframe && editmode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else if(limitsky() && editmode) renderexplicitsky(true);

    renderao();
    GLERROR;

    // render grass after AO to avoid disturbing shimmering patterns
    generategrass();
    rendergrass();
    GLERROR;

    glFlush();

    if(!rhinoq || !oqfrags || (wireframe && editmode))
    {
        renderradiancehints();
        GLERROR;
    }

    rendershadowatlas();
    GLERROR;

    shadegbuffer();
    GLERROR;

    if(fogmat)
    {
        setfog(fogmat, fogbelow, 1, abovemat);

        renderwaterfog(fogmat, fogbelow);

        setfog(fogmat, fogbelow, clamp(fogbelow, 0.0f, 1.0f), abovemat);
    }

    rendertransparent();
    GLERROR;

    if(fogmat) setfog(fogmat, fogbelow, 1, abovemat);

    if(editmode)
    {
        extern int outline;
        if(!wireframe && outline) renderoutline();
        GLERROR;
        rendereditmaterials();
        GLERROR;
        renderparticles();
        GLERROR;

        extern int hidehud;
        if(!hidehud)
        {
            glDepthMask(GL_FALSE);
            renderblendbrush();
            rendereditcursor();
            glDepthMask(GL_TRUE);
        }
    }

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    if(fogoverlay && fogmat != MAT_AIR) drawfogoverlay(fogmat, fogbelow, clamp(fogbelow, 0.0f, 1.0f), abovemat);

    GLuint outfbo = scalefbo ? scalefbo : ovr::lensfbo[viewidx];
    doaa(setuppostfx(vieww, viewh, outfbo), processhdr);
    renderpostfx(outfbo);
    if(scalefbo) doscale(ovr::lensfbo[viewidx]);
}

void gl_drawmainmenu()
{
    if(ovr::enabled)
    {
        glBindFramebuffer_(GL_FRAMEBUFFER, ovr::lensfbo[viewidx]);
        glViewport(0, 0, hudw, hudh);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    renderbackground(NULL, NULL, NULL, NULL, true);
}

VAR(hidestats, 0, 0, 1);
VAR(hidehud, 0, 0, 1);

VARP(crosshairsize, 0, 15, 50);
VARP(cursorsize, 0, 18, 30);

void drawcrosshair(int w, int h)
{
    lua::push_external("cursor_exists");
    lua_pushboolean(lua::L, true);
    lua_call(lua::L, 1, 1);
    bool windowhit = lua_toboolean(lua::L, -1); lua_pop(lua::L, 1);
    if(!windowhit && (hidehud || mainmenu)) return; //(hidehud || player->state==CS_SPECTATOR || player->state==CS_DEAD)) return;

    float r = 1, g = 1, b = 1, cx = 0.5f, cy = 0.5f, chsize;
    Texture *crosshair = NULL;
    if(windowhit)
    {
        static Texture *cursor = NULL;
        if(!cursor) cursor = textureload("media/interface/cursor", 3, true);
        crosshair = cursor;
        chsize = cursorsize*w/900.0f;
        lua::push_external("cursor_get_position");
        lua_call(lua::L, 0, 2);

        cx = lua_tonumber(lua::L, -2);
        cy = lua_tonumber(lua::L, -1);
        lua_pop(lua::L, 2);
    }
    else
    {
        string cr = "media/interface/hud/crosshair";
        if (lua::push_external("gui_get_crosshair")) {
            lua_call(lua::L, 0, 1);
            formatstring(cr, "media/interface/hud/%s", lua_tostring(lua::L, -1));
            lua_pop(lua::L, 1);
        }
        crosshair = textureload(cr);
        chsize = crosshairsize*w/900.0f;
    }
    if(crosshair->type&Texture::ALPHA) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    else glBlendFunc(GL_ONE, GL_ONE);
    gle::colorf(r, g, b);
    float x = cx*w - (windowhit ? 0 : chsize/2.0f);
    float y = cy*h - (windowhit ? 0 : chsize/2.0f);
    glBindTexture(GL_TEXTURE_2D, crosshair->id);

    hudquad(x, y, chsize, chsize);
}

VARP(wallclock, 0, 0, 1);
VARP(wallclock24, 0, 0, 1);
VARP(wallclocksecs, 0, 0, 1);

static time_t walltime = 0;

VARP(showfps, 0, 1, 1);
VARP(showfpsrange, 0, 0, 1);
VAR(statrate, 1, 200, 1000);

FVARP(conscale, 1e-3f, 0.33f, 1e3f);

CLUAICOMMAND(console_scale_get, float, (), return conscale;);

bool fullconsole_visible();

void gl_drawhud()
{
    int w = hudw, h = hudh;
    if(forceaspect) w = int(ceil(h*forceaspect));

    gettextres(w, h);

    hudmatrix.ortho(0, w, h, 0, -1, 1);
    resethudmatrix();
    hudshader->set();

    gle::colorf(1, 1, 1);

    debuglights();

    glEnable(GL_BLEND);

    debugparticles();

    hudshader->set();

    float conw = w/conscale, conh = h/conscale, abovehud = conh - FONTH;
    if(!hidehud && !mainmenu)
    {
        if(!hidestats)
        {
            pushhudmatrix();
            hudmatrix.scale(conscale, conscale, 1);
            flushhudmatrix();

            int roffset = 0;
            if(showfps)
            {
                static int lastfps = 0, prevfps[3] = { 0, 0, 0 }, curfps[3] = { 0, 0, 0 };
                if(totalmillis - lastfps >= statrate)
                {
                    memcpy(prevfps, curfps, sizeof(prevfps));
                    lastfps = totalmillis - (totalmillis%statrate);
                }
                int nextfps[3];
                getfps(nextfps[0], nextfps[1], nextfps[2]);
                loopi(3) if(prevfps[i]==curfps[i]) curfps[i] = nextfps[i];
                if(showfpsrange) draw_textf("fps %d+%d-%d", conw-7*FONTH, conh-FONTH*3/2, curfps[0], curfps[1], curfps[2]);
                else draw_textf("fps %d", conw-5*FONTH, conh-FONTH*3/2, curfps[0]);
                roffset += FONTH;
            }

            printtimers(conw, conh);

            if(wallclock)
            {
                if(!walltime) { walltime = time(NULL); walltime -= totalmillis/1000; if(!walltime) walltime++; }
                time_t walloffset = walltime + totalmillis/1000;
                struct tm *localvals = localtime(&walloffset);
                static string buf;
                if(localvals && strftime(buf, sizeof(buf), wallclocksecs ? (wallclock24 ? "%H:%M:%S" : "%I:%M:%S%p") : (wallclock24 ? "%H:%M" : "%I:%M%p"), localvals))
                {
                    // hack because not all platforms (windows) support %P lowercase option
                    // also strip leading 0 from 12 hour time
                    char *dst = buf;
                    const char *src = &buf[!wallclock24 && buf[0]=='0' ? 1 : 0];
                    while(*src) *dst++ = tolower(*src++);
                    *dst++ = '\0';
                    draw_text(buf, conw-5*FONTH, conh-FONTH*3/2-roffset);
                    roffset += FONTH;
                }
            }

            pophudmatrix();
        }

        if(!editmode)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            game::gameplayhud(w, h);
            abovehud = min(abovehud, conh*game::abovegameplayhud());
        }

        rendertexturepanel(w, h);
    }

    lua::push_external("gui_above_hud");
    lua_call(lua::L, 0, 1);
    abovehud = min(abovehud, float(conh*lua_tonumber(lua::L, -1)));
    lua_pop(lua::L, 1);

    pushhudmatrix();
    hudmatrix.scale(conscale, conscale, 1);
    flushhudmatrix();
    abovehud -= rendercommand(FONTH/2, abovehud - FONTH/2, conw-FONTH);
    if(!hidehud && !fullconsole_visible()) renderconsole(conw, conh, abovehud - FONTH/2);
    pophudmatrix();

    drawcrosshair(w, h);

    gle::disable();

    glDisable(GL_BLEND);

    if(frametimer)
    {
        glFinish();
        framemillis = getclockmillis() - totalmillis;
    }
}

int renderw = 0, renderh = 0, hudx = 0, hudy = 0, hudw = 0, hudh = 0;

CLUAICOMMAND(hud_get_w, int, (), return hudw;);
CLUAICOMMAND(hud_get_h, int, (), return hudh;);

void gl_setupframe(bool force)
{
    extern int scr_w, scr_h;
    renderw = min(scr_w, screenw);
    renderh = min(scr_h, screenh);
    hudw = screenw;
    hudh = screenh;
    ovr::setup();
    if(!force) return;
    setuplights();
}

void gl_drawframe()
{
    synctimers();
    xtravertsva = xtraverts = glde = gbatches = vtris = vverts = 0;
    flipqueries();
    aspect = forceaspect ? forceaspect : hudw/float(hudh);
    float fovx = curfov;
    if(ovr::enabled && ovr::fov) fovx *= ovr::fov/fov;
    fovy = 2*atan2(tan(fovx/2*RAD), aspect)/RAD;
    vieww = hudw;
    viewh = hudh;
    loopi(2)
    {
        if(mainmenu) gl_drawmainmenu();
        else if (ClientSystem::scenarioStarted()) gl_drawview();
        lua::push_external("gui_render"); lua_call(lua::L, 0, 0);
        gl_drawhud();
        if(!ovr::enabled) break;
        ovr::warp();
        ++viewidx;
        hudx += hudw;
    }
    viewidx = 0;
    hudx = 0;
}

void cleanupgl()
{
    clearminimap();
    cleanuptimers();
    gle::cleanup();
    ovr::cleanup();
}

/* OF: extra Lua APIs */

CLUAICOMMAND(gl_scissor_enable, void, (), glEnable(GL_SCISSOR_TEST););
CLUAICOMMAND(gl_scissor_disable, void, (), glDisable(GL_SCISSOR_TEST););

CLUACOMMAND(gl_scissor, void, (int, int, int, int), glScissor);

CLUAICOMMAND(gl_blend_enable, void, (), glEnable(GL_BLEND););
CLUAICOMMAND(gl_blend_disable, void, (), glDisable(GL_BLEND););

CLUACOMMAND(gl_blend_func, void, (uint, uint), glBlendFunc);

CLUAICOMMAND(gl_bind_texture, void, (int id), glBindTexture(GL_TEXTURE_2D,
    id););

CLUAICOMMAND(gl_texture_param, void, (uint pname, int param),
    glTexParameteri(GL_TEXTURE_2D, pname, param););

CLUACOMMAND(hudmatrix_push,  void, (), pushhudmatrix);
CLUAICOMMAND(hudmatrix_pop,   void, (), pophudmatrix(););
CLUAICOMMAND(hudmatrix_flush, void, (), flushhudmatrix(););
CLUACOMMAND(hudmatrix_reset, void, (), resethudmatrix);

CLUAICOMMAND(hudmatrix_translate, void, (float x, float y, float z),
    hudmatrix.translate(vec(x, y, z)););
CLUAICOMMAND(hudmatrix_scale, void, (float x, float y, float z),
    hudmatrix.scale(vec(x, y, z)););
CLUAICOMMAND(hudmatrix_ortho, void, (float l, float r, float b, float t,
    float zn, float zf), hudmatrix.ortho(l, r, b, t, zn, zf););
