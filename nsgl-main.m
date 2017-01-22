#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGL/gl.h>
#import <cairo.h>
#import <cairo-gl.h>
#import <glib.h>
#import <librsvg/rsvg.h>

#define WIDTH 800
#define HEIGHT 600

#define LINEWIDTH 1.5

#define FILL_R 0.1
#define FILL_G 0.1
#define FILL_B 0.75
#define FILL_OPACITY 0.5

#define STROKE_R 0.1
#define STROKE_G 0.75
#define STROKE_B 0.1
#define STROKE_OPACITY 1.0

#define NUMPTS 6

#define CHEAT_SHADOWS 0         /* 1: use opaque gear shadows,
* 0: semitransparent shadows like qgears2 */

/* Set to 1 if the input can have superluminant pixels.  Cairo doesn't
 * produce them. */
#define DO_CLAMP_INPUT 0

#define pi 3.14159265358979323846264338327

double scale = 1.0;

static double animpts[NUMPTS * 2];
static double deltas[NUMPTS * 2];
static int fill_gradient = 1;

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
    
    cairo_save(cr); {
        
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
            cairo_pattern_set_filter (pattern, CAIRO_FILTER_FAST);
            
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
        
        cairo_restore(cr);
        
    }
    
}

void rand_drawing(cairo_t *cr, cairo_surface_t *surface)
{
    
    int i;
    double r, g, b, a;
    
    // Clear background as white
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    cairo_paint(cr);
    
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
    for (i = 0; i < 100; i++)
    {
        int shape = drand48() * 3;
        float width = drand48() * 50 + 1;
        
        int line_cap = drand48() * 3;
        cairo_line_cap_t line_cap_style = CAIRO_LINE_CAP_BUTT;
        if (line_cap == 1)
            line_cap_style = CAIRO_LINE_CAP_ROUND;
        else if (line_cap == 2)
            line_cap_style = CAIRO_LINE_CAP_SQUARE;
        
        int line_join = drand48() * 3;
        cairo_line_join_t line_join_style = CAIRO_LINE_JOIN_MITER;
        if (line_join == 1)
            line_join_style = CAIRO_LINE_JOIN_ROUND;
        else if (line_join == 2)
            line_join_style = CAIRO_LINE_JOIN_BEVEL;
        
        double dash[] = {0.0, 0.0};
        dash[0] = drand48() * 50;
        dash[1] = drand48() * 50;
        
        cairo_set_dash(cr, dash, 2, 0);
        cairo_set_line_width(cr, width);
        cairo_set_line_join(cr, line_join_style);
        cairo_set_line_cap(cr, line_cap_style);
        
        // Random color
        r = drand48();
        g = drand48();
        b = drand48();
        a = drand48();
        cairo_set_source_rgba(cr, r, g, b, a);
        
        // Random position
        float x = drand48() * WIDTH;
        float y = drand48() * HEIGHT;
        float side = drand48() * 300;
        
        if (shape == 0)
        {
            // Draw a square
            cairo_rectangle(cr, x, y, side, side);
            cairo_fill(cr);
        }
        else if (shape == 1)
        {
            // Draw a circle
            cairo_arc(cr, x, y, side/2, 0.0, 2.0 * M_PI);
            cairo_stroke(cr);
        }
        else
        {
            // Draw a triangle
            cairo_move_to(cr, x, y);
            cairo_line_to(cr, x + side, y);
            cairo_line_to(cr, x, y + side);
            cairo_close_path(cr);
            cairo_stroke(cr);
        }
    }
    cairo_surface_flush(surface);
}

static double spritewidth[6];
static double spriteheight[6];
static double spritex[6];
static double spritey[6];
static double spritexvelocity[6];
static double spriteyvelocity[6];

void initRects() {
    
    int i;
    
    for( i = 0; i < 6; i++)
    {
        spritewidth[i] = 190;
        spriteheight[i] = 240;
        spritex[i] = drand48() * (WIDTH - 190);
        spritey[i] = drand48() * (HEIGHT - 240);
        spritexvelocity[i] = 0.0;
        spriteyvelocity[i] = 0.0;
        
    }
    
}

