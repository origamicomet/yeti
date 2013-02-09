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

#include <lwe/time_step_policy.h>

lwe_time_step_policy_t lwe_string_to_time_step_policy(
  lwe_const_str_t str )
{
  if (strcmp("variable", str) == 0)
    return LWE_TIME_STEP_POLICY_VARIABLE;  
  else if (strcmp("fixed", str) == 0)
    return LWE_TIME_STEP_POLICY_FIXED;    
  else if (strcmp("smoothed", str) == 0)
    return LWE_TIME_STEP_POLICY_SMOOTHED;    
  else if (strcmp("smoothed_with_payback", str) == 0)
    return LWE_TIME_STEP_POLICY_SMOOTHED_WITH_PAYBACK;
  
  lwe_fail("Invalid time-step policy `%s`", str);    
}