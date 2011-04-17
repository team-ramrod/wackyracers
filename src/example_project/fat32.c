#include "fat32.h"

//local parameters
//Structure to access Master Boot Record for getting info about partitions
struct mbr_info
{
 unsigned char nothing[446];		//ignore, placed here to fill the gap in the structure
 unsigned char partition_data[64];	//partition records (16x4)
 unsigned int signature;		//0xaa55
};

//Structure to access info of the first partition of the disk 
struct partition_info
{
 unsigned char status;				//0x80 - active partition
 unsigned char head_start;			//starting head
 unsigned int cyl_sect_start;		//starting cylinder and sector
 unsigned char type;				//partition type 
 unsigned char head_end;			//ending head of the partition
 unsigned int cyl_sect_end;			//ending cylinder and sector
 unsigned long first_sector;		//total sectors between MBR & the first sector of the partition
 unsigned long sectors_total;		//size of this partition in sectors
};
//Structure to access boot sector data
struct bs
{
 unsigned char jump_boot[3]; //default: 0x009000EB
 unsigned char oem_name[8];
 unsigned int bytes_per_sector; //deafault: 512
 unsigned char sector_per_cluster;
 unsigned int reserved_sector_count;
 unsigned char number_of_fats;
 unsigned int root_entry_count;
 unsigned int total_sectors_f16; //must be 0 for FAT32
 unsigned char media_type;
 unsigned int fat_size_f16; //must be 0 for FAT32
 unsigned int sectors_per_track;
 unsigned int number_of_heads;
 unsigned long hidden_sectors;
 unsigned long total_sectors_f32;
 unsigned long fat_size_f32; //count of sectors occupied by one FAT
 unsigned int ext_flags;
 unsigned int fs_version; //0x0000 (defines version 0.0)
 unsigned long root_cluster; //first cluster of root directory (=2)
 unsigned int fs_info; //sector number of FSinfo structure (=1)
 unsigned int backup_boot_sector;
 unsigned char reserved[12];
 unsigned char drive_number;
 unsigned char reserved1;
 unsigned char boot_signature;
 unsigned long volume_id;
 unsigned char volume_label[11]; //"NO NAME "
 unsigned char file_system_type[8]; //"FAT32"
 unsigned char boot_data[420];
 unsigned int boot_end_signature; //0xaa55
};
//Structure to access FSinfo sector data
struct fs_info
{
 unsigned long lead_signature; //0x41615252
 unsigned char reserved1[480];
 unsigned long structure_signature; //0x61417272
 unsigned long free_cluster_count; //initial: 0xffffffff
 unsigned long next_free_cluster; //initial: 0xffffffff
 unsigned char reserved2[12];
 unsigned long trail_signature; //0xaa550000
};
//Structure to access Directory Entry in the FAT
struct directory
{
 unsigned char name[11];
 unsigned char attrib; //file attributes
 unsigned char nt_reserved; //always 0
 unsigned char time_tenth; //tenths of seconds, set to 0 here
 unsigned int create_time; //time file was created
 unsigned int create_date; //date file was created
 unsigned int last_access_date;
 unsigned int first_cluster_hi; //higher word of the first cluster number
 unsigned int write_time; //time of last write
 unsigned int write_date; //date of last write
 unsigned int first_cluster_lo; //lower word of the first cluster number
 unsigned long file_size; //size of file in bytes
};

uint8_t fat32;

void fat32_init(void){
    fat32 = attach_interrupt(sd_write_interrupt);
}

