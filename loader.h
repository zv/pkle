#ifndef __LOADER_H__
#define __LOADER_H__

#include <stdint.h>
#include <vector>
#include <string>

struct encrypted_file {
  std::vector<uint8_t> bytes; /* first few bytes of the archive stream. includes the encryption header */
  std::string name; /* name of the file, as it appears in the archive */
  uint32_t crc32;
  uint32_t compressed_size;
  uint32_t original_size;
};

std::vector<encrypted_file> load_file(const std::string& file_name);

#endif /*__LOADER_H__ */
