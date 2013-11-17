#!/bin/bash
# Copyright (c) 2010-14 Bifrost Entertainment AS and Tommy Nguyen
# Distributed under the MIT License.
# (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

DST="lib/GL/glext.h"
URL="http://www.opengl.org/registry/api/GL/glext.h"
[[ ! -f $DST ]] && echo "Please run from root of repository" && exit 1
curl $URL -o $DST