//***************************************************************************
//Function: to read data from boot sector of SD card, to determine important
//parameters like bytesPerSector, sectorsPerCluster etc.
//Arguments: none
//return: none
//***************************************************************************
unsigned char get_boot_sector_data(void)
{
 struct bs *bpb; //mapping the buffer onto the structure
 struct mbr_info *mbr;
 struct partition_info *partition;
 unsigned long data_sectors;
 unsigned char error;
 
 file.unused_sectors = 0;
 error= sd_read_single_block(0);
 
 bpb= (struct bs *) file.buffer_fat32;
 
 if (bpb->jump_boot[0]!= 0xe9 && bpb->jump_boot[0]!= 0xeb)   //check if it is boot sector
 {
  mbr= (struct mbr_info *) file.buffer_fat32;       //if it is not boot sector, it must be MBR
  
  if (mbr->signature!= 0xaa55)
   return 1;       //if it is not even MBR then it's not FAT32
  
  partition= (struct partition_info *)(mbr->partition_data);//first partition
  file.unused_sectors = partition->first_sector; //the unused sectors, hidden to the FAT
  
  sd_read_single_block(partition->first_sector);//read the bpb sector
  bpb = (struct bs *) file.buffer_fat32;
  if (bpb->jump_boot[0]!=0xe9 && bpb->jump_boot[0]!=0xeb)
   return 1;
 }
 
 file.bytes_per_sector= bpb->bytes_per_sector;
 file.sector_per_cluster= bpb->sector_per_cluster;
 file.reserved_sector_count = bpb->reserved_sector_count;
 file.root_cluster = bpb->root_cluster;// + (sector / sectorPerCluster) +1;
 file.first_data_sector= bpb->hidden_sectors + file.reserved_sector_count + (bpb->number_of_fats * bpb->fat_size_f32);
 
 data_sectors= bpb->total_sectors_f32 - bpb->reserved_sector_count - ( bpb->number_of_fats * bpb->fat_size_f32);
 file.total_clusters= data_sectors / file.sector_per_cluster;
 
 if ((get_set_free_cluster(fat32_total_free, fat32_get, 0)) > file.total_clusters)  //check if FSinfo free clusters count is valid
  free_cluster_count_updated= 0;
 else
  free_cluster_count_updated= 1;
 
 return 0;
}

//***************************************************************************
//Function: to calculate first sector address of any given cluster
//Arguments: cluster number for which first sector is to be found
//return: first sector address
//***************************************************************************
unsigned long get_first_sector(unsigned long cluster_number)
{
 return (((cluster_number - 2) * file.sector_per_cluster) + file.first_data_sector);
}

//***************************************************************************
//Function: get cluster entry value from FAT to find out the next cluster in the chain
//or set new cluster entry in FAT
//Arguments: 1. current cluster number, 2. get_set (=GET, if next cluster is to be found or = SET,
//if next cluster is to be set 3. next cluster number, if argument#2 = SET, else 0
//return: next cluster number, if if argument#2 = GET, else 0
//****************************************************************************
unsigned long get_set_next_cluster(unsigned long cluster_number, unsigned char get_set, unsigned long cluster_entry)
{
 unsigned int fat_entry_offset;
 unsigned long *fat_entry_value;
 unsigned long fat_entry_sector;
 unsigned char retry = 0;
 
 //get sector number of the cluster entry in the FAT
 fat_entry_sector= file.unused_sectors + file.reserved_sector_count + ((cluster_number * 4) / file.bytes_per_sector) ;
 
 //get the offset address in that sector number
 fat_entry_offset= (unsigned int) ((cluster_number * 4) % file.bytes_per_sector);
 
 //read the sector into a buffer
 while (retry< 10)
 {
  if (!sd_read_single_block(fat_entry_sector)) 
   break; 
  retry++;
 }
 
 //get the cluster address from the buffer
 fat_entry_value= (unsigned long *) &file.buffer_fat32[fat_entry_offset];
 
 if (get_set== fat32_get)
  return ((*fat_entry_value) & 0x0fffffff);
 
 *fat_entry_value= cluster_entry;   //for setting new value in cluster entry in FAT
 
 sd_write_single_block(fat_entry_sector);
 
 return 0;
}

