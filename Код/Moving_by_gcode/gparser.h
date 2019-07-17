#pragma once
#include <SD.h>
#include "printer.h"

int get_index(char& c);

void do_command(String& str);

void do_G0(String& str);