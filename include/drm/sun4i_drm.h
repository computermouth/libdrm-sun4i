/*
 * Copyright (c) 2012-2013, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef _UAPI_SUN4I_DRM_H_
#define _UAPI_SUN4I_DRM_H_

#include <drm.h>

#define DRM_SUN4I_GEM_CREATE_TILED     (1 << 0)
#define DRM_SUN4I_GEM_CREATE_BOTTOM_UP (1 << 1)

struct drm_sun4i_gem_create {
	__u64 size;
	__u32 flags;
	__u32 handle;
};

struct drm_sun4i_gem_mmap {
	__u32 handle;
	__u32 pad;
	__u64 offset;
};

struct drm_sun4i_syncpt_read {
	__u32 id;
	__u32 value;
};

struct drm_sun4i_syncpt_incr {
	__u32 id;
	__u32 pad;
};

struct drm_sun4i_syncpt_wait {
	__u32 id;
	__u32 thresh;
	__u32 timeout;
	__u32 value;
};

#define DRM_SUN4I_NO_TIMEOUT	(0xffffffff)

struct drm_sun4i_open_channel {
	__u32 client;
	__u32 pad;
	__u64 context;
};

struct drm_sun4i_close_channel {
	__u64 context;
};

struct drm_sun4i_get_syncpt {
	__u64 context;
	__u32 index;
	__u32 id;
};

struct drm_sun4i_get_syncpt_base {
	__u64 context;
	__u32 syncpt;
	__u32 id;
};

struct drm_sun4i_syncpt {
	__u32 id;
	__u32 incrs;
};

struct drm_sun4i_cmdbuf {
	__u32 handle;
	__u32 offset;
	__u32 words;
	__u32 pad;
};

struct drm_sun4i_reloc {
	struct {
		__u32 handle;
		__u32 offset;
	} cmdbuf;
	struct {
		__u32 handle;
		__u32 offset;
	} target;
	__u32 shift;
	__u32 pad;
};

struct drm_sun4i_waitchk {
	__u32 handle;
	__u32 offset;
	__u32 syncpt;
	__u32 thresh;
};

struct drm_sun4i_submit {
	__u64 context;
	__u32 num_syncpts;
	__u32 num_cmdbufs;
	__u32 num_relocs;
	__u32 num_waitchks;
	__u32 waitchk_mask;
	__u32 timeout;
	__u64 syncpts;
	__u64 cmdbufs;
	__u64 relocs;
	__u64 waitchks;
	__u32 fence;		/* Return value */

	__u32 reserved[5];	/* future expansion */
};

#define DRM_SUN4I_GEM_TILING_MODE_PITCH 0
#define DRM_SUN4I_GEM_TILING_MODE_TILED 1
#define DRM_SUN4I_GEM_TILING_MODE_BLOCK 2

struct drm_sun4i_gem_set_tiling {
	/* input */
	__u32 handle;
	__u32 mode;
	__u32 value;
	__u32 pad;
};

struct drm_sun4i_gem_get_tiling {
	/* input */
	__u32 handle;
	/* output */
	__u32 mode;
	__u32 value;
	__u32 pad;
};

#define DRM_SUN4I_GEM_BOTTOM_UP		(1 << 0)
#define DRM_SUN4I_GEM_FLAGS		(DRM_SUN4I_GEM_BOTTOM_UP)

struct drm_sun4i_gem_set_flags {
	/* input */
	__u32 handle;
	/* output */
	__u32 flags;
};

struct drm_sun4i_gem_get_flags {
	/* input */
	__u32 handle;
	/* output */
	__u32 flags;
};

#define DRM_SUN4I_GEM_CREATE		0x00
#define DRM_SUN4I_GEM_MMAP		0x01
#define DRM_SUN4I_SYNCPT_READ		0x02
#define DRM_SUN4I_SYNCPT_INCR		0x03
#define DRM_SUN4I_SYNCPT_WAIT		0x04
#define DRM_SUN4I_OPEN_CHANNEL		0x05
#define DRM_SUN4I_CLOSE_CHANNEL		0x06
#define DRM_SUN4I_GET_SYNCPT		0x07
#define DRM_SUN4I_SUBMIT		0x08
#define DRM_SUN4I_GET_SYNCPT_BASE	0x09
#define DRM_SUN4I_GEM_SET_TILING	0x0a
#define DRM_SUN4I_GEM_GET_TILING	0x0b
#define DRM_SUN4I_GEM_SET_FLAGS		0x0c
#define DRM_SUN4I_GEM_GET_FLAGS		0x0d

#define DRM_IOCTL_SUN4I_GEM_CREATE DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_GEM_CREATE, struct drm_sun4i_gem_create)
#define DRM_IOCTL_SUN4I_GEM_MMAP DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_GEM_MMAP, struct drm_sun4i_gem_mmap)
#define DRM_IOCTL_SUN4I_SYNCPT_READ DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_SYNCPT_READ, struct drm_sun4i_syncpt_read)
#define DRM_IOCTL_SUN4I_SYNCPT_INCR DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_SYNCPT_INCR, struct drm_sun4i_syncpt_incr)
#define DRM_IOCTL_SUN4I_SYNCPT_WAIT DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_SYNCPT_WAIT, struct drm_sun4i_syncpt_wait)
#define DRM_IOCTL_SUN4I_OPEN_CHANNEL DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_OPEN_CHANNEL, struct drm_sun4i_open_channel)
#define DRM_IOCTL_SUN4I_CLOSE_CHANNEL DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_CLOSE_CHANNEL, struct drm_sun4i_open_channel)
#define DRM_IOCTL_SUN4I_GET_SYNCPT DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_GET_SYNCPT, struct drm_sun4i_get_syncpt)
#define DRM_IOCTL_SUN4I_SUBMIT DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_SUBMIT, struct drm_sun4i_submit)
#define DRM_IOCTL_SUN4I_GET_SYNCPT_BASE DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_GET_SYNCPT_BASE, struct drm_sun4i_get_syncpt_base)
#define DRM_IOCTL_SUN4I_GEM_SET_TILING DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_GEM_SET_TILING, struct drm_sun4i_gem_set_tiling)
#define DRM_IOCTL_SUN4I_GEM_GET_TILING DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_GEM_GET_TILING, struct drm_sun4i_gem_get_tiling)
#define DRM_IOCTL_SUN4I_GEM_SET_FLAGS DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_GEM_SET_FLAGS, struct drm_sun4i_gem_set_flags)
#define DRM_IOCTL_SUN4I_GEM_GET_FLAGS DRM_IOWR(DRM_COMMAND_BASE + DRM_SUN4I_GEM_GET_FLAGS, struct drm_sun4i_gem_get_flags)

#endif