//********************************************************************************************
//Function: to get or set next free cluster or total free clusters in FSinfo sector of SD card
//Arguments: 1.flag:TOTAL_FREE or NEXT_FREE, 
//			 2.flag: GET or SET 
//			 3.new FS entry, when argument2 is SET; or 0, when argument2 is GET
//return: next free cluster, if arg1 is NEXT_FREE & arg2 is GET
//        total number of free clusters, if arg1 is TOTAL_FREE & arg2 is GET
//		  0xffffffff, if any error or if arg2 is SET
//********************************************************************************************
unsigned long get_set_free_cluster(unsigned char tot_or_next, unsigned char get_set, unsigned long fs_entry)
{
 struct fs_info *fs = (struct fs_info *) &file.buffer_fat32;
 unsigned char error; 
 
 sd_read_single_block(file.unused_sectors + 1);
 
 if((fs->lead_signature != 0x41615252) || (fs->structure_signature != 0x61417272) || (fs->trail_signature !=0xaa550000))
  return 0xffffffff;
 
 if (get_set== fat32_get)
 {
  if (tot_or_next== fat32_total_free)
   return (fs->free_cluster_count);
  else // when totOrNext = NEXT_FREE
   return (fs->next_free_cluster);
 }
 else
 {
  if (tot_or_next== fat32_total_free)
   fs->free_cluster_count= fs_entry;
  else // when totOrNext = NEXT_FREE
   fs->next_free_cluster= fs_entry;
  
  error= sd_write_single_block(file.unused_sectors + 1);	//update FSinfo
 }
 
 return 0xffffffff;
}

//***************************************************************************
//Function: to get DIR/FILE list or a single file address (cluster number) or to delete a specified file
//Arguments: #1 - flag: GET_LIST, GET_FILE or DELETE #2 - pointer to file name (0 if arg#1 is GET_LIST)
//return: first cluster of the file, if flag = GET_FILE
//        print file/dir list of the root directory, if flag = GET_LIST
//		  Delete the file mentioned in arg#2, if flag = DELETE
//****************************************************************************
struct directory* find_files(unsigned char flag, unsigned char *filename)
{
 unsigned long cluster, sector, first_sector, first_cluster, next_cluster;
 struct directory *dir;
 unsigned int i;
 unsigned char j;
 
 cluster= file.root_cluster; //root cluster
 sd.delay= 0;
 
 while (sd.ok)
 {
  first_sector= get_first_sector(cluster);
  
  for (sector= 0; sector< file.sector_per_cluster; sector++)
  {
   sd_read_single_block(first_sector + sector);
   for (i= 0; i< file.bytes_per_sector; i+= 32)
   {
    dir= (struct directory *) &file.buffer_fat32[i];
	
	if (dir->name[0]== fat32_empty) //indicates end of the file list of the directory
	{
	 if ((flag== fat32_get_file) || (flag== fat32_delete))// file does not exits
	 return 0;
	}
	
	if ((dir->name[0]!= fat32_deleted) && (dir->attrib != attr_long_name))
	{
	 if ((flag== fat32_get_file) || (flag== fat32_deleted))
	 {
	   for (j= 0; j< 11; j++)
	    if (dir->name[j]!= filename[j]) 
		 break;
	   if (j== 11)
	   {
	    if (flag== fat32_get_file)
		{
		 file.append_file_sector= first_sector + sector;
		 file.append_file_location= i;
		 file.append_start_cluster= (((unsigned long) dir->first_cluster_hi) << 16) | dir->first_cluster_lo;
		 file.file_size= dir->file_size;
		 return (dir);
		}
		else    //when flag= DELETE
		{
		 first_cluster= (((unsigned long) dir->first_cluster_hi) << 16) | dir->first_cluster_lo;
		 
		 //mark file as 'deleted' in FAT table
		 dir->name[0]= fat32_deleted;
		 sd_write_single_block(first_sector+sector);
		 free_memory_update(fat32_add, dir->file_size);
		 //update next free cluster entry in FSinfo sector
		 cluster= get_set_free_cluster(fat32_next_free, fat32_get, 0);
		 if (first_cluster< cluster)
		  get_set_free_cluster(fat32_next_free, fat32_set, first_cluster);
		 
		 //mark all the clusters allocated to the file as 'free'
		 sd.delay= 0;
		 while (sd.ok)
		 {
		  next_cluster= get_set_next_cluster(first_cluster, fat32_get, 0);
		  get_set_next_cluster(first_cluster, fat32_set, 0);
		  if (next_cluster> 0x0ffffff6)
		   return 0;
		  first_cluster= next_cluster;
		 }
		}
	   }
	  }
	  else  //when flag= GET_LIST
	  {
	   for (j= 0; j< 11; j++)
	   {
	    if (j== 8);
	   }
	   
	   if ((dir->attrib != 0x10) && (dir->attrib != 0x08))
		display_memory(fat32_low, dir->file_size);
	  }
	 }
	}
   }
   
   cluster= (get_set_next_cluster(cluster, fat32_get, 0));
   if (cluster> 0x0ffffff6)
    return 0;
   if (cluster== 0)
	return 0;
  }
 
