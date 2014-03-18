// Copyright (C) 2012--2014 Chi-kwan Chan
// Copyright (C) 2012--2014 Steward Observatory
//
// This file is part of GRay.
//
// GRay is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GRay is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GRay.  If not, see <http://www.gnu.org/licenses/>.

#include "gray.h"

#ifndef DT_DUMP
#define DT_DUMP 1
#endif

#ifdef ENABLE_GL
int Para::solve(Data &data)
{
  debug("solve(*%p)\n", &data);

  while(!glfwWindowShouldClose(vis::window)) {
    static size_t count = 0;
    static size_t delta = 32;
    const  size_t limit = 1024;

    if(global::dt_dump != 0.0) {
      double ms;
      if(count + delta < limit) {
        ms = data.evolve(global::dt_dump * delta / limit);
        if(0 == ms) break;
        count += delta;
      } else {
	ms = data.evolve(global::dt_dump * (limit - count) / limit);
        if(0 == ms) break;
        count = 0;
      }
      if(ms < 20 && delta < limit) delta *= 2;
      if(ms > 80 && delta > 1    ) delta /= 2;
    }

#if defined(ENABLE_PRIME) || defined(ENABLE_LEAP)
    vis::sense();
#endif
    data.show();

    glfwSwapBuffers(vis::window);
    glfwPollEvents();
  }

  data.spec(global::output); // TODO: check if glutMainLoop() actually exit...
  return 0;
}
#else
int Para::solve(Data &data)
{
  debug("solve(*%p)\n", &data);

  if(global::dt_dump != 0.0) {
    data.dump(global::format);
    while(0 < data.evolve(global::dt_dump))
      data.dump(global::format);
  } else
    while(0 < data.evolve(DT_DUMP));

  data.spec(global::output);
  return 0;
}
#endif
