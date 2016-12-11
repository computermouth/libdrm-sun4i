/*
 * Copyright © 2012, 2013 Thierry Reding
 * Copyright © 2013 Erik Faye-Lund
 * Copyright © 2014 NVIDIA Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __DRM_SUN4I_H__
#define __DRM_SUN4I_H__ 1

#include <stdint.h>
#include <stdlib.h>

struct drm_sun4i_bo;
struct drm_sun4i;

int drm_sun4i_new(struct drm_sun4i **drmp, int fd);
void drm_sun4i_close(struct drm_sun4i *drm);

int drm_sun4i_bo_new(struct drm_sun4i_bo **bop, struct drm_sun4i *drm,
		     uint32_t flags, uint32_t size);
int drm_sun4i_bo_wrap(struct drm_sun4i_bo **bop, struct drm_sun4i *drm,
		      uint32_t handle, uint32_t flags, uint32_t size);
struct drm_sun4i_bo *drm_sun4i_bo_ref(struct drm_sun4i_bo *bo);
void drm_sun4i_bo_unref(struct drm_sun4i_bo *bo);
int drm_sun4i_bo_get_handle(struct drm_sun4i_bo *bo, uint32_t *handle);
int drm_sun4i_bo_map(struct drm_sun4i_bo *bo, void **ptr);
int drm_sun4i_bo_unmap(struct drm_sun4i_bo *bo);

int drm_sun4i_bo_get_flags(struct drm_sun4i_bo *bo, uint32_t *flags);
int drm_sun4i_bo_set_flags(struct drm_sun4i_bo *bo, uint32_t flags);

struct drm_sun4i_bo_tiling {
	uint32_t mode;
	uint32_t value;
};

int drm_sun4i_bo_get_tiling(struct drm_sun4i_bo *bo,
			    struct drm_sun4i_bo_tiling *tiling);
int drm_sun4i_bo_set_tiling(struct drm_sun4i_bo *bo,
			    const struct drm_sun4i_bo_tiling *tiling);

#endif /* __DRM_SUN4I_H__ */
