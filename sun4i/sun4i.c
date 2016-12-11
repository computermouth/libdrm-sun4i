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

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>

#include <xf86drm.h>

#include <sun4i_drm.h>

#include "private.h"

static void drm_sun4i_bo_free(struct drm_sun4i_bo *bo)
{
	struct drm_sun4i *drm = bo->drm;
	struct drm_gem_close args;

	if (bo->map)
		munmap(bo->map, bo->size);

	memset(&args, 0, sizeof(args));
	args.handle = bo->handle;

	drmIoctl(drm->fd, DRM_IOCTL_GEM_CLOSE, &args);

	free(bo);
}

static int drm_sun4i_wrap(struct drm_sun4i **drmp, int fd, bool close)
{
	struct drm_sun4i *drm;

	if (fd < 0 || !drmp)
		return -EINVAL;

	drm = calloc(1, sizeof(*drm));
	if (!drm)
		return -ENOMEM;

	drm->close = close;
	drm->fd = fd;

	*drmp = drm;

	return 0;
}

int drm_sun4i_new(struct drm_sun4i **drmp, int fd)
{
	bool supported = false;
	drmVersionPtr version;

	version = drmGetVersion(fd);
	if (!version)
		return -ENOMEM;

	if (!strncmp(version->name, "sun4i", version->name_len))
		supported = true;

	drmFreeVersion(version);

	if (!supported)
		return -ENOTSUP;

	return drm_sun4i_wrap(drmp, fd, false);
}

void drm_sun4i_close(struct drm_sun4i *drm)
{
	if (!drm)
		return;

	if (drm->close)
		close(drm->fd);

	free(drm);
}

int drm_sun4i_bo_new(struct drm_sun4i_bo **bop, struct drm_sun4i *drm,
		     uint32_t flags, uint32_t size)
{
	struct drm_sun4i_gem_create args;
	struct drm_sun4i_bo *bo;
	int err;

	if (!drm || size == 0 || !bop)
		return -EINVAL;

	bo = calloc(1, sizeof(*bo));
	if (!bo)
		return -ENOMEM;

	atomic_set(&bo->ref, 1);
	bo->flags = flags;
	bo->size = size;
	bo->drm = drm;

	memset(&args, 0, sizeof(args));
	args.flags = flags;
	args.size = size;

	err = drmCommandWriteRead(drm->fd, DRM_SUN4I_GEM_CREATE, &args,
				  sizeof(args));
	if (err < 0) {
		err = -errno;
		free(bo);
		return err;
	}

	bo->handle = args.handle;

	*bop = bo;

	return 0;
}

int drm_sun4i_bo_wrap(struct drm_sun4i_bo **bop, struct drm_sun4i *drm,
		      uint32_t handle, uint32_t flags, uint32_t size)
{
	struct drm_sun4i_bo *bo;

	if (!drm || !bop)
		return -EINVAL;

	bo = calloc(1, sizeof(*bo));
	if (!bo)
		return -ENOMEM;

	atomic_set(&bo->ref, 1);
	bo->handle = handle;
	bo->flags = flags;
	bo->size = size;
	bo->drm = drm;

	*bop = bo;

	return 0;
}

struct drm_sun4i_bo *drm_sun4i_bo_ref(struct drm_sun4i_bo *bo)
{
	if (bo)
		atomic_inc(&bo->ref);

	return bo;
}

void drm_sun4i_bo_unref(struct drm_sun4i_bo *bo)
{
	if (bo && atomic_dec_and_test(&bo->ref))
		drm_sun4i_bo_free(bo);
}

int drm_sun4i_bo_get_handle(struct drm_sun4i_bo *bo, uint32_t *handle)
{
	if (!bo || !handle)
		return -EINVAL;

	*handle = bo->handle;

	return 0;
}

int drm_sun4i_bo_map(struct drm_sun4i_bo *bo, void **ptr)
{
	struct drm_sun4i *drm = bo->drm;

	if (!bo->map) {
		struct drm_sun4i_gem_mmap args;
		int err;

		memset(&args, 0, sizeof(args));
		args.handle = bo->handle;

		err = drmCommandWriteRead(drm->fd, DRM_SUN4I_GEM_MMAP, &args,
					  sizeof(args));
		if (err < 0)
			return -errno;

		bo->offset = args.offset;

		bo->map = mmap(0, bo->size, PROT_READ | PROT_WRITE, MAP_SHARED,
			       drm->fd, bo->offset);
		if (bo->map == MAP_FAILED) {
			bo->map = NULL;
			return -errno;
		}
	}

	if (ptr)
		*ptr = bo->map;

	return 0;
}

int drm_sun4i_bo_unmap(struct drm_sun4i_bo *bo)
{
	if (!bo)
		return -EINVAL;

	if (!bo->map)
		return 0;

	if (munmap(bo->map, bo->size))
		return -errno;

	bo->map = NULL;

	return 0;
}

int drm_sun4i_bo_get_flags(struct drm_sun4i_bo *bo, uint32_t *flags)
{
	struct drm_sun4i_gem_get_flags args;
	struct drm_sun4i *drm = bo->drm;
	int err;

	if (!bo)
		return -EINVAL;

	memset(&args, 0, sizeof(args));
	args.handle = bo->handle;

	err = drmCommandWriteRead(drm->fd, DRM_SUN4I_GEM_GET_FLAGS, &args,
				  sizeof(args));
	if (err < 0)
		return -errno;

	if (flags)
		*flags = args.flags;

	return 0;
}

int drm_sun4i_bo_set_flags(struct drm_sun4i_bo *bo, uint32_t flags)
{
	struct drm_sun4i_gem_get_flags args;
	struct drm_sun4i *drm = bo->drm;
	int err;

	if (!bo)
		return -EINVAL;

	memset(&args, 0, sizeof(args));
	args.handle = bo->handle;
	args.flags = flags;

	err = drmCommandWriteRead(drm->fd, DRM_SUN4I_GEM_SET_FLAGS, &args,
				  sizeof(args));
	if (err < 0)
		return -errno;

	return 0;
}

int drm_sun4i_bo_get_tiling(struct drm_sun4i_bo *bo,
			    struct drm_sun4i_bo_tiling *tiling)
{
	struct drm_sun4i_gem_get_tiling args;
	struct drm_sun4i *drm = bo->drm;
	int err;

	if (!bo)
		return -EINVAL;

	memset(&args, 0, sizeof(args));
	args.handle = bo->handle;

	err = drmCommandWriteRead(drm->fd, DRM_SUN4I_GEM_GET_TILING, &args,
				  sizeof(args));
	if (err < 0)
		return -errno;

	if (tiling) {
		tiling->mode = args.mode;
		tiling->value = args.value;
	}

	return 0;
}

int drm_sun4i_bo_set_tiling(struct drm_sun4i_bo *bo,
			    const struct drm_sun4i_bo_tiling *tiling)
{
	struct drm_sun4i_gem_set_tiling args;
	struct drm_sun4i *drm = bo->drm;
	int err;

	if (!bo)
		return -EINVAL;

	memset(&args, 0, sizeof(args));
	args.handle = bo->handle;
	args.mode = tiling->mode;
	args.value = tiling->value;

	err = drmCommandWriteRead(drm->fd, DRM_SUN4I_GEM_SET_TILING, &args,
				  sizeof(args));
	if (err < 0)
		return -errno;

	return 0;
}
