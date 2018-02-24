/*
    TabToolbar - a small utility library for Qt, providing tabbed toolbars
  Copyright (C) 2018 Oleksii Sierov
  
    TabToolbar is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TabToolbar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TabToolbar.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef TT_API_H
#define TT_API_H

#if defined(_WIN32)
 #ifdef _MSC_VER
  #pragma warning(disable: 4251)
 #endif

 #if defined(TT_BUILDING_DLL)
  #define TT_API __declspec(dllexport)
 #else
  #define TT_API __declspec(dllimport)
 #endif

#elif defined(__GNUC__)
  
 #if defined(TT_BUILDING_DLL)
  #define TT_API __attribute__((visibility("default")))
 #else
  #define TT_API
 #endif
    
#endif

#endif //TT_API_H
