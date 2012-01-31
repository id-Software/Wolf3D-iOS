/*
 
 Copyright (C) 2009-2011 id Software LLC, a ZeniMax Media company. 

 This file is part of the WOLF3D iOS v2.1 GPL Source Code. 

 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 
 */

#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <setjmp.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>
#include <dlfcn.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include "iphone/gles_glue.h"


#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/oalStaticBufferExtension.h>
#undef ALCAPI
#define ALCAPI

#include "Tremor/ivorbisfile.h"

#include "env/arch.h"
#include "env/angle.h"
#include "env/matrix.h"
#include "env/vector.h"
#include "env/mymath.h"
#include "env/common_utils.h"
#include "env/cvar.h"
#include "env/zmem.h"
#include "env/mymath.h"
#include "env/cmd.h"
#include "env/num_type.h"
#include "env/filestring.h"
#include "env/filesystem.h"
#include "env/app_def.h"
#include "env/common.h"
#include "env/video.h"
#include "env/texture_manager.h"
#include "env/font_manager.h"
#include "env/renderer.h"
#include "env/opengl_local.h"
#include "env/timer.h"
#include "env/com_string.h"
#include "env/console.h"
#include "env/sound_local.h"
#include "env/sound.h"
#include "env/memory.h"
#include "env/random_number.h"
#include "env/openal_binding.h"
#include "env/glob.h"
#include "env/opengl_local.h"
#include "env/tga.h"
#include "env/wavfile.h"
#include "env/oggfile.h"


#include "wolf/wolf_math.h"
#include "wolf/wolf_powerups.h"
#include "wolf/wolf_sprites.h"
#include "wolf/wolf_level.h"
#include "wolf/wolf_local.h"
#include "wolf/wolf_actors.h"
#include "wolf/wolf_actor_ai.h"
#include "wolf/wolf_player.h"
#include "wolf/wolf_raycast.h"
#include "wolf/wolf_renderer.h"

#include "iphone/iphone_wolf.h"

#include "iphone_alerts.h"

#include <CoreFoundation/CFDate.h>

extern CFAbsoluteTime soundTime;
extern CFAbsoluteTime menuTime;

void picTimingPrint();
void picTimingClear();