 return 0;
}

//***************************************************************************
//Function: if flag=READ then to read file from SD card and send contents to UART 
//if flag=VERIFY then functions will verify whether a specified file is already existing
//Arguments: flag (READ or VERIFY) and pointer to the file name
//return: 0, if normal operation or flag is READ
//	      1, if file is already existing and flag= VERIFY
//		  2, if file name is incompatible
//***************************************************************************
unsigned char read_file(unsigned char flag, unsigned char *filename)
{
 struct directory *dir;
 unsigned long cluster, byte_counter= 0, file_size, first_sector;
 unsigned int k;
 unsigned char j, error;
 
 error= convert_filename(filename); //convert filename into FAT format
 if (error)
  return 2;
 
 dir= find_files(fat32_get_file, filename); //get the file location
 if (dir== 0)
  return 0;
 
 if (flag== fat32_verify) 
  return 1;	//specified file name is already existing
 
 cluster= (((unsigned long) dir->first_cluster_hi) << 16) | dir->first_cluster_lo;
 
 file_size= dir->file_size;
 
 sd.delay= 0;
 while (sd.ok)
 {
  first_sector= get_first_sector(cluster);
  for (j= 0; j< file.sector_per_cluster; j++)
  {
   sd_read_single_block(first_sector + j);
   
   for (k= 0; k< 512; k++)
   {
    if ((byte_counter++)>= file_size )
	 return 0;
   }
  }
  
  cluster= get_set_next_cluster(cluster, fat32_get, 0);
  if (cluster== 0)
   return 0;
 }
 
 return 0;
}

//***************************************************************************
//Function: to convert normal short file name into FAT format
//Arguments: pointer to the file name
//return: 0, if successful else 1.
//***************************************************************************
unsigned char convert_filename(unsigned char *filename)
{
 unsigned char filename_fat[11];
 unsigned char j, k;
 char local_str[21];
 
 for (j= 0; j< 12; j++)
  if (filename[j]== '.') break;
 
 for (k= 0; k< j; k++) //setting file name
  filename_fat[k]= filename[k];
 
 for (k= j; k<= 7; k++) //filling file name trail with blanks
  filename_fat[k]= ' ';
 
 j++;
 for (k= 8; k< 11; k++) //setting file extention
 {
  if (filename[j]!= 0)
   filename_fat[k]= filename[j++];
  else //filling extension trail with blanks
   while (k< 11)
    filename_fat[k++]= ' ';
 }
 
 for (j= 0; j< 11; j++) //converting small letters to caps
  if ((filename_fat[j]>= 0x61) && (filename_fat[j]<= 0x7a))
   filename_fat[j]-= 0x20;
 
 for (j= 0; j< 11; j++)
  filename[j]= filename_fat[j];
 
 return 0;
}

