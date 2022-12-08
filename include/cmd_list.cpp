#include <iostream>

void first_cmd(){
    u_int8_t arr[8] = { 0x92, 0, 0, 0, 0x78, 0, 0 ,0};
    int64_t cmd = arr[7]||arr[6]<<8||arr[5]<<16||arr[4]<<24||arr[3]<<32||arr[2]<<40||arr[1]<<48||arr[0]<<56;
    std::cout << cmd << std::endl;
}