

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "cairosdl.h"
#include <SDL_opengl.h>

#undef min

#define LINEWIDTH 3.0

#define FILL_R 0.1
#define FILL_G 0.1
#define FILL_B 0.75
#define FILL_OPACITY 0.5

#define STROKE_R 0.1
#define STROKE_G 0.75
#define STROKE_B 0.1
#define STROKE_OPACITY 1.0

#define WINDOW_WIDTH    512
#define WINDOW_HEIGHT   512

#define NUMPTS 6

#define CHEAT_SHADOWS 0         /* 1: use opaque gear shadows,
* 0: semitransparent shadows like qgears2 */

#define ASHIFT CAIROSDL_ASHIFT
#define RSHIFT ((24 + ASHIFT) % 32)
#define GSHIFT ((16 + ASHIFT) % 32)
#define BSHIFT (( 8 + ASHIFT) % 32)

#define AMASK (255U << ASHIFT)
#define RMASK (255U << RSHIFT)
#define GMASK (255U << GSHIFT)
#define BMASK (255U << BSHIFT)

/* Set to 1 if the input can have superluminant pixels.  Cairo doesn't
 * produce them. */
#define DO_CLAMP_INPUT 1

#define pi 3.14159265358979323846264338327


static double animpts[NUMPTS * 2];
static double deltas[NUMPTS * 2];
static int fill_gradient = 1;

GLuint texture;

static void
gear (cairo_t *cr,
      double inner_radius,
      double outer_radius,
      int teeth,
      double tooth_depth)
{
    int i;
    double r0, r1, r2;
    double angle, da;
    
    r0 = inner_radius;
    r1 = outer_radius - tooth_depth / 2.0;
    r2 = outer_radius + tooth_depth / 2.0;
    
    da = 2.0 * pi / (double) teeth / 4.0;
    
    cairo_new_path (cr);
    
    angle = 0.0;
    cairo_move_to (cr, r1 * cos (angle + 3 * da), r1 * sin (angle + 3 * da));
    
    for (i = 1; i <= teeth; i++) {
        angle = i * 2.0 * pi / (double) teeth;
        
        cairo_line_to (cr, r1 * cos (angle), r1 * sin (angle));
        cairo_line_to (cr, r2 * cos (angle + da), r2 * sin (angle + da));
        cairo_line_to (cr, r2 * cos (angle + 2 * da), r2 * sin (angle + 2 * da));
        
        if (i < teeth)
            cairo_line_to (cr, r1 * cos (angle + 3 * da),
                           r1 * sin (angle + 3 * da));
    }
    
    cairo_close_path (cr);
    
    cairo_move_to (cr, r0 * cos (angle + 3 * da), r0 * sin (angle + 3 * da));
    
    for (i = 1; i <= teeth; i++) {
        angle = i * 2.0 * pi / (double) teeth;
        
        cairo_line_to (cr, r0 * cos (angle), r0 * sin (angle));
    }
    
    cairo_close_path (cr);
}

void
trap_setup (cairo_surface_t *target, int w, int h)
{
    int i;
    
    (void)target;
    
    
    for (i = 0; i < (NUMPTS * 2); i += 2) {
        animpts[i + 0] = (float) (((double)(rand()) / RAND_MAX) * w);
        animpts[i + 1] = (float) (((double)(rand()) / RAND_MAX) * h);
        deltas[i + 0] = (float) (((double)(rand()) / RAND_MAX) * 6.0 + 4.0);
        deltas[i + 1] = (float) (((double)(rand()) / RAND_MAX) * 6.0 + 4.0);
        if (animpts[i + 0] > w / 2.0) {
            deltas[i + 0] = -deltas[i + 0];
        }
        if (animpts[i + 1] > h / 2.0) {
            deltas[i + 1] = -deltas[i + 1];
        }
    }
}