void simulStep(double spritex[], double spritey[], double spritewidth[], double spriteheight[], double spriteyvelocity[], double spritexvelocity[], double deltatime)
{
    
    double coeff_of_restitution = 0.55;
    double speed_of_grav = 150.0;
    double jumble_delay = 15 * 1000;
    double max_velocity = 23;
    
    int i;
    
    for(i = 0; i < 6; i++)
    {
        
        spritexvelocity[i] += (max_velocity / 2.0 ) - (drand48() * max_velocity);
        spriteyvelocity[i] += (max_velocity / 2.0 ) - (drand48() * max_velocity);
        
        //Move sprites
        spritex[i] = spritex[i] + (spritexvelocity[i] * deltatime);
        spritey[i] = spritey[i] + (spriteyvelocity[i] * deltatime);
        
        //gravity calculation
        spriteyvelocity[i] += speed_of_grav * deltatime;
        
        if( ( spritex[i] < 0 && spritexvelocity[i] < 0 ) || ( spritex[i] > ( (WIDTH + 10) - spriteheight[i] ) && spritexvelocity[i] > 0 ) )
        {
            spritexvelocity[i] = -spritexvelocity[i] * coeff_of_restitution;
            spritex[i] = MAX(0, MIN(spritex[i], ( (WIDTH + 10) - spritewidth[i] )));
            
            if (ABS(spritexvelocity[i]) < 0.01)
                spritexvelocity[i] = 0.0;
        }
        
        
        if( ( spritey[i] < 0 && spriteyvelocity[i] < 0 ) || ( spritey[i] > ( (HEIGHT) - spriteheight[i] ) && spriteyvelocity[i] > 0 ) )
        {
            spriteyvelocity[i] = -spriteyvelocity[i] * coeff_of_restitution;
            spritey[i] = MAX(0, MIN(spritey[i], ( (HEIGHT) - spriteheight[i] )));
            
            if (ABS(spriteyvelocity[i]) > 0.01)
                spriteyvelocity[i] = 0.0;
        }
        
    }
    
}

void drawRects(cairo_t *cr, cairo_surface_t *surface) {
    
    int i;
    
    cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
    
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 0.0);
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_rectangle (cr, 0, 0, WIDTH, HEIGHT);
    cairo_fill (cr);
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    
    for(i = 0; i < 6; i++)
    {
        
        if(i == 0) {
            cairo_set_source_rgba(cr, 1, 1, 1, 1);
        }
        else {
            cairo_set_source_rgba(cr, i * .15, i * .15, i * .15, .75);
        }
        
        cairo_rectangle(cr, spritex[i], spritey[i], 190, 240);
        cairo_fill(cr);
        
    }
    
    cairo_surface_flush(surface);
    
}

void drawSVG(cairo_t* cr, RsvgHandle* svg, cairo_surface_t *surface) {
    
    // Clear background as white
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    cairo_paint(cr);
    
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
    
    cairo_save(cr);
    rsvg_handle_render_cairo(svg, cr);
    cairo_restore(cr);
    
    cairo_surface_flush(surface);
    
}

void drawPNG(cairo_t* cr, cairo_surface_t *surface) {
    
    // Clear background as white
    cairo_set_source_rgba(cr, 1, 1, 1, 1);
    cairo_paint(cr);
    
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
    
    cairo_surface_t *image = cairo_image_surface_create_from_png("/Users/jessebennett/Documents/monster.png");
    
    cairo_set_source_surface(cr, image, 0, 0);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    
    cairo_surface_destroy(image);
    
    cairo_surface_flush(surface);
    
}


const NSOpenGLPixelFormatAttribute attrs[] = {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFADepthSize, 24,
    NSOpenGLPFAStencilSize, 8,
    NSOpenGLPFAAlphaSize, 8,
    NSOpenGLPFASampleBuffers, 1,
    NSOpenGLPFASamples, 4,
    NSOpenGLPFAMultisample,
    0
};