//************************************************************************************
//Function: to create a file in FAT32 format in the root directory if given 
//			file name does not exist; if the file already exists then append the data
//Arguments: pointer to the file name
//return: none
//************************************************************************************
void sd_write_interrupt(void)
{
 unsigned char j, data, error, file_created_flag= 0, sector_end_flag= 0, sector= 0, append_file= 0, append_start= 0;
 unsigned int k, i, first_cluster_high= 0, first_cluster_low= 0, date_fat32, time_fat32;  //value 0 is assigned just to avoid warning in compilation
 struct directory *dir;
 unsigned long cluster, next_cluster, prev_cluster, first_sector, cluster_count, extra_memory;
 char filename[11];
 
 sprintf(filename, "%02d%02d%02d%02d.txt", date_time.year, date_time.month, date_time.day, date_time.hour);
 
 date_fat32= date_time.day + (date_time.month<<5) + ((date_time.year+20)<<9);
 time_fat32= (date_time.second>>1) + (date_time.minute<<5) + (date_time.hour<<11);
 
 switch (read_file(fat32_verify, filename))
 {
  case 1 :
   append_file= true;
   cluster_count= file.file_size / (file.sector_per_cluster * file.bytes_per_sector);
   cluster= cluster_count + file.append_start_cluster;
   sector= (file.file_size - (cluster_count * file.sector_per_cluster * file.bytes_per_sector)) / file.bytes_per_sector; //last sector number of the last cluster of the file
   append_start= true;
//   uart1_write("!1\r");
   break;
  
  case 2 :
   interrupts[fat32].flag = bit_false;
//   uart1_write("!2\r");
   sd.ok= bit_false; //fault in sd card
   //uart0_write("invalid filename\r");
   return; //invalid file name
   break;
  
  case 0 :
   cluster= get_set_free_cluster(fat32_next_free, fat32_get, 0);
   if(cluster> file.total_clusters)
    cluster= file.root_cluster;
   
   cluster= search_next_free_cluster(cluster);
   
   //no free clusters
   if (cluster== 0)
   {
    interrupts[fat32].flag = bit_false;
	sd.ok= bit_false; //fault in sd card
//	uart1_write("!3\r");
   //uart0_write("no free clusters1\r");
    return;
   }
   
   get_set_next_cluster(cluster, fat32_set, fat32_eof);   //last cluster of the file, marked EOF
   
   first_cluster_high = (unsigned int) ((cluster & 0xffff0000) >> 16 );
   first_cluster_low = (unsigned int) ( cluster & 0x0000ffff);
   file.file_size= 0;
//   uart1_write("!4\r");
   break;
 }
 
 k= 0;
 while (true)
 {
  if (append_start)
  {
   append_start= false;
   file.start_block= get_first_sector(cluster) + sector;
   sd_read_single_block(file.start_block);
   i= file.file_size % file.bytes_per_sector;
   j= sector;
  }
  else
  {
   file.start_block= get_first_sector(cluster);
   i= 0;
   j= 0;
  }
  
  do
  {
   if (sector_end_flag== 1) //special case when the last character in previous sector was '\r'
   {
    file.buffer_fat32[i++]= '\n'; //appending 'Line Feed (LF)' character
	file.file_size++;
   }
   
   sector_end_flag= 0;
   
   data= file.tmp_buffer_fat32[k];
   k++;
   file.buffer_fat32[i++]= data;
   file.file_size++;
   if (data== '\r')  //'Carriage Return (CR)' character
   {
    if (i== 512)
	 sector_end_flag= 1;  //flag to indicate that the appended '\n' char should be put in the next sector
	else
	{
	 file.buffer_fat32[i++]= '\n'; //appending 'Line Feed (LF)' character
	 file.file_size++;
	}
   }
   
   if (i>= 512)   //though 'i' will never become greater than 512, it's kept here to avoid infinite loop in case it happens to be greater than 512 due to some data corruption
   {
    i= 0;
	error= sd_write_single_block(file.start_block);
	j++;
	if (j== file.sector_per_cluster) 
	{
	 j = 0; 
//	 uart1_write("!5\r");
	 break;
	}
	file.start_block++; 
   }
  } while (data!= '~');
  
  if (data== '~')
  {
   file.file_size--;	//to remove the last entered '~' character
   i--;
   for (; i< 512;i++)  //fill the rest of the buffer with 0x00
    file.buffer_fat32[i]= 0x00;
   error= sd_write_single_block(file.start_block);
//   uart1_write("!6\r");
   break;
  }
  
  prev_cluster= cluster;
  cluster= search_next_free_cluster(prev_cluster); //look for a free cluster starting from the current cluster
  
  //no free clusters
  if (cluster== 0)
  {
   interrupts[fat32].flag = bit_false;
//   uart1_write("!7\r");
   sd.ok= bit_false; //fault in sd card
   //uart0_write("no free clusters2\r");
   return;
  }
  
  get_set_next_cluster(prev_cluster, fat32_set, cluster);
  get_set_next_cluster(cluster, fat32_set, fat32_eof);   //last cluster of the file, marked EOF
 }
 
 get_set_free_cluster(fat32_next_free, fat32_set, cluster); //update FSinfo next free cluster entry
 
 if (append_file)  //executes this loop if file is to be appended
 {
  sd_read_single_block(file.append_file_sector);
  dir= (struct directory *) &file.buffer_fat32[file.append_file_location];
  
  dir->last_access_date= 0;   //date of last access ignored
  dir->write_time= time_fat32;  //setting new time of last write, obtained from RTC
  dir->write_date= date_fat32;  //setting new date of last write, obtained from RTC
  extra_memory= file.file_size - dir->file_size;
  dir->file_size= file.file_size;
  sd_write_single_block(file.append_file_sector);
  free_memory_update(fat32_remove, extra_memory); //updating free memory count in FSinfo sector;
  
  interrupts[fat32].flag = bit_false;
//  uart1_write("!8\r");
   //uart0_write("sd written\r");
  return;
 }
 
 //executes following portion when new file is created
 prev_cluster= file.root_cluster; //root cluster
 
 while (true)
 {
  first_sector= get_first_sector(prev_cluster);
  
  for (sector= 0; sector< file.sector_per_cluster; sector++)
  {
   sd_read_single_block(first_sector + sector);
   
   for (i= 0; i< file.bytes_per_sector; i+= 32)
   {
    dir = (struct directory *) &file.buffer_fat32[i];
	
	if (file_created_flag)   //to mark last directory entry with 0x00 (empty) mark indicating end of the directory file list
	{
	 interrupts[fat32].flag = bit_false;
//	 uart1_write("!9\r");
    //uart0_write("sd written\r");
	 return;
	}
	
	if ((dir->name[0]== fat32_empty) || (dir->name[0]== fat32_deleted))  //looking for an empty slot to enter file info
	{
	 for (j= 0; j< 11; j++)
	  dir->name[j]= filename[j];
	 dir->attrib= attr_archive;	//settting file attribute as 'archive'
	 dir->nt_reserved= 0;			//always set to 0
	 dir->time_tenth= 0;			//always set to 0
	 dir->create_time= time_fat32; 	//setting time of file creation, obtained from RTC
	 dir->create_date= date_fat32; 	//setting date of file creation, obtained from RTC
	 dir->last_access_date= date_fat32;   	//date of last access ignored
	 dir->write_time= time_fat32;  	//setting new time of last write, obtained from RTC
	 dir->write_date= date_fat32;  	//setting new date of last write, obtained from RTC
	 dir->first_cluster_hi= first_cluster_high;
	 dir->first_cluster_lo= first_cluster_low;
	 dir->file_size= file.file_size;
	 
	 sd_write_single_block(first_sector + sector);
	 file_created_flag= 1;
	 free_memory_update(fat32_remove, file.file_size); //updating free memory count in FSinfo sector
	}
   }
  }
  
  cluster= get_set_next_cluster(prev_cluster, fat32_get, 0);
  
  if (cluster> 0x0ffffff6)
  {
   if (cluster== fat32_eof)   //this situation will come when total files in root is multiple of (32*sectorPerCluster)
   {
    cluster= search_next_free_cluster(prev_cluster); //find next cluster for root directory entries
	get_set_next_cluster(prev_cluster, fat32_set, cluster); //link the new cluster of root to the previous cluster
	get_set_next_cluster(cluster, fat32_set, fat32_eof);  //set the new cluster as end of the root directory
   }
   else
   {
    interrupts[fat32].flag = bit_false;
//	uart1_write("!10\r");
    //uart0_write("sd written\r");
    return;
   }
  }
  
  if (cluster== 0)
  {
   interrupts[fat32].flag = bit_false; 
    //uart0_write("sd written\r");
   return;
  }
  
  prev_cluster= cluster;
 }
 
 interrupts[fat32].flag = bit_false;
// uart1_write("!11\r");
    //uart0_write("sd written\r");
 return;
}

