#include "loader.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>
#include <list>

/* https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT */
struct LocalFileHeader {
  uint32_t signature;        /* local file header signature     4 bytes  (0x04034b50) */
  uint16_t version;          /*  version needed to extract      2 bytes */
  uint16_t flags;            /*  general purpose bit flag       2 bytes */
  uint16_t method;           /*  compression method             2 bytes */
  uint16_t modified_time;    /*  last mod file time             2 bytes */
  uint16_t modified_date;    /*  last mod file date              2 bytes */
  uint32_t crc32;            /*  crc-32                          4 bytes */
  uint32_t compressed_size;  /*  compressed size                 4 bytes */
  uint32_t original_size;    /*  uncompressed size               4 bytes */
  uint16_t file_name_length; /*  file name length                2 bytes */
  uint16_t extra_length;     /*  extra field length              2 bytes */
} __attribute((packed));

/* https://users.cs.jmu.edu/buchhofp/forensics/formats/pkzip.html#datadescriptor */
struct DataDescriptor {
  uint32_t crc32;
  uint32_t compressed_size;
  uint32_t original_size;
} __attribute((packed));

std::string itoa(const uint32_t n) {
  std::stringstream ss;
  ss << std::hex << ss;
  return ss.str();
}

std::string hash(const uint32_t crc32, const uint32_t compressedSize, const uint32_t originalSize) {
  return itoa(crc32) + "|" + itoa(compressedSize) + "|" + itoa(originalSize);
}

std::vector<uint8_t> readFileBytes(FILE* file, const size_t length) {
  /* only need the first few bytes of the file */
  std::vector<uint8_t> buffer(24);
  size_t bytesRead = fread(&buffer[0], 1, buffer.size(), file);
  if (bytesRead != 24) {
    return std::vector<uint8_t>();
  }

  /* skip over the reset of the file data */
  fseek(file, length-bytesRead, SEEK_CUR);

  /* trim to number of elements read */
  buffer.resize(bytesRead);
  return buffer;
}

std::vector<encrypted_file> load_file(const std::string& file_name) {

  FILE* file = fopen(file_name.c_str(), "rb");
  if (!file) {
    std::cout << "Error: unable to open input file for reading." << std::endl;
    std::vector<encrypted_file>();
  }

  std::cout << "Scanning archive for duplicates..." << std::endl;

  std::map<std::string, std::vector<encrypted_file> > files;
  bool duplicatesFound = false;

  while (true) {
    LocalFileHeader header; /* a zip file is an array of local file headers */
    size_t bytesRead = fread(&header, 1, sizeof(LocalFileHeader), file);
    if (bytesRead != sizeof(LocalFileHeader)) {
      break; /* EOF */
    }
    if(header.signature != 0x04034b50) {
      break; /* EOF */
    }
    std::vector<char> fileName(header.file_name_length);
    bytesRead = fread(&fileName[0], 1, header.file_name_length, file);
    if (bytesRead != header.file_name_length) {
      break; /* EOF */
    }
    fileName.push_back(0); /* null terminate */

    fseek(file, header.extra_length, SEEK_CUR); /* skip over this "extra data" */

    if (header.flags & 1) { /* file is encrypted */
      std::cout << "  " << &fileName[0] << " (" << std::dec << header.original_size << " bytes with checksum 0x" << std::hex << header.crc32 << std::endl;

      encrypted_file entry;
      entry.bytes = readFileBytes(file, header.compressed_size);
      if (!entry.bytes.size()) {
        break; /* EOF */
      }
      entry.name = &fileName[0];
      entry.crc32 = header.crc32;
      entry.compressed_size = header.compressed_size;
      entry.original_size = header.original_size;

      /* BEGIN TEST */
      //CipherState cs = cipher_init(keyBytes("password"));
      //for (size_t i = 0; i < entry.bytes.size(); ++i) {
        //entry.bytes[i] = cipher_decrypt(cs, entry.bytes[i]).toInteger();
      //}
      //printf("TEST: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", entry.bytes[11], entry.bytes[12], entry.bytes[13], entry.bytes[14], entry.bytes[15]);
      //return 0;
      /* END TEST */

      std::string infoHash = hash(header.crc32, header.compressed_size, header.original_size);
      std::vector<encrypted_file>& entryList = files[infoHash];
      entryList.push_back(entry);
      if (entryList.size() >= 2) {
        duplicatesFound = true; /* for output formatting */
      }
    } else { /* skip over unencrypted files */
      fseek(file, header.compressed_size, SEEK_CUR);
    }

    uint32_t signature;
    bytesRead = fread(&signature, 1, sizeof(signature), file);
    if (bytesRead != sizeof(signature)) {
      break; /* EOF */
    }
    DataDescriptor descriptor;
    if (signature == 0x08074b50) {
      bytesRead = fread(&descriptor, 1, sizeof(descriptor), file);
      if (bytesRead != sizeof(descriptor)) {
        break; /* EOF */
      }
    } else { /* no prefix. just read the rest of the bytes */
      descriptor.crc32 = signature;
      size_t offset = sizeof(signature);
      char* desc = (char*)&descriptor;
      bytesRead = fread(desc+offset, 1, sizeof(descriptor)-offset, file);
      if (bytesRead != sizeof(descriptor)-offset) {
        break; /* EOF */
      }
    }
  }

  fclose(file);

  if (!duplicatesFound) {
    std::cout << "No duplicate files were found." << std::endl;
    return std::vector<encrypted_file>();
  }

  std::cout << "Duplicate files were found:" << std::endl;

  std::list<std::vector<encrypted_file> > sorted;
  std::map<std::string,std::vector<encrypted_file> >::const_iterator i = files.begin();
  for ( ; i != files.end(); ++i) {
    const std::vector<encrypted_file>& entry = i->second;
    if (entry.size() <= 1) {
      continue;
    }
    std::cout << "  " << entry.size() << " files (" << std::dec << entry[0].original_size << " bytes with checksum 0x" << std::hex << entry[0].crc32 << std::endl;
    if (sorted.size() && sorted.front().size() < entry.size()) {
      sorted.push_front(entry);
    } else {
      sorted.push_back(entry);
    }
  }

  return sorted.front();
}