@interface StretchView : NSOpenGLView {
@private
    NSTimer *timer;
    cairo_surface_t *surface;
    cairo_t *cr;
    NSTimeInterval now;
    CVDisplayLinkRef displayLink; //display link for managing rendering thread
    NSOpenGLContext *context;
    cairo_device_t *device;
    CFTimeInterval startTime;
    RsvgHandle*  svg;
    RsvgDimensionData dims;
}
- (void) draw;

@end


@implementation StretchView

+ (NSOpenGLPixelFormat*)defaultPixelFormat
{
    
    NSOpenGLPixelFormat *classPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    
    if (!classPixelFormat)
        exit (-1);
    
    return classPixelFormat;
}

- (void)prepareOpenGL
{
    
    [self.openGLContext makeCurrentContext];
    
    GLint swapInt = 1;
    
    [self.openGLContext setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, (__bridge void * _Nullable)(self));
    
    // Set the display link for the current renderer
    CGLContextObj cglContext = self.openGLContext.CGLContextObj;
    CGLPixelFormatObj cglPixelFormat = self.pixelFormat.CGLPixelFormatObj;
    
    NSOpenGLPixelFormat *classPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    
    cglPixelFormat = (__bridge CGLPixelFormatObj)classPixelFormat;
    
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    // Activate the display link
    CVDisplayLinkStart(displayLink);
    
    context = super.openGLContext;
    
    device = cairo_nsgl_device_create ((__bridge void *)(context));
    
    cairo_gl_device_set_thread_aware(device, true);
    
    surface = cairo_gl_surface_create_for_view (device, (__bridge void *)(self), WIDTH, HEIGHT);
    
    cr = cairo_create (surface);
    
    startTime = CACurrentMediaTime();
    
    initRects();
    
    svg = rsvg_handle_new_from_file ("/Users/jessebennett/Documents/jessetext.svg", NULL);
    rsvg_handle_get_dimensions (svg, &dims);
    
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(__bridge StretchView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    
    // There is no autorelease pool when this method is called
    // because it will be called from a background thread
    // It's important to create one or you will leak objects
    @autoreleasepool {
    
    // Add your drawing codes here
    
        [self.openGLContext makeCurrentContext];
        
        // We draw on a secondary thread through the display link
        // Add a mutex around to avoid the threads from accessing the context simultaneously
        CGLLockContext(self.openGLContext.CGLContextObj);
        
        CFTimeInterval elapsedTime = CACurrentMediaTime() - startTime;
        
        drawPNG(cr, surface);
        
        //drawSVG(cr, svg, surface);
        
        //simulStep(spritex, spritey, spritewidth, spriteheight, spritexvelocity, spriteyvelocity, (double)elapsedTime / 1000);
        
        //drawRects(cr, surface);
        
        //trap_render(cr, WIDTH, HEIGHT);
        
        //rand_drawing(cr, surface);
        
        [self.openGLContext flushBuffer];
        
        CGLUnlockContext(self.openGLContext.CGLContextObj);
        
        
        return kCVReturnSuccess;
    }
}

- (void)dealloc
{
    // Release the display link
    CVDisplayLinkRelease(displayLink);
    
    [NSOpenGLContext clearCurrentContext];
    
    cairo_destroy(cr);
    
    cairo_surface_destroy(surface);
    
}

- (void)drawRect:(NSRect)dirtyRect
{
    //will draw just once?
}

- (void) draw
{
    [self setNeedsDisplay: YES];
    
}

@end

int main (int argc, char **argv)
{
    @autoreleasepool {
    //[NSApplication sharedApplication];
    
        int style = (NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask);
        
        NSScreen *screen = [NSScreen mainScreen];
        NSRect frame = screen.visibleFrame;
        int frame_height = frame.size.height;
        
        NSWindow *win = [[ NSWindow alloc] initWithContentRect: NSMakeRect (0, frame_height - HEIGHT, WIDTH, HEIGHT)
                                                     styleMask: style
                                                       backing: NSBackingStoreBuffered
                                                         defer: YES];
        
        StretchView *view = [[StretchView alloc] initWithFrame: NSMakeRect (0, 0, WIDTH, HEIGHT)];
        win.contentView = view;
        [win makeKeyAndOrderFront: win];
        [NSApp run];
        
    
    }
    
    return 0;
}