//***************************************************************************
//Function: to search for the next free cluster in the root directory
//          starting from a specified cluster
//Arguments: Starting cluster
//return: the next free cluster
//****************************************************************
unsigned long search_next_free_cluster(unsigned long start_cluster)
{
 unsigned long cluster, *value, sector;
 unsigned char i;
 
 start_cluster-= (start_cluster % 128);   //to start with the first file in a FAT sector
 
 for (cluster= start_cluster; cluster< file.total_clusters; cluster+= 128)
 {
  sector= file.unused_sectors + file.reserved_sector_count + ((cluster * 4) / file.bytes_per_sector);
  sd_read_single_block(sector);
  for (i= 0; i< 128; i++)
  {
   value= (unsigned long *) &file.buffer_fat32[i*4];
   if(((*value) & 0x0fffffff)== 0)
    return (cluster+i);
  }  
 }
 
 return 0;
}

//***************************************************************************
//Function: to display total memory and free memory of SD card, using UART
//Arguments: none
//return: none
//Note: this routine can take upto 15sec for 1GB card (@1MHz clock)
//it tries to read from SD whether a free cluster count is stored, if it is stored
//then it will return immediately. Otherwise it will count the total number of
//free clusters, which takes time
//****************************************************************************
void memory_statistics(void)
{
 unsigned long free_clusters, total_cluster_count, cluster;
 unsigned long total_memory, free_memory;
 unsigned long sector, *value;
 unsigned int i;   
 
 total_memory= file.total_clusters * file.sector_per_cluster / 1024;
 total_memory*= file.bytes_per_sector;
 
 display_memory(fat32_high, total_memory);
 
 free_clusters= get_set_free_cluster(fat32_total_free, fat32_get, 0);
 //freeClusters= 0xffffffff;
 
 if (free_clusters> file.total_clusters)
 {
  free_cluster_count_updated= 0;
  free_clusters= 0;
  total_cluster_count= 0;
  cluster= file.root_cluster;
  sd.delay= 0;
  while (sd.ok)
  {
   sector= file.unused_sectors + file.reserved_sector_count + ((cluster * 4) / file.bytes_per_sector);
   sd_read_single_block(sector);
   for (i= 0; i< 128; i++)
   {
    value= (unsigned long *) &file.buffer_fat32[i*4];
	if (((*value)& 0x0fffffff)== 0)
	 free_clusters++;
	
	total_cluster_count++;
	if (total_cluster_count== (file.total_clusters+2)) 
	 break;
   }
   
   if (i< 128) 
    break;
   
   cluster+= 128;
  }
 }
 
 if (!free_cluster_count_updated)
  get_set_free_cluster (fat32_total_free, fat32_set, free_clusters); //update FSinfo next free cluster entry
 
 free_cluster_count_updated= 1;  //set flag
 free_memory= free_clusters * file.sector_per_cluster / 1024;
 free_memory*= file.bytes_per_sector ;
 
 display_memory(fat32_high, free_memory); 
}