static void
stroke_and_fill_animate (double *pts,
                         double *deltas,
                         int index,
                         int limit)
{
    double newpt = pts[index] + deltas[index];
    
    if (newpt <= 0) {
        newpt = -newpt;
        deltas[index] = (double) (((double)(rand()) / RAND_MAX) * 4.0 + 2.0);
    } else if (newpt >= (double) limit) {
        newpt = 2.0 * limit - newpt;
        deltas[index] = - (double) (((double)(rand()) / RAND_MAX) * 4.0 + 2.0);
    }
    pts[index] = newpt;
}

static void
stroke_and_fill_step (int w, int h)
{
    int i;
    
    for (i = 0; i < (NUMPTS * 2); i += 2) {
        stroke_and_fill_animate (animpts, deltas, i + 0, w);
        stroke_and_fill_animate (animpts, deltas, i + 1, h);
    }
}

static double gear1_rotation = 0.35;
static double gear2_rotation = 0.33;
static double gear3_rotation = 0.50;

void
trap_render (cairo_t *cr, int w, int h)
{
    double *ctrlpts = animpts;
    int len = (NUMPTS * 2);
    double prevx = ctrlpts[len - 2];
    double prevy = ctrlpts[len - 1];
    double curx = ctrlpts[0];
    double cury = ctrlpts[1];
    double midx = (curx + prevx) / 2.0;
    double midy = (cury + prevy) / 2.0;
    int i;
    int pass;
    
    cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
    
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.0);
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_rectangle (cr, 0, 0, w, h);
    cairo_fill (cr);
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    cairo_set_source_rgba (cr, 0.75, 0.75, 0.75, 1.0);
    cairo_set_line_width (cr, 1.0);
    
    cairo_save (cr); {
        cairo_scale (cr, (double) w / 512.0, (double) h / 512.0);
        
        cairo_save (cr); {
            cairo_translate (cr, -10.0, -10.0);
            cairo_translate (cr, 170.0, 330.0);
            cairo_rotate (cr, gear1_rotation);
            gear (cr, 30.0, 120.0, 20, 20.0);
            cairo_set_source_rgba (cr, 0.70, 0.70, 0.70, 0.70 + CHEAT_SHADOWS);
            cairo_fill (cr);
            cairo_restore (cr);
        }
        cairo_save (cr); {
            cairo_translate (cr, -10.0, -10.0);
            cairo_translate (cr, 369.0, 330.0);
            cairo_rotate (cr, gear2_rotation);
            gear (cr, 15.0, 75.0, 12, 20.0);
            cairo_set_source_rgba (cr, 0.70, 0.70, 0.70, 0.70 + CHEAT_SHADOWS);
            cairo_fill (cr);
            cairo_restore (cr);
        }
        cairo_save (cr); {
            cairo_translate (cr, -10.0, -10.0);
            cairo_translate (cr, 170.0, 116.0);
            cairo_rotate (cr, gear3_rotation);
            gear (cr, 20.0, 90.0, 14, 20.0);
            cairo_set_source_rgba (cr, 0.70, 0.70, 0.70, 0.70 + CHEAT_SHADOWS);
            cairo_fill (cr);
            cairo_restore (cr);
        }
        
        cairo_save (cr); {
            cairo_translate (cr, 170.0, 330.0);
            cairo_rotate (cr, gear1_rotation);
            gear (cr, 30.0, 120.0, 20, 20.0);
            cairo_set_source_rgb (cr, 0.75, 0.75, 0.75);
            cairo_fill_preserve (cr);
            cairo_set_source_rgb (cr, 0.25, 0.25, 0.25);
            cairo_stroke (cr);
            cairo_restore (cr);
        }
        cairo_save (cr); {
            cairo_translate (cr, 369.0, 330.0);
            cairo_rotate (cr, gear2_rotation);
            gear (cr, 15.0, 75.0, 12, 20.0);
            cairo_set_source_rgb (cr, 0.75, 0.75, 0.75);
            cairo_fill_preserve (cr);
            cairo_set_source_rgb (cr, 0.25, 0.25, 0.25);
            cairo_stroke (cr);
            cairo_restore (cr);
        }
        cairo_save (cr); {
            cairo_translate (cr, 170.0, 116.0);
            cairo_rotate (cr, gear3_rotation);
            gear (cr, 20.0, 90.0, 14, 20.0);
            cairo_set_source_rgb (cr, 0.75, 0.75, 0.75);
            cairo_fill_preserve (cr);
            cairo_set_source_rgb (cr, 0.25, 0.25, 0.25);
            cairo_stroke (cr);
            cairo_restore (cr);
        }
        
        cairo_restore (cr);
    }
    
    gear1_rotation += 0.01;
    gear2_rotation -= (0.01 * (20.0 / 12.0));
    gear3_rotation -= (0.01 * (20.0 / 14.0));
    
    stroke_and_fill_step (w, h);
    
    cairo_translate (cr, -10, -10);
    for (pass = 1; pass <= 2; pass++) {
        cairo_new_path (cr);
        cairo_move_to (cr, midx, midy);
        
        for (i = 2; i <= (NUMPTS * 2); i += 2) {
            double x2, x1 = (midx + curx) / 2.0;
            double y2, y1 = (midy + cury) / 2.0;
            
            prevx = curx;
            prevy = cury;
            if (i < (NUMPTS * 2)) {
                curx = ctrlpts[i + 0];
                cury = ctrlpts[i + 1];
            } else {
                curx = ctrlpts[0];
                cury = ctrlpts[1];
            }
            midx = (curx + prevx) / 2.0;
            midy = (cury + prevy) / 2.0;
            x2 = (prevx + midx) / 2.0;
            y2 = (prevy + midy) / 2.0;
            cairo_curve_to (cr, x1, y1, x2, y2, midx, midy);
        }
        cairo_close_path (cr);
        
        if (pass == 1) {
            cairo_set_source_rgba (cr, 0,0,0,77/255.0);
            cairo_fill (cr);
            cairo_translate (cr, 10, 10);
        }
    }
    
    if (fill_gradient) {
        double x1, y1, x2, y2;
        cairo_pattern_t *pattern;
        
        cairo_fill_extents (cr, &x1, &y1, &x2, &y2);
        
        pattern = cairo_pattern_create_linear (x1, y1, x2, y2);
        cairo_pattern_add_color_stop_rgba (pattern, 0.0, 0.0, 0.0, 1.0, 0.75);
        cairo_pattern_add_color_stop_rgba (pattern, 1.0, 1.0, 0.0, 0.0, 1.0);
        cairo_pattern_set_filter (pattern, CAIRO_FILTER_BILINEAR);
        
        cairo_move_to (cr, 0, 0);
        cairo_set_source (cr, pattern);
        cairo_pattern_destroy (pattern);
    } else {
        cairo_set_source_rgba (cr, FILL_R, FILL_G, FILL_B, FILL_OPACITY);
    }
    
    cairo_fill_preserve (cr);
    cairo_set_source_rgba (cr, STROKE_R, STROKE_G, STROKE_B, STROKE_OPACITY);
    cairo_set_line_width (cr, LINEWIDTH);
    cairo_stroke (cr);
    
    double x, y, ux=1, uy=1;
    char text[]="Composite";

    cairo_font_extents_t fe;
    cairo_text_extents_t te;
    
    /* Prepare drawing area */
    
    /* Example is in 26.0 x 1.0 coordinate space */
    cairo_scale (cr, 240, 240);
    cairo_set_font_size (cr, 0.25);
    
    cairo_font_options_t *cfo;
    
    // alocate memory for font options
    cfo = cairo_font_options_create();
    
    /* Drawing code goes here */
    cairo_set_source_rgb (cr, 0.1, 0.1, 0.1);
    cairo_select_font_face (cr, "Purisa",
                            CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_font_options_set_antialias(cfo, CAIRO_ANTIALIAS_GRAY);
    cairo_font_extents (cr, &fe);
    
    cairo_device_to_user_distance (cr, &ux, &uy);
    
    cairo_font_extents (cr, &fe);
    cairo_text_extents (cr, text, &te);
    x = 0.5 - te.x_bearing - te.width / 4;
    y = 0.5 - fe.descent + fe.height / 2;
    
    /* text */
    cairo_move_to (cr, x, y);
    cairo_set_source_rgb (cr, 0, 1, 0);
    cairo_show_text (cr, text);
    cairo_move_to (cr, x, y+0.3);
    cairo_show_text (cr, "Vectors");
    
    // clean up the font option
    cairo_font_options_destroy(cfo);
    
    cairosdl_destroy (cr, cairo_get_target (cr));
}


