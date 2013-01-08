// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#include <lwe/foundation.h>
#include <lwe/render_device.h>
#include <lwe/window.h>
#include <lwe/swap_chain.h>

static lwe_window_t* _window = NULL;
static lwe_swap_chain_t* _swap_chain = NULL;

int main( int argc, char** argv )
{
  lwe_render_device_create(0);

  _window = lwe_window_open(
    "Engine", 1280, 720
  );

  _swap_chain = lwe_swap_chain_create(
    _window, 1280, 720, LWE_PIXEL_FORMAT_R8G8B8A8, false, false
  );

  lwe_window_show(_window);

  while (true) {
    lwe_message_pump();
    lwe_swap_chain_present(_swap_chain);
  }

  return EXIT_SUCCESS;
}