//************************************************************
//Function: To convert the unsigned long value of memory into 
//          text string and send to UART
//Arguments: 1. unsigned char flag. If flag is HIGH, memory will be displayed in KBytes, else in Bytes. 
//			 2. unsigned long memory value
//return: none
//************************************************************
void display_memory(unsigned char flag, unsigned long memory)
{
 unsigned char i;
 
 for (i= 12; i> 0; i--) //converting freeMemory into ASCII string
 {
  if (i==5 || i==9)
  {
   file.memory_string[i-1]= ',';
   i--;
  }
  
  file.memory_string[i-1]= (memory % 10) | 0x30;
  memory /= 10;
  
  if (memory== 0) 
   break;
 }
 
 if (flag== fat32_high) 
  file.memory_string[13]= 'K';
}

//********************************************************************
//Function: to delete a specified file from the root directory
//Arguments: pointer to the file name
//return: none
//********************************************************************
void delete_file(unsigned char *filename)
{
 unsigned char error;
 
 error= convert_filename(filename);
 if (error) 
  return;
 
 find_files(fat32_delete, filename);
}

//********************************************************************
//Function: update the free memory count in the FSinfo sector. 
//			Whenever a file is deleted or created, this function will be called
//			to ADD or REMOVE clusters occupied by the file
//Arguments: #1.flag ADD or REMOVE #2.file size in Bytes
//return: none
//********************************************************************
void free_memory_update(unsigned char flag, unsigned long size)
{
 unsigned long free_clusters;  
 
 //convert file size into number of clusters occupied
 if ((size % 512)== 0) 
  size= size / 512;
 else 
  size= (size / 512) +1;
 
 if((size % 8)== 0) 
  size= size / 8;
 else 
  size= (size / 8) +1;
 
 if (free_cluster_count_updated)
 {
  free_clusters= get_set_free_cluster(fat32_total_free, fat32_get, 0);
  if (flag== fat32_add)
   free_clusters= free_clusters + size;
  else  //when flag= REMOVE
   free_clusters= free_clusters - size;
  
  get_set_free_cluster(fat32_total_free, fat32_set, free_clusters);
 }
}

