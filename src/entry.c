/* ========================================================================== */
/*                                                                            */
/* This file is part of Butane.                                               */
/*                                                                            */
/* Author(s):                                                                 */
/*                                                                            */
/*   Michael Williams <devbug@bitbyte.ca>                                     */
/*                                                                            */
/* See LICENSE.md for licensing information.                                  */
/*                                                                            */
/* ========================================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

int main(const size_t num_of_args, const char *args[]) {
  (void)num_of_args; (void)args;
  setlocale(LC_ALL, "en_US.UTF-8");
  fprintf(stdout, "Hello, Butane!\n");
  return EXIT_SUCCESS;
}
