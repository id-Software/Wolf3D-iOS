/*
 *  wolfiphone.h
 *  wolf3d
 *
 *  Created by John Carmack on 3/13/09.
 *  Copyright 2009 idSoftware. All rights reserved.
 *
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
#include "wolf/wolf_level.h"
#include "wolf/wolf_local.h"
#include "wolf/wolf_sprites.h"
#include "wolf/wolf_actors.h"
#include "wolf/wolf_actor_ai.h"
#include "wolf/wolf_player.h"
#include "wolf/wolf_powerups.h"
#include "wolf/wolf_raycast.h"
#include "wolf/wolf_renderer.h"

#include "iphone/iphone_wolf.h"