void save_to_sd(void)
{
 char str[max_str_length];
 unsigned char j;
 static unsigned int i;
 static char save_str[max_str_length];
 
 if ((interrupts[fat32].flag == bit_false) && 
     (dataflash.flags.recording == bit_false) && (dataflash.flags.erase== bit_false))
 {
  if (sd.delay++> 200)	   	//test if the sd_card is unplugged
  {
   sd_init();
   sd.delay= 0;
  }
  
  if (sd.ok)
  {
   sprintf(str, "%s\r%02d-%02d-%02d,%02d:%02d:%02d.%d,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%1.2f,%d,%d",
    save_str, date_time.day, date_time.month, date_time.year+2000,
    date_time.hour, date_time.minute, date_time.second, date_time.tenth_second/2,
    parameters.acceleration_y, parameters.acceleration_x, parameters.acceleration_y,
    parameters.gyro_x, parameters.gyro_y,
    parameters.compass_x, parameters.compass_y, parameters.compass_z,
    parameters.battery_voltage, parameters.generator_voltage_1, parameters.generator_voltage_2,
    parameters.load_current_1, parameters.load_current_2,
    parameters.water_pressure, parameters.hydraulic_pressure_1, parameters.hydraulic_pressure_2,
    parameters.arm_angle,

    control.close_1, control.close_2);
   save_str[0]= 0x00;
   
   for (j= 0; j<= strlen(str); j++, i++)
   {
    if (i< 511)
     file.tmp_buffer_fat32[i]= str[j];
    else
    {
     i= 0;
     for (; j<= strlen(str); j++, i++)
	  save_str[i]= str[j];
     file.tmp_buffer_fat32[511]= '~';
     i= 0;
	 interrupts[fat32].flag = bit_true;
	 break;
    }
   }
  }
 }
}
