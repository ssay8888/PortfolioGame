#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <ole2.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <io.h>
#include <unordered_map>
#pragma warning (push)
#pragma warning (disable: 26812 6255 6387 26812 6031 6258 6001 26451 26495 26498 6386)
#include <boost/asio.hpp>
#pragma warning (pop)
#include <iostream>