static cairo_user_data_key_t const CAIROSDL_TARGET_KEY[1] = {{1}};


static void
sdl_surface_destroy_func (void *param)
{
    SDL_Surface *sdl_surface = (SDL_Surface *)param;
    if (sdl_surface != NULL)
        SDL_FreeSurface (sdl_surface);
}


cairo_surface_t *
cairosdl_surface_create (
                         SDL_Surface *sdl_surface)
{
    cairo_surface_t *target;
    cairo_format_t format;
    
    
    /* Cairo only supports a limited number of pixels formats.  Make
     * sure the surface format is compatible. */
    if (sdl_surface->format->BytesPerPixel != 4 ||
        sdl_surface->format->BitsPerPixel != 32)
        goto unsupported_format;
    
    if (sdl_surface->format->Rmask != CAIROSDL_RMASK ||
        sdl_surface->format->Gmask != CAIROSDL_GMASK ||
        sdl_surface->format->Bmask != CAIROSDL_BMASK)
        goto unsupported_format;
    
    switch (sdl_surface->format->Amask) {
        case CAIROSDL_AMASK:
            format = CAIRO_FORMAT_RGB24;
            break;
        case 0:
            format = CAIRO_FORMAT_RGB24;
            break;
        default:
            goto unsupported_format;
    }
    
    
    
    /* Make the target point to either the SDL_Surface's data itself
     * or a shadow image surface if we need to unpremultiply pixels. */
    if (format == CAIRO_FORMAT_RGB24) {
        /* The caller is expected to have locked the surface (_if_ it
         * needs locking) so that sdl_surface->pixels is valid and
         * constant for the lifetime of the cairo_surface_t.  However,
         * we're told not to call any OS functions when a surface is
         * locked, so we really shouldn't call
         * cairo_image_surface_create () as it will malloc, so really
         * if the surface needs locking this shouldn't be used.
         *
         * However, it turns out malloc is actually safe on many (all?)
         * platforms so we'll just go ahead anyway. */
        
        
        unsigned char *data = (unsigned char*)(sdl_surface->pixels);
        
        target = cairo_image_surface_create_for_data (data,
                                                      format,
                                                      sdl_surface->w,
                                                      sdl_surface->h,
                                                      sdl_surface->pitch);
        
        
        
    }
    else {
        /* Need a shadow image surface. */
        
        target = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                             sdl_surface->w,
                                             sdl_surface->h);
        
    }
    
    if (cairo_surface_status (target) == CAIRO_STATUS_SUCCESS) {
        sdl_surface->refcount++;
        
        cairo_surface_set_user_data (target,
                                     CAIROSDL_TARGET_KEY,
                                     sdl_surface,
                                     sdl_surface_destroy_func);
        
        
        
        
    }
    
    return target;
    
