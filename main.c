// The MIT License (MIT)

// Copyright (c) 2022 Kove Ochre-Salter

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

#include <stdio.h>
#include <stdlib.h>

char *brightness_path = "";
char *max_brightness_path = "";

FILE *brightness_file = NULL;
FILE *max_brightness_file = NULL;

int brightness = 0;
int max_brightness = 0;

int delta = 0;
int relative_delta = 0;

void up()
{
  brightness_path = getenv("BCTL_BRIGHTNESS");
  max_brightness_path = getenv("BCTL_MAX_BRIGHTNESS");

  if (brightness_path == NULL || max_brightness_path == NULL)
  {
    fprintf(stderr, "error: the environment variables are not set!\n");

    exit(EXIT_FAILURE);
  }

  brightness_file = fopen(brightness_path, "r+");
  max_brightness_file = fopen(max_brightness_path, "r");

  if (brightness_file == NULL || max_brightness_file == NULL)
  {
    fprintf(stderr, "error: the device could not be opened!\n");

    exit(EXIT_FAILURE);
  }

  fscanf(brightness_file, "%d", &brightness);
  fscanf(max_brightness_file, "%d", &max_brightness);
}

void down()
{
  fclose(brightness_file);
  fclose(max_brightness_file);
}

int main(int argc, char **argv)
{
  up();

  if (argc == 1)
  {
    fprintf(stdout, "%d%%\n", (int)((float)brightness * 100.0 / (float)max_brightness));
  }
  else if (argc == 2 && (isdigit(argv[1][0]) || argv[1][0] == '+' || argv[1][0] == '-'))
  {
    int i = !isdigit(argv[1][0]);

    while (argv[1][i] != '\0')
    {
      relative_delta *= 10;
      relative_delta += argv[1][i] - '0';
      i++;
    }

    delta = max_brightness / 100 * relative_delta;

    switch (argv[1][0])
    {
    case '+':
      brightness += delta;
      break;

    case '-':
      if (delta <= brightness)
      {
        brightness -= delta;
      }
      else
      {
        brightness = 0;
      }
      break;

    default:
      brightness = delta;
      break;
    }

    fprintf(brightness_file, "%d", brightness);
    fflush(brightness_file);
  }
  else
  {
    fprintf(stderr, "error: invalid arguments were supplied!\n");

    return 1;
  }

  down();

  return 0;
}
