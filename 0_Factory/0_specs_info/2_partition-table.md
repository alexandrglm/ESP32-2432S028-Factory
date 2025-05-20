## Extracted Partition Table

#### 

```csv
# ESP-IDF Partition Table
# Name, Type, SubType, Offset, Size, Flags

nvs,data,nvs,0x9000,20K,
otadata,data,ota,0xe000,8K,
app0,app,ota_0,0x10000,1280K,
app1,app,ota_1,0x150000,1280K,
spiffs,data,spiffs,0x290000,1472K,

```

| Name           | Type     | Offset     | Size               |
| -------------- | -------- | ---------- | ------------------ |
| nvs            | Data     | 0x09000    | 20 KB              |
| otadata        | Data     | 0x0E000    | 8 KB               |
| app0           | App      | 0x10000    | 1280 KB            |
| app1           | App      | 0x150000   | 1280 KB            |
| spiffs         | Data     | 0x290000   | 1472 KB            |
| SD Card mapped | "Broken" | 0xFFFFFFFF | 4194303 KB (~4 GB) |

## About SD CARD mapped memory directions

- **Offset 0xFFFFFFFF** indicates an invalid or unassigned flash address.

- **Type/SubType 0xEB/0xEB** are non-standard values in ESP-IDF partition tables, indicating this is not a real partition.

- **Size 4194303 KB (~4 GB)** seems to be the maximum size of the microSD card supported by this board.
  
  