unsupported_format:
    /* Nasty kludge to get a cairo surface in CAIRO_INVALID_FORMAT
     * state. */
    return cairo_image_surface_create (
                                       (cairo_format_t)-1, 0, 0);
}

void
cairosdl_destroy (cairo_t *cr, cairo_surface_t *surface)
{
    
    cairo_surface_destroy(surface);
    cairo_destroy (cr);
    
}


/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
    exit(rc);
}

#ifdef _WIN32
#undef main
#endif

static SDL_bool shaders_supported;
static int      current_shader = 0;

enum {
    SHADER_COLOR,
    SHADER_TEXTURE,
    SHADER_TEXCOORDS,
    NUM_SHADERS
};

typedef struct {
    GLhandleARB program;
    GLhandleARB vert_shader;
    GLhandleARB frag_shader;
    const char *vert_source;
    const char *frag_source;
} ShaderData;

static ShaderData shaders[NUM_SHADERS] = {
    
    /* SHADER_COLOR */
    { 0, 0, 0,
        /* vertex shader */
        "varying vec4 v_color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
        "    v_color = gl_Color;\n"
        "}",
        /* fragment shader */
        "varying vec4 v_color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = v_color;\n"
        "}"
    },
    
    /* SHADER_TEXTURE */
    { 0, 0, 0,
        /* vertex shader */
        "varying vec4 v_color;\n"
        "varying vec2 v_texCoord;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
        "    v_color = gl_Color;\n"
        "    v_texCoord = vec2(gl_MultiTexCoord0);\n"
        "}",
        /* fragment shader */
        "varying vec4 v_color;\n"
        "varying vec2 v_texCoord;\n"
        "uniform sampler2D tex0;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = texture2D(tex0, v_texCoord) * v_color;\n"
        
        /*
        "  vec2 onePixel = vec2(1.0 / 480.0, 1.0 / 320.0);\n"
        
        
        "  vec2 texCoord = v_texCoord;\n"
        
        
        "  vec4 color;\n"
        "  color.rgb = vec3(0.0);\n"
        "  color -= texture2D(tex0, texCoord - onePixel) * 5.0;\n"
        "  color += texture2D(tex0, texCoord + onePixel) * 5.0;\n"
        
        "  color.rgb = vec3((color.r + color.g + color.b) / 3.0);\n"
        "  gl_FragColor = mix(vec4(color.rgb, 1), texture2D(tex0, v_texCoord) * v_color, 0.45);\n"*/
        
        "}"
    },
    
    /* SHADER_TEXCOORDS */
    { 0, 0, 0,
        /* vertex shader */
        "varying vec2 v_texCoord;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
        "    v_texCoord = vec2(gl_MultiTexCoord0);\n"
        "}",
        /* fragment shader */
        "varying vec2 v_texCoord;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    vec4 color;\n"
        "    vec2 delta;\n"
        "    float dist;\n"
        "\n"
        "    delta = vec2(0.5, 0.5) - v_texCoord;\n"
        "    dist = dot(delta, delta);\n"
        "\n"
        "    color.r = v_texCoord.x;\n"
        "    color.g = v_texCoord.x * v_texCoord.y;\n"
        "    color.b = v_texCoord.y;\n"
        "    color.a = 1.0 - (dist * 4.0);\n"
        "    gl_FragColor = color;\n"
        "}"
    },
};

static PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
static PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
static PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
static PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
static PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
static PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
static PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
static PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
static PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
static PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
static PFNGLUNIFORM1IARBPROC glUniform1iARB;
static PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;

static SDL_bool CompileShader(GLhandleARB shader, const char *source)
{
    GLint status;
    
    glShaderSourceARB(shader, 1, &source, NULL);
    glCompileShaderARB(shader);
    glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &status);
    if (status == 0) {
        GLint length;
        char *info;
        
        glGetObjectParameterivARB(shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
        info = SDL_stack_alloc(char, length+1);
        glGetInfoLogARB(shader, length, NULL, info);
        fprintf(stderr, "Failed to compile shader:\n%s\n%s", source, info);
        SDL_stack_free(info);
        
        return SDL_FALSE;
    } else {
        return SDL_TRUE;
    }
}

static SDL_bool CompileShaderProgram(ShaderData *data)
{
    const int num_tmus_bound = 4;
    int i;
    GLint location;
    
    glGetError();
    
    /* Create one program object to rule them all */
    data->program = glCreateProgramObjectARB();
    
    /* Create the vertex shader */
    data->vert_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    if (!CompileShader(data->vert_shader, data->vert_source)) {
        return SDL_FALSE;
    }
    
    /* Create the fragment shader */
    data->frag_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    if (!CompileShader(data->frag_shader, data->frag_source)) {
        return SDL_FALSE;
    }
    
    /* ... and in the darkness bind them */
    glAttachObjectARB(data->program, data->vert_shader);
    glAttachObjectARB(data->program, data->frag_shader);
    glLinkProgramARB(data->program);
    
    /* Set up some uniform variables */
    glUseProgramObjectARB(data->program);
    for (i = 0; i < num_tmus_bound; ++i) {
        char tex_name[5];
        SDL_snprintf(tex_name, SDL_arraysize(tex_name), "tex%d", i);
        location = glGetUniformLocationARB(data->program, tex_name);
        if (location >= 0) {
            glUniform1iARB(location, i);
        }
    }
    glUseProgramObjectARB(0);
    
    return (SDL_bool)(glGetError() == GL_NO_ERROR);
}

