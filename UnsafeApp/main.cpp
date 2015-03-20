

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
#define DO_CLAMP_INPUT 0

#define pi 3.14159265358979323846264338327

double scale = 1.0;

static double animpts[NUMPTS * 2];
static double deltas[NUMPTS * 2];
static int fill_gradient = 1;

GLuint texture;



int cairo_code_tape_get_width() { return 512; }
int cairo_code_tape_get_height() { return 512; }
void cairo_code_tape_render(cairo_t *cr) {
    cairo_surface_t *temp_surface;
    cairo_t *old_cr;
    cairo_pattern_t *pattern;
    cairo_matrix_t matrix;
    
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
    pattern = cairo_pattern_create_rgba(1,1,1,1);
    cairo_set_source(cr, pattern);
    cairo_pattern_destroy(pattern);
    
    if (scale >= 3.0)
    {
        scale = 1.0;
    }
    
    scale += 0.01;
    
    cairo_translate(cr, -scale * 50, -scale * 50);
    cairo_scale (cr, scale, scale);
    
    cairo_new_path(cr);
    cairo_move_to(cr, 276.128906, 335.082031);
    cairo_line_to(cr, 283.789063, 350.203125);
    cairo_line_to(cr, 283.789063, 360.039063);
    cairo_line_to(cr, 286.828125, 360.039063);
    cairo_line_to(cr, 286.828125, 350.203125);
    cairo_line_to(cr, 293.90625, 335.117188);
    cairo_line_to(cr, 290.9375, 335.117188);
    cairo_line_to(cr, 285.414063, 347.199219);
    cairo_line_to(cr, 279.304688, 335.082031);
    cairo_close_path(cr);
    cairo_move_to(cr, 255.574219, 360.074219);
    cairo_line_to(cr, 258.335938, 360.074219);
    cairo_line_to(cr, 260.753906, 352.757813);
    cairo_line_to(cr, 268.65625, 352.757813);
    cairo_line_to(cr, 271.417969, 360.039063);
    cairo_line_to(cr, 274.320313, 360.039063);
    cairo_line_to(cr, 264.6875, 334.667969);
    cairo_line_to(cr, 264.378906, 334.667969);
    cairo_close_path(cr);
    cairo_move_to(cr, 268.035156, 350.582031);
    cairo_line_to(cr, 261.304688, 350.582031);
    cairo_line_to(cr, 264.515625, 341.089844);
    cairo_close_path(cr);
    cairo_move_to(cr, 237.644531, 335.117188);
    cairo_line_to(cr, 237.644531, 360.074219);
    cairo_line_to(cr, 252.558594, 360.074219);
    cairo_line_to(cr, 252.558594, 357.660156);
    cairo_line_to(cr, 240.546875, 357.660156);
    cairo_line_to(cr, 240.546875, 336.464844);
    cairo_curve_to(cr, 240.546875, 336.289063, 240.566406, 336.152344, 240.605469, 336.054688);
    cairo_curve_to(cr, 240.640625, 335.953125, 240.691406, 335.851563, 240.75, 335.746094);
    cairo_curve_to(cr, 240.785156, 335.6875, 240.820313, 335.605469, 240.859375, 335.496094);
    cairo_curve_to(cr, 240.898438, 335.386719, 240.90625, 335.261719, 240.890625, 335.117188);
    cairo_close_path(cr);
    cairo_move_to(cr, 217.335938, 335.117188);
    cairo_line_to(cr, 217.335938, 360.039063);
    cairo_line_to(cr, 220.234375, 360.039063);
    cairo_line_to(cr, 220.234375, 349.027344);
    cairo_line_to(cr, 225.136719, 349.027344);
    cairo_curve_to(cr, 225.910156, 349.042969, 226.675781, 349.003906, 227.433594, 348.914063);
    cairo_curve_to(cr, 228.1875, 348.820313, 228.941406, 348.589844, 229.699219, 348.21875);
    cairo_curve_to(cr, 230.851563, 347.613281, 231.726563, 346.765625, 232.320313, 345.6875);
    cairo_curve_to(cr, 232.914063, 344.605469, 233.210938, 343.421875, 233.214844, 342.136719);
    cairo_curve_to(cr, 233.214844, 340.851563, 232.914063, 339.660156, 232.3125, 338.558594);
    cairo_curve_to(cr, 231.714844, 337.457031, 230.824219, 336.59375, 229.644531, 335.964844);
    cairo_curve_to(cr, 228.867188, 335.574219, 228.085938, 335.335938, 227.304688, 335.238281);
    cairo_curve_to(cr, 226.523438, 335.144531, 225.730469, 335.101563, 224.929688, 335.117188);
    cairo_close_path(cr);
    cairo_move_to(cr, 220.234375, 337.742188);
    cairo_line_to(cr, 224.929688, 337.742188);
    cairo_curve_to(cr, 225.449219, 337.730469, 225.960938, 337.757813, 226.46875, 337.816406);
    cairo_curve_to(cr, 226.976563, 337.878906, 227.480469, 338.03125, 227.988281, 338.277344);
    cairo_curve_to(cr, 228.765625, 338.679688, 229.355469, 339.242188, 229.75, 339.953125);
    cairo_curve_to(cr, 230.148438, 340.667969, 230.347656, 341.4375, 230.347656, 342.261719);
    cairo_curve_to(cr, 230.347656, 343.070313, 230.160156, 343.8125, 229.785156, 344.488281);
    cairo_curve_to(cr, 229.40625, 345.164063, 228.859375, 345.6875, 228.132813, 346.066406);
    cairo_curve_to(cr, 227.65625, 346.300781, 227.175781, 346.449219, 226.695313, 346.503906);
    cairo_curve_to(cr, 226.214844, 346.5625, 225.730469, 346.585938, 225.242188, 346.578125);
    cairo_line_to(cr, 220.199219, 346.578125);
    cairo_close_path(cr);
    cairo_move_to(cr, 311.691406, 214.988281);
    cairo_line_to(cr, 302.511719, 208.308594);
    cairo_line_to(cr, 296.800781, 218.121094);
    cairo_line_to(cr, 295.035156, 206.90625);
    cairo_line_to(cr, 284.0625, 209.808594);
    cairo_line_to(cr, 290.738281, 200.628906);
    cairo_line_to(cr, 280.929688, 194.921875);
    cairo_line_to(cr, 292.140625, 193.152344);
    cairo_line_to(cr, 289.242188, 182.179688);
    cairo_line_to(cr, 298.417969, 188.859375);
    cairo_line_to(cr, 304.128906, 179.046875);
    cairo_line_to(cr, 305.894531, 190.257813);
    cairo_line_to(cr, 316.871094, 187.359375);
    cairo_line_to(cr, 310.191406, 196.539063);
    cairo_line_to(cr, 320.003906, 202.246094);
    cairo_line_to(cr, 308.789063, 204.015625);
    cairo_close_path(cr);
    cairo_move_to(cr, 225.347656, 209.808594);
    cairo_line_to(cr, 213.609375, 207.703125);
    cairo_line_to(cr, 212.710938, 219.59375);
    cairo_line_to(cr, 205.902344, 209.804688);
    cairo_line_to(cr, 196.855469, 217.578125);
    cairo_line_to(cr, 198.960938, 205.839844);
    cairo_line_to(cr, 187.070313, 204.941406);
    cairo_line_to(cr, 196.859375, 198.128906);
    cairo_line_to(cr, 189.085938, 189.085938);
    cairo_line_to(cr, 200.824219, 191.191406);
    cairo_line_to(cr, 201.722656, 179.300781);
    cairo_line_to(cr, 208.535156, 189.089844);
    cairo_line_to(cr, 217.578125, 181.316406);
    cairo_line_to(cr, 215.472656, 193.054688);
    cairo_line_to(cr, 227.363281, 193.953125);
    cairo_line_to(cr, 217.574219, 200.761719);
    cairo_close_path(cr);
    cairo_move_to(cr, 170.757813, 349.054688);
    cairo_line_to(cr, 170.757813, 306.554688);
    cairo_line_to(cr, 339.757813, 306.554688);
    cairo_line_to(cr, 339.757813, 391.554688);
    cairo_line_to(cr, 170.757813, 391.554688);
    cairo_close_path(cr);
    cairo_move_to(cr, 57.757813, 231.554688);
    cairo_line_to(cr, 57.757813, 147.554688);
    cairo_line_to(cr, 99.757813, 147.554688);
    cairo_line_to(cr, 99.757813, 315.554688);
    cairo_line_to(cr, 57.757813, 315.554688);
    cairo_close_path(cr);
    cairo_move_to(cr, 410.757813, 231.554688);
    cairo_line_to(cr, 410.757813, 147.554688);
    cairo_line_to(cr, 451.757813, 147.554688);
    cairo_line_to(cr, 451.757813, 315.554688);
    cairo_line_to(cr, 410.757813, 315.554688);
    cairo_close_path(cr);
    cairo_move_to(cr, 137.632813, 273.871094);
    cairo_curve_to(cr, 131.515625, 270.074219, 131.757813, 273.121094, 131.757813, 200.054688);
    cairo_curve_to(cr, 131.757813, 140.113281, 131.925781, 133.082031, 133.394531, 130.59375);
    cairo_curve_to(cr, 136.703125, 124.992188, 138.671875, 124.554688, 160.613281, 124.554688);
    cairo_curve_to(cr, 176.070313, 124.554688, 180.84375, 124.855469, 181.460938, 125.863281);
    cairo_curve_to(cr, 181.898438, 126.585938, 183.371094, 127.9375, 184.730469, 128.863281);
    cairo_curve_to(cr, 186.992188, 130.410156, 192.980469, 130.554688, 254.757813, 130.554688);
    cairo_curve_to(cr, 316.535156, 130.554688, 322.523438, 130.410156, 324.785156, 128.863281);
    cairo_curve_to(cr, 326.144531, 127.9375, 327.617188, 126.585938, 328.058594, 125.863281);
    cairo_curve_to(cr, 328.675781, 124.851563, 333.535156, 124.554688, 349.402344, 124.554688);
    cairo_curve_to(cr, 371.890625, 124.554688, 373.800781, 124.972656, 377.125, 130.59375);
    cairo_curve_to(cr, 378.59375, 133.082031, 378.757813, 140.113281, 378.757813, 200.054688);
    cairo_curve_to(cr, 378.757813, 273.355469, 379.015625, 270.199219, 372.71875, 273.917969);
    cairo_curve_to(cr, 370.199219, 275.40625, 359.65625, 275.550781, 255.105469, 275.527344);
    cairo_curve_to(cr, 150.703125, 275.503906, 140.019531, 275.351563, 137.632813, 273.871094);
    cairo_close_path(cr);
    cairo_move_to(cr, 355.757813, 200.054688);
    cairo_line_to(cr, 355.757813, 147.554688);
    cairo_line_to(cr, 154.757813, 147.554688);
    cairo_line_to(cr, 154.757813, 252.554688);
    cairo_line_to(cr, 355.757813, 252.554688);
    cairo_close_path(cr);
    cairo_move_to(cr, 169.632813, 240.871094);
    cairo_curve_to(cr, 163.816406, 237.261719, 163.757813, 236.859375, 163.757813, 199.554688);
    cairo_curve_to(cr, 163.757813, 162.097656, 163.8125, 161.726563, 169.796875, 158.191406);
    cairo_curve_to(cr, 172.300781, 156.710938, 180.46875, 156.554688, 254.757813, 156.554688);
    cairo_curve_to(cr, 329.050781, 156.554688, 337.214844, 156.710938, 339.71875, 158.191406);
    cairo_curve_to(cr, 345.703125, 161.726563, 345.757813, 162.097656, 345.757813, 199.554688);
    cairo_curve_to(cr, 345.757813, 237.011719, 345.703125, 237.382813, 339.71875, 240.917969);
    cairo_curve_to(cr, 337.214844, 242.398438, 329.070313, 242.550781, 254.605469, 242.527344);
    cairo_curve_to(cr, 180.25, 242.503906, 172.003906, 242.34375, 169.632813, 240.871094);
    cairo_close_path(cr);
    cairo_move_to(cr, 322.757813, 199.554688);
    cairo_line_to(cr, 322.757813, 179.554688);
    cairo_line_to(cr, 186.757813, 179.554688);
    cairo_line_to(cr, 186.757813, 219.554688);
    cairo_line_to(cr, 322.757813, 219.554688);
    cairo_close_path(cr);
    cairo_move_to(cr, 201.757813, 103.054688);
    cairo_line_to(cr, 201.757813, 98.554688);
    cairo_line_to(cr, 307.757813, 98.554688);
    cairo_line_to(cr, 307.757813, 107.554688);
    cairo_line_to(cr, 201.757813, 107.554688);
    cairo_close_path(cr);
    cairo_move_to(cr, 201.757813, 103.054688);
    cairo_set_tolerance(cr, 0.1);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
    cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);
    cairo_fill_preserve(cr);
    /********************/
    
    cairosdl_destroy (cr, cairo_get_target (cr));
    
}




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
    
    double x, y, px, ux=1, uy=1, dashlength;
    char text[]="";
    cairo_font_extents_t fe;
    cairo_text_extents_t te;
    
    /* Prepare drawing area */
    //surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 240, 240);
    //cr = cairo_create (surface);
    /* Example is in 26.0 x 1.0 coordinate space */
    cairo_scale (cr, 240, 240);
    cairo_set_font_size (cr, 0.5);
    
    cairo_font_options_t *cfo;
    
    // alocate memory for font options
    cfo = cairo_font_options_create();
    
    /* Drawing code goes here */
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    cairo_select_font_face (cr, "Andale Mono",
                            CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_font_options_set_antialias(cfo, CAIRO_ANTIALIAS_GRAY);
    cairo_font_extents (cr, &fe);
    
    cairo_device_to_user_distance (cr, &ux, &uy);
    if (ux > uy)
        px = ux;
    else
        px = uy;
    cairo_font_extents (cr, &fe);
    cairo_text_extents (cr, text, &te);
    x = 0.5 - te.x_bearing - te.width / 4;
    y = 0.5 - fe.descent + fe.height / 2;
    
    /* text */
    cairo_move_to (cr, x, y);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_show_text (cr, text);
    
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
        //"    gl_FragColor = texture2D(tex0, v_texCoord) * v_color;\n"
        
        
        
         "vec4 blurSample = vec4(0,0,0,0);\n"
         "vec4 tmpPix;\n"
         "vec4 offPix;\n"
         "vec2 uv = v_texCoord.st;\n"
         "vec4 base = texture2D(tex0, v_texCoord);\n"
         
         "for(int i=-4;i<5;i++)\n"
         "{\n"
         
         "   float loopMod = float(i);\n"
         
         "    tmpPix = texture2D(tex0,uv + vec2( loopMod*0.005,-0.0005*loopMod ));\n"
         "    offPix = -0.05+tmpPix;\n"
         "    offPix = offPix * vec4(5,5,5,5);\n"
         "    int compare = int(offPix.r+offPix.g+offPix.b);\n"
         
         "    if( compare>0 )\n"
         "    {\n"
         "            blurSample = blurSample + offPix;\n"
         "    }\n"
         "}\n"
         
         "for(int i=-4;i<5;i++)\n"
         "{\n"
         "    float loopMod2 = float(i);\n"
         
         "    tmpPix = texture2D(tex0,uv + vec2( -0.0005*loopMod2,loopMod2*0.005 ));\n"
         "            offPix = 0.05+tmpPix;\n"
         "    offPix = offPix * vec4(5,5,5,5);\n"
         "    int compare2 = int(offPix.r+offPix.g+offPix.b);\n"
         
         "    if( compare2>0 )\n"
         "    {\n"
         "        blurSample += offPix;\n"
         "    }\n"
         
         "}\n"
         
         "vec4 result = base + blurSample - (2.0 * base * blurSample);\n"
         "result = clamp(result, 0.0, 1.0);\n"
         "gl_FragColor = mix(base,result,0.75);\n"
         
        
        /*
        "  vec2 onePixel = vec2(1.0 / 480.0, 1.0 / 320.0);\n"
        
        
        "  vec2 texCoord = v_texCoord;\n"
        
        
        "  vec4 color;\n"
        "  color.rgb = vec3(0.0);\n"
        "  color -= texture2D(tex0, texCoord - onePixel) * 5.0;\n"
        "  color += texture2D(tex0, texCoord + onePixel) * 5.0;\n"
        
        "  color.rgb = vec3((color.r + color.g + color.b) / 3.0);\n"
        "  gl_FragColor = mix(vec4(color.rgb, 1), texture2D(tex0, v_texCoord) * v_color, 0.45);\n"
        */
        
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

void SDL_GL_LoadTexture(SDL_Surface * surface, GLfloat * texcoord, GLuint texture)
{
    
    int w, h;
    
    /* Use the surface width and height expanded to powers of 2 */
    w = power_of_two(surface->w);
    h = power_of_two(surface->h);
    texcoord[0] = 0.0f;         /* Min X */
    texcoord[1] = 0.0f;         /* Min Y */
    texcoord[2] = (GLfloat) surface->w / w;     /* Max X */
    texcoord[3] = (GLfloat) surface->h / h;     /* Max Y */
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    
    
}

/* A general OpenGL initialization function.    Sets all of the initial parameters. */
GLuint InitGL(int Width, int Height)                    // We call this right after our OpenGL window is created.
{
    
    GLuint texture;
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
    glOrtho(-1.5, 1.5, -1.5 / aspect, 1.5 / aspect, 0.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    
    /* Create an OpenGL texture for the image */
    glGenTextures(1, &texture);
    
    return texture;
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
event_loop (unsigned flags, int width, int height)
{
    
    SDL_Event event;
    
    SDL_Window *window;
    
    SDL_Surface *screen;
    GLfloat texcoords[4];
    
    window = SDL_CreateWindow("Cairo Gears",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    
    // Create an OpenGL context associated with the window.
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    
    
    texture = InitGL(width, height);
    
    InitShaders();
    
    
    
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
        
        screen = SDL_CreateRGBSurface (
                                       SDL_SWSURFACE, width, height, 32,
                                       CAIROSDL_RMASK,
                                       CAIROSDL_GMASK,
                                       CAIROSDL_BMASK,
                                       CAIROSDL_AMASK); /* Amask */
        
        
        //cairo_code_tape_render(cairo_create(cairosdl_surface_create(screen)));
        trap_render(cairo_create(cairosdl_surface_create(screen)), width, height);
        
        SDL_GL_LoadTexture(screen, texcoords, texture);
        
        DrawGLScene(window, texture, texcoords);
        
        SDL_FreeSurface(screen);
        
    }
    
    
    QuitShaders();
    
    
    
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
    int flags = SDL_SWSURFACE;
    int init_flags = SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE;
    
    
    if (SDL_Init (init_flags) < 0) {
        fprintf (stderr, "Failed to initialise SDL: %s\n",
                 SDL_GetError ());
        exit (1);
    }
    atexit (SDL_Quit);
    
    event_loop (flags, width, height);
    return 0;
}



