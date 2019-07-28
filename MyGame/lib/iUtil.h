#pragma once

char* loadFile(int& length, const char* szFormat, ...);
void writeFile(const char* path, char* buf, int bufLength);
