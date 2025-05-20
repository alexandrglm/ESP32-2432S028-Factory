

# `APP0.bin` output analysis



```bash
esptool.py image_info APP0.bin


dd if=part_app0.bin of=segment1.bin bs=1 skip=$((0x18)) count=$((0x2b830))
dd if=part_app0.bin of=segment2.bin bs=1 skip=$((0x2b850)) count=$((0x2518))
dd if=part_app0.bin of=segment3.bin bs=1 skip=$((0x2dd70)) count=$((0x400))
dd if=part_app0.bin of=segment4.bin bs=1 skip=$((0x2e178)) count=$((0x1e90))
dd if=part_app0.bin of=segment5.bin bs=1 skip=$((0x30010)) count=$((0x480c8))
dd if=part_app0.bin of=segment6.bin bs=1 skip=$((0x780e0)) count=$((0x8f70))

```



| Segment | Size (hex) | Size (dec) | Mem. Direction | Offset     | Tyoe            |
| ------- | ---------- | ---------- | -------------- | ---------- | --------------- |
| 1       | 0x2b830    | 178,096    | 0x3f400020     | 0x00000018 | DROM (data ROM) |
| 2       | 0x02518    | 9,144      | 0x3ffbdb60     | 0x0002b850 | DRAM (data)     |
| 3       | 0x00400    | 1,024      | 0x40080000     | 0x0002dd70 | IRAM (code)     |
| 4       | 0x01e90    | 7,280      | 0x40080400     | 0x0002e178 | IRAM (cod)      |
| 5       | 0x480c8    | 294,536    | 0x400d0018     | 0x00030010 | IROM (code)     |
| 6       | 0x08f70    | 36,272     | 0x40082290     | 0x000780e0 | IRAM (code)     |

```bash

```