static void DestroyShaderProgram(ShaderData *data)
{
    if (shaders_supported) {
        glDeleteObjectARB(data->vert_shader);
        glDeleteObjectARB(data->frag_shader);
        glDeleteObjectARB(data->program);
    }
}

static SDL_bool InitShaders()
{
    int i;
    
    /* Check for shader support */
    shaders_supported = SDL_FALSE;
    if (SDL_GL_ExtensionSupported("GL_ARB_shader_objects") &&
        SDL_GL_ExtensionSupported("GL_ARB_shading_language_100") &&
        SDL_GL_ExtensionSupported("GL_ARB_vertex_shader") &&
        SDL_GL_ExtensionSupported("GL_ARB_fragment_shader")) {
        glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) SDL_GL_GetProcAddress("glAttachObjectARB");
        glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) SDL_GL_GetProcAddress("glCompileShaderARB");
        glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) SDL_GL_GetProcAddress("glCreateProgramObjectARB");
        glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC) SDL_GL_GetProcAddress("glCreateShaderObjectARB");
        glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC) SDL_GL_GetProcAddress("glDeleteObjectARB");
        glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) SDL_GL_GetProcAddress("glGetInfoLogARB");
        glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) SDL_GL_GetProcAddress("glGetObjectParameterivARB");
        glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) SDL_GL_GetProcAddress("glGetUniformLocationARB");
        glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) SDL_GL_GetProcAddress("glLinkProgramARB");
        glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) SDL_GL_GetProcAddress("glShaderSourceARB");
        glUniform1iARB = (PFNGLUNIFORM1IARBPROC) SDL_GL_GetProcAddress("glUniform1iARB");
        glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) SDL_GL_GetProcAddress("glUseProgramObjectARB");
        if (glAttachObjectARB &&
            glCompileShaderARB &&
            glCreateProgramObjectARB &&
            glCreateShaderObjectARB &&
            glDeleteObjectARB &&
            glGetInfoLogARB &&
            glGetObjectParameterivARB &&
            glGetUniformLocationARB &&
            glLinkProgramARB &&
            glShaderSourceARB &&
            glUniform1iARB &&
            glUseProgramObjectARB) {
            shaders_supported = SDL_TRUE;
        }
    }
    
    if (!shaders_supported) {
        return SDL_FALSE;
    }
    
    /* Compile all the shaders */
    for (i = 0; i < NUM_SHADERS; ++i) {
        if (!CompileShaderProgram(&shaders[i])) {
            fprintf(stderr, "Unable to compile shader!\n");
            return SDL_FALSE;
        }
    }
    
    /* We're done! */
    return SDL_TRUE;
}

static void QuitShaders()
{
    int i;
    
    for (i = 0; i < NUM_SHADERS; ++i) {
        DestroyShaderProgram(&shaders[i]);
    }
}

/* Quick utility function for texture creation */
static int
power_of_two(int input)
{
    int value = 1;
    
    while (value < input) {
        value <<= 1;
    }
    return value;
}

GLuint
SDL_GL_LoadTexture(SDL_Surface * surface, GLfloat * texcoord)
{
    GLuint texture;
    int w, h;
    
    /* Use the surface width and height expanded to powers of 2 */
    w = power_of_two(surface->w);
    h = power_of_two(surface->h);
    texcoord[0] = 0.0f;         /* Min X */
    texcoord[1] = 0.0f;         /* Min Y */
    texcoord[2] = (GLfloat) surface->w / w;     /* Max X */
    texcoord[3] = (GLfloat) surface->h / h;     /* Max Y */
    
    /* Create an OpenGL texture for the image */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    
    
    return texture;
}

