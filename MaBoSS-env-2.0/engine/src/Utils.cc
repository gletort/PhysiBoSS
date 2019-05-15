/* 
   MaBoSS (Markov Boolean Stochastic Simulator)
   Copyright (C) 2011-2018 Institut Curie, 26 rue d'Ulm, Paris, France
   
   MaBoSS is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   MaBoSS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
*/

/*
   Module:
     Utils.cc

   Authors:
     Eric Viara <viara@sysra.com>
 
   Date:
     May 2018
*/

#include "Utils.h"
#include "MaBEstEngine.h"
#include <sys/stat.h>

const std::string NL_PATTERN = "@--NL--@";
unsigned int ConfigOpt::runconfig_file_cnt = 0;
unsigned int ConfigOpt::runconfig_expr_cnt = 0;

int checkArgMissing(const char* prog, const char* opt, int nn, int argc)
{
  if (nn == argc-1) {
    std::cerr << '\n' << prog << ": missing value after option " << opt << '\n';
    return 1;
  }
  return 0;
}

int fileGetContents(const std::string& file, std::string& contents)
{
  int fd = open(file.c_str(), O_RDONLY);
  if (fd < 0) {
    std::cerr << "cannot open file " + file + " for reading" << std::endl;
    return 1;
  }
  struct stat st;
  if (fstat(fd, &st) < 0) {
    std::cerr << "cannot stat file " + file + "" << std::endl;
    return 1;
  }

  size_t size = st.st_size;
  char* buffer = new char[size+1];
  buffer[size] = 0;

  size_t size_read = 0;
  for (;;) {
    ssize_t rsize = read(fd, buffer+size_read, size-size_read);
    if (rsize <= 0) {
      perror("read");
      return 1;
    }
    size_read += rsize;
    if (size_read == size) {
      break;
    }
    if (size_read > size) {
      perror("read");
      return 1;
    }
  }
  close(fd);

  contents = buffer;
  delete [] buffer;
  return 0;
}

int filePutContents(const std::string& file, const std::string& data)
{
  FILE* fd = fopen(file.c_str(), "w");
  if (fd == NULL) {
    std::cerr << "cannot open file " + file + " for writing" << std::endl;
    return 1;
  }
  fputs(data.c_str(), fd);
  fclose(fd);

  return 0;
}

std::string stringReplaceAll(const std::string& subject, const std::string& from, const std::string& to)
{
  std::string str = subject;
  std::string::size_type from_size = from.length();
  std::string::size_type to_size = to.length();
  std::string::size_type itpos;
  std::string::size_type pos;

  pos = 0;
  while ((itpos = str.find(from, pos)) != std::string::npos) {
    str = str.replace(str.begin()+itpos, str.begin()+itpos+from_size, to);
    pos = itpos + to_size;
  }

  return str;

}

// EV: 2018-12-19
// std::hexfloat is missing on some gcc compiler version
// introduced this function to print double in hexa format
const char* fmthexdouble(double d)
{
  static const int MAXBUF_FMTDOUBLE = 8;
  static char buf[MAXBUF_FMTDOUBLE][64];
  static int buf_ind = 0;
  if (buf_ind >= MAXBUF_FMTDOUBLE) {
    buf_ind = 0;
  }
  sprintf(buf[buf_ind], "%a", d);
  return buf[buf_ind++];
}
