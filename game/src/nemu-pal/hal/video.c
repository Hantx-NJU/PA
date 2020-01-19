#include "hal.h"
#include "device/video.h"
#include "device/palette.h"

#include <string.h>
#include <stdlib.h>

int get_fps();

#define BYTE_PIXEL_OFFSET(dst, x, y) (((dst)->pitch) * (y) + (x))
#define BYTE_PIXEL_PTR(dst, x, y) ({ \
    SDL_Surface *__dst = (dst); \
    assert(__dst->pitch == __dst->w); \
    (((char *) (__dst->pixels)) + BYTE_PIXEL_OFFSET(__dst, x, y)); \
})

#define get_left(rect) ((rect)->x)
#define get_top(rect) ((rect)->y)
#define get_right(rect) (get_left(rect) + (rect)->w) // macro not safe
#define get_bottom(rect) (get_top(rect) + (rect)->h) // macro not safe

#define max(a, b) ((a) > (b) ? (a) : (b)) // macro not safe
#define min(a, b) ((a) < (b) ? (a) : (b)) // macro not safe

typedef int SDL_bool;
#define SDL_TRUE (1)
#define SDL_FALSE (0)
SDL_bool SDL_IntersectRect(const SDL_Rect* A, const SDL_Rect* B, SDL_Rect* result)
{
    int left, right, top, bottom;
    left = max(get_left(A), get_left(B));
    top = max(get_top(A), get_top(B));
    right = min(get_right(A), get_right(B));
    bottom = min(get_bottom(A), get_bottom(B));
    
    if (right > left && bottom > top) {
        result->x = left;
        result->y = top;
        result->w = right - left;
        result->h = bottom - top;
        return SDL_TRUE;
    } else {
        return SDL_FALSE;
    }
}


void GetSurfaceRect(SDL_Surface *surface, SDL_Rect *rect)
{
    rect->x = 0;
    rect->y = 0;
    rect->w = surface->w;
    rect->h = surface->h;
}

void SDL_GetClipRect(SDL_Surface *surface, SDL_Rect *rect)
{
    //printf("x=%d y=%d ", (int) surface->clip_rect.x, (int) surface->clip_rect.y);
    //printf("h=%d w=%d\n", (unsigned) surface->clip_rect.h, (unsigned) surface->clip_rect.w);
    SDL_Rect *clip_rect = &surface->clip_rect;
    if (clip_rect->x == 0 && clip_rect->y == 0 && clip_rect->h == 0 && clip_rect->w == 0) {
        GetSurfaceRect(surface, rect);
    } else {
        *rect = *clip_rect;
    }
    
    assert(rect->x >= 0 && rect->y >= 0);
    assert(rect->x + rect->w <= surface->w);
    assert(rect->y + rect->h <= surface->h);
}
void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect,
					 SDL_Surface *dst, SDL_Rect *dstrect)
{
	assert(dst && src);

	//int sx = (srcrect == NULL ? 0 : srcrect->x);
	//int sy = (srcrect == NULL ? 0 : srcrect->y);
	//int dx = (dstrect == NULL ? 0 : dstrect->x);
	//int dy = (dstrect == NULL ? 0 : dstrect->y);
	//int w = (srcrect == NULL ? src->w : srcrect->w);
	//int h = (srcrect == NULL ? src->h : srcrect->h);
	//if(dst->w - dx < w) { w = dst->w - dx; }
	//if(dst->h - dy < h) { h = dst->h - dy; }
	//if(dstrect != NULL) {
	//	dstrect->w = w;
	//	dstrect->h = h;
	//}

	/* TODO: copy pixels from position (`sx', `sy') with size
	 * `w' X `h' of `src' surface to position (`dx', `dy') of
	 * `dst' surface.
	 */

SDL_bool bret;
	
	// get src surface rect;
	SDL_Rect src_surface_rect;
	GetSurfaceRect(src, &src_surface_rect);
	if (srcrect) {
    	bret = SDL_IntersectRect(srcrect, &src_surface_rect, &src_surface_rect);
        if (bret == SDL_FALSE) return;
    }
	
	// get dst surface rect
    SDL_Rect dst_surface_rect;
    SDL_GetClipRect(dst, &dst_surface_rect);
	if (dstrect) {
	    SDL_Rect trect;
    	trect.x = dstrect->x;
	    trect.y = dstrect->y;
	    trect.w = src_surface_rect.w;
	    trect.h = src_surface_rect.h;
	    bret = SDL_IntersectRect(&trect, &dst_surface_rect, &dst_surface_rect);
        if (bret == SDL_FALSE) return;
	}

	
	// calc height and width needed to copy
	int copy_width = min(src_surface_rect.w, dst_surface_rect.w);
	int copy_height = min(src_surface_rect.h, dst_surface_rect.h);
	
	// do real copy
	int dst_line, src_line;
	for (dst_line = get_top(&dst_surface_rect),
	     src_line = get_top(&src_surface_rect);
	     
	        copy_height--;
	        
	            dst_line++,
	            src_line++
	    ) {
	    memcpy(BYTE_PIXEL_PTR(dst, dst_surface_rect.x, dst_line), 
	           BYTE_PIXEL_PTR(src, src_surface_rect.x, src_line),
	           copy_width);
	    assert(dst_line < dst->h);
	    assert(src_line < src->h);
	    assert(dst_surface_rect.x + copy_width <= dst->w);
	    assert(src_surface_rect.x + copy_width <= src->w);
	}
	//assert(0);
}

void SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, uint32_t color)
{
	assert(dst);
	assert(color <= 0xff);

	/* TODO: Fill the rectangle area described by `dstrect'
	 * in surface `dst' with color `color'. If dstrect is
	 * NULL, fill the whole surface.
	 */

	assert(0);
}

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors,
					int firstcolor, int ncolors)
{
	assert(s);
	assert(s->format);
	assert(s->format->palette);
	assert(firstcolor == 0);

	if (s->format->palette->colors == NULL || s->format->palette->ncolors != ncolors)
	{
		if (s->format->palette->ncolors != ncolors && s->format->palette->colors != NULL)
		{
			/* If the size of the new palette is different 
			 * from the old one, free the old one.
			 */
			free(s->format->palette->colors);
		}

		/* Get new memory space to store the new palette. */
		s->format->palette->colors = malloc(sizeof(SDL_Color) * ncolors);
		assert(s->format->palette->colors);
	}

	/* Set the new palette. */
	s->format->palette->ncolors = ncolors;
	memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

	if (s->flags & SDL_HWSURFACE)
	{
		/* TODO: Set the VGA palette by calling write_palette(). */
		//assert(0);
		write_palette(colors, ncolors);
	}
}

/* ======== The following functions are already implemented. ======== */

void SDL_UpdateRect(SDL_Surface *screen, int x, int y, int w, int h)
{
	assert(screen);
	assert(screen->pitch == 320);

	// this should always be true in NEMU-PAL
	assert(screen->flags & SDL_HWSURFACE);

	if (x == 0 && y == 0)
	{
		/* Draw FPS */
		vmem = VMEM_ADDR;
		char buf[80];
		sprintf(buf, "%dFPS", get_fps());
		draw_string(buf, 0, 0, 10);
	}
}

void SDL_SoftStretch(SDL_Surface *src, SDL_Rect *srcrect,
					 SDL_Surface *dst, SDL_Rect *dstrect)
{
	assert(src && dst);
	int x = (srcrect == NULL ? 0 : srcrect->x);
	int y = (srcrect == NULL ? 0 : srcrect->y);
	int w = (srcrect == NULL ? src->w : srcrect->w);
	int h = (srcrect == NULL ? src->h : srcrect->h);

	assert(dstrect);
	if (w == dstrect->w && h == dstrect->h)
	{
		/* The source rectangle and the destination rectangle
		 * are of the same size. If that is the case, there
		 * is no need to stretch, just copy. */
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		SDL_BlitSurface(src, &rect, dst, dstrect);
	}
	else
	{
		/* No other case occurs in NEMU-PAL. */
		assert(0);
	}
}

SDL_Surface *SDL_CreateRGBSurface(uint32_t flags, int width, int height, int depth,
								  uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask)
{
	SDL_Surface *s = malloc(sizeof(SDL_Surface));
	assert(s);
	s->format = malloc(sizeof(SDL_PixelFormat));
	assert(s);
	s->format->palette = malloc(sizeof(SDL_Palette));
	assert(s->format->palette);
	s->format->palette->colors = NULL;

	s->format->BitsPerPixel = depth;

	s->flags = flags;
	s->w = width;
	s->h = height;
	s->pitch = (width * depth) >> 3;
	s->pixels = (flags & SDL_HWSURFACE ? (void *)VMEM_ADDR : malloc(s->pitch * height));
	assert(s->pixels);

	return s;
}

SDL_Surface *SDL_SetVideoMode(int width, int height, int bpp, uint32_t flags)
{
	return SDL_CreateRGBSurface(flags, width, height, bpp,
								0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}

void SDL_FreeSurface(SDL_Surface *s)
{
	if (s != NULL)
	{
		if (s->format != NULL)
		{
			if (s->format->palette != NULL)
			{
				if (s->format->palette->colors != NULL)
				{
					free(s->format->palette->colors);
				}
				free(s->format->palette);
			}

			free(s->format);
		}

		if (s->pixels != NULL && s->pixels != VMEM_ADDR)
		{
			free(s->pixels);
		}

		free(s);
	}
}
