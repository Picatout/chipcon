/*
 * filesys.h
 *
 * Created: 2014-10-11 21:09:04
 *  Author: Jacques
 */ 


#ifndef FILESYS_H_
#define FILESYS_H_

#include "hardware.h"

// sdcard and FAT library
#include "fat/fat.h"
#include "fat/partition.h"
#include "fat/sd_raw.h"



uint8_t fs_mount();
void fs_umount();

uint8_t  fs_open_dir(char *dir_name);
void fs_close_dir();

uint8_t fs_open_file(char *file_name);
void fs_close_file();
uint8_t fs_load_file(uint16_t file_no);
uint8_t fs_read_dir(struct fat_dir_entry_struct *dir_entry);


#endif /* FILESYS_H_ */