/* A general OpenGL initialization function.    Sets all of the initial parameters. */
void InitGL(int Width, int Height)                    // We call this right after our OpenGL window is created.
{
    GLdouble aspect;
    
    glViewport(0, 0, Width, Height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);        // This Will Clear The Background Color To Black
    glClearDepth(1.0);                // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);                // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);            // Enables Depth Testing
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);            // Enables Smooth Color Shading
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                // Reset The Projection Matrix
    
    aspect = (GLdouble)Width / Height;
    glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene(SDL_Window *window, GLuint texture, GLfloat * texcoord)
{
    /* Texture coordinate lookup, to make it simple */
    enum {
        MINX,
        MINY,
        MAXX,
        MAXY
    };
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // Clear The Screen And The Depth Buffer
    glLoadIdentity();                // Reset The View
    
    // draw a textured square (quadrilateral)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f,1.0f,1.0f);
    if (shaders_supported) {
        glUseProgramObjectARB(shaders[current_shader].program);
    }
    
    glBegin(GL_QUADS);                // start drawing a polygon (4 sided)
    glTexCoord2f(texcoord[MINX], texcoord[MINY]);
    glVertex3f(-1.0f, 1.0f, 0.0f);        // Top Left
    glTexCoord2f(texcoord[MAXX], texcoord[MINY]);
    glVertex3f( 1.0f, 1.0f, 0.0f);        // Top Right
    glTexCoord2f(texcoord[MAXX], texcoord[MAXY]);
    glVertex3f( 1.0f,-1.0f, 0.0f);        // Bottom Right
    glTexCoord2f(texcoord[MINX], texcoord[MAXY]);
    glVertex3f(-1.0f,-1.0f, 0.0f);        // Bottom Left
    glEnd();                    // done with the polygon
    
    if (shaders_supported) {
        glUseProgramObjectARB(0);
    }
    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1,&texture);
    
    
    
    // swap buffers to display, since we're double buffered.
    SDL_GL_SwapWindow(window);
    
    
}

static void
event_loop (int width, int height)
{
    
    SDL_Event event;
    
    SDL_Window *window;
    
    SDL_Surface *screen;
    GLfloat texcoords[4];
    
    
    window = SDL_CreateWindow("Unsafe System",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
        
    
    
    // Create an OpenGL context associated with the window.
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    
    
    
    /* Loop, drawing and checking events */
    InitGL(width, height);
    
    InitShaders();
    
    screen = SDL_CreateRGBSurface (
                                   SDL_SWSURFACE, width, height, 32,
                                   CAIROSDL_RMASK,
                                   CAIROSDL_GMASK,
                                   CAIROSDL_BMASK,
                                   CAIROSDL_AMASK); /* Amask */
    
    /* Main render loop */
    int done = 0;
    while(!done){
        
        while (SDL_PollEvent (&event)) {
            
            if (event.type == SDL_QUIT ) {
                done = 1;
            }
            
            if ( event.type == SDL_KEYDOWN ) {
                if ( event.key.keysym.sym == SDLK_SPACE ) {
                    current_shader = (current_shader + 1) % NUM_SHADERS;
                }
                if ( event.key.keysym.sym == SDLK_ESCAPE ) {
                    done = 1;
                }
            }
            
        }
        
        if(current_shader == 1)
        {
            trap_render (cairo_create(cairosdl_surface_create(screen)), width, height);
        
            texture = SDL_GL_LoadTexture(screen, texcoords);
        }
        
        DrawGLScene(window, texture, texcoords);
        
        
        
    }
    
    
    QuitShaders();
    
    SDL_FreeSurface(screen);
    
    // Once finished with OpenGL functions, the SDL_GLContext can be deleted.
    SDL_GL_DeleteContext(glcontext);
    
    //SDL_DestroyTexture(sprite);
    //SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    
    quit(0);
}


int
main (int argc, char *argv[])
{
    
    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    
    int init_flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE;
    
    
    if (SDL_Init (init_flags) < 0) {
        fprintf (stderr, "Failed to initialise SDL: %s\n",
                 SDL_GetError ());
        exit (1);
    }
    atexit (SDL_Quit);
    
    event_loop (width, height);
    return 0;
}



